#ifndef __LV2_IO_H__
#define __LV2_IO_H__

#include <stdint.h>
#include <sys/types.h>
#include <lv2/lv2.h>
#include <lv2/symbols.h>

#define CELL_FS_O_CREAT		000100
#define CELL_FS_O_EXCL		000200
#define CELL_FS_O_TRUNC		001000
#define CELL_FS_O_APPEND	002000
#define CELL_FS_O_ACCMODE	000003
#define CELL_FS_O_RDONLY	000000
#define CELL_FS_O_RDWR		000002
#define	CELL_FS_O_WRONLY	000001
#define CELL_FS_O_MSELF		010000

#ifndef SEEK_SET
#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif
#ifndef SEEK_END
#define SEEK_END 2
#endif

#undef MAX_PATH
#define MAX_PATH	0x420

typedef int CellFsMode;
#define CELL_FS_S_IFMT 	0170000		/* type of file */
#define CELL_FS_S_IFDIR	0040000		/* directory */
#define CELL_FS_S_IFREG	0100000		/* regular */
#define CELL_FS_S_IFLNK 0120000		/* symbolic link */
#define CELL_FS_S_IFWHT 0160000		/* unknown */
#define CELL_FS_S_IRWXU 0000700		/* RWX mask for owner */
#define CELL_FS_S_IRUSR 0000400		/* R for owner */
#define CELL_FS_S_IWUSR 0000200		/* W for owner */
#define CELL_FS_S_IXUSR 0000100		/* X for owner */
#define CELL_FS_S_IRWXG 0000070		/* RWX mask for group */
#define CELL_FS_S_IRGRP 0000040		/* R for group */
#define CELL_FS_S_IWGRP 0000020		/* W for group */
#define CELL_FS_S_IXGRP 0000010		/* X for group */
#define CELL_FS_S_IRWXO 0000007		/* RWX mask for other */
#define	CELL_FS_S_IROTH 0000004		/* R for other */
#define	CELL_FS_S_IWOTH 0000002		/* W for other */
#define	CELL_FS_S_IXOTH 0000001		/* X for other */

#ifndef time_t
#define time_t int64_t
#endif


typedef struct 
{
	CellFsMode st_mode;
	int st_uid;
	int st_gid;
	time_t st_atime;
	time_t st_mtime;
	time_t st_ctime;
	uint64_t st_size;
	uint64_t st_blksize;
	uint64_t dummy[1];
} __attribute__((__packed__)) CellFsStat;

typedef struct 
{
	time_t actime;
	time_t modtime;
} __attribute__((packed)) CellFsUtimbuf;

typedef struct
{
	void *dummy;
} *fs_object_handle_t;

/* Internal */

LV2_EXPORT int cellFsUnlink_internal(void *structure, const char *path, uint64_t unk);

/* End of internal */

LV2_EXPORT int cellFsOpen(const char *path, int flags, int *fd, int mode, void *arg, uint64_t size);
LV2_EXPORT int cellFsClose(int fd);
LV2_EXPORT int cellFsRead(int fd, void *buf, uint64_t nbytes, uint64_t *nread);
LV2_EXPORT int cellFsWrite(int fd, void *buf, uint64_t nbytes, uint64_t *nwrite);
LV2_EXPORT int cellFsLseek(int fd, int64_t offset, int whence, uint64_t *pos);
LV2_EXPORT int cellFsStat(const char *path, CellFsStat *sb);
LV2_EXPORT int cellFsMkdir(const char *path, CellFsMode mode);
LV2_EXPORT int cellFsUtime(const char *path, CellFsUtimbuf *timep);

#ifdef io_sub_rtoc_entry_1

LV2_EXPORT int cellFsRename_internal(void *structure, const char *from, const char *to, uint64_t unk);

static INLINE int cellFsRename(const char *from, const char *to)
{
	uint64_t *structure = (uint64_t *) *(uint64_t *)MKA(TOC+io_rtoc_entry_1);
	structure = (uint64_t *)structure[io_sub_rtoc_entry_1];
	return cellFsRename_internal(structure, from, to, 0);
}

static INLINE int cellFsUnlink(const char *path)
{
	uint64_t *structure = (uint64_t *) *(uint64_t *)MKA(TOC+io_rtoc_entry_1);
	structure = (uint64_t *)structure[io_sub_rtoc_entry_1];
	return cellFsUnlink_internal(structure, path, 0);
}

#endif

LV2_EXPORT int cellFsUtilMount(const char *block_dev, const char *filesystem, const char *mount_point, int unk, int read_only, int unk2, char *argv[], int argc);
// Version for when the function is hooked:
int cellFsUtilMount_h(const char *block_dev, const char *filesystem, const char *mount_point, int unk, int read_only, int unk2, char *argv[], int argc);

LV2_EXPORT int cellFsUtilUmount(const char *mount_point, uint64_t unk, int force);
// cellFsUtilNewfs("CELL_FS_UTILITY:HDD1", "CELL_FS_FAT", 0, NULL, 0);
LV2_EXPORT int cellFsUtilNewfs(const char *block_dev, const char *filesystem, int unk, char *argv[], int argc);

LV2_EXPORT int pathdup_from_user(char *path, char **out);

LV2_EXPORT int open_fs_object(void *unk, char *path, void **fs_object, void **unkret1, fs_object_handle_t *handle, void *unk2);
LV2_EXPORT int close_fs_object(void *unk, fs_object_handle_t handle);

LV2_EXPORT int get_path_by_fd(int fd, char *path);

LV2_EXPORT char* path_clean(char* path);

#endif /* __LV2_IO_H__ */

