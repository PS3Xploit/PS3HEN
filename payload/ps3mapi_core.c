/*PS3 MANAGER API
 * Copyright (c) 2014-2015 _NzV_.
 *
 * This code is write by _NzV_ <donm7v@gmail.com>.
 * It may be used for any purpose as long as this notice remains intact on all
 * source code distributions.
 */
 
#include <lv2/lv2.h>
#include <lv2/libc.h>
#include <lv2/memory.h>
#include <lv2/patch.h>
#include <lv2/syscall.h>
#include <lv2/thread.h>
#include <lv2/modules.h>
#include <lv2/io.h>
#include <lv2/error.h>
#include <lv2/symbols.h>
#include <lv1/patch.h>

#include "ps3mapi_core.h"

//-----------------------------------------------
//CORE
//-----------------------------------------------

int ps3mapi_get_fw_type(char *fw)
{
	char tmp_fw[16];
	strcpy(tmp_fw, PS3MAPI_FW_TYPE);
	return copy_to_user(&tmp_fw, get_secure_user_ptr(fw), strlen(tmp_fw));
}

//-----------------------------------------------
//PROCESSES
//-----------------------------------------------

#define MAX_PROCESS 16

int ps3mapi_get_all_processes_pid(process_id_t *pid_list)
{
	uint32_t tmp_pid_list[MAX_PROCESS];
	uint64_t *proc_list = *(uint64_t **)MKA(TOC+process_rtoc_entry_1);	
	proc_list = *(uint64_t **)proc_list;
	proc_list = *(uint64_t **)proc_list;

	for (int i = 0; i < MAX_PROCESS; i++)
	{
		process_t process = (process_t)proc_list[1];	
		proc_list += 2;	

		if ((((uint64_t)process) & 0xFFFFFFFF00000000ULL) != MKA(0)) 
		{
			tmp_pid_list[i] = 0; 
			continue;
		}

		char *proc_name = get_process_name(process);
		if(((uint64_t)(proc_name) & 0xFF00000000000000ULL)!=MKA(0))
		{
			tmp_pid_list[i] = 0; //strlen crashing because proc name==NULL
			continue;
		}

		if ( 0 < strlen(proc_name)) 
			tmp_pid_list[i] = process->pid;	
		else 
			tmp_pid_list[i] = 0;
	}

	return copy_to_user(&tmp_pid_list, get_secure_user_ptr(pid_list), sizeof(tmp_pid_list));
}

process_t ps3mapi_internal_get_process_by_pid(process_id_t pid)
{
	uint64_t *proc_list = *(uint64_t **)MKA(TOC+process_rtoc_entry_1);	
	proc_list = *(uint64_t **)proc_list;
	proc_list = *(uint64_t **)proc_list;

	for (int i = 0; i < MAX_PROCESS; i++)
	{
		process_t p = (process_t)proc_list[1];	
		proc_list += 2;		

		if ((((uint64_t)p) & 0xFFFFFFFF00000000ULL) != MKA(0)) 
			continue;
		if (p->pid == pid) 
			return p;
	}

	return NULL;
}

int ps3mapi_get_process_name_by_pid(process_id_t pid, char *name)
{
	process_t process = ps3mapi_internal_get_process_by_pid(pid);

	if (process <= 0) 
		return ESRCH;

	char proc_name[25];
	sprintf(proc_name, "%s", get_process_name(process));
	return copy_to_user(&proc_name, get_secure_user_ptr(name),  strlen(proc_name));
}

int ps3mapi_get_process_by_pid(process_id_t pid, process_t process)
{
	uint64_t *proc_list = *(uint64_t **)MKA(TOC+process_rtoc_entry_1);	
	proc_list = *(uint64_t **)proc_list;
	proc_list = *(uint64_t **)proc_list;

	for (int i = 0; i < MAX_PROCESS; i++)
	{
		process_t p = (process_t)proc_list[1];	
		proc_list += 2;		

		if ((((uint64_t)p) & 0xFFFFFFFF00000000ULL) != MKA(0))
			continue;
		if (p->pid == pid) 
			return copy_to_user(&p, get_secure_user_ptr(process), sizeof(process_t));
	}

	return ESRCH;
}

int ps3mapi_get_current_process_critical(process_t process)
{
	suspend_intr();
	process_t p = get_current_process();
	resume_intr();

	if (p <= 0) 
		return ESRCH;
	else 
		return copy_to_user(&p, get_secure_user_ptr(process), sizeof(process_t));
}

int ps3mapi_get_current_process(process_t process)
{
	process_t p = get_current_process();

	if (p <= 0) 
		return ESRCH;
	else 
		return copy_to_user(&p, get_secure_user_ptr(process), sizeof(process_t));
}

//-----------------------------------------------
//MEMORY
//-----------------------------------------------

int ps3mapi_set_process_mem(process_id_t pid, uint64_t addr, char *buf, int size)
{
	process_t process = ps3mapi_internal_get_process_by_pid(pid);

	if (process <= 0)
		return ESRCH;
	else 
		return process_write_memory(process, (void *)addr, (void *)get_secure_user_ptr(buf), size, 1);
}

int ps3mapi_get_process_mem(process_id_t pid, uint64_t addr, char *buf, int size)
{
	void *buff;
	process_t process = ps3mapi_internal_get_process_by_pid(pid);

	if (process <= 0) 
		return ESRCH;

	buff = alloc(size, 0x27);

	if (!buff) 
		return ENOMEM;

	int ret = copy_from_process(process, (void *)addr, buff, size);
	if (ret != SUCCEEDED) {dealloc(buff, 0x27); 
		return ret;}

	ret = copy_to_user(buff, (void *)get_secure_user_ptr(buf), size);
	dealloc(buff, 0x27);
	return ret;
}

int ps3mapi_process_page_allocate(process_id_t pid, uint64_t size, uint64_t page_size, uint64_t flags, uint64_t is_executable, uint64_t *page_table)
{
	process_t process = ps3mapi_internal_get_process_by_pid(pid);

	if (process <= 0)
		return ESRCH;

	int ret;
	void *kbuf, *vbuf;

	if (page_size > 0)
	{
		ret = page_allocate(process, size, flags, page_size, &kbuf);
	}
	else
	{
		ret = page_allocate_auto(process, size, flags, &kbuf);
	}

	if (ret != SUCCEEDED)
	{
		return ENOMEM;
	}

	if (is_executable == 0)
	{
		ret = page_export_to_proc(process, kbuf, 0x40000, &vbuf);
		if (ret != SUCCEEDED)
		{
			page_free(process, kbuf, flags);
			return ENOMEM;
		}
	}
	else
	{
		uint64_t addr = MKA(mmapper_flags_temp_patch);
		*(uint32_t *)(addr) = 0x3B804004; // li r28, 0x4004
		clear_icache((void *)addr, 4);

		ret = page_export_to_proc(process, kbuf, 0x40000, &vbuf);

		if (ret != SUCCEEDED)
		{
			page_free(process, kbuf, flags);
			return ENOMEM;
		}

		*(uint32_t *)(addr) = 0x3B804000; // li r28, 0x4000
		clear_icache((void *)addr, 4);
	}

	uint64_t temp_table[2];
	temp_table[0] = (uint64_t)vbuf;
	temp_table[1] = (uint64_t)kbuf;
	ret = copy_to_user(temp_table, get_secure_user_ptr(page_table), sizeof(uint64_t)*2);

	if (ret != SUCCEEDED)
	{
		page_unexport_from_proc(process, vbuf);
		page_free(process, kbuf, flags);
		return ret;
	}
	
	if (kbuf)
	{
		page_free(process, kbuf, flags);
	}

	return SUCCEEDED;
}

int ps3mapi_process_page_free(process_id_t pid, uint64_t flags, uint64_t *page_table)
{
	process_t process = ps3mapi_internal_get_process_by_pid(pid);

	if (process <= 0)
		return ESRCH;

	uint64_t temp_table[2];
	int ret = copy_from_user(get_secure_user_ptr(page_table), temp_table, sizeof(uint64_t)*2);
	if (ret) // (ret != SUCCEEDED)
		return EINVAL;

	uint64_t *vbuf = (uint64_t*) temp_table[0];
	uint64_t *kbuf = (uint64_t*) temp_table[1];
	page_unexport_from_proc(process, vbuf);
	page_free(process, kbuf, flags);

	return SUCCEEDED;
}

//-----------------------------------------------
//MODULES
//-----------------------------------------------

#define MAX_MODULES 128


int ps3mapi_get_all_process_modules_prx_id(process_id_t pid, sys_prx_id_t *prx_id_list)
{
	process_t process = ps3mapi_internal_get_process_by_pid(pid);

	if (process <= 0) 
		return ESRCH;

	sys_prx_id_t tmp_prx_id_list[MAX_MODULES];
	sys_prx_id_t *list;
	uint32_t *unk;
	uint32_t n, unk2;

	list = alloc(MAX_MODULES*sizeof(sys_prx_module_info_t), 0x35);

	if (!list) 
		return ENOMEM;

	unk = alloc(MAX_MODULES*sizeof(uint32_t), 0x35);

	if (!unk) {dealloc(list, 0x35); 
		return ENOMEM;}
	
	int ret = prx_get_module_list(process, list, unk, MAX_MODULES, &n, &unk2);
	if (ret == SUCCEEDED)
	{
		for (int i = 0; i < MAX_MODULES; i++)
		{
			if (i < n) 
				tmp_prx_id_list[i] = list[i];
			else 
				tmp_prx_id_list[i] = 0;
		}

		ret =copy_to_user(&tmp_prx_id_list, get_secure_user_ptr(prx_id_list), sizeof(tmp_prx_id_list));
	}

	dealloc(list, 0x35);
	dealloc(unk, 0x35);
	return ret;
}

int ps3mapi_get_process_module_name_by_prx_id(process_id_t pid, sys_prx_id_t prx_id, char *name)
{
	process_t process = ps3mapi_internal_get_process_by_pid(pid);

	if (process <= 0) 
		return ESRCH;

	char *filename = alloc(256, 0x35);

	if (!filename) 
		return ENOMEM;

	sys_prx_segment_info_t *segments = alloc(sizeof(sys_prx_segment_info_t), 0x35);

	if (!segments) 
	{
		dealloc(filename, 0x35); 
		return ENOMEM;
	}

	char tmp_name[30];
	sys_prx_module_info_t modinfo;
	memset(&modinfo, 0, sizeof(sys_prx_module_info_t));
	modinfo.filename_size = 256;
	modinfo.segments_num = 1;
	int ret = prx_get_module_info(process, prx_id, &modinfo, filename, segments);

	if (ret == SUCCEEDED)
	{
		sprintf(tmp_name, "%s", modinfo.name);
		ret = copy_to_user(&tmp_name, get_secure_user_ptr(name), strlen(tmp_name));		
	}

	dealloc(filename, 0x35);
	dealloc(segments, 0x35);
	return ret;	
}

int ps3mapi_get_process_module_filename_by_prx_id(process_id_t pid, sys_prx_id_t prx_id, char *name)
{
	process_t process = ps3mapi_internal_get_process_by_pid(pid);

	if (process <= 0) 
		return ESRCH;

	char *filename = alloc(256, 0x35);

	if (!filename) 
		return ENOMEM;

	sys_prx_segment_info_t *segments = alloc(sizeof(sys_prx_segment_info_t), 0x35);

	if (!segments) {dealloc(filename, 0x35); 
		return ENOMEM;}

	char tmp_name[256];
	sys_prx_module_info_t modinfo;
	memset(&modinfo, 0, sizeof(sys_prx_module_info_t));
	modinfo.filename_size = 256;
	modinfo.segments_num = 1;
	int ret = prx_get_module_info(process, prx_id, &modinfo, filename, segments);

	if (ret == SUCCEEDED)
	{
		sprintf(tmp_name, "%s", filename);
		ret = copy_to_user(&tmp_name, get_secure_user_ptr(name), strlen(tmp_name));		
	}

	dealloc(filename, 0x35);
	dealloc(segments, 0x35);
	return ret;	
}

int ps3mapi_load_process_modules(process_id_t pid, char *path, void *arg, uint32_t arg_size)
{
	process_t process = ps3mapi_internal_get_process_by_pid(pid);

	if (process <= 0) 
		return ESRCH;

	path = get_secure_user_ptr(path);
	arg = get_secure_user_ptr(arg);
	void *kbuf, *vbuf;
	sys_prx_id_t prx;
	int ret;	

	if (arg != NULL && arg_size > KB(64))
		return EINVAL;		

	prx = prx_load_module(process, 0, 0, path);

	if (prx < 0) 
		return prx;

	if (arg && arg_size > 0)
	{	
		page_allocate_auto(process, KB(64), 0x2F, &kbuf);
		page_export_to_proc(process, kbuf, 0x40000, &vbuf);
		memcpy(kbuf, arg, arg_size);		
	}
	else 
		vbuf = NULL;

	ret = prx_start_module_with_thread(prx, process, 0, (uint64_t)vbuf);
	if (vbuf)
	{
		page_unexport_from_proc(process, vbuf);
		page_free(process, kbuf, 0x2F);
	}	

	if (ret != SUCCEEDED)
	{
		prx_stop_module_with_thread(prx, process, 0, 0);
		prx_unload_module(prx, process);
	}

	return ret;
}

int ps3mapi_unload_process_modules(process_id_t pid, sys_prx_id_t prx_id)
{
	process_t process = ps3mapi_internal_get_process_by_pid(pid);

	if (process <= 0) 
		return ESRCH;

	int ret = prx_stop_module_with_thread(prx_id, process, 0, 0);

	if (ret == SUCCEEDED) 
		ret = prx_unload_module(prx_id, process);

	return ret;
}

int ps3mapi_get_process_module_info(process_id_t pid, sys_prx_id_t prx_id, sys_prx_module_info_t *info)
{
	process_t process = ps3mapi_internal_get_process_by_pid(pid);

	if (process <= 0)
		return ESRCH;

	info = get_secure_user_ptr(info);
	sys_prx_module_info_t modinfo;

	int ret = copy_from_user(info, &modinfo, 0x48);

	if (ret != SUCCEEDED)
		return EINVAL;

	if ((modinfo.segments == 0) || (modinfo.filename == 0) || (modinfo.segments_num == 0) || (modinfo.filename_size == 0))
	{
		return EFAULT;
	}

	char *filename = alloc(modinfo.filename_size, 0x35);

	if (!filename)
		return ENOMEM;

	sys_prx_segment_info_t *segments = alloc(modinfo.segments_num * sizeof(sys_prx_segment_info_t), 0x35);

	if (!segments)
	{
		dealloc(filename, 0x35);
		return ENOMEM;
	}

	ret = prx_get_module_info(process, prx_id, &modinfo, filename, segments);

	if (ret == SUCCEEDED)
	{
		ret = copy_to_user(segments, (void *)(uintptr_t)modinfo.segments, modinfo.segments_num * sizeof(sys_prx_segment_info_t));

		if (ret != SUCCEEDED)
		{
			dealloc(segments, 0x35);
			return ret;
		}

		ret = copy_to_user(filename, (void *)(uintptr_t)modinfo.filename, modinfo.filename_size);

		if (ret != SUCCEEDED)
		{
			dealloc(filename, 0x35);
			return ret;
		}

		ret = copy_to_user(&modinfo, info, 0x48);
	}

	dealloc(filename, 0x35);
	dealloc(segments, 0x35);
	return ret;
}

//-----------------------------------------------
//THREAD
//-----------------------------------------------

int ps3mapi_create_process_thread(process_id_t pid, thread_t *thread, void *entry, uint64_t arg, int prio, size_t stacksize, char *threadname)
{
	process_t process = ps3mapi_internal_get_process_by_pid(pid);

	if (process <= 0)
		return ESRCH;

	thread = get_secure_user_ptr(thread);
	entry = get_secure_user_ptr(entry);
	threadname = get_secure_user_ptr(threadname);

	int ret;
	uint64_t exit_code;

	ret = ppu_user_thread_create(process, thread, entry, arg, prio, stacksize, PPU_THREAD_CREATE_JOINABLE, (const char *)threadname);

	if (ret != 0)
		return ret;

	ppu_thread_join(*thread, &exit_code);

	return ret;
}

//-----------------------------------------------
//SYSCALL
//-----------------------------------------------

int ps3mapi_check_syscall(int num)
{
	uint64_t sc_null = *(uint64_t *)MKA(syscall_table_symbol);
	if (*(uint64_t *)MKA(syscall_table_symbol + (8 * num)) != sc_null)
	{
		uint64_t syscall_not_impl = *(uint64_t *)sc_null;
		if ((*(uint64_t *)MKA(syscall_table_symbol + (8 * num))) != syscall_not_impl) 
			return SUCCEEDED;
	}
	return ENOSYS;
}

int ps3mapi_disable_syscall(int num)
{
	uint64_t syscall_not_impl = *(uint64_t *)MKA(syscall_table_symbol);
	*(uint64_t *)MKA(syscall_table_symbol+ 8 * num) = syscall_not_impl;
	return SUCCEEDED;
}

//-----------------------------------------------
//PSID/IDPS
//-----------------------------------------------

int ps3mapi_get_idps(uint64_t *idps)
{
	if (!PS3MAPI_IDPS_1) 
		return ESRCH;

	uint64_t idps_tmp[2];
	idps_tmp[0] = *(uint64_t *)PS3MAPI_IDPS_1;
	idps_tmp[1] = *(uint64_t *)(PS3MAPI_IDPS_1+8);
	return copy_to_user(&idps_tmp, get_secure_user_ptr(idps), sizeof(idps_tmp));	
}


int ps3mapi_set_idps(uint64_t part1, uint64_t part2)
{
	if (!PS3MAPI_IDPS_1) 
		return ESRCH;

	*(uint64_t *)PS3MAPI_IDPS_1 = part1;
	*(uint64_t *)(PS3MAPI_IDPS_1+8) = part2;

	if (!PS3MAPI_IDPS_2) 
		return ESRCH;

	*(uint64_t *)PS3MAPI_IDPS_2 = part1;
	*(uint64_t *)(PS3MAPI_IDPS_2+8) = part2;
	return SUCCEEDED;
}

int ps3mapi_get_psid(uint64_t *psid)
{
	if (!PS3MAPI_PSID) 
		return ESRCH;

	uint64_t psid_tmp[2];
	psid_tmp[0] = *(uint64_t *)PS3MAPI_PSID;
	psid_tmp[1] = *(uint64_t *)(PS3MAPI_PSID+8);
	return copy_to_user(&psid_tmp, get_secure_user_ptr(psid), sizeof(psid_tmp));	
}

int ps3mapi_set_psid(uint64_t part1, uint64_t part2)
{
	if (!PS3MAPI_PSID) 
		return ESRCH;
	
	*(uint64_t *)PS3MAPI_PSID = part1;
	*(uint64_t *)(PS3MAPI_PSID+8) = part2;
	return SUCCEEDED;
}
