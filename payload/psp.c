#include <lv2/lv2.h>
#include <lv2/error.h>
#include <lv2/io.h>
#include <lv2/memory.h>
#include <lv2/modules.h>
#include <lv2/object.h>
#include <lv2/thread.h>
#include <lv2/synchronization.h>
#include <lv2/time.h>
#include <lv2/libc.h>
#include <lv2/patch.h>
#include "common.h"
#include "psp.h"
#include "modulespatch.h"


uint32_t psp_tag;
uint8_t psp_keys[16];
uint8_t psp_code;
char pspemu_path[36];
char psptrans_path[37];

#define SPRX_NUM			90
#define NUM_SCE_PSP_MODULES	5

static char *sce_psp_modules[NUM_SCE_PSP_MODULES] =
{
	"sceFont_Library",
	"scePsmfP_library",
	"sceHeap_Library",
	"sceMpeg_library",
	"sceATRAC3plus_Library",
};

char *umd_file;
static mutex_t mutex;
static int user_mutex;
static int umd_fd=-1;
static char psp_id[11];
static uint64_t pemucorelib_base;
static uint64_t emulator_api_base;
static uint32_t *savedata_param;

static INLINE uint32_t swap32(uint32_t data)
{
	uint32_t ret = (((data) & 0xff) << 24);
	ret |= (((data) & 0xff00) << 8);
	ret |= (((data) & 0xff0000) >> 8);
	ret |= (((data) >> 24) & 0xff);
	
	return ret;
}

static INLINE uint16_t swap16(uint16_t data)
{
	uint32_t ret = (data<<8)&0xFF00;
	ret |= ((data>>8)&0xFF);
	
	return ret;
}

static INLINE __attribute__((unused)) int is_sce_psp_module(char *name)
{
	for (int i = 0; i < NUM_SCE_PSP_MODULES; i++)
	{
		if (strcmp(sce_psp_modules[i], name) == 0)
			return 1;
	}
	
	return 0;
}

int sys_psp_read_header(int fd, char *buf, uint64_t nbytes, uint64_t *nread)
{
	int ret;
	uint32_t n, unk2;
	uint64_t umd_size;
	sys_prx_id_t *list;
	uint32_t *unk;
	process_t process;
	
	#ifdef DEBUG
		//DPRINTF("umd read header: %p %lx\n", buf, nbytes);
	#endif
	
	buf = get_secure_user_ptr(buf);
	nread = get_secure_user_ptr(nread);
		
	if (!umd_file || nbytes != 0x100)
		return EABORT;
	
	pemucorelib_base = 0;
	emulator_api_base = 0;
	list = alloc(SPRX_NUM*sizeof(sys_prx_module_info_t), 0x35);
	unk = alloc(SPRX_NUM*sizeof(uint32_t), 0x35);
	process = get_current_process();	
	
	ret = prx_get_module_list(process, list, unk, SPRX_NUM, &n, &unk2);
	if (ret == 0)
	{
		char *filename = alloc(256, 0x35);
		sys_prx_segment_info_t *segments = alloc(sizeof(sys_prx_segment_info_t), 0x35);
		
		for (int i = 0; i < n; i++)
		{
			sys_prx_module_info_t modinfo;
			
			memset(&modinfo, 0, sizeof(sys_prx_module_info_t));
			modinfo.filename_size = 256;
			modinfo.segments_num = 1;
			
			if (prx_get_module_info(process, list[i], &modinfo, filename, segments) == 0)
			{
				if (strstr(filename, "/emulator_api.sprx"))
				{
					emulator_api_base = segments[0].base;
					#ifdef DEBUG
						//DPRINTF("emulator_api base = %08lx\n", emulator_api_base);
					#endif	
				}
				else if (strstr(filename, "/PEmuCoreLib.sprx"))
				{
					pemucorelib_base = segments[0].base;						
					#ifdef DEBUG
						//DPRINTF("PEmuCoreLib base = %08lx\n", pemucorelib_base);
					#endif
				}
			}
		}
		
		if (pemucorelib_base == 0 || emulator_api_base == 0)
			ret = EABORT;
		
		dealloc(filename, 0x35);
		dealloc(segments, 0x35);
	}
	
	dealloc(list, 0x35);
	dealloc(unk, 0x35);
	
	if (ret != 0)
		return ret;
		
	ret = cellFsOpen(umd_file, CELL_FS_O_RDONLY, &umd_fd, 0, NULL, 0);
	if (ret != 0)
		return ret;
	
	cellFsLseek(umd_fd, 0, SEEK_END, &umd_size);

	// Fake header. We will write only values actually used
	memset(buf, 0, 0x100);
	*(uint32_t *)(buf+0x0c) = 0x10;
	*(uint32_t *)(buf+0x64) = (umd_size/0x800)-1; // Last sector of umd	
	strncpy(buf+0x70, psp_id, 10);
						
	#ifdef DEBUG
		//DPRINTF("ID: %s\n", psp_id);
	#endif
	
	if (mutex && user_mutex)
	{
		mutex = NULL;
		user_mutex = 0;
	}
	
	*nread = 0x100;
	return 0;
}

int sys_psp_read_umd(int unk, void *buf, uint64_t sector, uint64_t ofs, uint64_t size)
{
	uint64_t offset, dummy;
	int ret;
							
	#ifdef DEBUG
		//DPRINTF("umd read %lx %lx %lx\n", sector, ofs, size);
	#endif
		
	if (!mutex)
	{
		object_handle_t obj_handle;
		void *object_table = get_current_process()->object_table;
		
		int ret = open_kernel_object(object_table, *(uint32_t *)(emulator_api_base+umd_mutex_offset), (void **)&mutex, &obj_handle, SYS_MUTEX_OBJECT);
				
		if (ret != 0)
		{
			#ifdef DEBUG
				//DPRINTF("Cannot open user mutex, using an own one\n");
			#endif
			mutex_create(&mutex, SYNC_PRIORITY, SYNC_NOT_RECURSIVE);
			user_mutex = 0;
		}
		else
		{
			#ifdef DEBUG
				//DPRINTF("user mutex opened succesfully\n");
			#endif
			user_mutex = 1;
			close_kernel_object_handle(object_table, obj_handle);
		}
	}
	
	mutex_lock(mutex, 0);
	offset = sector*0x800;
	
	if (ofs != 0)	
		offset = offset+0x800-ofs;	
	
	ret = cellFsLseek(umd_fd, offset, SEEK_SET, &dummy);
	if (ret != 0)
	{
		mutex_unlock(mutex);
		return ret;
	}
	
	ret = cellFsRead(umd_fd, get_secure_user_ptr(buf), size, &dummy);
	mutex_unlock(mutex);
	
	if (ret == 0)
		ret = (int)size;
	
	return ret;
}

int sys_psp_set_umdfile(char *file, char *id, int prometheus)
{
	int ret;
	
	file = get_secure_user_ptr(file);
	id = get_secure_user_ptr(id);
	
	if (file == NULL)
	{
		if (umd_file)
		{
			dealloc(umd_file, 0x27);
			umd_file = NULL;			
		}
		if (mutex)
		{
			if (user_mutex)			
				user_mutex = 0;			
			else			
				mutex_destroy(mutex);			
				
			mutex = NULL;
		}
		
		condition_psp_iso = 0;
		condition_psp_dec = 0;
		condition_psp_keys = 0;	
		condition_psp_prometheus = 0;
		return 0;
	}
	
	strncpy(psp_id, id, 10);
	psp_id[10] = 0;
	
	if (strlen(psp_id) != 10)
		return EINVAL;
	
	ret = pathdup_from_user(file, &umd_file);
	if (ret != 0)
		return ret;
	
	condition_psp_iso = 1;
	condition_psp_prometheus = prometheus;
	
#ifdef DEBUG
	if (prometheus)	
		DPRINTF("Using prometheus patch.\n");	
#endif

	return 0;
}

int sys_psp_set_decrypt_options(int decrypt_patch, uint32_t tag, uint8_t *keys, uint8_t code, uint32_t tag2, uint8_t *keys2, uint8_t code2)
{
	if (!umd_file)
		return EABORT;
	
	keys = get_secure_user_ptr(keys);
	
	if (keys)
	{
		if (copy_from_user(keys, psp_keys, 0x10) == 0)
		{
			psp_tag = tag;
			psp_code = code;
			condition_psp_keys = 1;			
		}
	}
	
	condition_psp_dec = decrypt_patch;	
	return 0;
}

int sys_psp_prx_patch(uint32_t *unk, uint8_t *elf_buf, void *link_register)
{
	unk = get_secure_user_ptr(unk);
	elf_buf = get_secure_user_ptr(elf_buf);
	
#ifdef DEBUG
	
	//DPRINTF("link_register = %p\n", link_register);
	
	if (link_register == (void *)(pemucorelib_base+prx_patch_call_lr))
	{		
		if (*(uint32_t *)&elf_buf[0] == 0x7F454C46)
		{
			Elf32_Ehdr *ehdr;
			Elf32_Phdr *phdr;
			PspModuleInfo *modinfo;
		
			ehdr = (Elf32_Ehdr *)elf_buf;
			phdr = (Elf32_Phdr *)(elf_buf+swap32(ehdr->e_phoff));
			modinfo = (PspModuleInfo *)(elf_buf+swap32(phdr[0].p_paddr));
			DPRINTF("Module %s (elf_buf=%p)\n", modinfo->modname, elf_buf);			
		}		
	}
	
#endif
		
	psp_func1(unk, elf_buf);
	return 0;
}

int sys_psp_set_emu_path(char *path)
{
	if (!path)
	{
		condition_psp_change_emu = 0;
		return 0;
	}
	
	#ifdef DEBUG
		//DPRINTF("sys_psp_set_emu_path has been deleted\n");
	#endif
	
	/*DPRINTF("pspemu path set to %s\n", path);
	
	path = get_secure_user_ptr(path);
	condition_psp_change_emu = 1;
	
	snprintf(pspemu_path, sizeof(pspemu_path), "%s/psp_emulator.self", path);
	snprintf(psptrans_path, sizeof(psptrans_path), "%s/psp_translator.self", path);*/
	
	return 0;
}

int sys_psp_post_savedata_initstart(int result, void *param)
{
	#ifdef DEBUG
		//DPRINTF("Savedata init start\n");
	#endif
	
	if (result == 0)	
		savedata_param = get_secure_user_ptr(param);	
	
	return result;
}

int sys_psp_post_savedata_shutdownstart(void)
{
	#ifdef DEBUG
		//DPRINTF("Savedata shutdown start\n");
	#endif
	
	if (savedata_param)
	{
		#ifdef DEBUG
			//DPRINTF("Original bind: %08X\n", savedata_param[0x34/4]);
		#endif
		savedata_param[0x34/4] = swap32(1); // SCE_UTILITY_SAVEDATA_BIND_OK
		savedata_param = NULL;
	}
	
	return 0;
}

#ifdef DEBUG

/* mips_registers actually contains much more than mips registers 
Known values:

0460 -> PC or current block to process
0478 -> PC too?
04D0 -> PC too?
0090 -> text offset in elf
0098 -> text size
*/

int sys_psp_sony_bug(uint32_t *mips_registers, void *unk, uint32_t mips_PC)
{
	DPRINTF("sys_psp_sony_bug, game is gonna crash\n");
	/*
	DPRINTF("PSP registers info:\n");
	DPRINTF("PC: %08X\n", mips_PC);
	DPRINTF("zr: %08X   at: %08X\n", mips_registers[0], mips_registers[1]);
	DPRINTF("v0: %08X   v1: %08X\n", mips_registers[2], mips_registers[3]);
	DPRINTF("a0: %08X   a1: %08X\n", mips_registers[4], mips_registers[5]);
	DPRINTF("a2: %08X   a3: %08X\n", mips_registers[6], mips_registers[7]);
	DPRINTF("t0: %08X   t1: %08X\n", mips_registers[8], mips_registers[9]);
	DPRINTF("t2: %08X   t3: %08X\n", mips_registers[10], mips_registers[11]);
	DPRINTF("t4: %08X   t5: %08X\n", mips_registers[12], mips_registers[13]);
	DPRINTF("t6: %08X   t7: %08X\n", mips_registers[14], mips_registers[15]);
	DPRINTF("s0: %08X   s1: %08X\n", mips_registers[16], mips_registers[17]);
	DPRINTF("s2: %08X   s3: %08X\n", mips_registers[18], mips_registers[19]);
	DPRINTF("s4: %08X   s5: %08X\n", mips_registers[20], mips_registers[21]);
	DPRINTF("s6: %08X   s7: %08X\n", mips_registers[22], mips_registers[23]);
	DPRINTF("t8: %08X   t9: %08X\n", mips_registers[24], mips_registers[25]);
	DPRINTF("k0: %08X   k1: %08X\n", mips_registers[26], mips_registers[27]);
	DPRINTF("gp: %08X   sp: %08X\n", mips_registers[28], mips_registers[29]);
	DPRINTF("s8: %08X   ra: %08X\n", mips_registers[30], mips_registers[31]);
	*/
	
	/*for (int i = 32; i < 1024; i += 2)
	{
		DPRINTF("%04X: %08X %08X\n", i*4, mips_registers[i], mips_registers[i+1]);
	}*/
	
	return 0;
}

#endif

