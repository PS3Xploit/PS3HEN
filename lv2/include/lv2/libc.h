#ifndef __LV2_LIBC_H__
#define __LV2_LIBC_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <lv2/lv2.h>

#define isalpha(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))

LV2_EXPORT int lv2_printf(const char* format, ...) __attribute__ ((format (printf, 1, 2)));
LV2_EXPORT int lv2_sprintf(char *str, const char* format, ...) __attribute__ ((format (printf, 2, 3)));
LV2_EXPORT int lv2_snprintf(char *str, size_t size, const char *format, ...) __attribute__ ((format (printf, 3, 4)));
LV2_EXPORT char *lv2_strcpy(char *s1, const char *s2);
LV2_EXPORT char *lv2_strncpy(char *s1, const char *s2, size_t n);
LV2_EXPORT char *lv2_strcat(char *s1, const char *s2);
LV2_EXPORT char *lv2_strncat(char *s1, const char *s2, size_t n);
LV2_EXPORT size_t lv2_strlen (const char * str);
LV2_EXPORT int lv2_strcmp(const char *s1, const char *s2);
LV2_EXPORT int lv2_strncmp(const char *s1, const char *s2, size_t n);
LV2_EXPORT char *lv2_strchr(const char *s, int c);
LV2_EXPORT char *lv2_strrchr(const char *s, int c);
LV2_EXPORT void *lv2_memcpy(void *dest, const void *src, size_t n);
LV2_EXPORT void *lv2_memset(void *s, int c, size_t n);
LV2_EXPORT int lv2_memcmp(const void *s1, const void *s2, size_t n);
LV2_EXPORT void *lv2_memchr(const void *s, int c, size_t n);

//#define printf 		lv2_printf
#define sprintf 	lv2_sprintf
#define snprintf	lv2_snprintf
#define strcpy		lv2_strcpy
#define strncpy		lv2_strncpy
#define strcat		lv2_strcat
#define strncat		lv2_strncat
#define strlen		lv2_strlen
#define strcmp		lv2_strcmp
#define strncmp		lv2_strncmp
#define strchr		lv2_strchr
#define strrchr		lv2_strrchr
#define memcpy		lv2_memcpy
#define memset		lv2_memset
#define memcmp		lv2_memcmp
#define memchr		lv2_memchr



#endif /* __LV2_LIBC_H__ */
