#ifndef __MAPPATH_H__
#define __MAPPATH_H__

#include <lv2/memory.h>

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

void unhook_all_map_path(void);

#endif /* __MAPPATH_H_ */

