// Mysis xmb_plugin.h v0.1
enum plugins
{
	system_plugin=0x00,
	xmb_plugin=0x01,
	explore_plugin=0x02,
	category_setting_plugin=0x03,
	user_plugin=0x04,
	sysconf_plugin=0x05,
	netconf_plugin=0x06,
	software_update_plugin=0x07,
	edy_plugin=0x08,
	print_plugin=0x09,
	deviceconf_plugin=0x0A,
	photoviewer_plugin=0x0B,
	audioplayer_plugin=0x0D,
	sacd_plugin=0x0E,
	eula_cddb_plugin=0x0F,
	videoplayer_plugin=0x10,
	bdp_plugin=0x11,
	bdp_disccheck_plugin=0x12,
	bdp_storage_plugin=0x13,
	game_plugin=0x14,
	gamedata_plugin=0x15,
	game_ext_plugin=0x16,
	ps3_savedata_plugin=0x17,
	vmc_savedata_plugin=0x18,
	premo_plugin=0x1A,
	webbrowser_plugin=0x1B,
	webrender_plugin=0x1C,
	xai_plugin=0x1D,
	friendim_plugin=0x1E,
	friendml_plugin=0x1F,
	avc_plugin=0x20,
	avc2_text_plugin=0x21,
	nas_plugin=0x22,
	npsignin_plugin=0x23,
	np_trophy_plugin=0x24,
	np_trophy_ingame=0x25,
	friendtrophy_plugin=0x26,
	profile_plugin=0x27,
	videodownloader_plugin=0x28,
	download_plugin=0x29,
	thumthum_plugin=0x2A,
	micon_lock_plugin=0x2B,
	dlna_plugin=0x2C,
	strviewer_plugin=0x2D,
	playlist_plugin=0x2F,
	newstore_plugin=0x31,
	hknw_plugin=0x32,
	kensaku_plugin=0x34,
	regcam_plugin=0x35,
	idle_plugin=0x36,
	filecopy_plugin=0x37,
	wboard_plugin=0x38,
	poweroff_plugin=0x39,
	videoeditor_plugin=0x3A,
	scenefolder_plugin=0x3B,
	eula_hcopy_plugin=0x3C,
	mtpinitiator_plugin=0x3E,
	campaign_plugin=0x3F,
	remotedownload_plugin=0x40
};

typedef struct
{
	int (*GetPluginIdByName)(const char *);      // char * plugin name - get plugin id by string name
	int (*GetPluginNameById)(int);               // int value (Plugin Enum ID) - gets plugin name string by Id
	int (*IsPluginViewAvailable)(int) ;          // can test if plugin view is available or not. (Result != 0)
	int (*LoadPlugin3)(int, void *, int) ;       // Loads corresponding plugin and executes the thread for view/interface usage
	int (*LoadPlugin4)(int *) ;                  // Utility::LoadPlugin, 1 Parameter: uint * { Plugin Enum Id, int, int, int }
	int (*Shutdown)(int,void *,int) ;            // int value (Plugin Enum Id), void * callback, int
	int (*DoUnk6)(int *) ;                       // shutdown as well?
	int (*DoUnk7)(int) ;                         // int value (Plugin Enum ID) - _paf_99919819 - (module activate?)
	int (*ActivatePlugin)(int) ;                 // int value (Plugin Enum ID)
	int (*DoUnk9)(int) ;                         // int value (Plugin Enum ID) to store
	int (*DoUnk10)(int) ;                        // int value to store - (module deactivate?)
	int (*DoUnk11)(int,int) ;
	int (*DoUnk12)(int,int,int) ;                // int (Plugin Enum Id) , int value (Plugin Enum ID), int value to store - SendFinalizeEndNotify?
	int (*DoUnk13)(int *) ;                      // uint* [0xED8] - receives
	int (*DoUnk14)(int *) ;                      // uint* [0xED8] - sets
	int (*DoUnk15)(int *) ;                      // struct *ptr
	int (*GetModuleLoadOpinion)(int) ;           // int value (Plugin Enum ID)
	int (*SetModuleLoadOpinion)(int, long unk) ; // int value (Plugin Enum ID), uint64 unknown - stores unknown value onto xmb plugin enum id's struct
	int (*DoUnk18)(void *, int) ;                // void * handler, int
	int (*DoUnk19)(void *, int) ;                // void * handler, int
	int (*DoUnk20)(int *, int *) ;               // out: int *, out: int *
	int (*DoUnk21)(int, int) ;                   // int value (Plugin Enum ID), int value (0 = _vshnet_965A7E9D( ) ,else= _vshnet_52122E7D(("PS3 %s"), ))
	int (*DoUnk22)(int) ;                        // executes given Plugin ID MOD0-Inferface Function 4)
	int (*DoUnk23)(const char *,int) ;           // Executes Action
	int (*DoUnk24)(const char *,int) ;           // Gets Plugin ID Enum based by Action string
	int (*DoUnk25)(void) ;
} xmb_plugin_xmm0;

xmb_plugin_xmm0 * xmm0_interface;

typedef struct
{
	int (*DoUnk0)(int);                         // int value (0 - 0x14)
	int (*DoUnk1)(int,int);                     // int value (can be gotten from 13), 11) ), int value (0 - 0x14)
	int (*DoUnk2)(int, float);                  // int value, float value
	int (*DoUnk3)(void);                        //
	int (*DoUnk4)(void);                        // returns int (like 7) 0x1C0)
	int (*DoUnk5)(float);                       // float value
	float (*DoUnk6)(void);                      // returns float
	int (*DoUnk7)(int,int);                     // int, int - (example: See Table Function 7) - page_xmb page_xmb_indicator
	int (*DoUnk8)(void);                        //
	int (*DoUnk9)(int*,void*,const wchar_t *);  // int *, int* , wchar * infotext1
	int (*DoUnk10)(void);                       // - close page infobar?
	int (*DoUnk11)(int);                        // int
	int (*DoUnk12)(int);                        // int id
	int (*DoUnk13)(void);                       // returns int - (Hakoniwa/VideoEdit-Enabled Flag?)
	int (*DoUnk14)(void);                       // returns uint8
	int (*DoUnk15)(int,int);                    // calls 16) with int,int, 0
	int (*DoUnk16)(void *, int, int);           // uint8_t * struct[] ,int (gamepad port id),int explore_action (0 = return, 1 = FocusMessageBox,2 = FocusFriend,3 = impose_pad_battery_notice)
	int (*DoUnk17)(void);                       // returns struct according to gametool/gametool2
	int (*DoUnk18)(int,int,float);              // int, float, float
	int (*DoUnk19)(int,int);                    // int,int (0x0C memsets 2 regions)
	int (*DoUnk20)(void *);                     // in:  uint8 [0x38]
	int (*DoUnk21)(void *);                     // out: uint8 [0x38]
	int (*showMsg)(const wchar_t *);            // wchar * - page_xmb_indicator - Shows XMB-Notifications at the bottom like "(Triangle): Options"
	void * (*DoUnk23)(void);                    // returns ptr* ( ptr[0x908] = -1 -> ClearWBoardFocus)
	int (*DoUnk24)(void);                       // - _vshcommon_34A05733 ("xmb_plugin2", 0x15480) - returns some list
	void * (*DoUnk25)(void);                    // returns sys_lwmutex_t * "_pf_lwm"
	int (*DoUnk26)(int);                        // int value (0=decrease(+page_xmb_fg) + xmb2_interface_7(0,0x1C0/0x40,0x1C0),else=increase (page_xmb_fg))
	int (*DoUnk27)(void);                       // - calls category_setting_plugin CAT0 interface #24 + explore_plugin CAT0 interface #24
	int (*DoUnk28)(const char *);               // char* - Example: ""wbrd:show?"" , ""tchat:fullvsh_ps""
	int (*DoUnk29)(void);                       //
	int (*DoUnk30)(void *, int*);               // void *callback,uint8 [0x2F8] - set new custom_render_plugin callback and its parameter r8
	int (*DoUnk31)(int);                        // int value (0=page close, else= setwidget) - page_xmb_bg
	int (*DoUnk32)(int);                        // int value (0=page close, else= setwidget) - page_xmb_fg
} xmb_plugin_xmb2;

xmb_plugin_xmb2 * xmb2_interface;

typedef struct
{
	int (*DoUnk0)(void);
	int (*DoUnk1)(void);
} xmb_plugin_mod0;

xmb_plugin_mod0 * mod0_interface;
