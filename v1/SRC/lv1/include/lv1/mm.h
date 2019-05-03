
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _MM_H_
#define _MM_H_

#include <lv1/lv1.h>

#define MM_LOAD_BASE(ptr, offset)			\
	__asm__ __volatile__ (					\
		"li %0, 1\n\t"						\
		"rldicr %0, %0, 63, 0\n\t"			\
		"oris %0, %0, %1\n\t"				\
		"ori %0, %0, %2\n\t" :				\
		"=r"(ptr) :							\
		"g"(((offset) >> 16) & 0xFFFF),		\
		"g"((offset) & 0xFFFF))

#define MM_EA2VA(ea)						((ea) & ~0x8000000000000000ULL)

int mm_init(void);

int mm_insert_htab_entry(u64 va_addr, u64 lpar_addr, u64 prot);

int mm_map_lpar_memory_region(u64 lpar_start_addr, u64 ea_start_addr, u64 size,
	u64 page_shift, u64 prot);

#endif
