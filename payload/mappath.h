#ifndef __MAPPATH_H__
#define __MAPPATH_H__

#include <lv2/memory.h>

#define SYS_MAP_PATH	35
//#define SYS_MAP_GAME	36

#define MAX_TABLE_SIZE 0x80000 //512kb

#define FLAG_COPY						1			//internal use
#define FLAG_TABLE						1 << 1		//internal use
#define FLAG_PROTECT					1 << 2		//internal or external use
#define FLAG_MAX_PRIORITY				1 << 3		//internal or external use
#define ACTDAT_PATH 		"/dev_hdd0/home/%08d/exdata/act.dat"
#define ACCOUNTID_VALUE		"/setting/user/%s/npaccount/accountid"

#define IDPS_KEYBITS 		128
#define ACT_DAT_KEYBITS 	128
#define RIF_KEYBITS 		128
#define RAP_KEYBITS 		128

#define ALLOC_ACT_DAT	 	(uint8_t*)(make_rif_buf)
#define ALLOC_CONTENT_ID	(char*)(make_rif_buf + 0x20)
#define ALLOC_PATH_BUFFER	(char*)(make_rif_buf + 0x20 + 0x28)
#define ALLOC_RIF_BUFFER 	(uint8_t*)(make_rif_buf + 0x20 + 0x28)

extern uint8_t skip_existing_rif;

uint8_t make_rif_buf[0x20 + 0x28 + 0x50 + 0x20 + 0x28]; // ACT_DAT[0x20] + CONTENT_ID[0x28] + RAP_PATH[0x50] + RIF_BUFFER[0x20] (rif_buffer reuse rap_path + 0x20 = 0x70) + 0x28(signaturs)

int create_act_dat(const char *userid);
void make_rif(const char *path);

uint32_t getMapTableLength();
uint64_t getMapTableByteSize();
void 	printMappingList();
void	map_path_patches(int syscall);
//int 	unmap_path(char *oldpath);
int  	map_path(char *oldpath, char *newpath, uint32_t flags);
int 	map_path_user(char *oldpath, char *newpath, uint32_t flags);
int 	map_path_user2(char *oldpath, char *newpath, uint32_t flags);
int 	sys_map_path(char *oldpath, char *newpath, uint32_t flags);
int 	sys_map_paths(char *paths[], char *new_paths[], uint32_t num);
int 	get_map_path(uint32_t num, char *path, char *new_path);
int 	sys_aio_copy_root(char *src, char *dst);
void 	unhook_all_map_path(void);

#endif /* __MAPPATH_H_ */
