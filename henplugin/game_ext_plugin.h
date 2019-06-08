// Mysis game_ext_plugin.h v0.1
typedef struct
{
	int (*LoadPage)(void);                    // 0 Parameter: (creates page_game_main widget)
	int (*DoUnk1)(void *, void *);            // 2 Parameter: uint8_t [0x270], uint8_t[0x10?]
	int (*DoUnk2)(char *, int, int);          // 3 Parameter: char * titleid[0x10], 1, 0 - (displays a message?) - link.xml related?
	int (*DoUnk3)(char *, int);               // 2 Parameter: char * (max.0x80bytes), 1 - (displays a message?) - link.xml related?
	int (*DoUnk4)(char *, char *, int, int);  // 4 Parameter: char *, char *, int, int - calls interface 2
	int (*DoUnk5)(char *, int, char *, int);  // 4 Parameter: char *, int, char *, int- calls interface 2
	int (*DoUnk6)(void *);                    // 1 Parameter: uint8 [0x5B0]
	int (*DoUnk7)(int,int,void*);             // 3 Parameter: uint8*,int,uint8* - ps2bootparam.dat / factory_process_comp_1x
	int (*DoUnk8)(void);                      // 0 Parameter: returns uint8
	int (*DoUnk9)(int, char*, void*,void*);   // 4 Parameter: ignored, char* message, uint8[0x270],{0: Type,+4: 0x10: char * unk  - Install "/dev_bdvd/PS3_GAME/PKGDIR/PKG../INSTALL.PKG" ?
	int (*DoUnk10)(int , void* , void * );    // 3 Parameter:
	int (*DoUnk11)(int, int, void*, void*);   // 4 Parameter: int value (=0 -> npsignin), ignored, uint8 [0x1B4]{message, pkg_path,..},uint8 [0x25]
	int (*DoUnk12)(void*);                    // 1 Parameter: uint8 [0x220] - auth_plugin if 5 (0x102,handler,0,0) / msg_only_for_psp_need_copy_ask
	int (*installTheme)(char*, char*);        // 2 Parameter: char * path, char * message - Theme Install/Copy ? (type: bdc, category: TI, CD_%u.p3t)
	int (*DoUnk14)(char*);                    // 1 Parameter: char * path? - page_update_history
	int (*DoUnk15)(void);                     // 0 Parameter: disables 2 second sleep on ps2bootparam.dat
	int (*DoUnk16)(char*);                    // 1 Parameter: strncpy char * [0x20] (TitleId/ContentId?)
	int (*DoUnk17)(char*);                    // 1 Parameter: char * [4] (set Category, example: "AT")
	int (*DoUnk18)(void);                     // 0 Parameter: returns uint8
	int (*DoUnk19)(void*);                    // 1 Parameter: uint8 * output
	int (*DoUnk20)(char*,int, char*);         // 3 Parameter: char * [0x80], wchar?, char* paramsfo?_dir[0x30]
	int (*DoUnk21)(char*,int,char*);          // 3 Parameter: char * titleid?[0x20], int value (2 = /dev_hdd0/game), char*[0x30]
	int (*DoUnk22)(char*,int,char*);          // 3 Parameter: char * [0x80], int?, char * [0x30]- update database+reload category game?
	int (*DoUnk23)(int,char*,int);            // 3 Parameter: int?, char * [0x38], int?
	int (*DoUnk24)(int,int);                  // 2 Parameter: int?, int?
	int (*DoUnk25)(void*);                    // 1 Parameter: uint8 [0xA4]
	int (*DoUnk26)(void*);                    // 1 Parameter: uint8 [0xA4]
	int (*DoUnk27)(void*);                    // 1 Parameter: uint8 [0x2470]
	int (*DoUnk28)(int*,char*);               // 2 Parameter: int* out, char * titleid[0x10]
	int (*DoUnk29)(void*,char*);              // 2 Parameter: void* callback, char*? - 0x2BC0 bytes, autodownload_plugin
	int (*DoUnk30)(void);                     // 0 Parameter: set autodownload skip
	int (*DoUnk31)(void);                     // 0 Parameter: returns uint8
	int (*DoUnk32)(void*,char*,void*,int);    // 4 Parameter: uint8 [0x270], char * [0x800], uint64 [2], uint8
	int (*DoUnk33)(char*);                    // 1 Parameter: char * titleid? [0x10] - msg auto update? - psn+/login checked.
	int (*installPKG)(char*);                 // 1 Parameter: char * pkg_path[0x400] (example: /dev_usb006/test.pkg)
} game_ext_plugin_interface;

game_ext_plugin_interface * game_ext_interface;
