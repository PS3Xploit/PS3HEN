#include <lv2/lv2.h>
#include <lv2/libc.h>
#include <lv2/memory.h>
#include <lv2/patch.h>
#include <lv2/syscall.h>
#include <lv2/io.h>
#include <lv2/error.h>
#include "common.h"
#include "mappath.h"
#include "modulespatch.h"
#include "syscall8.h"
#include "ps3mapi_core.h"
#include <lv2/security.h>
#include <lv2/synchronization.h>

#define MAX_TABLE_ENTRIES 18

typedef struct _MapEntry
{
	char *oldpath;
	char *newpath;
	int  newpath_len;
	int  oldpath_len;
	uint32_t flags;	
} MapEntry;

MapEntry map_table[MAX_TABLE_ENTRIES];
uint8_t photo_gui = 1;

static mutex_t map_mtx = 0;

void init_mtx(){
	if(!map_mtx){
		mutex_create(&map_mtx, SYNC_PRIORITY, SYNC_NOT_RECURSIVE);
		#ifdef  DEBUG
			DPRINTF("init_mtx=: map_mtx 0x%8X created\n", (uint64_t)map_mtx);
		#endif 
	}
}

// void map_first_slot(char *old, char *newp)
// {
	// init_mtx();
	// mutex_lock(map_mtx, 0);
	// map_table[0].oldpath=old;
	// map_table[0].newpath = alloc(MAX_PATH, 0x27);
	// strncpy(map_table[0].newpath, newp, MAX_PATH-1);
	// map_table[0].newpath_len=strlen(newp);
	// map_table[0].oldpath_len = strlen(old);
	// map_table[0].flags = 0;
	// mutex_unlock(map_mtx);
// }
// void map_path_slot(char *old, char *newp, int slot)
// {
	// init_mtx();
	// mutex_lock(map_mtx, 0);
    // if(slot>=0 && slot<=MAX_TABLE_ENTRIES)
    // {
        // map_table[slot].oldpath=old;
        // map_table[slot].newpath = alloc(MAX_PATH, 0x27);
        // strncpy(map_table[slot].newpath, newp, MAX_PATH-1);
        // map_table[slot].newpath_len=strlen(newp);
        // map_table[slot].oldpath_len = strlen(old);
        // map_table[slot].flags = 0;
		// mutex_unlock(map_mtx);
    // }
// }
void map_path_slot(char *old, char *newp, uint32_t flags, int slot)
{
	init_mtx();
	mutex_lock(map_mtx, 0);
    if(slot>=0 && slot<=MAX_TABLE_ENTRIES)
    {
        map_table[slot].oldpath=old;
        map_table[slot].newpath = newp;
		map_table[slot].newpath_len=strlen(newp);
        map_table[slot].oldpath_len = strlen(old);
        map_table[slot].flags = flags;
		mutex_unlock(map_mtx);
    }
}
int map_path(char *oldpath, char *newpath, uint32_t flags)
{
	int i, firstfree = -1;
	
	if (!oldpath || strlen(oldpath) == 0)
		return -1;
	
	#ifdef  DEBUG
		DPRINTF("Map path: %s -> %s\n", oldpath, newpath);
	#endif

	if (newpath && strcmp(oldpath, newpath) == 0)
		newpath = NULL;
	
	if (strcmp(oldpath, "/dev_bdvd") == 0)	
		condition_apphome = (newpath != NULL);	
		
	init_mtx();
	mutex_lock(map_mtx, 0);
	#ifdef  DEBUG
		DPRINTF("map_path=: mutex locked\n");
	#endif 
	for (i = 1; i < MAX_TABLE_ENTRIES; i++)
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
					map_table[i].oldpath_len = 0;
					map_table[i].newpath_len = 0;					
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
		if (firstfree < 0){
			mutex_unlock(map_mtx);
			return EKRESOURCE;
		}
		if (!newpath || strlen(newpath) == 0){
			mutex_unlock(map_mtx);
			return 0;
		}
		
		map_table[firstfree].flags = flags;
		int len = strlen(oldpath);
		map_table[firstfree].oldpath_len=len;
		
		if (flags & FLAG_COPY)
		{
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
	mutex_unlock(map_mtx);
	#ifdef  DEBUG
		DPRINTF("map_path=: mutex unlocked\n");
	#endif 
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

int get_map_path(unsigned int num, char *path, char *new_path)
{
	if(num > MAX_TABLE_ENTRIES) return MAX_TABLE_ENTRIES;
	init_mtx();
	mutex_lock(map_mtx, 0);
	#ifdef  DEBUG
		DPRINTF("get_map_path=: mutex locked\n");
	#endif 
	if(map_table[num].oldpath_len == 0 || map_table[num].newpath_len == 0 || !map_table[num].newpath || !map_table[num].oldpath || !path) return -1;

	copy_to_user(&path, get_secure_user_ptr(map_table[num].oldpath), map_table[num].oldpath_len);

	if(new_path)
		copy_to_user(&new_path, get_secure_user_ptr(map_table[num].newpath), map_table[num].newpath_len);
	
	mutex_unlock(map_mtx);
	#ifdef  DEBUG
		DPRINTF("get_map_path=: mutex unlocked\n");
	#endif 
	return 0;
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
		init_mtx();
		mutex_lock(map_mtx, 0);
		#ifdef  DEBUG
			DPRINTF("sys_map_paths=: mutex locked\n");
		#endif 
		for (int i = 1; i < MAX_TABLE_ENTRIES; i++)
		{
			if (map_table[i].flags & FLAG_TABLE)
			{
				if (map_table[i].flags & FLAG_COPY)	
					dealloc(map_table[i].oldpath, 0x27);
				if (!(map_table[i].flags & FLAG_PROTECT))
					dealloc(map_table[i].newpath, 0x27);
				map_table[i].oldpath = NULL;
				map_table[i].newpath = NULL;	
				map_table[i].flags = 0;
			}
		}
		mutex_unlock(map_mtx);
		mutex_destroy(map_mtx);
		map_mtx=0;
		#ifdef  DEBUG
			DPRINTF("sys_map_paths=: mutex unlocked and destroyed\n");
		#endif 
		
	}
	
	return ret;
}
//////////////////////////////////////////////////////////////////////////////////////
// KW - HOMEBREW BLOCKER SUPPORT CODE TO USE IN open_path_hook()
//
// Functions, global vars and directives are here to improve code readability
// Le funzionalità, le variabili globali e le direttive sono qui per migliorare la lettura del codice
//
// declaration for read_text_line() which is defined in modulespatch.c after removal of the "static" declaration.
// la dechiarazione per read_text_line() viene definita in modulepatch.c soltanto dopo aver rimosso la dichiarazione "static"
int read_text_line(int fd, char *line, unsigned int size, int *eof);

#define BLACKLIST_FILENAME "/dev_hdd0/tmp/blacklist.cfg"
#define WHITELIST_FILENAME "/dev_hdd0/tmp/whitelist.cfg"
#define MAX_LIST_ENTRIES 30 // Maximum elements for noth the custom blacklist and whitelist.

static int __initialized_lists = 0; // Are the lists initialized ?
static int __blacklist_entries = 0; // Module global var to hold the current blacklist entries.
static char *__blacklist;
static int __whitelist_entries = 0; // Module global var to hold the current whitelist entries.
static char *__whitelist;


//
// init_list()
//
// inits a list.
// returns the number of elements read from file
char line[0x10];

static int init_list(char *list, char *path, int maxentries)
	{
		int loaded, f;

		if (cellFsOpen(path, CELL_FS_O_RDONLY, &f, 0, NULL, 0) != 0)
			return 0; // failed to open
		
		loaded = 0;

		while (loaded < maxentries)
		{
			int eof;
			if (read_text_line(f, line, sizeof(line), &eof) > 0)
			if (strlen(line) >=9) // avoid copying empty lines
			{	
				strncpy(list + (9*loaded), line, 9); // copy only the first 9 chars - if it has lees than 9, it will fail future checks. should correct in file.
				loaded++;
			}

			if (eof)
				break;
		}
		cellFsClose(f);
		return loaded;
	}


//
// listed()
//
// tests if a char gameid[9] is in the blacklist or whitelist
// initialize the both lists, if not yet initialized;
// receives the list to test blacklist (1) or whitelist (0), and the gameid
// to initialize the lists, tries to read them from file BLACKLIST_FILENAME and WHITELIST_FILENAME
//
// prova se un gameid[9] è in blacklist o whitelist
// inizializza entrambi gli elenchi, se non ancora inizializzati;
// riceve l'elenco per testare la blacklist (1) o la whitelist (0) e il gameid
// per inizializzare gli elenchi, prova a leggerli dal file BLACKLIST_FILENAME e WHITELIST_FILENAME

static int listed(int blacklist, char *gameid)
	{
		char *list;
		int i, elements;
		if (!__initialized_lists)
		{
			__blacklist=alloc(9*MAX_LIST_ENTRIES,0x27);
			__whitelist=alloc(9*MAX_LIST_ENTRIES,0x27);
			__blacklist_entries = init_list(__blacklist, BLACKLIST_FILENAME, MAX_LIST_ENTRIES);
			__whitelist_entries = init_list(__whitelist, WHITELIST_FILENAME, MAX_LIST_ENTRIES);
			__initialized_lists = 1;
		}

		if (blacklist)
		{
			list = __blacklist; 
			elements = __blacklist_entries;
		}
		else
		{
			list = __whitelist; 
			elements = __whitelist_entries;
		}

		for (i = 0; i < elements; i++)
		if (!strncmp(list+(9*i),gameid, 9))
			return 1; // gameid is in the list

		// if it got here, it is not in the list. return 0
		return 0;
	}

#define IDPS_KEYBITS 128
#define ACT_DAT_KEYBITS 128
#define RIF_KEYBITS 128
#define RAP_KEYBITS 128
	
static uint8_t libft2d_access = 0;

// BEGIN KW & AV block access to homebrews when syscalls are disabled
// After the core tests it will test first if the gameid is in whitelist.cfg (superseeds previous tests)
// In the it will test if the gameid is in blacklist.cfg (superseeds all previous tests)
// ** WARNING ** This syscall disablement test assumes that the syscall table entry 6 (peek) was replaced by the original value (equals syscall 0 entry) as done by PSNPatch
// ** WARNING ** If only a parcial disablement was made, this assumption WILL FAIL !!!
//
// Iniziato da KW & AV blocca l'accesso a homebrews quando le syscalls sono disabilitati
// Dopo i test di base, verificherà innanzitutto se il gameid è in whitelist.cfg (test superiori precedenti)
// Nel verificherà se il gameid è in blacklist.cfg (sostituisce tutti i test precedenti)
// ** AVVERTENZA ** Questo test di disabilitazione delle syscall, presuppone che la voce di tabella syscall 6 (peek) sia stata sostituita dal valore originale (equivale alla voce syscall 0) come fatto da PSNPatch
// ** AVVERTENZA ** Se è stata effettuata solo una disattivazione parziale, questa assunzione AVRA' ESITO NEGATIVO !!!

void aescbc128_decrypt(unsigned char *key, unsigned char *iv, unsigned char *in, unsigned char *out, int len)
{
	aescbccfb_dec(out,in,len,key,128,iv);
	// Reset the IV.
	memset(iv, 0, 0x10);
}

unsigned char RAP_KEY[] = {0x86, 0x9F, 0x77, 0x45, 0xC1, 0x3F, 0xD8, 0x90, 0xCC, 0xF2, 0x91, 0x88, 0xE3, 0xCC, 0x3E, 0xDF};
unsigned char RAP_PBOX[] = {0x0C, 0x03, 0x06, 0x04, 0x01, 0x0B, 0x0F, 0x08, 0x02, 0x07, 0x00, 0x05, 0x0A, 0x0E, 0x0D, 0x09};
unsigned char RAP_E1[] = {0xA9, 0x3E, 0x1F, 0xD6, 0x7C, 0x55, 0xA3, 0x29, 0xB7, 0x5F, 0xDD, 0xA6, 0x2A, 0x95, 0xC7, 0xA5};
unsigned char RAP_E2[] = {0x67, 0xD4, 0x5D, 0xA3, 0x29, 0x6D, 0x00, 0x6A, 0x4E, 0x7C, 0x53, 0x7B, 0xF5, 0x53, 0x8C, 0x74};
unsigned char crap_pants[13]={"///no_exists"};

void get_rif_key(unsigned char* rap, unsigned char* rif)
{
	int i;
	int round;

	unsigned char key[0x10];
	unsigned char iv[0x10];
	memset(key, 0, 0x10);
	memset(iv, 0, 0x10);

	// Initial decrypt.
	aescbc128_decrypt(RAP_KEY, iv, rap, key, 0x10);

	// rap2rifkey round.
	for (round = 0; round < 5; ++round)
	{
		for (i = 0; i < 16; ++i)
		{
			int p = RAP_PBOX[i];
			key[p] ^= RAP_E1[p];
		}
		for (i = 15; i >= 1; --i)
		{
			int p = RAP_PBOX[i];
			int pp = RAP_PBOX[i - 1];
			key[p] ^= key[pp];
		}
		int o = 0;
		for (i = 0; i < 16; ++i)
		{
			int p = RAP_PBOX[i];
			unsigned char kc = key[p] - o;
			unsigned char ec2 = RAP_E2[p];
			if (o != 1 || kc != 0xFF)
			{
				o = kc < ec2 ? 1 : 0;
				key[p] = kc - ec2;
			}
			else if (kc == 0xFF)
			{
				key[p] = kc - ec2;
			}
			else
			{
				key[p] = kc;
			}
		}
	}

	memcpy(rif, key, 0x10);
}

int read_act_dat_and_make_rif(uint8_t *idps,uint8_t *rap, uint8_t *act_dat, char *content_id, char *out)
{	
	uint8_t idps_const[0x10]={0x5E,0x06,0xE0,0x4F,0xD9,0x4A,0x71,0xBF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
	uint8_t rif_key_const[0x10]={0xDA,0x7D,0x4B,0x5E,0x49,0x9A,0x4F,0x53,0xB1,0xC1,0xA1,0x4A,0x74,0x84,0x44,0x3B};

	uint8_t act_dat_key[0x10];
	uint64_t account_id;
	//uint8_t rifkey[0x10];

	memcpy(&account_id, act_dat+0x8,0x8);
	memcpy(act_dat_key,act_dat+0x10,0x10);

	uint8_t *rif;
	page_allocate_auto(NULL, 0x98, 0x2F, (void *)&rif);
	
	memset(rif,0,0x98);

	get_rif_key(rap, rif+0x50); //convert rap to rifkey(klicensee)
	uint8_t iv[16];
	memset(iv,0,0x10);
	
	aescbccfb_enc(idps_const,idps_const,0x10,idps,IDPS_KEYBITS,iv);
	memset(iv,0,0x10);
	
	aescbccfb_dec(act_dat_key,act_dat_key,0x10,idps_const,IDPS_KEYBITS,iv);
	memset(iv,0,0x10);
	
	aescbccfb_enc(rif+0x50,rif+0x50,0x10,act_dat_key,ACT_DAT_KEYBITS,iv);
	memset(iv,0,0x10);
	
	aescbccfb_enc(rif+0x40,rif+0x40,0x10,rif_key_const,RIF_KEYBITS,iv);
	memset(iv,0,0x10);

	uint64_t timestamp=0x000001619BF6DDCA; 
	uint32_t version_number=1;
	uint32_t license_type=0x00010002;
	uint64_t expiration_time=0;
	
	memcpy(rif, &version_number,4);
	memcpy(rif+4, &license_type,4);
	memcpy(rif+8,&account_id,8);
	memcpy(rif+0x10, content_id, 0x24);
	memcpy(rif+0x60, &timestamp, 8);
	memcpy(rif+0x68, &expiration_time,8);
	memset(rif+0x70,1,0x28); //compatible with old cfw patches too

	int fd;
	uint64_t size;
	if(cellFsOpen(out, CELL_FS_O_WRONLY|CELL_FS_O_CREAT|CELL_FS_O_TRUNC, &fd, 0666, NULL, 0)==0)
	{
		cellFsWrite(fd, rif, 0x98, &size);
		cellFsClose(fd);
	}
	page_free(NULL, (void *)rif, 0x2F);
		return 0;
}

LV2_HOOKED_FUNCTION_POSTCALL_2(void, open_path_hook, (char *path0, int mode))
{
	int syscalls_disabled = ((*(uint64_t *)MKA(syscall_table_symbol + 8 * 6)) == (*(uint64_t *)MKA(syscall_table_symbol)));

		if (syscalls_disabled && path0 && !strncmp(path0, "/dev_hdd0/game/", 15) && strstr(path0 + 15, "/EBOOT.BIN"))
		{
		// syscalls are disabled and an EBOOT.BIN is being called from hdd. Let's test it.
		char *gameid = path0 + 15;

		// flag "whitelist" id's
		int allow =
		!strncmp(gameid, "NP", 2) ||
		!strncmp(gameid, "BL", 2) ||
		!strncmp(gameid, "BC", 2) ||
		!strncmp(gameid, "KOEI3", 5) ||
		!strncmp(gameid, "KTGS3", 5) ||
		!strncmp(gameid, "MRTC0", 5) ||
		!strncmp(gameid, "ASIA0", 5) ||
		!strncmp(gameid, "_DEL_", 5) || // Fix data corruption if you uninstall game/game update/homebrew with syscall disabled # Alexander's
		!strncmp(gameid, "_INST_", 6) || // 80010006 error fix when trying to install a game update with syscall disabled. # Joonie's, Alexander's, Aldo's
		!strncmp(gameid, "GUST0", 5) ;
		;

		// flag some "blacklist" id's
		if (
			!strncmp(gameid, "BLES806", 7) || // Multiman and assorted tools are in the format BLES806**
			!strncmp(gameid, "BLJS10018", 9) || // PSNPatch Stealth (older versions were already detected as non-NP/BC/BL)
			!strncmp(gameid, "BLES08890", 9) || // PSNope by user
			!strncmp(gameid, "BLES13408", 9) || // FCEU NES Emulator
			!strncmp(gameid, "BLES01337", 9) || // Awesome File Manager
			!strncmp(gameid, "BLND00001", 9) || // dev_blind
			!strncmp(gameid, "NPEA90124", 9) //|| // SEN Enabler
			//!strcmp (path0, "/dev_bdvd/PS3_UPDATE/PS3UPDAT.PUP") //bluray disk updates
			) allow = 0;

			// test whitelist.cfg and blacklist.cfg
			if (listed(0, gameid)) // whitelist.cfg test
				allow = 1;
			if (listed(1, gameid)) // blacklist.cfg test
				allow = 0;
		
			// let's now block homebrews if the "allow" flag is false
			if (!allow)
			{
				set_patched_func_param(1, (uint64_t)crap_pants);
				return;
			}
		}

	if((strstr(path0,".rif")) && (!strncmp(path0,"/dev_hdd0/home/",14)))
	{
		CellFsStat stat;
		char content_id[0x24];
		strncpy(content_id, strrchr(path0,'/')+1, 0x24);
		char *buf;
		page_allocate_auto(NULL, 0x60, 0x2F, (void *)&buf);
		memset(buf,0,0x60);
		
		sprintf(buf,"/dev_usb000/exdata/%.36s.rap",content_id);
		int path_chk=cellFsStat(buf,&stat);
		if(path_chk!=0)
		{
			sprintf(buf,"/dev_usb001/exdata/%.36s.rap",content_id);
			path_chk=cellFsStat(buf,&stat);
		}
		if(path_chk!=0)
		{
			sprintf(buf,"/dev_hdd0/exdata/%.36s.rap",content_id);
			path_chk=cellFsStat(buf,&stat);
		}
		uint8_t is_ps2_classic = !strncmp(content_id, "2P0001-PS2U10000_00-0000111122223333", 0x24);
		uint8_t is_psp_launcher = !strncmp(content_id, "UP0001-PSPC66820_00-0000111122223333", 0x24);
		if(path_chk==0 || is_ps2_classic || is_psp_launcher)
		{
			uint8_t rap[0x10] = {0xF5, 0xDE, 0xCA, 0xBB, 0x09, 0x88, 0x4F, 0xF4, 0x02, 0xD4, 0x12, 0x3C, 0x25, 0x01, 0x71, 0xD9};
			uint8_t idps[0x10];
			char *act_path;
			page_allocate_auto(NULL, 0x60, 0x2F, (void *)&act_path);
			memset(act_path,0,0x60);
			
			char act_dat_name[0x10]={"/act.dat"};
			strncpy(act_path, path0, strrchr(path0,'/')-path0);
			strcpy(act_path+strlen(act_path),act_dat_name);
			char output[0x60];
			sprintf(output, "/%s",path0);
			DPRINTF("act_path:%s content_id:%s output:%s rap_path:%s\n",act_path,content_id,output,buf);
			
			uint8_t *act_dat;
			page_allocate_auto(NULL, 0x1038, 0x2F, (void *)&act_dat);
			int fd;
			uint64_t nread;

			if(!is_ps2_classic || !is_psp_launcher)
			{
				if(cellFsOpen(buf, CELL_FS_O_RDONLY, &fd, 0, NULL, 0)==0)
				{
					cellFsRead(fd, rap, 0x10, &nread);
					cellFsClose(fd);
				}
			}
			
			if(cellFsOpen(act_path, CELL_FS_O_RDONLY, &fd, 0, NULL, 0)==0)
			{
				cellFsRead(fd, act_dat, 0x1038, &nread);
				cellFsClose(fd);
			}
			
			page_free(NULL, (void *)act_path, 0x2F);
			page_free(NULL, (void *)buf, 0x2F);
			
			memcpy(idps,(void*)PS3MAPI_IDPS_2,0x10);
			if(nread==0x1038)
			{
				read_act_dat_and_make_rif(idps,rap,act_dat,content_id,output);
			}
			page_free(NULL, (void *)act_dat, 0x2F);
		}
		else
		{
			page_free(NULL, (void *)buf, 0x2F);
		}
	}
			
	
	if (path0[0]=='/')
	{
        char *path=path0;
        if(path[1]=='/') path++; //if(path[1]=='/') path++;
        /*if (path && ((strcmp(path, "/dev_bdvd/PS3_UPDATE/PS3UPDAT.PUP") == 0)))  // Blocks FW update from Game discs!     
        {    
            char not_update[40];
            sprintf(not_update, "/dev_bdvd/PS3_NOT_UPDATE/PS3UPDAT.PUP");
            set_patched_func_param(1, (uint64_t)not_update);
			#ifdef  DEBUG
				DPRINTF("Update from disc blocked!\n");
			#endif 
        }
		else*/ // Disabled due to the issue with multiMAN can't copy update files from discs.
		//if(path[7]=='v' || path[7]=='m')
		{
			//DPRINTF("?: [%s]\n", path);
	
			//if(path[1]=='/') DPRINTF("!!! This will usually error out!\n");//path++;
			//if(path[0]=='/')
				
			////////////////////////////////////////////////////////////////////////////////////
			// Photo_GUI integration with webMAN MOD - DeViL303 & AV                          //
			////////////////////////////////////////////////////////////////////////////////////
			if(!libft2d_access)
			{
				libft2d_access = photo_gui && !strcmp(path, "/dev_flash/sys/internal/libft2d.sprx");
			}
			else
			{
				libft2d_access = 0;

				if(!strncmp(path, "/dev_hdd0/photo/", 16))
				{
					char *photo = path + 16; size_t len = strlen(photo);
					if (len < 8) ;
					else if(!strcmp(photo + len -4, ".PNG") || !strcmp(photo + len -4, ".JPG") || !strcmp(photo + len -8, "_COV.JPG") || !strncasecmp(photo + len -8, ".iso.jpg", 8) || !strncasecmp(photo + len -8, ".iso.png", 8))
					{
						#ifdef  DEBUG
						DPRINTF("CREATING /dev_hdd0/tmp/wm_request\n");
						#endif
						int fd;
						if(cellFsOpen("/dev_hdd0/tmp/wm_request", CELL_FS_O_CREAT | CELL_FS_O_WRONLY | CELL_FS_O_TRUNC, &fd, 0666, NULL, 0) == 0)
						{
							cellFsWrite(fd, path, (len + 16), NULL);
							cellFsClose(fd);
						}
					}
				}
			}
			////////////////////////////////////////////////////////////////////////////////////

			init_mtx();
			mutex_lock(map_mtx, 0);
			#ifdef  DEBUG
				DPRINTF("open_path_hook=: mutex locked\n");
			#endif 
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
			mutex_unlock(map_mtx);
			#ifdef  DEBUG
				DPRINTF("sys_aio_copy_root=: mutex unlocked\n");
			#endif 
		}
		
		DPRINTF("open_path %s\n", path); 
	}
}

int sys_aio_copy_root(char *src, char *dst)
{
	int len;
	
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
		init_mtx();
		mutex_lock(map_mtx, 0);
		#ifdef  DEBUG
			DPRINTF("sys_aio_copy_root=: mutex locked\n");
		#endif 
		// find /dev_bdvd
		for (int i = 1; i < MAX_TABLE_ENTRIES; i++)
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
				
				#ifdef  DEBUG
					DPRINTF("AIO: root replaced by %s\n", dst);		
				#endif 	
				
				break;
			}
		}
		mutex_unlock(map_mtx);
		#ifdef  DEBUG
			DPRINTF("sys_aio_copy_root=: mutex unlocked\n");
		#endif 
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



