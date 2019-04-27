#ifndef __LV2_PAD_H__
#define __LV2_PAD_H__

#include <stdint.h>
#include <sys/types.h>
#include <lv2/lv2.h>

// Note values in this file are different than user mode functions

#define PAD_BTN_OFFSET_DIGITAL		1
#define PAD_CTRL_LEFT				(1 << 15)
#define PAD_CTRL_DOWN				(1 << 14)
#define PAD_CTRL_RIGHT				(1 << 13)
#define PAD_CTRL_UP					(1 << 12)
#define PAD_CTRL_START				(1 << 11)
#define PAD_CTRL_R3					(1 << 10)
#define PAD_CTRL_L3					(1 << 9)
#define PAD_CTRL_SELECT				(1 << 8)
#define PAD_CTRL_SQUARE				(1 << 7)
#define PAD_CTRL_CROSS				(1 << 6)
#define PAD_CTRL_CIRCLE				(1 << 5)
#define PAD_CTRL_TRIANGLE			(1 << 4)
#define PAD_CTRL_R1					(1 << 3)
#define PAD_CTRL_L1					(1 << 2)
#define PAD_CTRL_R2					(1 << 1)
#define PAD_CTRL_L2					(1 << 0)

#define PAD_BTN_OFFSET_PS			2
#define PAD_CTRL_PS					(1 << 8)

#define PAD_BTN_OFFSET_ANALOG_LEFT	3
#define PAD_BTN_OFFSET_ANALOG_RIGHT	4
#define PAD_CTRL_ANALOG_X(x)		(x >> 8)
#define PAD_CTRL_ANALOG_Y(x)		(x & 0xFF)


// TODO: add rest


typedef struct _pad_data
{
	uint16_t button[64];
} __attribute__((packed)) pad_data;

// Return value: the length of data or < 0 -> error
// if (pad_get_data(&data) > 0)
// 	if (data.button[PAD_BTN_OFFSET_DIGITAL] & PAD_CTRL_TRIANGLE)
//		triangle pressed;
int pad_get_data(pad_data *data);

#endif /* __LV2_PAD_H__ */

