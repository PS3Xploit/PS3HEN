#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lv2/lv2.h>
#include <lv2/libc.h>
#include <lv2/memory.h>
#include <lv2/patch.h>
#include <lv2/syscall.h>
#include <lv2/usb.h>
#include <lv2/storage.h>
#include <lv2/thread.h>
#include <lv2/synchronization.h>
#include <lv2/modules.h>
#include <lv2/io.h>
#include <lv2/time.h>
#include <lv2/security.h>
#include <lv2/pad.h>
#include <lv2/error.h>
#include <lv2/symbols.h>
#include <lv1/stor.h>
#include <lv1/patch.h>
#include <lv2/symbols.h>
#include "storage_ext.h"
#include "common.h"
#include "config.h"
#include "sm_ext.h"
#include "region.h"
#include "ps3mapi_core.h"

#define MAX_POKES 100

struct pokes {
	uint64_t addrs_backup;
	uint64_t value_backup;
	uint64_t value_pokes;
} POKES;

struct pokes *pokess=NULL;

void pre_map_process_memory(void *object, uint64_t process_addr, uint64_t size, uint64_t flags, void *unk, void *elf, uint64_t *out);


uint8_t cleared_stage1 = 0;

LV2_HOOKED_FUNCTION_POSTCALL_7(void, pre_map_process_memory, (void *object, uint64_t process_addr, uint64_t size, uint64_t flags, void *unk, void *elf, uint64_t *out))
{
	#ifdef  DEBUG
//	DPRINTF("Map %lx %lx %s\n", process_addr, size, get_current_process() ? get_process_name(get_current_process())+8 : "KERNEL");
	#endif
/// LIBFS patching is currently disabled
/*	uint32_t i=0;
	if(size>=0xd684)
	{
//		DPRINTF("map memory first 8 bytes are: %016llx\n",*(uint64_t *)elf);
		for(i=0;i<size;i+=4)
		{
			if(*(uint32_t *)(elf+i)==0x2F830000 && *(uint32_t *)(elf+i+4)==0x7C0802A6 && *(uint32_t *)(elf+i+8)==0x3D208001 && *(uint32_t *)(elf+i+0xc)==0xF821FF71 &&
		    	*(uint32_t *)(elf+i+0x10)==0xFBC10080 && *(uint32_t *)(elf+i+0x14)==0xFBE10088 && *(uint32_t *)(elf+i+0x18)==0xFBA10078 && *(uint32_t *)(elf+i+0x1c)==0xF80100A0 && *(uint64_t *)(elf+i+0x20)==0x7C7F1B787C9E2378)
				{
					DPRINTF("FOUND LIBFS!\n");
					*(uint64_t *)(elf+i)=0xF821FF917C0802A6;
					*(uint64_t *)(elf+i+8)=0xF80100807C852378;
					*(uint64_t *)(elf+i+0x10)=0x7C641B7838606637;
					*(uint64_t *)(elf+i+0x18)=0x3960000844000002;
					*(uint64_t *)(elf+i+0x20)=0xE80100807C0803A6;
					*(uint64_t *)(elf+i+0x28)=0x382100704E800020;
					break;
				}
		}
	}*/
	// Not the call address, but the call to the caller (process load code for .self)
	if (get_call_address(1) == (void *)MKA(process_map_caller_call))
	{       
		if ((process_addr == 0x10000) && (flags == 0x2008004) && (cleared_stage1 == 0))
		{
			#ifdef  DEBUG
			DPRINTF("Making Retail VSH text writable, Size: 0x%lx\n", size);   
			#endif
			// Change flags, RX -> RWX, make vsh text writable
			set_patched_func_param(4, 0x2004004);
		}
		else if  (flags == 0x2008004) set_patched_func_param(4, 0x2004004);// Change flags, RX -> RWX
	}	
}


static unsigned int poke_count = 0;

void pokeq(uint64_t address, uint64_t value)
{
	*(uint64_t *)address=value;
}

static void do_pokes(void)
{
	for(unsigned int i = 0; i < poke_count; i++)
	{
		pokeq( pokess[i].addrs_backup, pokess[i].value_pokes );
	}
}

static void remove_pokes(void)
{
	for(unsigned int i = 0; i < poke_count; i++)
	{
		pokeq( pokess[i].addrs_backup, pokess[i].value_backup);
	}
	poke_count = 0;
}


uint8_t condition_apphome=0;
#define SYS_MAP_PATH	35
#define SYS_MAP_GAME	36

#define FLAG_COPY		1
#define FLAG_TABLE		2
#define FLAG_PROTECT	4

void	map_path_patches(int syscall);
int  	map_path(char *oldpath, char *newpath, uint32_t flags);
int 	map_path_user(char *oldpath, char *newpath, uint32_t flags);
int 	sys_map_path(char *oldpath, char *newpath);
int 	sys_map_paths(char *paths[], char *new_paths[], unsigned int num);
/*int	sys_map_game(char *path);
int	sys_hermes_pathtable(uint64_t pathtable);*/

int 	sys_aio_copy_root(char *src, char *dst);

#define MAX_TABLE_ENTRIES 16

typedef struct _MapEntry
{
	char *oldpath;
	char *newpath;
	int  newpath_len;
	uint32_t flags;	
} MapEntry;

MapEntry map_table[MAX_TABLE_ENTRIES];

void unhook_all_map_path(void);

int map_path(char *oldpath, char *newpath, uint32_t flags)
{
	int i, firstfree = -1;
	
	if (!oldpath || strlen(oldpath) == 0)
		return -1;
	
	if (newpath && strcmp(oldpath, newpath) == 0)
		newpath = NULL;
	
	if (strcmp(oldpath, "/dev_bdvd") == 0)	
		condition_apphome = (newpath != NULL);	
	
	for (i = 0; i < MAX_TABLE_ENTRIES; i++)
	{
		if (map_table[i].oldpath)
		{
			if (strcmp(oldpath, map_table[i].oldpath) == 0)
			{
				if (newpath && strlen(newpath))
				{
					strncpy(map_table[i].newpath, newpath, MAX_PATH-1);	
					map_table[i].newpath[MAX_PATH-1] = 0;
					map_table[i].newpath_len = strlen(newpath);
					map_table[i].flags = (map_table[i].flags&FLAG_COPY) | (flags&(~FLAG_COPY));
				}
				else
				{
					if (map_table[i].flags & FLAG_COPY)
						dealloc(map_table[i].oldpath, 0x27);
					
					dealloc(map_table[i].newpath, 0x27);					
					map_table[i].oldpath = NULL;
					map_table[i].newpath = NULL;	
					map_table[i].flags = 0;
				}
				
				break;
			}
		}
		else if (firstfree < 0)		
			firstfree = i;		
	}
	
	if (i == MAX_TABLE_ENTRIES)
	{
		if (firstfree < 0)
			return EKRESOURCE;
		
		if (!newpath || strlen(newpath) == 0)
			return 0;
		
		map_table[firstfree].flags = flags;
		
		if (flags & FLAG_COPY)
		{
			int len = strlen(oldpath);
			map_table[firstfree].oldpath = alloc(len+1, 0x27);
			strncpy(map_table[firstfree].oldpath, oldpath, len);
			map_table[firstfree].oldpath[len] = 0;
		}
		else		
			map_table[firstfree].oldpath = oldpath;		
		
		map_table[firstfree].newpath = alloc(MAX_PATH, 0x27);
		strncpy(map_table[firstfree].newpath, newpath, MAX_PATH-1);	
		map_table[firstfree].newpath[MAX_PATH-1] = 0;
		map_table[firstfree].newpath_len = strlen(newpath);
	}
	
	return 0;	
}

int map_path_user(char *oldpath, char *newpath, uint32_t flags)
{
	char *oldp, *newp;
	
	#ifdef  DEBUG
		DPRINTF("map_path_user, called by process %s: %s -> %s\n", get_process_name(get_current_process_critical()), oldpath, newpath); 
	#endif
	
	if (oldpath == 0)
		return -1;
	
	int ret = pathdup_from_user(get_secure_user_ptr(oldpath), &oldp);
	if (ret != 0)
		return ret;
	
	if (newpath == 0)	
		newp = NULL;	
	else
	{
		ret = pathdup_from_user(get_secure_user_ptr(newpath), &newp);
		if (ret != 0)
		{
			dealloc(oldp, 0x27);
			return ret;
		}
	}
	
	ret = map_path(oldp, newp, flags | FLAG_COPY);
	
	dealloc(oldp, 0x27);	
	if (newp)
		dealloc(newp, 0x27);
	
	return ret;
}

LV2_SYSCALL2(int, sys_map_path, (char *oldpath, char *newpath))
{
	extend_kstack(0);
	return map_path_user(oldpath, newpath, 0);	
}

int sys_map_paths(char *paths[], char *new_paths[], unsigned int num)
{
	uint32_t *u_paths = (uint32_t *)get_secure_user_ptr(paths);
	uint32_t *u_new_paths = (uint32_t *)get_secure_user_ptr(new_paths);
	int unmap = 0;	
	int ret = 0;
	
	if (!u_paths)	
		unmap = 1;	
	else
	{
		if (!u_new_paths)
			return EINVAL;
		
		for (unsigned int i = 0; i < num; i++)
		{
			ret = map_path_user((char *)(uint64_t)u_paths[i], (char *)(uint64_t)u_new_paths[i], FLAG_TABLE);
			if (ret != 0)
			{
				unmap = 1;
				break;
			}
		}
	}
	
	if (unmap)
	{
		for (int i = 0; i < MAX_TABLE_ENTRIES; i++)
		{
			if (map_table[i].flags & FLAG_TABLE)
			{
				if (map_table[i].flags & FLAG_COPY)	
					dealloc(map_table[i].oldpath, 0x27);
				
				dealloc(map_table[i].newpath, 0x27);					
				map_table[i].oldpath = NULL;
				map_table[i].newpath = NULL;	
				map_table[i].flags = 0;
			}
		}
	}
	
	return ret;
}


LV2_HOOKED_FUNCTION_POSTCALL_2(void, open_path_hook, (char *path0, int mode))
{
	if (path0[0]=='/')
	{
        char *path=path0;
        if(path[1]=='/') path++; 
		{
			for (int i = MAX_TABLE_ENTRIES-1; i >= 0; i--)
			{
				if (map_table[i].oldpath)
				{
					int len = strlen(map_table[i].oldpath);
			
					if (path && strncmp(path, map_table[i].oldpath, len) == 0)
					{
						strcpy(map_table[i].newpath+map_table[i].newpath_len, path+len);
						set_patched_func_param(1, (uint64_t)map_table[i].newpath);
						
						#ifdef  DEBUG
							//DPRINTF("=: [%s]\n", map_table[i].newpath);
						#endif 
						break;
					}
				}
			}
		}
		//DPRINTF("open_path %s\n", path);  // listing file as they are being accessed
	}
//	if(strstr(path0,"/dev_bdvd/PS3_GAME/LICDIR/LIC.DAT"))
//		path0[10]=0x00;
}

int sys_aio_copy_root(char *src, char *dst)
{
	int len;
	DPRINTF("AIO COPY:%s %s\n",src,dst);
	src = get_secure_user_ptr(src);
	dst = get_secure_user_ptr(dst);
	
	// Begin original function implementation	
	if (!src)
		return EFAULT;
	
	len = strlen(src);
	
	if (len >= 0x420 || len <= 1 || src[0] != '/')
		return EINVAL;
	
	strcpy(dst, src); 
	
	for (int i = 1; i < len; i++)
	{
		if (dst[i] == 0)
			break;
		
		if (dst[i] == '/')
		{
			dst[i] = 0;
			break;
		}
	}
	
	
	if (strlen(dst) >= 0x20)
		return EINVAL;	
	
	// Here begins custom part of the implementation
	if (strcmp(dst, "/dev_bdvd") == 0 && condition_apphome) // if dev_bdvd and jb game mounted 
	{
		// find /dev_bdvd
		for (int i = 0; i < MAX_TABLE_ENTRIES; i++)
		{
			if (map_table[i].oldpath && strcmp(map_table[i].oldpath, "/dev_bdvd") == 0)
			{
				for (int j = 1; j < map_table[i].newpath_len; j++)
				{
					dst[j] = map_table[i].newpath[j];
					
					if (dst[j] == 0)
						break;
					
					if (dst[j] == '/')
					{
						dst[j] = 0;
						break;
					}
				}
				
				
				break;
			}
		}
	}			
		
	return 0;
}

void unhook_all_map_path(void)
{
	suspend_intr();
	unhook_function_with_postcall(open_path_symbol, open_path_hook, 2);
	resume_intr();
}

void map_path_patches(int syscall)
{
	hook_function_with_postcall(open_path_symbol, open_path_hook, 2);
	
	if (syscall)
		create_syscall2(SYS_MAP_PATH, sys_map_path);	
}



uint64_t support_advanced_flags(uint64_t *r3, uint64_t r4);
int pre_modules_verification(uint32_t *ret, uint32_t error);

LV2_HOOKED_FUNCTION_COND_POSTCALL_2(int, pre_modules_verification, (uint32_t *ret, uint32_t error))
{
	*ret = 0;
	return 0;
}

static uint8_t *saved_buf;
static void *saved_sce_hdr;

LV2_HOOKED_FUNCTION_PRECALL_2(int, post_lv1_call_99_wrapper, (uint64_t *spu_obj, uint64_t *spu_args))
{
	// This replaces an original patch of psjailbreak, since we need to do more things
	process_t process = get_current_process();

	saved_buf = (void *)spu_args[0x20/8];
	saved_sce_hdr = (void *)spu_args[8/8];

	return 0;
}

LV2_EXPORT int decrypt_func(uint64_t *, uint32_t *);

//#define patch_func2 					0x59D84 // Confirmed
//#define patch_func2_offset 				0x2C

LV2_HOOKED_FUNCTION_POSTCALL_2_RET_MANUAL(uint64_t, modules_patching, (uint64_t *arg1, uint32_t *arg2))
{
//	unhook_function_with_postcall_manual(0x59D84 + patch_func2_offset, modules_patching,2);
	uint64_t *ptr;
	uint32_t *ptr32;

	ptr = (uint64_t *)(*(uint64_t *)MKA(TOC+decrypt_rtoc_entry_2));
	ptr = (uint64_t *)ptr[0x68/8];
	ptr = (uint64_t *)ptr[0x18/8];
	ptr32 = (uint32_t *)ptr;
	uint32_t *p = (uint32_t *)arg1[0x18/8];

	
	// +4.30 -> 0x13 (exact firmware since it happens is unknown)
	// 3.55 -> 0x29
	
	if ((p[0x30/4] >> 16) == 0x13)
	{	
		int last_chunk = 0;

		if (((ptr[0x10/8] << 24) >> 56) == 0xFF)
		{
			ptr[0x10/8] |= 2;
			*arg2 = 0x2C;
			last_chunk = 1;
		}
		else
		{
			ptr[0x10/8] |= 3;
			*arg2 = 6;
		}
		memcpy(saved_buf, (void *)ptr[8/8], ptr32[4/4]);
		saved_buf += ptr32[4/4];
	}
	else
	{
		decrypt_func(arg1, arg2);
	}

//	hook_function_with_postcall_manual(0x59D84 + patch_func2_offset, modules_patching,2);
	return 0;
}

LV2_HOOKED_FUNCTION_POSTCALL_2_RET_MANUAL(uint64_t, support_advanced_flags, (uint64_t *r3, uint64_t r4))
{
	f_desc_t f;
	f.addr=(void *)MKA(0x4ef1c);
	f.toc=(void *)MKA(TOC);
	uint64_t (*func)(uint64_t *, uint64_t)=(void *)&f;
	if(get_call_address(1)==(void *)0x8000000000059db0)
	{
		func(r3,r4);
		*(uint64_t *)(r4+8)=0; //ecdsa flag
		return 0;
	}
	*(uint32_t *)0x800000000005a6f8=0x419e00ac;
	*(uint32_t *)0x80000000000564b0=0x7FE307B4;
	*(uint32_t *)0x8000000000056614=0x48216FB5;
	*(uint32_t *)0x80000000000203fc=0x419DFF84;
	*(uint32_t *)0x8000000000020408=0x419D0258;
	
	
	*(uint32_t *)0x800000000005a6e4=0x419e00ac;
	*(uint32_t *)0x8000000000059dc4=0x7FE307B4;
	*(uint32_t *)0x8000000000056230=0x48240EED;
	*(uint64_t *)0x80000000002275f4=0xE86900007C6307B4;
	*(uint64_t *)0x80000000002d2b34=0xF821FE617CE802A6;
//	*(uint64_t *)0x8000000000003d90=0xF821FEB17C0802A6;
	*(uint64_t *)0x800000000005658C=0x63FF003D419EFFD4;
	*(uint64_t *)0x8000000000056650=0x3FE0800163FF003E;
	*(uint64_t *)0x8000000000056604=0x2F840004409C0048;

	*(uint64_t *)0x8000000000474A80=0;
	unhook_function_with_postcall(open_path_symbol, open_path_hook, 2);
	unhook_function_with_cond_postcall(modules_verification_symbol, pre_modules_verification, 2);
	unhook_function_with_postcall_manual(0x4ef1c, support_advanced_flags, 2);
	unhook_function_with_postcall(map_process_memory_symbol, pre_map_process_memory, 7);
//	unhook_function_with_postcall_manual(lv1_call_99_wrapper_symbol, post_lv1_call_99_wrapper, 2);
//	unhook_function_with_precall(lv1_call_99_wrapper_symbol, post_lv1_call_99_wrapper, 2);
//	unhook_function_with_postcall_manual(0x59D84 + patch_func2_offset, modules_patching,2);
	
	unhook_all_storage_ext();

	if(poke_count)
	{
//		remove_pokes();
	}

	uint64_t ret=0;
	
	ret=func(r3,r4); //this function calls appldr and triggers boom!

	
	hook_function_with_cond_postcall(modules_verification_symbol, pre_modules_verification, 2);
	hook_function_with_postcall(map_process_memory_symbol, pre_map_process_memory, 7);
	*(uint32_t *)0x80000000000564b0=0x38600000; // patch_func8_offset1 
	*(uint32_t *)0x8000000000056614=0x60000000; // patch_func8_offset2 
	*(uint32_t *)0x80000000000203fc=0x60000000; // user_thread_prio_patch	for netiso
	*(uint32_t *)0x8000000000020408=0x60000000; // user_thread_prio_patch2	net iso
	*(uint32_t *)0x8000000000059dc4=0x38600000; // patch_func2_offset (modules_patching)
	*(uint32_t *)0x8000000000056230=0x38600001; // ignore LIC.DAT check	
	*(uint32_t *)0x800000000005a6f8=0x60000000; // fix 80010009 error
	*(uint32_t *)0x800000000005a6e4=0x60000000; // fix 80010009 error
	*(uint64_t *)0x80000000002275f4=0x38600000F8690000;  // fix 0x8001002B / 80010017 errors  known as ODE patch
	*(uint64_t *)0x80000000002d2b34=0x386000004e800020; // ????
//	*(uint64_t *)0x8000000000003d90=0x386000014e800020; - // psjailbreak, PL3, etc destroy this function to copy their code there.
	*(uint64_t *)0x800000000005658C=0x63FF003D60000000;  // fix 8001003D error
	*(uint64_t *)0x8000000000056650=0x3FE080013BE00000; // fix 8001003E error
	*(uint64_t *)0x8000000000056604=0x2F84000448000098; //PATCH_JUMP
	
	*(uint64_t *)0x8000000000474A80=0;
	*(uint64_t *)(r4+8)=0; //ecdsa flag
	hook_function_with_postcall_manual(0x4ef1c, support_advanced_flags, 2);
	hook_function_with_postcall(open_path_symbol, open_path_hook, 2);
//	hook_function_with_postcall_manual(lv1_call_99_wrapper_symbol, post_lv1_call_99_wrapper, 2);
//	hook_function_with_precall(lv1_call_99_wrapper_symbol, post_lv1_call_99_wrapper, 2);
//	hook_function_with_postcall_manual(0x59D84 + patch_func2_offset, modules_patching,2);

	storage_ext_patches();
	if(poke_count)
	{
//		do_pokes();
	}
	return 0;
}

LV2_SYSCALL2(uint64_t, sys_cfw_peek, (uint64_t *addr))
{
	uint64_t ret = *addr;

	return ret;
}

LV2_SYSCALL2(void, sys_cfw_poke, (uint64_t addr, uint64_t value))
{
	DPRINTF("LV2 poke %p %016lx\n", (void*)addr, value);
	if(addr>0x8000000000363c10 && addr<0x8000000000363c88)
	{
		return;
	}

	if(addr>0x8000000000352230)
	{
		*(uint64_t *)MKA(addr)=value;
		return;
	}
	
/*	if(poke_count >= MAX_POKES)
	{
		return;
	}

	pokess[poke_count].addrs_backup=addr;
	pokess[poke_count].value_backup=*(uint64_t *)MKA(addr);
	pokess[poke_count].value_pokes=value;

	poke_count++;

	*(uint64_t *)MKA(addr) = value;*/
}

LV2_SYSCALL2(void, sys_cfw_lv1_call, (uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t num))
{
	/* DO NOT modify */
	asm("mflr 0\n");
	asm("std 0, 16(1)\n");
	asm("mr 11, 10\n");
	asm("sc 1\n");
	asm("ld 0, 16(1)\n");
	asm("mtlr 0\n");
	asm("blr\n");
}

LV2_SYSCALL2(void, sys_cfw_lv2_func, (uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t num))
{
	/* DO NOT modify */
	asm("mflr 0\n");
	asm("std 0, 16(1)\n");
	asm("mr 0, 10\n");
	asm("mtctr 0\n");
	asm("bctrl\n");
	asm("ld 0, 16(1)\n");
	asm("mtlr 0\n");
	asm("blr\n");
}

#include "syscall8.h"

#if defined(FIRMWARE_3_41)
#define FIRMWARE_VERSION	0x0000
#elif defined(FIRMWARE_3_55)
#define FIRMWARE_VERSION	0x0355
#elif defined(FIRMWARE_4_30)
#define FIRMWARE_VERSION	0x0430
#elif defined(FIRMWARE_4_46)
#define FIRMWARE_VERSION	0x0446
#elif defined(FIRMWARE_4_84)
#define FIRMWARE_VERSION	0x0484
#endif

#define IS_CFW			1

#define MAKE_VERSION(cobra, fw, type) ((cobra&0xFF) | ((fw&0xffff)<<8) | ((type&0x1)<<24))
#define COBRA_VERSION		0x0F
#define COBRA_VERSION_BCD	0x0751

static INLINE int sys_get_version(uint32_t *version)
{
	uint32_t pv = MAKE_VERSION(COBRA_VERSION, FIRMWARE_VERSION, IS_CFW); 
	return copy_to_user(&pv, get_secure_user_ptr(version), sizeof(uint32_t));
}

static INLINE int sys_get_version2(uint16_t *version)
{
	uint16_t cb = COBRA_VERSION_BCD;
	return copy_to_user(&cb, get_secure_user_ptr(version), sizeof(uint16_t));
}


LV2_SYSCALL2(int64_t, syscall8, (uint64_t function, uint64_t param1, uint64_t param2, uint64_t param3, uint64_t param4, uint64_t param5, uint64_t param6, uint64_t param7))
{	
	extend_kstack(0);
	
	DPRINTF("Syscall 8 (PS3HEN) -> %lx\n", function);
	
	switch (function)
	{
		case SYSCALL8_OPCODE_PS3MAPI:
			switch ((int)param1)
			{
				//DPRINTF("syscall8: PS3M_API function 0x%x\n", (int)param1);

				//----------
				//CORE
				//----------
				case PS3MAPI_OPCODE_GET_CORE_VERSION:
					return PS3MAPI_CORE_VERSION;
				break;
				case PS3MAPI_OPCODE_GET_CORE_MINVERSION:
					return PS3MAPI_CORE_MINVERSION;
				break;
				case PS3MAPI_OPCODE_GET_FW_VERSION:
					return PS3MAPI_FW_VERSION;
				break;
				case PS3MAPI_OPCODE_GET_FW_TYPE:
					return ps3mapi_get_fw_type((char *)param2);
				break;

				case PS3MAPI_OPCODE_SET_ACCESS_KEY: //dummy
					return 0;
				//----------------
				//PEEK & POKE (av)
				//----------------
				case PS3MAPI_OPCODE_LV2_PEEK:
					return *(uint64_t *)param2;
				break;
				
				case PS3MAPI_OPCODE_LV2_POKE:
					if(param2>0x8000000000352230)
					{
						*(uint64_t *)param2=param3;
					}
					return 0;
				break;

				//----------
				//PROCESS
				//----------
				case PS3MAPI_OPCODE_GET_ALL_PROC_PID:
					return ps3mapi_get_all_processes_pid((process_id_t *)param2);
				break;
				case PS3MAPI_OPCODE_GET_PROC_NAME_BY_PID:
					return ps3mapi_get_process_name_by_pid((process_id_t)param2, (char *)param3);
				break;
				case PS3MAPI_OPCODE_GET_PROC_BY_PID:
					return ps3mapi_get_process_by_pid((process_id_t)param2, (process_t)param3);
				break;
				case PS3MAPI_OPCODE_GET_CURRENT_PROC:
					return ps3mapi_get_current_process((process_t)param2);
				break;
				case PS3MAPI_OPCODE_GET_CURRENT_PROC_CRIT:
					return ps3mapi_get_current_process_critical((process_t)param2);
				break;
				//----------
				//MEMORY
				//----------
				case PS3MAPI_OPCODE_GET_PROC_MEM:
					return ps3mapi_get_process_mem((process_id_t)param2, param3, (char *)param4, (int)param5);
				break;
				case PS3MAPI_OPCODE_SET_PROC_MEM:
					return ps3mapi_set_process_mem((process_id_t)param2, param3, (char *)param4, (int)param5);
				break;
				//----------
				//MODULE
				//----------
				case PS3MAPI_OPCODE_GET_ALL_PROC_MODULE_PID:
					return ps3mapi_get_all_process_modules_prx_id((process_id_t)param2, (sys_prx_id_t *)param3);
				break;
				case PS3MAPI_OPCODE_GET_PROC_MODULE_NAME:
					return ps3mapi_get_process_module_name_by_prx_id((process_id_t)param2, (sys_prx_id_t)param3, (char *)param4);
				break;
				case PS3MAPI_OPCODE_GET_PROC_MODULE_FILENAME:
					return ps3mapi_get_process_module_filename_by_prx_id((process_id_t)param2, (sys_prx_id_t)param3, (char *)param4);
				break;
				case PS3MAPI_OPCODE_LOAD_PROC_MODULE:
					return ps3mapi_load_process_modules((process_id_t)param2, (char *)param3, (void *)param4, (uint32_t)param5);
				break;
				case PS3MAPI_OPCODE_UNLOAD_PROC_MODULE:
					return ps3mapi_unload_process_modules((process_id_t)param2, (sys_prx_id_t)param3);
				break;
				//----------
				//VSH PLUGINS
				//----------
				case PS3MAPI_OPCODE_UNLOAD_VSH_PLUGIN:
					return ps3mapi_unload_vsh_plugin((char *)param2);
				break;
				case PS3MAPI_OPCODE_GET_VSH_PLUGIN_INFO:
					return ps3mapi_get_vsh_plugin_info((unsigned int)param2, (char *)param3, (char *)param4);
				break;
				//----------
				//SYSCALL
				//----------
				case PS3MAPI_OPCODE_DISABLE_SYSCALL:
					return ps3mapi_disable_syscall((int)param2);
				break;
				case PS3MAPI_OPCODE_CHECK_SYSCALL:
					return ps3mapi_check_syscall((int)param2);
				break;
				//-----------
				//PSID/IDPS
				//-----------
				case PS3MAPI_OPCODE_GET_IDPS:
					return ps3mapi_get_idps((uint64_t *)param2);
				break;
				case PS3MAPI_OPCODE_SET_IDPS:
					return ps3mapi_set_idps(param2, param3);
				break;
				case PS3MAPI_OPCODE_GET_PSID:
					return ps3mapi_get_psid((uint64_t *)param2);
				break;
				case PS3MAPI_OPCODE_SET_PSID:
					return ps3mapi_set_psid(param2, param3);
				break;
				//----------
				//DEFAULT
				//----------
				default:
					DPRINTF("syscall8: Unsupported PS3M_API opcode: 0x%lx\n", function);
					return ENOSYS;
				break;
			}
		break;
		
		case SYSCALL8_OPCODE_POKE_LV2:
				pokeq(param1, param2);
				return 0;
		break;
		
		case SYSCALL8_OPCODE_IS_HEN:
			return 0x1337;
		break;
		
		case SYSCALL8_OPCODE_READ_COBRA_CONFIG:
			return sys_read_cobra_config((CobraConfig *)param1);
		break;
		
		case SYSCALL8_OPCODE_WRITE_COBRA_CONFIG:
			return sys_write_cobra_config((CobraConfig *)param1);
			
		case SYSCALL8_OPCODE_GET_VERSION:
			return sys_get_version((uint32_t *)param1);
		break;
		
		case SYSCALL8_OPCODE_GET_VERSION2:
			return sys_get_version2((uint16_t *)param1);
		break;
		
		case SYSCALL8_OPCODE_GET_DISC_TYPE:
			return sys_storage_ext_get_disc_type((unsigned int *)param1, (unsigned int *)param2, (unsigned int *)param3);
		break;

		case SYSCALL8_OPCODE_READ_PS3_DISC:
			return sys_storage_ext_read_ps3_disc((void *)param1, param2, (uint32_t)param3);
		break;

		case SYSCALL8_OPCODE_FAKE_STORAGE_EVENT:
			return sys_storage_ext_fake_storage_event(param1, param2, param3);
		break;

		case SYSCALL8_OPCODE_GET_EMU_STATE:
			return sys_storage_ext_get_emu_state((sys_emu_state_t *)param1);
		break;
		
		case SYSCALL8_OPCODE_MOUNT_PS3_DISCFILE:
			return sys_storage_ext_mount_ps3_discfile(param1, (char **)param2);
		break;

		case SYSCALL8_OPCODE_MOUNT_DVD_DISCFILE:
			return sys_storage_ext_mount_dvd_discfile(param1, (char **)param2);
		break;

		case SYSCALL8_OPCODE_MOUNT_BD_DISCFILE:
			return sys_storage_ext_mount_bd_discfile(param1, (char **)param2);
		break;
		
		case SYSCALL8_OPCODE_MOUNT_PSX_DISCFILE:
			return sys_storage_ext_mount_psx_discfile((char *)param1, param2, (ScsiTrackDescriptor *)param3);
		break;
		
		case SYSCALL8_OPCODE_MOUNT_DISCFILE_PROXY:
			return sys_storage_ext_mount_discfile_proxy(param1, param2, param3, param4, param5, param6, (ScsiTrackDescriptor *)param7);
		break;

		case SYSCALL8_OPCODE_UMOUNT_DISCFILE:
			return sys_storage_ext_umount_discfile();
		break;

		case SYSCALL8_OPCODE_MAP_PATHS:
			return sys_map_paths((char **)param1, (char **)param2, param3);
		break;

		case SYSCALL8_OPCODE_AIO_COPY_ROOT:
			return sys_aio_copy_root((char *)param1, (char *)param2);
		break;

		case SYSCALL8_OPCODE_GET_ACCESS:
		case SYSCALL8_OPCODE_REMOVE_ACCESS:
			return 0;//needed for mmCM
			
		default:
			return ENOSYS;
	}
	
	
	return ENOSYS;
}

void patch_homebrew(uint64_t arg0)
{
	*(uint32_t *)0x80000000003E2E40=0x01000000; // patch_data1_offset
	*(uint32_t *)0x8000000000059dc4=0x38600000; // patch_func2_offset (modules_patching)
	*(uint32_t *)0x8000000000056230=0x38600001; // ignore LIC.DAT check	
	*(uint32_t *)0x800000000005a6f8=0x60000000; // fix 80010009 error
	*(uint32_t *)0x800000000005a6e4=0x60000000; // fix 80010009 error
	*(uint64_t *)0x80000000002275f4=0x38600000F8690000;  // fix 0x8001002B / 80010017 errors  known as ODE patch
	*(uint64_t *)0x80000000002d2b34=0x386000004e800020; // ????
//	*(uint64_t *)0x8000000000003d90=0x386000014e800020; - // psjailbreak, PL3, etc destroy this function to copy their code there.
	*(uint64_t *)0x800000000005658C=0x63FF003D60000000;  // fix 8001003D error
	*(uint64_t *)0x8000000000056650=0x3FE080013BE00000; // fix 8001003E error
	*(uint64_t *)0x8000000000056604=0x2F84000448000098; //PATCH_JUMP
	*(uint32_t *)0x80000000000564b0=0x38600000; // patch_func8_offset1  - force lv2open return 0
	*(uint32_t *)0x8000000000056614=0x60000000; // patch_func8_offset2 
	*(uint32_t *)0x80000000000203fc=0x60000000; // user_thread_prio_patch	for netiso
	*(uint32_t *)0x8000000000020408=0x60000000; // user_thread_prio_patch2	net iso
	
	*(uint64_t *)0x8000000000474A80=0;
	map_path_patches(1);
	hook_function_with_cond_postcall(modules_verification_symbol, pre_modules_verification, 2);
	hook_function_with_postcall_manual(0x4ef1c, support_advanced_flags, 2);
	hook_function_with_postcall(map_process_memory_symbol, pre_map_process_memory, 7);
//	hook_function_with_postcall_manual(lv1_call_99_wrapper_symbol, post_lv1_call_99_wrapper, 2);
//	hook_function_with_precall(lv1_call_99_wrapper_symbol, post_lv1_call_99_wrapper, 2);
//	hook_function_with_postcall_manual(0x59D84 + patch_func2_offset, modules_patching,2);

	CellFsStat stat;
	if(cellFsStat("/dev_hdd0/plugins/CFW/bdplayer/bdp_BDMV.self",&stat)==0)
	{
		map_path("/dev_flash/", "/dev_hdd0/plugins/CFW/",0);
	}

	pokess = alloc(MAX_POKES * sizeof(POKES), 0x27);
	create_syscall2(6, sys_cfw_peek);
	create_syscall2(7, sys_cfw_poke);
	create_syscall2(8, syscall8);
	create_syscall2(10, sys_cfw_lv1_call);
	create_syscall2(15, sys_cfw_lv2_func);
		
//	ppu_thread_exit(0);
return;
}

//Boot plugin support, currently kernel panics when launching homebrew
#define MAX_VSH_PLUGINS 			7
#define BOOT_PLUGINS_FILE			"/dev_usb000/boot_plugins_nocobra.txt"
#define BOOT_PLUGINS_FIRST_SLOT 	1
#define MAX_BOOT_PLUGINS			(MAX_VSH_PLUGINS-BOOT_PLUGINS_FIRST_SLOT)

process_t vsh_process;
sys_prx_id_t vsh_plugins[MAX_VSH_PLUGINS];
//uint8_t safe_mode;

int read_text_line(int fd, char *line, unsigned int size, int *eof)
{
	int i = 0;
	int line_started = 0;

	if (size == 0)
		return -1;

	*eof = 0;

	while (i < (size-1))
	{
		uint8_t ch;
		uint64_t r;

		if (cellFsRead(fd, &ch, 1, &r) != 0 || r != 1)
		{
			*eof = 1;
			break;
		}

		if (!line_started)
		{
			if (ch > ' ')
			{
				line[i++] = (char)ch;
				line_started = 1;
			}
		}
		else
		{
			if (ch == '\n' || ch == '\r')
				break;

			line[i++] = (char)ch;
		}
	}

	line[i] = 0;

	// Remove space chars at end
	for (int j = i-1; j >= 0; j--)
	{
		if (line[j] <= ' ')
		{
			line[j] = 0;
			i = j;
		}
		else		
			break;		
	}

	return i;
}

int prx_load_vsh_plugin(unsigned int slot, char *path)
{
	sys_prx_id_t prx;
	int ret=-1;	
	
	if (slot >= MAX_VSH_PLUGINS)
		return ret;
	
	if (vsh_plugins[slot] != 0)
	{
		return ret;
	}
	
	prx = prx_load_module(vsh_process, 0, 0, path);
	
	if (prx < 0)
		return prx;
	
	
	ret = prx_start_module_with_thread(prx, vsh_process, 0, 0);
	
	if (ret == 0)
	{
		vsh_plugins[slot] = prx;
	}
	else
	{
		prx_stop_module_with_thread(prx, vsh_process, 0, 0);
		prx_unload_module(prx, vsh_process);
	}
	
	return ret;
}

void load_boot_plugins(void)
{
	cellFsUtilMount_h("CELL_FS_IOS:BUILTIN_FLSH1", "CELL_FS_FAT", "/dev_blind", 0, 0, 0, 0, 0);
	int fd;
	int current_slot = BOOT_PLUGINS_FIRST_SLOT;
	int num_loaded = 0;
	
	if (cellFsOpen(BOOT_PLUGINS_FILE, CELL_FS_O_RDONLY, &fd, 0, NULL, 0) != 0)
		return;
	
	while (num_loaded < MAX_BOOT_PLUGINS)
	{
		char path[128];
		int eof;
		
		if (read_text_line(fd, path, sizeof(path), &eof) > 0)
		{
			int ret = prx_load_vsh_plugin(current_slot, path);
			
			if (ret >= 0)
			{
				current_slot++;
				num_loaded++;
			}
		}
		
		if (eof)
			break;
	}
	
	cellFsClose(fd);
}

// Kernel version of prx_unload_vsh_plugin
int prx_unload_vsh_plugin(unsigned int slot)
{
	int ret;
	sys_prx_id_t prx;
	
	#ifndef  DEBUG
	DPRINTF("Trying to unload vsh plugin %x\n", slot);
	#endif

	if (slot >= MAX_VSH_PLUGINS)
		return EINVAL;

	prx = vsh_plugins[slot];
		
	#ifndef  DEBUG
	DPRINTF("Current plugin: %08X\n", prx);
	#endif


	if (prx == 0)
		return ENOENT;

	ret = prx_stop_module_with_thread(prx, vsh_process, 0, 0);
	if (ret == 0)
	{
		ret = prx_unload_module(prx, vsh_process);
	}		
	#ifndef  DEBUG
	else
	{
		DPRINTF("Stop failed: %x!\n", ret);
	}
	#endif

	if (ret == 0)
	{
		vsh_plugins[slot] = 0;
		#ifndef  DEBUG
		DPRINTF("Vsh plugin unloaded succesfully!\n");
		#endif
	}
	#ifndef  DEBUG
	else
	{
		DPRINTF("Unload failed : %x!\n", ret);
	}
	#endif

	return ret;
}

// User version of prx_unload_vsh_plugin. Implementation is same.
int sys_prx_unload_vsh_plugin(unsigned int slot)
{
	return prx_unload_vsh_plugin(slot);

}

/*
LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_8(int, post_cellFsUtilMount, (const char *block_dev, const char *filesystem, const char *mount_point, int unk, int read_only, int unk2, char *argv[], int argc))
{
	if (strcmp(mount_point, "/dev_hdd0") == 0 && strcmp(filesystem, "CELL_FS_UFS") == 0)
	{
		unhook_function_on_precall_success(cellFsUtilMount_symbol, post_cellFsUtilMount, 8);
		if(vsh_process)
		{
			load_boot_plugins();
		}
			
	}
	return 0;
}

LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_8(int, load_process_hooked, (process_t process, int fd, char *path, int r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t sp_70))
{
	if (strcmp(path, "/dev_flash/vsh/module/vsh.self") == 0)
	{		
		vsh_process = process;
		unhook_function_on_precall_success(load_process_symbol, load_process_hooked, 9);
	}	
	
	return 0;
}
 */
 
int ps3mapi_unload_vsh_plugin(char *name)
{
    if (vsh_process <= 0) return ESRCH;
	for (unsigned int slot = 0; slot < MAX_VSH_PLUGINS; slot++)
	{
		if (vsh_plugins[slot] == 0) continue;
		char *filename = alloc(256, 0x35);
		if (!filename) return ENOMEM;
		sys_prx_segment_info_t *segments = alloc(sizeof(sys_prx_segment_info_t), 0x35);
		if (!segments) {dealloc(filename, 0x35); return ENOMEM;}
		sys_prx_module_info_t modinfo;
		memset(&modinfo, 0, sizeof(sys_prx_module_info_t));
		modinfo.filename_size = 256;
		modinfo.segments_num = 1;
		int ret = prx_get_module_info(vsh_process, vsh_plugins[slot], &modinfo, filename, segments);
		if (ret == SUCCEEDED)
		{
			if (strcmp(modinfo.name, get_secure_user_ptr(name)) == 0) 
				{
					dealloc(filename, 0x35);
					dealloc(segments, 0x35);
					return prx_unload_vsh_plugin(slot);
				}				
		}
		dealloc(filename, 0x35);
		dealloc(segments, 0x35);
	}
	return ESRCH;
}

int ps3mapi_get_vsh_plugin_info(unsigned int slot, char *name, char *filename)
{
	if (vsh_process <= 0) return ESRCH;
	if (vsh_plugins[slot] == 0) return ENOENT;
	char *tmp_filename = alloc(256, 0x35);
	if (!tmp_filename) return ENOMEM;
	sys_prx_segment_info_t *segments = alloc(sizeof(sys_prx_segment_info_t), 0x35);
	if (!segments) {dealloc(tmp_filename, 0x35); return ENOMEM;}
	char tmp_filename2[256];
	char tmp_name[30];
	sys_prx_module_info_t modinfo;
	memset(&modinfo, 0, sizeof(sys_prx_module_info_t));
	modinfo.filename_size = 256;
	modinfo.segments_num = 1;
	int ret = prx_get_module_info(vsh_process, vsh_plugins[slot], &modinfo, tmp_filename, segments);
	if (ret == SUCCEEDED)
	{
			sprintf(tmp_name, "%s", modinfo.name);
			ret = copy_to_user(&tmp_name, get_secure_user_ptr(name), strlen(tmp_name));	
			sprintf(tmp_filename2, "%s", tmp_filename);
			ret = copy_to_user(&tmp_filename2, get_secure_user_ptr(filename), strlen(tmp_filename2));
	}
	dealloc(tmp_filename, 0x35);
	dealloc(segments, 0x35);
	return ret;
}
 
void main(void)
{
//	thread_t my_thread;
//	ppu_thread_create(&my_thread, patch_homebrew, 0, -0x1D8, 0x4000, 0, "Patch Homebrew Thread");
	#ifdef  DEBUG
	debug_init();
	extern uint64_t _start;
	extern uint64_t __self_end;
	DPRINTF("Welcome to PS3HEN (load base = %p, end = %p) (version = %08X)\n", &_start, &__self_end, MAKE_VERSION(COBRA_VERSION, FIRMWARE_VERSION, IS_CFW));
	#endif
	patch_homebrew(0);
	//hook_function_on_precall_success(load_process_symbol, load_process_hooked, 9);
	//hook_function_on_precall_success(cellFsUtilMount_symbol, post_cellFsUtilMount, 8);
	storage_ext_init();
	storage_ext_patches();
	region_patches();
	init_mount_hdd0();
	load_boot_plugins();
	return;
}
