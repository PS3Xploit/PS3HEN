/*@{*/
#ifndef __LV2_ERROR_H__
#define __LV2_ERROR_H__

/* error code definition */

#ifdef __cplusplus
extern "C" {
#endif

/** The operation is successfully executed.*/

#define CELL_FS_SUCCEEDED  0
#define SUCCEEDED    0
#define CELL_OK	     0
#define UNDEFINED         -1
#define FAILED            -1

/** The resource is temporarily unavailable.
 *
 * e.g. The number of threads in the system is exceeding the limit.
 */
#define EAGAIN		-2147418111	/* 0x80010001 */

/** An invalid argument value is specified.
 *
 * e.g. An out-of-range argument or an invalid flag.
 */
#define EINVAL		-2147418110	/* 0x80010002 */

/** The feature is not yet implemented. */
#define ENOSYS		-2147418109	/* 0x80010003 */

/** Memory allocation failure */
#define ENOMEM		-2147418108	/* 0x80010004 */

/** The resource (process, thread, synchronous object, etc) with the
 * specified identifier does not exist.
 */
#define ESRCH       -2147418107	/* 0x80010005 */

/** The file does not exist. */
#define ENOENT		-2147418106	/* 0x80010006 */

/**  The file is not a valid ELF file. (The file is in unrecognized format.) */
#define ENOEXEC		-2147418105	/* 0x80010007 */

/** Resource deadlock is avoided. */
#define EDEADLK     -2147418104	/* 0x80010008 */

/** The operation is not permitted. */
#define EPERM       -2147418103	/* 0x80010009 */

/** The device or resource is busy */
#define EBUSY		-2147418102	/* 0x8001000A */

/** The operation is timed out */
#define ETIMEDOUT	-2147418101	/* 0x8001000B */

/** The operation is aborted */
#define EABORT		-2147418100	/* 0x8001000C */

/** Invalid memory access */
#define EFAULT		-2147418099	/* 0x8001000D */


/** State of the target thread is invalid. */
#define ESTAT       -2147418097	/* 0x8001000F */

/** Alignment is invalid. */
#define EALIGN      -2147418096	/* 0x80010010 */

/** Shortage of the kernel resources */
#define EKRESOURCE  -2147418095	/* 0x80010011 */

/** The file is a directory */
#define EISDIR      -2147418094	/* 0x80010012 */

/** Operation canceled */
#define ECANCELED   -2147418093	/* 0x80010013 */

/** Entry already exists */
#define EEXIST      -2147418092	/* 0x80010014 */

/** Port is already connected */
#define EISCONN     -2147418091	/* 0x80010015 */

/** Port is not connected */
#define ENOTCONN    -2147418090	/* 0x80010016 */

/** Program authentication fail */
#define EAUTHFAIL   -2147418089	/* 0x80010017 */

/** The file is not a MSELF */
#define ENOTMSELF   -2147418088	/* 0x80010018 */

/** System version error */
#define ESYSVER     -2147418087	/* 0x80010019 */

/** Fatal system error */
#define EAUTHFATAL  -2147418086	/* 0x8001001A */

#define EDOM          -2147418085	/* 0x8001001B */
#define ERANGE        -2147418084	/* 0x8001001C */
#define EILSEQ        -2147418083	/* 0x8001001D */
#define EFPOS         -2147418082	/* 0x8001001E */
#define EINTR         -2147418081	/* 0x8001001F */
#define EFBIG         -2147418080	/* 0x80010020 */
#define EMLINK        -2147418079	/* 0x80010021 */
#define ENFILE        -2147418078	/* 0x80010022 */
#define ENOSPC        -2147418077	/* 0x80010023 */
#define ENOTTY        -2147418076	/* 0x80010024 */
#define EPIPE         -2147418075	/* 0x80010025 */
#define EROFS         -2147418074	/* 0x80010026 */
#define ESPIPE        -2147418073	/* 0x80010027 */
#define E2BIG         -2147418072	/* 0x80010028 */
#define EACCES        -2147418071	/* 0x80010029 */
#define EBADF         -2147418070	/* 0x8001002A */
#define EIO           -2147418069	/* 0x8001002B */
#define EMFILE        -2147418068	/* 0x8001002C */
#define ENODEV        -2147418067	/* 0x8001002D */
#define ENOTDIR       -2147418066	/* 0x8001002E */
#define ENXIO         -2147418065	/* 0x8001002F */
#define EXDEV         -2147418064	/* 0x80010030 */
#define EBADMSG       -2147418063	/* 0x80010031 */
#define EINPROGRESS   -2147418062	/* 0x80010032 */
#define EMSGSIZE      -2147418061	/* 0x80010033 */
#define ENAMETOOLONG  -2147418060	/* 0x80010034 */
#define ENOLCK        -2147418059	/* 0x80010035 */
#define ENOTEMPTY     -2147418058	/* 0x80010036 */
#define ENOTSUP       -2147418057	/* 0x80010037 */
#define EFSSPECIFIC   -2147418056	/* 0x80010038 */
#define EOVERFLOW     -2147418055	/* 0x80010039 */
#define ENOTMOUNTED   -2147418054	/* 0x8001003A */
#define ENOTSDATA     -2147418053	/* 0x8001003B */

/** SDK version error */
#define ESDKVER       -2147418052	/* 0x8001003C */
#define ENOLICDISC    -2147418051	/* 0x8001003D */
#define ENOLICENT     -2147418050	/* 0x8001003E */

#ifdef __cplusplus
}
#endif
#endif							/* __LV2_KERNEL_ERROR_CODE_H__ */
/*@}*/

