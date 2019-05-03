#ifndef __LV2_HID_H__
#define __LV2_HID_H__

#include <stdint.h>
#include <sys/types.h>
#include <lv2/lv2.h>

LV2_EXPORT int hid_mgr_read_usb(uint8_t unk1, void *buf, uint32_t cmd, uint8_t unk2);
// Len is both in/out!
LV2_EXPORT int hid_mgr_read_bt(uint8_t unk1, void *buf, uint16_t *len, uint8_t unk2);

#endif /* __LV2_HID_H__ */

