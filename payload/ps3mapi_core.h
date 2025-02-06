/*PS3 MANAGER API
 * Copyright (c) 2014 _NzV_.
 *
 * This code is write by _NzV_ <donm7v@gmail.com>.
 * It may be used for any purpose as long as this notice remains intact on all
 * source code distributions.
 */

#ifndef __PS3MAPI_H__
#define __PS3MAPI_H__

#include <lv2/process.h>
#include <lv2/modules.h>

#define SYSCALL8_OPCODE_PS3MAPI			 		0x7777

//-----------------------------------------------
//CORE
//-----------------------------------------------

#define PS3MAPI_CORE_VERSION			 		0x0124
#define PS3MAPI_CORE_MINVERSION			 		0x0111

#if defined(FIRMWARE_4_80)
	#define PS3MAPI_FW_VERSION			 		0x0480
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"CEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL
	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL
	#define PS3MAPI_PSID						0x8000000000474B0CULL
#endif
#if defined(FIRMWARE_4_81)
	#define PS3MAPI_FW_VERSION			 		0x0481
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"CEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL
	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL
	#define PS3MAPI_PSID						0x8000000000474B0CULL
#endif
#if defined(FIRMWARE_4_82)
	#define PS3MAPI_FW_VERSION			 		0x0482
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"CEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL
	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL
	#define PS3MAPI_PSID						0x8000000000474B0CULL
#endif
#if defined(FIRMWARE_4_82DEX)
	#define PS3MAPI_FW_VERSION			 		0x0482
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"DEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"DEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x8000000000409A30ULL
	#define PS3MAPI_IDPS_2			 			0x800000000049CAF4ULL
	#define PS3MAPI_PSID						0x800000000049CB0CULL
#endif
#if defined(FIRMWARE_4_83)
	#define PS3MAPI_FW_VERSION			 		0x0483
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"CEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL
	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL
	#define PS3MAPI_PSID						0x8000000000474B0CULL
#endif
#if defined(FIRMWARE_4_84)
	#define PS3MAPI_FW_VERSION			 		0x0484
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"CEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL
	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL
	#define PS3MAPI_PSID						0x8000000000474B0CULL
#endif
#if defined(FIRMWARE_4_84DEX)
	#define PS3MAPI_FW_VERSION			 		0x0484
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"DEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"DEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x8000000000409A30ULL
	#define PS3MAPI_IDPS_2			 			0x800000000049CAF4ULL
	#define PS3MAPI_PSID						0x800000000049CB0CULL
#endif
#if defined(FIRMWARE_4_85)
	#define PS3MAPI_FW_VERSION			 		0x0485
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"CEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL
	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL
	#define PS3MAPI_PSID						0x8000000000474B0CULL
#endif

#if defined(FIRMWARE_4_86)
	#define PS3MAPI_FW_VERSION			 		0x0486
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"CEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL
	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL
	#define PS3MAPI_PSID						0x8000000000474B0CULL
#endif

#if defined(FIRMWARE_4_87)
	#define PS3MAPI_FW_VERSION			 		0x0487
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"CEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL
	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL
	#define PS3MAPI_PSID						0x8000000000474B0CULL
#endif

#if defined(FIRMWARE_4_88)
	#define PS3MAPI_FW_VERSION			 		0x0488
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"CEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL
	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL
	#define PS3MAPI_PSID						0x8000000000474B0CULL
#endif

#if defined(FIRMWARE_4_89)
	#define PS3MAPI_FW_VERSION			 		0x0489
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"CEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL
	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL
	#define PS3MAPI_PSID						0x8000000000474B0CULL
#endif

#if defined(FIRMWARE_4_90)
	#define PS3MAPI_FW_VERSION			 		0x0490
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"CEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL
	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL
	#define PS3MAPI_PSID						0x8000000000474B0CULL
#endif

#if defined(FIRMWARE_4_91)
	#define PS3MAPI_FW_VERSION			 		0x0491
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"CEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL
	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL
	#define PS3MAPI_PSID						0x8000000000474B0CULL
#endif

#if defined(FIRMWARE_4_92)
	#define PS3MAPI_FW_VERSION			 		0x0492
	#if defined(IS_MAMBA)
		#define PS3MAPI_FW_TYPE			 		"CEX MAMBA"
	#else
		#define PS3MAPI_FW_TYPE			 		"CEX COBRA"
	#endif
	#define PS3MAPI_IDPS_1			 			0x80000000003E2E30ULL
	#define PS3MAPI_IDPS_2			 			0x8000000000474AF4ULL
	#define PS3MAPI_PSID						0x8000000000474B0CULL
#endif

#define PS3MAPI_OPCODE_GET_CORE_VERSION			0x0011
#define PS3MAPI_OPCODE_GET_CORE_MINVERSION		0x0012
#define PS3MAPI_OPCODE_GET_FW_TYPE				0x0013
#define PS3MAPI_OPCODE_GET_FW_VERSION			0x0014

int ps3mapi_get_fw_type(char *fw);

//-----------------------------------------------
//PROCESSES
//-----------------------------------------------

#define PS3MAPI_OPCODE_GET_ALL_PROC_PID			0x0021
#define PS3MAPI_OPCODE_GET_PROC_NAME_BY_PID		0x0022
#define PS3MAPI_OPCODE_GET_PROC_BY_PID			0x0023
#define PS3MAPI_OPCODE_GET_CURRENT_PROC			0x0024
#define PS3MAPI_OPCODE_GET_CURRENT_PROC_CRIT	0x0025

typedef uint32_t process_id_t;

int ps3mapi_get_all_processes_pid(process_id_t *pid_list);
int ps3mapi_get_process_name_by_pid(process_id_t pid, char *name);
int ps3mapi_get_process_by_pid(process_id_t pid, process_t process);
int ps3mapi_process_kill_by_pid(process_id_t pid);
int ps3mapi_get_current_process_critical(process_t process);
int ps3mapi_get_current_process(process_t process);

//-----------------------------------------------
//MEMORY
//-----------------------------------------------

#define PS3MAPI_OPCODE_GET_PROC_MEM				0x0031
#define PS3MAPI_OPCODE_SET_PROC_MEM				0x0032
#define PS3MAPI_OPCODE_PROC_PAGE_ALLOCATE		0x0033
#define PS3MAPI_OPCODE_PROC_PAGE_FREE			0x0034

int ps3mapi_set_process_mem(process_id_t pid, uint64_t addr, char *buf, int size);
int ps3mapi_get_process_mem(process_id_t pid, uint64_t addr, char *buf, int size);
int ps3mapi_process_page_allocate(process_id_t pid, uint64_t size, uint64_t page_size, uint64_t flags, uint64_t is_executable, uint64_t *page_table);
int ps3mapi_process_page_free(process_id_t pid, uint64_t flags, uint64_t *page_table);

//-----------------------------------------------
//MODULES
//-----------------------------------------------

#define PS3MAPI_OPCODE_GET_ALL_PROC_MODULE_PID		0x0041
#define PS3MAPI_OPCODE_GET_PROC_MODULE_NAME			0x0042
#define PS3MAPI_OPCODE_GET_PROC_MODULE_FILENAME		0x0043
#define PS3MAPI_OPCODE_LOAD_PROC_MODULE				0x0044
#define PS3MAPI_OPCODE_UNLOAD_PROC_MODULE			0x0045
#define PS3MAPI_OPCODE_UNLOAD_VSH_PLUGIN			0x0046 //Look in modulespatch.c for code.
#define PS3MAPI_OPCODE_GET_VSH_PLUGIN_INFO			0x0047 //Look in modulespatch.c for code.
#define PS3MAPI_OPCODE_GET_PROC_MODULE_INFO			0x0048

int ps3mapi_get_all_process_modules_prx_id(process_id_t pid, sys_prx_id_t *prx_id_list);
int ps3mapi_get_process_module_name_by_prx_id(process_id_t pid, sys_prx_id_t prx_id, char *name);
int ps3mapi_get_process_module_filename_by_prx_id(process_id_t pid, sys_prx_id_t prx_id, char *name);
int ps3mapi_load_process_modules(process_id_t pid, char *path, void *arg, uint32_t arg_size);
int ps3mapi_unload_process_modules(process_id_t pid, sys_prx_id_t prx_id);
int ps3mapi_get_process_module_info(process_id_t pid, sys_prx_id_t prx_id, sys_prx_module_info_t *info);

//-----------------------------------------------
//THREAD
//-----------------------------------------------

#define SYSCALL8_OPCODE_PROC_CREATE_THREAD			0x6E03 // not eough params for PS3MAPI_OPCODE

int ps3mapi_create_process_thread(process_id_t pid, thread_t *thread, void *entry, uint64_t arg, int prio, size_t stacksize, char *threadname);

//-----------------------------------------------
//SYSCALL
//-----------------------------------------------

#define SYSCALL8_OPCODE_STEALTH_TEST			0x3993 //KW PSNPatch stealth extension compatibility
#define SYSCALL8_OPCODE_STEALTH_ACTIVATE    	0x3995 //KW PSNPatch stealth extension compatibility
#define SYSCALL8_STEALTH_OK						0x5555 //KW PSNPatch stealth extension compatibility
#define PS3MAPI_OPCODE_CHECK_SYSCALL			0x0091
#define PS3MAPI_OPCODE_DISABLE_SYSCALL			0x0092
#define PS3MAPI_OPCODE_PDISABLE_SYSCALL8 		0x0093 //Look in main.c for code.
#define PS3MAPI_OPCODE_PCHECK_SYSCALL8 			0x0094 //Look in main.c for code.

int ps3mapi_check_syscall(int num);
int ps3mapi_disable_syscall(int num);

//-----------------------------------------------
//PEEK POKE
//-----------------------------------------------

#define PS3MAPI_OPCODE_SUPPORT_SC8_PEEK_POKE	0x1000
#define PS3MAPI_OPCODE_LV2_PEEK					0x1006
#define PS3MAPI_OPCODE_LV2_POKE					0x1007
#define PS3MAPI_OPCODE_LV1_PEEK					0x1008
#define PS3MAPI_OPCODE_LV1_POKE					0x1009

#define PS3MAPI_OPCODE_SUPPORT_SC8_PEEK_POKE_OK	0x6789

//-----------------------------------------------
//SECURITY
//-----------------------------------------------

#define PS3MAPI_OPCODE_SET_ACCESS_KEY			0x2000
#define PS3MAPI_OPCODE_REQUEST_ACCESS			0x2001

//-----------------------------------------------
//REMOVE COBRA/MAMBA HOOK
//-----------------------------------------------

#define PS3MAPI_OPCODE_REMOVE_HOOK			0x0101 //Look in main.c for code.

//-----------------------------------------------
//PSID/IDPS
//-----------------------------------------------

#define PS3MAPI_OPCODE_GET_IDPS 		0x0081
#define PS3MAPI_OPCODE_SET_IDPS 		0x0082
#define PS3MAPI_OPCODE_GET_PSID 		0x0083
#define PS3MAPI_OPCODE_SET_PSID			0x0084

//-----------------------------------------------
//MISC
//-----------------------------------------------
#define PS3MAPI_OPCODE_PHOTO_GUI		0x2222

int ps3mapi_get_idps(uint64_t *idps);
int ps3mapi_set_idps(uint64_t part1, uint64_t part2);
int ps3mapi_get_psid(uint64_t *psid);
int ps3mapi_set_psid(uint64_t part1, uint64_t part2);

#endif /* __PS3MAPI_H__ */
