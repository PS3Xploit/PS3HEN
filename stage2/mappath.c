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

#define MAX_TABLE_ENTRIES 16

typedef struct _MapEntry
{
	char *oldpath;
	char *newpath;
	int  newpath_len;
	uint32_t flags;	
} MapEntry;

MapEntry map_table[MAX_TABLE_ENTRIES];

// TODO: map_path and open_path_hook should be mutexed...

uint8_t photo_gui = 1;

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
static char __blacklist[9*MAX_LIST_ENTRIES];
static int __whitelist_entries = 0; // Module global var to hold the current whitelist entries.
static char __whitelist[9*MAX_LIST_ENTRIES];


//
// init_list()
//
// inits a list.
// returns the number of elements read from file

static int init_list(char *list, char *path, int maxentries)
	{
		int loaded, f;

		if (cellFsOpen(path, CELL_FS_O_RDONLY, &f, 0, NULL, 0) != 0)
			return 0; // failed to open
		
		loaded = 0;

		while (loaded < maxentries)
		{
			char line[128];
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
			// initialize the lists if not yet done
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
				const char new_path[12] = "/no_exists";
				set_patched_func_param(1, (uint64_t)new_path);
				return;
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
				
				#ifdef  DEBUG
					DPRINTF("AIO: root replaced by %s\n", dst);		
				#endif 	
				
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



