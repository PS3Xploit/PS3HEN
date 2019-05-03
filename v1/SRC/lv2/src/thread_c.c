#include <lv2/lv2.h>
#include <lv2/symbols.h>
#include <lv2/error.h>
#include <lv2/thread.h>
#include <lv2/process.h>
#include <lv2/memory.h>

LV2_EXPORT thread_t create_user_thread(void *unk, void *entry, uint64_t arg, uint64_t unk2, uint32_t stack, uint64_t stacksize, process_t process, uint64_t flags, void *unk3, const char *name);
LV2_EXPORT thread_t create_user_thread2(void *unk, void *entry, uint64_t arg, uint64_t unk2, uint32_t stack, uint64_t stacksize, process_t process, uint64_t flags, const char *name);
LV2_EXPORT int start_thread(thread_t thread, int priority, uint64_t unk);
LV2_EXPORT int register_thread(process_t process, thread_t thread, uint64_t unk);
LV2_EXPORT int run_thread(thread_t thread);
LV2_EXPORT uint32_t allocate_user_stack(process_t process, uint64_t *stacksize);
LV2_EXPORT void deallocate_user_stack(process_t process, uint32_t stack, uint64_t stacksize);

int ppu_user_thread_create(process_t process, thread_t *thread, void *entry, uint64_t arg, int prio, uint64_t stacksize, uint64_t flags, const char *threadname)
{
	uint64_t *spr0;
	uint32_t user_stack;	
	uint64_t translated_flags;
			
	user_stack = allocate_user_stack(process, &stacksize);			
	if (user_stack == 0)	
		return ENOMEM;	
	
	if (flags & PPU_THREAD_CREATE_JOINABLE)	
		translated_flags = 0x10003;	
	else	
		translated_flags = 0x10001;	
	
	if (flags & PPU_THREAD_CREATE_INTERRUPT)	
		translated_flags |= 0x4000;	
	
	spr0 = mfsprg0();	
	*thread = create_user_thread2((void *)spr0[0x48/8], entry, arg, 200, user_stack, stacksize, process, translated_flags, threadname);
	
	if (!(thread))
	{
		deallocate_user_stack(process, user_stack, stacksize);
		return EAGAIN;
	}
	
	start_thread(*thread, prio+0x300, 0);
	register_thread(process, *thread, 0);
	
	if (!(flags & PPU_THREAD_CREATE_INTERRUPT))	
		run_thread(*thread);		
	
	return 0;
}
