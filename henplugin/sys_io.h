#ifndef __SYS_IO_H__
#define __SYS_IO_H__


//sys_io_8E06D13B  // ?
//sys_io_662BB2AD  // ?
//sys_io_5F81900C  // sys_config_unregister_service
//sys_io_78F058A2  // sys_config_register_service
//sys_io_F5D9D571  // sys_config_remove_service_listener
//sys_io_6AE10596  // sys_config_add_service_listener
//sys_io_6D367953  // sys_config_stop
//sys_io_068FCBC6  // sys_config_start

//sys_io_E5E139DD  // ? cellPad...

extern int32_t sys_io_8A00F264(uint32_t port_no, CellPadPeriphData *data);  // cellPadPeriphGetData()
#define PadPeriphGetData sys_io_8A00F264

extern int32_t sys_io_4CC9B68D(CellPadPeriphInfo *info);  // cellPadPeriphGetInfo()
#define PadPeriphGetInfo sys_io_4CC9B68D

extern int32_t sys_io_578E3C98(uint32_t port_no, uint32_t mode);  // cellPadSetPortSetting()
#define PadSetPortSetting sys_io_578E3C98

extern int32_t sys_io_A703A51D(CellPadInfo2 *info);  // cellPadGetInfo2()
#define PadGetInfo2 sys_io_A703A51D

extern int32_t sys_io_F65544EE(uint32_t port_no, CellPadActParam *param);  // cellPadSetActDirect()
#define PadSetActDirect sys_io_F65544EE

extern int32_t sys_io_6BC09C61(uint32_t port_no, uint32_t *device_type, CellPadData *data);  // cellPadGetDataExtra()
#define PadGetDataExtra sys_io_6BC09C61

extern int32_t sys_io_3733EA3C(void);  // ? cellPad...           // base ptr
//sys_io_DBF4C59C  // ? cellPad...
//sys_io_10DA8DCC  // ? cellPad...
//sys_io_5C016E79  // ? cellPad...

extern int32_t sys_io_8B8231E5(int32_t handle);  // cellPadLddGetPortNo()
#define PadLddGetPortNo sys_io_8B8231E5

//sys_io_73BB2D74  // ? cellPad...

extern int32_t sys_io_20A97BA2(void);  // cellPadLddRegisterController()
#define PadLddRegisterController sys_io_20A97BA2

extern int32_t sys_io_BAFD6409(int32_t handle, CellPadData *data);  // cellPadLddDataInsert()
#define PadLddDataInsert sys_io_BAFD6409

extern int32_t sys_io_E442FAA8(int32_t handle);  // cellPadLddUnregisterController()
#define PadLddUnregisterController sys_io_E442FAA8

//sys_io_7009B738  // ? cellPad...
//sys_io_78200559  // cellPadInfoSensorMode


extern int32_t sys_io_BE5BE3BA(void);  // cellPadSetSensorMode
extern int32_t sys_io_0E2DFAAD(void);  // cellPadInfoPressMode
extern int32_t sys_io_F83F8182(void);  // cellPadSetPressMode
//extern int32_t sys_io_13ED2BA4(void);  // ? cellPad...
extern int32_t sys_io_8B72CDA1(void);  // cellPadGetData
extern int32_t sys_io_3F797DFF(void);  // cellPadGetRawData
extern int32_t sys_io_3AAAD464(void);  // cellPadGetInfo
extern int32_t sys_io_0D5F2C14(void);  // cellPadClearBuf

extern int32_t sys_io_4D9B75D5(void);  // cellPadEnd()
#define PadEnd sys_io_4D9B75D5

extern int32_t sys_io_1CF98800(uint32_t max_connect);  // cellPadInit()
#define PadInit sys_io_1CF98800

extern int32_t sys_io_1F71ECBE(void);  // cellKbGetConfiguration
extern int32_t sys_io_3F72C56E(void);  // cellKbSetLEDStatus
extern int32_t sys_io_DEEFDFA7(void);  // cellKbSetReadMode
extern int32_t sys_io_A5F85E4D(void);  // cellKbSetCodeType
extern int32_t sys_io_4AB1FA77(void);  // cellKbCnvRawCode
extern int32_t sys_io_FF0A21B7(void);  // cellKbRead
extern int32_t sys_io_2F1774D5(void);  // cellKbGetInfo
extern int32_t sys_io_2073B7F6(void);  // cellKbClearBuf
extern int32_t sys_io_BFCE3285(void);  // cellKbEnd
extern int32_t sys_io_433F6EC0(void);  // cellKbInit


//extern int32_t sys_io_CCC785DF(void);  // ? cellMouse...
extern int32_t sys_io_2D16DA4F(void);  // cellMouseSetTabletMode
extern int32_t sys_io_4D0B3B1F(void);  // cellMouseInfoTabletMode
extern int32_t sys_io_21A62E9B(void);  // cellMouseGetTabletDataList
extern int32_t sys_io_6BD131F0(void);  // cellMouseGetDataList
extern int32_t sys_io_3138E632(void);  // cellMouseGetData
extern int32_t sys_io_A328CC35(void);  // cellMouseGetRawData
extern int32_t sys_io_5BAF30FB(void);  // cellMouseGetInfo
extern int32_t sys_io_3EF66B95(void);  // cellMouseClearBuf
extern int32_t sys_io_E10183CE(void);  // cellMouseEnd
extern int32_t sys_io_C9030138(void);  // cellMouseInit


#endif // __SYS_IO_H__ 
