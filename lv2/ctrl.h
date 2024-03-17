// Created and improved by Evilnat

#ifndef __LV2_CTRL_H__
#define __LV2_CTRL_H__

#include <stdint.h>
#include <sys/types.h>
#include <lv2/lv2.h>
#include <lv2/symbols.h>

#define SYSM_OBJ_OFFSET 		0x8000000000347E30ULL

#define SINGLE_BEEP 			0x6
#define DOUBLE_BEEP 			0x36
#define TRIPLE_BEEP 			0x1B6
#define CONTINUOUS_BEEP			0xFFFF

uint64_t *sysm_obj;

LV2_EXPORT int sm_ring_buzzer_internal(uint64_t *sysm_obj, uint16_t packet, uint8_t arg3, uint8_t beep);

LV2_EXPORT int sm_get_temperature_internal(uint64_t *sysm_obj, int id, uint32_t *temp, int time_out);
LV2_EXPORT int sm_get_fan_policy_internal(uint64_t *sysm_obj, uint8_t id, uint8_t *st, uint8_t *mode, uint8_t *speed, uint8_t *unk, int time_out);
LV2_EXPORT int sm_set_fan_policy_internal(uint64_t *sysm_obj, uint8_t unk, uint8_t mode, uint8_t speed);


static INLINE void sm_ring_buzzer(uint16_t beep)
{
	memcpy(&sysm_obj, (void *)SYSM_OBJ_OFFSET, 8); 
	sm_ring_buzzer_internal(sysm_obj, 0x29, 0xA, beep);
}

static INLINE void sm_get_temperature(int id, uint32_t *temp)
{
	memcpy(&sysm_obj, (void *)SYSM_OBJ_OFFSET, 8); 
	sm_get_temperature_internal(sysm_obj, id, temp, 1000000);
	*temp >>= 24; // return °C
}

static INLINE void sm_get_fan_policy(uint8_t id, uint8_t *st, uint8_t *mode, uint8_t *speed, uint8_t *unk)
{
	memcpy(&sysm_obj, (void *)SYSM_OBJ_OFFSET, 8); 
	sm_get_fan_policy_internal(sysm_obj, id, st, mode, speed, unk, 1000000);
}

static INLINE void sm_set_fan_policy(uint8_t unk, uint8_t mode, uint8_t speed)
{
	memcpy(&sysm_obj, (void *)SYSM_OBJ_OFFSET, 8); 
	sm_set_fan_policy_internal(sysm_obj, unk, mode, speed);
}

#endif /* __LV2_CTRL_H__ */
