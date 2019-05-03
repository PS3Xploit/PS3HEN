#ifndef __LV2_TIME_H__
#define __LV2_TIME_H__

#include <lv2/lv2.h>

#define SECONDS(s) 	   (s*1000000)
#define MILISECONDS(s) (s*1000)

#ifndef time_t
#define time_t int64_t
#endif

static INLINE uint64_t get_ticks(void)
{
	unsigned int tbl, tbu0, tbu1;
	do 
	{
		__asm__ __volatile__( "mftbu %0":"=r"( tbu0 ) );
		__asm__ __volatile__( "mftb %0":"=r"( tbl ) );
		__asm__ __volatile__( "mftbu %0":"=r"( tbu1 ) );
	} while (tbu0 != tbu1);
	
	return (((uint64_t) tbu0 ) << 32) | tbl;
}

time_t get_time_seconds(void);

#endif /* __LV2_TIME_H__ */



