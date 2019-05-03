#include <lv2/lv2.h>
#include <lv2/modules.h>
#include <lv2/thread.h>

int prx_get_module_name_by_address(process_t process, void *addr, char *name)
{
	sys_prx_module_info_t modinfo;
	sys_prx_id_t id = prx_get_module_id_by_address(process, addr);
	
	if (id < 0)
		return id;
	
	memset(&modinfo, 0, sizeof(modinfo));
	int ret = prx_get_module_info(process, id, &modinfo, NULL, NULL);
	
	if (ret < 0)
		return ret;
	
	strncpy(name, modinfo.name, 30);
	return 0;
}

int prx_start_module_with_thread(sys_prx_id_t id, process_t process, uint64_t flags, uint64_t arg)
{
	int ret;
	uint64_t meminfo[5];
	uint32_t toc[2];
	thread_t thread;
	uint64_t exit_code;
	
	meminfo[0] = sizeof(meminfo);
	meminfo[1] = 1;
	
	ret = prx_start_module(id, process, flags, meminfo);
	if (ret != 0)
		return ret;	
	
	ret = copy_from_process(process, (void *)meminfo[2], toc, sizeof(toc));
	if (ret != 0)
		return ret;
	
	ret = ppu_user_thread_create(process, &thread, toc, arg, 0, 0x1000, PPU_THREAD_CREATE_JOINABLE, "");
	if (ret != 0)
		return ret;
	
	ppu_thread_join(thread, &exit_code);
	meminfo[1] = 2;
	meminfo[3] = 0;
	
	return prx_start_module(id, process, flags, meminfo);
}

int prx_stop_module_with_thread(sys_prx_id_t id, process_t process, uint64_t flags, uint64_t arg)
{
	int ret;
	uint64_t meminfo[5];
	uint32_t toc[2];
	thread_t thread;
	uint64_t exit_code;
	
	meminfo[0] = sizeof(meminfo);
	meminfo[1] = 1;
	
	ret = prx_stop_module(id, process, flags, meminfo);
	if (ret != 0)
		return ret;	
	
	ret = copy_from_process(process, (void *)meminfo[2], toc, sizeof(toc));
	if (ret != 0)
		return ret;
	
	ret = ppu_user_thread_create(process, &thread, toc, arg, 0, 0x1000, PPU_THREAD_CREATE_JOINABLE, "");
	if (ret != 0)
		return ret;
	
	return ppu_thread_join(thread, &exit_code);	
}




