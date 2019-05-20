#ifndef __MODULESPATCH_H__
#define __MODULESPATCH_H__

#include <lv2/process.h>
#include <lv2/thread.h>

#if defined(FIRMWARE_4_82) || defined(FIRMWARE_4_84)

#define VSH_HASH						0xb6b6d000002e0000
#define BDP_DISC_CHECK_PLUGIN_HASH		0x9940000000003000
#define EXPLORE_PLUGIN_HASH				0xc50d0000000e0000
#define EXPLORE_CATEGORY_GAME_HASH		0xde52d00000050000
#define PS1_EMU_HASH					0xcc28400000090000
#define PS1_NETEMU_HASH 				0xcc284000000b0000
#define GAME_EXT_PLUGIN_HASH			0xcc26800000010000
#define LIBFS_EXTERNAL_HASH				0x05fd000000006000
#define NAS_PLUGIN_HASH				0xc50d000000020000
#define BDP_BDMV_HASH				0xcc28700000370000
#define BDP_BDVD_HASH				0xcc28d00000430000
#define DOWNLOAD_PLUGIN_HASH				0xcc26800000020000
#define AUTODOWNLOAD_PLUGIN_HASH				0x6a1af00000040000
#define PREMO_PLUGIN_HASH				0x1000000000010000
#define PREMO_GAME_PLUGIN_HASH			0x5bc3000000010000
//// Currently unused ////
#define BASIC_PLUGINS_HASH				0x55f4700000010000
#define PSP_EMULATOR_HASH				0xcc29b00000020000
#define PEMUCORELIB_HASH				0x40425000000c0000
#define EMULATOR_API_HASH				0x8409f00000010000
#define EMULATOR_DRM_HASH				0xbbb8800000005000
#define EMULATOR_DRM_DATA_HASH			0x2f3ab00000010000
#define LIBSYSUTIL_SAVEDATA_PSP_HASH	0x0dfdc00000003000

/* download_plugin */
#define elf_patch1_download				0x42e2c
#define elf_patch2_download				0x42e34
/// advanced download patches found by Devil303
#define elf_patch3_download				0x4D708
#define elf_patch4_download				0x4E528
#define elf_patch5_download				0x4E548
#define elf_patch6_download				0x4E624

/* autodownload_plugin */
#define elf_patch1_autodownload                0x82650  
#define elf_patch2_autodownload                0x82658

/*Cinavia patch*/
#define bdp_cinavia_patch				0x15ecd0
#define bdp_cinavia1_patch				0x15ecfc

/* vsh */
#define vsh_text_size					0x6B0000
#define ps2tonet_patch					0xC4E34
#define ps2tonet_size_patch 			0xC4E28
#define psp_drm_patch1					0x242FC0
#define psp_drm_patch2					0x243A60
#define psp_drm_patch3					0x24369C
#define psp_drm_patch4					0x243EA8
#define psp_drm_patchA					0x2430DC
#define psp_drm_patchB					0x24397C
#define psp_drm_patchC					0x242B14
#define psp_drm_patchD					0x2430C4
#define psp_drm_patchE					0x2430C8
#define psp_drm_patchF					0x243A94
#define vmode_patch_offset				0x4431DC

/* basic_plugins */
//#define ps1emu_type_check_offset		0x1F78C
//#define pspemu_path_offset			0x4A9F8
//#define psptrans_path_offset			0x4B628

/* explore_plugin */
#define app_home_offset      			0x2514C0 // HAN support files has DEX version of this module
#define ps2_nonbw_offset      			0xDDDC4	// HAN support files has DEX version of this module
//#define app_home_offset				0x250910 // Retail
//#define ps2_nonbw_offset				0xDD9B4 // Retail

/* explore_category_game */
#define ps2_nonbw_offset2				0x68324

/* bdp_disccheck_plugin */
#define dvd_video_region_check_offset	0x152C

/* ps1_emu */
#define ps1_emu_get_region_offset		0x3E74

/* ps1_netemu */
#define ps1_netemu_get_region_offset	0xA4E74

/* game_ext_plugin */
#define sfo_check_offset				0x23B90
#define ps2_nonbw_offset3				0x17000
#define ps_region_error_offset			0x68C0
#define remote_play_offset			0x5fc0

/* premo_plugin */
#define pcremote_play_offset1			0xb6fc

/* premo_game_plugin */
#define pcremote_play_offset2			0xc8fc

/* psp_emulator */
#define psp_set_psp_mode_offset 		0x1C18

/* emulator_api */
#define psp_read						0x102D8
#define psp_read_header 				0x1125C
#define psp_drm_patch5					0x11080
#define psp_drm_patch6					0x110B0
#define psp_drm_patch7					0x110C8
#define psp_drm_patch8					0x110CC
#define psp_drm_patch9					0x1120C
#define psp_drm_patch11 				0x11210
#define psp_drm_patch12 				0x11220
#define psp_product_id_patch1			0x11320
#define psp_product_id_patch3			0x115F8
#define umd_mutex_offset				(0x64480+0x38C)

/* nas_plugin */

#define patch1_nas							0x2eae4
#define patch2_nas							0x2ec48
#define patch3_nas							0x1F75C // Install ALL pkgs

/* libfs */
#define aio_copy_root_offset			0xD658

#endif /* FIRMWARE */

typedef struct
{
	uint32_t offset;
	uint32_t data;
	uint8_t *condition;
} SprxPatch;

extern uint8_t condition_ps2softemu;
extern uint8_t condition_apphome;
extern uint8_t condition_psp_iso;
extern uint8_t condition_psp_dec;
extern uint8_t condition_psp_keys;
extern uint8_t condition_psp_change_emu;
extern uint8_t condition_psp_prometheus;
extern uint64_t vsh_check;
extern uint8_t safe_mode;
//extern process_t vsh_process;

/* Functions for kernel */
void do_patch();
void do_patch32();
void modules_patch_init(void);
void load_boot_plugins(void);
int prx_load_vsh_plugin(unsigned int slot, char *path, void *arg, uint32_t arg_size);
int prx_unload_vsh_plugin(unsigned int slot);

void load_boot_plugins_kernel(void);

/* Syscalls */
int sys_prx_load_vsh_plugin(unsigned int slot, char *path, void *arg, uint32_t arg_size);
int sys_prx_unload_vsh_plugin(unsigned int slot);
int sys_thread_create_ex(sys_ppu_thread_t *thread, void *entry, uint64_t arg, int prio, uint64_t stacksize, uint64_t flags, const char *threadname);

// PS3Mapi v1.2.1
int ps3mapi_unload_vsh_plugin(char* name); 
void unhook_all_modules(void);
int ps3mapi_get_vsh_plugin_info(unsigned int slot, char *name, char *filename);

#endif /* __MODULESPATCH_H__ */
