@echo off

set BDP_DISC_CHECK_PLUGIN=vsh/module/bdp_disccheck_plugin.sprx
set EXPLORE_PLUGIN=vsh/module/explore_plugin.sprx
set PS1_EMU=ps1emu/ps1_emu.self
set PS1_NETEMU=ps1emu/ps1_netemu.self
set GAME_EXT_PLUGIN=vsh/module/game_ext_plugin.sprx
set LIBFS_EXTERNAL=sys/external/libfs.sprx
set NAS_PLUGIN=vsh/module/nas_plugin.sprx
set BDP_BDMV=bdplayer/bdp_BDMV.self
set BDP_BDVD=bdplayer/bdp_BDVD.self
set DOWNLOAD_PLUGIN=vsh/module/download_plugin.sprx
set AUTODOWNLOAD_PLUGIN=vsh/module/autodownload_plugin.sprx
set PREMO_PLUGIN=vsh/module/premo_plugin.sprx
set PREMO_GAME_PLUGIN=vsh/module/premo_game_plugin.sprx
set PSP_EMULATOR=pspemu/psp_emulator.self
set PEMUCORELIB=pspemu/release/PEmuCoreLib.sprx
set EMULATOR_API=pspemu/release/emulator_api.sprx
set EMULATOR_DRM=pspemu/release/emulator_drm.sprx
set EMULATOR_DRM_DATA=pspemu/release/emulator_drm.sprx
set LIBSYSUTIL_SAVEDATA_PSP=sys/external/libsysutil_savedata_psp.sprx
set LIBAUDIO=sys/external/libaudio.sprx
set VSH=vsh/module/vsh.self
set BASIC_PLUGINS=vsh/module/basic_plugins.sprx
set EXPLORE_CATEGORY_GAME=vsh/module/explore_category_game.sprx

set BDP_DISC_CHECK_PLUGIN_HASH=0
set EXPLORE_PLUGIN_HASH=0
set PS1_EMU_HASH=0
set PS1_NETEMU_HASH=0
set GAME_EXT_PLUGIN_HASH=0
set LIBFS_EXTERNAL_HASH=0
set NAS_PLUGIN_HASH=0
set BDP_BDMV_HASH=0
set BDP_BDVD_HASH=0
set DOWNLOAD_PLUGIN_HASH=0
set AUTODOWNLOAD_PLUGIN_HASH=0
set PREMO_PLUGIN_HASH=0
set PREMO_GAME_PLUGIN_HASH=0
set PSP_EMULATOR_HASH=0
set PEMUCORELIB_HASH=0
set EMULATOR_API_HASH=0
set EMULATOR_DRM_HASH=0
set EMULATOR_DRM_DATA_HASH=0
set LIBSYSUTIL_SAVEDATA_PSP_HASH=0
set LIBAUDIO_HASH=0
set VSH_HASH=0
set BASIC_PLUGINS_HASH=0
set EXPLORE_CATEGORY_GAME_HASH=0

del /f /s /q hashlist.txt

cls
echo Run this script from the root of /dev/flash/
echo.
echo.
pause

hashcalc.exe %BDP_DISC_CHECK_PLUGIN% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set BDP_DISC_CHECK_PLUGIN_HASH=%%b)
echo #define BDP_DISC_CHECK_PLUGIN_HASH %BDP_DISC_CHECK_PLUGIN_HASH%>hashlist.txt

hashcalc.exe %EXPLORE_PLUGIN% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set EXPLORE_PLUGIN_HASH=%%b)
echo #define EXPLORE_PLUGIN_HASH %EXPLORE_PLUGIN_HASH%>>hashlist.txt

hashcalc.exe %PS1_EMU% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set PS1_EMU_HASH=%%b)
echo #define PS1_EMU_HASH %PS1_EMU_HASH%>>hashlist.txt

hashcalc.exe %PS1_NETEMU% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set PS1_NETEMU_HASH=%%b)
echo #define PS1_NETEMU_HASH %PS1_NETEMU_HASH%>>hashlist.txt

hashcalc.exe %GAME_EXT_PLUGIN% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set GAME_EXT_PLUGIN_HASH=%%b)
echo #define GAME_EXT_PLUGIN_HASH %GAME_EXT_PLUGIN_HASH%>>hashlist.txt

hashcalc.exe %LIBFS_EXTERNAL% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set LIBFS_EXTERNAL_HASH=%%b)
echo #define LIBFS_EXTERNAL_HASH %LIBFS_EXTERNAL_HASH%>>hashlist.txt

hashcalc.exe %NAS_PLUGIN% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set NAS_PLUGIN_HASH=%%b)
echo #define NAS_PLUGIN_HASH %NAS_PLUGIN_HASH%>>hashlist.txt

hashcalc.exe %BDP_BDMV% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set BDP_BDMV_HASH=%%b)
echo #define BDP_BDMV_HASH %BDP_BDMV_HASH%>>hashlist.txt

hashcalc.exe %BDP_BDVD% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set BDP_BDVD_HASH=%%b)
echo #define BDP_BDVD_HASH %BDP_BDVD_HASH%>>hashlist.txt

hashcalc.exe %DOWNLOAD_PLUGIN% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set DOWNLOAD_PLUGIN_HASH=%%b)
echo #define DOWNLOAD_PLUGIN_HASH %DOWNLOAD_PLUGIN_HASH%>>hashlist.txt

hashcalc.exe %AUTODOWNLOAD_PLUGIN% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set AUTODOWNLOAD_PLUGIN_HASH=%%b)
echo #define AUTODOWNLOAD_PLUGIN_HASH %AUTODOWNLOAD_PLUGIN_HASH%>>hashlist.txt

hashcalc.exe %PREMO_PLUGIN% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set PREMO_PLUGIN_HASH=%%b)
echo #define PREMO_PLUGIN_HASH %PREMO_PLUGIN_HASH%>>hashlist.txt

hashcalc.exe %PREMO_GAME_PLUGIN% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set PREMO_GAME_PLUGIN_HASH=%%b)
echo #define PREMO_GAME_PLUGIN_HASH %PREMO_GAME_PLUGIN_HASH%>>hashlist.txt

hashcalc.exe %PSP_EMULATOR% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set PSP_EMULATOR_HASH=%%b)
echo #define PSP_EMULATOR_HASH %PSP_EMULATOR_HASH%>>hashlist.txt

hashcalc.exe %PEMUCORELIB% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set PEMUCORELIB_HASH=%%b)
echo #define PEMUCORELIB_HASH %PEMUCORELIB_HASH%>>hashlist.txt

hashcalc.exe %EMULATOR_API% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set EMULATOR_API_HASH=%%b)
echo #define EMULATOR_API_HASH %EMULATOR_API_HASH%>>hashlist.txt

hashcalc.exe %EMULATOR_DRM% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set EMULATOR_DRM_HASH=%%b)
echo #define EMULATOR_DRM_HASH %EMULATOR_DRM_HASH%>>hashlist.txt

hashcalc.exe %EMULATOR_DRM_DATA% | find "Section 1: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set EMULATOR_DRM_DATA_HASH=%%b)
echo #define EMULATOR_DRM_DATA_HASH %EMULATOR_DRM_DATA_HASH%>>hashlist.txt

hashcalc.exe %LIBSYSUTIL_SAVEDATA_PSP% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set LIBSYSUTIL_SAVEDATA_PSP_HASH=%%b)
echo #define LIBSYSUTIL_SAVEDATA_PSP_HASH %LIBSYSUTIL_SAVEDATA_PSP_HASH%>>hashlist.txt

hashcalc.exe %LIBAUDIO% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set LIBAUDIO_HASH=%%b)
echo #define LIBAUDIO_HASH %LIBAUDIO_HASH%>>hashlist.txt

hashcalc.exe %VSH% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set VSH_HASH=%%b)
echo #define VSH_HASH %VSH_HASH%>>hashlist.txt

hashcalc.exe %BASIC_PLUGINS% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set BASIC_PLUGINS_HASH=%%b)
echo #define BASIC_PLUGINS_HASH %BASIC_PLUGINS_HASH%>>hashlist.txt

hashcalc.exe %EXPLORE_CATEGORY_GAME% | find "Section 0: " > tmp.txt
(for /f "tokens=2,* delims= " %%a in (tmp.txt) do set EXPLORE_CATEGORY_GAME_HASH=%%b)
echo #define EXPLORE_CATEGORY_GAME_HASH %EXPLORE_CATEGORY_GAME_HASH%>>hashlist.txt

del /f /s /q tmp.txt


:end


