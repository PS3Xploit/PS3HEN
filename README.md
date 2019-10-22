#PS3HEN 3.0.0

stage0.bin is appended to actual stackframe

MultiFW support has been added.
[Supported Firmwares]

* CEX HFW 4.84.2
* CEX HFW 4.85.1

** CFWs can be used for testing, but not recommended to use**

<br>
<br>

Changelog<br>
-----------<br>

#3.0.0<br>
#Global Changes<br>
  - No Longer Officially Supporting 4.82 Firmware<br>
#HEN Plugin Changes<br>
  - Fixed freezing if DVD or CD is already inserted into PS3 when HEN is enabled<br>
  - Showing error message if reply length is too short from server<br>
#Payload Changes<br>
  - Fixed Freezing Problems on All Models<br>
  - Fixed Issues With Incompatible Models<br>
  - Improved Sanity Checks<br>
<br>
<br>

#2.4.0<br>
#Global Changes<br>
- Stage2 size went from 100kb+ to 90kb<br>
#Payload Changes<br>
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
#2.3.3 BETA<br>
#Payload Changes<br>
- Remapping HFW XML from /dev_flash/ now instead of /dev_hdd0/<br>
<br>
<br>
#2.3.2<br>
#HEN Plugin Changes<br>
- Updated To Support 4.85 HFW<br>
#Payload Changes<br>
- Updated To Support 4.85 HFW<br>
<br>
<br>
#2.3.1<br>
#HEN Plugin Changes<br>
- Fixed the issue when Network is disabled<br>
#Payload Changes<br>
- Now mounts both dev_rewrite & dev_blind to save the scene<br>
#Resource Changes<br>
- Added duplicate icon fix by DeViL303<br>
<br>
<br>
#2.3.0<br>
#Global Changes<br>
- Exploit init is faster<br>
- Increased sleep in html, removed from bins<br>
#HEN Plugin Changes<br>
- HEN Updater added with version check<br>
- Fixed crash that sometimes happened on init<br>
- If HEN doesnt init an automated soft reboot takes place<br>
- HEN can be enabled when failed, and ran successively, without reboot<br>
- Removed infinite loop. One second is 10 tries, otherwise reboot<br>
#Payload Changes<br>
- SELF Decrypter Fix<br>
- Fixed some blackscreen issues<br>
- Fixed freezing if someone tries to enable hen successfully twice<br>
- Optimizations added to how much stack is available to the syscalls (original vs before original-0xd0). Thanks aldostools<br>
- Handler requests are passed fast, removed many branch conditions there for faster handling<br>
- Updated extended download plugin patches, per DeViL303 suggestion<br>
- HEN queue is drained before the patches get disabled, and also synchronized properly the check to synchronize remove and do patches<br>
<br>
<br>
#2.2.2<br>
#Payload Changes<br>
- HEN_init freezing fixed by timer change<br>
- Syscall handler bug fixed, removed 2 second hang on xmb no cleanup thread<br>
- HashCalc bug fixed, strict hashcheck<br>
- USB Package installation support for HEN installer<br>
#Resource Changes<br>
- Updated videoplayer_plugin.sprx to use proper DEX version for each firmware version<br>
<br>
<br>
#2.2.1<br>
#Global Changes<br>
- The stackframe and PS3HEN bins are now merged as a single payload binary (PS3HEN.BIN)<br>
- Added HEN refresh and version display on initialize, using embedded plugin<br>
- Replaced dev_blind with dev_rewrite to maintain RW state at all times<br>
- On Boot, HEN Enabler icon is selected by default<br>
#HEN Plugin Changes<br>
- HEN version notification on boot<br>
- Refreshes Game and Network Category<br>
- Enables in-game Screenshot feature<br>
- Unloads Itself<br>
#Payload Changes<br>
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
#Resource Changes<br>
- Default Theme Pack removed from main package and can now download from Themes Updater<br>
- HEN Theme Pack by Itroublve_Hacker downloadable package updated with fixed icons<br>
- PKG Linker is now located under Package Manager - Install Packages<br>
- Cleaned Up Unused XML Entries. Added 1 query for external.xml on usb000 (thanks DeViL303)<br>
- Replaced Manual link from Network column with PS3Xploit Home link<br>
- Added new coldboot, icons, and JS/HTML overlay (thanks xps3riments)<br>
<br>
<br>
#2.1.1<br>
#Stackframe Binary<br>
- 4.82 CEX is now supported! Each FW version has its own stackframe, package, and update XML.<br>
#PS3HEN Payload<br>
- PSNPatch is fixed, no more freezing from syscall removal<br>
- Remap for HFW_settings is now fully protected, no more disappearing HFW tools<br>
- Stability patches added on initial boot process<br>
- HEN Check added to not freeze if missing files<br>
#Resources<br>
- PKG Linker entries added to category_game.xml<br>
<br>
<br>
#2.1.0<br>
@Many thanks to Habib who accidentally released v1.0.0 as anonymous ;)<br>
#PS3HEN Payload<br>
- Payload size is reduced by 20kb<br>
- Advanced QA Flag. This DOES NOT allow downgrading!<br>
- Debug Settings Enabler added<br>
- AES calculation now uses internal library from LV2<br>
- RAP can now be loaded / accessed from dev_hdd0/exdata<br>
#Resources<br>
- Added Update Themes option to PS3HEN Updater menu<br>
- Added theme pack by "Itroublve Hacker" to PS3HEN Updater -> Update Themes<br>
- Small text edit on "Theme selector" is now "Theme Selector" under Hybrid Firmware Tools<br>
<br>
<br>
#2.0.2<br>
#Stackframe Binary<br>
- C00 unlocker activated by default<br>
#PS3HEN Payload<br>
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
#Resources<br>
- Fixed infinite spinning wheel when in-game<br>
- Both REBUG and Stock Edition available<br>
- Hybrid Firmware Tools available via PKG installation<br>
- HEN updater support available under Network Category<br>
- Official firmware updates via internet blocked<br>
<br>
<br>
#2.0.1 BETA<br>
- mappath is used for enabling xai_plugin so it wouldn't appear on fresh boot.<br>
- Remote Play with PC restriction removed<br>
- @DeViL303's extended download_plugin support added (offline pkg includes v1.01 HEN edition)<br>
- Hybrid Firmware Tools (equivalent to CFW settings with less features)<br>
- Both Stock and REBUG edition (theme) available.<br>
- webMAN MOD 1.47.20 beta integrated (auto-refresh Game and Network Categories, detection of re-enabled cfw syscalls)<br>
- Added option to re-enable cfw syscall by accessing the system update menu on XMB Settings<br>
<br>
<br>
#2.0.0<br>
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
#Notes:<br>
@WebmanMOD tested with 1.47.17 and 1.47.19, with fan control and PS3MAPI working<br>
<br>
<br>
#1.0.0<br>
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
#CFW PATCHES:<br>
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
#NOTES:<br>
@ if you get error 80010017 launching homebrew that simply means HEN failure, restart console and try again!(restart is important!) also try deleting cache, browsing data, cookies and the likes from browser, make the exploit page the home page<br>
#DEVELOPERS:<br>
-#define SYSCALL8_OPCODE_IS_HEN ⇨ 0x1337<br>
-using this if return 0x1337 its hen<br>
-ADVANCED POKE:syscall8(0x7003, addr, value); ⇨ this allows poking any location in lv2 memory BUT you have to restore original value before exiting to another application or exiting to xmb.USE WISELY OTHERWISE PS3 SHUTS DOWN<br>
- BDMIRROR:Managunz FTW!(please use Cobra payload because by default its MULTIMAN)<br>
- NOTE:mounting dev_blind will actually mount dev_flash. change files directly from dev_flash instead or hdd0/plugins/CFW/<br>
- Kernel Mode returns 0x53434500 on success to user webkit 0x8a000000. its good to measure HEN success. right now hen is already close to or is 100%<br>

