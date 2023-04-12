#ifndef __PAF_H__
#define __PAF_H__

// paf_1565FC46  // ?

//extern void* paf_AF58E756(void);  // Gets the base of loaded plugins. The pointer returned is used in paf::View::Find to find plugins.
#define GetSPRXBaseAddr paf_AF58E756

// paf_A4819ADB  // ? main_mem / rwlock_unlock
// paf_0EAA28B8  // ?
// paf_59BDA198  // ? rsx command buffer

extern void paf_85D1D23B(void);  // ?, base pointer: pad_data / pad_lwmtx

// paf_A5B4FB55  // ? init libs: gcm, io, ...

extern uint32_t *paf_23AFB290(uint32_t arg, uint32_t id);  // gets Interface from plugin, 	uint32_t plugin->GetInterface(uint32_t return from paf_F21655F3, uint32_t identifier)
#define plugin_GetInterface paf_23AFB290

// paf_35A9C2F9  // ?
// paf_728BFDFC  // ?
// paf_F5BEB953  // ?
// paf_2C4E7D1C  // ?
// paf_0A1DC401  // sets Interface from plugin 	uint32_t SetInterface(unknown, uint32_t identifier, void *ptr_structure)
// paf_2361E649  // ?
// paf_2166E819  // ? PageActivate
// paf_E014D3BA  // ?

extern uint32_t paf_794CEACB(uint32_t plugin, const char* widget); // paf_794CEACB  // finds widget in a plugin 	Example: plugin->FindWidget(return from paf_F21655F3, "page_autooff_guide")
#define FindWidget paf_794CEACB

extern uint32_t paf_50CDB465(uint32_t plugin, const char *sound);  // finds/gets Sound resource 	uint32_t paf_50CDB465(uint32_t plugin /*paf_F21655F3*/, const char *sound)
#define GetRCOSound paf_50CDB465

extern void paf_B93AFE7E(uint32_t plugin, const char *sound, float arg1, int arg2);  // Example: paf_B93AFE7E( paf_F21655F3("system_plugin"), "snd_trophy", 1, 0)
#define PlayRCOSound paf_B93AFE7E
//Example: PlayRCOSound( FindLoadedPlugin("system_plugin"), "snd_trophy", 1, 0)

extern uint32_t paf_3A8454FC(int32_t* texptr, uint32_t plugin, const char* name); // return the texture address (atleast that's what i imagine what it does) by plugin name and texture name.
//extern uint32_t *paf_3A8454FC(uint32_t* texture, uint32_t plugin, const char *texture_name);
#define LoadRCOTexture paf_3A8454FC

//void(*vshcommon_A20E43DB)(int32_t, const char* eventName, int32_t, int32_t* texture, int32_t*, const char*, const char*, float, const wchar_t* text, int32_t, int32_t, int32_t);
//int dummy = 0; vshcommon_A20E43DB(0, const char* eventName, int32_t, int32_t* texture /*paf_3A8454FC*/, &dummy, "", "", 0f, L"notification", 0, 0, 0)

// paf_B96A18B0  // ? texture
// paf_A499E2BE  // ? texture
// paf_3A8454FC  // finds/gets Texture resource 	Example: out: uint32_t* texture, return from paf_F21655F3, "tex_notification_info"
extern wchar_t *paf_89B67B9C(uint32_t plugin, const char *text);  // gets localized string text from plugin resource (Different strings are returned depending on the system language) 	Example: (w)char (paf_F21655F3("system_plugin"), "msg_signed_out")
#define LoadRCOText paf_89B67B9C
// paf_E50657B1  // ?
// paf_11E195B3  // Finds Resource Object (located in .rco) 	void* paf_11E195B3(uint32_t plugin, const char* object) // Example: paf_B93AFE7E( paf_F21655F3("system_plugin"), "page_notification")

// paf_75457863  // ? close page
// paf_3CEC3833  // ? sets up a plugin widget ? (page_...)

extern uint32_t paf_F21655F3(const char *sprx_name);  // finds a loaded plugin 	uint32_t paf::View::Find(const char* plugin) Example: uint32_t paf::View::Find("system_plugin")
#define FindLoadedPlugin paf_F21655F3
#define View_Find paf_F21655F3

// paf_65BE85B3  // _UnloadView 	_paf_65BE85B3(char *sprx_name, 0, 0)
// paf_C6B89CEE  // ?
// paf_28B0A5D9  // ?

// paf_B9152E15  // ? interface
//extern void paf_2F5CEE6D();  // stop and unload sprx 	void paf_2F5CEE6D(int32_t* prx_struct)
// paf_CF068D31  // load (optional on memcontainer) and start sprx 	void paf_CF068D31(int32_t* prx_struct, const char* path, int /*ignored*/, int /*0*/, (optional) const sys_memory_container_t *mem_container)
// paf_08598228  // ?
// paf_01D59A4E  // ?

// paf_D2CBEDF8  // ?
// paf_E22D7B0C  // ?
// extern int32_t paf_EB793E25(void);  // ret -1
// extern int32_t paf_A28C8100(void);  // ret -1
// extern int32_t paf_B8979AD7(void);  // ret -1
// extern int32_t paf_57B6F6B2(void);  // ret -1
// extern int32_t paf_C35764DF(void);  // ret -1
// extern int32_t paf_DF3A2CA7(void);  // ret -1
// extern int32_t paf_BF575328(void);  // ret -1
// extern int32_t paf_29C739F4(void);  // ret -1

// paf_CC3EC021  // ?
// paf_0BA57D3D  // ?
// paf_05063ACE  // ?
// paf_16D6FF8C  // ?
// paf_35DD3495  // ?
// paf_93BE3FFD  // ?
// paf_329E92C6  // ?
// paf_6840C10B  // ?
// paf_76341300  // ?
// paf_931F2317  // ?

// paf_9A482757  // ?
// paf_4D79D004  // ?
// paf_48ED55DE  // ?
// paf_869F0A81  // ?
// paf_017AF3F8  // ?
// paf_07404EC3  // ?
// paf_8C0743AA  // ?
// paf_338E0ABD  // ?
// paf_3182E966  // ?
// paf_62D80499  // ?
// paf_2B16B368  // ?
// paf_F706662E  // ?
// paf_C64E88CC  // ?
// paf_FAC1637A  // ?
// paf_C2C6F5E9  // ?
// paf_A9A8352C  // ?
// paf_A6E24DB4  // ?
// paf_DFFC18F9  // ?

// paf_93E4A094  // ?
// paf_28CF8811  // ?
// paf_60CF1B3C  // ?
// paf_FC88E588  // ?

// paf_8C7A6C86  // ?
// paf_01DE8773  // ?
// paf_1261112B  // ?

// paf_022BF05A  // ?
// paf_E06D639B  // ?
// paf_857BFE1C  // ?
// paf_5F7E8C84  // ?
// paf_6C0930C4  // ?

// paf_02AB5840  // ?
// paf_40211524  // ?
// paf_48658F0A  // ?

// paf_9B68942A  // ?
// paf_88EAEDCB  // ?

// paf_8DBFF0E4  // ?
// paf_C05E02B1  // ?
// paf_0C96D94F  // ?

// paf_8BB8EDAA  // ?
// paf_AC9394C5  // ?

// paf_880A3367  // ?
// paf_B09E098F  // ?
// paf_1F31959E  // ? set functionspointer, system_plugin, "tex_default_text_focus"
// paf_8BC095CF  // ? set functionspointer, system_plugin, "tex_msgdialog_bar"
// paf_786FBB2D  // ? set functionspointer, system_plugin, "tex_default_scroll_..."
// paf_84FC5B16  // ? set functionspointer, system_plugin, "tex_default_progress_..."
// paf_84D7A733  // ? set functionspointer, system_plugin, "snd_..."
// paf_D31D0770  // ? set and ret old
// int32_t paf_FD14966A  // ? get

// paf_03283B28  // ?
// paf_3899A4AA  // ?
// paf_38A64783  // ?
// paf_C3087A3E  // ?
// paf_17C07E8F  // ?

// paf_00440E11  // ?
// paf_41E5C922  // ?
// paf_B7DDF1AD  // ?
// paf_B50187AF  // ?
// paf_2609FF4E  // ?
// paf_2D5C7C8D  // ?
// paf_CC47CC04  // ?
// paf_37002D7A  // ?

// paf_05050439  // ? "mbox_seat"

// paf_56E1ACC6  // ? get a u32 proc addr, return int 0

// paf_9EE0EA8F  // ?
// paf_D43ED139  // ?
// paf_74EFEB13  // ?
// paf_F9829DF0  // ?
// paf_31D6869E  // ?
// paf_04C19A94  // ?
// paf_1010156F  // ?

// paf_983EA578  // ? font
// paf_F72BABD5  // ?
// paf_638A0B38  // ? load/unload prx
// paf_A12E9E8B  // ?

// paf_C7630260  // ?

// paf_BE8E04CE  // ? lock mtx
// paf_C0CDB967  // ? unlock mtx

// paf_763D3390  // ?

extern const char *paf_546B3D02(void);  // returns "PhWidget"

// paf_FF527836  // ?
// paf_00C9AFA5  // ?
// paf_ADE746B6  // ?

// paf_41DC3A12  // ?
// paf_C83D7652  // ?

// paf_12CACB14  // ?
// paf_6458160F  // ?

// paf_0241B884  // ?
// paf_04EC636E  // ?

// paf_9AB6B3EC  // ?
// paf_6CD13DF4  // ?

// paf_75295C38  // ?

// paf_5364F246  // ?

// paf_E1F0804D  // ?

// paf_016A3B00  // ?

// paf_0A9E290A  // ?
// paf_9DAFD7F7  // ?
// paf_020B8A99  // ?

// paf_04EB288F  // ?

// paf_7A200FA1  // ?

// paf_35556CA6  // ?


extern const char *paf_41BBFE5E(void);  // returns "PhScene"

// paf_1802C687  // ?

extern const char *paf_10DEDCC7(void);  // returns "PhPlane"

extern const char *paf_E36C18F5(void);  // returns "PhPlaneDiv"

// paf_0C16A258  // ?
// paf_8ABAE2F3  // ?

extern const char *paf_24A5BD6B(void);  // returns "PhButton"

// paf_4427D818  // ?

// paf_8A93BC70  // ?

// paf_C9008050  // ?

// paf_344BAB53  // ?

extern const char *paf_B7DFCE90(void);  // returns "PhText"
// paf_62685916  // ?
// paf_B37491C6  // ?

// paf_022FDDB8  // ?
// paf_70AEB483  // ?

extern const char *paf_009207F4(void);  // returns "PhScroll"

extern const char *paf_BA6D149A(void);  // returns "PhLabelPrim"

extern const char *paf_C88CA4B2(void);  // returns "PhLevelMeter"

extern const char *paf_E801C345(void);  // returns "PhProgress"

extern const char *paf_BF66BF2D(void);  // returns "PhCheckBox"

// paf_B7377945  // _ZN3paf10PhCheckBox6CreateEv	paf::PhCheckBox::Create(void)
// paf_5A85BEFC  // _ZN3paf10PhCheckBox8SetCheckEbb	paf::PhCheckBox::SetCheck(bool, bool)

// paf_176E3BC4  // _ZN3paf10PhCheckBoxC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhCheckBox::PhCheckBox(paf::PhWidget *, paf::PhAppear *)

extern const char *paf_703117AD(void);  // returns "PhXmBar"

// paf_C1A00166  // _ZN3paf7PhXmBar13GetListWidgetEi	paf::PhXmBar::GetListWidget(int)

// paf_DDE862B5  // _ZN3paf7PhXmBar17SetOtherItemAlphaEfff	paf::PhXmBar::SetOtherItemAlpha(float, float, float)

// paf_B24DE5AC  // _ZN3paf7PhXmBar16SetOtherTopAlphaEfff	paf::PhXmBar::SetOtherTopAlpha(float, float, float)
// paf_5837B7E2  // _ZN3paf7PhXmBar16SetTopLabelAlphaEfff	paf::PhXmBar::SetTopLabelAlpha(float, float, float)

// paf_9976D7EA  // ?
// paf_85BAAAA8  // _ZN3paf7PhXmBar8IconMoveERK4vec4ffbiii	paf::PhXmBar::IconMove(vec4 const&, float, float, bool, int, int, int)
// paf_6C8F62F7  // _ZN3paf7PhXmBar19GetFocusedItemIndexEi	paf::PhXmBar::GetFocusedItemIndex(int)

extern const char *paf_4FF7B8A9(void);  // returns "PhXmList"
// paf_5DBA65AA  // ?
// paf_01059AF3  // ?

// paf_007D4D35  // ?
// paf_2D312F8F  // ?

// paf_07047F37  // ?

// paf_81840F26  // ?

extern const char *paf_C84FD77B(void);  // returns "PhXmItem"

// paf_E8149213  // ?

// paf_04081C1E  // ?

// paf_97141C43  // ?
// paf_BA3A5223  // ?
// paf_D8734249  // ?
// paf_5DF8473F  // ?
// paf_83123FE2  // ?

// paf_D8E06B04  // ?
// paf_8BFC59D7  // ?

// paf_F3BA0BD0  // ?

// paf_C5B890DA  // ?
// paf_CB134D56  // ?

// paf_9F9E8F9D  // ?

// paf_7B2124FE  // ?

// paf_643FFA7C  // ?

// paf_A2EB53BC  // ?
// paf_8AC40FBD  // ?

// paf_0A4091F9  // ?

// paf_0A03DEFC  // ?

// paf_040D12CE  // ?
// paf_053D12C7  // ?

extern const char *paf_4C36ABBB(void);  // returns "PhItemSpin"

// paf_46291F6A  // ?

// paf_E1E55641  // ?
// paf_FCF06EAB  // ?

// paf_85830708  // ?

// paf_05E98BC2  // ?
// paf_9910148E  // ?

extern const char *paf_CA9160F6(void);  // returns "PhNumSpin"
extern const char *paf_59A11C82(void);  // returns "PhNumSpin"

// paf_E7C62F48  // ?
// paf_D28C76AB  // ?
// paf_40725ECF  // ?

extern const char *paf_D64EDE7C(void);  // returns "PhList"

// paf_094CD214  // ?

// paf_43A9D78C  // ?

extern const char *paf_F7630798(void);  // returns "PhInfoList"

// paf_0D1BA494  // ?

// paf_816D1A8F  // ?
// paf_EE05CF53  // ?

extern const char *paf_A98865F8(void);  // returns "PhMenuList"
// paf_88DB3841  // ?

extern const char *paf_90F4F801(void);  // returns "PhCheckBoxList"

extern const char *paf_DDD4ACF6(void);  // returns "PhLabelText"

// paf_3976AFC7  // ?

// paf_E0A86325  // ?

// paf_1F54F28F  // ?

extern const char *paf_545D47A2(void);  // returns "PhClock"

// paf_BBFF3B2F  // ? get.. (u32 address(IN), u16 arg2[4](OUT))

// paf_EA36F30F  // ?

// paf_B821B0A5  // ?

// paf_8A65C27B  // ?

// paf_88B11467  // ?


////////////////////////////////////////////////////////////////////////
extern const char *paf_3806365F(void);  // returns "PhIPAddr"

// paf_685F1C8B  // ?

// paf_CF1E9BCB  // ?

// paf_B361BDA6  // ? sprintf(), "%u.%u.%u.%u"(ip)

// paf_0ACCBD0C  // ?

// paf_C79F9AA9  // ?
// paf_8238B1AE  // ?

// paf_8430EA1E  // ?
// paf_02FD5318  // ?

// paf_B90FF595  // ?

// paf_0A498946  // ?
// paf_CB68FF26  // ?
// paf_98655BEF  // ?
// paf_9C192D1E  // ?

// paf_E16D3CF3  // ?
// paf_C9BF92AF  // ?
// paf_EE64CA3B  // ?

// paf_3B9E324C  // ?

// paf_00BC617A  // ?
// paf_A0974E08  // ?
// paf_B273FF5D  // ?
// paf_14BB4CF1  // ?

// paf_BAA424C6  // ?

// paf_EA43988A  // ?
// paf_5F5DA385  // Gets Dialog Size 	int paf_5F5DA385( uint16_t *size_width, uint16_t *size_height, int dialog_type)

// paf_01B2CEDD  // ?

// paf_8F2D2950  // ?

// paf_D6A97CBE  // ?

// paf_61302357  // ?
// paf_4D7B6927  // ?

// paf_305556B1  // ?

// paf_50239384	  // File_AllocLoad
// paf_6B9B2829	  // File_GetSize
// paf_70DADB9D	  // File_Load
// paf_9DB8660C	  // File_Save

// paf_E1F51D8A  // ?
// paf_FE0C1F10  // Job_Queue
// ??? _Export_paf_job_start:
// paf_D385A4FC  // Job End?/Stop?/DeQueue?

// paf_95320F1E  // ?

// paf_D390D3E7  // ?

// paf_AB1A7C3B  // ?

// paf_227E5B00  // ?

// paf_272404F8  // ?

// paf_C53B6CC9  // ?

// paf_9BDC1912  // ?

// paf_A17CE034  // ?

// paf_40A9CDA4  // ?
// paf_611F2E24  // ?

// paf_D13D6CB4  // ?

// paf_5526D803  // ?
// paf_5D34055E  // ?
// paf_EC892261  // ?
// paf_FD1D3EE0  // ?

// paf_72292B61	  // InputDevice_DisableAnalog
// paf_FC58CA65	  // InputDevice_EnableAnalog
// paf_855C13DC	  // InputDevice_EnableAnalogThreshold
// paf_32658F00	  // InputDevice_Get
// paf_3BC69C3A	  // InputDevice_GetKeyAssign
// paf_3FAD4311	  // InputDevice_GetOwnership
// paf_55944323	  // InputDevice_IsOwner
// paf_55EE69A7	  // InputDevice_Lock
// paf_DAFE04B9	  // InputDevice_Ps3Pad_EnableAnalog
// paf_9C8D34DC	  // InputDevice_Ps3Pad_SetFilterHandler
// paf_FA17FC05	  // InputDevice_ReleaseOwnership
// paf_0F0A07EA	  // InputDevice_SetKeyAssign
// paf_18B76C7D	  // InputDevice_SetModifierKey
// paf_E26BBDE4	  // InputDevice_Unlock

// paf_C05D2A7D  // ?
// paf_99DFB4C0  // ?
// paf_D15AE1C4  // ?

// paf_0582FE87  // ?

// paf_BB04609B  // GetSparkInfo
// paf_07C79FE6  // ?
// paf_97FA2A5F  // ?
// paf_23756B95  // ?
// paf_F400A82B  // ?
// paf_70B5810D  // ?
// paf_C6764EC6  // ?


// paf_36362357  // ?
// paf_3ADA193F  // AudioOutGetState
// paf_95748E9E  // AudioOutGetDeviceInfo
// paf_538E485C  // AudioOutGetNumberOfDevice

// paf_5E1FA570  // ?
// paf_7B2E7FBA  // ?
// paf_E33AADB2  // ?
// paf_141A7D65  // ?
// paf_706B947D  // ?

// paf_CC8004C6  // ?
// paf_5AAA5547  // ?
// paf_1C71A71C  // ?
// paf_BF6BB62C  // ?
// paf_F54AF3D4  // ?

// paf_BB2CC415  // ?

// paf_CFF699A0  // ?

// paf_9F58ADCF  // ?
// paf_23810ED8  // ?

// paf_CC9C89FB  // ?
// paf_1CCF9E4D  // ?

// paf_97D7BF2A  // ?

// paf_D700EC59  // ?

// paf_77A08742  // ?
// paf_6C9FF8D1  // ?

// paf_526C2374  // ?
// paf_84484D13  // ?
// paf_18320785  // ?

// paf_0D84691B  // ?
// paf_2FA1E410  // ?
// paf_E3088E1C  // ?

// paf_EE8F2CC4  // ?

// paf_4F381F53  // ?
// paf_724042E6  // ?
// paf_023F6755  // ?

// paf_3D841FE9  // ?
// paf_EFC3730F  // ?

// paf_FD846612  // ? stop_unload_sprx

// paf_88DDE2F3  // font_get_ get a font struct, info/handle of first loaded font

// paf_64C305D6  // ? vpshader
// paf_8FEC308A  // ? vpshader

// paf_036D6368  // ?

// paf_00C147B2  // ?

// paf_2164ACB8  // ? gcm control register...
// paf_BD1BF0B7  // ? set...
// paf_0FFACD6D  // void get_local_memory_base_add_and_size(u32 *addr, u32 *size) // 0xC0000000(addr) 0x0FE00000(size, vsh.self)

extern uint32_t paf_F476E8AA(void);  //  u32 get_display_width
#define getDisplayWidth paf_F476E8AA

extern uint32_t paf_AC984A12(void);  // u32 get_display_height
#define getDisplayHeight paf_AC984A12

// paf_FED92BA1  // get window offset x
// paf_04C8FC89  // get window offset y
// paf_A01698C9  // u32 set_display_width(u32 width) // ret = old value
// paf_9C1A2D6A  // u32 set_display_height(u32 height) // ret = old value
// paf_EACF4E4B  // u32 set_window_offset_x(u32 offset) // ret = old value
// paf_FDA5E90E  // u32 set_window_offset_y(u32 offset) // ret = old value
// paf_D16FB2D5  // ? set...

// paf_800E9F93  // ?

// paf_042933D6  // ?
// paf_1B131E20  // ?
// paf_EBE6BBC1  // ?
// paf_4418293F  // ?

// paf_A0FCDF5F  // ?

// paf_A11682E8  // ?
// paf_00DEAD04  // ? set...
// paf_5D55D85E  // ? get...

extern int32_t paf_FFE0FBC9(uint32_t *pitch, uint32_t *unk1);  // unk1 = 0x12 color bit depth? ret 0
#define getDisplayPitch paf_FFE0FBC9

// int32_t paf_076402CA  // ? get... (uint32_t *unk1, uint32_t *unk2, uint32_t *unk3) ret 0
// paf_29EE6E08  // ? set...

extern void paf_92AA2072(uint32_t *unk1, uint32_t *offset, uint32_t *pitch);  // current displayed buffer values(offset only in xmb), unk1 = 0x12 color bit depth?

extern void paf_420FAB72(int32_t *x, int32_t *y, int32_t *w, int32_t *h);  // get viewport values
#define getDisplayViewport paf_420FAB72

extern void paf_94AE6813(int32_t *x, int32_t *y, int32_t *w, int32_t *h);  // get scissor box values
#define getDisplayScissorBox paf_94AE6813

// paf_3F2FD49C  // depth_buffer

// paf_2E34FF61  // ?

// paf_39B40DF9  // gcm set render target

// paf_4EE7ABBB  // gcm event port

// paf_E4FE1333  // gcm set depth mask
// paf_2C519D3B  // gcm set front face
// paf_D6AEAC39  // gcm set shade mode
// paf_9F3D573E  // gcm set depth function
// paf_3C7E4CE0  // gcm set scissor box

// paf_84D5E999  // gcm set clear color
// paf_19731532  // gcm set clear depth stencil
// extern void paf_191AD326(uint16_t x, uint16_t y, uint16_t width, uint16_t height)  // gcm set viewport
// paf_ADD67C35  // ? gcm set a float array, (1,1,1,1) vertex data ?

// paf_124DDBC5  // ? gcm set...
// paf_E93B9B31  // ?
// paf_CA1E9947  // ? gcm set...



// paf_77A54856  // ?

// paf_5BD9FD7E  // ?

// paf_CB144BDA  // ?

// paf_AA53DFAD  // ?

// paf_00868264  // ? alloc
// paf_278774DE  // ? alloc

// paf_3234EEEB  // ?
// paf_0CC51D56  // ?

// paf_DF038193  // ?

// paf_849CCA15  // ?

// paf_D1748BF8  // ?
// paf_F8140602  // ?
int32_t paf_0C74837D(int32_t* surfacePool, int32_t p1, int32_t p2, int32_t imageMode, int32_t imageOrder, bool p5, int32_t p6, int32_t p7);
// paf_8EF81BFE  // ?

// paf_4192B349  // ?
// paf_C9826818  // ?

// paf_698E01BE  // ?

extern int32_t paf_BFA7DC8D(int32_t arg1, int32_t arg2);  // ? XMB(0, 1), else (0, 0), gcm_surface_spu_thread_group
// paf_AA8860B9  // ?
// paf_EDB089C8  // ?

// paf_2D9FDEB5  // ?
// paf_1A278F2E  // ?
// paf_B0AF6657  // ?
// paf_605BA9AA  // ?
// paf_9F353652  // ?
// paf_52C9A794  // ?
// paf_ECB29B67  // ?
// paf_57EA66CD  // ?
// paf_E9DA6705  // ?
// paf_56B0FCBF  // ?

// paf_C177FF73  // ?

// paf_76CF943F  // ?

// paf_B2856340  // ?

// paf_0AD64B13  // ?

// paf_61998E81  // ?
// paf_B3B6EFA4  // ?

// paf_48BEFA0B  // ?
// paf_DF712D5B  // ?

// paf_DAFE04B9  // InputDevice_Ps3Pad_EnableAnalog

// paf_5C5EE898  // ?
// paf_8112C91E  // ?
// paf_9C8D34DC  // InputDevice_Ps3Pad_SetFilterHandler
// paf_6C937D68  // ?
// paf_9E7ABD81  // ?

// paf_5687CB15  // ? pad...

extern void paf_503187C0(void);  // base pointer, pad

// paf_0F9B4667  // ? pad...

// paf_37D5F621  // ? pad...
// paf_06CBE4AD  // ? pad...

// paf_DE35D405  // ?

// paf_25C5CE19  // ? kb... sys_hid_manager_ioctl

// paf_268D558E  // ? kb...

// paf_8A50E914  // ? cellKbSetReadMode

// paf_134CEB7A  // ?

// paf_3139BCBD  // ?

extern int32_t paf_55F2C2A6(void);  // eject disk

// paf_5264AC5C	  // PAF_Resource_DOMGetNodeChildByID
// paf_20BA8E1A	  // PAF_Resource_DOMGetNodeChildByPos
// paf_718C9629	  // PAF_Resource_DOMGetNodeData
// paf_8598228	  // PAF_Resource_DOMGetNodeFirstChild
// paf_6997E44A	  // PAF_Resource_DOMGetNodeID
// paf_3FEF8AC9	  // PAF_Resource_DOMGetNodeNext
// paf_8B9AAD3E	  // PAF_Resource_DOMGetNodeType
// paf_19A0F619	  // PAF_Resource_GetPageNodeByID
// paf_1D59A4E	  // PAF_Resource_GetWidgetNodeByID
// paf_76AD182B	  // PAF_Resource_ResolveRefNode
// paf_8644EEA8	  // PAF_Resource_ResolveRefString
// paf_EFC45C20	  // PAF_Resource_ResolveRefWString

// paf_B2EC23EB	  // PhiChangeResource
// paf_40410F89	  // PhiGetDisplay
// paf_385FAD55	  // PhiPostKeycodeEvent
// paf_56760422	  // PhiPostSystemEvent
// paf_97B18ECE	  // PhiRegistResultDisplayCB
// paf_D7138829	  // PhiSetLayoutTable
// paf_4C19A94	  // PhiUpdateDisplay
// paf_D8AF2727	  // PhiUpdateDisplay_platform
// paf_526C2374	  // Platform_GetConfig

// paf_E0A3AA72	  // _Z10normalize3RK4vec4	normalize3(vec4 const&)
// paf_5E671849	  // _Z11pafGumScalePK4vec3	pafGumScale(vec3 const*)
// paf_1167D7F4	  // _Z15mat4_from_scaleRK4vec4	mat4_from_scale(vec4 const&)
// paf_959BF279	  // _Z15pafGumDrawArrayiiiPKvS0_	pafGumDrawArray(int, int, int, void const*, void const*)
// paf_654B91B5	  // _Z15pafGumPopMatrixv	pafGumPopMatrix(void)
// paf_87BBE314	  // _Z15pafGumTranslatePK4vec3	pafGumTranslate(vec3 const*)
// paf_4C229CCA	  // _Z16mat4_from_rotyxzfff	mat4_from_rotyxz(float, float, float)
// paf_5919DAD3	  // _Z16pafGumLoadMatrixPK4mat4	pafGumLoadMatrix(mat4 const*)
// paf_8E7A2AED	  // _Z16pafGumMatrixModei	pafGumMatrixMode(int)
// paf_B4D3C063	  // _Z16pafGumPushMatrixv	pafGumPushMatrix(void)
// paf_88DDE2F3	  // _Z17pafGumStoreMatrixP4mat4	pafGumStoreMatrix(mat4 *)
// paf_5F096D38	  // _Z18pafGumLoadIdentityv	pafGumLoadIdentity(void)
// paf_D774E03E	  // _Z19mat4_from_translatefff	mat4_from_translate(float, float, float)
// paf_A337B314	  // _Z5lerp2RK4vec4S1_f	lerp2(vec4 const&, vec4 const&, float)
// paf_1738DDE3	  // _Z5lerp3RK4vec4S1_f	lerp3(vec4 const&, vec4 const&, float)
// paf_3D841FE9	  // _Z5lerp4RK4vec4S1_f	lerp4(vec4 const&, vec4 const&, float)
// paf_FD846612	  // _Z6clamp4RK4vec4ff	clamp4(vec4 const&, float, float)
// paf_5232FAAF	  // _Z7inverseRK4mat4	inverse(mat4 const&)
// paf_3CAE2F8A	  // _Z9normalizeRK4mat4	normalize(mat4 const&)
// paf_EFC3730F	  // _Z9transposeRK4mat4	transpose(mat4 const&)

// paf_EAD0EFAC	  // _ZN3Ime21OskCreateInputContextEPNS_17ImeOskCreateParamE	Ime::OskCreateInputContext(Ime::ImeOskCreateParam *)
// paf_FEAD5829	  // _ZN3Ime21RegisterPanelCallbackEPNS_20OskpanelCallbackListE	Ime::RegisterPanelCallback(Ime::OskpanelCallbackList *)
// paf_F66AAF4B	  // _ZN3Ime22OskReleaseInputContextEPNS_21InputContextInterfaceE	Ime::OskReleaseInputContext(Ime::InputContextInterface *)

// paf_64527FA9	  // _ZN3paf10DecompressEPvjPKvj	paf::Decompress(void *, unsigned int, void const*, unsigned int)
// paf_5D836E75	  // _ZN3paf10Job_CancelEPNS_9Job_QueueEPNS_10Job_ThreadEi	paf::Job_Cancel(paf::Job_Queue *, paf::Job_Thread *, int)
// paf_26E9507E	  // _ZN3paf10MessageBox10HideDeleteEv	paf::MessageBox::HideDelete(void)
// paf_7001CAC7	  // _ZN3paf10MessageBox12GetSpaceInfoEPNS0_9SpaceInfoE	paf::MessageBox::GetSpaceInfo(paf::MessageBox::SpaceInfo *)
// paf_C84F0B21	  // _ZN3paf10MessageBox12SetLayoutPosEii	paf::MessageBox::SetLayoutPos(int, int)
// paf_6F8663E4	  // _ZN3paf10MessageBox15SetProgressTextERKSbIwSt11char_traitsIwESaIwEEi	paf::MessageBox::SetProgressText(std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> const&, int)
// paf_84D7A733	  // _ZN3paf10MessageBox25SetMessageBoxSoundHandlerEPFvNS0_9SoundTypeEE	paf::MessageBox::SetMessageBoxSoundHandler(void (*)(paf::MessageBox::SoundType))
// paf_1F31959E	  // _ZN3paf10MessageBox25SetQueryButtonInfoHandlerEPFbNS0_10ButtonTypeEPNS0_10ButtonInfoEE	paf::MessageBox::SetQueryButtonInfoHandler(bool (*)(paf::MessageBox::ButtonType, paf::MessageBox::ButtonInfo *))
// paf_8BC095CF	  // _ZN3paf10MessageBox26SetQueryTextureInfoHandlerEPFbNS0_11TextureTypeEPNS0_11TextureInfoEE	paf::MessageBox::SetQueryTextureInfoHandler(bool (*)(paf::MessageBox::TextureType, paf::MessageBox::TextureInfo *))
// paf_84FC5B16	  // _ZN3paf10MessageBox30SetQueryProgressBarInfoHandlerEPFvPNS0_15ProgressBarInfoEE	paf::MessageBox::SetQueryProgressBarInfoHandler(void (*)(paf::MessageBox::ProgressBarInfo *))
// paf_BF66BF2D	  // _ZN3paf10PhCheckBox10WidgetTypeEv	paf::PhCheckBox::WidgetType(void)
// paf_B7377945	  // _ZN3paf10PhCheckBox6CreateEv	paf::PhCheckBox::Create(void)
// paf_5A85BEFC	  // _ZN3paf10PhCheckBox8SetCheckEbb	paf::PhCheckBox::SetCheck(bool, bool)
// paf_176E3BC4	  // _ZN3paf10PhCheckBoxC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhCheckBox::PhCheckBox(paf::PhWidget *, paf::PhAppear *)
// paf_BE04476B	  // _ZN3paf10PhDrawTextENS_6PhFont9GlyphTypeERNS_12SurfaceRCPtrINS_7SurfaceEEEiiPKwjRKS0_PKNS_17PhTextLetterSpaceE	paf::PhDrawText(paf::PhFont::GlyphType, paf::SurfaceRCPtr<paf::Surface> &, int, int, wchar_t const*, unsigned int, paf::PhFont const&, paf::PhTextLetterSpace const*)
// paf_55063D70	  // _ZN3paf10PhInfoList10SetItemNumEi	paf::PhInfoList::SetItemNum(int)
// paf_F7630798	  // _ZN3paf10PhInfoList10WidgetTypeEv	paf::PhInfoList::WidgetType(void)
// paf_D1BA494	  // _ZN3paf10PhInfoList13SetSelectableEibb	paf::PhInfoList::SetSelectable(int, bool, bool)
// paf_318C080C	  // _ZN3paf10PhInfoList13SetVisibleTopEf	paf::PhInfoList::SetVisibleTop(float)
// paf_BEDFF5EE	  // _ZN3paf10PhInfoList14GetPartsWidgetEi	paf::PhInfoList::GetPartsWidget(int)
// paf_D8EB2AD2	  // _ZN3paf10PhInfoList21SetPrivateSurfacePoolEPNS_11SurfacePoolE	paf::PhInfoList::SetPrivateSurfacePool(paf::SurfacePool *)
// paf_EE05CF53	  // _ZN3paf10PhInfoList6CreateEif	paf::PhInfoList::Create(int, float)
// paf_816D1A8F	  // _ZN3paf10PhInfoListC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhInfoList::PhInfoList(paf::PhWidget *, paf::PhAppear *)
// paf_B39C42DC	  // _ZN3paf10PhItemSpin10SetTextureERKNS_12SurfaceRCPtrINS_7SurfaceEEEi	paf::PhItemSpin::SetTexture(paf::SurfaceRCPtr<paf::Surface> const&, int)
// paf_4C36ABBB	  // _ZN3paf10PhItemSpin10WidgetTypeEv	paf::PhItemSpin::WidgetType(void)
// paf_907A4C6F	  // _ZN3paf10PhItemSpin11UpdateStateEv	paf::PhItemSpin::UpdateState(void)
// paf_B9E387DD	  // _ZN3paf10PhItemSpin12UpdateLayoutEb	paf::PhItemSpin::UpdateLayout(bool)
// paf_E50065BE	  // _ZN3paf10PhItemSpin14SetLayoutStyleEiiif	paf::PhItemSpin::SetLayoutStyle(int, int, int, float)
// paf_CC0D0938	  // _ZN3paf10PhItemSpin6CreateEib	paf::PhItemSpin::Create(int, bool)
// paf_8AAC61B1	  // _ZN3paf10PhItemSpin7SetTextERKSbIwSt11char_traitsIwESaIwEEi	paf::PhItemSpin::SetText(std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> const&, int)
// paf_688EACFF	  // _ZN3paf10PhItemSpin8GetStyleEiRb	paf::PhItemSpin::GetStyle(int, bool &)
// paf_ADCC6046	  // _ZN3paf10PhItemSpin8GetStyleEiiR4vec4	paf::PhItemSpin::GetStyle(int, int, vec4 &)
// paf_A08ABACC	  // _ZN3paf10PhItemSpin8SetStyleEib	paf::PhItemSpin::SetStyle(int, bool)
// paf_EDE62D3D	  // _ZN3paf10PhItemSpin8SetStyleEiiRK4vec4	paf::PhItemSpin::SetStyle(int, int, vec4 const&)
// paf_96599FE7	  // _ZN3paf10PhItemSpin8SetStyleEiib	paf::PhItemSpin::SetStyle(int, int, bool)
// paf_62DCA3E2	  // _ZN3paf10PhItemSpin8SetStyleEiif	paf::PhItemSpin::SetStyle(int, int, float)
// paf_4E348797	  // _ZN3paf10PhItemSpin8SetStyleEiii	paf::PhItemSpin::SetStyle(int, int, int)
// paf_A4514F28	  // _ZN3paf10PhItemSpinC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhItemSpin::PhItemSpin(paf::PhWidget *, paf::PhAppear *)
// paf_D6A50AAC	  // _ZN3paf10PhItemSpinC2EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhItemSpin::PhItemSpin(paf::PhWidget *, paf::PhAppear *)
// paf_97E124F1	  // _ZN3paf10PhItemSpinD2Ev	paf::PhItemSpin::~PhItemSpin()
// paf_A98865F8	  // _ZN3paf10PhMenuList10WidgetTypeEv	paf::PhMenuList::WidgetType(void)
// paf_4618F156	  // _ZN3paf10PhMenuList11SetSelectedEi	paf::PhMenuList::SetSelected(int)
// paf_1CD57819	  // _ZN3paf10PhMenuList12SetShowStateEii	paf::PhMenuList::SetShowState(int, int)
// paf_3E1D246B	  // _ZN3paf10PhMenuList14ChangeSelectedEi	paf::PhMenuList::ChangeSelected(int)
// paf_88DB3841	  // _ZN3paf10PhMenuList14SetSelectedPosEi	paf::PhMenuList::SetSelectedPos(int)
// paf_B9502D8B	  // _ZN3paf10PhMenuListC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhMenuList::PhMenuList(paf::PhWidget *, paf::PhAppear *)
// paf_E801C345	  // _ZN3paf10PhProgress10WidgetTypeEv	paf::PhProgress::WidgetType(void)
// paf_301F3376	  // _ZN3paf10PhProgress11ChangeValueEffi	paf::PhProgress::ChangeValue(float, float, int)
// paf_BEC5537F	  // _ZN3paf10PhProgress11SetMaxValueEf	paf::PhProgress::SetMaxValue(float)
// paf_15FD7978	  // _ZN3paf10PhProgress15RequestSetValueEf	paf::PhProgress::RequestSetValue(float)
// paf_2883F1FC	  // _ZN3paf10PhProgress18RequestChangeValueEffi	paf::PhProgress::RequestChangeValue(float, float, int)
// paf_9FB0F27B	  // _ZN3paf10PhProgress8SetValueEf	paf::PhProgress::SetValue(float)
// paf_4D930842	  // _ZN3paf10PhProgressC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhProgress::PhProgress(paf::PhWidget *, paf::PhAppear *)
// paf_A3415D11	  // _ZN3paf10PhSPrimDiv6RenderEPKNS_8PhWidgetERK4vec4b	paf::PhSPrimDiv::Render(paf::PhWidget const*, vec4 const&, bool)
// paf_275212A0	  // _ZN3paf10PhSPrimDiv8SetStyleEiRK4vec4	paf::PhSPrimDiv::SetStyle(int, vec4 const&)
// paf_D4579440	  // _ZN3paf10PhSPrimDiv8SetStyleEif	paf::PhSPrimDiv::SetStyle(int, float)
// paf_CF52B916	  // _ZN3paf10PhSPrimDiv8SetStyleEii	paf::PhSPrimDiv::SetStyle(int, int)
// paf_2F8B3B85	  // _ZN3paf10PhSPrimDivC1ERNS_9PhSRenderE	paf::PhSPrimDiv::PhSPrimDiv(paf::PhSRender &)
// paf_BA57D3D	  // _ZN3paf10PhSPrimDivD1Ev	paf::PhSPrimDiv::~PhSPrimDiv()

// paf_41054F69	  // _ZN3paf11HalfImageL8EPvPKviiii	paf::HalfImageL8(void *, void const*, int, int, int, int)
// paf_BA6D149A	  // _ZN3paf11PhLabelPrim10WidgetTypeEv	paf::PhLabelPrim::WidgetType(void)
// paf_44F95B67	  // _ZN3paf11PhLabelPrimC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhLabelPrim::PhLabelPrim(paf::PhWidget *, paf::PhAppear *)
// paf_DDD4ACF6	  // _ZN3paf11PhLabelText10WidgetTypeEv	paf::PhLabelText::WidgetType(void)
// paf_429ED666	  // _ZN3paf11PhLabelText12SetLineColorEjRK4vec4	paf::PhLabelText::SetLineColor(unsigned int, vec4 const&)
// paf_FDD0EB53	  // _ZN3paf11PhLabelText14ReleaseSurfaceEj	paf::PhLabelText::ReleaseSurface(unsigned int)
// paf_A3224451	  // _ZN3paf11PhLabelText14SetLineVisibleEjb	paf::PhLabelText::SetLineVisible(unsigned int, bool)
// paf_96486AF8	  // _ZN3paf11PhLabelText15SetColumnOffsetEf	paf::PhLabelText::SetColumnOffset(float)
// paf_D11A3565	  // _ZN3paf11PhLabelText15SetShadowRenderEPNS_9PhSRenderE	paf::PhLabelText::SetShadowRender(paf::PhSRender *)
// paf_CB864F5D	  // _ZN3paf11PhLabelText21SetPrivateSurfacePoolEPNS_11SurfacePoolE	paf::PhLabelText::SetPrivateSurfacePool(paf::SurfacePool *)
// paf_20183B78	  // _ZN3paf11PhLabelTextC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhLabelText::PhLabelText(paf::PhWidget *, paf::PhAppear *)
// paf_73878BBE	  // _ZN3paf11SplitStringEPSt6vectorISsSaISsEERKSsS5_j	paf::SplitString(std::vector<std::string, std::allocator<std::string>> *, std::string const&, std::string const&, unsigned int)
// paf_AA8860B9	  // _ZN3paf11SurfaceBase18ConvertPixelFormatE9ImageMode	paf::SurfaceBase::ConvertPixelFormat(ImageMode)
// paf_DC7E61D3	  // _ZN3paf11SurfaceCLUT4CopyEPv	paf::SurfaceCLUT::Copy(void *)
// paf_68596AF1	  // _ZN3paf11SurfaceCLUTC1EPNS_11SurfacePoolE9ImageModei	paf::SurfaceCLUT::SurfaceCLUT(paf::SurfacePool *, ImageMode, int)
// paf_868264	  // _ZN3paf11SurfaceCLUTnwEj	paf::SurfaceCLUT::operator new(unsigned int)
// paf_64115670	  // _ZN3paf11SurfacePool4FreeEPv	paf::SurfacePool::Free(void *)
// paf_34EBB81E	  // _ZN3paf11SurfacePool8AllocateEii	paf::SurfacePool::Allocate(int, int)

// paf_24F6808A	  // _ZN3paf12PhDispatcher9EventExecEPNS_7PhEventE	paf::PhDispatcher::EventExec(paf::PhEvent *)
// paf_C88CA4B2	  // _ZN3paf12PhLevelMeter10WidgetTypeEv	paf::PhLevelMeter::WidgetType(void)
// paf_D8D768ED	  // _ZN3paf12PhLevelMeter11SetSelectedEi	paf::PhLevelMeter::SetSelected(int)
// paf_24A9E5BA	  // _ZN3paf12PhLevelMeter6CreateEii	paf::PhLevelMeter::Create(int, int)
// paf_A3770DD7	  // _ZN3paf12PhLevelMeterC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhLevelMeter::PhLevelMeter(paf::PhWidget *, paf::PhAppear *)

// paf_C0F6890C	  // _ZN3paf13AVCopyControl10SetWSSCodeEjj	paf::AVCopyControl::SetWSSCode(unsigned int, unsigned int)
// paf_4B00C738	  // _ZN3paf13AVCopyControl11SetVBIDCodeEjj	paf::AVCopyControl::SetVBIDCode(unsigned int, unsigned int)
// paf_22C482F2	  // _ZN3paf13AVCopyControlC1Ev	paf::AVCopyControl::AVCopyControl(void)
// paf_CACDFB2D	  // _ZN3paf13PhAppearPlane10SetTextureERKNS_12SurfaceRCPtrINS_7SurfaceEEEi	paf::PhAppearPlane::SetTexture(paf::SurfaceRCPtr<paf::Surface> const&, int)
// paf_D7F0A558	  // _ZN3paf13PhAppearPlane11RenderStateEjb	paf::PhAppearPlane::RenderState(unsigned int, bool)
// paf_23CCCCBB	  // _ZN3paf13PhAppearPlane8SetStyleEii	paf::PhAppearPlane::SetStyle(int, int)
// paf_C11E4924	  // _ZN3paf13PhApplication13GetRootScreenEv	paf::PhApplication::GetRootScreen(void)
// paf_EA225A69	  // _ZN3paf13PhApplication15CalcLayoutValueEiiiRK4vec4	paf::PhApplication::CalcLayoutValue(int, int, int, vec4 const&)
// paf_CCE0FADA	  // _ZN3paf13PhApplication15SetCustomRenderEPFvPS0_E	paf::PhApplication::SetCustomRender(void (*)(paf::PhApplication*))
// paf_530D8265	  // _ZN3paf13PhApplication21DeleteUpdateDisplayCBEPFiiiiE	paf::PhApplication::DeleteUpdateDisplayCB(int (*)(int, int, int))
// paf_7902DF28	  // _ZN3paf13PhApplication21RegistUpdateDisplayCBEPFiiiiE	paf::PhApplication::RegistUpdateDisplayCB(int (*)(int,int,int))
// paf_1ED44FDD	  // _ZN3paf13PhApplication9PostEventEPNS_7PhEventE	paf::PhApplication::PostEvent(paf::PhEvent *)

// paf_28E3DF10	  // _ZN3paf14GraphicsMemory3MapEPvj	paf::GraphicsMemory::Map(void *, unsigned int)
// paf_4922A06A	  // _ZN3paf14GraphicsMemory4Area13AllocFromHeapENS0_10DeviceTypeEijj	paf::GraphicsMemory::Area::AllocFromHeap(paf::GraphicsMemory::DeviceType, int, unsigned int, unsigned int)
// paf_AA1A8C53	  // _ZN3paf14GraphicsMemory4Area13AllocFromPoolENS0_10DeviceTypeEijPj	paf::GraphicsMemory::Area::AllocFromPool(paf::GraphicsMemory::DeviceType, int, unsigned int, unsigned int *)
// paf_9BA36D14	  // _ZN3paf14GraphicsMemory4Area4FreeEPv	paf::GraphicsMemory::Area::Free(void *)
// paf_33944117	  // _ZN3paf14Job_isCanceledEPNS_10Job_ThreadE	paf::Job_isCanceled(paf::Job_Thread *)
// paf_E02A30D5	  // _ZN3paf14PhAppearXmItem20SetDefaultLabelColorERK4vec4	paf::PhAppearXmItem::SetDefaultLabelColor(vec4 const&)
// paf_90F4F801	  // _ZN3paf14PhCheckBoxList10WidgetTypeEv	paf::PhCheckBoxList::WidgetType(void)
// paf_EEFAA794	  // _ZN3paf14PhCheckBoxList8GetCheckEi	paf::PhCheckBoxList::GetCheck(int)
// paf_6B57CB4D	  // _ZN3paf14PhCheckBoxList8SetCheckEibb	paf::PhCheckBoxList::SetCheck(int, bool, bool)
// paf_689DCA1F	  // _ZN3paf14PhGetTextWidthENS_6PhFont9GlyphTypeERKSbIwSt11char_traitsIwESaIwEEjjRKS0_PKNS_17PhTextLetterSpaceE	paf::PhGetTextWidth(paf::PhFont::GlyphType, std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> const&, unsigned int, unsigned int, paf::PhFont const&, paf::PhTextLetterSpace const*)
// paf_7D594F19	  // _ZN3paf14PhLabelPrimDivC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhLabelPrimDiv::PhLabelPrimDiv(paf::PhWidget *, paf::PhAppear *)

// paf_45F7D5ED	  // _ZN3paf15CriticalSection5EnterEv	paf::CriticalSection::Enter(void)
// paf_5C6C78E5	  // _ZN3paf15CriticalSection5LeaveEv	paf::CriticalSection::Leave(void)
// paf_E9A06D0A	  // _ZN3paf15CriticalSectionC1Ev	paf::CriticalSection::CriticalSection(void)
// paf_DE9C6F25	  // _ZN3paf15CriticalSectionD1Ev	paf::CriticalSection::~CriticalSection()
// paf_FE0C1F10	  // _ZN3paf15Job_CreateQueueEv	paf::Job_CreateQueue(void)

// paf_D385A4FC	  // _ZN3paf16Job_DestroyQueueEPNS_9Job_QueueE	paf::Job_DestroyQueue(paf::Job_Queue *)
// paf_AC90BC8F	  // _ZN3paf18Job_GetNumOfThreadEPNS_9Job_QueueE	paf::Job_GetNumOfThread(paf::Job_Queue *)
// paf_6E6A92EB	  // _ZN3paf23SetFormatStringCallbackEPFPKwwE	paf::SetFormatStringCallback(wchar_t const* (*)(wchar_t))

// paf_ED058E54	  // _ZN3paf4Cond4WaitEv	paf::Cond::Wait(void)
// paf_75D9466C	  // _ZN3paf4Cond6NotifyEv	paf::Cond::Notify(void)
// paf_DFE2B134	  // _ZN3paf4Cond9NotifyAllEv	paf::Cond::NotifyAll(void)
// paf_AA422FCB	  // _ZN3paf4CondC1ERNS_5MutexEPKc	paf::Cond::Cond(paf::Mutex &, char const*)
// paf_8D81B2F3	  // _ZN3paf4CondD1Ev	paf::Cond::~Cond()
// paf_7EE103EF	  // _ZN3paf4View12PageActivateEPKc	paf::View::PageActivate(char const*)
// paf_2166E819	  // _ZN3paf4View12PageActivateEPv	paf::View::PageActivate(void *)
// paf_202CA2A7	  // _ZN3paf4View12PageCloseAllEv	paf::View::PageCloseAll(void)
// paf_A1DC401	  // _ZN3paf4View12SetInterfaceEiPv	paf::View::SetInterface(int, void *)
// paf_E50657B1	  // _ZN3paf4View12SetLocaleAllE6Locale	paf::View::SetLocaleAll(Locale)
// paf_E014D3BA	  // _ZN3paf4View12StopSoundAllEv	paf::View::StopSoundAll(void)
// paf_2F58C5DF	  // _ZN3paf4View14PageInactivateEPKc	paf::View::PageInactivate(char const*)
// paf_DF5E3BFD	  // _ZN3paf4View15SetDrawPriorityEi	paf::View::SetDrawPriority(int)
// paf_9DB21A04	  // _ZN3paf4View15SetEventHandlerEPKNS_18EventFunctionEntryE	paf::View::SetEventHandler(paf::EventFunctionEntry const*)
// paf_8BC523EB	  // _ZN3paf4View16PlayAnimSequenceEPNS_8PhWidgetEPKc	paf::View::PlayAnimSequence(paf::PhWidget *, char const*)
// paf_F21655F3	  // _ZN3paf4View4FindEPKc	paf::View::Find(char const*)
// paf_92F7E387	  // _ZN3paf4View8ActivateEv	paf::View::Activate(void)
// paf_50CDB465	  // _ZN3paf4View8GetSoundEPKc	paf::View::GetSound(char const*)
// paf_2C4E7D1C	  // _ZN3paf4View9InitParamC1Ev	paf::View::InitParam::InitParam(void)
// paf_724E5A8B	  // _ZN3paf4View9PageCloseEPKc	paf::View::PageClose(char const*)
// paf_75457863	  // _ZN3paf4View9PageCloseEPv	paf::View::PageClose(void *)
// paf_91404635	  // _ZN3paf4View9SetLocaleE6Locale	paf::View::SetLocale(Locale)
// paf_76BBC067	  // _ZN3paf5Image10Rotation90Ei	paf::Image::Rotation90(int)
// paf_BD89CC93	  // _ZN3paf5Image10SetSubRectERKNS_9ImageRectE	paf::Image::SetSubRect(paf::ImageRect const&)
// paf_1FBFAA13	  // _ZN3paf5Image12ToCLUTBufferEb	paf::Image::ToCLUTBuffer(bool)
// paf_5B872B03	  // _ZN3paf5Image13SetCancelFuncEPFbPvES1_	paf::Image::SetCancelFunc(bool (*)(void *), void *)
// paf_9DEF39DE	  // _ZN3paf5Image15SetGlobalOptionENS_15ImageOptionCodeEi	paf::Image::SetGlobalOption(paf::ImageOptionCode, int)
// paf_AA5755FA	  // _ZN3paf5Image4CopyEv	paf::Image::Copy(void)
// paf_11DE1214	  // _ZN3paf5Image4LoadEb	paf::Image::Load(bool)
// paf_7968B603	  // _ZN3paf5Image4OpenEPKcP12malloc_stateNS_11ImageFormatEjj	paf::Image::Open(char const*, malloc_state *, paf::ImageFormat, unsigned int, unsigned int)
// paf_F61F19E1	  // _ZN3paf5Image4OpenEPvjP12malloc_stateNS_11ImageFormatE	paf::Image::Open(void *, unsigned int, malloc_state *, paf::ImageFormat)
// paf_9576C835	  // _ZN3paf5Image6ResizeERKNS_11ImageExtentENS_12OpResizeTypeE	paf::Image::Resize(paf::ImageExtent const&, paf::OpResizeType)
// paf_4E48E271	  // _ZN3paf5Image6UnLoadEv	paf::Image::UnLoad(void)
// paf_11395765	  // _ZN3paf5Image7OpenGIMEPvj	paf::Image::OpenGIM(void *, unsigned int)
// paf_D1B181E0	  // _ZN3paf5Image7SetClipERKNS_9ImageRectE	paf::Image::SetClip(paf::ImageRect const&)
// paf_C4F74046	  // _ZN3paf5Image8ToBufferEb	paf::Image::ToBuffer(bool)
// paf_E0AB62A4	  // _ZN3paf5Image9Convert32Ev	paf::Image::Convert32(void)
// paf_84529ACC	  // _ZN3paf5Image9SetExtentERKNS_11ImageExtentENS_12OpResizeTypeE	paf::Image::SetExtent(paf::ImageExtent const&, paf::OpResizeType)
// paf_C69ECA71	  // _ZN3paf5ImageD1Ev	paf::Image::~Image()
// paf_AD9051B6	  // _ZN3paf5SleepEi	paf::Sleep(int)
// paf_95748E9E	  // _ZN3paf5Sound6Output13GetDeviceInfoEjjP26xSettingAudioOutDeviceInfo	paf::Sound::Output::GetDeviceInfo(unsigned int, unsigned int, xSettingAudioOutDeviceInfo *)
// paf_9437A62E	  // _ZN3paf5Sound6Output17ConfigureAudioOutEj32xSettingDisplayAudioOutConfigure	paf::Sound::Output::ConfigureAudioOut(unsigned int, xSettingDisplayAudioOutConfigure)
// paf_74AA6AB7	  // _ZN3paf5Sound6Output17LoadAudioRegistryEjP24xSettingAudioOutRegistry	paf::Sound::Output::LoadAudioRegistry(unsigned int, xSettingAudioOutRegistry *)
// paf_538E485C	  // _ZN3paf5Sound6Output18GetNumberOfDevicesEj	paf::Sound::Output::GetNumberOfDevices(unsigned int)
// paf_CC8004C6	  // _ZN3paf5Sound6Output19ReleaseAudioChannelEi	paf::Sound::Output::ReleaseAudioChannel(int)
// paf_706B947D	  // _ZN3paf5Sound6Output19SetDefaultConfigureEv	paf::Sound::Output::SetDefaultConfigure(void)
// paf_5AAA5547	  // _ZN3paf5Sound6Output20AllocateAudioChannelEv	paf::Sound::Output::AllocateAudioChannel(void)
// paf_36362357	  // _ZN3paf5Sound6Output4MuteEj20xSettingAudioOutMute	paf::Sound::Output::Mute(unsigned int, xSettingAudioOutMute)
// paf_1C71A71C	  // _ZN3paf5Sound6Output4StopEPv	paf::Sound::Output::Stop(void *)
// paf_21DEB478	  // _ZN3paf5Sound6Output6EnableEb	paf::Sound::Output::Enable(bool)
// paf_3ADA193F	  // _ZN3paf5Sound6Output8GetStateEjjP21xSettingAudioOutState	paf::Sound::Output::GetState(unsigned int, unsigned int, xSettingAudioOutState *)
// paf_FC23139F	  // _ZN3paf5Sound6Output9ConfigureEjP29xSettingAudioOutConfigurationP22xSettingAudioOutOptionj	paf::Sound::Output::Configure(unsigned int, xSettingAudioOutConfiguration *, xSettingAudioOutOption *, unsigned int)

//int paf_3F7CB0BF(void*, int, void*); // paf_3F7CB0BF	  // _ZN3paf6Module12SetInterfaceEiPv	paf::Module::SetInterface(int, void *)
//static int Module_SetInterface(void* a1, int a2, void* a3) { return paf_3F7CB0BF(a1, a2, a3); }

// paf_2F5CEE6D	  // _ZN3paf6ModuleD1Ev	paf::Module::~Module()
// paf_27527B03	  // _ZN3paf6PhFont14GetNumFontListEv	paf::PhFont::GetNumFontList(void)
// paf_22BF05A	  // _ZN3paf6PhFont8SetStyleERKNS_11PhFontStyleE	paf::PhFont::SetStyle(paf::PhFontStyle const&)
// paf_40C4C4D9	  // _ZN3paf6PhFontC1Ev	paf::PhFont::PhFont(void)
// paf_2CC30288	  // _ZN3paf6PhFontD1Ev	paf::PhFont::~PhFont()
// paf_7531D5C0	  // _ZN3paf6PhList10SetItemNumEi	paf::PhList::SetItemNum(int)
// paf_D64EDE7C	  // _ZN3paf6PhList10WidgetTypeEv	paf::PhList::WidgetType(void)
// paf_3F5AB405	  // _ZN3paf6PhList12GetShowStateEi	paf::PhList::GetShowState(int)
// paf_AF27EE12	  // _ZN3paf6PhList12PopBackLabelEi	paf::PhList::PopBackLabel(int)
// paf_B624302F	  // _ZN3paf6PhList12PushBackItemEi	paf::PhList::PushBackItem(int)
// paf_FEBB5DBB	  // _ZN3paf6PhList13SetVisibleTopEi	paf::PhList::SetVisibleTop(int)
// paf_14EBB064	  // _ZN3paf6PhList14GetPartsWidgetEi	paf::PhList::GetPartsWidget(int)
// paf_46CC88BE	  // _ZN3paf6PhList15SetShadowRenderEPNS_9PhSRenderE	paf::PhList::SetShadowRender(paf::PhSRender *)
// paf_94CD214	  // _ZN3paf6PhList17PushBackLabelTextEi	paf::PhList::PushBackLabelText(int)
// paf_AED08D9E	  // _ZN3paf6PhList18PushBackLabelImageEi	paf::PhList::PushBackLabelImage(int)
// paf_C3F7B60E	  // _ZN3paf6PhList19PushBackLabelShadowEi	paf::PhList::PushBackLabelShadow(int)
// paf_9F143A8C	  // _ZN3paf6PhList6RedrawEv	paf::PhList::Redraw(void)
// paf_5903BC00	  // _ZN3paf6PhListC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhList::PhList(paf::PhWidget *, paf::PhAppear *)
// paf_F15A0DE5	  // _ZN3paf6PhSpin10SetTextureERKNS_12SurfaceRCPtrINS_7SurfaceEEEi	paf::PhSpin::SetTexture(paf::SurfaceRCPtr<paf::Surface> const&, int)
// paf_FDD01957	  // _ZN3paf6PhSpin11SetSelectedEi	paf::PhSpin::SetSelected(int)
// paf_3A5DB54E	  // _ZN3paf6PhSpin12SetShowStateEi	paf::PhSpin::SetShowState(int)
// paf_69EB7F9A	  // _ZN3paf6PhSpin14ChangeSelectedEii	paf::PhSpin::ChangeSelected(int, int)
// paf_40D12CE	  // _ZN3paf6PhSpin18HandleKeycodeEventEPNS_7PhEventE	paf::PhSpin::HandleKeycodeEvent(paf::PhEvent *)
// paf_53D12C7	  // _ZN3paf6PhSpin22HandleFocusSwitchEventEPNS_7PhEventE	paf::PhSpin::HandleFocusSwitchEvent(paf::PhEvent *)
// paf_A03DEFC	  // _ZN3paf6PhSpin8DrawThisEjb	paf::PhSpin::DrawThis(unsigned int, bool)
// paf_A4091F9	  // _ZN3paf6PhSpin8GetStyleEiRi	paf::PhSpin::GetStyle(int, int &)
// paf_2030C3B9	  // _ZN3paf6PhSpin8SetFocusEPNS_7PhEventEj	paf::PhSpin::SetFocus(paf::PhEvent *, unsigned int)
// paf_1DCA42A6	  // _ZN3paf6PhSpin8SetStyleEii	paf::PhSpin::SetStyle(int, int)
// paf_4569ABC3	  // _ZN3paf6PhText10InsertTextEjRKSbIwSt11char_traitsIwESaIwEE	paf::PhText::InsertText(unsigned int, std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> const&)
// paf_FE119299	  // _ZN3paf6PhText10SetVScrollEPNS_8PhScrollE	paf::PhText::SetVScroll(paf::PhScroll *)
// paf_B7DFCE90	  // _ZN3paf6PhText10WidgetTypeEv	paf::PhText::WidgetType(void)
// paf_C7CF2A06	  // _ZN3paf6PhText12SetFirstLineEf	paf::PhText::SetFirstLine(float)
// paf_EB73AD01	  // _ZN3paf6PhText12SetFontStyleERKNS_11PhFontStyleE	paf::PhText::SetFontStyle(paf::PhFontStyle const&)
// paf_84E6B20D	  // _ZN3paf6PhText13EnsureVisibleEv	paf::PhText::EnsureVisible(void)
// paf_2DB4683D	  // _ZN3paf6PhText13SetCurrentPosEj	paf::PhText::SetCurrentPos(unsigned int)
// paf_BD01AEF9	  // _ZN3paf6PhText14ReleaseSurfaceEv	paf::PhText::ReleaseSurface(void)
// paf_A0118D9E	  // _ZN3paf6PhText14RemoveAllAttrsEv	paf::PhText::RemoveAllAttrs(void)
// paf_91CD1263	  // _ZN3paf6PhText15SetColumnOffsetEf	paf::PhText::SetColumnOffset(float)
// paf_2DBA5B93	  // _ZN3paf6PhText21SetPrivateSurfacePoolEPNS_11SurfacePoolE	paf::PhText::SetPrivateSurfacePool(paf::SurfacePool *)
// paf_402E693B	  // _ZN3paf6PhText6LineUpEj	paf::PhText::LineUp(unsigned int)
// paf_74093C9A	  // _ZN3paf6PhText7AddAttrENS_14PhTextAttrTypeEjj	paf::PhText::AddAttr(paf::PhTextAttrType, unsigned int, unsigned int)
// paf_22FDDB8	  // _ZN3paf6PhText7SetSizeEfff	paf::PhText::SetSize(float, float, float)
// paf_E75F9969	  // _ZN3paf6PhText8LineDownEj	paf::PhText::LineDown(unsigned int)
// paf_6B620D94	  // _ZN3paf6PhText9EraseTextEjj	paf::PhText::EraseText(unsigned int, unsigned int)
// paf_201528C1	  // _ZN3paf6PhText9ShowCaretEb	paf::PhText::ShowCaret(bool)
//void paf_7F0930C6(void* text, void* parent, void* appear);	  // _ZN3paf6PhTextC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhText::PhText(paf::PhWidget *, paf::PhAppear *)
//static void PhText_Constructor(void* text, void* parent, void* appear) { paf_7F0930C6(text, parent, appear); }

// paf_9C1D60C4	  // _ZN3paf6Thread4JoinEv	paf::Thread::Join(void)
// paf_97D7BF2A	  // _ZN3paf6Thread5StartEv	paf::Thread::Start(void)
// paf_DFE8806F	  // _ZN3paf6ThreadC2EijPKcj	paf::Thread::Thread(int, unsigned int, char const*, unsigned int)
// paf_EBD30F24	  // _ZN3paf6ThreadD2Ev	paf::Thread::~Thread()

// paf_545D47A2	  // _ZN3paf7PhClock10WidgetTypeEv	paf::PhClock::WidgetType(void)
// paf_F8DB34C7	  // _ZN3paf7PhClock15SetShadowRenderEPNS_9PhSRenderE	paf::PhClock::SetShadowRender(paf::PhSRender *)
// paf_BBFF3B2F	  // _ZN3paf7PhClock7GetTimeERNS0_10datetime_tE	paf::PhClock::GetTime(paf::PhClock::datetime_t &)
// paf_1B75E85F	  // _ZN3paf7PhClock7SetTimeERKNS0_10datetime_tE	paf::PhClock::SetTime(paf::PhClock::datetime_t const&)
// paf_2293AB67	  // _ZN3paf7PhClockC1EPNS_8PhWidgetE	paf::PhClock::PhClock(paf::PhWidget *)
// paf_C2F5EAF9	  // _ZN3paf7PhEventC1EjPNS_8PhWidgetEjiiii	paf::PhEvent::PhEvent(unsigned int, paf::PhWidget *, unsigned int, int, int, int, int)
// paf_9C4C1564	  // _ZN3paf7PhEventC1Ejjjiiii	paf::PhEvent::PhEvent(unsigned int, unsigned int, unsigned int, int, int, int, int)

// paf_10DEDCC7	  // _ZN3paf7PhPlane10WidgetTypeEv	paf::PhPlane::WidgetType(void)
//void paf_D0197A7D(void* plane, void* widget, void* appear);	  // _ZN3paf7PhPlaneC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhPlane::PhPlane(paf::PhWidget *, paf::PhAppear *)
//static void PhPlane_Constructor(void* plane, void* widget, void* appear) { paf_D0197A7D(plane, widget, appear); }

//void paf_CC58846C(void* plane, void* widget, void* appear);	  // _ZN3paf7PhPlaneC2EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhPlane::PhPlane(paf::PhWidget *, paf::PhAppear *)
//static void PhPlane_Constructor2(void* plane, void* widget, void* appear) { paf_CC58846C(plane, widget, appear); }

//void paf_0C16A258(void* plane);	  // _ZN3paf7PhPlaneD2Ev	paf::PhPlane::~PhPlane()
//static void PhPlane_Destructor(void* plane) { paf_0C16A258(plane); }

// paf_BFF616B8	  // _ZN3paf7PhSPrim17UpdateMatrixColorEPKNS_8PhWidgetERK4vec4	paf::PhSPrim::UpdateMatrixColor(paf::PhWidget const*, vec4 const&)
// paf_E31907BE	  // _ZN3paf7PhSPrim6RenderEPKNS_8PhWidgetERK4vec4b	paf::PhSPrim::Render(paf::PhWidget const*, vec4 const&, bool)
// paf_5063ACE	  // _ZN3paf7PhSPrim8SetStyleEiRK4vec4	paf::PhSPrim::SetStyle(int, vec4 const&)
// paf_B6584CE7	  // _ZN3paf7PhSPrim8SetStyleEib	paf::PhSPrim::SetStyle(int, bool)
// paf_6F1299BE	  // _ZN3paf7PhSPrim8SetStyleEif	paf::PhSPrim::SetStyle(int, float)
// paf_EEE37C22	  // _ZN3paf7PhSPrim8SetStyleEii	paf::PhSPrim::SetStyle(int, int)
// paf_E3C1B660	  // _ZN3paf7PhSPrimC1ERNS_9PhSRenderE	paf::PhSPrim::PhSPrim(paf::PhSRender &)
// paf_FF99F7AB	  // _ZN3paf7PhSText11RefreshTextEv	paf::PhSText::RefreshText(void)
// paf_F72BABD5	  // _ZN3paf7PhSText12SetFontStyleERKNS_11PhFontStyleE	paf::PhSText::SetFontStyle(paf::PhFontStyle const&)
// paf_DD32A35C	  // _ZN3paf7PhSText14GetLayoutStyleEiRiRf	paf::PhSText::GetLayoutStyle(int, int &, float &)
// paf_6BE9B314	  // _ZN3paf7PhSText14SetLayoutStyleEiif	paf::PhSText::SetLayoutStyle(int, int, float)
// paf_A6F56DD8	  // _ZN3paf7PhSText7DestroyEv	paf::PhSText::Destroy(void)
// paf_561CBD17	  // _ZN3paf7PhSText7IsReadyEv	paf::PhSText::IsReady(void)
// paf_FBAF2957	  // _ZN3paf7PhSText7SetTextERKSbIwSt11char_traitsIwESaIwEE	paf::PhSText::SetText(std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> const&)
// paf_638A0B38	  // _ZN3paf7PhSText8SetStyleEiRK4vec4	paf::PhSText::SetStyle(int, vec4 const&)
// paf_8E3B155F	  // _ZN3paf7PhSText8SetStyleEib	paf::PhSText::SetStyle(int, bool)

//extern "C" void paf_165AD4A6(void* phSText, int r4, float r5);	  // _ZN3paf7PhSText8SetStyleEif	paf::PhSText::SetStyle(int, float)
//static void PhSText_SetStyle(void* phSText, int r4, float r5) { paf_165AD4A6(phSText, r4, r5); }

// paf_983EA578	  // _ZN3paf7PhSText8SetStyleEii	paf::PhSText::SetStyle(int, int)
// paf_66977993	  // _ZN3paf7PhSTextC1ERNS_9PhSRenderEPS1_	paf::PhSText::PhSText(paf::PhSRender &, paf::PhSRender*)
// paf_41BBFE5E	  // _ZN3paf7PhScene10WidgetTypeEv	paf::PhScene::WidgetType(void)
// paf_873C6688	  // _ZN3paf7PhTimer12GetFrameTimeEv	paf::PhTimer::GetFrameTime(void)
// paf_BC8DD07D	  // _ZN3paf7PhXmBar10ScrollLeftEfPNS_7PhEventE	paf::PhXmBar::ScrollLeft(float, paf::PhEvent *)
// paf_703117AD	  // _ZN3paf7PhXmBar10WidgetTypeEv	paf::PhXmBar::WidgetType(void)
// paf_F744FE00	  // _ZN3paf7PhXmBar11ScrollRightEfPNS_7PhEventE	paf::PhXmBar::ScrollRight(float, paf::PhEvent *)
// paf_534AC5C4	  // _ZN3paf7PhXmBar11SetTopAlphaEfff	paf::PhXmBar::SetTopAlpha(float, float, float)
// paf_C1A00166	  // _ZN3paf7PhXmBar13GetListWidgetEi	paf::PhXmBar::GetListWidget(int)
// paf_B24DE5AC	  // _ZN3paf7PhXmBar16SetOtherTopAlphaEfff	paf::PhXmBar::SetOtherTopAlpha(float, float, float)
// paf_5837B7E2	  // _ZN3paf7PhXmBar16SetTopLabelAlphaEfff	paf::PhXmBar::SetTopLabelAlpha(float, float, float)
// paf_DDE862B5	  // _ZN3paf7PhXmBar17SetOtherItemAlphaEfff	paf::PhXmBar::SetOtherItemAlpha(float, float, float)
// paf_6C8F62F7	  // _ZN3paf7PhXmBar19GetFocusedItemIndexEi	paf::PhXmBar::GetFocusedItemIndex(int)
// paf_6A76DCEE	  // _ZN3paf7PhXmBar7SetFadeEfff	paf::PhXmBar::SetFade(float, float, float)
// paf_7BAAD6D1	  // _ZN3paf7PhXmBar8HideFadeEff	paf::PhXmBar::HideFade(float, float)
// paf_85BAAAA8	  // _ZN3paf7PhXmBar8IconMoveERK4vec4ffbiii	paf::PhXmBar::IconMove(vec4 const&, float, float, bool, int, int, int)
// paf_C94E8621	  // _ZN3paf7PhXmBar8SelectInEfffi	paf::PhXmBar::SelectIn(float, float, float, int)
// paf_739C8C04	  // _ZN3paf7PhXmBar8SetAlphaEfff	paf::PhXmBar::SetAlpha(float, float, float)
// paf_62ED555B	  // _ZN3paf7PhXmBar8ShowFadeEff	paf::PhXmBar::ShowFade(float, float)
// paf_EC7C4469	  // _ZN3paf7PhXmBar9HideLabelEiff	paf::PhXmBar::HideLabel(int, float, float)
// paf_EB1BB57F	  // _ZN3paf7PhXmBar9SelectOutEf	paf::PhXmBar::SelectOut(float)
// paf_8846BC65	  // _ZN3paf7PhXmBar9ShowLabelEiff	paf::PhXmBar::ShowLabel(int, float, float)
// paf_E861FA35	  // _ZN3paf7Surface10AttachCLUTERKNS_12SurfaceRCPtrINS_11SurfaceCLUTEEE	paf::Surface::AttachCLUT(paf::SurfaceRCPtr<paf::SurfaceCLUT> const&)
// paf_BFA7DC8D	  // _ZN3paf7Surface10SetFeatureENS0_7FeatureEb	paf::Surface::SetFeature(paf::Surface::Feature, bool)
// paf_698E01BE	  // _ZN3paf7Surface11GetPageSizeEii9ImageMode10ImageOrder	paf::Surface::GetPageSize(int, int, ImageMode, ImageOrder)
// paf_BC91AEB3	  // _ZN3paf7Surface12RenderTargetEv	paf::Surface::RenderTarget(void)
// paf_CC51D56	  // _ZN3paf7Surface4CopyEiPKv10ImageOrderii	paf::Surface::Copy(int, void const*, ImageOrder, int, int)
// paf_4EABD9DE	  // _ZN3paf7Surface4LockEi	paf::Surface::Lock(int)
// paf_D51B3CCE	  // _ZN3paf7Surface6UnlockEv	paf::Surface::Unlock(void)
// paf_C74837D	  // _ZN3paf7SurfaceC1EPNS_11SurfacePoolEii9ImageMode10ImageOrderbii	paf::Surface::Surface(paf::SurfacePool *, int, int, ImageMode, ImageOrder, bool, int, int)
// paf_849CCA15	  // _ZN3paf7SurfaceC1Eii9ImageMode10ImageOrderbiPvii	paf::Surface::Surface(int, int, ImageMode, ImageOrder, bool, int, void *, int, int)
// paf_278774DE	  // _ZN3paf7SurfacenwEj	paf::Surface::operator new(unsigned int)

// paf_9D191F72	  // _ZN3paf8DateTime13SetDateFormatEPKw	paf::DateTime::SetDateFormat(wchar_t const*)
// paf_2D388628	  // _ZN3paf8DateTime13SetTimeFormatEPKw	paf::DateTime::SetTimeFormat(wchar_t const*)
// paf_D251AE84	  // _ZN3paf8DateTime17SetDateTimeFormatEPKw	paf::DateTime::SetDateTimeFormat(wchar_t const*)
// paf_D4946610	  // _ZN3paf8DateTime3NowEv	paf::DateTime::Now(void)
// paf_ED5FDE32	  // _ZN3paf8DateTime6FormatEPwjPKw	paf::DateTime::Format(wchar_t *, unsigned int, wchar_t const*)
// paf_24A5BD6B	  // _ZN3paf8PhButton10WidgetTypeEv	paf::PhButton::WidgetType(void)
// paf_21C770A3	  // _ZN3paf8PhButtonC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhButton::PhButton(paf::PhWidget *, paf::PhAppear *)
// paf_647CB8EF	  // _ZN3paf8PhCamera12UpdateParamsEv	paf::PhCamera::UpdateParams(void)
// paf_7415A11D	  // _ZN3paf8PhCamera16SetVirtualScreenEfff	paf::PhCamera::SetVirtualScreen(float, float, float)
// paf_BAFE871A	  // _ZN3paf8PhCamera26SetVirtualScreenAdjustDistEff	paf::PhCamera::SetVirtualScreenAdjustDist(float, float)
// paf_9D014B62	  // _ZN3paf8PhCamera5SetupEv	paf::PhCamera::Setup(void)
// paf_F87ADBD7	  // _ZN3paf8PhCameraC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhCamera::PhCamera(paf::PhWidget *, paf::PhAppear *)
// paf_3806365F	  // _ZN3paf8PhIPAddr10WidgetTypeEv	paf::PhIPAddr::WidgetType(void)
// paf_B361BDA6	  // _ZN3paf8PhIPAddr8GetValueEPhi	paf::PhIPAddr::GetValue(unsigned char *, int)
// paf_ACCBD0C	  // _ZN3paf8PhIPAddr8SetValueEPh	paf::PhIPAddr::SetValue(unsigned char *)
// paf_9207F4	  // _ZN3paf8PhScroll10WidgetTypeEv	paf::PhScroll::WidgetType(void)
// paf_1C7E659C	  // _ZN3paf8PhScroll11ChangeValueEffi	paf::PhScroll::ChangeValue(float, float, int)
// paf_D965C0BE	  // _ZN3paf8PhScroll11SetMaxValueEf	paf::PhScroll::SetMaxValue(float)
// paf_1C83C0B6	  // _ZN3paf8PhScroll16SetVisibleAmountEf	paf::PhScroll::SetVisibleAmount(float)
// paf_1AD29E49	  // _ZN3paf8PhScroll8SetValueEf	paf::PhScroll::SetValue(float)
// paf_BC2C23A8	  // _ZN3paf8PhScrollC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhScroll::PhScroll(paf::PhWidget *, paf::PhAppear *)

// paf_65F530A4	  // _ZN3paf8PhWidget10SetColor32Ej	paf::PhWidget::SetColor32(unsigned int)
// paf_2557E245	  // _ZN3paf8PhWidget10SetPostureERK4vec4S3_PS0_	paf::PhWidget::SetPosture(vec4 const&, vec4 const&, paf::PhWidget*)
// paf_B84F742B	  // _ZN3paf8PhWidget10SetTextureERKNS_12SurfaceRCPtrINS_7SurfaceEEEi	paf::PhWidget::SetTexture(paf::SurfaceRCPtr<paf::Surface> const&, int)
// paf_546B3D02	  // _ZN3paf8PhWidget10WidgetTypeEv	paf::PhWidget::WidgetType(void)
// paf_3F4844B4	  // _ZN3paf8PhWidget11EndEditTextEv	paf::PhWidget::EndEditText(void)
//void paf_190D4130(void* widget, void* child);	  // _ZN3paf8PhWidget11RegistChildEPS0_	paf::PhWidget::RegistChild(paf::PhWidget*)
//static void PhWidget_RegistChild(void* widget, void* child) { paf_190D4130(widget, child); }

//void paf_593FB1E5(void* widget, void const* child);	  // _ZN3paf8PhWidget11RemoveChildEPKS0_	paf::PhWidget::RemoveChild(paf::PhWidget const*)
//static void PhWidget_RemoveChild(void* widget, void const* child) { paf_593FB1E5(widget, child); }
                                                        // paf_D75F6183	  // _ZN3paf8PhWidget11RemoveChildEi	paf::PhWidget::RemoveChild(int)
// paf_2CE5FF1D	  // _ZN3paf8PhWidget11SetDispatchEj	paf::PhWidget::SetDispatch(unsigned int)
// paf_D1CDDADD	  // _ZN3paf8PhWidget11UpdateStateEv	paf::PhWidget::UpdateState(void)
// paf_6846D51A	  // _ZN3paf8PhWidget12ReleaseFocusEb	paf::PhWidget::ReleaseFocus(bool)
// paf_51F323AA	  // _ZN3paf8PhWidget12SetFocusAnimEii	paf::PhWidget::SetFocusAnim(int, int)
//void paf_AFB4ECB9(void* widget, int layoutX, int layoutY, int layoutZ, vec4 pos);	  // _ZN3paf8PhWidget12SetLayoutPosEiii4vec4	paf::PhWidget::SetLayoutPos(int, int, int, vec4)
//static void PhWidget_SetLayoutPos(void* widget, int layoutX, int layoutY, int layoutZ, vec4 pos) { paf_AFB4ECB9(widget, layoutX, layoutY, layoutZ, pos); }

// paf_4A6A2F04	  // _ZN3paf8PhWidget12UpdateLayoutEb	paf::PhWidget::UpdateLayout(bool)
// paf_10893289	  // _ZN3paf8PhWidget12UpdateMatrixEv	paf::PhWidget::UpdateMatrix(void)
// paf_2BD63EAD	  // _ZN3paf8PhWidget13BeginEditTextERKNS_10PhEditText8OskParamEi	paf::PhWidget::BeginEditText(paf::PhEditText::OskParam const&, int)
// paf_16A3B00	  // _ZN3paf8PhWidget13DestroyWidgetEv	paf::PhWidget::DestroyWidget(void)
// paf_EE3D6DD0	  // _ZN3paf8PhWidget13SetAppearanceEPNS_8PhAppearE	paf::PhWidget::SetAppearance(paf::PhAppear *)
//void paf_6B2A00C5(void* widget, int layoutX, int layoutY, int layoutZ, vec4 size);	  // _ZN3paf8PhWidget13SetLayoutSizeEiii4vec4	paf::PhWidget::SetLayoutSize(int, int, int, vec4)
//static void PhWidget_SetLayoutSize(void* widget, int layoutX, int layoutY, int layoutZ, vec4 size) { paf_6B2A00C5(widget, layoutX, layoutY, layoutZ, size); }

// paf_613A24BF	  // _ZN3paf8PhWidget13UpdateCamerasEv	paf::PhWidget::UpdateCameras(void)
//void paf_384F93FC(void* widget);	  // _ZN3paf8PhWidget13UpdatePrepareEv	paf::PhWidget::UpdatePrepare(void)
//static void PhWidget_UpdatePrepare(void* widget) { paf_384F93FC(widget); }

// paf_695E35BE	  // _ZN3paf8PhWidget14GetLayoutStyleEiRiRf	paf::PhWidget::GetLayoutStyle(int, int &, float &)
// paf_4B619490	  // _ZN3paf8PhWidget14GetLayoutStyleEiRiS1_S1_R4vec4	paf::PhWidget::GetLayoutStyle(int, int &, int &, int &, vec4 &)
// paf_B36CA4B4	  // _ZN3paf8PhWidget14GetLayoutStyleEiiRiRf	paf::PhWidget::GetLayoutStyle(int, int, int &, float &)
// paf_C9AFA5	  // _ZN3paf8PhWidget14GetLayoutStyleEiiRiS1_S1_R4vec4	paf::PhWidget::GetLayoutStyle(int, int, int &, int &, int &, vec4 &)
//void paf_F6E0A48F(void* widget, int r4, int r5, float f1);	  // _ZN3paf8PhWidget14SetLayoutStyleEiif	paf::PhWidget::SetLayoutStyle(int, int, float)
//static void PhWidget_SetLayoutStyleIIF(void* widget, int r4, int r5, float f1) { paf_F6E0A48F(widget, r4, r5, f1); }

// paf_EF6C6834	  // _ZN3paf8PhWidget14SetLayoutStyleEiiif	paf::PhWidget::SetLayoutStyle(int, int, int, float)
// paf_9DF5AAD4	  // _ZN3paf8PhWidget14SetLayoutStyleEiiii4vec4	paf::PhWidget::SetLayoutStyle(int, int, int, int, vec4)
// paf_D6135FC4	  // _ZN3paf8PhWidget14SetLayoutStyleEiiiii4vec4	paf::PhWidget::SetLayoutStyle(int, int, int, int, int, vec4)
// paf_ADF743E4	  // _ZN3paf8PhWidget14SetPos_ontimerERK4vec4PS0_	paf::PhWidget::SetPos_ontimer(vec4 const&, paf::PhWidget*)
//void paf_67437488(void* widget, float* rotation, void* widgetTemplate, int r6);	  // _ZN3paf8PhWidget14SetRot_ontimerERK4vec4PS0_i	paf::PhWidget::SetRot_ontimer(vec4 const&, paf::PhWidget*, int)
//static void PhWidget_SetRot_ontimer(void* widget, float* rotation, void* widgetTemplate, int r6) { paf_67437488(widget, rotation, widgetTemplate, r6); }

// paf_B5505299	  // _ZN3paf8PhWidget15ReorderChildrenEPKS0_S2_i	paf::PhWidget::ReorderChildren(paf::PhWidget const*, paf::PhWidget const*, int)
// paf_5FD00911	  // _ZN3paf8PhWidget15SetSize_ontimerERK4vec4	paf::PhWidget::SetSize_ontimer(vec4 const&)
//void paf_BF4B155C(void* widget);	  // _ZN3paf8PhWidget15UpdateLayoutPosEv	paf::PhWidget::UpdateLayoutPos(void)
//static void PhWidget_UpdateLayoutPos(void* widget) { paf_BF4B155C(widget); }

// paf_6BDB86A9	  // _ZN3paf8PhWidget16SetMetaAlphaModeEi	paf::PhWidget::SetMetaAlphaMode(int)
// paf_73C6B7C4	  // _ZN3paf8PhWidget16SetScale_ontimerERK4vec4	paf::PhWidget::SetScale_ontimer(vec4 const&)
//void paf_DF031EDD(void* widget);	  // _ZN3paf8PhWidget16UpdateLayoutSizeEv	paf::PhWidget::UpdateLayoutSize(void)
//static void PhWidget_UpdateLayoutSize(void* widget) { paf_DF031EDD(widget); }

// paf_E434952C	  // _ZN3paf8PhWidget17GetLayoutPosValueEv	paf::PhWidget::GetLayoutPosValue(void)
// paf_B527E7E5	  // _ZN3paf8PhWidget17HandleAnalogEventEPNS_7PhEventE	paf::PhWidget::HandleAnalogEvent(paf::PhEvent *)
// paf_9DF5EBC4	  // _ZN3paf8PhWidget17UpdateLayoutStyleEi	paf::PhWidget::UpdateLayoutStyle(int)
// paf_FBAD1D7A	  // _ZN3paf8PhWidget18GetLayoutSizeValueEv	paf::PhWidget::GetLayoutSizeValue(void)
// paf_26ED24A6	  // _ZN3paf8PhWidget18HandleFocusInEventEPNS_7PhEventE	paf::PhWidget::HandleFocusInEvent(paf::PhEvent *)
// paf_4F3E14A7	  // _ZN3paf8PhWidget18HandleKeycodeEventEPNS_7PhEventE	paf::PhWidget::HandleKeycodeEvent(paf::PhEvent *)
// paf_8E8DB4B0	  // _ZN3paf8PhWidget19HandleFocusOutEventEPNS_7PhEventE	paf::PhWidget::HandleFocusOutEvent(paf::PhEvent *)
// paf_2B86EEC6	  // _ZN3paf8PhWidget20SetMetaAlpha_ontimerEfb	paf::PhWidget::SetMetaAlpha_ontimer(float, bool)
// paf_AD484F91	  // _ZN3paf8PhWidget22HandleFocusSwitchEventEPNS_7PhEventE	paf::PhWidget::HandleFocusSwitchEvent(paf::PhEvent *)
// paf_A921D353	  // _ZN3paf8PhWidget6GetPosEPS0_RK4vec4	paf::PhWidget::GetPos(paf::PhWidget*, vec4 const&)
// paf_C2E555BA	  // _ZN3paf8PhWidget6GetRotEPS0_RK4vec4i	paf::PhWidget::GetRot(paf::PhWidget*, vec4 const&, int)
// paf_683C91D8	  // _ZN3paf8PhWidget6LookAtEPS0_RK4vec4S1_S4_	paf::PhWidget::LookAt(paf::PhWidget*, vec4 const&, paf::PhWidget*, vec4 const&)
// paf_9AEFF88B	  // _ZN3paf8PhWidget6LookAtERK4vec4S3_	paf::PhWidget::LookAt(vec4 const&, vec4 const&)
// paf_241B884	  // _ZN3paf8PhWidget7SetTextERKSbIwSt11char_traitsIwESaIwEEi	paf::PhWidget::SetText(std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> const&, int)
// paf_63D446B8	  // _ZN3paf8PhWidget8DrawThisEjb	paf::PhWidget::DrawThis(unsigned int, bool)
// paf_3CF24218	  // _ZN3paf8PhWidget8GetChildEi	paf::PhWidget::GetChild(int)
// paf_D314EF39	  // _ZN3paf8PhWidget8GetStyleEiR4mat4	paf::PhWidget::GetStyle(int, mat4 &)
// paf_9379F77A	  // _ZN3paf8PhWidget8GetStyleEiR4vec4	paf::PhWidget::GetStyle(int, vec4 &)
// paf_1E54DB1E	  // _ZN3paf8PhWidget8GetStyleEiRb	paf::PhWidget::GetStyle(int, bool &)
// paf_7EF048AB	  // _ZN3paf8PhWidget8GetStyleEiRf	paf::PhWidget::GetStyle(int, float &)
// paf_547917DF	  // _ZN3paf8PhWidget8GetStyleEiRi	paf::PhWidget::GetStyle(int, int &)
// paf_C03F89E6	  // _ZN3paf8PhWidget8GetStyleEiiR4mat4	paf::PhWidget::GetStyle(int, int, mat4 &)
// paf_58F29B96	  // _ZN3paf8PhWidget8GetStyleEiiR4vec4	paf::PhWidget::GetStyle(int, int, vec4 &)
// paf_5FFC22E1	  // _ZN3paf8PhWidget8GetStyleEiiRb	paf::PhWidget::GetStyle(int, int, bool &)
// paf_A2108B85	  // _ZN3paf8PhWidget8GetStyleEiiRf	paf::PhWidget::GetStyle(int, int, float &)
// paf_B4F7ED4F	  // _ZN3paf8PhWidget8GetStyleEiiRi	paf::PhWidget::GetStyle(int, int, int &)
// paf_D2649242	  // _ZN3paf8PhWidget8SetFocusEPNS_7PhEventEj	paf::PhWidget::SetFocus(paf::PhEvent *, unsigned int)
// paf_F467C45F	  // _ZN3paf8PhWidget8SetStyleEiRK4vec4	paf::PhWidget::SetStyle(int, vec4 const&)

//void paf_7A7BCEED(void* widget, int style, bool value);	  // _ZN3paf8PhWidget8SetStyleEib	paf::PhWidget::SetStyle(int, bool)
//static void PhWidget_SetStyleB(void* widget, int style, bool value) { paf_7A7BCEED(widget, style, value); }

//void paf_A97D0803(void* widget, int style, float value);	  // _ZN3paf8PhWidget8SetStyleEif	paf::PhWidget::SetStyle(int, float)
//static void PhWidget_SetStyleF(void* widget, int style, float value) { paf_A97D0803(widget, style, value); }

//void paf_04EC636E(void* widget, int style, int value);	  // _ZN3paf8PhWidget8SetStyleEii	paf::PhWidget::SetStyle(int, int)
//static void PhWidget_SetStyleI(void* widget, int style, int value) { paf_04EC636E(widget, style, value); }

// paf_E8129023	  // _ZN3paf8PhWidget8SetStyleEiiRK4vec4	paf::PhWidget::SetStyle(int, int, vec4 const&)
// paf_B902CB91	  // _ZN3paf8PhWidget8SetStyleEiib	paf::PhWidget::SetStyle(int, int, bool)
// paf_38162763	  // _ZN3paf8PhWidget8SetStyleEiif	paf::PhWidget::SetStyle(int, int, float)
// paf_88161054	  // _ZN3paf8PhWidget8SetStyleEiii	paf::PhWidget::SetStyle(int, int, int)

//void* paf_D557F850(void* widget, char const* child, int r5);	  // _ZN3paf8PhWidget9FindChildEPKci	paf::PhWidget::FindChild(char const*, int)
//static void* PhWidget_FindChild(void* widget, char const* child, int r5) { return paf_D557F850(widget, child, r5); }


// paf_24A977F7	  // _ZN3paf8PhWidget9PauseAnimEb	paf::PhWidget::PauseAnim(bool)
// paf_C028CB7C	  // _ZN3paf8PhWidget9StartAnimEb	paf::PhWidget::StartAnim(bool)

//void paf_738BAAC0(void* widget);	  // _ZN3paf8PhWidgetD2Ev	paf::PhWidget::~PhWidget()
//static void PhWidget_Destructor(void* widget) { paf_738BAAC0(widget); }

// paf_55FE8B14	  // _ZN3paf8PhXmItem10BlinkStartEf	paf::PhXmItem::BlinkStart(float)
// paf_C84FD77B	  // _ZN3paf8PhXmItem10WidgetTypeEv	paf::PhXmItem::WidgetType(void)
// paf_A4438D60	  // _ZN3paf8PhXmItem11AnimIconSetERKNS_12SurfaceRCPtrINS_7SurfaceEEES5_RK4vec4iiiS8_iiiii	paf::PhXmItem::AnimIconSet(paf::SurfaceRCPtr<paf::Surface> const&, paf::SurfaceRCPtr<paf::Surface> const&, vec4 const&, int, int, int, vec4 const&, int, int, int, int, int)
// paf_44E93267	  // _ZN3paf8PhXmItem11PlayerStartEffRK4vec4	paf::PhXmItem::PlayerStart(float, float, vec4 const&)
// paf_DB9195CE	  // _ZN3paf8PhXmItem12AnimIconMoveERK4vec4ffiii	paf::PhXmItem::AnimIconMove(vec4 const&, float, float, int, int, int)
// paf_55921E70	  // _ZN3paf8PhXmItem12AnimIconPlayEf	paf::PhXmItem::AnimIconPlay(float)
// paf_39D3A244	  // _ZN3paf8PhXmItem12AnimIconStopEv	paf::PhXmItem::AnimIconStop(void)
// paf_CA784678	  // _ZN3paf8PhXmItem12GetLineCountEi	paf::PhXmItem::GetLineCount(int)
// paf_FB7731FB	  // _ZN3paf8PhXmItem12ReleaseLabelEi	paf::PhXmItem::ReleaseLabel(int)
// paf_4081C1E	  // _ZN3paf8PhXmItem13SetLabelAlphaEfff	paf::PhXmItem::SetLabelAlpha(float, float, float)
// paf_C49D8C3E	  // _ZN3paf8PhXmItem16UpdateLabelColorEv	paf::PhXmItem::UpdateLabelColor(void)
// paf_54A7300E	  // _ZN3paf8PhXmItem5ClearEb	paf::PhXmItem::Clear(bool)
// paf_925494CB	  // _ZN3paf8PhXmItem7FocusInEff	paf::PhXmItem::FocusIn(float, float)
// paf_3A50E116	  // _ZN3paf8PhXmItem8BlinkEndEf	paf::PhXmItem::BlinkEnd(float)
// paf_19840447	  // _ZN3paf8PhXmItem8FocusOutEff	paf::PhXmItem::FocusOut(float, float)
// paf_226FBD53	  // _ZN3paf8PhXmItem8IconMoveERK4vec4ffiii	paf::PhXmItem::IconMove(vec4 const&, float, float, int, int, int)
// paf_34C6C205	  // _ZN3paf8PhXmItem8IconSizeERK4vec4ffiii	paf::PhXmItem::IconSize(vec4 const&, float, float, int, int, int)
// paf_5F68B433	  // _ZN3paf8PhXmItem9PlayerEndEff	paf::PhXmItem::PlayerEnd(float, float)
// paf_3441B471	  // _ZN3paf8PhXmItemC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhXmItem::PhXmItem(paf::PhWidget *, paf::PhAppear *)
// paf_C5987EC3	  // _ZN3paf8PhXmList10DeleteItemEif	paf::PhXmList::DeleteItem(int, float)
// paf_2D9D1687	  // _ZN3paf8PhXmList10InsertItemEif	paf::PhXmList::InsertItem(int, float)
// paf_A166B23E	  // _ZN3paf8PhXmList10SetItemNumEi	paf::PhXmList::SetItemNum(int)
// paf_1059AF3	  // _ZN3paf8PhXmList10SetKeyMaskEi	paf::PhXmList::SetKeyMask(int)
// paf_915890C7	  // _ZN3paf8PhXmList10UpdateFormEf	paf::PhXmList::UpdateForm(float)
// paf_B35AAC2B	  // _ZN3paf8PhXmList10UpdateItemEi	paf::PhXmList::UpdateItem(int)
// paf_4FF7B8A9	  // _ZN3paf8PhXmList10WidgetTypeEv	paf::PhXmList::WidgetType(void)
// paf_EA96A0D0	  // _ZN3paf8PhXmList11AnimIconSetERKNS_12SurfaceRCPtrINS_7SurfaceEEES5_RK4vec4iiiS8_iiiii	paf::PhXmList::AnimIconSet(paf::SurfaceRCPtr<paf::Surface> const&, paf::SurfaceRCPtr<paf::Surface> const&, vec4 const&, int, int, int, vec4 const&, int, int, int, int, int)
// paf_80334194	  // _ZN3paf8PhXmList11SetItemTypeEii	paf::PhXmList::SetItemType(int, int)
// paf_F35FE0BC	  // _ZN3paf8PhXmList11SetTopAlphaEfff	paf::PhXmList::SetTopAlpha(float, float, float)
// paf_9624DDC9	  // _ZN3paf8PhXmList12SetItemAlphaEfff	paf::PhXmList::SetItemAlpha(float, float, float)
// paf_796A32FE	  // _ZN3paf8PhXmList13GetItemWidgetEi	paf::PhXmList::GetItemWidget(int)
// paf_FEDE9551	  // _ZN3paf8PhXmList13SetArrowAlphaEfff	paf::PhXmList::SetArrowAlpha(float, float, float)
// paf_A74FFFC9	  // _ZN3paf8PhXmList13SetLabelAlphaEfff	paf::PhXmList::SetLabelAlpha(float, float, float)
// paf_4DAEF6BE	  // _ZN3paf8PhXmList14ChangeListModeEii	paf::PhXmList::ChangeListMode(int, int)
// paf_B94B439F	  // _ZN3paf8PhXmList16SetTopLabelAlphaEfff	paf::PhXmList::SetTopLabelAlpha(float, float, float)
// paf_1B087988	  // _ZN3paf8PhXmList16UpdateLabelColorEv	paf::PhXmList::UpdateLabelColor(void)
// paf_5D711BEE	  // _ZN3paf8PhXmList17SetFocusItemAlphaEfff	paf::PhXmList::SetFocusItemAlpha(float, float, float)
// paf_12F2B781	  // _ZN3paf8PhXmList17SetOtherItemAlphaEfff	paf::PhXmList::SetOtherItemAlpha(float, float, float)
// paf_AC0EB3AB	  // _ZN3paf8PhXmList18SetFocusInTopColorERK4vec4ff	paf::PhXmList::SetFocusInTopColor(vec4 const&, float, float)
// paf_33A47C59	  // _ZN3paf8PhXmList18SetOtherLabelAlphaEfff	paf::PhXmList::SetOtherLabelAlpha(float, float, float)
// paf_6E0347D4	  // _ZN3paf8PhXmList18SetValueLabelAlphaEfff	paf::PhXmList::SetValueLabelAlpha(float, float, float)
// paf_A2C876E8	  // _ZN3paf8PhXmList19SetFocusOutTopColorERK4vec4ff	paf::PhXmList::SetFocusOutTopColor(vec4 const&, float, float)
// paf_D005CC9F	  // _ZN3paf8PhXmList6RedrawEi	paf::PhXmList::Redraw(int)
// paf_DEF981C4	  // _ZN3paf8PhXmList7FocusInEf	paf::PhXmList::FocusIn(float)
// paf_7047F37	  // _ZN3paf8PhXmList8BlinkEndEf	paf::PhXmList::BlinkEnd(float)
// paf_814B3D90	  // _ZN3paf8PhXmList8FocusOutEf	paf::PhXmList::FocusOut(float)
// paf_6964C72A	  // _ZN3paf8PhXmList8IconMoveEiRK4vec4ffbiii	paf::PhXmList::IconMove(int, vec4 const&, float, float, bool, int, int, int)
// paf_2689337F	  // _ZN3paf8PhXmList8IconSizeEiRK4vec4ffiii	paf::PhXmList::IconSize(int, vec4 const&, float, float, int, int, int)
// paf_459B404D	  // _ZN3paf8PhXmList9ArrowMoveERK4vec4ffi	paf::PhXmList::ArrowMove(vec4 const&, float, float, int)
// paf_27D4B92D	  // _ZN3paf8PhXmList9HideItemsEff	paf::PhXmList::HideItems(float, float)
// paf_BB12535D	  // _ZN3paf8PhXmList9SelectOutEf	paf::PhXmList::SelectOut(float)
// paf_14CF6448	  // _ZN3paf8PhXmList9ShowItemsEff	paf::PhXmList::ShowItems(float, float)
// paf_89E5CC7F	  // _ZN3paf8SyncCall4CallEPFiPvES1_	paf::SyncCall::Call(int (*)(void *), void *)
// paf_EB7D2C26	  // _ZN3paf8SyncCall5CheckEv	paf::SyncCall::Check(void)
// paf_1CCF9E4D	  // _ZN3paf8SyncCallC1Ev	paf::SyncCall::SyncCall(void)
// paf_CC9C89FB	  // _ZN3paf8SyncCallD1Ev	paf::SyncCall::~SyncCall()

// paf_CFF699A0	  // _ZN3paf9CallQueue4PostEPFvPvES1_	paf::CallQueue::Post(void (*)(void *), void *)
// paf_9F58ADCF	  // _ZN3paf9CallQueue5CheckEv	paf::CallQueue::Check(void)
// paf_ED7791B6	  // _ZN3paf9Framework16GetSoundInstanceEv	paf::Framework::GetSoundInstance(void)
// paf_AF58E756	  // _ZN3paf9Framework8InstanceEv	paf::Framework::Instance(void)
// paf_541F8EBB	  // _ZN3paf9Framework9InitParamC1Ev	paf::Framework::InitParam::InitParam(void)
// paf_A5B4FB55	  // _ZN3paf9FrameworkC1ERKNS0_9InitParamE	paf::Framework::Framework(paf::Framework::InitParam const&)
// paf_54E2C229	  // _ZN3paf9FrameworkD1Ev	paf::Framework::~Framework()
// paf_868C48A1	  // _ZN3paf9HalfImageEPvPKviiii	paf::HalfImage(void *, void const*, int, int, int, int)
// paf_FDCB65D3	  // _ZN3paf9ImageAttr12SetDecOptionENS_15ImageOptionCodeEi	paf::ImageAttr::SetDecOption(paf::ImageOptionCode, int)
// paf_350B4536	  // _ZN3paf9Job_StartEPNS_9Job_QueueEPFiPvPNS_10Job_ThreadEES2_iiPFviS2_S4_iE	paf::Job_Start(paf::Job_Queue *, int (*)(void *, paf::Job_Thread *), void *, int, int, void (*)(int, void *, paf::Job_Thread *, int))

//int paf_FE5A7950(void* handler, int cbId, void* phEvent);	  // _ZN3paf9PhHandler10DoCallBackEiPNS_7PhEventE	paf::PhHandler::DoCallBack(int, paf::PhEvent *)
//static int PhHandler_DoCallback(void* handler, int cbId, void* phEvent) { return paf_FE5A7950(handler, cbId, phEvent); }

//int paf_B60C4316(void* handler, void* phEvent);	  // _ZN3paf9PhHandler11HandlerProcEPNS_7PhEventE	paf::PhHandler::HandlerProc(paf::PhEvent *)
//static int PhHandler_HandlerProc(void* handler, void* phEvent) { return paf_B60C4316(handler, phEvent); }

//int paf_2CBA5A33(void* handler, int cb);	  // _ZN3paf9PhHandler11KillTimerCBEi	paf::PhHandler::KillTimerCB(int)
//static int PhHandler_KillTimerCB(void* handler, int cb) { return paf_2CBA5A33(handler, cb); }

//int paf_986427A7(void* handler, int cbId, void(*cb)(void* widget, void* phEvent, void*), void*);	  // _ZN3paf9PhHandler11SetCallBackEiPFvPNS_8PhWidgetEPNS_7PhEventEPvES5_	paf::PhHandler::SetCallBack(int, void (*)(paf::PhWidget *, paf::PhEvent *, void *), void *)
//static int PhHandler_SetCallback(void* handler, int cbId, void(*cb)(void* widget, void* phEvent, void*), void* a3) { return paf_986427A7(handler, cbId, cb, a3); }

//int paf_818162F2(void* handler, void(*cb)(void* widget, void* phEvent, void*), void*);		  // _ZN3paf9PhHandler11SetDeleteCBEPFvPNS_8PhWidgetEPNS_7PhEventEPvES5_	paf::PhHandler::SetDeleteCB(void (*)(paf::PhWidget *, paf::PhEvent *, void *), void *)
//static int PhHandler_SetDeleteCB(void* handler, void(*cb)(void* widget, void* phEvent, void*), void* a3) { return paf_818162F2(handler, cb, a3); }

//int paf_F7D65DC1(void* handler, void* phEvent);	  // _ZN3paf9PhHandler16HandleFocusEventEPNS_7PhEventE	paf::PhHandler::HandleFocusEvent(paf::PhEvent *)
//static int PhHandler_HandleFocusEvent(void* handler, void* phEvent) { return paf_F7D65DC1(handler, phEvent); }

//int paf_DBFE46EA(void* handler, void* phEvent);	  // _ZN3paf9PhHandler16HandlePointEventEPNS_7PhEventE	paf::PhHandler::HandlePointEvent(paf::PhEvent *)
//static int PhHandler_HandlePointEvent(void* handler, void* phEvent) { return paf_DBFE46EA(handler, phEvent); }

//int paf_C051D361(void* handler, void* phEvent);	  // _ZN3paf9PhHandler16HandleStateEventEPNS_7PhEventE	paf::PhHandler::HandleStateEvent(paf::PhEvent *)
//static int PhHandler_HandleStateEvent(void* handler, void* phEvent) { return paf_C051D361(handler, phEvent); }

//int paf_C96D94F(void* handler, void* phEvent);	  // _ZN3paf9PhHandler17HandleSystemEventEPNS_7PhEventE	paf::PhHandler::HandleSystemEvent(paf::PhEvent *)
//static int PhHandler_HandleSystemEvent(void* handler, void* phEvent) { return paf_C96D94F(handler, phEvent); }

//int paf_CE14748C(void* handler, void* phEvent);	  // _ZN3paf9PhHandler18HandleCommandEventEPNS_7PhEventE	paf::PhHandler::HandleCommandEvent(paf::PhEvent *)
//static int PhHandler_HandleCommandEvent(void* handler, void* phEvent) { return paf_CE14748C(handler, phEvent); }

//int paf_A4D0FDCB(void* handler, void* phEvent);	  // _ZN3paf9PhHandler18HandleKeycodeEventEPNS_7PhEventE	paf::PhHandler::HandleKeycodeEvent(paf::PhEvent *)
//static int PhHandler_HandleKeycodeEvent(void* handler, void* phEvent) { return paf_A4D0FDCB(handler, phEvent); }

//bool paf_4D0CAA13(void* handler, int cbId);	  // _ZN3paf9PhHandler9IsTimerCBEi	paf::PhHandler::IsTimerCB(int)
//static bool PhHandler_IsTimerCB(void* handler, int cbId) { return paf_4D0CAA13(handler, cbId); }

// paf_59A11C82	  // _ZN3paf9PhNumSpin10WidgetTypeEv	paf::PhNumSpin::WidgetType(void)
// paf_B923666A	  // _ZN3paf9PhNumSpin11UpdateStateEv	paf::PhNumSpin::UpdateState(void)
// paf_B367A64E	  // _ZN3paf9PhNumSpin12UpdateLayoutEb	paf::PhNumSpin::UpdateLayout(bool)
// paf_233E39D1	  // _ZN3paf9PhNumSpin14SetLayoutStyleEiif	paf::PhNumSpin::SetLayoutStyle(int, int, float)
// paf_EC70F53F	  // _ZN3paf9PhNumSpin14SetLayoutStyleEiiif	paf::PhNumSpin::SetLayoutStyle(int, int, int, float)
// paf_8043F265	  // _ZN3paf9PhNumSpin16SetColor_ontimerERK4vec4	paf::PhNumSpin::SetColor_ontimer(vec4 const&)
// paf_85BA32C4	  // _ZN3paf9PhNumSpin6CreateEiibiPPNS_7PhSTextEPKi	paf::PhNumSpin::Create(int, int, bool, int, paf::PhSText **, int const*)
// paf_24EBED2B	  // _ZN3paf9PhNumSpin7SetTextERKSbIwSt11char_traitsIwESaIwEEi	paf::PhNumSpin::SetText(std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> const&, int)
// paf_92A06C2B	  // _ZN3paf9PhNumSpin8DrawThisEjb	paf::PhNumSpin::DrawThis(unsigned int, bool)
// paf_9FF4CD0A	  // _ZN3paf9PhNumSpin8GetStyleEiRb	paf::PhNumSpin::GetStyle(int, bool &)
// paf_15BBE128	  // _ZN3paf9PhNumSpin8GetStyleEiiR4vec4	paf::PhNumSpin::GetStyle(int, int, vec4 &)
// paf_EE7108A5	  // _ZN3paf9PhNumSpin8SetStyleEib	paf::PhNumSpin::SetStyle(int, bool)
// paf_FC406066	  // _ZN3paf9PhNumSpin8SetStyleEif	paf::PhNumSpin::SetStyle(int, float)
// paf_37A6B2F6	  // _ZN3paf9PhNumSpin8SetStyleEiiRK4vec4	paf::PhNumSpin::SetStyle(int, int, vec4 const&)
// paf_ECB11C5E	  // _ZN3paf9PhNumSpin8SetStyleEiib	paf::PhNumSpin::SetStyle(int, int, bool)
// paf_E4B244BE	  // _ZN3paf9PhNumSpin8SetStyleEiif	paf::PhNumSpin::SetStyle(int, int, float)
// paf_D14F5F81	  // _ZN3paf9PhNumSpin8SetStyleEiii	paf::PhNumSpin::SetStyle(int, int, int)
// paf_BDA96809	  // _ZN3paf9PhNumSpinC1EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhNumSpin::PhNumSpin(paf::PhWidget *, paf::PhAppear *)
// paf_BAFCBA67	  // _ZN3paf9PhNumSpinC2EPNS_8PhWidgetEPNS_8PhAppearE	paf::PhNumSpin::PhNumSpin(paf::PhWidget *, paf::PhAppear *)
// paf_7893B654	  // _ZN3paf9PhNumSpinD2Ev	paf::PhNumSpin::~PhNumSpin()
// paf_B8979AD7	  // _ZN3paf9PhSRender8SetStyleEiRK4vec4	paf::PhSRender::SetStyle(int, vec4 const&)
// paf_EB793E25	  // _ZN3paf9PhSRender8SetStyleEib	paf::PhSRender::SetStyle(int, bool)
// paf_A28C8100	  // _ZN3paf9PhSRender8SetStyleEif	paf::PhSRender::SetStyle(int, float)
// paf_E22D7B0C	  // _ZN3paf9PhSRender8SetStyleEii	paf::PhSRender::SetStyle(int, int)

// paf_14A9CC1D	  // _ZN4mat4C1Ef	mat4::mat4(float)
// paf_964CE2DD	  // _ZN4mat4C1Effffffffffffffff	mat4::mat4(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float)
// paf_BC6E66D6	  // _ZN4mat4aSEf	mat4::operator=(float)
// paf_EE8F2CC4	  // _ZN4mat4ixEi	mat4::operator[](int)

// paf_4F2A97E5	  // _ZN4rectC1Ef	rect::rect(float)
// paf_FADF8AFF	  // _ZN4rectC1Effff	rect::rect(float, float, float, float)
// paf_A922709B	  // _ZN4rectaSEf	rect::operator=(float)

// paf_18320785	  // _ZN4vec23setEff	vec2::set(float, float)
// paf_84484D13	  // _ZN4vec2C1Ef	vec2::vec2(float)
// paf_B1A2076D	  // _ZN4vec2aSERK4vec4	vec2::operator=(vec4 const&)
// paf_DF27BD9B	  // _ZN4vec3C1Efff	vec3::vec3(float, float, float)
// paf_D72BCDCB	  // _ZN4vec4C1ERK4vec2	vec4::vec4(vec2 const&)
// paf_D84691B	  // _ZN4vec4C1Ef	vec4::vec4(float)
// paf_C44F501D	  // _ZN4vec4aSEf	vec4::operator=(float)
// paf_585EC026	  // _ZN4vec4ixEi	vec4::operator[](int)

// paf_2B50BE7D	  // _ZNK3paf10PhItemSpin9IsInheritEPKc	paf::PhItemSpin::IsInherit(char const*)const
// paf_B71DC906	  // _ZNK3paf11PhLabelText12GetLineCountEv	paf::PhLabelText::GetLineCount(void)const
// paf_161E2246	  // _ZNK3paf11PhLabelText12GetMaxHeightEv	paf::PhLabelText::GetMaxHeight(void)const
// paf_3B002423	  // _ZNK3paf11PhLabelText12GetTextWidthEv	paf::PhLabelText::GetTextWidth(void)const
// paf_46E3E3FD	  // _ZNK3paf11PhLabelText13GetLineHeightEv	paf::PhLabelText::GetLineHeight(void)const
// paf_2B35D7DD	  // _ZNK3paf11PhLabelText14GetMaxAscenderEv	paf::PhLabelText::GetMaxAscender(void)const
// paf_B8A630CD	  // _ZNK3paf11SurfaceBase13PixelsToBytesEi	paf::SurfaceBase::PixelsToBytes(int)const
// paf_EDB089C8	  // _ZNK3paf11SurfaceBase20GetDevicePixelFormatEv	paf::SurfaceBase::GetDevicePixelFormat(void)const
// paf_5C54EEAF	  // _ZNK3paf11SurfacePool11GetFreeSizeEv	paf::SurfacePool::GetFreeSize(void)const
// paf_794CEACB	  // _ZNK3paf4View10FindWidgetEPKc	paf::View::FindWidget(char const*)const
// paf_3A8454FC	  // _ZNK3paf4View10GetTextureEPKc	paf::View::GetTexture(char const*)const
// paf_A499E2BE	  // _ZNK3paf4View10GetTextureEPv	paf::View::GetTexture(void *)const
// paf_42B2CD4A	  // _ZNK3paf4View10GetTopPageEv	paf::View::GetTopPage(void)const
// paf_23AFB290	  // _ZNK3paf4View12GetInterfaceEi	paf::View::GetInterface(int)const
// paf_430FCFED	  // _ZNK3paf4View6NotifyEi	paf::View::Notify(int)const
// paf_46D0F875	  // _ZNK3paf4View7GetPageEPKc	paf::View::GetPage(char const*)const
// paf_F5BEB953	  // _ZNK3paf4View8ArgumentcvSsEv	paf::View::Argument::operator std::string(void)const
// paf_E5B6AF9E	  // _ZNK3paf4View8ArgumentcviEv	paf::View::Argument::operator int(void)const
// paf_99919819	  // _ZNK3paf4View8IsActiveEv	paf::View::IsActive(void)const
// paf_11E195B3	  // _ZNK3paf4View8PageRootEPKc	paf::View::PageRoot(char const*)const
// paf_BCAACE40	  // _ZNK3paf4View8PageRootEPv	paf::View::PageRoot(void *)const
// paf_89B67B9C	  // _ZNK3paf4View9GetStringEPKc	paf::View::GetString(char const*)const
int paf_B9152E15(void*, int);	  // _ZNK3paf6Module12GetInterfaceEi	paf::Module::GetInterface(int)const
// paf_2AB5840	  // _ZNK3paf6PhFont11GetCharInfoENS0_9GlyphTypeEtP18SceFont_t_charInfo	paf::PhFont::GetCharInfo(paf::PhFont::GlyphType, unsigned short, SceFont_t_charInfo *)const
// paf_723DB220	  // _ZNK3paf6PhFont11GetCharInfoENS0_9GlyphTypeEtPNS0_10BitmapInfoEPi	paf::PhFont::GetCharInfo(paf::PhFont::GlyphType, unsigned short, paf::PhFont::BitmapInfo *, int *)const

// paf_304EAE6F	  // _ZNK3paf6PhText12GetFirstLineEv	paf::PhText::GetFirstLine(void)const
//int paf_E8355FCC(void* text);	  // _ZNK3paf6PhText12GetLineCountEv	paf::PhText::GetLineCount(void)const
//static int PhText_GetLineCount(void* text) { return paf_E8355FCC(text); }

//float paf_65036474(void* text);	  // _ZNK3paf6PhText12GetTextWidthEv	paf::PhText::GetTextWidth(void)const
//static float PhText_GetTextWidth(void* text) { return paf_65036474(text); }

// paf_77479F6F	  // _ZNK3paf6PhText13GetCurrentPosEv	paf::PhText::GetCurrentPos(void)const

//float paf_20477524(void* text);	  // _ZNK3paf6PhText13GetLineHeightEv	paf::PhText::GetLineHeight(void)const
//static float PhText_GetLineHeight(void* text) { return paf_20477524(text); }

// paf_3F10A8D7	  // _ZNK3paf6PhText13GetTextLengthEv	paf::PhText::GetTextLength(void)const
// paf_4F5319E0	  // _ZNK3paf6PhText14GetCurrentLineEv	paf::PhText::GetCurrentLine(void)const
// paf_9196EEB8	  // _ZNK3paf6PhText15GetColumnOffsetEv	paf::PhText::GetColumnOffset(void)const
// paf_68D2B06D	  // _ZNK3paf6PhText15GetVisibleLinesEv	paf::PhText::GetVisibleLines(void)const
// paf_9FBCD5C8	  // _ZNK3paf7PhPlane9IsInheritEPKc	paf::PhPlane::IsInherit(char const*)const
// paf_29C739F4	  // _ZNK3paf7PhSPrim8GetStyleEiR4mat4	paf::PhSPrim::GetStyle(int, mat4 &)const
// paf_99DB254A	  // _ZNK3paf7PhSPrim8GetStyleEiR4vec4	paf::PhSPrim::GetStyle(int, vec4 &)const
// paf_E2925302	  // _ZNK3paf7PhSPrim8GetStyleEiRb	paf::PhSPrim::GetStyle(int, bool &)const
// paf_F7B61EE3	  // _ZNK3paf7PhSPrim8GetStyleEiRf	paf::PhSPrim::GetStyle(int, float &)const
// paf_86E89034	  // _ZNK3paf7PhSPrim8GetStyleEiRi	paf::PhSPrim::GetStyle(int, int &)const
// paf_BE085CD4	  // _ZNK3paf7PhSText8GetStyleEiR4vec4	paf::PhSText::GetStyle(int, vec4 &)const
// paf_CBDF7130	  // _ZNK3paf7PhSText8GetStyleEiRb	paf::PhSText::GetStyle(int, bool &)const
// paf_F5116A56	  // _ZNK3paf7PhSText8GetStyleEiRf	paf::PhSText::GetStyle(int, float &)const
// paf_D2DC1C89	  // _ZNK3paf7PhSText8GetStyleEiRi	paf::PhSText::GetStyle(int, int &)const
// paf_168EA34B	  // _ZNK3paf7PhScene14GetCameraCountEv	paf::PhScene::GetCameraCount(void)const
// paf_C47FB4B9	  // _ZNK3paf7PhScene9GetCameraEi	paf::PhScene::GetCamera(int)const
// paf_2E5D1BB3	  // _ZNK3paf7PhTimer6DoFuncEfi	paf::PhTimer::DoFunc(float, int)const
// paf_DD8E775C	  // _ZNK3paf7Surface10GetPagePtrEi	paf::Surface::GetPagePtr(int)const
// paf_E81D263A	  // _ZNK3paf7Surface13GetActivePageENS0_8PageModeE	paf::Surface::GetActivePage(paf::Surface::PageMode)const
// paf_7A200FA1	  // _ZNK3paf8PhCamera16GetVirtualScreenERfS1_S1_	paf::PhCamera::GetVirtualScreen(float &, float &, float &)const
// paf_51D70976	  // _ZNK3paf8PhWidget10GetTextureERNS_12SurfaceRCPtrINS_7SurfaceEEEi	paf::PhWidget::GetTexture(paf::SurfaceRCPtr<paf::Surface> &, int)const
// paf_44CDAE21	  // _ZNK3paf8PhWidget12GetLayoutPosERiS1_S1_R4vec4	paf::PhWidget::GetLayoutPos(int &, int &, int &, vec4 &)const
// paf_3242B3F4	  // _ZNK3paf8PhWidget13GetLayoutSizeERiS1_S1_R4vec4	paf::PhWidget::GetLayoutSize(int &, int &, int &, vec4 &)const
// paf_E1654E20	  // _ZNK3paf8PhWidget15GetEditTextPartERi	paf::PhWidget::GetEditTextPart(int &)const
// paf_A9E290A	  // _ZNK3paf8PhWidget7GetTextERSbIwSt11char_traitsIwESaIwEEi	paf::PhWidget::GetText(std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> &, int)const
// paf_F5C65829	  // _ZNK3paf8PhWidget9IsInheritEPKc	paf::PhWidget::IsInherit(char const*)const
// paf_880E3977	  // _ZNK3paf9ImageAttr11GetMemWidthEi	paf::ImageAttr::GetMemWidth(int)const
// paf_CA9160F6	  // _ZNK3paf9PhNumSpin13GetWidgetTypeEv	paf::PhNumSpin::GetWidgetType(void)const
// paf_6C5A6746	  // _ZNK3paf9PhNumSpin9IsInheritEPKc	paf::PhNumSpin::IsInherit(char const*)const
// paf_246B55D0	  // _ZNK3paf9PhSRender12SetupTextureERKNS_12SurfaceRCPtrINS_7SurfaceEEE	paf::PhSRender::SetupTexture(paf::SurfaceRCPtr<paf::Surface> const&)const
// paf_14B72562	  // _ZNK3paf9PhSRender12SetupTextureERKNS_12SurfaceRCPtrINS_7SurfaceEEERK4vec4	paf::PhSRender::SetupTexture(paf::SurfaceRCPtr<paf::Surface> const&, vec4 const&)const
// paf_BF575328	  // _ZNK3paf9PhSRender8GetStyleEiR4mat4	paf::PhSRender::GetStyle(int, mat4 &)const
// paf_DF3A2CA7	  // _ZNK3paf9PhSRender8GetStyleEiR4vec4	paf::PhSRender::GetStyle(int, vec4 &)const
// paf_57B6F6B2	  // _ZNK3paf9PhSRender8GetStyleEiRb	paf::PhSRender::GetStyle(int, bool &)const
// paf_C35764DF	  // _ZNK3paf9PhSRender8GetStyleEiRf	paf::PhSRender::GetStyle(int, float &)const
// paf_9C778A46	  // _ZNK3paf9PhSRender8GetStyleEiRi	paf::PhSRender::GetStyle(int, int &)const

// paf_9EB7F788	  // _ZNK4vec4eqERKS_	vec4::operator==(vec4 const&)const

// paf_CA16B292	  // _ZmLR4mat4RKS_	operator*=(mat4 &, mat4 const&)
// paf_23F6755	  // _ZmLR4vec4RKS_	operator*=(vec4 &, vec4 const&)
// paf_87D90A81	  // _ZmiRK4vec4S1_	operator-(vec4 const&, vec4 const&)
// paf_DA6B5FAD	  // _ZmlRK4mat4RK4vec4	operator*(mat4 const&, vec4 const&)
// paf_DC05D196	  // _ZmlRK4mat4S1_	operator*(mat4 const&, mat4 const&)
// paf_151D5C78	  // _ZmlRK4vec4S1_	operator*(vec4 const&, vec4 const&)
// paf_4F381F53	  // _ZmlRK4vec4f	operator*(vec4 const&, float)
// paf_724042E6	  // _ZpLR4vec4RKS_	operator+=(vec4 &, vec4 const&)
// paf_AABDBC6E	  // _ZplRK4vec4S1_	operator+(vec4 const&, vec4 const&)

// paf_1EFB3790	  // pafGuBlendFunc
// paf_399C0BF0	  // pafGuCgCreateFragmentShader
// paf_A65A3868	  // pafGuCgCreateShader
// paf_CA103173	  // pafGuCgCreateVertexShader
// paf_8FEC308A	  // pafGuCgPopShader
// paf_64C305D6	  // pafGuCgPushShader
// paf_97BD8488	  // pafGuCgReleaseFragmentShader
// paf_B1D5806A	  // pafGuCgReleaseVertexShader
// paf_6B7D0D60	  // pafGuCgSetFragmentShader
// paf_E921EAA5	  // pafGuCgSetShader
// paf_D61B5EFC	  // pafGuCgSetVertexShader
// paf_20E5286E	  // pafGuClear
// paf_84D5E999	  // pafGuClearColor
// paf_19731532	  // pafGuClearDepth
// paf_ADD67C35	  // pafGuColor4f
// paf_BD1BF0B7	  // pafGuDataLocation
// paf_3F2FD49C	  // pafGuDepthBuffer
// paf_9F3D573E	  // pafGuDepthFunc
// paf_E4FE1333	  // pafGuDepthMask
// paf_E93B9B31	  // pafGuDisable
// paf_41C145DC	  // pafGuDrawArray
// paf_39B40DF9	  // pafGuDrawBuffer
// paf_CA1E9947	  // pafGuEnable
// paf_F00CAD11	  // pafGuFrameCount
// paf_2C519D3B	  // pafGuFrontFace
// paf_A0FCDF5F	  // pafGuGetCurrDisplayDeviceSurf
// paf_7D45A4FF	  // pafGuGetCurrVirtualScreenSurf
// paf_76402CA	  // pafGuGetDisplayMode
// paf_AC984A12	  // pafGuGetDrawSurfH
// paf_F476E8AA	  // pafGuGetDrawSurfW
// paf_FFACD6D	  // pafGuGetVideoMemoryTotalArea
// paf_98F14FD9	  // pafGuGetVirtualScreenSurf
// paf_2E34FF61	  // pafGuMuteDisplay
// paf_124DDBC5	  // pafGuRefreshStatus
// paf_3C7E4CE0	  // pafGuScissor
// paf_9C1A2D6A	  // pafGuSetDrawSurfH
// paf_A01698C9	  // pafGuSetDrawSurfW
// paf_D6AEAC39	  // pafGuShadeModel
// paf_4B9B85B4	  // pafGuSwapCount
// paf_36D6368	  // pafGuSwapStatus
// paf_2164ACB8	  // pafGuSyncGpu
// paf_3B90102F	  // pafGuSyncGpuAll
// paf_C8F4E2CF	  // pafGuTexFilter
// paf_62854AA7	  // pafGuTexImage
// paf_344E1CEB	  // pafGuTexImage2
// paf_C147B2	  // pafGuTexLevelMode
// paf_6001C52A	  // pafGuTexMode
// paf_BB0AA6D5	  // pafGuTexOffset
// paf_2D6B25EA	  // pafGuTexScale
// paf_6941C365	  // pafGuTexWrap
// paf_191AD326	  // pafGuViewport
// paf_4DED0C42	  // sceWaveAudioGetRestSample
// paf_467B2193	  // sceWaveAudioSetFormat
// paf_F75DD0B5	  // sceWaveAudioSetSample
// paf_483CBA61	  // sceWaveAudioSetVolumeEx
// paf_C5EC06C5	  // sceWaveAudioWriteBlockingEx

// paf_65BE85B3	  // _UnloadView	_paf_65BE85B3(char * plugin_name,0,0)
// paf_A1DC401	  // sets Interface from plugin	uint32_t SetInterface(unknown, uint32_t identifier, void * ptr_structure)
// paf_23AFB290	  // gets Interface from plugin, should allow calling its exports/funcs	uint32_t plugin->GetInterface(uint32_t return from paf_F21655F3, uint32_t identifier)
// paf_3A8454FC	  // finds/gets Texture resource	Example: out: uint32_t* texture, return from paf_F21655F3, "tex_notification_info"
// paf_50CDB465	  // finds/gets Sound resource	uint32_t paf_50CDB465(uint32_t plugin /*paf_F21655F3*/, const char* sound)
// paf_3CEC3833	  // sets up a plugin widget ? (page_...)
// paf_794CEACB	  // finds widget in a plugin	Example: plugin->FindWidget(return from paf_F21655F3, "page_autooff_guide")
// paf_89B67B9C	  // gets localized string text from plugin resource (Different strings are returned depending on the system language)	Example: (w)char (paf_F21655F3("system_plugin"), "msg_signed_out")
// paf_F21655F3	  // finds a loaded plugin	uint32_t paf::View::Find(const char* plugin) Example: uint32_t paf::View::Find("system_plugin")
//void paf_CF068D31(void*, const char*, int, int, void*);	  // load (optional on memcontainer) and start sprx	void paf_CF068D31(int32_t* prx_struct, const char* path, int /*ignored*/, int /*0*/, (optional) const sys_memory_container_t *mem_container)
//void paf_2F5CEE6D(void*);	  // stop and unload sprx	void paf_2F5CEE6D(int32_t* prx_struct)

// paf_546B3D02	  // returns "PhWidget"	const char* paf_546B3D02()
// paf_41BBFE5E	  // returns "PhScene"	const char* paf_41BBFE5E()
// paf_10DEDCC7	  // returns "PhPlane"	const char* paf_10DEDCC7()
// paf_E36C18F5	  // returns "PhPlaneDiv"	const char* paf_E36C18F5()
// paf_24A5BD6B	  // returns "PhButton"	const char* paf_24A5BD6B()
// paf_B7DFCE90	  // returns "PhText"	const char* paf_B7DFCE90()
// paf_9207F4	  // returns "PhScroll"	const char* paf_9207F4()
// paf_BA6D149A	  // returns "PhLabelPrim"	const char* paf_BA6D149A()
// paf_C88CA4B2	  // returns "PhLevelMeter"	const char* paf_C88CA4B2()
// paf_E801C345	  // returns "PhProgress"	const char* paf_E801C345()
// paf_BF66BF2D	  // returns "PhCheckBox"	const char* paf_BF66BF2D()
// paf_703117AD	  // returns "PhXmBar"	const char* paf_703117AD()
// paf_4FF7B8A9	  // returns "PhXmList"	const char* paf_4FF7B8A9()
// paf_C84FD77B	  // returns "PhXmItem"	const char* paf_C84FD77B()
// paf_4C36ABBB	  // returns "PhItemSpin"	const char* paf_4C36ABBB()
// paf_CA9160F6	  // returns "PhNumSpin"	const char* paf_CA9160F6()
// paf_59A11C82	  // returns "PhNumSpin"	const char* paf_59A11C82()
// paf_D64EDE7C	  // returns "PhList"	const char* paf_D64EDE7C()
// paf_F7630798	  // returns "PhInfoList"	const char* paf_F7630798()
// paf_A98865F8	  // returns "PhMenuList"	const char* paf_A98865F8()
// paf_90F4F801	  // returns "PhCheckBoxList"	const char* paf_90F4F801()
// paf_DDD4ACF6	  // returns "PhLabelText"	const char* paf_DDD4ACF6()
// paf_545D47A2	  // returns "PhClock"	const char* paf_545D47A2()
// paf_3806365F	  // returns "PhIPAddr"	const char* paf_3806365F()
// paf_AF58E756	  // Gets the base of loaded plugins. The pointer returned is used in paf::View::Find to find plugins.	void* paf_AF58E756()
// paf_5F5DA385	  // Gets Dialog Size	int paf_5F5DA385( uint16_t * size_width, uint16_t * size_height, int dialog_type)


#endif // __PAF_H__
