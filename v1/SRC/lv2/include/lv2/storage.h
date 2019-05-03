#ifndef __LV2_STORAGE_H__
#define __LV2_STORAGE_H__

#include <stdint.h>
#include <lv2/lv2.h>
#include <lv1/lv1.h>

/* Devices */
#define ATA_HDD					0x101000000000007
#define BDVD_DRIVE				0x101000000000006
#define PATA0_HDD_DRIVE			0x101000000000008
#define PATA0_BDVD_DRIVE		BDVD_DRIVE
#define PATA1_HDD_DRIVE			ATA_HDD
#define BUILTIN_FLASH			0x100000000000001
#define NAND_FLASH				BUILTIN_FLASH
#define NOR_FLASH				0x100000000000004
#define MEMORY_STICK			0x103000000000010
#define SD_CARD					0x103000100000010
#define COMPACT_FLASH			0x103000200000010
#define USB_MASS_STORAGE_1(n)	(0x10300000000000A+n) 	  /* For 0-5 */
#define USB_MASS_STORAGE_2(n)	(0x10300000000001F+(n-6)) /* For 6-127 */

#define	HDD_PARTITION(n)		(ATA_HDD | (n<<32))
#define FLASH_PARTITION(n)		(BUILTIN_FLASH | (n<<32))

/* Commands for storage_send_device_command */
#define STORAGE_COMMAND_NATIVE			0x01
#define STORAGE_COMMAND_GET_DEVICE_SIZE	0x10
#define STORAGE_COMMAND_GET_DEVICE_TYPE	0x11

/* Device types returned by STORAGE_COMMAND_GET_DEVICE_TYPE and some scsi commands */
#define DEVICE_TYPE_PS3_DVD		0xFF70
#define DEVICE_TYPE_PS3_BD		0xFF71
#define DEVICE_TYPE_PS2_CD		0xFF60
#define DEVICE_TYPE_PS2_DVD		0xFF61
#define DEVICE_TYPE_PSX_CD		0xFF50
#define DEVICE_TYPE_BDROM		0x40
#define DEVICE_TYPE_BDMR_SR		0x41 /* Sequential record */
#define DEVICE_TYPE_BDMR_RR 	0x42 /* Random record */
#define DEVICE_TYPE_BDMRE		0x43
#define DEVICE_TYPE_DVD			0x10 /* DVD-ROM, DVD+-R, DVD+-RW etc, they are differenced by booktype field in some scsi command */
#define DEVICE_TYPE_CD			0x08 /* CD-ROM, CD-DA, CD-R, CD-RW, etc, they are differenced somehow with scsi commands */


typedef uint32_t device_handle_t; // Kernel
typedef uint32_t sys_device_handle_t; // User

typedef struct
{
	char *label; 	 		// 0
	uint32_t unk_08; 		// 8                                                    0xB8   0x90
	uint32_t unk_0C; 		// 0x0C                                                 0xBC   0x94
	uint64_t unk_10;  		// 0x10  not copied in sys_storage_get_device_info      0xC0   unused
	uint64_t sector_count; 	// 0x18													0xC8   0x98
	uint32_t sector_size; 	// 0x20                                                 0xD0   0xA0
	uint32_t unk_24; 		// 0x24 not copied in sys_storage_get_device_info		0xD4   unused
	uint32_t unk_28; 		// 0x28 not copied in sys_storage_get_device_info		0xD8   unused
	uint32_t unk_2C; 		// 0x2C not copied in sys_storage_get_device_info		0xDC   unused
	uint32_t unk_30; 		// 0x30													0xE0   0xA4
	uint32_t unk_34; 		// 0x34 not copied in sys_storage_get_device_info		0xE4   unused
	uint64_t unk_38; 		// 0x38 not copied in sys_storage_get_device_info		0xE8   unused
	uint8_t unk_40[8]; 		// 0x40													0xF0   0xA8
	
} __attribute__((packed)) device_info_t;

typedef struct
{
	uint32_t inlen;
	uint32_t unk1; 
	uint32_t outlen;
	uint32_t unk2;
	uint32_t unk3;
} __attribute__((packed)) StorageCmdScsiData;

LV2_EXPORT int storage_get_device_info(uint64_t device_id, device_info_t *device_info);

LV2_EXPORT int storage_get_device_config(int *unk, int *unk2);

// storage_open(0x0101000000000006, 0, &device_handle, 0); // dev_bdvd
LV2_EXPORT int storage_open(uint64_t device_id, uint64_t unk, device_handle_t *device_handle, uint64_t unk2);
LV2_EXPORT int storage_close(device_handle_t device_handle);
// storage_read(3, 0, 0x220, 0x10, buf, &nread, 4); // flash?
// storage_read(0x13, 0, 0x030761E0, 0x80, buf, &nread, 0); // hdd 
// storage_read(0x24, 0, 0x3C01, 0x20, buf, &nread, 4); // usb. Raw sector, not partition one.
// storage_read(0x41, 0, 0x10, 0x10, buf, &nread, 0); // dev_bdvd
LV2_EXPORT int storage_read(device_handle_t device_handle, uint64_t unk, uint64_t start_sector, uint32_t sector_count, void *buf, uint32_t *nread, uint64_t flags);
LV2_EXPORT int storage_write(device_handle_t device_handle, uint64_t unk, uint64_t start_sector, uint32_t sector_count, void *buf, uint32_t *nwrite, uint64_t flags);


/* get device type?
  command = 0x11
  indata  = 0x01010071   (also works with indata 00000000)
  outdata = 0x0000FF71   PS3 BD game
  outdata = 0x00000040   BD-ROM
  outdata = 0x00000041   BD-R
  outdata = 0x0000FF61   PS2 DVD game
  outdata = 0x00000010   DVD (any except ps2)
  outdata = 0x00000008   CD (any except ps1?)
*/

LV2_EXPORT int storage_send_device_command(device_handle_t device_handle, unsigned int command, void *indata, uint64_t inlen, void *outdata, uint64_t outlen, uint64_t *unkret);

LV2_EXPORT int storage_map_io_memory(uint64_t device, void *buf, uint64_t size);
LV2_EXPORT int storage_unmap_io_memory(uint64_t device, void *buf);

static INLINE uint64_t storage_get_flash_device(void)
{
	u64 value, v2;
	
	lv1_get_repository_node_value(PS3_LPAR_ID_PME, FIELD_FIRST("sys", 0), FIELD("flash", 0), FIELD("ext", 0), 0, &value, &v2);
	
	if (value&1)
		return NAND_FLASH;
	
	return NOR_FLASH;
}


#endif /* __LV2_STORAGE_H__ */

