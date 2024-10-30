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
int usb_emergency_update = 0;
int hen_restore = 0;

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

// Debug Log to external device
#define LOG_FILE_PATH "/dev_usb000/PS3HEN.log"
void DLOG(const char *format, ...);
void DLOG(const char *format, ...) {
    CellFsStat stat;
    CellFsErrno fsErr;
    uint64_t written;
    int fd;
    char buffer[256];

    fsErr = cellFsStat("/dev_usb000/", &stat);
    if (fsErr != CELL_FS_SUCCEEDED) {
        DPRINTF("HENPLUGIN->USB device not found: 0x%08x\n", fsErr);
        return;
    }

    fsErr = cellFsOpen(LOG_FILE_PATH, CELL_FS_O_WRONLY | CELL_FS_O_CREAT | CELL_FS_O_APPEND, &fd, NULL, 0);
    if (fsErr != CELL_FS_SUCCEEDED) {
        DPRINTF("HENPLUGIN->Failed to open log file: 0x%08x\n", fsErr);
        return;
    }

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    cellFsWrite(fd, buffer, strlen(buffer), &written);

    cellFsClose(fd);
}

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
#define SC_SYS_CONTROL_LED     386
#define LED_GREEN              1
#define LED_RED                2
#define LED_YELLOW             2 //RED+GREEN (RED alias due green is already on)
#define LED_OFF                0
#define LED_ON                 1
#define LED_BLINK_FAST         2
#define LED_BLINK_SLOW         3

static void led(uint64_t color, uint64_t mode);
static void led(uint64_t color, uint64_t mode) {
    system_call_2(SC_SYS_CONTROL_LED, color, mode);
}

// Resets all LEDs to OFF
static void reset_leds(void);
static void reset_leds(void) {
    led(LED_RED, LED_OFF);
    led(LED_GREEN, LED_OFF);
}

static void set_led(const char* preset);
static void set_led(const char* preset) {
    DPRINTF("HENPLUGIN->set_led->preset: %s\n", preset);
    reset_leds();  // Turn off all LEDs initially

    if (strcmp(preset, "install_start") == 0) {
        DPRINTF("HENPLUGIN->set_led->install_start\n");
        led(LED_GREEN, LED_BLINK_FAST);
    } else if (strcmp(preset, "install_success") == 0) {
        DPRINTF("HENPLUGIN->set_led->install_success\n");
        led(LED_GREEN, LED_ON);
    } else if (strcmp(preset, "install_failed") == 0) {
        DPRINTF("HENPLUGIN->set_led->install_failed\n");
        led(LED_RED, LED_BLINK_FAST);
    } else if (strcmp(preset, "off") == 0) {
        DPRINTF("HENPLUGIN->set_led->off\n");
        reset_leds();
    }
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
	
	const wchar_t* build_type_path = L"";
	const wchar_t* fw_version = L"";
	const wchar_t* kernel_type = L"";
	const wchar_t* pkg_suffix = L"";
	const wchar_t* pkg_url_tmp = L"http://www.ps3xploit.me/hen/%ls/%ls/%ls/installer/Latest_HEN_Installer%ls";
	const wchar_t* pkg_dl_path = L"/dev_hdd0";
	wchar_t pkg_url[256];
	
	if(use_wmm_pkg==1)
	{
		pkg_suffix=L"_WMM_signed.pkg";
	}
	else
	{
		pkg_suffix=L"_signed.pkg";
	}
	
	if(build_type==RELEASE)
	{
		build_type_path=L"release";
	}
	else
	{
		build_type_path=L"dev";
	}
	
	
	// 4.80 CEX
	// Kernel offset is off by 0x10 so we are checking this value instead of the timestamp
	if(val==0x3A32350000000000ULL)
	{
		fw_version = L"480";
		kernel_type = L"cex";
	}
	
	// 4.81 CEX
	else if(val==0x323031362F31302FULL)
	{
		fw_version = L"481";
		kernel_type = L"cex";
	}
	
	// 4.82 CEX
	else if(val==0x323031372F30382FULL)
	{
		fw_version = L"482";
		kernel_type = L"cex";
	}
		
	// 4.83 CEX
	else if(val==0x323031382F30392FULL)
	{
		fw_version = L"483";
		kernel_type = L"cex";
	}
		
	// 4.84 CEX
	else if(val==0x323031392F30312FULL)
	{
		fw_version = L"484";
		kernel_type = L"cex";
	}
		
	// 4.85 CEX
	else if(val==0x323031392F30372FULL)
	{
		fw_version = L"485";
		kernel_type = L"cex";
	}
		
	// 4.86 CEX
	else if(val==0x323032302F30312FULL)
	{
		fw_version = L"486";
		kernel_type = L"cex";
	}
		
	// 4.87 CEX
	else if(val==0x323032302F30372FULL)
	{
		fw_version = L"487";
		kernel_type = L"cex";
	}
		
	// 4.88 CEX
	else if(val==0x323032312F30342FULL)
	{
		fw_version = L"488";
		kernel_type = L"cex";
	}
		
	// 4.89 CEX
	else if(val==0x323032322F30322FULL)
	{
		fw_version = L"489";
		kernel_type = L"cex";
	}
		
	// 4.90 CEX
	// Kernel offset is off by 0x10 so we are checking this value instead of the timestamp
	else if(val==0x3A35340000000000ULL)
	{
		fw_version = L"490";
		kernel_type = L"cex";
	}
		
	// 4.91 CEX
	else if(val==0x323032332F31322FULL)
	{
		fw_version = L"491";
		kernel_type = L"cex";
	}

	// 4.82 DEX
	else if(valD==0x323031372F30382FULL)
	{
		fw_version = L"482";
		kernel_type = L"dex";
	}
		
	// 4.84 DEX
	else if(valD==0x323031392F30312FULL)
	{
		fw_version = L"484";
		kernel_type = L"dex";
	}
		
	//DPRINTF("HENPLUGIN->build_type_path: %ls\n",(char*)build_type_path);
	//DPRINTF("HENPLUGIN->pkg_url_tmp: %ls\n",(char*)pkg_url_tmp);
	//DPRINTF("HENPLUGIN->pkg_dl_path: %ls\n",(char*)pkg_dl_path);
	//DPRINTF("HENPLUGIN->fw_version: %ls\n",(char*)fw_version);
	//DPRINTF("HENPLUGIN->kernel_type: %ls\n",(char*)kernel_type);
	//DPRINTF("HENPLUGIN->pkg_suffix: %ls\n",(char*)pkg_suffix);
	
	swprintf(pkg_url, sizeof(pkg_url), pkg_url_tmp, build_type_path, fw_version, kernel_type, pkg_suffix);
	DPRINTF("HENPLUGIN->pkg_url: %ls\n",(char*)pkg_url);
	download_interface->DownloadURL(0, pkg_url, (wchar_t*)pkg_dl_path);
	
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
	DPRINTF("HENPLUGIN->plugin shutdown via xmb call launched");
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

// Toggles can be accessed by HFW Tools menu
// Clear Browser and PSN Cache (thanks xfrcc for original idea)
// Clear PSN cache (thanks LuanTeles)
void clear_web_cache_check(void);
void clear_web_cache_check(void) {
    DPRINTF("HENPLUGIN->Clear Web Cache Check Started\n");
    int userNumber = xsetting_CC56EB2D()->GetCurrentUserNumber();
    CellFsStat stat;
    int cleared_total = 0;
    const char* paths[] = {
        "/dev_hdd0/home/%08i/webbrowser/history.xml",
        "/dev_hdd0/home/%08i/http/auth_cache.dat",
        "/dev_hdd0/home/%08i/http/cookie.dat",
        "/dev_hdd0/home/%08i/community/CI.TMP",
        "/dev_hdd0/home/%08i/community/MI.TMP",
        "/dev_hdd0/home/%08i/community/PTL.TMP"
    };
    const char* toggles[] = {
        "/dev_hdd0/hen/toggles/clear_web_history.on",
        "/dev_hdd0/hen/toggles/clear_web_auth_cache.on",
        "/dev_hdd0/hen/toggles/clear_web_cookie.on",
        "/dev_hdd0/hen/toggles/clear_ci.on",
        "/dev_hdd0/hen/toggles/clear_mi.on",
        "/dev_hdd0/hen/toggles/clear_ptl.on"
    };

    char msg[0x400];
    int msg_length = 0;

    for (int i = 0; i < 6; i++) {
        char path[0x80];
        snprintf(path, sizeof(path), paths[i], userNumber);
        if (cellFsStat(path, &stat) == 0 && cellFsStat(toggles[i], &stat) == 0) {
            //DPRINTF("HENPLUGIN->path: %s\n", paths[i]);
            DPRINTF("HENPLUGIN->Toggle Activated: %s\n", toggles[i]);
            cellFsUnlink(path);
            
            // Extract only the filename from the path
            const char* filename = strrchr(path, '/');
            if (filename != NULL) {
                filename++; // Move past the '/'
            } else {
                filename = path; // Use the whole path if '/' is not found
            }

            int item_length = snprintf(msg + msg_length, sizeof(msg) - msg_length, "Cleared %s\n", filename);
            //DPRINTF("HENPLUGIN->msg: %s\n", msg);
            if (item_length < 0 || (size_t)msg_length + item_length >= sizeof(msg)) {
                DPRINTF("HENPLUGIN->Error: Message buffer overflow\n");
                break;
            }
            msg_length += item_length;
            cleared_total++;
        }
    }

    if (cleared_total > 0) {
        DPRINTF("HENPLUGIN->Clear Web Cache Check Finished\n");
        show_msg(msg);
    } else {
        DPRINTF("HENPLUGIN->No Clear Web Cache Toggles Activated\n");
    }
}

void set_build_type(void);
void set_build_type(void)
{
    CellFsStat stat;

    // Check the internal HDD path first
    if (cellFsStat("/dev_hdd0/hen/toggles/dev_build_type.on", &stat) == 0) {
        build_type = DEV;
        DPRINTF("HENPLUGIN->Found dev_build_type.on at /dev_hdd0/hen/toggles/\n");
    } else {
        DPRINTF("HENPLUGIN->Checking for dev_build_type.on at /dev_hdd0/hen/toggles/\n");

        // Loop through USB paths 000 to 007
        for (int i = 0; i < 8; i++) {
            char usb_path[32];
            snprintf(usb_path, sizeof(usb_path), "/dev_usb00%d/dev_build_type.on", i);
            
            if (cellFsStat(usb_path, &stat) == 0) {
                build_type = DEV;
                DPRINTF("HENPLUGIN->Found dev_build_type.on at %s\n", usb_path);
                break;
            } else {
                DPRINTF("HENPLUGIN->Checking for dev_build_type.on at %s\n", usb_path);
            }
        }
    }

    DPRINTF("HENPLUGIN->Setting build_type to %i\n", build_type);
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
	DPRINTF("HENPLUGIN->Begin restore_act_dat\n");
	
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
	
	DPRINTF("HENPLUGIN->Done restore_act_dat\n");
}

static int sysLv2FsMkdir(const char *path, int mode)
{
    system_call_2(811, (uint64_t)(uintptr_t)path, (uint64_t)mode);
    return_to_user_prog(int);
}

static int sysLv2FsRename(const char *from, const char *to)
{
    system_call_2(812, (uint64_t)(uintptr_t)from, (uint64_t)(uintptr_t)to);
    return_to_user_prog(int);
}

// Create default directories if they do not exist (thanks LuanTeles)
void create_default_dirs(void);
void create_default_dirs(void) {
    DPRINTF("HENPLUGIN->Begin checking and creating default directories under /dev_hdd0/\n");

    const char* dirs[] = {
        "/dev_hdd0/BDISO", "/dev_hdd0/DVDISO", "/dev_hdd0/GAMES", "/dev_hdd0/PS2ISO",
        "/dev_hdd0/PS3ISO", "/dev_hdd0/PSPISO", "/dev_hdd0/PSXISO", "/dev_hdd0/ROMS",
        "/dev_hdd0/exdata", "/dev_hdd0/packages", "/dev_hdd0/plugins", "/dev_hdd0/theme",
        "/dev_hdd0/updater", "/dev_hdd0/updater/01"
    };
    CellFsStat stat;
    size_t dirCount = sizeof(dirs) / sizeof(dirs[0]);
    for (size_t i = 0; i < dirCount; ++i) {
        if (cellFsStat(dirs[i], &stat) != CELL_OK) {
            if (sysLv2FsMkdir(dirs[i], CELL_FS_S_IFDIR | 0777) != CELL_OK) {
                DPRINTF("HENPLUGIN->Error creating directory: %s\n", dirs[i]);
            }
        }
    }

    DPRINTF("HENPLUGIN->Done checking and creating default directories under /dev_hdd0/\n");
}

// Shamelessly taken and modified from webmanMOD (thanks aldostools)
static void play_rco_sound(const char *sound)
{
	View_Find = getNIDfunc("paf", 0xF21655F3, 0);
	uint32_t plugin = View_Find("system_plugin");
	if(plugin)
	{
		PlayRCOSound(plugin, sound, 1, 0);
		DPRINTF("HENPLUGIN->PlayRCOSound(%0X, %s, 1, 0)\n",plugin,sound);
	}
}

static void close_browser_plugins(void);
static void close_browser_plugins(void)
{
	View_Find = getNIDfunc("paf", 0xF21655F3, 0);
	int is_browser_open=View_Find("webbrowser_plugin");
	
	// Silk
	DPRINTF("HENPLUGIN->close_browser_plugins: silk \n");
	while(is_browser_open)
	{	
		sys_timer_usleep(70000);
		is_browser_open=View_Find("webbrowser_plugin");
	}
	
	// Webkit
	DPRINTF("HENPLUGIN->close_browser_plugins: webkit \n");
	is_browser_open=View_Find("webrender_plugin");
	while(is_browser_open)
	{
		sys_timer_usleep(70000);
		is_browser_open=View_Find("webrender_plugin");
	}
	DPRINTF("HENPLUGIN->unload_web_plugins \n");
	unload_web_plugins();
}

static void package_install(void);
static void package_install(void)
{
	DPRINTF("HENPLUGIN->package_install: %s \n", pkg_path);
	CellFsStat stat;
	
	LoadPluginById(0x16, (void *)installPKG_thread);
	
	if(cellFsStat(pkg_path,&stat)==0)
	{
		if (usb_emergency_update==1)
		{
			char manual_reboot_txt[0x80];
			sprintf(manual_reboot_txt, "HEN_UPD.PKG installing from USB.\nWhen finished, reboot manually.");
			show_msg((char *)manual_reboot_txt);
		}
		else 
		{
			// Original logic for waiting for package installation to complete
			DPRINTF("HENPLUGIN->IS_INSTALLING: %08X\nthread3_install_finish: %i\n",IS_INSTALLING,thread3_install_finish);
			while (!thread3_install_finish || IS_INSTALLING)
			{
				DPRINTF("HENPLUGIN->IS_INSTALLING: %08X\n",IS_INSTALLING);
				sys_timer_usleep(2000000); // check every 2 seconds
			}
			reboot_flag=1;
		}
	}
}

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
	//DPRINTF("HENPLUGIN->hen_version: %x\n",hen_version);
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

	// Emergency USB HEN Installer
	for (int i = 0; i < 8; i++) {
		char usb_path[32];
		snprintf(usb_path, sizeof(usb_path), "/dev_usb00%d/HEN_UPD.pkg", i);

		// Check if the package exists on the current USB path
		if (cellFsStat(usb_path, &stat) == 0) {
			DPRINTF("HENPLUGIN->Checking for emergency installer on %s\n", usb_path);
			
			usb_emergency_update = 1;  // Set USB Emergency Update Flag
			//set_led("install_start");
			play_rco_sound("snd_trophy");
			DPRINTF("HENPLUGIN->Installing Emergency Package From %s\n", usb_path);
			
			// Set pkg_path to the found USB path
			memset(pkg_path, 0, 256);
			strcpy(pkg_path, usb_path);
			
			close_browser_plugins();
			package_install();
			
			//sys_timer_usleep(20000000); // wait 20 seconds to allow package to finish
			//set_led("install_success");
			
			goto done;
		} else {
			DPRINTF("HENPLUGIN->Checking for emergency installer on %s\n", usb_path);
		}
	}
	
	// Create default directories for BDISO, PSXISO, PS2ISO, PS3ISO, PSPISO, etc
	create_default_dirs();
	
	// restore act.dat from act.bak backup
	restore_act_dat();
	
	// If default HEN Check file is missing, assume HEN is not installed
	do_install_hen=(cellFsStat("/dev_flash/vsh/resource/explore/icon/hen_enable.png",&stat));
	//DPRINTF("HENPLUGIN->do_install_hen: %x\n",do_install_hen);
	
	// Default restore option check for use if HDD formatted game folder missing (3.3.1+)
	hen_restore=(cellFsStat("/dev_hdd0/game/PS3XPLOIT/USRDIR/hen.installed",&stat));
	DPRINTF("HENPLUGIN->hen_restore: %x\n",hen_restore);
	
	// Check for webMAN-MOD
	if((cellFsStat("/dev_hdd0/plugins/webftp_server.sprx",&stat)==0) || (cellFsStat("/dev_hdd0/plugins/webftp_server_lite.sprx",&stat)==0))
	{
		is_wmm_installed=1;
		DPRINTF("HENPLUGIN->WMM Detected\n");
	}
	
	// Display message about the removal of boot plugins
	// Created from payload if HEN is installing, so plugins cannot be loaded
	if(cellFsStat("/dev_hdd0/tmp/installer.active",&stat)==0)
	{
		is_hen_installing=1;
		//play_rco_sound("snd_trophy");
		char msg_boot_plugins[0x80];
		sprintf(msg_boot_plugins, "Boot Plugins Text Have Been\nTemporarily Disabled Until Next Reboot!");
		show_msg((char *)msg_boot_plugins);
		cellFsUnlink("/dev_hdd0/tmp/installer.active");
	}
	
	do_update=(cellFsStat("/dev_hdd0/hen_updater.off",&stat) ? hen_updater() : 0);// 20211011 Added update toggle thanks bucanero for original PR
	//DPRINTF("HENPLUGIN->Checking do_update: %i\n",do_update);
	
	// Removing temp installer packages so old ones can't be installed
	DPRINTF("HENPLUGIN->Removing Temp Installer Packages\n");
	cellFsUnlink("/dev_hdd0/Latest_HEN_Installer_signed.pkg");
	cellFsUnlink("/dev_hdd0/Latest_HEN_Installer_WMM_signed.pkg");
	
	if((do_install_hen!=0) || (do_update==1) || hen_restore!=0)
	{
		set_led("install_start");
		close_browser_plugins();
		
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
			//DPRINTF("HENPLUGIN->Waiting for package to finish downloading\n");
		}
		
		package_install();
			
		goto done;
	}
	else
	{   
		// Removing temp packages
		//cellFsUnlink(pkg_path);
		//cellFsUnlink("/dev_hdd0/Latest_HEN_Installer_signed.pkg");
		//cellFsUnlink("/dev_hdd0/Latest_HEN_Installer_WMM_signed.pkg");
	}
	
done:
	// Restore boot_plugins files
	DPRINTF("HENPLUGIN->Restoring boot plugin files\n");
	if(cellFsStat("/dev_hdd0/boot_plugins.hen",&stat)==0)
	{
		sysLv2FsRename("/dev_hdd0/boot_plugins.hen", "/dev_hdd0/boot_plugins.txt");
	}

	if(cellFsStat("/dev_hdd0/boot_plugins_kernel.hen",&stat)==0)
	{
		sysLv2FsRename("/dev_hdd0/boot_plugins_kernel.hen", "/dev_hdd0/boot_plugins_kernel.txt");
	}
	
	DPRINTF("HENPLUGIN->Removing temp files\n");
	cellFsUnlink("/dev_hdd0/theme/PS3HEN.p3t");// Removing temp HEN installer
	cellFsUnlink("/dev_hdd0/HENplugin.sprx");// Removing temp HEN Plugin SPRX
	
	done=1;
	
	DPRINTF("HENPLUGIN->Done! Preparing to exit thread\n");	
	
	if(reboot_flag==1)
	{	
		DPRINTF("HENPLUGIN->Reboot flag is active\n");
		
		set_led("install_success");
		play_rco_sound("snd_trophy");
		
		char reboot_txt[0x80];
		sprintf(reboot_txt, "Installation Complete!\n\nPrepare For Reboot...");
		show_msg((char *)reboot_txt);
		sys_timer_usleep(10000000);// Wait a few seconds
		reboot_ps3();// Default Soft Reboot
	}
	
	clear_web_cache_check();// Clear WebBrowser cache check (thanks xfrcc)
	
	DPRINTF("HENPLUGIN->Exiting main thread!\n");	
	
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
