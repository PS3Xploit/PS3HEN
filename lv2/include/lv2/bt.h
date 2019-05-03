#ifndef __LV2_BT_H__
#define __LV2_BT_H__

#include <stdint.h>
#include <sys/types.h>
#include <lv2/lv2.h>
#include <lv2/symbols.h>

LV2_EXPORT int bt_set_controller_info_internal(void *bt_work, void *buf, uint16_t vid, uint16_t pid);

static INLINE int bt_set_controller_info(void *buf, uint16_t vid, uint16_t pid)
{
	return bt_set_controller_info_internal((void *) *(uint64_t *)MKA(TOC + bt_rtoc_entry_1), buf, vid, pid);
}

#endif /* __LV2_BT_H__ */

