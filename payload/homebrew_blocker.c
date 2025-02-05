//////////////////////////////////////////////////////////////////////////////////////
// 																				    //
//         KW - HOMEBREW BLOCKER SUPPORT CODE TO USE IN open_path_hook()            //
//            																        //
//   Functions, global vars and directives are here to improve code readability     //
//																					//
//////////////////////////////////////////////////////////////////////////////////////

#include <lv2/thread.h>
#include <lv2/io.h>
#include <lv2/error.h>
#include "homebrew_blocker.h"
#include "mappath.h"
#include "modulespatch.h"

#define BLACKLIST_FILENAME	"/dev_hdd0/tmp/blacklist.cfg"
#define WHITELIST_FILENAME	"/dev_hdd0/tmp/whitelist.cfg"

#define MAX_LIST_ENTRIES	32 // Maximum elements for noth the custom blacklist and whitelist.

int CFW2OFW_game = 0;
uint8_t allow_restore_sc = 1;
uint8_t block_psp_launcher = 0;

static int __initialized_lists = 0; // Are the lists initialized ?
static int __blacklist_entries = 0; // Module global var to hold the current blacklist entries.
static char __blacklist[9 * MAX_LIST_ENTRIES];
static int __whitelist_entries = 0; // Module global var to hold the current whitelist entries.
static char __whitelist[9 * MAX_LIST_ENTRIES];

// inits a list.
// returns the number of elements read from file
static int init_list(char *list, char *path, int maxentries)
{
	int loaded, f;
	// uncomment this to avoid hook recursivity & remappings if appropriate 1/2
	//lock_mtx(&pgui_mtx);
	if (cellFsOpen(path, CELL_FS_O_RDONLY, &f, 0, NULL, 0) != 0)
		return SUCCEEDED; // failed to open

	loaded = 0;

	while (loaded < maxentries)
	{
		char line[128];
		int eof;
		
		if (read_text_line(f, line, sizeof(line), &eof) > 0)
		if (strlen(line) >= 9) // avoid copying empty lines
		{
			strncpy(list + (9 * loaded), line, 9); // copy only the first 9 chars - if it has lees than 9, it will fail future checks. should correct in file.
			loaded++;
		}

		if (eof)
			break;
	}

	cellFsClose(f);
	// uncomment this to avoid hook recursivity & remappings if appropriate 2/2
	//unlock_mtx(&pgui_mtx);
	return loaded;
}

// tests if a char gameid[9] is in the blacklist or whitelist
// initialize the both lists, if not yet initialized;
// receives the list to test blacklist (1) or whitelist (0), and the gameid
// to initialize the lists, tries to read them from file BLACKLIST_FILENAME and WHITELIST_FILENAME
static int listed(int blacklist, char *gameid)
{
	char *list;
	int i, elements;

	if (!__initialized_lists)
	{
		// initialize the lists if not yet done
		//__blacklist=(char*)alloc(9*MAX_LIST_ENTRIES,0x27);
		//__whitelist=(char*)alloc(9*MAX_LIST_ENTRIES,0x27);

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
	{
		if (!strncmp(list + (9 * i), gameid, 9))
			return 1; // gameid is in the lists
	}

	return SUCCEEDED; // if it got here, it is not in the list. return 0
}

// BEGIN KW & AV block access to homebrews when syscalls are disabled
// After the core tests it will test first if the gameid is in whitelist.cfg (superseeds previous tests)
// In the it will test if the gameid is in blacklist.cfg (superseeds all previous tests)
// ** WARNING ** This syscall disablement test assumes that the syscall table entry 6 (peek) was replaced by the original value (equals syscall 0 entry) as done by PSNPatch
// ** WARNING ** If only a parcial disablement was made, this assumption WILL FAIL !!!
int block_homebrew(const char *path)
{
	int allow = 1;
	//uint8_t is_hdd0 = !strncmp(path, "/dev_hdd0/", 10);
	//uint8_t is_game_dir = (is_hdd0 && !strncmp(path + 10, "game/", 5));

	uint8_t is_hdd0 = !strncmp(path, "/dev_hdd0/game/", 15);

	if(is_hdd0)
	{
		// syscalls are disabled and an EBOOT.BIN is being called from hdd. Let's test it.
		char *gameid = (char *)path + 15;
		
		// block PSP Launchers if PSP umd was not set
		if (!umd_file && (!strncmp(gameid, "PSPC66820/USRDIR", 16) || !strncmp(gameid, "PSPM66820/USRDIR", 16)))
		{
			block_psp_launcher = 1;
		}
			
		int syscalls_disabled = ((*(uint64_t *)MKA(syscall_table_symbol + 8 * 6)) == (*(uint64_t *)MKA(syscall_table_symbol)));

		// CFW2OFW fix by Evilnat
		int path_len = 15 + strlen(path + 15);
		if(!strcmp(path + path_len - 8, "LIC.EDAT"))
			CFW2OFW_game = 1;

		if (syscalls_disabled && path && strstr(path + 15, "/EBOOT.BIN"))
		{
			// flag "whitelist" id's
			allow = !strncmp(gameid, "NP", 2) ||
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
			if (!strncmp(gameid, "BLES806", 7)   || // Multiman and assorted tools are in the format BLES806**
				!strncmp(gameid, "BLJS10018", 9) || // PSNPatch Stealth (older versions were already detected as non-NP/BC/BL)
				!strncmp(gameid, "BLES08890", 9) || // PSNope by user
				!strncmp(gameid, "BLES13408", 9) || // FCEU NES Emulator
				!strncmp(gameid, "BLES01337", 9) || // Awesome File Manager
				!strncmp(gameid, "BLND00001", 9) || // dev_blind
				!strncmp(gameid, "NPEA90124", 9) || // SEN Enabler
				// !strncmp(gameid, "NP0APOLLO", 9) || // Apollo Save Tool
				// !strncmp(gameid, "NP00PKGI3", 9) // // pkgi-ps3
				!strncmp(gameid, "NP0", 3) ||   // NP0APOLLO / NP00PKGI3 / NP0ANSIVW
				!strncmp(gameid, "PS3XPLOIT", 9)    // PS3Xploit Default Game Path
				// !strcmp (path, "/dev_bdvd/PS3_UPDATE/PS3UPDAT.PUP") //bluray disk updates
				) allow = 0;

			// test whitelist.cfg and blacklist.cfg
			if (listed(0, gameid)) // whitelist.cfg test
				allow = 1;
			if (listed(1, gameid)) // blacklist.cfg test
				allow = 0;
		}
	}

	return allow;
}

/*
static int check_syscalls()
{
	uint8_t syscalls_disabled = ((*(uint64_t *)MKA(syscall_table_symbol + 8 * 6)) == (*(uint64_t *)MKA(syscall_table_symbol)));

	return syscalls_disabled;
}

void restore_syscalls(const char *path)
{
	// Restore disabled CFW Syscalls without reboot just entering to Settings > System Update on XMB - aldostools
	if(allow_restore_sc)
	{
		if(!strcmp(path, "/dev_flash/vsh/module/software_update_plugin.sprx"))
		{			
			if(check_syscalls())
				create_syscalls();
		}
	}
}

void check_signin(const char *path)
{
	if(!strcmp(path, "/dev_flash/vsh/module/npsignin_plugin.sprx"))
	{
		// Lock/Unlock Sign In to PSN if DeViL303's RCO exists    
		if(check_syscalls())
			map_path(NPSIGNIN_UNLOCK, NULL, 0);
		else
		{	
			CellFsStat stat;
			if(cellFsStat(NPSIGNIN_LOCK, &stat) == SUCCEEDED)
				map_path(NPSIGNIN_UNLOCK, NPSIGNIN_LOCK, 0);
			else
				map_path(NPSIGNIN_UNLOCK, NULL, 0);
		}
	}
}
*/
