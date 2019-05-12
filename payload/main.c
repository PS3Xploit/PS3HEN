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
#include <lv2/error.h>
#include <lv2/symbols.h>
#include <lv1/stor.h>
#include <lv1/patch.h>
#include "common.h"
#include "syscall8.h"
//#include "cobra.h"
#include "modulespatch.h"
#include "mappath.h"
#include "storage_ext.h"
#include "region.h"
#include "config.h"
#include "sm_ext.h"
#include "laboratory.h"
#include "ps3mapi_core.h"

// Format of version:
// byte 0, 7 MS bits -> reserved
// byte 0, 1 LS bit -> 1 = CFW version, 0 = OFW/exploit version
// byte 1 and 2 -> ps3 fw version in BCD e.g 3.55 = 03 55. For legacy reasons, 00 00 means 3.41
// byte 3 is cobra firmware version,
// 1 = version 1.0-1.2,
// 2 = 2.0,
// 3 = 3.0
// 4 = 3.1
// 5 = 3.2
// 6 = 3.3
// 7 = 4.0
// 8 = 4.1
// 9 = 4.2
// A = 4.3
// B = 4.4
// C = 5.0
// D = 5.1
// E = 6.0
// F = 7.0

#define CB_LOCATION "/dev_habib/rebug/cobra/stage2.cex"

#define COBRA_VERSION		0x0F
#define COBRA_VERSION_BCD	0x0810
#define HEN_REV				0x0200

#if defined(FIRMWARE_4_84)
	#define FIRMWARE_VERSION	0x0484
#endif

#if defined(CFW)
	#define IS_CFW			1
#else
	#define IS_CFW			0
#endif

#define MAKE_VERSION(cobra, fw, type) ((cobra&0xFF) | ((fw&0xffff)<<8) | ((type&0x1)<<24))

typedef struct
{
	uint32_t address;
	uint32_t data;
} Patch;

static Patch kernel_patches[] =
{
	{ patch_data1_offset, 0x01000000 },
	{ patch_func8_offset1, LI(R3, 0) }, // force lv2open return 0

	// disable calls in lv2open to lv1_send_event_locally which makes the system crash
	{ patch_func8_offset2, NOP },
	{ patch_func9_offset, NOP }, // 4.30 - watch: additional call after

	// psjailbreak, PL3, etc destroy this function to copy their code there.
	// We don't need that, but let's dummy the function just in case that patch is really necessary
	{ mem_base2, LI(R3, 1) },
	{ mem_base2 + 4, BLR },

	// sys_sm_shutdown, for ps2 let's pass to copy_from_user a fourth parameter
	{ shutdown_patch_offset, MR(R6, R31) },
	{ module_sdk_version_patch_offset, NOP },

	// User thread prio hack (needed for netiso)
	{ user_thread_prio_patch, NOP },
	{ user_thread_prio_patch2, NOP },

	// ODE Protection removal (needed for CFW 4.60+) 
	{ lic_patch, LI(R3, 1) },
	{ ode_patch, LI(R3, 0) },
	{ ode_patch + 4, STD(R3, 0, R9) },
};

#define N_KERNEL_PATCHES	(sizeof(kernel_patches) / sizeof(Patch))

int inst_and_run_kernel(uint8_t *payload, int size)
{
	if((!size) || (size>0x10000))
		return -1;
	
	if(!payload)
		return -2;
	
	memcpy((void *)0x80000000007f0000, get_secure_user_ptr(payload), size);

	f_desc_t f;
	int (* func)(void);
	f.addr = (void *)0x80000000007f0000;
	f.toc = (void *)MKA(TOC);
	func = (void *)&f;
	func();
	return 0;
}

int inst_and_run_kernel_dynamic(uint8_t *payload, int size, uint64_t *residence)
{
	if(!size)
		return -1;
	
	if(!payload)
		return -2;
	
	void *skprx=alloc(size, 0x27);
	if(skprx)
	{
		memcpy(skprx, get_secure_user_ptr(payload), size);

		f_desc_t f;
		int (* func)(void);
		f.addr = skprx;
		f.toc = (void *)MKA(TOC);
		func = (void *)&f;
		func();
		uint64_t resident=(uint64_t)skprx;
		copy_to_user(&resident, get_secure_user_ptr(residence), 8);
		return 1;
	}
	
	return 0;
}

int unload_plugin_kernel(uint64_t residence)
{
	dealloc((void *)residence, 0x27);
	return 0;
}

f_desc_t extended_syscall8;

int64_t syscall8(uint64_t function, uint64_t param1, uint64_t param2, uint64_t param3, uint64_t param4, uint64_t param5, uint64_t param6, uint64_t param7);

LV2_SYSCALL2(uint64_t, sys_cfw_peek, (uint64_t *addr))
{
	/* if (block_peek)
		return (uint64_t)ENOSYS; */

	//DPRINTF("peek %p\n", addr);

	uint64_t ret = *addr;

	// Fix compatibilty issue with prx loader. It searches for a string... that is also in this payload, and then lv2_peek((vsh_str + 0x70)) crashes the system.
	if (ret == 0x5F6D61696E5F7673)
	{
		extern uint64_t _start;
		extern uint64_t __self_end;

		if ((uint64_t)addr >= (uint64_t)&_start && (uint64_t)addr < (uint64_t)&__self_end)
		{
			DPRINTF("peek to addr %p blocked for compatibility.\n", addr);
			return 0;
		}
	}

	return ret;
}

static void *current_813;

void _sys_cfw_poke(uint64_t *addr, uint64_t value);

LV2_HOOKED_FUNCTION(void, sys_cfw_new_poke, (uint64_t *addr, uint64_t value))
{
	//DPRINTF("New poke called\n");

	_sys_cfw_poke(addr, value);
	asm volatile("icbi 0,%0; isync" :: "r"(addr));
}

LV2_HOOKED_FUNCTION(void *, sys_cfw_memcpy, (void *dst, void *src, uint64_t len))
{
	DPRINTF("sys_cfw_memcpy: %p %p 0x%lx\n", dst, src, len);

	if (len == 8)
	{
		_sys_cfw_poke(dst, *(uint64_t *)src);
		return dst;
	}

	return memcpy(dst, src, len);
}

LV2_SYSCALL2(void, sys_cfw_poke, (uint64_t *ptr, uint64_t value))
{
	DPRINTF("LV2 poke %p %016lx\n", ptr, value);
	uint64_t addr=(uint64_t)ptr;
	if(addr>0x8000000000352230)
	{
		*ptr=value;
		return;
	}
}

LV2_SYSCALL2(void, sys_cfw_lv1_poke, (uint64_t lv1_addr, uint64_t lv1_value))
{
	DPRINTF("LV1 poke %p %016lx\n", (void*)lv1_addr, lv1_value);	
	lv1_poked(lv1_addr, lv1_value);
}

LV2_SYSCALL2(void, sys_cfw_lv1_call, (uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t num))
{
	/* DO NOT modify */
	asm("mflr 0\n");
	asm("std 0, 16(1)\n");
	asm("mr 11, 10\n");
	asm("sc 1\n");
	asm("ld 0, 16(1)\n");
	asm("mtlr 0\n");
	asm("blr\n");
}

LV2_SYSCALL2(void, sys_cfw_lv2_func, (uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t num))
{
	/* DO NOT modify */
	asm("mflr 0\n");
	asm("std 0, 16(1)\n");
	asm("mr 0, 10\n");
	asm("mtctr 0\n");
	asm("bctrl\n");
	asm("ld 0, 16(1)\n");
	asm("mtlr 0\n");
	asm("blr\n");
}

static INLINE int sys_get_version(uint32_t *version)
{
	uint32_t pv = MAKE_VERSION(COBRA_VERSION, FIRMWARE_VERSION, IS_CFW);
	return copy_to_user(&pv, get_secure_user_ptr(version), sizeof(uint32_t));
}

static INLINE int sys_get_version2(uint16_t *version)
{
	uint16_t cb = COBRA_VERSION_BCD;
	return copy_to_user(&cb, get_secure_user_ptr(version), sizeof(uint16_t));
}


/*
LV2_HOOKED_FUNCTION(uint64_t, sys_cfw_storage_send_device_command, (uint32_t device_handle, unsigned int command, void indata, uint64_t inlen, void outdata, uint64_t outlen))
{
 DPRINTF("sys_storage_send_device_command\n");

 sys_storage_send_device_command(device_handle, command, indata, inlen, outdata, outlen);
 int64_t debug_print(const char* buffer, size_t size);
 void debug_print_hex(void *buf, uint64_t size);
 void debug_print_hex_c(void *buf, uint64_t size);

}
*/

void do_patch(uint64_t addr, uint64_t patch)
{
	*(uint64_t *)addr=patch;
	clear_icache((void *)addr, 8);
}


void do_patch32(uint64_t addr, uint32_t patch)
{
	*(uint32_t *)addr=patch;
	clear_icache((void *)addr, 4);
}


LV2_HOOKED_FUNCTION_COND_POSTCALL_2(uint64_t, support_advanced_flags, (uint64_t *r3, uint64_t r4))
{
	f_desc_t f;
	f.addr=(void *)MKA(0x4ef1c);
	f.toc=(void *)MKA(TOC);
	uint64_t (*func)(uint64_t *, uint64_t)=(void *)&f;
	if(get_call_address(1)==(void *)0x8000000000059db0)
	{
		func(r3,r4);
		*(uint64_t *)(r4+8)=0; //ecdsa flag
		return 0;
	}
	do_patch32(0x800000000005a6f8,0x419e00ac);
	do_patch32(0x80000000000564b0,0x7FE307B4);
	do_patch32(0x8000000000056614,0x48216FB5);
	do_patch32(0x80000000000203fc,0x419DFF84);
	do_patch32(0x8000000000020408,0x419D0258);
	
	
	do_patch32(0x800000000005a6e4,0x419e00ac);
	do_patch32(0x8000000000059dc4,0x7FE307B4);
	do_patch32(0x8000000000056230,0x48240EED);
	do_patch(0x80000000002275f4,0xE86900007C6307B4);
	do_patch(0x80000000002d2b34,0xF821FE617CE802A6);
//	do_patch(0x8000000000003d90,0xF821FEB17C0802A6);
	do_patch(0x800000000005658C,0x63FF003D419EFFD4);
	do_patch(0x8000000000056650,0x3FE0800163FF003E);
	do_patch(0x8000000000056604,0x2F840004409C0048);

	*(uint64_t *)0x8000000000474A80=0;
	unhook_all_modules();
	unhook_function_with_cond_postcall(0x4ef1c, support_advanced_flags, 2);
	
	unhook_all_storage_ext();
	unhook_all_region();
	unhook_all_map_path();

	uint64_t ret=0;
	
	ret=func(r3,r4); //this function calls appldr and triggers boom!

	
	do_patch32(0x80000000000564b0,0x38600000); // patch_func8_offset1 
	do_patch32(0x8000000000056614,0x60000000); // patch_func8_offset2 
	do_patch32(0x80000000000203fc,0x60000000); // user_thread_prio_patch	for netiso
	do_patch32(0x8000000000020408,0x60000000); // user_thread_prio_patch2	net iso
	do_patch32(0x8000000000059dc4,0x38600000);// patch_func2_offset (modules_patching)
	do_patch32(0x8000000000056230,0x38600001); // ignore LIC.DAT check	
	do_patch32(0x800000000005a6f8,0x60000000); // fix 80010009 error
	do_patch32(0x800000000005a6e4,0x60000000); // fix 80010009 error
	do_patch(0x80000000002275f4,0x38600000F8690000);  // fix 0x8001002B / 80010017 errors  known as ODE patch
	do_patch(0x80000000002d2b34,0x386000004e800020); // ????
//	do_patch(0x8000000000003d90,0x386000014e800020); - // psjailbreak, PL3, etc destroy this function to copy their code there.
	do_patch(0x800000000005658C,0x63FF003D60000000);  // fix 8001003D error
	do_patch(0x8000000000056650,0x3FE080013BE00000); // fix 8001003E error
	do_patch(0x8000000000056604,0x2F84000448000098); //PATCH_JUMP
	
	*(uint64_t *)0x8000000000474A80=0;
//	*(uint64_t *)(r4+8)=0; //ecdsa flag
	hook_function_with_cond_postcall(0x4ef1c, support_advanced_flags, 2);
	region_patches();
	modules_patch_init();
	map_path_patches(0);
	
	storage_ext_patches();
	return 0;
}


static inline void ps3mapi_unhook_all(void)
{
	unhook_all_modules();
	unhook_all_region();
	unhook_all_map_path();
    unhook_all_storage_ext();
	//unhook_all_permissions();
}

static uint8_t photo_gui;

static uint64_t ps3mapi_key = 0;
static uint8_t ps3mapi_access_tries = 0;
static uint8_t ps3mapi_access_granted = 1;

static int ps3mapi_partial_disable_syscall8 = 0;
static uint8_t disable_cobra = 0;

LV2_SYSCALL2(int64_t, syscall8, (uint64_t function, uint64_t param1, uint64_t param2, uint64_t param3, uint64_t param4, uint64_t param5, uint64_t param6, uint64_t param7))
{
	extend_kstack(0);

	//DPRINTF("Syscall 8 -> %lx\n", function);
	
	// -- AV: temporary disable cobra syscall (allow dumpers peek 0x1000 to 0x9800)
	static uint8_t tmp_lv1peek = 0;

	if(ps3mapi_partial_disable_syscall8 == 0 && extended_syscall8.addr == 0 && ps3mapi_access_granted)
	{
		if((function >= 0x9800) || (function & 3)) 
			tmp_lv1peek=0; else

		if(function <= 0x1000) 
		{
			tmp_lv1peek=1; 

			if(function <= SYSCALL8_OPCODE_ENABLE_COBRA) 
			{
				if(param1>=SYSCALL8_DISABLE_COBRA_CAPABILITY) 
					return (param1==SYSCALL8_DISABLE_COBRA_CAPABILITY) ? SYSCALL8_DISABLE_COBRA_OK : disable_cobra; 
				else 
					disable_cobra = (function==SYSCALL8_OPCODE_DISABLE_COBRA && param1==1);
			}
		}
	
	}
	else 
		tmp_lv1peek=0;
	// --

/*
	static uint32_t pid_blocked = 0;
	uint32_t pid;

	// Some processsing to avoid crashes with lv1 dumpers
	pid = get_current_process_critical()->pid;

	if (pid == pid_blocked)
	{
		if (function <= 0x1000 || function >= 0x9800 || (function & 3)) // * Keep all cobra opcodes below 0x9800 * /
		{
			DPRINTF("App was unblocked from using syscall8\n");
			pid_blocked = 0;
		}
		else
		{
			DPRINTF("App was blocked from using syscall8\n");
			return ENOSYS;
		}
	}

	if (function == (SYSCALL8_OPCODE_GET_VERSION-8))
	{
		// 0x6FF8. On 0x7000 it *could* crash
		pid_blocked = pid;
		return ENOSYS;
	}
	else if (function == (SYSCALL8_OPCODE_PSP_POST_SAVEDATA_INITSTART-8))
	{
		// 0x3000, On 0x3008 it *could* crash
		pid_blocked = pid;
		return ENOSYS;
	}
*/

	if ((function == SYSCALL8_OPCODE_PS3MAPI) && ((int)param1 == PS3MAPI_OPCODE_REQUEST_ACCESS) && (param2 == ps3mapi_key) && (ps3mapi_access_tries < 3)) 
	{
		ps3mapi_access_tries = 0; 
		ps3mapi_access_granted = 1;
	}

	if((!ps3mapi_access_granted) && (ps3mapi_key != 0))
	{
		ps3mapi_access_tries += 1;

		if(ps3mapi_access_tries > 3) 
			ps3mapi_access_tries = 99;

		return ENOSYS;
	}

	if (3 <= ps3mapi_partial_disable_syscall8)
	{
		if (function == SYSCALL8_OPCODE_PS3MAPI && ps3mapi_access_granted)
		{
			if ((int)param1 == PS3MAPI_OPCODE_PDISABLE_SYSCALL8)
			{
				ps3mapi_partial_disable_syscall8 = (int)param2;
				return SUCCEEDED;
			}
			else if ((int)param1 == PS3MAPI_OPCODE_PCHECK_SYSCALL8)			
				return ps3mapi_partial_disable_syscall8;			
			else return ENOSYS;
		}
		else 
			return ENOSYS;
	}

	if ((function != SYSCALL8_OPCODE_PS3MAPI) && (2 <= ps3mapi_partial_disable_syscall8)) 
		return ENOSYS;

	// -- AV: disable cobra without reboot (use lv1 peek)
	if(disable_cobra) 
		return 0;

	switch (function)
	{
		case SYSCALL8_OPCODE_PS3MAPI:
			switch ((int)param1)
			{
				//DPRINTF("syscall8: PS3M_API function 0x%x\n", (int)param1);

				//----------
				//CORE
				//----------
				case PS3MAPI_OPCODE_GET_CORE_VERSION:
					return PS3MAPI_CORE_VERSION;
				break;
				case PS3MAPI_OPCODE_GET_CORE_MINVERSION:
					return PS3MAPI_CORE_MINVERSION;
				break;
				case PS3MAPI_OPCODE_GET_FW_VERSION:
					return PS3MAPI_FW_VERSION;
				break;
				case PS3MAPI_OPCODE_GET_FW_TYPE:
					return ps3mapi_get_fw_type((char *)param2);
				break;

				//----------------
				//PEEK & POKE (av)
				//----------------
				case PS3MAPI_OPCODE_SUPPORT_SC8_PEEK_POKE:
					return PS3MAPI_OPCODE_SUPPORT_SC8_PEEK_POKE_OK;
				break;
				case PS3MAPI_OPCODE_LV1_PEEK:
					return 0;
				break;
				case PS3MAPI_OPCODE_LV1_POKE:
					return 0;
				break;
				case PS3MAPI_OPCODE_LV2_PEEK:
					return *(uint64_t *)param2;
				break;
				case PS3MAPI_OPCODE_LV2_POKE:
					if(param2>0x8000000000352230)
					{
						*(uint64_t *)param2=param3;
					}
					return 0;
				break;

				//----------
				//SECURITY
				//----------
				case PS3MAPI_OPCODE_SET_ACCESS_KEY:
					ps3mapi_key = param2;
					ps3mapi_access_granted = 0;
					ps3mapi_access_tries = 0;
					return 0;

				//----------
				//PROCESS
				//----------
				case PS3MAPI_OPCODE_GET_ALL_PROC_PID:
					return ps3mapi_get_all_processes_pid((process_id_t *)param2);
				break;
				case PS3MAPI_OPCODE_GET_PROC_NAME_BY_PID:
					return ps3mapi_get_process_name_by_pid((process_id_t)param2, (char *)param3);
				break;
				case PS3MAPI_OPCODE_GET_PROC_BY_PID:
					return ps3mapi_get_process_by_pid((process_id_t)param2, (process_t)param3);
				break;
				case PS3MAPI_OPCODE_GET_CURRENT_PROC:
					return ps3mapi_get_current_process((process_t)param2);
				break;
				case PS3MAPI_OPCODE_GET_CURRENT_PROC_CRIT:
					return ps3mapi_get_current_process_critical((process_t)param2);
				break;
				//----------
				//MEMORY
				//----------
				case PS3MAPI_OPCODE_GET_PROC_MEM:
					return ps3mapi_get_process_mem((process_id_t)param2, param3, (char *)param4, (int)param5);
				break;
				case PS3MAPI_OPCODE_SET_PROC_MEM:
					return ps3mapi_set_process_mem((process_id_t)param2, param3, (char *)param4, (int)param5);
				break;
				//----------
				//MODULE
				//----------
				case PS3MAPI_OPCODE_GET_ALL_PROC_MODULE_PID:
					return ps3mapi_get_all_process_modules_prx_id((process_id_t)param2, (sys_prx_id_t *)param3);
				break;
				case PS3MAPI_OPCODE_GET_PROC_MODULE_NAME:
					return ps3mapi_get_process_module_name_by_prx_id((process_id_t)param2, (sys_prx_id_t)param3, (char *)param4);
				break;
				case PS3MAPI_OPCODE_GET_PROC_MODULE_FILENAME:
					return ps3mapi_get_process_module_filename_by_prx_id((process_id_t)param2, (sys_prx_id_t)param3, (char *)param4);
				break;
				case PS3MAPI_OPCODE_LOAD_PROC_MODULE:
					return ps3mapi_load_process_modules((process_id_t)param2, (char *)param3, (void *)param4, (uint32_t)param5);
				break;
				case PS3MAPI_OPCODE_UNLOAD_PROC_MODULE:
					return ps3mapi_unload_process_modules((process_id_t)param2, (sys_prx_id_t)param3);
				break;
				//----------
				//VSH PLUGINS
				//----------
				case PS3MAPI_OPCODE_UNLOAD_VSH_PLUGIN:
					return ps3mapi_unload_vsh_plugin((char *)param2);
				break;
				case PS3MAPI_OPCODE_GET_VSH_PLUGIN_INFO:
					return ps3mapi_get_vsh_plugin_info((unsigned int)param2, (char *)param3, (char *)param4);
				break;
				//----------
				//SYSCALL
				//----------
				case PS3MAPI_OPCODE_DISABLE_SYSCALL:
					return ps3mapi_disable_syscall((int)param2);
				break;
				case PS3MAPI_OPCODE_CHECK_SYSCALL:
					return ps3mapi_check_syscall((int)param2);
				break;
				case PS3MAPI_OPCODE_PDISABLE_SYSCALL8:
					ps3mapi_partial_disable_syscall8 = (int)param2;
					return SUCCEEDED;
				break;
				case PS3MAPI_OPCODE_PCHECK_SYSCALL8:
					return ps3mapi_partial_disable_syscall8;
				break;
				//----------
				//REMOVE HOOK
				//----------
				case PS3MAPI_OPCODE_REMOVE_HOOK:
					ps3mapi_unhook_all(); //REMOVE ALL "MAMBA/COBRA" HOOK
					return SUCCEEDED;
				break;
				//-----------
				//PSID/IDPS
				//-----------
				case PS3MAPI_OPCODE_GET_IDPS:
					return ps3mapi_get_idps((uint64_t *)param2);
				break;
				case PS3MAPI_OPCODE_SET_IDPS:
					return ps3mapi_set_idps(param2, param3);
				break;
				case PS3MAPI_OPCODE_GET_PSID:
					return ps3mapi_get_psid((uint64_t *)param2);
				break;
				case PS3MAPI_OPCODE_SET_PSID:
					return ps3mapi_set_psid(param2, param3);
				break;
				//----------
				//MISC
				//----------
				case PS3MAPI_OPCODE_PHOTO_GUI:
					photo_gui = (uint8_t)param2;
					return photo_gui;
				break;
				//----------
				//DEFAULT
				//----------
				default:
					DPRINTF("syscall8: Unsupported PS3M_API opcode: 0x%lx\n", function);
					return ENOSYS;
				break;
			}
		break;

		case SYSCALL8_OPCODE_STEALTH_TEST:  //KW PSNPatch stealth extension compatibility
			return SYSCALL8_STEALTH_OK;
		break;

		case SYSCALL8_OPCODE_STEALTH_ACTIVATE: //KW PSNPatch stealth extension compatibility
		{
				uint64_t syscall_not_impl = *(uint64_t *)MKA(syscall_table_symbol);
				//*(uint64_t *)MKA(syscall_table_symbol+ 8* 8) = syscall_not_impl;
				ps3mapi_partial_disable_syscall8 = 2; //NzV Edit: Keep PS3M_API Features only.
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 9) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 10) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 11) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 15) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 35) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 36) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 38) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 6) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 7) = syscall_not_impl;
			return SYSCALL8_STEALTH_OK;
		}
		break;

		case SYSCALL8_OPCODE_GET_VERSION:
			return sys_get_version((uint32_t *)param1);
		break;
		
		case SYSCALL8_OPCODE_GET_VERSION2:
			return sys_get_version2((uint16_t *)param1);
		break;

		case SYSCALL8_OPCODE_GET_DISC_TYPE:
			return sys_storage_ext_get_disc_type((unsigned int *)param1, (unsigned int *)param2, (unsigned int *)param3);
		break;

		case SYSCALL8_OPCODE_READ_PS3_DISC:
			return sys_storage_ext_read_ps3_disc((void *)param1, param2, (uint32_t)param3);
		break;

		case SYSCALL8_OPCODE_FAKE_STORAGE_EVENT:
			return sys_storage_ext_fake_storage_event(param1, param2, param3);
		break;

		case SYSCALL8_OPCODE_GET_EMU_STATE:
			return sys_storage_ext_get_emu_state((sys_emu_state_t *)param1);
		break;

		case SYSCALL8_OPCODE_MOUNT_PS3_DISCFILE:
			return sys_storage_ext_mount_ps3_discfile(param1, (char **)param2);
		break;

		case SYSCALL8_OPCODE_MOUNT_DVD_DISCFILE:
			return sys_storage_ext_mount_dvd_discfile(param1, (char **)param2);
		break;

		case SYSCALL8_OPCODE_MOUNT_BD_DISCFILE:
			return sys_storage_ext_mount_bd_discfile(param1, (char **)param2);
		break;
		
		case SYSCALL8_OPCODE_RUN_PAYLOAD:
			return inst_and_run_kernel((uint8_t *)param1, param2);
		break;

		case SYSCALL8_OPCODE_RUN_PAYLOAD_DYNAMIC:
			return inst_and_run_kernel_dynamic((uint8_t *)param1, param2, (uint64_t *)param3);
		break;

		case SYSCALL8_OPCODE_UNLOAD_PAYLOAD_DYNAMIC:
			return unload_plugin_kernel(param1);
		break;

		case SYSCALL8_OPCODE_MOUNT_PSX_DISCFILE:
			return sys_storage_ext_mount_psx_discfile((char *)param1, param2, (ScsiTrackDescriptor *)param3);
		break;

		case SYSCALL8_OPCODE_MOUNT_PS2_DISCFILE:
			return sys_storage_ext_mount_ps2_discfile(param1, (char **)param2, param3, (ScsiTrackDescriptor *)param4);
		break;

		case SYSCALL8_OPCODE_MOUNT_DISCFILE_PROXY:
			return sys_storage_ext_mount_discfile_proxy(param1, param2, param3, param4, param5, param6, (ScsiTrackDescriptor *)param7);
		break;

		case SYSCALL8_OPCODE_UMOUNT_DISCFILE:
			return sys_storage_ext_umount_discfile();
		break;

		case SYSCALL8_OPCODE_GET_ACCESS:
		case SYSCALL8_OPCODE_REMOVE_ACCESS:
			return 0;//needed for mmCM

		case SYSCALL8_OPCODE_READ_COBRA_CONFIG:
			return sys_read_cobra_config((CobraConfig *)param1);
		break;

		case SYSCALL8_OPCODE_WRITE_COBRA_CONFIG:
			return sys_write_cobra_config((CobraConfig *)param1);
		break;

		case SYSCALL8_OPCODE_COBRA_USB_COMMAND:
			//return sys_cobra_usb_command(param1, param2, param3, (void *)param4, param5);
			return 0;
		break;

		case SYSCALL8_OPCODE_AIO_COPY_ROOT:
			return sys_aio_copy_root((char *)param1, (char *)param2);
		break;

		case SYSCALL8_OPCODE_MAP_PATHS:
			return sys_map_paths((char **)param1, (char **)param2, param3);
		break;

		case SYSCALL8_OPCODE_VSH_SPOOF_VERSION:
			return ENOSYS; //sys_vsh_spoof_version((char *)param1);
		break;

		case SYSCALL8_OPCODE_LOAD_VSH_PLUGIN:
			return sys_prx_load_vsh_plugin(param1, (char *)param2, (void *)param3, param4);
		break;

		case SYSCALL8_OPCODE_UNLOAD_VSH_PLUGIN:
			return sys_prx_unload_vsh_plugin(param1);
		break;

		case SYSCALL8_OPCODE_SEND_POWEROFF_EVENT:
			return sys_sm_ext_send_poweroff_event((int)param1);
		break;

#ifdef DEBUG
		case SYSCALL8_OPCODE_DUMP_STACK_TRACE:
			dump_stack_trace3((void *)param1, 16);
			return 0;
		break;
		
		case SYSCALL8_OPCODE_POKE_LV2:
			*(uint64_t *)param1=param2;
			return 0;
		break;
		
		case SYSCALL8_OPCODE_IS_HEN:
			return 0x1337;
		break;
		
		case SYSCALL8_OPCODE_HEN_REV:
			return HEN_REV;
		break;

		/*case SYSCALL8_OPCODE_GENERIC_DEBUG:
			return sys_generic_debug(param1, (uint32_t *)param2, (void *)param3);
		break;*/
#endif

		default:
			if (1 <= ps3mapi_partial_disable_syscall8)	return ENOSYS;

	}

	DPRINTF("Unsupported syscall8 opcode: 0x%lx\n", function);
	return ENOSYS;
}

LV2_SYSCALL2(int, sm_set_fan_policy_sc,(uint8_t arg1, uint8_t arg2, uint8_t arg3))
{
	f_desc_t f;
	f.addr=(void*)MKA(0x264734);
	f.toc=(void *)MKA(TOC);
	int(*func)(uint64_t, uint8_t,uint8_t,uint8_t)=(void*)&f;
	return func(0x8000000000474C38,arg1,arg2,arg3);
}

LV2_SYSCALL2(int, sm_get_fan_policy_sc,(uint8_t id, uint8_t *st, uint8_t *policy, uint8_t * mode, uint8_t *duty))
{
	uint8_t st1;
	uint8_t policy1;
	uint8_t mode1;
	uint8_t duty1;
	f_desc_t f;
	f.addr=(void*)MKA(0x26293c);
	f.toc=(void *)MKA(TOC);
	int(*func)(uint64_t, uint8_t,uint8_t *,uint8_t *,uint8_t *,uint8_t *,uint64_t)=(void*)&f;
	int ret=func(0x8000000000474C38,id,&st1,&policy1,&mode1,&duty1,10000000);
	if(ret==0)
	{
		ret=copy_to_user(&st1, st,1);
		if(ret==0)
		{
			ret=copy_to_user(&policy1, policy,1);
			if(ret==0)
			{
				ret=copy_to_user(&mode1, mode,1);
				if(ret==0)
				{
					ret=copy_to_user(&duty1, duty,1);
				}
			}
		}		
	}
	return ret;
}

static INLINE void apply_kernel_patches(void)
{

	create_syscall2(8, syscall8);
	create_syscall2(6, sys_cfw_peek);
	create_syscall2(7, sys_cfw_poke);
//	create_syscall2(9, sys_cfw_lv1_poke);
	create_syscall2(10, sys_cfw_lv1_call);
//	create_syscall2(11, sys_cfw_lv1_peek);
	create_syscall2(15, sys_cfw_lv2_func);
	create_syscall2(389, sm_set_fan_policy_sc);
	create_syscall2(409, sm_get_fan_policy_sc);
}

int main(void)
{
#ifdef DEBUG
	debug_init();
//	debug_install();
	extern uint64_t _start;
	extern uint64_t __self_end;
	DPRINTF("Stage 2 says hello (load base = %p, end = %p) (version = %08X)\n", &_start, &__self_end, MAKE_VERSION(COBRA_VERSION, FIRMWARE_VERSION, IS_CFW));
#endif

//	hook_function_with_cond_postcall(0x4ef1c, support_advanced_flags, 2);
	storage_ext_init();
	modules_patch_init();
//	drm_init();

	apply_kernel_patches();
	map_path_patches(1);
	storage_ext_patches();
	region_patches();
//	permissions_patches();
	
	load_boot_plugins();
	load_boot_plugins_kernel();
	init_mount_hdd0();
/*	CellFsStat stat;
	if(cellFsStat("/dev_hdd0/plugins/CFW/bdplayer/bdp_BDMV.self",&stat)==0)
	{
		map_path("/dev_flash/","/dev_hdd0/plugins/CFW/",0);
	}*/
#ifdef DEBUG
	// "Laboratory"
	//do_hook_all_syscalls();
	//do_dump_threads_info_test();
	//do_dump_processes_test();
	//do_hook_load_module();
	//do_hook_mutex_create();
	//do_ps2net_copy_test();
	//do_dump_modules_info_test();
	//do_pad_test();
#endif

	return 0;
}
