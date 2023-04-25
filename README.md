# PS3HEN 3.2.2

stage0.bin is appended to actual stackframe

MultiFW support has been added.
[Supported Firmwares]

* CEX OFW 4.82
* CEX HFW 4.83.1 (thanks Joonie)
* CEX HFW 4.84.2 (thanks Joonie/habib)
* DEX HFW 4.84.2 (thanks Joonie)
* CEX HFW 4.85.1 (thanks Joonie/habib)
* CEX HFW 4.86.1 (thanks Joonie/habib)
* CEX HFW 4.87.1 (thanks Joonie)
* CEX HFW 4.88.1 (thanks zecoxao/Joonie)
* CEX HFW 4.89.1 (thanks littlebalup)
* CEX HFW 4.90.1 (thanks Joonie)

** CFWs can be used for testing, but not recommended to use**

<br>
<br>

Changelog<br>
-----------<br>

# 3.2.2<br>
# HEN Global Changes<br>
  - Added support for 4.80 and 4.81 CEX firmware<br>
  - Added support for 4.82 DEX firmware<br>
  - Automatic Updates are now disabled by default. You can use HFW Tools->Maintenance and toggle back on/off<br>
# HEN Plugin Changes<br>
  - Added support for RCO Sounds and LED Status (thanks aldostools)<br>
  - Updated install finished detection method (thanks aldostools)<br>
  - Updated and cleaned up downloadPKG_thread2 function<br>
  - Added detection of dev_build toggle for usb000 and usb001<br>
# Payload Changes<br>
  - Pad function fixed to not hang when more than one device is connected (thanks aldostools)<br>
  - Disabled VSH Attach To Debugger as default, to not conflict with some CFW (4.82/4.84 DEX)<br>
  - Multiple Updates: PSP launcher blacklisting + more (thanks aldostools)<br>
  - Added L2 to disable mappath remap on hen launch<br>
# Resource Changes<br>
  - Updated software_update_plugin.rco message to display the .me domain instead of the .com<br>
  - Added *Uninstall HEN* option to HFW Tools->Maintenance<br>
  - Added *Disable Remapping On Next Reboot* option in HFW Tools->Maintenance. This will disable the remapped files from mappath so these files can be managed and users can also use the backup/restore option from XMB (thanks Fredy66)<br>
  - Added *Toggle Hotkey Polling* option in HFW Tools->Maintenance. This will enable/disable check_combo_buttons function when HEN launches. (thanks FFF256)<br>
  - Added *Toggle app_home Support* option in HFW Tools->Maintenance. This updates xml containing the seg_gamedebug entries (thanks Fredy66)<br>
  - Added *Enable Quick Preview Option On XMB* option in HFW Tools->Developer. This swaps the explore_plugin.sprx between CEX and DEX versions<br>
<br>
<br>

# 3.2.1<br>
# HEN Global Changes<br>
  - Added support for 4.84 DEX firmware<br>
  - Updated DEX resources and stackframe (4.84 DEX/PEX Only) (thanks Joonie)<br>
  - Added support for 4.83 firmware (HFW) (thanks Joonie)<br>
  - Re-added support for 4.82 firmware (OFW)<br>
  - Updates to PS3HEN_GEN and added makefile (thanks bucanero)<br>
# Installer Changes<br>
  - Bytes will be written by webkit if HEN is being loaded from HTML, so HEN can set the installing flag<br>
  - Added downloadhenD() function to allow DEX p3t download if DEX checkbox is checked (4.82D/4.84D Only)<br>
# HEN Plugin Changes<br>
  - Added check for webMAN-MOD, and if installed will download package with WMM included, only if HEN is installing<br>
  - Updated peekq offsets and values for CEX/DEX when downloading packages<br>
# Payload Changes<br>
  - Updated detected COBRA version to 8.4<br>
  - Updated PS3MAPI_CORE_VERSION to 1.24 (thanks bucanero)<br>
  - Added check for HEN installing. Boot plugins text files will be deleted if HEN is ran from installer html<br>
  - Updated UMD/PSP blacklisting and fixed spacing (thanks aldostools)<br>
# Resource Changes<br>
  - Added VSH message to alert users of deleted boot plugins text<br>
  - Updated HEN Welcome Message for developer mode/release<br>
<br>
<br>

# 3.2.0<br>
# HEN Global Changes<br>
  - Added support for 4.90 firmware. Special thanks to lmn7 and Joonie for their work on porting offsets, HFW, and other code contributions<br>
  - Xai updated to support new HFW Tools options. Special thanks to Evilnat for his help and code contributions
# HEN Plugin Changes<br>
  - Automatic reboot after successful initial HEN installation from Network and USB<br>
  - Added Clear Browser Cache code, currently disabled. This will be moved into xai_plugin (thanks xfrcc)<br>
  - Changes to HEN plugin attempting to make it unload properly (thanks @aldostools, TheRouLetteBoi)<br>
  - Updated host domain name from ps3xploit.com to ps3xploit.me<br>
# Payload Changes<br>
  - Added mutex functionality to map_path and open_path_hook (thanks bguerville)<br>
  - Updated, fixed, and optimized map_path and open_path_hook. Also kept support for legacy homebrew that uses map_path (thanks bguerville)<br>
  - Added stat to open_path_hook for DEBUG build only (thanks DeViL303 for the idea and bguerville for the code and implementation)<br>
  - Changed compatibility for modules patching. Firmware versions 4.84-4.88 share values, but 4.89 has its own values now for hashes and sprx patches<br>
  - Added support for custom subchannel data via LSD files (thanks @aldostools)<br>
  - PSX BIOS patched with product code 0x85 for PAL games (thanks @aldostools)<br>
  - Option to force PAL or NTSC including the word in the file name (thanks @aldostools)<br>
  - Added support for .sbi files and improve the performance seeking the custom subchannels (thanks @aldostools)<br>
  - Added toggle for libaudio BT patch (thanks in1975)<br>
  - Updated act.dat restore function (thanks bucanero)<br>
  - Support for rap and RAP extension (lowercase/uppercase) (thanks aldostools)<br>
  - Added button detection on launch. Currently will look for R2 held, to disable boot plugins.<br>
# Resource Changes<br>
  - HEN Enable and Package Manager have been separated from category_game.xml<br>
  - Package Manager hidden on boot and shows full on HEN launch (thanks LuanTeles, DeViL303)<br>
  - The HEN Enable egg menu item will be shown on boot and will be hidden after the XMB is refreshed. This will be updated later to refresh automatically. (thanks LuanTeles, DeViL303)<br>
  - Added Developer options under HFW Tools -> Developer<br>
  - Added the ability to switch HEN from Release and Debug modes via HFW Tools -> Developer (USB will be added next release)<br>
  - Added the option to remove hen_enable.png to allow install from browser via HFW Tools -> Developer<br>
  - Only supporting Stock and Rebug themes for now until custom colors can be fixed<br>
<br>
<br>

# 3.1.1<br>
# Payload Changes<br>
  - HOTFIX: Updated HEN_REV to 0x0311<br>
# Resource Changes<br>
  - Replaced explore_plugin.sprx with OFW version, 4.89 only (Replacement is needed to make sure the old one is overwritten on dirty 4.89 installs)<br>
  - Removed unused RCO files that match OFW (explore_category_psn.rco, explore_category_video.rco, wboard_plugin.rco, xmb_ingame.rco, xmb_plugin_normal.rco)<br>
  - Temporarily fixed display issues with Trophies, Package Manager (it does currently work, but stutters on sub-menu, and has broken icon until HEN enabled), and other XMB items. This fix may have improvements, moving forward. Thanks to everyone who reported issues (aleks1992, Colek, Cyberdev, Farzin, H3N7R1K, LuanTeles, PopCornLover, ReMiX2000, SolidGoldPug, Xjordy13x, Yoti) and to anyone we missed!<br>
<br>
<br>

# 3.1.0<br>
# HEN Plugin Changes<br>
  - Updated To Support 4.89 HFW<br>
  - Added act.dat Backup/Restore feature (thanks bucanero)<br>
# Payload Changes<br>
  - Updated To Support 4.89 HFW<br>
  - COBRA PS3MAPI Changes: Updated To Identify 4.89 Firmware<br>
# Resource Changes<br>
  - Added Dump PSID option to HFW Tools<br>
  - Added Toggle Automatic Update option to HFW Tools<br>
  - Updated text on Theme Selector in HFW Tools<br>
  - Added new icons for updated HFW Tools options (thanks xps3riments)<br>
  - Updated RCO files to fix display issues with 4.89 only (thanks sandungas)<br>
    - explore_plugin_full.rco: 44292838814555627FC8F9F9CB632CD8<br>
    - software_update_plugin.rco: D1FAF469796E6894AA1CECCFAC7895EF<br>
  - Updated layout_grid_table files for 480p and 272p to fix display issues in Remote Play with 4.89 only (thanks sandungas)<br>
<br>
<br>

# 3.0.3<br>
# HEN Plugin Changes<br>
  - Updated To Support 4.88 HFW<br>
# Payload Changes<br>
  - Updated To Support 4.88 HFW<br>
  - COBRA PS3MAPI Changes: Updated To Identify 4.88 Firmware<br>
<br>
<br>

# 3.0.2<br>
# HEN Plugin Changes<br>
  - Updated To Support 4.87 HFW<br>
# Payload Changes<br>
  - Updated To Support 4.87 HFW<br>
  - COBRA PS3MAPI Changes: Updated To Identify 4.87 Firmware<br>
# Resource Changes<br>
  - Updated HEN Loader category.xml to use new icon (thanks xps3riments)<br>
<br>
<br>

# 3.0.1<br>
# Global Changes<br>
  - Updated To Support 4.86 HFW<br>
# HEN Plugin Changes<br>
  - Updated To Support 4.86 HFW<br>
# Payload Changes<br>
  - Updated To Support 4.86 HFW<br>
  - COBRA PS3MAPI Changes (thanks to @TheRouLetteBoi)<br>
  - UAdded a better set process memory by using the function used to actually write to process, this will allow user to write to memory where writing permissions are disabled<br>
  - Added ps3mapi_process_page_allocate this function will allocate memory into the eboot process allowing your to write/read/execute code into start_address parameter<br>
  - Added ps3mapi_get_process_module_info which will get the name, module path, module segments, module start and module stop address all in one function<br>
  -     Added ps3mapi_create_process_thread to create thread into the process, This is useful if you want to load a small function into the process without needed make and load a sprx module<br>
<br>
<br>

# 3.0.0<br>
# Global Changes<br>
  - No Longer Officially Supporting 4.82 Firmware<br>
# HEN Plugin Changes<br>
  - Fixed freezing if DVD or CD is already inserted into PS3 when HEN is enabled<br>
  - Showing error message if reply length is too short from server<br>
# Payload Changes<br>
  - Fixed Freezing Problems on All Models<br>
  - Fixed Issues With Incompatible Models<br>
  - Improved Sanity Checks<br>
<br>
<br>

# 2.4.0<br>
# Global Changes<br>
- Stage2 size went from 100kb+ to 90kb<br>
# Payload Changes<br>
- Added VSH patches and disabled signature check of rif, now other tools are compatible<br>
- PS3MAPI can now write to VSH text segment like CFW<br>
- Disabled VSH check in rif that R and S cant be just 0<br>
- DLC/PSX games rap support added<br>
- Fixed hitching of PSX PAL on NTSC TV and vice versa<br>
- Removed unnecessary hooks on cellfsopen/read/close possibly increasing stability<br>
- Speed improvement when loading npdrm type 2 games (need original or rap activated RIF), CPU couldnt handle generating ecdsa fast enough<br>
- Fixed issue where people sometimes got stuck downloading games from PSN<br>
<br>
<br>

# 2.3.3 BETA<br>
# Payload Changes<br>
- Remapping HFW XML from /dev_flash/ now instead of /dev_hdd0/<br>
<br>
<br>

# 2.3.2<br>
# HEN Plugin Changes<br>
- Updated To Support 4.85 HFW<br>
# Payload Changes<br>
- Updated To Support 4.85 HFW<br>
<br>
<br>

# 2.3.1<br>
# HEN Plugin Changes<br>
- Fixed the issue when Network is disabled<br>
# Payload Changes<br>
- Now mounts both dev_rewrite & dev_blind to save the scene<br>
# Resource Changes<br>
- Added duplicate icon fix by DeViL303<br>
<br>
<br>

# 2.3.0<br>
# Global Changes<br>
- Exploit init is faster<br>
- Increased sleep in html, removed from bins<br>
# HEN Plugin Changes<br>
- HEN Updater added with version check<br>
- Fixed crash that sometimes happened on init<br>
- If HEN doesnt init an automated soft reboot takes place<br>
- HEN can be enabled when failed, and ran successively, without reboot<br>
- Removed infinite loop. One second is 10 tries, otherwise reboot<br>
# Payload Changes<br>
- SELF Decrypter Fix<br>
- Fixed some blackscreen issues<br>
- Fixed freezing if someone tries to enable hen successfully twice<br>
- Optimizations added to how much stack is available to the syscalls (original vs before original-0xd0). Thanks aldostools<br>
- Handler requests are passed fast, removed many branch conditions there for faster handling<br>
- Updated extended download plugin patches, per DeViL303 suggestion<br>
- HEN queue is drained before the patches get disabled, and also synchronized properly the check to synchronize remove and do patches<br>
<br>
<br>

# 2.2.2<br>
# Payload Changes<br>
- HEN_init freezing fixed by timer change<br>
- Syscall handler bug fixed, removed 2 second hang on xmb no cleanup thread<br>
- HashCalc bug fixed, strict hashcheck<br>
- USB Package installation support for HEN installer<br>
# Resource Changes<br>
- Updated videoplayer_plugin.sprx to use proper DEX version for each firmware version<br>
<br>
<br>

# 2.2.1<br>
# Global Changes<br>
- The stackframe and PS3HEN bins are now merged as a single payload binary (PS3HEN.BIN)<br>
- Added HEN refresh and version display on initialize, using embedded plugin<br>
- Replaced dev_blind with dev_rewrite to maintain RW state at all times<br>
- On Boot, HEN Enabler icon is selected by default<br>
# HEN Plugin Changes<br>
- HEN version notification on boot<br>
- Refreshes Game and Network Category<br>
- Enables in-game Screenshot feature<br>
- Unloads Itself<br>
# Payload Changes<br>
- HEN Installer feature added and memory management changes<br>
- Fixed Encryption<br>
- HMAC Hash Validation<br>
- PSP ISO Launcher Support<br>
- PS2Classics launcher activation on the fly (thanks aldostools)<br>
- Impoved compatibility with apps like MultiMan and others which replace syscall 6-10<br>
- Cleanup thread added and Faster boot times for app increasing stability<br>
- Embedded buffers and removed memory fragmentation<br>
- Memory Management of map_path improved<br>
- Memory Optimization (no embedded buffer for kernel plugin, only allocs when requested)<br>
- Fail-safe added for stage0 incase stage2 not found (thanks aldostools)<br>
- Fixed bug where if kernel plugin was more than 64kb it will crash PS3<br>
- Fixed extern typo of 64mb to 64kb<br>
- Added missing COBRA patches & BT/USB passthrough support added<br>
- PS3MAPI bugfix + Stability<br>
- Self Threading Support. Fixes the issue with a few games (SC Trilogy and etc). Currently ISO/NetISO not supported<br>
- Improved SELF auth (Fixes games that run multiple executables. Stability improved)<br>
# Resource Changes<br>
- Default Theme Pack removed from main package and can now download from Themes Updater<br>
- HEN Theme Pack by Itroublve_Hacker downloadable package updated with fixed icons<br>
- PKG Linker is now located under Package Manager - Install Packages<br>
- Cleaned Up Unused XML Entries. Added 1 query for external.xml on usb000 (thanks DeViL303)<br>
- Replaced Manual link from Network column with PS3Xploit Home link<br>
- Added new coldboot, icons, and JS/HTML overlay (thanks xps3riments)<br>
<br>
<br>

# 2.1.1<br>
# Stackframe Binary<br>
- 4.82 CEX is now supported! Each FW version has its own stackframe, package, and update XML.<br>
# PS3HEN Payload<br>
- PSNPatch is fixed, no more freezing from syscall removal<br>
- Remap for HFW_settings is now fully protected, no more disappearing HFW tools<br>
- Stability patches added on initial boot process<br>
- HEN Check added to not freeze if missing files<br>
# Resources<br>
- PKG Linker entries added to category_game.xml<br>
<br>
<br>

# 2.1.0<br>
@Many thanks to Habib who accidentally released v1.0.0 as anonymous ;)<br>
# PS3HEN Payload<br>
- Payload size is reduced by 20kb<br>
- Advanced QA Flag. This DOES NOT allow downgrading!<br>
- Debug Settings Enabler added<br>
- AES calculation now uses internal library from LV2<br>
- RAP can now be loaded / accessed from dev_hdd0/exdata<br>
# Resources<br>
- Added Update Themes option to PS3HEN Updater menu<br>
- Added theme pack by "Itroublve Hacker" to PS3HEN Updater -> Update Themes<br>
- Small text edit on "Theme selector" is now "Theme Selector" under Hybrid Firmware Tools<br>
<br>
<br>

# 2.0.2<br>
# Stackframe Binary<br>
- C00 unlocker activated by default<br>
# PS3HEN Payload<br>
- RAP activation on the fly, default path : usb000/exdata/<rap> or usb001/exdata/<rap><br>
- Fixed issue with official NPDRM content rif deletion and unable to boot error<br>
- PS2 classics launcher support<br>
- Added @DeViL303's advanced download plugin patches<br>
- Fixed Install All Packages<br>
- Fixed explore_plugin patches<br>
- App restriction on RemotePlay with PC removed<br>
- Improved games compatibilty e.g COD3<br>
- Enabling dev_blind by default<br>
- Multiple path on boot_plugins & boot_plugins_kernel (HDD & USB) Thanks to @aldostools<br>
- Hybrid Firmware Tools available when HEN's activated (Enable HEN to use this feature) *<br>
- *Only available via PS3HEN PKG installation<br>
# Resources<br>
- Fixed infinite spinning wheel when in-game<br>
- Both REBUG and Stock Edition available<br>
- Hybrid Firmware Tools available via PKG installation<br>
- HEN updater support available under Network Category<br>
- Official firmware updates via internet blocked<br>
<br>
<br>

# 2.0.1 BETA<br>
- mappath is used for enabling xai_plugin so it wouldn't appear on fresh boot.<br>
- Remote Play with PC restriction removed<br>
- @DeViL303's extended download_plugin support added (offline pkg includes v1.01 HEN edition)<br>
- Hybrid Firmware Tools (equivalent to CFW settings with less features)<br>
- Both Stock and REBUG edition (theme) available.<br>
- webMAN MOD 1.47.20 beta integrated (auto-refresh Game and Network Categories, detection of re-enabled cfw syscalls)<br>
- Added option to re-enable cfw syscall by accessing the system update menu on XMB Settings<br>
<br>
<br>

# 2.0.0<br>
- HOTFIX: Removed HEN Check From Offline Packages<br>
- Fake flash is no longer used, in favor of on-the-fly patching<br>
- Fixed blackscreen crashes<br>
- Fixed random recovery kicks<br>
- ISO support added<br>
- PS3MAPI support can now read/set process mem using webman<br>
- KW stealth extensions added<br>
- Random lv2 panic fixed<br>
- Added check in html for hen success<br>
- Kernel plugins support<br>
- Photo gui opcode support for webman<br>
- Syscall 389/409 product mode check disabled<br>
- Opcode 1339 added, returns HEN version (0x0200)<br>
- Full BD/DVD ISO support (AACS decryption required for BDRip)<br>
# Notes:<br>
@WebmanMOD tested with 1.47.17 and 1.47.19, with fan control and PS3MAPI working<br>
<br>
<br>

# 1.0.0<br>
- Managunz backup manager works best for jb rips(ISO not supported)!<br>
- MULTIMAN works too but compatibility is not the same.<br>
- PSXISO Support is there!!!!<br>
- BD/DVD Region patches<br>
- BDISO support(stutter with xmb, use showtime)<br>
- BOOTPLUGINS WORK location "/dev_usb000/boot_plugins_nocobra.txt"(Use webman original one and not the mod one. also disable - content scan on boot in settings)<br>
- Discless games work with disc icon!<br>
- Syscall 6 added<br>
- Syscall 7 added(address>0x8000000000352230) and disabled overwriting syscall 0->15<br>
- Syscall 15 added<br>
- Syscall 8 opcodes added for detection HEN and for advanced lv2 poke(read DEVELOPER SECTION)<br>
- Whole kernel memory RWX(execute kernel payload like this at high locations or hook syscalls etc)<br>
- PS3MAPI support for modding<br>
- Debug PKG install<br>
- Homebrew resigned for 3.55 and less support!<br>
- Homebrew Root Flags enabled!<br>
- HAN PKG install support<br>
- PSN Connectivity<br>
- All process executed after HEN have rwx permissions!<br>
- HAN Enabled by default!<br>
# CFW PATCHES:<br>
- CFW settings<br>
- Retail/DEBUG pkg installation<br>
- Unlink to Delete<br>
- Remote play with PC<br>
- Download debug pkg on retail<br>
- Remote play ignores SFO check<br>
- Cinavia protection<br>
- Videoplayer_plugin<br>
- DVD region check (not cracking RCE)<br>
- REBUG themed RCO & XML<br>
- AIO copy<br>
# NOTES:<br>
@ if you get error 80010017 launching homebrew that simply means HEN failure, restart console and try again!(restart is important!) also try deleting cache, browsing data, cookies and the likes from browser, make the exploit page the home page<br>
# DEVELOPERS:<br>
-#define SYSCALL8_OPCODE_IS_HEN ⇨ 0x1337<br>
-using this if return 0x1337 its hen<br>
-ADVANCED POKE:syscall8(0x7003, addr, value); ⇨ this allows poking any location in lv2 memory BUT you have to restore original value before exiting to another application or exiting to xmb.USE WISELY OTHERWISE PS3 SHUTS DOWN<br>
- BDMIRROR:Managunz FTW!(please use Cobra payload because by default its MULTIMAN)<br>
- NOTE:mounting dev_blind will actually mount dev_flash. change files directly from dev_flash instead or hdd0/plugins/CFW/<br>
- Kernel Mode returns 0x53434500 on success to user webkit 0x8a000000. its good to measure HEN success. right now hen is already close to or is 100%<br>

