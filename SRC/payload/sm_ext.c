#include <lv2/lv2.h>
#include <lv2/process.h>
#include <lv2/memory.h>
#include <lv2/patch.h>
#include <lv2/syscall.h>
#include "common.h"
#include "sm_ext.h"
//#include "modulespatch.h"

static int patch_done = 0;
static int is_reboot = 0;
process_t vsh_process;

LV2_SYSCALL2(int, send_poweroff_event, (uint64_t *event_data1, uint64_t *event_data2, uint64_t *event_data3, uint32_t unk))
{
	f_desc_t f;
	f.addr=(void*)MKA(0xAB48);
	f.toc=(void*)MKA(TOC);
	int(*func)(uint64_t *,uint64_t *, uint64_t *,uint32_t)=(void*)&f;
	process_t p = get_current_process_critical();
	
	if (p == vsh_process)
	{
		uint64_t data1, data2, data3;
		
		if (is_reboot)
		{
			data1 = 5;
			data2 = 2;
		}
		else
		{
			data1 = 3;
			data2 = 0;
		}
		
		data3 = 0;		
		
		copy_to_user(&data1, get_secure_user_ptr(event_data1), sizeof(data1));
		copy_to_user(&data2, get_secure_user_ptr(event_data2), sizeof(data2));
		copy_to_user(&data3, get_secure_user_ptr(event_data3), sizeof(data3));
				
//		unhook_function_with_cond_postcall(get_syscall_address(SYS_SM_GET_EXT_EVENT2), send_poweroff_event, 4);
		*(uint64_t *)0x80000000003647B0=0x8000000000303C58;
		
		patch_done = 0;
	}
	else	
		return func(event_data1,event_data2,event_data3,unk);	
	
	return 0;
}

int sys_sm_ext_send_poweroff_event(int reboot)
{
	is_reboot = reboot;
	
	if (!patch_done)
	{
		create_syscall2(378, send_poweroff_event);
		patch_done = 1;
	}
	
	return 0;
}
