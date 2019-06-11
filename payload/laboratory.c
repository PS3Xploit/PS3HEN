#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lv2/lv2.h>
#include <lv2/libc.h>
#include <lv2/memory.h>
#include <lv2/patch.h>
#include <lv2/syscall.h>
#include <lv2/usb.h>
#include <lv2/storage.h>
#include <lv2/thread.h>
#include <lv2/synchronization.h>
#include <lv2/modules.h>
#include <lv2/io.h>
#include <lv2/time.h>
#include <lv2/security.h>
#include <lv2/pad.h>
#include <lv2/error.h>
#include <lv2/symbols.h>
#include <lv1/stor.h>
#include <lv1/patch.h>

#include "common.h"

static void dump_threads_info(void)
{
	uint8_t *thread_info = (uint8_t *)MKA(thread_info_symbol);
	int num_threads;
	
	num_threads = *(uint32_t *)&thread_info[8];
	
	for (int i = 0; i < num_threads; i++)
	{
		DPRINTF("Thread: %s   entry: %016lx    PC: %016lx\n", (char *)(thread_info+0x58), *(uint64_t *)(thread_info+0xB0), *(uint64_t *)(thread_info+0x208));
		thread_info += 0x600;
	}
}

static void dump_threads_info_test(uint64_t arg0)
{	
	DPRINTF("Threads info will be dumped in 13 seconds.\n");
	timer_usleep(SECONDS(13));
	
	DPRINTF("----Dump threads info begin----\n");
	dump_threads_info();
	DPRINTF("----Dump threads info end----\n");
	
	ppu_thread_exit(0);
}

void do_dump_threads_info_test(void)
{
	thread_t my_thread;
	ppu_thread_create(&my_thread, dump_threads_info_test, 0, -0x1D8, 0x4000, 0, "Dump Threads");		
}

static void dump_process(process_t process)
{
	char path[0x420];
	void *buf;
	uint64_t addr;
	int ret, fd;
	
	DPRINTF("Dumping process %s\n", get_process_name(process));
	sprintf(path, "/dev_usb000/%s.main_segment", get_process_name(process));
	
	buf = alloc(KB(64), 0x27);
	if (!buf)
	{
		DPRINTF("Not enough memory.\n");
		return;
	}
	
	ret = cellFsOpen(path, CELL_FS_O_WRONLY|CELL_FS_O_CREAT|CELL_FS_O_TRUNC, &fd, 0666, NULL, 0);
	if (ret != 0)
	{
		DPRINTF("Cannot create %s\n", path);
		return;
	}
	
	for (addr = 0x10000; ; addr += KB(64))
	{
		uint64_t written;
		
		ret = copy_from_process(process, (void *)addr, buf, KB(64));
		if (ret != 0)
		{
			DPRINTF("End of maing segment: %lx\n", addr);
			break;
		}
		
		cellFsWrite(fd, buf, KB(64), &written);
	}
	
	cellFsClose(fd);
	
	sprintf(path, "/dev_usb000/%s.heap_segment", get_process_name(process));
	
	ret = cellFsOpen(path, CELL_FS_O_WRONLY|CELL_FS_O_CREAT|CELL_FS_O_TRUNC, &fd, 0666, NULL, 0);
	if (ret != 0)
	{
		DPRINTF("Cannot create %s\n", path);
		return;
	}
	
	for (addr = 0x30000000; ; addr += KB(64))
	{
		uint64_t written;
		
		ret = copy_from_process(process, (void *)addr, buf, KB(64));
		if (ret != 0)
		{
			DPRINTF("End of heap segment: %lx\n", addr);
			break;
		}
		
		cellFsWrite(fd, buf, KB(64), &written);
	}
	
	cellFsClose(fd);
	dealloc(buf, 0x27);
}

static void dump_processes(void)
{
	uint64_t *proc_list = *(uint64_t **)MKA(TOC+process_rtoc_entry_1);
	
	proc_list = *(uint64_t **)proc_list;
	proc_list = *(uint64_t **)proc_list;	
		
	for (int i = 0; i < 0x10; i++)
	{
		process_t process = (process_t)proc_list[1];
		
		proc_list += 2;		
		DPRINTF("%p\n", process);
		
		if ((((uint64_t)process) & 0xFFFFFFFF00000000ULL) != MKA(0))
			continue;
		
		dump_process(process);	
	}	
}

static void dump_processes_test(uint64_t arg0)
{	
	DPRINTF("Processes will be dumped in 74 seconds.\nGo, go, run what you want to dump!\n");
	timer_usleep(SECONDS(74));
	
	DPRINTF("----Dump processes begin----\n");
	dump_processes();
	DPRINTF("----Dump processes end----\n");
	
	ppu_thread_exit(0);
}

void do_dump_processes_test(void)
{
	thread_t my_thread;
	ppu_thread_create(&my_thread, dump_processes_test, 0, -0x1D8, 0x4000, 0, "Dump Proc Thread");		
}

LV2_HOOKED_FUNCTION_POSTCALL_6(void, load_module_hook, (char *r3))
{
	DPRINTF("Load module: %s (%s)\n", r3, get_process_name(get_current_process())+8);	
}

void do_hook_load_module(void)
{
	hook_function_with_postcall(get_syscall_address(_SYS_PRX_LOAD_MODULE_ON_MEMCONTAINER ), load_module_hook, 6);	
}

LV2_HOOKED_FUNCTION_POSTCALL_2(void, mutex_create_hooked, (void *mutex, uint64_t blah))
{
	process_t process = get_current_process();
	//char *name = get_process_name(process)+8;	
	if (1)
	{
		static char name[32];
		uint64_t r3 = (uint64_t)mutex;
		
		if (r3 > 0xb90000 && r3 < 0x100000000ULL && prx_get_module_name_by_address(process, mutex, name) == 0)
		{
			/*DPRINTF("Addr: %lx\n", r3);
			DPRINTF("Module: %s\n", name);
			DPRINTF("Call address: %p\n", get_call_address(1));*/
			
			if (strcmp(name, "sceEmulatorApi") == 0)
			{
				DPRINTF("Addr: %lx\n", r3);
				DPRINTF("Module: %s\n", name);
				DPRINTF("Call address: %p\n", get_call_address(1));
			}
		}
	}
		
}

void do_hook_mutex_create(void)
{
	hook_function_with_postcall(get_syscall_address(SYS_MUTEX_CREATE), mutex_create_hooked, 2);
}

static void read_and_clear_ps2netemu_log(void)
{
	int fd;
	uint64_t dummy;
	
	if (cellFsOpen("/dev_hdd0/log_file.bin", CELL_FS_O_RDONLY, &fd, 0, NULL, 0) != 0)
	{
		DPRINTF("Open log file failed\n");
		return;
	}
	
	uint8_t *buf;
			
	if (page_allocate_auto(NULL, 0x10000, 0x2F, (void **)&buf) != 0)
	{
		DPRINTF("page_allocate failed\n");
		return;
	}
			
	memset(buf, 0, 0x10000);
	
	cellFsRead(fd, buf, 0x10000, &dummy);
	cellFsClose(fd);
	
	DPRINTF("%s\n", buf);
	
	if (cellFsOpen("/dev_hdd0/log_file.bin", CELL_FS_O_WRONLY|CELL_FS_O_CREAT|CELL_FS_O_TRUNC, &fd, 0666, NULL, 0) != 0)
	{
		DPRINTF("open log write failed\n");
		return;
	}
	
	memset(buf, 0, 0x10000);
	cellFsWrite(fd, buf, 0x10000, &dummy);
	cellFsClose(fd);
	
	page_free(NULL, buf, 0x2F);
}

static void ps2net_copy_test(uint64_t arg0)
{
	DPRINTF("+++ Sleeping for 16 seconds\n");
	timer_usleep(SECONDS(16));
	read_and_clear_ps2netemu_log();
	DPRINTF("Attempting to write ps2_netemu.self\n");
	
	if (cellFsUtilMount_h("CELL_FS_IOS:BUILTIN_FLSH1", "CELL_FS_FAT", "/dev_wflash", 0, 0, 0, NULL, 0) != 0)
	{
		DPRINTF("Mount R/W failed\n");
		ppu_thread_exit(0);
	}
	
	int src, dst;
	uint64_t rw;
	
	if (cellFsOpen("/dev_usb000/ps2_netemu.self", CELL_FS_O_RDONLY, &src, 0, NULL, 0) != 0)
	{
		DPRINTF("open Src read failed\n");
		goto umount_exit;
	}
	
	if (cellFsOpen("/dev_wflash/ps2emu/ps2_netemu.self", CELL_FS_O_WRONLY|CELL_FS_O_CREAT|CELL_FS_O_TRUNC, &dst, 0666, NULL, 0) != 0)
	{
		DPRINTF("open dst write failed\n");
		cellFsClose(src);
		goto umount_exit;
	}
	
	uint8_t *buf;
			
	if (page_allocate_auto(NULL, 0x10000, 0x2F, (void **)&buf) != 0)
	{
		DPRINTF("page_allocate failed\n");
		cellFsClose(src);
		cellFsClose(dst);
		goto umount_exit;
	}
			
	memset(buf, 0, 0x10000);
	
	DPRINTF("Enter copy loop\n");
	uint64_t total = 0;
	
	while (1)
	{
		if (cellFsRead(src, buf, 0x10000, &rw) != 0)
		{
			DPRINTF("cellFsRead failed\n");
			cellFsClose(src);
			cellFsClose(dst);
			page_free(NULL, buf, 0x2F);
			goto umount_exit;
		}
		
		if (cellFsWrite(dst, buf, rw, &rw) != 0)
		{
			DPRINTF("cellFsWrite failed\n");
			cellFsClose(src);
			cellFsClose(dst);
			page_free(NULL, buf, 0x2F);
			goto umount_exit;
		}
		
		total += rw;
		
		if (rw < 0x10000)
			break;
	}
	
	cellFsClose(src);
	cellFsClose(dst);
	page_free(NULL, buf, 0x2F);
	
	DPRINTF("++++ copy finished (%ld) (%ld KB)\n", total, total / 1024);
	
umount_exit:

	if (cellFsUtilUmount("/dev_wflash", 0, 0) != 0)
		cellFsUtilUmount("/dev_wflash", 0, 1);
	
	ppu_thread_exit(0);
}

void do_ps2net_copy_test(void)
{
	thread_t my_thread;
	ppu_thread_create(&my_thread, ps2net_copy_test, 0, -0x1D8, 0x4000, 0, "Ps2 net copy");
}

#define SPRX_NUM 128

static void dump_process_modules_info(process_t process)
{
	sys_prx_id_t *list;
	uint32_t *unk;
	uint32_t n, unk2;
	
	DPRINTF("******** %s ********\n", get_process_name(process));
	
	list = alloc(SPRX_NUM*sizeof(sys_prx_module_info_t), 0x35);
	unk = alloc(SPRX_NUM*sizeof(uint32_t), 0x35);
	
	if (prx_get_module_list(process, list, unk, SPRX_NUM, &n, &unk2) == 0)
	{
		char *filename = alloc(256, 0x35);
		sys_prx_segment_info_t *segments = alloc(sizeof(sys_prx_segment_info_t), 0x35);
				
		for (int i = 0; i < n; i++)
		{
			sys_prx_module_info_t modinfo;
			
			memset(&modinfo, 0, sizeof(sys_prx_module_info_t));
			modinfo.filename_size = 256;
			modinfo.segments_num = 1;
			
			if (prx_get_module_info(process, list[i], &modinfo, filename, segments) == 0)			
				DPRINTF("Module %s\nText_addr:%08lX\n", filename, segments[0].base);			
		}
		
		dealloc(filename, 0x35);
		dealloc(segments, 0x35);
	}
	
	DPRINTF("****************\n");
	
	dealloc(list, 0x35);
	dealloc(unk, 0x35);
}

static void dump_processes_modules_info(void)
{
	uint64_t *proc_list = *(uint64_t **)MKA(TOC+process_rtoc_entry_1);
	
	proc_list = *(uint64_t **)proc_list;
	proc_list = *(uint64_t **)proc_list;	
		
	for (int i = 0; i < 0x10; i++)
	{
		process_t process = (process_t)proc_list[1];
		
		proc_list += 2;		
				
		if ((((uint64_t)process) & 0xFFFFFFFF00000000ULL) != MKA(0))
			continue;
		
		dump_process_modules_info(process);	
	}	
}

static void dump_modules_info_test(uint64_t arg0)
{	
	DPRINTF("Modules info will be dumped in 71 seconds.\n");
	timer_usleep(SECONDS(71));
	
	dump_processes_modules_info();	
	ppu_thread_exit(0);
}

void do_dump_modules_info_test(void)
{
	thread_t my_thread;
	ppu_thread_create(&my_thread, dump_modules_info_test, 0, -0x1D8, 0x4000, 0, "Dump Modules Info");		
}


static void pad_test(uint64_t arg0)
{
	int error_shown = 0;
	pad_data data, odata;
	
	memset(&data, 0, sizeof(pad_data));
	memset(&odata, 0, sizeof(pad_data));
	odata.button[PAD_BTN_OFFSET_ANALOG_LEFT] = 0x8080;
	odata.button[PAD_BTN_OFFSET_ANALOG_RIGHT] = 0x8080;
	
	DPRINTF("PAD TEST: press R2+triangle to leave.\n");
	
	while (1)
	{
		int len = pad_get_data(&data);
		
		if (len < 0)
		{
			if (!error_shown)
			{
				DPRINTF("PAD err %x\n", len);
				error_shown = 1;
			}
		}
		else		
			error_shown = 0;		
		
		if (len >= ((PAD_BTN_OFFSET_DIGITAL+1)*2))
		{
			uint16_t digital = data.button[PAD_BTN_OFFSET_DIGITAL] & ~odata.button[PAD_BTN_OFFSET_DIGITAL];
			
			odata.button[PAD_BTN_OFFSET_DIGITAL] = data.button[PAD_BTN_OFFSET_DIGITAL];
			
			if (digital & PAD_CTRL_LEFT)			
				DPRINTF("LEFT pressed\n");			
			
			if (digital & PAD_CTRL_RIGHT)			
				DPRINTF("RIGHT pressed\n");			
			
			if (digital & PAD_CTRL_UP)			
				DPRINTF("UP pressed\n");			
			
			if (digital & PAD_CTRL_DOWN)			
				DPRINTF("DOWN pressed\n");			
			
			if (digital & PAD_CTRL_START)			
				DPRINTF("START pressed\n");			
			
			if (digital & PAD_CTRL_SELECT)			
				DPRINTF("SELECT pressed\n");			
			
			if (digital & PAD_CTRL_SQUARE)			
				DPRINTF("SQUARE pressed\n");			
			
			if (digital & PAD_CTRL_TRIANGLE)			
				DPRINTF("TRIANGLE pressed\n");			
			
			if (digital & PAD_CTRL_CROSS)			
				DPRINTF("CROSS pressed\n");			
			
			if (digital & PAD_CTRL_CIRCLE)			
				DPRINTF("CIRCLE pressed\n");			
			
			if (digital & PAD_CTRL_L1)			
				DPRINTF("L1 pressed\n");			
			
			if (digital & PAD_CTRL_R1)			
				DPRINTF("R1 pressed\n");			
			
			if (digital & PAD_CTRL_L2)			
				DPRINTF("L2 pressed\n");			
			
			if (digital & PAD_CTRL_R2)			
				DPRINTF("R2 pressed\n");			
			
			if (digital & PAD_CTRL_L3)			
				DPRINTF("L3 pressed\n");			
			
			if (digital & PAD_CTRL_R3)			
				DPRINTF("R3 pressed\n");			
			
			if ((data.button[PAD_BTN_OFFSET_DIGITAL] & (PAD_CTRL_R2|PAD_CTRL_TRIANGLE)) == (PAD_CTRL_R2|PAD_CTRL_TRIANGLE))			
				break;			
		}
		
		if (len >= ((PAD_BTN_OFFSET_PS+1)*2))
		{
			uint16_t ps = data.button[PAD_BTN_OFFSET_PS] & ~odata.button[PAD_BTN_OFFSET_PS];
			
			odata.button[PAD_BTN_OFFSET_PS] = data.button[PAD_BTN_OFFSET_PS];
			
			if (ps & PAD_CTRL_PS)			
				DPRINTF("PS pressed\n");			
		}
		
		if (len >= ((PAD_BTN_OFFSET_ANALOG_LEFT+1)*2))
		{			
			uint16_t current = data.button[PAD_BTN_OFFSET_ANALOG_LEFT];
			uint16_t old = odata.button[PAD_BTN_OFFSET_ANALOG_LEFT];
			
			odata.button[PAD_BTN_OFFSET_ANALOG_LEFT] = current;
			
			if (PAD_CTRL_ANALOG_X(current) < 0x40 && PAD_CTRL_ANALOG_X(old) >= 0x40)			
				DPRINTF("ANALOG L LEFT pressed\n");			
			
			if (PAD_CTRL_ANALOG_X(current) > 0xC0 && PAD_CTRL_ANALOG_X(old) <= 0xC0)			
				DPRINTF("ANALOG L RIGHT pressed\n");			
			
			if (PAD_CTRL_ANALOG_Y(current) < 0x40 && PAD_CTRL_ANALOG_Y(old) >= 0x40)			
				DPRINTF("ANALOG L UP pressed\n");			
			
			if (PAD_CTRL_ANALOG_Y(current) > 0xC0 && PAD_CTRL_ANALOG_Y(old) <= 0xC0)			
				DPRINTF("ANALOG L DOWN pressed\n");					
		}
		
		if (len >= ((PAD_BTN_OFFSET_ANALOG_RIGHT+1)*2))
		{			
			uint16_t current = data.button[PAD_BTN_OFFSET_ANALOG_RIGHT];
			uint16_t old = odata.button[PAD_BTN_OFFSET_ANALOG_RIGHT];
			
			odata.button[PAD_BTN_OFFSET_ANALOG_RIGHT] = current;
			
			if (PAD_CTRL_ANALOG_X(current) < 0x40 && PAD_CTRL_ANALOG_X(old) >= 0x40)			
				DPRINTF("ANALOG R LEFT pressed\n");			
			
			if (PAD_CTRL_ANALOG_X(current) > 0xC0 && PAD_CTRL_ANALOG_X(old) <= 0xC0)			
				DPRINTF("ANALOG R RIGHT pressed\n");			
			
			if (PAD_CTRL_ANALOG_Y(current) < 0x40 && PAD_CTRL_ANALOG_Y(old) >= 0x40)			
				DPRINTF("ANALOG R UP pressed\n");			
			
			if (PAD_CTRL_ANALOG_Y(current) > 0xC0 && PAD_CTRL_ANALOG_Y(old) <= 0xC0)			
				DPRINTF("ANALOG R DOWN pressed\n");					
		}
				
		timer_usleep(50000);
	}
	
	DPRINTF("PAD test end\n");
	
	ppu_thread_exit(0);
}

void do_pad_test(void)
{
	thread_t my_thread;
	ppu_thread_create(&my_thread, pad_test, 0, -0x1D8, 0x4000, 0, "Pad Test");
}

