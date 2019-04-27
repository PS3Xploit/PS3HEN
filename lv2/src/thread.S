#include <lv2/macros.h.S>
#include <lv2/symbols.h>

LV2_FUNCTION(ppu_thread_create, ppu_thread_create_symbol)
LV2_FUNCTION(ppu_thread_exit, ppu_thread_exit_symbol)
LV2_FUNCTION(ppu_thread_join, ppu_thread_join_symbol)
LV2_FUNCTION(ppu_thread_delay, ppu_thread_delay_symbol)
#ifdef create_kernel_thread_symbol
LV2_FUNCTION(create_kernel_thread, create_kernel_thread_symbol)
LV2_FUNCTION(create_user_thread, create_user_thread_symbol)
LV2_FUNCTION(create_user_thread2, create_user_thread2_symbol)
LV2_FUNCTION(start_thread, start_thread_symbol)
LV2_FUNCTION(run_thread, run_thread_symbol)
LV2_FUNCTION(register_thread, register_thread_symbol)
LV2_FUNCTION(allocate_user_stack, allocate_user_stack_symbol)
LV2_FUNCTION(deallocate_user_stack, deallocate_user_stack_symbol)
#endif

.global get_current_thread
get_current_thread:

	mfsprg0 %r9
	ld	%r4, 0xA0(%r9)
	ld	%r3, 0x40(%r4)
	blr

.global get_current_thread_name
get_current_thread_name:

	mfsprg0 %r3
	blr
	


