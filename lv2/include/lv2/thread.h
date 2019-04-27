#ifndef __LV2_THREAD_H__
#define __LV2_THREAD_H__

#include <stdint.h>
#include <lv2/lv2.h>
#include <lv2/interrupt.h>
#include <lv2/process.h>

#define	PPU_THREAD_CREATE_JOINABLE	0x0000000000000001
#define	PPU_THREAD_CREATE_INTERRUPT	0x0000000000000002

typedef struct
{
	void *unk_0; // ptr to some funcs
	uint64_t unk_8;
	uint32_t unk_10;
	uint32_t unk_14;
	void *unk_18;
	void *unk_20; // same as unk_18? :S
	uint64_t unk_28[3];
	void *unk_40; // same as unk_0?
	// ...
} *thread_t;

typedef uint64_t sys_ppu_thread_t;

LV2_EXPORT int ppu_thread_create(thread_t *thread, void *entry, uint64_t arg, int prio, uint64_t stacksize, uint64_t flags, const char *threadname);
LV2_EXPORT int ppu_thread_exit(uint64_t val);
LV2_EXPORT int ppu_thread_join(thread_t thread, uint64_t *vptr);

LV2_EXPORT int ppu_thread_delay(thread_t, uint64_t usecs, int unk, int unk2);

thread_t get_current_thread(void);
static INLINE int timer_usleep(uint64_t usecs) { return ppu_thread_delay(get_current_thread(), usecs, 0, 1); }

int ppu_user_thread_create(process_t process, thread_t *thread, void *entry, uint64_t arg, int prio, uint64_t stacksize, uint64_t flags, const char *threadname);

#endif /* __LV2_THREAD_H__ */
