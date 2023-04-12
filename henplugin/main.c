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

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <types.h>
#include "allocator.h"
#include "common.h"
#include "stdc.h"
#include "download_plugin.h"
#include "game_ext_plugin.h"
#include "xmb_plugin.h"
#include "xregistry.h"
//#include "paf.h"

#include <sys/sys_time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/timer.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netex/net.h>
#include <netex/errno.h>
#include <cell/fs/cell_fs_errno.h>
#include <cell/fs/cell_fs_file_api.h>
#include <ppu_intrinsics.h>
#include <cstdlib>
#pragma comment(lib, "net_stub")
#pragma comment(lib, "netctl_stub")

#define SERVER_PORT htons(80)
#define HOST_SERVER "www.ps3xploit.me"

#define RELEASE 0
#define DEV 1

int build_type = RELEASE;

int Socket;
struct hostent *Host;
struct sockaddr_in SocketAddress;
char RequestBuffer[512];

SYS_MODULE_INFO(HENPLUGIN, 0, 1, 0);
SYS_MODULE_START(henplugin_start);
SYS_MODULE_STOP(henplugin_stop);
SYS_MODULE_EXIT(henplugin_stop);

#define THREAD_NAME "henplugin_thread"
#define STOP_THREAD_NAME "henplugin_stop_thread"

extern uint32_t vshmain_EB757101(void);        // get running mode flag, 0 = XMB is running
                                               //                        1 = PS3 Game is running
                                               //                        2 = Video Player (DVD/BD) is running
                                               //                        3 = PSX/PSP Emu is running

#define GetCurrentRunningMode vshmain_EB757101 // _ZN3vsh18GetCooperationModeEv	 | vsh::GetCooperationMode(void)
#define IS_ON_XMB		(GetCurrentRunningMode() == 0)
#define IS_INGAME		(GetCurrentRunningMode() != 0)

static sys_ppu_thread_t thread_id=-1;
static int done = 0;

uint16_t hen_version;
int henplugin_start(uint64_t arg);
int henplugin_stop(void);
int is_wmm_installed = 0;
int is_hen_installing = 0;
int use_wmm_pkg = 0;

extern int vshmain_87BB0001(int param);
int (*vshtask_notify)(int, const char *) = NULL;

//static int (*vshmain_is_ss_enabled)(void) = NULL;
static int (*View_Find)(const char *) = NULL;
static void *(*plugin_GetInterface)(int,int) = NULL;

/*
static int (*set_SSHT_)(int) = NULL;

static int opd[2] = {0, 0};
*/

#define IS_INSTALLING		(View_Find("game_plugin") != 0)
#define IS_INSTALLING_NAS	(View_Find("nas_plugin") != 0)
#define IS_DOWNLOADING		(View_Find("download_plugin") != 0)

// Play RCO Sound
extern void paf_B93AFE7E(uint32_t plugin, const char *sound, float arg1, int arg2);
#define PlayRCOSound paf_B93AFE7E

// Category IDs: 0 User 1 Setting 2 Photo 3 Music 4 Video 5 TV 6 Game 7 Net 8 PSN 9 Friend
typedef struct
{
	int (*DoUnk0)(void);  // 1 Parameter: int value 0 - 4
	int (*DoUnk1)(void);  // 0 Parameter: returns an interface
	int (*DoUnk2)(void);  // 0 Parameter: returns an interface
	int (*DoUnk3)(void);  // 0 Parameter: returns an uint[0x14 / 0x24]
	int (*DoUnk4)(void);
	int (*DoUnk5)(void);  // 3 Parameter: list[] {(reload_category game/network/..,reload_category_items game/...), command amount}  - send (sequences of)xmb command(s)
	int (*ExecXMBcommand)(const char *,void *,int); // 3 Parameter: char* (open_list nocheck/...), void * callback(can be 0), 0
	int (*DoUnk7)(void);  // 2 Parameter:
	int (*DoUnk8)(void);  // 3 Parameter:
	int (*DoUnk9)(void);  // 3 Parameter: void *, void *, void *
	int (*DoUnk10)(void); // 2 Parameter: char * , int * out
	int (*DoUnk11)(char*,char*,uint8_t[]); // 3 Parameter: char * query , char * attribute? , uint8 output[]
	int (*DoUnk12)(void); // 1 Parameter: struct
	int (*DoUnk13)(void); // return 0 / 1 Parameter: int 0-9
	int (*DoUnk14)(void); // return 0 / 2 Parameter: int 0-9,
	int (*DoUnk15)(void); // 3 Parameter: int 0-9, ,
	int (*DoUnk16)(void); // nullsub / 3 Parameter: int 0-9, ,
	int (*DoUnk17)(void); // 5 Parameter: int 0-9,
	int (*DoUnk18)(void); // 1 Parameter:
	int (*DoUnk19)(void); // 1 Parameter:
	int (*DoUnk20)(void); // nullsub / PlayIndicate, 2 Parameter: , int value, (0 = show?, 1=update?, -1 = hide) -  (set_playing 0x%x 0x%llx 0x%llx 0x%llx 0x%llx")
	int (*DoUnk21)(void); // nullsub / 1 Parameter: uint * list (simply both parameter from 20/2 and 3rd terminating = -1)
	int (*DoUnk22)(void); // 0 Parameter / 1 Parameter:
	int (*DoUnk23)(void); // -
	int (*DoUnk24)(void); // 0 Parameter:
	int (*DoUnk25)(void); // 0 Parameter:
	int (*DoUnk26)(void); // 2 Parameter: char * (TropViewMode/backup/FaustPreview...) , char * (group/fixed/on...)
	int (*DoUnk27)(void); // 1 Parameter: char *
	int (*DoUnk28)(void); // 2 Parameter: char * (ReloadXil/AvcRoomItem/...), uint8 xml?_parameters[]
	int (*DoUnk29)(void); // 2 Parameter: char * ,
} explore_plugin_interface;

explore_plugin_interface * explore_interface;

/*
typedef struct
{
	int (*DoUnk0)(char *);// 1 Parameter: char * action
} explore_plugin_act0_interface;
explore_plugin_act0_interface * explore_act0_interface;
*/

/*
typedef struct
{
	int (*DoUnk0)(int); // 1 Parameter: int (0/1)
	int (*DoUnk1)(void); // 0 Parameter: - return int
	int (*DoUnk2)(char *arg1); // 1 Parameter: char * 
	int (*DoUnk3)(char *arg1); // 1 Parameter: char *
	int (*DoUnk4)(char *arg1, wchar_t * out); // 2 Parameter: char *, wchar_t * out
	int (*DoUnk5)(char *arg1, uint8_t *arg2); // 2 Parameter: char *, uint8_t *
	int (*DoUnk6)(char *arg1); // 1 Parameter: char *
	int (*DoUnk7)(char *arg1); // 1 Parameter: char *
} xai_plugin_interface;

xai_plugin_interface * xai_interface;
*/

static void * getNIDfunc(const char * vsh_module, uint32_t fnid, int offset)
{
	// 0x10000 = ELF
	// 0x10080 = segment 2 start
	// 0x10200 = code start

	uint32_t table = (*(uint32_t*)0x1008C) + 0x984; // vsh table address

	while(((uint32_t)*(uint32_t*)table) != 0)
	{
		uint32_t* export_stru_ptr = (uint32_t*)*(uint32_t*)table; // ptr to export stub, size 2C, "sys_io" usually... Exports:0000000000635BC0 stru_635BC0:    ExportStub_s <0x1C00, 1, 9, 0x39, 0, 0x2000000, aSys_io, ExportFNIDTable_sys_io, ExportStubTable_sys_io>
		const char* lib_name_ptr =  (const char*)*(uint32_t*)((char*)export_stru_ptr + 0x10);
		if(strncmp(vsh_module, lib_name_ptr, strlen(lib_name_ptr)) == 0)
		{
			// we got the proper export struct
			uint32_t lib_fnid_ptr = *(uint32_t*)((char*)export_stru_ptr + 0x14);
			uint32_t lib_func_ptr = *(uint32_t*)((char*)export_stru_ptr + 0x18);
			uint16_t count = *(uint16_t*)((char*)export_stru_ptr + 6); // number of exports
			for(int i = 0; i < count; i++)
			{
				if(fnid == *(uint32_t*)((char*)lib_fnid_ptr + i*4))
				{
					// take address from OPD
					return (void**)*((uint32_t*)(lib_func_ptr) + i) + offset;
				}
			}
		}
		table += 4;
	}
	return 0;
}

/*static int sys_timer_sleep(uint64_t sleep_time)
{
	system_call_1(0x8e,sleep_time);
	return (int)p1;
}*/

// LED Control (thanks aldostools)
#define SC_SYS_CONTROL_LED				(386)
#define LED_GREEN			1
#define LED_RED				2
#define LED_YELLOW			2 //RED+GREEN (RED alias due green is already on)
#define LED_OFF			0
#define LED_ON			1
#define LED_BLINK_FAST		2
#define LED_BLINK_SLOW		3
static void led(uint64_t color, uint64_t mode)
{
	system_call_2(SC_SYS_CONTROL_LED, (uint64_t)color, (uint64_t)mode);
}

// Reboot PS3
int reboot_flag=0;
void reboot_ps3(void);
void reboot_ps3(void)
{
	cellFsUnlink("/dev_hdd0/tmp/turnoff");
	system_call_3(379, 0x200, 0, 0);// Soft Reboot
	//system_call_3(379, 0x1200, 0, 0);// Hard Reboot
}

static void show_msg(char* msg)
{
	if(!vshtask_notify)
		vshtask_notify = getNIDfunc("vshtask", 0xA02D46E7, 0);

	if(!vshtask_notify) return;

	if(strlen(msg) > 200) msg[200] = NULL; // truncate on-screen message
	vshtask_notify(0, msg);

}
#define process_id_t uint32_t
#define SYSCALL8_OPCODE_PS3MAPI			 		0x7777
#define PS3MAPI_OPCODE_GET_ALL_PROC_PID			0x0021
#define PS3MAPI_OPCODE_GET_PROC_NAME_BY_PID		0x0022
#define PS3MAPI_OPCODE_GET_PROC_MEM				0x0031
#define PS3MAPI_OPCODE_SET_PROC_MEM				0x0032
#define MAX_PROCESS 16

process_id_t vsh_pid=0;
/*
static int poke_vsh(uint64_t address, char *buf,int size)
{
	if(!vsh_pid)
	{
		uint32_t tmp_pid_list[MAX_PROCESS];
		char name[25];
		int i;
		system_call_3(8, SYSCALL8_OPCODE_PS3MAPI,PS3MAPI_OPCODE_GET_ALL_PROC_PID,(uint64_t)(uint32_t)tmp_pid_list);
		for(i=0;i<MAX_PROCESS;i++)
		{
			system_call_4(8, SYSCALL8_OPCODE_PS3MAPI,PS3MAPI_OPCODE_GET_PROC_NAME_BY_PID,tmp_pid_list[i],(uint64_t)(uint32_t)name);
			if(strstr(name,"vsh"))
			{
				vsh_pid=tmp_pid_list[i];
				break;
			}
		}
		if(!vsh_pid)
			return -1;
	}
	system_call_6(8,SYSCALL8_OPCODE_PS3MAPI,PS3MAPI_OPCODE_SET_PROC_MEM,vsh_pid,address,(uint64_t)(uint32_t)buf,size);
	return_to_user_prog(int);
}
*/
static void enable_ingame_screenshot(void)
{
	((int*)getNIDfunc("vshmain",0x981D7E9F,0))[0] -= 0x2C;
}
/*
static int sys_map_path(char *old, char *new)
{
	system_call_2(35, (uint64_t)(uint32_t)old,(uint64_t)(uint32_t)new);
	return (int)p1;
}
*/
static void reload_xmb(void)
{
	while(!IS_ON_XMB)
	{
		sys_timer_usleep(70000);
	}
	explore_interface->ExecXMBcommand("reload_category game",0,0);
	explore_interface->ExecXMBcommand("reload_category network",0,0);
	
	/*
	char* q=(char*)malloc(89);
	char* a=(char*)malloc(18);
	memcpy(q,"xcb://localhost/query?sort=+Game:Common.titleForSort&cond=Oe+Game:Game.titleId RELOADXMB",89);
	memcpy(a,"Game:Game.titleId",18);
	q[89]='\0'; // add null termination
	a[18]='\0'; // add null termination
	uint8_t output[8]={0,0,0,0,0,0,0,0};
	int ret=explore_interface->DoUnk11(&q[0],&a[0],&output[0]);
	if(ret)
	{
		printf ("Error 0x%X\n",ret);
	}
	else
	{
		for(int i=0;i<8;i++)
		{
			printf ("Output %i = 0x%X\n", i, output[i]);
		}
	}
	*/
}

static inline void _sys_ppu_thread_exit(uint64_t val)
{
	system_call_1(41, val);
}

static inline sys_prx_id_t prx_get_module_id_by_address(void *addr)
{
	system_call_1(461, (uint64_t)(uint32_t)addr);
	return (int)p1;
}

#define SC_STOP_PRX_MODULE 				(482)
#define SC_UNLOAD_PRX_MODULE 			(483)
#define SC_COBRA_SYSCALL8 8

static void unload_prx_module(void)
{

	sys_prx_id_t prx = prx_get_module_id_by_address(unload_prx_module);

	{system_call_3(SC_UNLOAD_PRX_MODULE, (uint64_t)prx, 0, NULL);}

}

/*
static void stop_prx_module(void)
{
	sys_prx_id_t prx = prx_get_module_id_by_address(stop_prx_module);
	int *result=NULL;

	{system_call_6(SC_STOP_PRX_MODULE, (uint64_t)prx, 0, NULL, (uint64_t)(uint32_t)result, 0, NULL);}

}
*/

// Updated 20220613 (thanks TheRouLetteBoi)
static void stop_prx_module(void)
{
    sys_prx_id_t prx = prx_get_module_id_by_address(stop_prx_module);
    int *result=NULL;
   
    uint64_t meminfo[5];
    meminfo[0] = 0x28;
    meminfo[1] = 2;
    meminfo[3] = 0;

    {system_call_6(SC_STOP_PRX_MODULE, (uint64_t)prx, 0, (uint64_t)(uint32_t)meminfo, (uint64_t)(uint32_t)result, 0, NULL);}

}

static int LoadPluginById(int id, void *handler)
{
	if(xmm0_interface == 0) // getting xmb_plugin xmm0 interface for loading plugin sprx
	{
		xmm0_interface = (xmb_plugin_xmm0 *)plugin_GetInterface(View_Find("xmb_plugin"), 0x584d4d30);
	}
	return xmm0_interface->LoadPlugin3(id, handler, 0);
}

static int UnloadPluginById(int id, void *handler)
{
	if(xmm0_interface == 0) // getting xmb_plugin xmm0 interface for loading plugin sprx
	{
		xmm0_interface = (xmb_plugin_xmm0 *)plugin_GetInterface(View_Find("xmb_plugin"), 0x584d4d30);
	}
	return xmm0_interface->Shutdown(id, handler, 1);
}

#define SYSCALL8_OPCODE_HEN_REV		0x1339

int do_install_hen=0;
int do_update=0;
int thread2_download_finish=0;
int thread3_install_finish=0;

#define SYSCALL_PEEK	6

static uint64_t peekq(uint64_t addr)
{
	system_call_1(SYSCALL_PEEK, addr);
	return_to_user_prog(uint64_t);
}

char pkg_path[256]={"/dev_hdd0/Latest_HEN_Installer_signed.pkg"};
char pkg_path_wmm[256]={"/dev_hdd0/Latest_HEN_Installer_WMM_signed.pkg"};

// FW version values are checked using a partial date from lv2 kernel. 4.89 Sample: 323032322F30322F = 2022/02/
static void downloadPKG_thread2(void)
{
	if(download_interface == 0) // test if download_interface is loaded for interface access
	{
		download_interface = (download_plugin_interface *)plugin_GetInterface(View_Find("download_plugin"), 1);
	}
	show_msg((char *)"Downloading Latest HEN Package");
	
	uint64_t val=peekq(0x80000000002FCB68ULL);// CEX
	uint64_t valD=peekq(0x800000000031F028ULL);// DEX
	
	// 4.80 CEX
	if(val==0x3A32350000000000ULL)
	{
		if(build_type==RELEASE)
		{
			if(use_wmm_pkg==1)
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/480/cex/installer/Latest_HEN_Installer_WMM_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/480/cex/installer/Latest_HEN_Installer_WMM_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
			else
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/480/cex/installer/Latest_HEN_Installer_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/480/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
		}
		else{
			DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/dev/480/cex/installer/Latest_HEN_Installer_signed.pkg\n");
			download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/dev/480/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
	}
	
	// 4.81 CEX
	else if(val==0x323031362F31302FULL)
	{
		if(build_type==RELEASE)
		{
			if(use_wmm_pkg==1)
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/481/cex/installer/Latest_HEN_Installer_WMM_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/481/cex/installer/Latest_HEN_Installer_WMM_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
			else
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/481/cex/installer/Latest_HEN_Installer_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/481/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
		}
		else{
			DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/dev/481/cex/installer/Latest_HEN_Installer_signed.pkg\n");
			download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/dev/481/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
	}
	
	// 4.82 CEX
	else if(val==0x323031372F30382FULL)
	{
		if(build_type==RELEASE)
		{
			if(use_wmm_pkg==1)
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/482/cex/installer/Latest_HEN_Installer_WMM_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/482/cex/installer/Latest_HEN_Installer_WMM_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
			else
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/482/cex/installer/Latest_HEN_Installer_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/482/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
		}
		else{
			DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/dev/482/cex/installer/Latest_HEN_Installer_signed.pkg\n");
			download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/dev/482/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
	}
		
	// 4.83 CEX
	else if(val==0x323031382F30392FULL)
	{
		if(build_type==RELEASE)
		{
			if(use_wmm_pkg==1)
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/483/cex/installer/Latest_HEN_Installer_WMM_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/483/cex/installer/Latest_HEN_Installer_WMM_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
			else
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/483/cex/installer/Latest_HEN_Installer_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/483/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
		}
		else{
			DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/dev/483/cex/installer/Latest_HEN_Installer_signed.pkg\n");
			download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/dev/483/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
	}
		
	// 4.84 CEX
	else if(val==0x323031392F30312FULL)
	{
		if(build_type==RELEASE)
		{
			if(use_wmm_pkg==1)
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/484/cex/installer/Latest_HEN_Installer_WMM_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/484/cex/installer/Latest_HEN_Installer_WMM_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
			else
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/484/cex/installer/Latest_HEN_Installer_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/484/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
		}
		else{
			DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/dev/484/cex/installer/Latest_HEN_Installer_signed.pkg\n");
			download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/dev/484/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
	}
		
	// 4.85 CEX
	else if(val==0x323031392F30372FULL)
	{
		if(build_type==RELEASE)
		{
			if(use_wmm_pkg==1)
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/485/cex/installer/Latest_HEN_Installer_WMM_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/485/cex/installer/Latest_HEN_Installer_WMM_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
			else
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/485/cex/installer/Latest_HEN_Installer_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/485/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
		}
		else{
			DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/dev/485/cex/installer/Latest_HEN_Installer_signed.pkg\n");
			download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/dev/485/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
	}
		
	// 4.86 CEX
	else if(val==0x323032302F30312FULL)
	{
		if(build_type==RELEASE)
		{
			if(use_wmm_pkg==1)
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/486/cex/installer/Latest_HEN_Installer_WMM_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/486/cex/installer/Latest_HEN_Installer_WMM_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
			else
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/486/cex/installer/Latest_HEN_Installer_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/486/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
		}
		else{
			DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/dev/486/cex/installer/Latest_HEN_Installer_signed.pkg\n");
			download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/dev/486/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
	}
		
	// 4.87 CEX
	else if(val==0x323032302F30372FULL)
	{
		if(build_type==RELEASE)
		{
			if(use_wmm_pkg==1)
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/487/cex/installer/Latest_HEN_Installer_WMM_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/487/cex/installer/Latest_HEN_Installer_WMM_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
			else
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/487/cex/installer/Latest_HEN_Installer_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/487/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
		}
		else{
			DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/dev/487/cex/installer/Latest_HEN_Installer_signed.pkg\n");
			download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/dev/487/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
	}
		
	// 4.88 CEX
	else if(val==0x323032312F30342FULL)
	{
		if(build_type==RELEASE)
		{
			if(use_wmm_pkg==1)
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/488/cex/installer/Latest_HEN_Installer_WMM_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/488/cex/installer/Latest_HEN_Installer_WMM_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
			else
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/488/cex/installer/Latest_HEN_Installer_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/488/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
		}
		else{
			DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/dev/488/cex/installer/Latest_HEN_Installer_signed.pkg\n");
			download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/dev/488/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
	}
		
	// 4.89 CEX
	else if(val==0x323032322F30322FULL)
	{
		if(build_type==RELEASE)
		{
			if(use_wmm_pkg==1)
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/489/cex/installer/Latest_HEN_Installer_WMM_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/489/cex/installer/Latest_HEN_Installer_WMM_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
			else
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/489/cex/installer/Latest_HEN_Installer_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/489/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
		}
		else{
			DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/dev/489/cex/installer/Latest_HEN_Installer_signed.pkg\n");
			download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/dev/489/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
	}
		
	// 4.90 CEX
	else if(val==0x3A35340000000000ULL) // for 4.90, kernel offset is off by 0x10 so we are checking this value instead of the timestamp
	{
		if(build_type==RELEASE)
		{
			if(use_wmm_pkg==1)
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/490/cex/installer/Latest_HEN_Installer_WMM_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/490/cex/installer/Latest_HEN_Installer_WMM_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
			else
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/490/cex/installer/Latest_HEN_Installer_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/490/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
		}
		else{
			DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/dev/490/cex/installer/Latest_HEN_Installer_signed.pkg\n");
			download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/dev/490/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
	}

	// 4.82 DEX
	else if(valD==0x323031372F30382FULL)
	{
		if(build_type==RELEASE)
		{
			if(use_wmm_pkg==1)
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/482/dex/installer/Latest_HEN_Installer_WMM_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/482/dex/installer/Latest_HEN_Installer_WMM_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
			else
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/482/dex/installer/Latest_HEN_Installer_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/482/dex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
		}
		else{
			DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/dev/482/dex/installer/Latest_HEN_Installer_signed.pkg\n");
			download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/dev/482/dex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
	}
		
	// 4.84 DEX
	else if(valD==0x323031392F30312FULL)
	{
		if(build_type==RELEASE)
		{
			if(use_wmm_pkg==1)
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/484/dex/installer/Latest_HEN_Installer_WMM_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/484/dex/installer/Latest_HEN_Installer_WMM_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
			else
			{
				DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/release/484/dex/installer/Latest_HEN_Installer_signed.pkg\n");
				download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/release/484/dex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
		}
		else{
			DPRINTF("Downloading PKG: http://www.ps3xploit.me/hen/dev/484/dex/installer/Latest_HEN_Installer_signed.pkg\n");
			download_interface->DownloadURL(0, (wchar_t *) L"http://www.ps3xploit.me/hen/dev/484/dex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
			}
	}
		
	thread2_download_finish=1;
}


static void installPKG_thread(void)
{
	if(game_ext_interface == 0) // test if game_ext_plugin is loaded for interface access
	{
		game_ext_interface = (game_ext_plugin_interface *)plugin_GetInterface(View_Find("game_ext_plugin"), 1);
		if(game_ext_interface == 0) return;
	}

	game_ext_interface->LoadPage();
	game_ext_interface->installPKG((char *)pkg_path);
	thread3_install_finish=1;
}

static void unloadSysPluginCallback(void)
{
	//Add potential callback process
	//show_msg((char *)"plugin shutdown via xmb call launched");
	DPRINTF("plugin shutdown via xmb call launched");
}

static void unload_web_plugins(void)
{

	while(View_Find("webrender_plugin"))
	{
		UnloadPluginById(0x1C, (void *)unloadSysPluginCallback);
		sys_timer_usleep(70000);
	}

	while(View_Find("webbrowser_plugin"))
	{
		UnloadPluginById(0x1B, (void *)unloadSysPluginCallback);
		sys_timer_usleep(70000);
	}

	explore_interface->ExecXMBcommand("close_all_list", 0, 0);
}

char server_reply[0x500];

int hen_updater(void);
int hen_updater(void)
{
	uint16_t latest_rev=0;
	Host = gethostbyname(HOST_SERVER);
	if(!Host)
	{
		show_msg((char *)"Could not resolve update Host!\n");
		return -1;
	}
    SocketAddress.sin_addr.s_addr = *((unsigned long*)Host->h_addr);
    SocketAddress.sin_family = AF_INET;
    SocketAddress.sin_port = SERVER_PORT;
    Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(Socket, (struct sockaddr *)&SocketAddress, sizeof(SocketAddress)) != 0) {
		show_msg((char *)"Failed To Connect To Update Server!");
        return -1;
    }

	strcpy(RequestBuffer, "GET ");
    if(build_type==RELEASE){strcat(RequestBuffer, "/hen/hen_version.bin");}
    if(build_type==DEV){strcat(RequestBuffer, "/hen/hen_version_dev.bin");}
    strcat(RequestBuffer, " HTTP/1.0\r\n");
	strcat(RequestBuffer, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/17.17134\r\n");
    strcat(RequestBuffer, "Accept-Language: en-US\r\n");
    strcat(RequestBuffer, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
    strcat(RequestBuffer, "Upgrade-Insecure-Requests: 1\r\n");
    strcat(RequestBuffer, "HOST: "HOST_SERVER"\r\n");
    strcat(RequestBuffer, "Connection: close\r\n");
    strcat(RequestBuffer, "\r\n");
    send(Socket, RequestBuffer, strlen(RequestBuffer), 0);

	int reply_len=0;
	int allowed_length=sizeof(server_reply);
    while (1)
    {
		int reply_len1=recv(Socket, &server_reply[reply_len], allowed_length, 0);
		if(reply_len1>0)
		{
			reply_len+=reply_len1;
			allowed_length-=reply_len1;
		}
		else
		{
			break;
		}
    }
	socketclose(Socket);
	if(reply_len<=6)
	{
		show_msg((char *)"Error on update server!");
		return 0;
	}

	if(strstr(server_reply,"200 OK"))
	{
		latest_rev=*(uint16_t *)(server_reply+reply_len-2);
	}
	else
	{
		show_msg((char *)"Update Server Responded With Error!");
		return 0;
	}

	char msg[100];
	sprintf(msg,"Latest PS3HEN available is %X.%X.%X",latest_rev>>8, (latest_rev & 0xF0)>>4, (latest_rev&0xF));
	show_msg((char*)msg);
	if(hen_version<latest_rev)
	{
		return 1;
	}
	return 0;
}

void clear_web_cache_check(void);
void clear_web_cache_check(void)
{
	// Clear WebBrowser cache (thanks xfrcc)
	// Toggles can be accessed by HFW Tools menu
	CellFsStat stat;
	
	char msg[0x80];
	int cleared_history = 0;
	int cleared_auth_cache = 0;
	int cleared_cookie = 0;
	int cleared_total = 0;
	
	char path1[0x40];
	sprintf(path1, "/dev_hdd0/home/%08i/webbrowser/history.xml", xsetting_CC56EB2D()->GetCurrentUserNumber());

	char path2[0x40];
	sprintf(path2, "/dev_hdd0/home/%08i/http/auth_cache.dat", xsetting_CC56EB2D()->GetCurrentUserNumber());

	char path3[0x40];
	sprintf(path3, "/dev_hdd0/home/%08i/http/cookie.dat", xsetting_CC56EB2D()->GetCurrentUserNumber());

	if(cellFsStat(path1,&stat)==0 && cellFsStat("/dev_hdd0/hen/toggles/clear_web_history.on",&stat)==0)
	{
		//DPRINTF("Toggle Activated: clear_web_history\n");
		cellFsUnlink(path1);
		cleared_history = 1;
		cleared_total++;
	}
	
	if(cellFsStat(path2,&stat)==0 && cellFsStat("/dev_hdd0/hen/toggles/clear_web_auth_cache.on",&stat)==0)
	{
		//DPRINTF("Toggle Activated: clear_web_auth_cache\n");
		cellFsUnlink(path2);
		cleared_auth_cache = 1;
		cleared_total++;
	}
	
	if(cellFsStat(path3,&stat)==0 && cellFsStat("/dev_hdd0/hen/toggles/clear_web_cookie.on",&stat)==0)
	{
		//DPRINTF("Toggle Activated: clear_web_cookie\n");
		cellFsUnlink(path3);
		cleared_cookie = 1;
		cleared_total++;
	}
	
	if(cleared_total>0)
	{
		sprintf(msg, "Clear Web Cache\nHistory[%i] / Auth Cache[%i] / Cookie[%i]", cleared_history, cleared_auth_cache, cleared_cookie);
		show_msg((char*)msg);
	}
	else
	{
		//DPRINTF("No Clear Web Cache Toggles Activated\n");
	}
}

void set_build_type(void);
void set_build_type(void)
{
	CellFsStat stat;
	if(cellFsStat("/dev_hdd0/hen/toggles/dev_build_type.on",&stat)==0){build_type=DEV;}
	DPRINTF("Setting build_type to %i\n", build_type);
}

static int sysLv2FsLink(const char *oldpath, const char *newpath)
{
    system_call_2(810, (uint64_t)(uint32_t)oldpath, (uint64_t)(uint32_t)newpath);
    return_to_user_prog(int);
}

// Restore act.dat (thanks bucanero)
void restore_act_dat(void);
void restore_act_dat(void)
{
	CellFsStat stat;
	char path1[64], path2[64];

	for (int i = 1; i < 0x100; i++)
	{
		sprintf(path1, "/dev_hdd0/home/%08d/exdata/act.bak", i);
		sprintf(path2, "/dev_hdd0/home/%08d/exdata/act.dat", i);
		
		if((cellFsStat(path1,&stat) == CELL_FS_SUCCEEDED) && (cellFsStat(path2,&stat) != CELL_FS_SUCCEEDED))
		{
			// copy act.bak to act.dat
			sysLv2FsLink(path1, path2);
		}
	}
}

// Shamelessly taken and modified from webmanMOD (thanks aldostools)
static void play_rco_sound(const char *sound)
{
	//const char *system_plugin = (char*)"system_plugin";
	//char *sep = strchr(sound, '|'); if(sep) {*sep = NULL, system_plugin = sep + 1;}
	View_Find = getNIDfunc("paf", 0xF21655F3, 0);
	uint32_t plugin = View_Find("system_plugin");
	//uint32_t plugin = View_Find(system_plugin);
	if(plugin)
	{
		PlayRCOSound(plugin, sound, 1, 0);
		DPRINTF("HENPLUGIN->PlayRCOSound(%0X, %s, 1, 0)\n",plugin,sound);
	}
}

static int tick_max=1000,tick_count=0,tick_expire=0;// Used for breaking out of while loop if package install hangs

static void henplugin_thread(__attribute__((unused)) uint64_t arg)
{
	set_build_type();
	
	View_Find = getNIDfunc("paf", 0xF21655F3, 0);
	plugin_GetInterface = getNIDfunc("paf", 0x23AFB290, 0);
	int view = View_Find("explore_plugin");
	system_call_1(8, SYSCALL8_OPCODE_HEN_REV); hen_version = (int)p1;
	char henver[0x30];
	if(build_type==DEV)
	{
		sprintf(henver, "Welcome to PS3HEN %X.%X.%X\nDeveloper Mode", hen_version>>8, (hen_version & 0xF0)>>4, (hen_version&0xF));
	}
	else
	{
		sprintf(henver, "Welcome to PS3HEN %X.%X.%X", hen_version>>8, (hen_version & 0xF0)>>4, (hen_version&0xF));
	}
	//DPRINTF("hen_version: %x\n",hen_version);
	show_msg((char *)henver);

	if(view==0)
	{
		view=View_Find("explore_plugin");
		sys_timer_usleep(70000);
	}
	explore_interface = (explore_plugin_interface *)plugin_GetInterface(view, 1);

	enable_ingame_screenshot();
	reload_xmb();

	CellFsStat stat;
	
	// Remove temp install check file here in case a package was installed containing it
	// If the file exists before the pkg install starts, it will cause an early reboot trigger
	cellFsUnlink("/dev_rewrite/vsh/resource/explore/xmb/zzz_hen_installed.tmp");

	// Emergency USB HEN Installer
	if(cellFsStat("/dev_usb000/HEN_UPD.pkg",&stat)==0)
	{
		play_rco_sound("snd_trophy");
		led(LED_YELLOW,LED_BLINK_FAST);
		//DPRINTF("Installing Emergency Package From USB\n");
		tick_count=0;// Reset tick count for package installation
		char hen_usb_update[0x80];
		sprintf(hen_usb_update, "Installing Emergency Package\n\nRemove HEN_UPD.pkg after install");
		memset(pkg_path,0,256);
		strcpy(pkg_path,"/dev_usb000/HEN_UPD.pkg");
		show_msg((char *)hen_usb_update);
		LoadPluginById(0x16, (void *)installPKG_thread);
		while(thread3_install_finish==0)
		{
			sys_timer_usleep(70000);
		}
		while(cellFsStat("/dev_rewrite/vsh/resource/explore/xmb/zzz_hen_installed.tmp",&stat)!=0)
		{
			sys_timer_usleep(70000);
			tick_count++;
			if(tick_count>=tick_max){tick_expire=1;break;};
			//DPRINTF("Waiting for package to finish installing\n");
		}
		reboot_flag=1;
		goto done;
	}
	
	// restore act.dat from act.bak backup
	restore_act_dat();
	
	// If default HEN Check file is missing, assume HEN is not installed
	do_install_hen=(cellFsStat("/dev_flash/vsh/resource/explore/icon/hen_enable.png",&stat));
	//DPRINTF("do_install_hen: %x\n",do_install_hen);
	
	// Check for webMAN-MOD
	if((cellFsStat("/dev_hdd0/plugins/webftp_server.sprx",&stat)==0) || (cellFsStat("/dev_hdd0/plugins/webftp_server_lite.sprx",&stat)==0))
	{
		is_wmm_installed=1;
		DPRINTF("HENPLUGIN->WMM Detected\n");
	}
	
	// Display message about the removal of boot plugins
	// Created from payload if HEN is installing, so plugins can not be loaded
	if(cellFsStat("/dev_hdd0/tmp/installer.active",&stat)==0)
	{
		is_hen_installing=1;
		//play_rco_sound("snd_trophy");
		char msg_boot_plugins[0x80];
		if(is_wmm_installed==1)
		{
			sprintf(msg_boot_plugins, "Boot Plugins Text Have Been Deleted!\nUpdate webMAN-MOD from PKG Manager after reboot.");
		}
		else
		{
			sprintf(msg_boot_plugins, "Boot Plugins Text Have Been Deleted!\nIf you have plugins, these files need updated");
		}
		show_msg((char *)msg_boot_plugins);
		cellFsUnlink("/dev_hdd0/tmp/installer.active");
	}
	
	do_update=(cellFsStat("/dev_hdd0/hen_updater.off",&stat) ? hen_updater() : 0);// 20211011 Added update toggle thanks bucanero for original PR
	//DPRINTF("Checking do_update: %i\n",do_update);
	
	if((do_install_hen!=0) || (do_update==1))
	{
		led(LED_YELLOW,LED_BLINK_FAST);
		int is_browser_open=View_Find("webbrowser_plugin");
		
		while(is_browser_open)
		{	
			sys_timer_usleep(70000);
			is_browser_open=View_Find("webbrowser_plugin");
		}
		is_browser_open=View_Find("webrender_plugin");
		while(is_browser_open)
		{
			sys_timer_usleep(70000);
			is_browser_open=View_Find("webrender_plugin");
		}
		unload_web_plugins();
		
		// Check for Webman-MOD and use PS3HEN-WMM Package Link
		if((is_wmm_installed==1) && (is_hen_installing==1) && (build_type==!DEV))
		{
			DPRINTF("HENPLUGIN->Use WMM Update Package\n");
			memset(pkg_path,0,256);
			strcpy(pkg_path,pkg_path_wmm);
			use_wmm_pkg=1;
		}
		
		DPRINTF("HENPLUGIN->pkg_path=%s\n",pkg_path);
	
		LoadPluginById(0x29,(void*)downloadPKG_thread2);

		while(thread2_download_finish==0)
		{
			sys_timer_usleep(70000);
		}

		while(IS_DOWNLOADING)
		{
			sys_timer_usleep(500000);
			//DPRINTF("Waiting for package to finish downloading\n");
		}
		
		// Fail Safe here in case of manual/other placement of file, will still reboot properly
		cellFsUnlink("/dev_rewrite/vsh/resource/explore/xmb/zzz_hen_installed.tmp");
		
		if(cellFsStat(pkg_path,&stat)==0)
		{
			// After package starts installing, this first loop exits
			LoadPluginById(0x16, (void *)installPKG_thread);
			while(thread3_install_finish==0)
			{
				sys_timer_usleep(70000);
			}
			
			// The package is now installing
			tick_count=0;// Reset tick count for package installation
			while(cellFsStat("/dev_rewrite/vsh/resource/explore/xmb/zzz_hen_installed.tmp",&stat)!=0)
			{
				//DPRINTF("tick_count: %i\n",tick_count);
				sys_timer_usleep(70000);
				tick_count++;
				if(tick_count>=tick_max){tick_expire=1;break;};
				//DPRINTF("Waiting for package to finish installing\n");
			}
			reboot_flag=1;
			
			goto done;
		}
	}
	else
	{   
		// Removing temp packages
		//cellFsUnlink(pkg_path);
		cellFsUnlink("/dev_hdd0/Latest_HEN_Installer_signed.pkg");
		cellFsUnlink("/dev_hdd0/Latest_HEN_Installer_WMM_signed.pkg");
	}
	
done:
	DPRINTF("Exiting main thread!\n");	
	
	cellFsUnlink("/dev_hdd0/theme/PS3HEN.p3t");// Removing temp HEN installer
	done=1;
	
	if(reboot_flag==1)
	{
		play_rco_sound("snd_trophy");
		led(LED_YELLOW,LED_OFF);
		led(LED_GREEN,LED_OFF);
		
		char reboot_txt[0x80];
		if(tick_expire==0)
		{
			led(LED_GREEN,LED_ON);
			sprintf(reboot_txt, "Installation Complete!\n\nPrepare For Reboot...");
		}
		else
		{
			led(LED_RED,LED_BLINK_FAST);
			sprintf(reboot_txt, "Error: Unable To Verify Installation!\nYou Must Reboot Manually!");
		}
		show_msg((char *)reboot_txt);
		sys_timer_usleep(8000000);// Wait a few seconds
		
		// Verify the temp file is removed
		while(cellFsStat("/dev_rewrite/vsh/resource/explore/xmb/zzz_hen_installed.tmp",&stat)==0)
		{
			sys_timer_usleep(70000);
			cellFsUnlink("/dev_rewrite/vsh/resource/explore/xmb/zzz_hen_installed.tmp");// Remove temp file
			//DPRINTF("Waiting for temporary zzz_hen_installed.tmp file to be removed\n");
		}
		if(tick_expire==0)
		{
			// Removing temp packages
			DPRINTF("HENPLUGIN->Removing Temp Installer Packages");
			cellFsUnlink("/dev_hdd0/Latest_HEN_Installer_signed.pkg");
			cellFsUnlink("/dev_hdd0/Latest_HEN_Installer_WMM_signed.pkg");
			
			reboot_ps3();// Default Soft Reboot
		}
	}
	
	clear_web_cache_check();// Clear WebBrowser cache check (thanks xfrcc)
	
	sys_ppu_thread_exit(0);
}

int henplugin_start(__attribute__((unused)) uint64_t arg)
{
	//sys_timer_sleep(40000);
	sys_ppu_thread_create(&thread_id, henplugin_thread, 0, 3000, 0x4000, SYS_PPU_THREAD_CREATE_JOINABLE, THREAD_NAME);
	// Exit thread using directly the syscall and not the user mode library or we will crash
	_sys_ppu_thread_exit(0);
	return SYS_PRX_RESIDENT;
}

static void henplugin_stop_thread(__attribute__((unused)) uint64_t arg)
{
	uint64_t exit_code;
	sys_ppu_thread_join(thread_id, &exit_code);
	sys_ppu_thread_exit(0);
}

/*
int henplugin_stop()
{
	sys_ppu_thread_t t_id;
	//int ret = sys_ppu_thread_create(&t_id, henplugin_stop_thread, 0, 3000, 0x2000, SYS_PPU_THREAD_CREATE_JOINABLE, STOP_THREAD_NAME);
	int ret = sys_ppu_thread_create(&t_id, henplugin_stop_thread, 0, 0, 0x2000, SYS_PPU_THREAD_CREATE_JOINABLE, STOP_THREAD_NAME);

	uint64_t exit_code;
	if (ret == 0) sys_ppu_thread_join(t_id, &exit_code);

	//sys_timer_usleep(70000);
	unload_prx_module();

	_sys_ppu_thread_exit(0);

	return SYS_PRX_STOP_OK;
}
*/

// Updated 20220613 (thanks TheRouLetteBoi)
int henplugin_stop()
{
	sys_ppu_thread_t t_id;
	int ret = sys_ppu_thread_create(&t_id, henplugin_stop_thread, 0, 3000, 0x2000, SYS_PPU_THREAD_CREATE_JOINABLE, STOP_THREAD_NAME);

	uint64_t exit_code;
	if (ret == 0) sys_ppu_thread_join(t_id, &exit_code);

	sys_timer_usleep(7000);
	stop_prx_module();

	_sys_ppu_thread_exit(0);

	return SYS_PRX_STOP_OK;
}
