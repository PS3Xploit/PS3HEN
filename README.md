#PS3HEN 2.4.0

stage0.bin is appended to actual stackframe

MultiFW support has been added.
[Supported Firmwares]

* CEX OFW 4.82
* CEX HFW 4.84.2
* CEX HFW 4.85.1

** CFWs can be used for testing, but not recommended to use**

<br>
<br>

Changelog
-----------

#2.4.0
#Global Changes
- Stage2 size went from 100kb+ to 90kb
#Payload Changes
- Added VSH patches and disabled signature check of rif, now other tools are compatible
- PS3MAPI can now write to VSH text segment like CFW
- Disabled VSH check in rif that R and S cant be just 0
- DLC/PSX games rap support added
- Fixed hitching of PSX PAL on NTSC TV and vice versa
- Removed unnecessary hooks on cellfsopen/read/close possibly increasing stability
- Speed improvement when loading npdrm type 2 games (need original or rap activated RIF), CPU couldnt handle generating ecdsa fast enough
- Fixed issue where people sometimes got stuck downloading games from PSN
<br>
<br>
#2.3.3 BETA
#Payload Changes
- Remapping HFW XML from /dev_flash/ now instead of /dev_hdd0/
<br>
<br>
#2.3.2
#HEN Plugin Changes
- Updated To Support 4.85 HFW
#Payload Changes
- Updated To Support 4.85 HFW
<br>
<br>
#2.3.1
#HEN Plugin Changes
- Fixed the issue when Network is disabled
#Payload Changes
- Now mounts both dev_rewrite & dev_blind to save the scene
#Resource Changes
- Added duplicate icon fix by DeViL303
<br>
<br>
#2.3.0
#Global Changes
- Exploit init is faster
- Increased sleep in html, removed from bins
#HEN Plugin Changes
- HEN Updater added with version check
- Fixed crash that sometimes happened on init
- If HEN doesnt init an automated soft reboot takes place
- HEN can be enabled when failed, and ran successively, without reboot
- Removed infinite loop. One second is 10 tries, otherwise reboot
#Payload Changes
- SELF Decrypter Fix
- Fixed some blackscreen issues
- Fixed freezing if someone tries to enable hen successfully twice
- Optimizations added to how much stack is available to the syscalls (original vs before original-0xd0). Thanks aldostools
- Handler requests are passed fast, removed many branch conditions there for faster handling
- Updated extended download plugin patches, per DeViL303 suggestion
- HEN queue is drained before the patches get disabled, and also synchronized properly the check to synchronize remove and do patches
<br>
<br>
#2.2.2
#Payload Changes
- HEN_init freezing fixed by timer change
- Syscall handler bug fixed, removed 2 second hang on xmb no cleanup thread
- HashCalc bug fixed, strict hashcheck
- USB Package installation support for HEN installer
#Resource Changes
- Updated videoplayer_plugin.sprx to use proper DEX version for each firmware version
<br>
<br>
#2.2.1
#Global Changes
- The stackframe and PS3HEN bins are now merged as a single payload binary (PS3HEN.BIN)
- Added HEN refresh and version display on initialize, using embedded plugin
- Replaced dev_blind with dev_rewrite to maintain RW state at all times
- On Boot, HEN Enabler icon is selected by default
#HEN Plugin Changes
- HEN version notification on boot
- Refreshes Game and Network Category
- Enables in-game Screenshot feature
- Unloads Itself
#Payload Changes
- HEN Installer feature added and memory management changes
- Fixed Encryption
- HMAC Hash Validation
- PSP ISO Launcher Support
- PS2Classics launcher activation on the fly (thanks aldostools)
- Impoved compatibility with apps like MultiMan and others which replace syscall 6-10
- Cleanup thread added and Faster boot times for app increasing stability
- Embedded buffers and removed memory fragmentation
- Memory Management of map_path improved
- Memory Optimization (no embedded buffer for kernel plugin, only allocs when requested)
- Fail-safe added for stage0 incase stage2 not found (thanks aldostools)
- Fixed bug where if kernel plugin was more than 64kb it will crash PS3
- Fixed extern typo of 64mb to 64kb
- Added missing COBRA patches & BT/USB passthrough support added
- PS3MAPI bugfix + Stability
- Self Threading Support. Fixes the issue with a few games (SC Trilogy and etc). Currently ISO/NetISO not supported
- Improved SELF auth (Fixes games that run multiple executables. Stability improved)
#Resource Changes
- Default Theme Pack removed from main package and can now download from Themes Updater
- HEN Theme Pack by Itroublve_Hacker downloadable package updated with fixed icons
- PKG Linker is now located under Package Manager - Install Packages
- Cleaned Up Unused XML Entries. Added 1 query for external.xml on usb000 (thanks DeViL303)
- Replaced Manual link from Network column with PS3Xploit Home link
- Added new coldboot, icons, and JS/HTML overlay (thanks xps3riments)
<br>
<br>
#2.1.1
#Stackframe Binary
- 4.82 CEX is now supported! Each FW version has its own stackframe, package, and update XML.
#PS3HEN Payload
- PSNPatch is fixed, no more freezing from syscall removal
- Remap for HFW_settings is now fully protected, no more disappearing HFW tools
- Stability patches added on initial boot process
- HEN Check added to not freeze if missing files
#Resources
- PKG Linker entries added to category_game.xml
<br>
<br>
#2.1.0
@Many thanks to Habib who accidentally released v1.0.0 as anonymous ;)
#PS3HEN Payload
- Payload size is reduced by 20kb
- Advanced QA Flag. This DOES NOT allow downgrading!
- Debug Settings Enabler added
- AES calculation now uses internal library from LV2
- RAP can now be loaded / accessed from dev_hdd0/exdata
#Resources
- Added Update Themes option to PS3HEN Updater menu
- Added theme pack by "Itroublve Hacker" to PS3HEN Updater -> Update Themes
- Small text edit on "Theme selector" is now "Theme Selector" under Hybrid Firmware Tools
<br>
<br>
#2.0.2
#Stackframe Binary
- C00 unlocker activated by default
#PS3HEN Payload
- RAP activation on the fly, default path : usb000/exdata/<rap> or usb001/exdata/<rap>
- Fixed issue with official NPDRM content rif deletion and unable to boot error
- PS2 classics launcher support
- Added @DeViL303's advanced download plugin patches
- Fixed Install All Packages
- Fixed explore_plugin patches
- App restriction on RemotePlay with PC removed
- Improved games compatibilty e.g COD3
- Enabling dev_blind by default
- Multiple path on boot_plugins & boot_plugins_kernel (HDD & USB) Thanks to @aldostools
- Hybrid Firmware Tools available when HEN's activated (Enable HEN to use this feature) *
- *Only available via PS3HEN PKG installation
#Resources
- Fixed infinite spinning wheel when in-game
- Both REBUG and Stock Edition available
- Hybrid Firmware Tools available via PKG installation
- HEN updater support available under Network Category
- Official firmware updates via internet blocked
<br>
<br>
#2.0.1 BETA
- mappath is used for enabling xai_plugin so it wouldn't appear on fresh boot.
- Remote Play with PC restriction removed
- @DeViL303's extended download_plugin support added (offline pkg includes v1.01 HEN edition)
- Hybrid Firmware Tools (equivalent to CFW settings with less features)
- Both Stock and REBUG edition (theme) available.
- webMAN MOD 1.47.20 beta integrated (auto-refresh Game and Network Categories, detection of re-enabled cfw syscalls)
- Added option to re-enable cfw syscall by accessing the system update menu on XMB Settings
<br>
<br>
#2.0.0
- HOTFIX: Removed HEN Check From Offline Packages
- Fake flash is no longer used, in favor of on-the-fly patching
- Fixed blackscreen crashes
- Fixed random recovery kicks
- ISO support added
- PS3MAPI support can now read/set process mem using webman
- KW stealth extensions added
- Random lv2 panic fixed
- Added check in html for hen success
- Kernel plugins support
- Photo gui opcode support for webman
- Syscall 389/409 product mode check disabled
- Opcode 1339 added, returns HEN version (0x0200)
- Full BD/DVD ISO support (AACS decryption required for BDRip)
#Notes:
@WebmanMOD tested with 1.47.17 and 1.47.19, with fan control and PS3MAPI working
<br>
<br>
#1.0.0
- Managunz backup manager works best for jb rips(ISO not supported)!
- MULTIMAN works too but compatibility is not the same.
- PSXISO Support is there!!!!
- BD/DVD Region patches
- BDISO support(stutter with xmb, use showtime)
- BOOTPLUGINS WORK location "/dev_usb000/boot_plugins_nocobra.txt"(Use webman original one and not the mod one. also disable - content scan on boot in settings)
- Discless games work with disc icon!
- Syscall 6 added
- Syscall 7 added(address>0x8000000000352230) and disabled overwriting syscall 0->15
- Syscall 15 added
- Syscall 8 opcodes added for detection HEN and for advanced lv2 poke(read DEVELOPER SECTION)
- Whole kernel memory RWX(execute kernel payload like this at high locations or hook syscalls etc)
- PS3MAPI support for modding
- Debug PKG install
- Homebrew resigned for 3.55 and less support!
- Homebrew Root Flags enabled!
- HAN PKG install support
- PSN Connectivity
- All process executed after HEN have rwx permissions!
- HAN Enabled by default!
#CFW PATCHES:
- CFW settings
- Retail/DEBUG pkg installation
- Unlink to Delete
- Remote play with PC
- Download debug pkg on retail
- Remote play ignores SFO check
- Cinavia protection
- Videoplayer_plugin
- DVD region check (not cracking RCE)
- REBUG themed RCO & XML
- AIO copy
#NOTES:
@ if you get error 80010017 launching homebrew that simply means HEN failure, restart console and try again!(restart is important!) also try deleting cache, browsing data, cookies and the likes from browser, make the exploit page the home page
#DEVELOPERS:
-#define SYSCALL8_OPCODE_IS_HEN ⇨ 0x1337
-using this if return 0x1337 its hen
-ADVANCED POKE:syscall8(0x7003, addr, value); ⇨ this allows poking any location in lv2 memory BUT you have to restore original value before exiting to another application or exiting to xmb.USE WISELY OTHERWISE PS3 SHUTS DOWN
- BDMIRROR:Managunz FTW!(please use Cobra payload because by default its MULTIMAN)
- NOTE:mounting dev_blind will actually mount dev_flash. change files directly from dev_flash instead or hdd0/plugins/CFW/
- Kernel Mode returns 0x53434500 on success to user webkit 0x8a000000. its good to measure HEN success. right now hen is already close to or is 100%

