#include <sdk_version.h>
#include <cellstatus.h>
#include <cell/cell_fs.h>
#include <cell/pad.h>

#include <sys/prx.h>
#include <sys/ppu_thread.h>
#include <sys/timer.h>
#include <sys/event.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <sys/memory.h>
#include <sys/ss_get_open_psid.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netex/net.h>
#include <netex/errno.h>




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common.h"


SYS_MODULE_INFO(TESTPLUGIN, 0, 1, 0);
SYS_MODULE_START(testplugin_start);
SYS_MODULE_STOP(testplugin_stop);

#define THREAD_NAME "testplugin_thread"
#define STOP_THREAD_NAME "testplugin_stop_thread"

static sys_ppu_thread_t thread_id=-1;
static int done = 0;

int testplugin_start(uint64_t arg);
int testplugin_stop(void);

extern int vshmain_87BB0001(int param);

static inline void _sys_ppu_thread_exit(uint64_t val)
{
	system_call_1(41, val);
}

static inline sys_prx_id_t prx_get_module_id_by_address(void *addr)
{
	system_call_1(461, (uint64_t)(uint32_t)addr);
	return (int)p1;
}

static void testplugin_thread(uint64_t arg)
{
	DPRINTF("Hello VSH\n");
	
	while (!done)
	{
		CellPadData data;
		
		if (cellPadGetData(0, &data) == CELL_PAD_OK && data.len > 0)
		{
			if ((data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] & CELL_PAD_CTRL_L3) && (data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_R2))
			{
				if (data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_CROSS)
				{				
					// power off
					vshmain_87BB0001(1);
					break;
				}
				else if (data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & CELL_PAD_CTRL_CIRCLE)
				{				
					// reboot
					vshmain_87BB0001(2);
					break;
				}
			}
		}
		
		sys_timer_usleep(70000);
	}
	
	DPRINTF("Exiting main thread!\n");	
	sys_ppu_thread_exit(0);
}

int testplugin_start(uint64_t arg)
{
	sys_ppu_thread_create(&thread_id, testplugin_thread, 0, 3000, 0x2000, SYS_PPU_THREAD_CREATE_JOINABLE, THREAD_NAME);
		
	// Exit thread using directly the syscall and not the user mode library or we will crash
	_sys_ppu_thread_exit(0);	
	return SYS_PRX_RESIDENT;
}

static void testplugin_stop_thread(uint64_t arg)
{
	DPRINTF("testplugin_stop_thread\n");
	done = 1;
	
	if (thread_id != (sys_ppu_thread_t)-1)
	{
		uint64_t exit_code;
		sys_ppu_thread_join(thread_id, &exit_code);
	}
	
	DPRINTF("Exiting stop thread!\n");
	sys_ppu_thread_exit(0);
}

static void finalize_module(void)
{
	uint64_t meminfo[5];
	
	sys_prx_id_t prx = prx_get_module_id_by_address(finalize_module);
	
	meminfo[0] = 0x28;
	meminfo[1] = 2;
	meminfo[3] = 0;
	
	system_call_3(482, prx, 0, (uint64_t)(uint32_t)meminfo);		
}

int testplugin_stop(void)
{
	sys_ppu_thread_t t;
	uint64_t exit_code;
	
	sys_ppu_thread_create(&t, testplugin_stop_thread, 0, 0, 0x2000, SYS_PPU_THREAD_CREATE_JOINABLE, STOP_THREAD_NAME);
	sys_ppu_thread_join(t, &exit_code);	
	
	finalize_module();
	_sys_ppu_thread_exit(0);
	return SYS_PRX_STOP_OK;
}
