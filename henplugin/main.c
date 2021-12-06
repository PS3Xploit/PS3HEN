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
#include "common.h"
#include "stdc.h"
#include "download_plugin.h"
#include "game_ext_plugin.h"
#include "xmb_plugin.h"

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
#define HOST_SERVER "www.ps3xploit.com"
 
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

extern int vshmain_87BB0001(int param);
int (*vshtask_notify)(int, const char *) = NULL;

int (*vshmain_is_ss_enabled)(void) = NULL;
int (*View_Find)(const char *) = NULL;
void *(*plugin_GetInterface)(int,int) = NULL;

int (*set_SSHT_)(int) = NULL;

int opd[2] = {0, 0};

#define IS_INSTALLING	(View_Find("game_plugin") != 0)
#define IS_INSTALLING_NAS	(View_Find("nas_plugin") != 0)
#define IS_DOWNLOADING	(View_Find("download_plugin") != 0)

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
	int (*DoUnk11)(void); // 3 Parameter: char * query , char * attribute? , uint8 output[]
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
static void enable_ingame_screenshot(void)
{
	((int*)getNIDfunc("vshmain",0x981D7E9F,0))[0] -= 0x2C;
}

int sys_map_path(char *old, char *new);
int sys_map_path(char *old, char *new)
{
	system_call_2(35, (uint64_t)(uint32_t)old,(uint64_t)(uint32_t)new);
	return (int)p1;
}

static void reload_xmb(void)
{
	while(!IS_ON_XMB)
	{
		sys_timer_usleep(70000);
	}
	explore_interface->ExecXMBcommand("reload_category game",0,0);
	explore_interface->ExecXMBcommand("reload_category network",0,0);
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

static void stop_prx_module(void)
{
	sys_prx_id_t prx = prx_get_module_id_by_address(stop_prx_module);
	int *result=NULL;

	{system_call_6(SC_STOP_PRX_MODULE, (uint64_t)prx, 0, NULL, (uint64_t)(uint32_t)result, 0, NULL);}

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

//int is_hen_installed=0;
int thread2_download_finish=0;
int thread3_install_finish=0;

#define SYSCALL_PEEK	6
	
static uint64_t peekq(uint64_t addr)
{
	system_call_1(SYSCALL_PEEK, addr);
	return_to_user_prog(uint64_t);
}


static void downloadPKG_thread2(void)
{

	if(download_interface == 0) // test if download_interface is loaded for interface access
	{
		download_interface = (download_plugin_interface *)plugin_GetInterface(View_Find("download_plugin"), 1);
	}
	show_msg((char *)"Downloading latest HEN pkg");
	uint64_t val=peekq(0x80000000002FCB68ULL);
	if(val==0x323031372F30382FULL) 
		{
			download_interface->DownloadURL(0, (wchar_t *) L"http://ps3xploit.com/hen/release/482/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
		}
	else if(val==0x323031392F30312FULL)
		{
			download_interface->DownloadURL(0,(wchar_t *) L"http://ps3xploit.com/hen/release/484/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
		}	
	else if(val==0x323031392F30372FULL)
		{
			download_interface->DownloadURL(0,(wchar_t *) L"http://ps3xploit.com/hen/release/485/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
		}	
	else if(val==0x323032302F30312FULL)
		{
			download_interface->DownloadURL(0,(wchar_t *) L"http://ps3xploit.com/hen/release/486/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
		}	
	else if(val==0x323032302F30372FULL)
		{
			download_interface->DownloadURL(0,(wchar_t *) L"http://ps3xploit.com/hen/release/487/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
		}	
	else if(val==0x323032312F30342FULL)
		{
			download_interface->DownloadURL(0,(wchar_t *) L"http://ps3xploit.com/hen/release/488/cex/installer/Latest_HEN_Installer_signed.pkg", (wchar_t *) L"/dev_hdd0");
		}	
	thread2_download_finish=1;
}

char pkg_path[256]={"/dev_hdd0/Latest_HEN_Installer_signed.pkg"};

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
    strcat(RequestBuffer, "/hen/hen_version.bin");
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

static void henplugin_thread(__attribute__((unused)) uint64_t arg)
{
	View_Find = getNIDfunc("paf", 0xF21655F3, 0);
	plugin_GetInterface = getNIDfunc("paf", 0x23AFB290, 0);
	int view = View_Find("explore_plugin");
	system_call_1(8, SYSCALL8_OPCODE_HEN_REV); hen_version = (int)p1;
	char henver[0x30];
	//sprintf(henver, "Welcome to PS3HEN %X.%02X", hen_version>>8, (hen_version & 0xFF));
	sprintf(henver, "Welcome to PS3HEN %X.%X.%X", hen_version>>8, (hen_version & 0xF0)>>4, (hen_version&0xF));
	
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
	
	if(cellFsStat("/dev_usb000/HEN_UPD.pkg",&stat)==0)
	{
		memset(pkg_path,0,256);
		strcpy(pkg_path,"/dev_usb000/HEN_UPD.pkg");
		LoadPluginById(0x16, (void *)installPKG_thread);
		while(thread3_install_finish==0)
		{
			sys_timer_usleep(70000);
		}
		goto done;
	}
	int do_update=(cellFsStat("/dev_hdd0/hen_updater.off",&stat) ? hen_updater() : 0);// 20211011 Added update toggle thanks bucanero for original PR
	if((cellFsStat("/dev_flash/vsh/resource/explore/icon/hen_enable.png",&stat)!=0) || (do_update==1))
	{
		cellFsUnlink("/dev_hdd0/theme/PS3HEN.p3t");
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
		LoadPluginById(0x29,(void*)downloadPKG_thread2);
		
		while(thread2_download_finish==0)
		{
			sys_timer_usleep(70000);
		}
		
		while(IS_DOWNLOADING)
		{
			sys_timer_usleep(500000);
		}
		
		if(cellFsStat("/dev_hdd0/Latest_HEN_Installer_signed.pkg",&stat)==0)
		{
			LoadPluginById(0x16, (void *)installPKG_thread);
			while(thread3_install_finish==0)
			{
				sys_timer_usleep(70000);
			}
			goto done;
		}
	}
	else
	{    
		cellFsUnlink("/dev_hdd0/Latest_HEN_Installer_signed.pkg");
	}
	
done:
	DPRINTF("Exiting main thread!\n");	
	done=1;
	
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
extern int henplugin_stop(void);

int henplugin_stop()
{
	sys_ppu_thread_t t_id;
	int ret = sys_ppu_thread_create(&t_id, henplugin_stop_thread, 0, 3000, 0x2000, SYS_PPU_THREAD_CREATE_JOINABLE, STOP_THREAD_NAME);

	uint64_t exit_code;
	if (ret == 0) sys_ppu_thread_join(t_id, &exit_code);

	sys_timer_usleep(70000);
	unload_prx_module();

	_sys_ppu_thread_exit(0);

	return SYS_PRX_STOP_OK;
}
