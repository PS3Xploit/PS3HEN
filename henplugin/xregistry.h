// Mysis xRegistry.h v0.1
// 3141card (C style version) source: http://www.psdevwiki.com/ps3/Talk:VSH
#ifndef __XREGISTRY_H__
#define __XREGISTRY_H__

#define xnet      xsetting_F48C0548
#define xusers    xsetting_CC56EB2D
#define xsettings xsetting_0AF1F161
#define xregistry xsetting_D0261D72

typedef struct xsetting_0AF1F161_class_t  // 28
{
	int32_t (*GetProductCode)(void);
	int32_t (*GetProductSubCode)(void);         // Model Type, Mobo Id
	int32_t (*GetUnk1)(void *);                 // uint8_t [0x1C] { hdmi, ieee802.11, msslot, sdslot, cfslot }
	int32_t (*SetUnk2)(void *);
	int32_t (*GetEnterButtonAssign)(int32_t *);
	int32_t (*SetEnterButtonAssign)(int32_t);
	int32_t (*GetLicenseArea)(int32_t *);
	int32_t (*SetSystemInitialize)(int32_t);
	int32_t (*GetSystemInitialize)(int32_t *);
	int32_t (*SetSystemLanguage)(int32_t);
	int32_t (*GetSystemLanguage)(int32_t *);
	int32_t (*SetSystemNickname)(char *);
	int32_t (*GetSystemNickname)(char *, int32_t *); // nick, length
	int32_t (*SetSystemCharacterCodeOem)(int32_t);
	int32_t (*GetSystemCharacterCodeOem)(int32_t *);
	int32_t (*SetSystemCharacterCodeOemValue)(int32_t);
	int32_t (*GetSystemCharacterCodeOemValue)(int32_t *);
	int32_t (*SetsystemCharacterCodeAnsi)(int32_t);
	int32_t (*GetSystemCharacterCodeAnsi)(int32_t *);
	int32_t (*ResetNicknameDvdRegionDlnaFlag)(void);
	int32_t (*SetSystemNotificationEnabled)(int32_t);
	int32_t (*GetSystemNotificationEnabled)(int32_t *);
	int32_t (*SetSystemDiscBootFirstEnabled)(int32_t);
	int32_t (*GetSystemDiscBootFirstEnabled)(int32_t *);
	int32_t (*SetSystemSoundEffectEnabled)(int32_t);
	int32_t (*GetSystemSoundEffectEnabled)(int32_t *);
	int32_t (*unk_new)(void *, void *);
	int32_t (*unk_delete)(void *, void *);
} xsetting_0AF1F161_class;

typedef struct xsetting_154430FC_class_t  // 48 sound
{
 //int32_t (*sub_4617B0) ?
 //int32_t (*sub_461774) ?
 //int32_t (*sub_461718) ?
 //int32_t (*sub_4616DC) ?
 //int32_t (*sub_4617DC) ?
 //int32_t (*sub_461674) ?
 //int32_t (*sub_461638) ?
 //int32_t (*sub_4615FC) ?
 //int32_t (*sub_4615B0) ?
 //int32_t (*sub_461564) ?
 //int32_t (*sub_461528) ?
 //int32_t (*sub_4614EC) ?
 //int32_t (*sub_4614B0) ?
 //int32_t (*sub_461474) ?
 //int32_t (*sub_461428) ?
 //int32_t (*sub_4613DC) ?
 //int32_t (*sub_461390) ?
 //int32_t (*sub_461354) ?
 //int32_t (*sub_461308) ?
 //int32_t (*sub_4612BC) ?
 //int32_t (*sub_461270) ?
 //int32_t (*sub_461224) ?
 //int32_t (*sub_4611E8) ?
 //int32_t (*sub_4611AC) ?
 //int32_t (*sub_461160) ?
 //int32_t (*sub_461134) ?
 //int32_t (*sub_4610F8) ?
 //int32_t (*sub_4610BC) ?
 //int32_t (*sub_461080) ?
 //int32_t (*sub_461034) ?
 //int32_t (*sub_460FF8) ?
 //int32_t (*sub_460FBC) ?
 //int32_t (*sub_460F70) ?
 //int32_t (*sub_460F10) ?
 //int32_t (*sub_460ED4) ?
 //int32_t (*sub_460E98) ?
 //int32_t (*sub_460E5C) ?
 //int32_t (*sub_460E20) ?
 //int32_t (*sub_460DF4) ?
 //int32_t (*sub_460DC8) ?
 //int32_t (*sub_460D8C) ?
 //int32_t (*sub_460D50) ?
 //int32_t (*sub_460D14) ?
 //int32_t (*sub_460CD8) ?
 //int32_t (*sub_460CAC) ?
 //int32_t (*sub_460C70) ?
 //int32_t (*sub_460C34) ?
 //int32_t (*sub_460BE8) ?
} xsetting_154430FC_class;

typedef struct xsetting_16A8A805_class_t  // 53
{
	int32_t (*SetBdMenuLanguage)(int32_t);
	int32_t (*GetBdMenuLanguage)(int32_t *);
	int32_t (*SetBdSoundLanguage)(int32_t);
	int32_t (*GetBdSoundLanguage)(int32_t *);
	int32_t (*SetBdCaptionLanguage)(int32_t);
	int32_t (*GetBdCaptionLanguage)(int32_t *);
	int32_t (*Set_unk6)(int32_t);
	int32_t (*Get_unk7)(int32_t *);
	int32_t (*SetDvdMenuLanguage)(int32_t);
	int32_t (*GetDvdMenuLanguage)(int32_t *);
	int32_t (*SetDvdSoundLanguage)(int32_t);
	int32_t (*GetDvdSoundLanguage)(int32_t *);
	int32_t (*SetDvdCaptionLanguage)(int32_t);
	int32_t (*GetDvdCaptionLanguage)(int32_t *);
	int32_t (*Set_unk14)(int32_t);
	int32_t (*Get_unk15)(int32_t *);
	int32_t (*SetFnr)(int32_t);
	int32_t (*GetFnr)(int32_t *);
	int32_t (*SetBnr)(int32_t);
	int32_t (*GetBnr)(int32_t *);
	int32_t (*SetMnr)(int32_t);
	int32_t (*GetMnr)(int32_t *);
	int32_t (*SetFnrForDvdRom)(int32_t);
	int32_t (*GetFnrForDvdRom)(int32_t *);
	int32_t (*SetBnrForDvdRom)(int32_t);
	int32_t (*GetBnrForDvdRom)(int32_t *);
	int32_t (*SetMnrForDvdRom)(int32_t);
	int32_t (*GetMnrForDvdRom)(int32_t *);
	int32_t (*SetCinemaConversion)(int32_t);
	int32_t (*GetCinemaConversion)(int32_t *);
	int32_t (*SetVolume)(int32_t);
	int32_t (*GetVolume)(int32_t *);
	int32_t (*SetDvdWideScreen)(int32_t);
	int32_t (*GetDvdWideScreen)(int32_t *);
	int32_t (*SetDvdRegionCode)(int32_t);
	int32_t (*GetDvdRegionCode)(int32_t *);
	int32_t (*SetBdRegionCode)(int32_t);
	int32_t (*GetBdRegionCode)(int32_t *);
	int32_t (*SetDvdTvSystem)(int32_t);
	int32_t (*GetDvdTvSystem)(int32_t *);
	int32_t (*SetDvdUpConvert)(int32_t);
	int32_t (*GetDvdUpConvert)(int32_t *);
	int32_t (*SetDrc)(int32_t);
	int32_t (*GetDrc)(int32_t *);
	int32_t (*GetColorButtonAssign)(int32_t *);
	int32_t (*SetNetworkConnect)(int32_t);
	int32_t (*GetNetworkConnect)(int32_t *);
	int32_t (*GetSoundFormatHdmi)(int32_t *);
	int32_t (*SetSoundFormatHdmi)(int32_t);
	int32_t (*GetVideoFormatHdmiColorSpace)(int32_t *);
	int32_t (*SetVideoFormatHdmiColorSpace)(int32_t);
	int32_t (*GetVideoFormatHdmi1080_24p)(int32_t *);
	int32_t (*SetVideoFormatHdmi1080_24p)(int32_t);
} xsetting_16A8A805_class;

typedef struct xsetting_1D6D60D4_class_t  // 4, printer
{
	//int32_t (*sub_4707D4) ?
	//int32_t (*sub_470688) ?
	//int32_t (*sub_470580) ?
	//int32_t (*sub_470484) ?
} xsetting_1D6D60D4_class;

typedef struct xsetting_43F98936_class_t  // 15
{
	int32_t (*sub_448B10)(void);   // ret 0x8002B261
	// int32_t (*sub_448AC8) ?
	// int32_t (*sub_4489C8) ?
	// int32_t (*sub_448894) ?
	// int32_t (*sub_4487B4) ?
	// int32_t (*sub_4486C4) ?
	// int32_t (*sub_448414) ?
	// int32_t (*sub_448328) ?
	// int32_t (*sub_448244) ?
	// int32_t (*sub_447E70) ?
	// int32_t (*sub_447964) ?
	// int32_t (*sub_447424) ?
	// int32_t (*sub_447220) ?
	// int32_t (*sub_447174) ?
	// int32_t (*sub_4470D4) ?
} xsetting_43F98936_class;

typedef struct xsetting_4712F276_class_t  // 79
{
 //int32_t (*sub_453EE4) ?
 //int32_t (*sub_453E74) ?
 //int32_t (*sub_453E0C) ?
 //int32_t (*sub_456778) ?
 //int32_t (*sub_4550E4) ?
 //int32_t (*sub_457920) ?
 //int32_t (*sub_4547A4) ?
 //int32_t (*sub_456B00) ?
 //int32_t (*sub_453D94) ?
 //int32_t (*sub_453D38) ?
 //int32_t (*sub_453CDC) ?
 //int32_t (*sub_453C54) ?
 //int32_t (*sub_453BCC) ?
 //int32_t (*sub_453B58) ?
 //int32_t (*sub_45206C) ?
 //int32_t (*sub_453AE4) ?
 //int32_t (*sub_453A5C) ?
 //int32_t (*sub_4539E8) ?
 //int32_t (*sub_453944) ?
 //int32_t (*sub_453860) ?
 //int32_t (*sub_45378C) ?
 //int32_t (*sub_453660) ?
 //int32_t (*sub_453608) ?
 //int32_t (*sub_4566F0) ?
 //int32_t (*sub_456460) ?
 //int32_t (*sub_4535A8) ?
 //int32_t (*sub_453544) ?
 //int32_t (*sub_45441C) ?
 //int32_t (*sub_457270) ?
 //int32_t (*sub_455F70) ?
 //int32_t (*sub_45756C) ?
 //int32_t (*sub_454B54) ?
 //int32_t (*sub_453494) ?
 //int32_t (*sub_45341C) ?
 //int32_t (*sub_4533D0) ?
 //int32_t (*sub_455A8C) ?
 //int32_t (*sub_4555D0) ?
 //int32_t (*sub_453320) ?
 //int32_t (*sub_453F54) ?
 //int32_t (*sub_456F8C) ?
 //int32_t (*sub_454F24) ?
 //int32_t (*sub_457D48) ?
 //int32_t (*sub_454D48) ?
 //int32_t (*sub_457F20) ?
 //int32_t (*sub_456D1C) ?
 //int32_t (*sub_452E5C) ?
 //int32_t (*sub_4528D4) ?
 //int32_t (*sub_452DAC) ?
 //int32_t (*sub_452838) ?
 //int32_t (*sub_452D20) ?
 //int32_t (*sub_452C10) ?
 //int32_t (*sub_452B80) ?
 //int32_t (*sub_4527BC) ?
 //int32_t (*sub_4530C8) ?
 //int32_t (*sub_452A40) ?
 //int32_t (*sub_4526B0) ?
 //int32_t (*sub_4529B0) ?
 //int32_t (*sub_452634) ?
 //int32_t (*sub_452AD0) ?
 //int32_t (*sub_45272C) ?
 //int32_t (*sub_45304C) ?
 //int32_t (*sub_452F60) ?
 //int32_t (*sub_452FD4) ?
 //int32_t (*sub_452F28) ?
 //int32_t (*sub_452EF0) ?
 //int32_t (*sub_452C9C) ?
 //int32_t (*sub_452934) ?
 //int32_t (*sub_4525D0) ?
 //int32_t (*sub_45663C) ?
 //int32_t (*sub_452448) ?
 //int32_t (*sub_4523C8) ?
 //int32_t (*sub_452354) ?
 //int32_t (*sub_4522D4) ?
 //int32_t (*sub_452268) ?
 //int32_t (*sub_4521F0) ?
 //int32_t (*sub_452180) ?
 //int32_t (*sub_452110) ?
 //int32_t (*sub_4520AC) ?
 //int32_t (*sub_452008) ?
} xsetting_4712F276_class;

typedef struct xsetting_58560CA4_class_t  // 6
{
	int32_t (*GetVideoScreenMode)(int32_t *);
	int32_t (*SetVideoScreenMode)(int32_t);
	int32_t (*GetVideoOutputChannel)(int32_t *);
	int32_t (*SetVideoOutputChannel)(int32_t);
	int32_t (*GetVideoPlayerVolume)(int32_t *);
	int32_t (*SetVideoPlayerVolume)(int32_t);
} xsetting_58560CA4_class;

typedef struct xsetting_5FB90B89_class_t  // 38, bt
{
	//int32_t (*sub_46CB78) ?
	//int32_t (*sub_46CB54) ?
	//int32_t (*sub_46CB30) ?
	//int32_t (*sub_46CB0C) ?
	//int32_t (*sub_46CAE8) ?
	//int32_t (*sub_46CAC4) ?
	//int32_t (*sub_46CAA0) ?
	//int32_t (*sub_46CA7C) ?
	//int32_t (*sub_46CA58) ?
	//int32_t (*sub_46CA34) ?
	//int32_t (*sub_46CA10) ?
	//int32_t (*sub_46C9E8) ?
	//int32_t (*sub_46C998) ?
	//int32_t (*sub_46C948) ?
	//int32_t (*sub_46C8FC) ?
	//int32_t (*sub_46C8B4) ?
	//int32_t (*sub_46C890) ?
	//int32_t (*sub_46C86C) ?
	//int32_t (*sub_46C9C4) ?
	//int32_t (*sub_46C970) ?
	//int32_t (*sub_46C924) ?
	//int32_t (*sub_46C8D8) ?
	//int32_t (*sub_46C848) ?
	//int32_t (*sub_46C824) ?
	//int32_t (*sub_46C800) ?
	//int32_t (*sub_46C7CC) ?
	//int32_t (*sub_46C7A4) ?
	//int32_t (*sub_46C77C) ?
} xsetting_5FB90B89_class;

typedef struct xsetting_660ECC35_class_t  // 8
{
	int32_t (*SetPhotoNormalScenario)(int32_t);
	int32_t (*GetPhotoNormalScenario)(int32_t *);
	int32_t (*SetPhotoSlideScenario)(int32_t);
	int32_t (*GetPhotoSlideScenario)(int32_t *);
	int32_t (*SetPhotoSlideInterval)(int32_t);
	int32_t (*GetPhotoSlideInterval)(int32_t *);
	int32_t (*SetPhotoSlideRepeat)(int32_t);
	int32_t (*GetPhotoSlideRepeat)(int32_t *);
} xsetting_660ECC35_class;

typedef struct xsetting_69C19C7F_class_t  // 23
{
	//int32_t(*sub_450C40) ?
	//int32_t(*sub_450B8C) ?
	//int32_t(*sub_44E990) ?
	//int32_t(*sub_44E854) ?
	//int32_t(*sub_44E6E8) ?
	//int32_t(*sub_44C8E4) ?
	//int32_t(*sub_44C8A4) ?
	//int32_t(*sub_44C884) ?
	//int32_t(*sub_44C854) ?
	//int32_t(*sub_44C824) ?
	//int32_t(*sub_44C7E4) ?
	//int32_t(*sub_44C7C4) ?
	//int32_t(*sub_44C794) ?
	//int32_t(*sub_44C764) ?
	//int32_t(*sub_44C724) ?
	//int32_t(*sub_44C704) ?
	//int32_t(*sub_44C6D4) ?
	//int32_t(*sub_44BD70) ?
	//int32_t(*sub_44BD44) ?
	//int32_t(*sub_44BD1C) ?
	//int32_t(*sub_44BCF4) ?
	//int32_t(*sub_44BCCC) ?
  //int32_t(*sub_44BCA4) ?
} xsetting_69C19C7F_class;

typedef struct xsetting_7125FEB5_class_t  // 34
{
	int32_t (*SaveMusicCodec)(int32_t);
	int32_t (*loadMusicCodec)(int32_t *);
	int32_t (*SaveBitRateAac)(int32_t);
	int32_t (*loadBitRateAac)(int32_t *);
	int32_t (*SaveBitRateMp3)(int32_t);
	int32_t (*loadBitRateMp3)(int32_t *);
	int32_t (*SaveBitRateAtrac)(int32_t);
	int32_t (*loadBitRateAtrac)(int32_t *);
	int32_t (*SaveCrossFadePlay)(int32_t);
	int32_t (*loadCrossFadePlay)(int32_t *);
	int32_t (*SaveRepeatMode)(int32_t);
	int32_t (*loadRepeatMode)(int32_t *);
	int32_t (*SaveShuffleMode)(int32_t);
	int32_t (*loadShuffleMode)(int32_t *);
	int32_t (*SaveVisualizerSelect)(int32_t);
	int32_t (*loadVisualizerSelect)(int32_t *);
	int32_t (*SaveCddaEula)(int32_t);
	int32_t (*loadCddaEula)(int32_t *);
	int32_t (*SaveCddaServer)(int32_t);
	int32_t (*loadCddaServer)(int32_t *);
	int32_t (*SaveAudioplayerVolume)(int32_t);
	int32_t (*loadAudioplayerVolume)(int32_t *);
	int32_t (*SaveSacdPlayerVolume)(int32_t);
	int32_t (*loadSacdPlayerVolume)(int32_t *);
	int32_t (*SaveAtracActivation)(int32_t);
	int32_t (*loadAtracActivation)(int32_t *);
	int32_t (*SaveWmaActivation)(int32_t);
	int32_t (*loadWmaActivation)(int32_t *);
	int32_t (*SaveCdOutputFs)(int32_t);
	int32_t (*loadCdOutputFs)(int32_t *);
	int32_t (*SaveDitherSetting)(int32_t);
	int32_t (*loadDitherSetting)(int32_t *);
	int32_t (*SaveBitmappingSetting)(int32_t);
	int32_t (*loadBitmappingSetting)(int32_t *);
} xsetting_7125FEB5_class;

typedef struct xsetting_7EDDAD29_class_t  // 8
{
	int32_t (*loadParentalInformation)(void *);  // uint8_t [0x20] { parental passwordNumber, bdPc, bdPcAge, dvdRegion, dvdLevel, gameLevel, browserStartControl }
	int32_t (*SaveDvdRegion)(char *);
	int32_t (*SaveBdPc)(int32_t);
	int32_t (*SaveBdPcAge)(int32_t);
	int32_t (*SaveDvdRegionWithCheck)(int32_t);
	int32_t (*SaveDvdLevel)(int32_t);
	int32_t (*SaveGameLevel)(int32_t);
	int32_t (*SaveBrowserStartControl)(int32_t);
} xsetting_7EDDAD29_class;

typedef struct xsetting_8B69F85A_class_t  // 12
{
  int32_t (*loadDateTimeInformation)(void *);  //uint8_t [0x18] {diffTime, dateFormat, timeFormat, timeZone, summerTime }
  int32_t (*SaveDiffTime)(int32_t);
  int32_t (*SaveDateFormat)(int32_t);
	int32_t (*SaveTimeFormat)(int32_t);
	int32_t (*SaveTimeZone)(int32_t);
	int32_t (*SaveSummerTime)(int32_t);
	//int32_t (*sub_448B10)(void); // ret 0x8002B261
	//int32_t(*sub_448C74) ?
	//int32_t(*sub_448C40) ?
	//int32_t(*sub_448C0C) ?       // ret 0
	//int32_t(*sub_448BB8)(int32_t, int32_t);  // set timezone and summertime
	//int32_t(*sub_448B44) ?       // ret 0
} xsetting_8B69F85A_class;

typedef struct xsetting_9EE60B4E_class_t  // 48 sound ?
{
	//int32_t(*sub_4617B0) ?
	//int32_t(*sub_461774) ?
	//int32_t(*sub_461718) ?
	//int32_t(*sub_4616DC) ?
	//int32_t(*sub_4617DC) ?
	//int32_t(*sub_461674) ?
	//int32_t(*sub_461638) ?
	//int32_t(*sub_4615FC) ?
	//int32_t(*sub_4615B0) ?
	//int32_t(*sub_461564) ?
	//int32_t(*sub_461528) ?
	//int32_t(*sub_4614EC) ?
	//int32_t(*sub_4614B0) ?
	//int32_t(*sub_461474) ?
	//int32_t(*sub_461428) ?
	//int32_t(*sub_4613DC) ?
	//int32_t(*sub_461390) ?
	//int32_t(*sub_461354) ?
	//int32_t(*sub_461308) ?
	//int32_t(*sub_4612BC) ?
	//int32_t(*sub_461270) ?
	//int32_t(*sub_461224) ?
	//int32_t(*sub_4611E8) ?
	//int32_t(*sub_4611AC) ?
	//int32_t(*sub_461160) ?
	//int32_t(*sub_461134) ?
	//int32_t(*sub_4610F8) ?
	//int32_t(*sub_4610BC) ?
	//int32_t(*sub_461080) ?
	//int32_t(*sub_461034) ?
	//int32_t(*sub_460FF8) ?
	//int32_t(*sub_460FBC) ?
	//int32_t(*sub_460F70) ?
	//int32_t(*sub_460F10) ?
	//int32_t(*sub_460ED4) ?
	//int32_t(*sub_460E98) ?
	//int32_t(*sub_460E5C) ?
	//int32_t(*sub_460E20) ?
	//int32_t(*sub_460DF4) ?
	//int32_t(*sub_460DC8) ?
	//int32_t(*sub_460D8C) ?
	//int32_t(*sub_460D50) ?
	//int32_t(*sub_460D14) ?
	//int32_t(*sub_460CD8) ?
	//int32_t(*sub_460CAC) ?
	//int32_t(*sub_460C70) ?
	//int32_t(*sub_460C34) ?
	//int32_t(*sub_460BE8) ?
} xsetting_9EE60B4E_class;

typedef struct xsetting_C1008335_class_t  // 10 io_mouse ?
{
	//int32_t(*sub_45E244) ?
	//int32_t(*sub_45E200) ?
	//int32_t(*sub_45E2CC) ?
	//int32_t(*sub_45E288) ?
	//int32_t(*sub_45E1B8) ret 0x800315A6
	//int32_t(*sub_45E1C4) ret 0x800315A6
	//int32_t(*sub_45E1D0) ret 0x800315A6
	//int32_t(*sub_45E1DC) ret 0x800315A6
	//int32_t(*sub_45E1E8) ret 0x800315A6
	//int32_t(*sub_45E1F4) ret 0x800315A6
} xsetting_C1008335_class;

typedef struct xsetting_CC56EB2D_class_t  // 42
{
	int32_t (*sub_486D98)(void);
	int32_t (*sub_486D5C)(void);
	int32_t (*sub_486D30)(void);
	int32_t (*sub_486D04)(void);
	int32_t (*sub_486CC8)(void);
	int32_t (*sub_486C9C)(void);
	int32_t (*sub_486C50)(void);
	int32_t (*sub_486BF0)(void);
	int32_t (*sub_486B84)(void);
	int32_t (*sub_486B48)(void);
	int32_t (*sub_486AFC)(void);
	int32_t (*sub_486AB0)(void);
	int32_t (*GetCurrentUserNumber)(void);
	int32_t (*sub_486A58)(void);
	int32_t (*sub_486A1C)(void);
	int32_t (*sub_4869E0)(void);
	int32_t (*GetRegistryValue)(uint32_t userid, uint32_t packetid, int *value);
	int32_t (*GetRegistryString)(uint32_t userid, uint32_t packetid, char * value, uint32_t maxlen);
	int32_t (*SetRegistryValue)(uint32_t userid, uint32_t packetid, int value);
	int32_t (*SetRegistryString)(uint32_t userid, uint32_t packetid, char *value, uint32_t maxlen);
	int32_t (*sub_486814)(void);
	int32_t (*sub_4867D8)(void);
	int32_t (*sub_48678C)(void);
	int32_t (*sub_486740)(void);
	int32_t (*sub_4866F4)(void);
	int32_t (*sub_4866A8)(void);
	int32_t (*sub_48665C)(void);  // ret 0x80031605
	int32_t (*sub_486610)(void);  // ret 0x80031605
	int32_t (*sub_4865C4)(void);  // ret 0x80031605
	int32_t (*sub_486578)(void);  // ret 0x80031605
	int32_t (*sub_48652C)(void);  // ret 0x80031605
	int32_t (*sub_4864E0)(void);  // ret 0x80031605
	int32_t (*GetRegistryHomePath)(uint32_t userid, char *path);
	int32_t (*sub_486448)(void);
	int32_t (*sub_48640C)(void);
	int32_t (*sub_4863B0)(void);
	int32_t (*sub_486384)(void);
	int32_t (*sub_486358)(void);
	int32_t (*sub_4862F8)(void);
	int32_t (*sub_48628C)(void);
	int32_t (*sub_48623C)(void);
	int32_t (*sub_4861D4)(void);
/*
	int GetRegistryKeyboardType()	{int v; GetRegistryValue(0,0x14, &v);return v;}
	int GetRegistryKeyboardJapaneseInput()	{int v; GetRegistryValue(0,0x15, &v);return v;}
	int GetRegistryKeyboardTradChineseInput()	{int v; GetRegistryValue(0,0x16, &v);return v;}
	int GetRegistryKeyboardBind()	{int v; GetRegistryValue(0,0x17, &v);return v;}
	int GetRegistryKeyboardRepeatStartingTime()	{int v; GetRegistryValue(0,0x18, &v);return v;}
	int GetRegistryKeyboardRepeadSpeed()	{int v; GetRegistryValue(0,0x19, &v);return v;}
	int GetRegistryMouseType()	{int v; GetRegistryValue(0,0x1E, &v);return v;}
	int GetRegistryMouseSpeed()	{int v; GetRegistryValue(0,0x1F, &v);return v;}
	int GetRegistryBrowserHomeUri(char * uri)	{return GetRegistryString(0,0x28,uri,0x80);}
	int GetRegistryBrowserCookieMode()	{int v; GetRegistryValue(0,0x29, &v);return v;}
	int GetRegistryBrowserJavascript()	{int v; GetRegistryValue(0,0x2A, &v);return v;}
	int GetRegistryBrowserDisplayMode()	{int v; GetRegistryValue(0,0x2B, &v);return v;}
	int GetRegistryBrowserExitConfirmation()	{int v; GetRegistryValue(0,0x2C, &v);return v;}
	int GetRegistryBrowserOverscan()	{int v; GetRegistryValue(0,0x2D, &v);return v;}
	int GetRegistryBrowserWindowSize()	{int v; GetRegistryValue(0,0x2E, &v);return v;}
	int GetRegistryBrowserFontSize()	{int v; GetRegistryValue(0,0x2F, &v);return v;}
	int GetRegistryBrowserEncoding()	{int v; GetRegistryValue(0,0x30, &v);return v;}
	int GetRegistryBrowserCacheSize()	{int v; GetRegistryValue(0,0x31, &v);return v;}
	int GetRegistryBrowserRestoreWindow()	{int v; GetRegistryValue(0,0x32, &v);return v;}
	int GetRegistryBrowserPopupBlock()	{int v; GetRegistryValue(0,0x33, &v);return v;}
	int GetRegistryBrowserDpi()	{int v; GetRegistryValue(0,0x34, &v);return v;}
	int GetRegistryBrowserInterlaceFilter()	{int v; GetRegistryValue(0,0x35, &v);return v;}
	int GetRegistryBrowserIfilter()	{int v; GetRegistryValue(0,0x36, &v);return v;}
	int GetRegistryBrowserIfilterAddress()	{int v; GetRegistryValue(0,0x37, &v);return v;}
	int GetRegistryBrowserIfilterPort()	{int v; GetRegistryValue(0,0x38, &v);return v;}
	int GetRegistryBrowserIfilterUsername()	{int v; GetRegistryValue(0,0x39, &v);return v;}
	int GetRegistryBrowserIfilterPassword()	{int v; GetRegistryValue(0,0x3A, &v);return v;}
	int GetRegistryBrowserIfilterBase64()	{int v; GetRegistryValue(0,0x3B, &v);return v;}
	int GetRegistryBrowserTrendEula()	{int v; GetRegistryValue(0,0x3C, &v);return v;}
	int GetRegistryBrowserTrendEnable()	{int v; GetRegistryValue(0,0x3D, &v);return v;}
	int GetRegistryInputForecastDictionary()	{int v; GetRegistryValue(0,0x46, &v);return v;}
	int GetRegistryInputTenKeyInputType()	{int v; GetRegistryValue(0,0x47, &v);return v;}
	int GetRegistryInputForecastDictionaryCh()	{int v; GetRegistryValue(0,0x48, &v);return v;}
	int GetRegistryInputKeyLayoutType()	{int v; GetRegistryValue(0,0x49, &v);return v;}
	int GetRegistryEdyEula()	{int v; GetRegistryValue(0,0x50, &v);return v;}
	int GetRegistryThemeWallpaper()	{int v; GetRegistryValue(0,0x5A, &v);return v;}
	int GetRegistryThemeFont()	{int v; GetRegistryValue(0,0x5B, &v);return v;}
	int GetRegistryThemeFontEu()	{int v; GetRegistryValue(0,0x5C, &v);return v;}
	int GetRegistryThemeFontRu()	{int v; GetRegistryValue(0,0x5D, &v);return v;}
	int GetRegistryThemeFile(char * f)	{return GetRegistryString(0,0x5E,f,0x80);}
	int GetRegistryThemeColor()	{int v; GetRegistryValue(0,0x5F, &v);return v;}
	int GetRegistryThemeEndarkbg()	{int v; GetRegistryValue(0,0x60, &v);return v;}
	int GetRegistryMicPitchshift()	{int v; GetRegistryValue(0,0x64, &v);return v;}
	int GetRegistryUserRestoreSignInPassword(char * p)	{return GetRegistryString(0,0x6E,p,0x80);}
	int GetRegistryUserRestoreSignInStatus()	{int v; GetRegistryValue(0,0x6F, &v);return v;}
	int GetRegistryYtaccountAccountid(char * a)	{return GetRegistryString(0,0x78,a,0x80);}
	int GetRegistryYtaccountPassword(char * p)	{return GetRegistryString(0,0x79,p,0x80);}
	int GetRegistryNpGuestCountry(char * c)	{return GetRegistryString(0,0x82,c,0x80);}
	int GetRegistryNpGuestLang(char * l)	{return GetRegistryString(0,0x83,l,0x80);}
	int GetRegistryNpGuestBirth()	{int v; GetRegistryValue(0,0x84, &v);return v;}
	int GetRegistryWBoardEnable()	{int v; GetRegistryValue(0,0x8C, &v);return v;}
	int GetRegistryWBoardFocusMask(){int v; GetRegistryValue(0,0x8D, &v);return v;}
	int GetRegistryNetOnlineFlag()	{int v; GetRegistryValue(0,0x96, &v);return v;}
	int GetRegistryFacebookAccount(char * a)	{return GetRegistryString(0,0xC8,a,0x80);}
	int GetRegistryFacebookSessionKey()	{int v; GetRegistryValue(0,0xC9, &v);return v;}
	int GetRegistryFacebookSessionSecret()	{int v; GetRegistryValue(0,0xCA, &v);return v;}
	int GetRegistryFacebookUid()	{int v; GetRegistryValue(0,0xCB, &v);return v;}
	int GetRegistryFacebookTrophy()	{int v; GetRegistryValue(0,0xCC, &v);return v;}
	int GetRegistryFacebookPurchase()	{int v; GetRegistryValue(0,0xCD, &v);return v;}
	int GetRegistryFacebookGameEvent()	{int v; GetRegistryValue(0,0xCE, &v);return v;}
	int GetRegistryFacebookHknwEnable()	{int v; GetRegistryValue(0,0xCF, &v);return v;}
	int GetRegistryFacebookAccessToken()	{int v; GetRegistryValue(0,0xD0, &v);return v;}
	int GetRegistryFacebookGameIntegrations()	{int v; GetRegistryValue(0,0xD1, &v);return v;}
	int GetRegistryPicasaMail(char * m)	{return GetRegistryString(0,0xD2,m,0x80);}
	int GetRegistryPicasaPassword(char * p)	{return GetRegistryString(0,0xD3,p,0x80);}
	int GetRegistryPicasaHknwEnable()	{int v; GetRegistryValue(0,0xD4, &v);return v;}
	int GetRegistryFlickrMail(char * m)	{return GetRegistryString(0,0xDC,m,0x80);}
	int GetRegistryFlickrPassword(char * p)	{return GetRegistryString(0,0xDD,p,0x80);}
	int GetRegistryFacebookRating()	{int v; GetRegistryValue(0,0xE6, &v);return v;}
	int SetRegistryWBoardFocusMask(int v){ return DoUnk18_SetRegistryValue(0,0x8D, v);}
*/
} xsetting_CC56EB2D_class;

typedef struct xsetting_CE27E884_class_t  // 10
{
	uint32_t (*savePremoPSPInformation)(void *); // uint8_t [0x4B8]
	uint32_t (*loadPremoPSPInformation)(void *); // uint8_t [0x4B8]
	uint32_t (*saveRemoteBoot)(uint32_t);
	uint32_t (*loadRemoteBoot)(uint32_t *);
	uint32_t (*saveBootCount)(uint32_t);
	uint32_t (*loadBootCount)(uint32_t *);
	uint32_t (*savePowerOffTime)(void *);
	uint32_t (*loadPowerOffTime)(void *);
	uint32_t (*saveAudioConfig)(uint32_t);
	uint32_t (*loadAudioConfig)(uint32_t *);
} xsetting_CE27E884_class;

typedef struct xsetting_D0261D72_class_t  // 11
{
	uint32_t (*saveAvcInitialCameraMode)(int32_t);
	uint32_t (*loadAvcInitialCameraMode)(int32_t *);
	uint32_t (*saveNpEnvironment)(char *, int32_t *);              // env, len
	uint32_t (*loadNpEnvironment)(char *, int32_t *);              // env, len
	uint32_t (*saveRegistryIntValue)(int32_t, int32_t);            // id, value
	uint32_t (*loadRegistryIntValue)(int32_t, int32_t*);           // id, value
	uint32_t (*saveRegistryStringValue)(int32_t, char *, int32_t); // id, string, len
	uint32_t (*loadRegistryStringValue)(int32_t, char *, int32_t); // id, string, len
	uint32_t (*Setunk1)(int32_t);
	uint32_t (*Getunk2)(int32_t, int32_t *);
	uint32_t (*Setunk3)(int32_t, int32_t);
/*
	int loadRegistryAvcVgaLastBitrate()	{int v; loadRegistryIntValue(0x00, &v);return v;}                                         //0x00 || "/setting/avc/vga/lastBitrate"
	int loadRegistryGameLevel0Control()	{int v; loadRegistryIntValue(0x01, &v);return v;}                                         //0x01 || "/setting/parental/gameLevel0Control"
	int loadRegistryNetworkServiceControl()	{int v; loadRegistryIntValue(0x02, &v);return v;}                                     //0x02 || "/setting/parental/networkServiceControl"
	int loadRegistryCddaServer()	{int v; loadRegistryIntValue(0x03, &v);return v;}                                             //0x03 || "/setting/music/cddaServer"
	int loadRegistryGameBgmPlayback()	{int v; loadRegistryIntValue(0x04, &v);return v;}                                         //0x04 || "/setting/music/gameBgmPlayback"
	int loadRegistryGameBgmVolume()	{int v; loadRegistryIntValue(0x05, &v);return v;}                                             //0x05 || "/setting/music/gameBgmVolume"
	int loadRegistryDummyBgmPlayer()	{int v; loadRegistryIntValue(0x06, &v);return v;}                                         //0x06 || "/setting/music/dummyBgmPlayer"
	int loadRegistryDynamicNormalizer()	{int v; loadRegistryIntValue(0x07, &v);return v;}                                         //0x07 || "/setting/music/dynamicNormalizer"
	int loadRegistryNpDebug()	{int v; loadRegistryIntValue(0x08, &v);return v;}                                                 //0x08 || "/setting/np/debug"
	int loadRegistryNpTitleId(char * titleid,int max_len)	{return loadRegistryStringValue(0x09,titleid,max_len);}               //0x09 || "/setting/np/titleId"
	int loadRegistryNavOnly()	{int v; loadRegistryIntValue(0x0A, &v);return v;}                                                 //0x0A || "/setting/np/navOnly"
	int loadRegistryNpAdClockDiff()	{int v; loadRegistryIntValue(0x0B, &v);return v;}                                             //0x0B || "/setting/np/npAdClockDiff"
	int loadRegistryDebugDrmError()	{int v; loadRegistryIntValue(0x0C, &v);return v;}                                             //0x0C || "/setting/np/debugDrmError"
	int loadRegistryDebugDrmClock()	{int v; loadRegistryIntValue(0x0D, &v);return v;}                                             //0x0D || "/setting/np/debugDrmClock"
	int loadRegistryDebugConsoleBind()	{int v; loadRegistryIntValue(0x0E, &v);return v;}                                         //0x0E || "/setting/np/debugConsoleBind"
	int loadRegistryDebugIngameCommerce2()	{int v; loadRegistryIntValue(0x0F, &v);return v;}                                     //0x0F || "/setting/np/debugIngameCommerce2"
	int loadRegistryDebugSFForce()	{int v; loadRegistryIntValue(0x10, &v);return v;}                                             //0x10 || "/setting/np/debugSFForce"
	int loadRegistryDebugSFManifest()	{int v; loadRegistryIntValue(0x11, &v);return v;}                                         //0x11 || "/setting/np/debugSFManifest"
	int loadRegistryDebugSFManifestURL()	{int v; loadRegistryIntValue(0x12, &v);return v;}                                     //0x12 || "/setting/np/debugSFManifestURL"
	int loadRegistryNpGeoFiltering()	{int v; loadRegistryIntValue(0x13, &v);return v;}                                         //0x13 || "/setting/np/npGeoFiltering"
	int loadRegistryGameUpdateImposeTest()	{int v; loadRegistryIntValue(0x14, &v);return v;}                                     //0x14 || "/setting/np/gameUpdateImposeTest"
	int loadRegistryGameUpdateForceOverwrite()	{int v; loadRegistryIntValue(0x15, &v);return v;}                                 //0x15 || "/setting/np/gameUpdateForceOverwrite"
	int loadRegistryFakeNpSnsThrottle()	{int v; loadRegistryIntValue(0x16, &v);return v;}                                         //0x16 || "/setting/np/fakeNpSnsThrottle"
	int loadRegistryFakeNpSnsThrottleWaitSeconds()	{int v; loadRegistryIntValue(0x17, &v);return v;}                             //0x17 || "/setting/np/fakeNpSnsThrottleWaitSeconds"
	int loadRegistryTppsProxyFlag()	{int v; loadRegistryIntValue(0x18, &v);return v;}                                             //0x18 || "/setting/np/tppsProxyFlag"
	int loadRegistryTppsProxyServer(char * server,int max_len)	{return loadRegistryStringValue(0x19,server,max_len);}  // ???    //0x19 || "/setting/np/tppsProxyServer"
	int loadRegistryTppsProxyPort()	{int v; loadRegistryIntValue(0x1A, &v);return v;}                                             //0x1A || "/setting/np/tppsProxyPort"
	int loadRegistryTppsProxyUserName(char * username,int max_len)	{return loadRegistryStringValue(0x1B,username,max_len);}      //0x1B || "/setting/np/tppsProxyUserName"
	int loadRegistryTppsProxyPassword(char * password,int max_len)	{return loadRegistryStringValue(0x1C,password,max_len);}      //0x1C || "/setting/np/tppsProxyPassword"
	int loadRegistryRegion()	{int v; loadRegistryIntValue(0x1D, &v);return v;}                                                 //0x1D || "/setting/system/region"
	int loadRegistryNotificationTrophy()	{int v; loadRegistryIntValue(0x1E, &v);return v;}                                     //0x1E || "/setting/system/notificationTrophy"
	int loadRegistryLicenseArea()	{int v; loadRegistryIntValue(0x1F, &v);return v;}                                             //0x1F || "/setting/system/licenseArea"
	int loadRegistryHddSerial(char * hddserial)	{return loadRegistryStringValue(0x20,hddserial,0x3D);}                            //0x20 || "/setting/system/hddSerial"
	int loadRegistryCoreDump()	{int v; loadRegistryIntValue(0x21, &v);return v;}                                                 //0x21 || "/setting/system/coreDump"
	int loadRegistryCoreDumpOptionTrigger()	{int v; loadRegistryIntValue(0x22, &v);return v;}                                     //0x22 || "/setting/system/coreDumpOptionTrigger"
	int loadRegistryCoreDumpOptionFileGen()	{int v; loadRegistryIntValue(0x23, &v);return v;}                                     //0x23 || "/setting/system/coreDumpOptionFileGen"
	int loadRegistryCoreDumpOptionExeCtrl()	{int v; loadRegistryIntValue(0x24, &v);return v;}                                     //0x24 || "/setting/system/coreDumpOptionExeCtrl"
	int loadRegistryMatEnable()	{int v; loadRegistryIntValue(0x25, &v);return v;}                                                 //0x25 || "/setting/system/matEnable"
	int loadRegistryUpdateServerUrl(char * url,int max_len)	{return loadRegistryStringValue(0x26,url,max_len);}                   //0x26 || "/setting/system/updateServerUrl"
	int loadRegistryFakeLimitSize()	{int v; loadRegistryIntValue(0x27, &v);return v;}                                             //0x27 || "/setting/system/fakeLimitSize"
	int loadRegistryFakeFreeSpace()	{int v; loadRegistryIntValue(0x28, &v);return v;}                                             //0x28 || "/setting/system/fakeFreeSpace"
	int loadRegistryFakeSavedataOwner()	{int v; loadRegistryIntValue(0x29, &v);return v;}                                         //0x29 || "/setting/system/fakeSavedataOwner"
	int loadRegistryFakeHddSpeed()	{int v; loadRegistryIntValue(0x2A, &v);return v;}                                             //0x2A || "/setting/system/fakeHddSpeed"
	int loadRegistryDebugGameType()	{int v; loadRegistryIntValue(0x2B, &v);return v;}                                             //0x2B || "/setting/system/debugGameType"
	int loadRegistryDebugBootPath()	{int v; loadRegistryIntValue(0x2C, &v);return v;}                                             //0x2C || "/setting/system/debugBootPath"
	int loadRegistryDebugDirName(char * path,int max_len)	{return loadRegistryStringValue(0x2D,path,max_len);}                  //0x2D || "/setting/system/debugDirName"
	int loadRegistryAppHomeBootPath()	{int v; loadRegistryIntValue(0x2E, &v);return v;}                                         //0x2E || "/setting/system/appHomeBootPath"
	int loadRegistryWolDex()	{int v; loadRegistryIntValue(0x2F, &v);return v;}                                                 //0x2F || "/setting/system/wolDex"
	int loadRegistryDispHddSpace()	{int v; loadRegistryIntValue(0x30, &v);return v;}                                             //0x30 || "/setting/system/dispHddSpace"
	int loadRegistryAutoNetworkUpdate()	{int v; loadRegistryIntValue(0x31, &v);return v;}                                         //0x31 || "/setting/system/autoNetworkUpdate"
	int loadRegistryAutoPowerOff()	{int v; loadRegistryIntValue(0x32, &v);return v;}                                             //0x32 || "/setting/system/autoPowerOff"
	int loadRegistryAutoPowerOffEx()	{int v; loadRegistryIntValue(0x33, &v);return v;}                                         //0x33 || "/setting/system/autoPowerOffEx"
	int loadRegistryAutoPowerOffDebug()	{int v; loadRegistryIntValue(0x34, &v);return v;}                                         //0x34 || "/setting/system/autoPowerOffDebug"
	int loadRegistryHdmiControl()	{int v; loadRegistryIntValue(0x35, &v);return v;}                                             //0x35 || "/setting/system/hdmiControl"
	int loadRegistryHdmiControlEx()	{int v; loadRegistryIntValue(0x36, &v);return v;}                                             //0x36 || "/setting/system/hdmiControlEx"
	int loadRegistryPowerOnDiscBoot()	{int v; loadRegistryIntValue(0x37, &v);return v;}                                         //0x37 || "/setting/system/powerOnDiscBoot"
	int loadRegistryPowerOnReset()	{int v; loadRegistryIntValue(0x38, &v);return v;}                                             //0x38 || "/setting/system/powerOnReset"
	int loadRegistryDisable15Timeout()	{int v; loadRegistryIntValue(0x39, &v);return v;}                                         //0x39 || "/setting/system/disable15Timeout"
	int loadRegistryDebugSystemUpdate()	{int v; loadRegistryIntValue(0x3A, &v);return v;}                                         //0x3A || "/setting/system/debugSystemUpdate"
	int loadRegistryFakePlus()	{int v; loadRegistryIntValue(0x3B, &v);return v;}                                                 //0x3B || "/setting/system/fakePlus"
	int loadRegistryTurnOffWarning()	{int v; loadRegistryIntValue(0x3C, &v);return v;}                                         //0x3C || "/setting/system/turnOffWarning"
	int loadRegistryBootMode(char * bootmode,int max_len)	{return loadRegistryStringValue(0x3D,bootmode,max_len);}              //0x3D || "/setting/system/bootMode"
	int loadRegistryCrashreportCrepo()	{int v; loadRegistryIntValue(0x3E, &v);return v;}                                         //0x3E || "/setting/system/crashreport/crepo"
	int loadRegistryCrashreportReporterStatus()	{int v; loadRegistryIntValue(0x3F, &v);return v;}                                 //0x3F || "/setting/system/crashreport/reporterStatus"
	int loadRegistryCrashreportVshGeneratorEnableFlag()	{int v; loadRegistryIntValue(0x40, &v);return v;}                         //0x40 || "/setting/system/crashreport/vshGeneratorEnableFlag"
	int loadRegistryDateTimeAutoCorrection()	{int v; loadRegistryIntValue(0x41, &v);return v;}                                 //0x41 || "/setting/dateTime/autoCorrection"
	int loadRegistryAutobootStartTime()	{int v; loadRegistryIntValue(0x42, &v);return v;}                                         //0x42 || "/setting/autoboot/startTime"
	int loadRegistryEdyDebug()	{int v; loadRegistryIntValue(0x43, &v);return v;}                                                 //0x43 || "/setting/edy/debug"
	int loadRegistryUpConvert()	{int v; loadRegistryIntValue(0x44, &v);return v;}                                                 //0x44 || "/setting/video/upConvert"
	int loadRegistryFnrLevel()	{int v; loadRegistryIntValue(0x45, &v);return v;}                                                 //0x45 || "/setting/video/fnr"
	int loadRegistryBnrLevel()	{int v; loadRegistryIntValue(0x46, &v);return v;}                                                 //0x46 || "/setting/video/bnr"
	int loadRegistryMnrLevel()	{int v; loadRegistryIntValue(0x47, &v);return v;}                                                 //0x47 || "/setting/video/mnr"
	int loadRegistrySequentialPlay()	{int v; loadRegistryIntValue(0x48, &v);return v;}                                         //0x48 || "/setting/video/sequentialPlay"
	int loadRegistryHD50HzOutput()	{int v; loadRegistryIntValue(0x49, &v);return v;}                                             //0x49 || "/setting/video/hd50HzOutput"
	int loadRegistryOutputExtMenu()	{int v; loadRegistryIntValue(0x4A, &v);return v;}                                             //0x4A || "/setting/video/outputExtMenu"
	int loadRegistryOutputExtFunc()	{int v; loadRegistryIntValue(0x4B, &v);return v;}                                             //0x4B || "/setting/video/outputExtFunc"
	int loadRegistryDtcpIpSettingMenu()	{int v; loadRegistryIntValue(0x4C, &v);return v;}                                         //0x4C || "/setting/video/dtcpipActMenu"
	int loadRegistryDefaultSubTitleLanguage()	{int v; loadRegistryIntValue(0x4D, &v);return v;}                                 //0x4D || "/setting/video/hddCaptionLanguage"
	int loadRegistryDefaultAudioLanguage()	{int v; loadRegistryIntValue(0x4E, &v);return v;}                                     //0x4E || "/setting/video/hddSoundLanguage"
	int loadRegistryDefaultClosedCaption()	{int v; loadRegistryIntValue(0x4F, &v);return v;}                                     //0x4F || "/setting/video/closedCaption"
	int loadRegistryEmuUpConvert()	{int v; loadRegistryIntValue(0x50, &v);return v;}                                             //0x50 || "/setting/game/emuUpConvert"
	int loadRegistryEmuSmoothing()	{int v; loadRegistryIntValue(0x51, &v);return v;}                                             //0x51 || "/setting/game/emuSmoothing"
	int loadRegistryMinisUpConvert()	{int v; loadRegistryIntValue(0x52, &v);return v;}                                         //0x52 || "/setting/game/minisUpConvert"
	int loadRegistryPspemuViewmode()	{int v; loadRegistryIntValue(0x53, &v);return v;}                                         //0x53 || "/setting/game/pspemuViewmode"
	int loadRegistryPspemu3dDisplay()	{int v; loadRegistryIntValue(0x54, &v);return v;}                                         //0x54 || "/setting/game/pspemu3dDisplay"
	int loadRegistryPspemu3dDepthAdjust()	{int v; loadRegistryIntValue(0x55, &v);return v;}                                     //0x55 || "/setting/game/pspemu3dDepthAdjust"
	int loadRegistryPspemu3dMenu()	{int v; loadRegistryIntValue(0x56, &v);return v;}                                             //0x56 || "/setting/game/pspemu3dMenu"
	int loadRegistryPspemuAdhocModeWlan()	{int v; loadRegistryIntValue(0x57, &v);return v;}                                     //0x57 || "/setting/game/pspemuAdhocModeWlan"
	int loadRegistryPspemuAdhocModeCh()	{int v; loadRegistryIntValue(0x58, &v);return v;}                                         //0x58 || "/setting/game/pspemuAdhocModeCh"
	int loadRegistryPs2emuSaveUtility()	{int v; loadRegistryIntValue(0x59, &v);return v;}                                         //0x59 || "/setting/game/ps2emuSaveUtility"
	int loadRegistryPs2softemuFunc()	{int v; loadRegistryIntValue(0x5A, &v);return v;}                                         //0x5A || "/setting/game/ps2softemuFunc"
	int loadRegistryPs2BgCaution()	{int v; loadRegistryIntValue(0x5B, &v);return v;}                                             //0x5B || "/setting/game/ps2BgCaution"
	int loadRegistryCameraPlfreq()	{int v; loadRegistryIntValue(0x5C, &v);return v;}                                             //0x5C || "/setting/camera/plfreq"
	int loadRegistryTvCategory()	{int v; loadRegistryIntValue(0x5D, &v);return v;}                                             //0x5D || "/setting/xmb/tvCategory"
	int loadRegistryHomeInstaller()	{int v; loadRegistryIntValue(0x5E, &v);return v;}                                             //0x5E || "/setting/xmb/homeInstaller"
	int loadRegistryHomeQAMode()	{int v; loadRegistryIntValue(0x5F, &v);return v;}                                             //0x5F || "/setting/xmb/homeQAMode"
	int loadRegistryDummyInGameXMB()	{int v; loadRegistryIntValue(0x60, &v);return v;}                                         //0x60 || "/setting/xmb/dummyInGameXMB"
	int loadRegistryYconExplained()	{int v; loadRegistryIntValue(0x61, &v);return v;}                                             //0x61 || "/setting/xmb/yconExplained"
	int loadRegistryXaiDebugFlag()	{int v; loadRegistryIntValue(0x62, &v);return v;}                                             //0x62 || "/setting/xai/debugFlag"
	int loadRegistryAdServerURL(char * url,int max_len)	{return loadRegistryStringValue(0x63,url,max_len);}                       //0x63 || "/setting/libad/adServerURL"
	int loadRegistryAdCatalogVersion(char * version,int max_len)	{return loadRegistryStringValue(0x64,version,max_len);}       //0x64 || "/setting/libad/adCatalogVersion"
	int loadRegistryAdEnableNotification()	{int v; loadRegistryIntValue(0x65, &v);return v;}                                     //0x65 || "/setting/libad/adEnableNotification"
	int loadRegistryUploadDebug()	{int v; loadRegistryIntValue(0x66, &v);return v;}                                             //0x66 || "/setting/upload/debug"
	int loadRegistryNetAutoDlDebug()	{int v; loadRegistryIntValue(0x67, &v);return v;}                                         //0x67 || "/setting/net/autoDlDebug"
	int loadRegistryNetAutoDlFlag()	{int v; loadRegistryIntValue(0x68, &v);return v;}                                             //0x68 || "/setting/net/autoDlFlag"
	int loadRegistryNetAutoDlTime()	{int v; loadRegistryIntValue(0x69, &v);return v;}                                             //0x69 || "/setting/net/autoDlTime"
	int loadRegistryNetAutoDlFunc()	{int v; loadRegistryIntValue(0x6A, &v);return v;}                                             //0x6A || "/setting/net/autoDlFunc"
	int loadRegistryNetEmulationType()	{int v; loadRegistryIntValue(0x6B, &v);return v;}   // questionable                       //0x6B || "/setting/net/emulationType"
	int loadRegistryNetAdhocSsidPrefix(char * prefix,int max_len)	{return loadRegistryStringValue(0x6C,prefix,max_len);}        //0x6C || "/setting/net/adhocSsidPrefix"
	int loadRegistryPadVibrationEnable()	{int v; loadRegistryIntValue(0x6D, &v);return v;}                                     //0x6D || "/setting/pad/vibrationEnable"
	int loadRegistryPadAutoPowerOff()	{int v; loadRegistryIntValue(0x6E, &v);return v;}                                         //0x6E || "/setting/pad/autoPowerOff"
	int loadRegistryPadMagnetometer()	{int v; loadRegistryIntValue(0x6F, &v);return v;}                                         //0x6F || "/setting/pad/magnetometer"
	int loadRegistrySound0Initial()	{int v; loadRegistryIntValue(0x70, &v);return v;}                                             //0x70 || "/setting/sound/0/initial"
	int loadRegistrySound1UsbHeadSetSound()	{int v; loadRegistryIntValue(0x71, &v);return v;}   // questionable                   //0x71 || "/setting/sound/1/usbHeadsetSound"
	int loadRegistryDlnaFlag()	{int v; loadRegistryIntValue(0x72, &v);return v;}                                                 //0x72 || "/setting/dlna/dlnaFlag"
	int loadRegistryDlnaDtcpipDevCert()	{int v; loadRegistryIntValue(0x73, &v);return v;}   // questionable                       //0x73 || "/setting/dlna/dtcpipDevCert"
	int loadRegistryBrowserTrendEula()	{int v; loadRegistryIntValue(0x74, &v);return v;}                                         //0x74 || "/setting/browser/trendEula"
	int loadRegistryBrowserTrendEnable()	{int v; loadRegistryIntValue(0x75, &v);return v;}                                     //0x75 || "/setting/browser/trendEnable"
	int loadRegistryBrowserTrendLastTime()	{int v; loadRegistryIntValue(0x76, &v);return v;}                                     //0x76 || "/setting/browser/trendLastTime"
	int loadRegistryBrowserTrendTtl()	{int v; loadRegistryIntValue(0x77, &v);return v;}                                         //0x77 || "/setting/browser/trendTtl"
	int loadRegistryBrowserTrendRegistered()	{int v; loadRegistryIntValue(0x78, &v);return v;}                                 //0x78 || "/setting/browser/trendRegistered"
	int loadRegistryBrowserHeapSize()	{int v; loadRegistryIntValue(0x79, &v);return v;}                                         //0x79 || "/setting/browser/heapSize"
	int loadRegistryBrowserDebugMenu()	{int v; loadRegistryIntValue(0x7A, &v);return v;}                                         //0x7A || "/setting/browser/debugMenu"
	int loadRegistryBrowserType()	{int v; loadRegistryIntValue(0x7B, &v);return v;}                                             //0x7B || "/setting/browser/browserType"
	int loadRegistryWboardBaseUri(char * uri,int max_len)	{return loadRegistryStringValue(0x7C,uri,max_len);}                   //0x7C || "/setting/wboard/baseUri"
	int loadRegistrySmssTargetServer()	{int v; loadRegistryIntValue(0x7D, &v);return v;}                                         //0x7D || "/setting/smss/targetServer"
	int loadRegistrySmssResultOutput()	{int v; loadRegistryIntValue(0x7E, &v);return v;}                                         //0x7E || "/setting/smss/resultOutput"
	int loadRegistryDisplayForceEnable3D()	{int v; loadRegistryIntValue(0x7F, &v);return v;}                                     //0x7F || "/setting/display/forceEnable3D"
	int loadRegistryDisplayScreenSize()	{int v; loadRegistryIntValue(0x80, &v);return v;}                                         //0x80 || "/setting/display/screenSize"
	int loadRegistryDisplayDeepColor()	{int v; loadRegistryIntValue(0x81, &v);return v;}                                         //0x81 || "/setting/display/deepColor"
*/
} xsetting_D0261D72_class;

typedef struct xsetting_ED5B559F_class_t  // 16
{
	//int32_t (*sub_45E35C) ?  Export_sys_io_sys_config_add_service_listener()
	//int32_t (*sub_45E328) ?  Export_sys_io_sys_config_remove_service_listener()
	//int32_t (*sub_45E750) ?
	//int32_t (*sub_45E6FC) ?
	//int32_t (*sub_45E6A8) ?
	//int32_t (*sub_45E644) ?
	//int32_t (*sub_45E310) ?
	//int32_t (*sub_45E600) ?
	//int32_t (*sub_45E5BC) ?
	//int32_t (*sub_45E578) ?  Export_sdk_4AF81871_io_pad_DisconnectGamePad()
	//int32_t (*sub_45E544) ?
	//int32_t (*sub_45E510) ?
	//int32_t (*sub_45E4AC) ?
	//int32_t (*sub_45E448) ?
	//int32_t (*sub_45E3F4) ?
	//int32_t (*sub_45E3A0) ?
} xsetting_ED5B559F_class;

////////////////////////////////////////////////////////////////////////
// network structs
typedef struct _address {
	char addr[0x10];
} address;

typedef struct _lease_info {
	char ip[0x10];
	uint64_t time;
} lease_info;

typedef struct _wireless_info {
	char ssid[0x20];
	char passPhrase[41];
} wireless_info;

typedef struct _net_info {
  int32_t device;                      //0x000:
  int32_t etherMode;                   //0x004:
  int32_t authProto;                   //0x008:
  char wirelessPassPhrase[0x41];       //0x00C: wepKey[0x1A] wpaKey[0x40]
  char ssid[0x20];                     //0x04D: ssid[0x20]
  uint8_t pad0[3];
  int32_t auth8021xType;               //0x070: int32_t auth8021xType
  char auth8021xAuthName[0x80];        //0x074: auth8021xAuthName[0x80]
  char auth8021xAuthKey[0x80];         //0x0F4: auth8021xAuthKey[0x80]
  int32_t howToSetupIp;                //0x174:
  char dhcpHostName[0xFF];             //0x178: dhcpHostName[0xFF]
  char authName[0x80];                 //0x277: authName[0x80]
  char authKey[0x80];                  //0x2F7: authKey[0x80]
  char ipAddress[0x10];                //0x377: ipAddress[0x10]
  char netmask[0x10];                  //0x387: netmask[0x10]
  char defaultRoute[0x10];             //0x397: defaultRoute[0x10]
  uint8_t pad1;
  int32_t dnsFlag;                     //0x3A8:
  char primaryDns[0x10];               //0x3AC:
  char secondaryDns[0x10];             //0x3BC:
  int32_t httpProxyFlag;               //0x3CC:
  char httpProxyServer[0x100];         //0x3D0:
  int16_t httpProxyPort;               //0x4D0:
  int16_t pad2;
  int32_t mtu;                         //0x4D4:
  int32_t upnpFlag;                    //0x4D8:
  int32_t aossFlag;                    //0x4DC:
  char aossWep64Ssid[0x20];            //0x4E0:
  char aossWep64Key[0x41];             //0x500:
  char aossWep128Ssid[0x20];           //0x541:
  char aossWep128Key[0x41];            //0x561:
  char aossWpaPskTkipSsid[0x20];       //0x5A2:
  char aossWpaPskTkipKey[0x41];        //0x5C2:
  char aossWpaPskAesSsid[0x20];        //0x603:
  char aossWpaPskAesKey[0x41];         //0x623:
} net_info;



typedef struct xsetting_F48C0548_class_t  // 38 net
{
	int32_t (*SetSettingNet_enable)(int32_t);
	int32_t (*GetSettingNet_enable)(int32_t *);
	int32_t (*SetSettingNet_device)(int32_t);
	int32_t (*GetSettingNet_device)(int32_t *);
	int32_t (*SetSettingNet_apAutoConfig)(int32_t);
	int32_t (*GetSettingNet_apAutoConfig)(int32_t *);
	int32_t (*SetSettingNet_eapMd5)(int32_t);
	int32_t (*GetSettingNet_eapMd5)(int32_t *);
	int32_t (*SetSettingNet_howToSetupIp)(int32_t);
	int32_t (*GetSettingNet_howToSetupIp)(int32_t *);
	int32_t (*SetSettingNet_ipAddress)(address *);
	int32_t (*GetSettingNet_ipAddress)(address *);
	int32_t (*SetSettingNet_netmask)(address *);
	int32_t (*GetSettingNet_netmask)(address *);
	int32_t (*SetSettingNet_defaultRoute)(address *);
	int32_t (*GetSettingNet_defaultRoute)(address *);
	int32_t (*SetSettingNet_dnsFlag)(int32_t);
	int32_t (*GetSettingNet_dnsFlag)(int32_t *);
	int32_t (*SetSettingNet_primaryDns)(address *);
	int32_t (*GetSettingNet_primaryDns)(address *);
	int32_t (*SetSettingNet_secondaryDns)(address *);
	int32_t (*GetSettingNet_secondaryDns)(address *);
	int32_t (*SetSettingNet_httpProxyFlag)(int32_t);
	int32_t (*GetSettingNet_httpProxyFlag)(int32_t *);

	int32_t (*sub_44A104)(void *);     // set "/setting/net/httpProxyServer"    [0x100]
	int32_t (*sub_449F0C)(void *);     // get "/setting/net/httpProxyServer"    [0x100]

	int32_t (*SetSettingNet_httpProxyPort)(int16_t);
	int32_t (*GetSettingNet_httpProxyPort)(int16_t *);
	int32_t (*SetSettingNet_upnpFlag)(int32_t);
	int32_t (*GetSettingNet_upnpFlag)(int32_t *);

	int32_t (*sub_44AE60)(net_info *);            // set network config, [0x664]
	int32_t (*sub_44A47C)(net_info *);            // get network config, [0x664]
	int32_t (*sub_44B2EC)(int32_t, net_info *);   // set network config, [0x664], arg1(device) 0 or 2
	int32_t (*sub_44AA24)(int32_t, net_info *);   // get network config, [0x664], arg1(device) 0 or 2

	int32_t (*SetLeaseIPAndTime)(int32_t, lease_info *);       // arg1(device) 0 or 2
	int32_t (*GetLeaseIPAndTime)(int32_t, lease_info *);       // arg1(device) 0 or 2
	int32_t (*SetWirelessSsidAndPassPhrase)(wireless_info *);  // set wirelessSsid[0x20] and wirelessPassPhrase[0x41]
	int32_t (*GetWirelessSsidAndPassPhrase)(wireless_info *);  // get wirelessSsid[0x20] and wirelessPassPhrase[0x41]
} xsetting_F48C0548_class;


typedef struct xsetting_FFAF9B19_class_t  // 12
{
	//int32_t (*sub_45DEC0) ?
	//int32_t (*sub_45DE7C) ?
	//int32_t (*sub_45E168) ?
	//int32_t (*sub_45E124) ?
	//int32_t (*sub_45E0E0) ?
	//int32_t (*sub_45E09C) ?
	//int32_t (*sub_45E058) ?
	//int32_t (*sub_45E014) ?
	//int32_t (*sub_45DFD0) ?
	//int32_t (*sub_45DF8C) ?
	//int32_t (*sub_45DF58) ?
	//int32_t (*sub_45DF04) ?
} xsetting_FFAF9B19_class;


// class pointers (vsh/lib/libxsetting_export_stub.a)
extern xsetting_0AF1F161_class* xsetting_0AF1F161(void);
extern xsetting_154430FC_class* xsetting_154430FC(void);
extern xsetting_16A8A805_class* xsetting_16A8A805(void);
extern xsetting_1D6D60D4_class* xsetting_1D6D60D4(void);
extern xsetting_43F98936_class* xsetting_43F98936(void);
extern xsetting_4712F276_class* xsetting_4712F276(void);
extern xsetting_58560CA4_class* xsetting_58560CA4(void);
extern xsetting_5FB90B89_class* xsetting_5FB90B89(void);
extern xsetting_660ECC35_class* xsetting_660ECC35(void);
extern xsetting_69C19C7F_class* xsetting_69C19C7F(void);
extern xsetting_7125FEB5_class* xsetting_7125FEB5(void);
extern xsetting_7EDDAD29_class* xsetting_7EDDAD29(void);
extern xsetting_8B69F85A_class* xsetting_8B69F85A(void);
extern xsetting_9EE60B4E_class* xsetting_9EE60B4E(void);
extern xsetting_C1008335_class* xsetting_C1008335(void);
extern xsetting_CC56EB2D_class* xsetting_CC56EB2D(void);
extern xsetting_CE27E884_class* xsetting_CE27E884(void);
extern xsetting_D0261D72_class* xsetting_D0261D72(void);
extern xsetting_ED5B559F_class* xsetting_ED5B559F(void);
extern xsetting_F48C0548_class* xsetting_F48C0548(void);
extern xsetting_FFAF9B19_class* xsetting_FFAF9B19(void);


#endif // __XREGISTRY_H__
