#ifndef __LV1_H__
#define __LV1_H__

#include <lv1/inttypes.h>
#include <lv1/lv1call.h>

#define PS3_LPAR_ID_PME 1

#define _PS(s) (s "\0\0\0\0\0\0\0\0")
#define S2I(s) ( \
	(((u64)_PS(s)[0])<<56) | \
	(((u64)_PS(s)[1])<<48) | \
	(((u64)_PS(s)[2])<<40) | \
	(((u64)_PS(s)[3])<<32) | \
	(((u64)_PS(s)[4])<<24) | \
	(((u64)_PS(s)[5])<<16) | \
	(((u64)_PS(s)[6])<<8) | \
	(((u64)_PS(s)[7])<<0))

#define FIELD_FIRST(s, i) ((S2I(s)>>32) + (i))
#define FIELD(s, i) (S2I(s) + (i))

#endif /* __LV1_H__ */

