#ifndef __LV1_PATCH_H__
#define __LV1_PATCH_H__

#include <lv1/lv1.h>

u32 lv1_test_peekpoke();

u8 lv1_peekb(u64 addr);
u16 lv1_peekh(u64 addr);
u32 lv1_peekw(u64 addr);
u64 lv1_peekd(u64 addr);

void lv1_pokeb(u64 addr, u8 value);
void lv1_pokeh(u64 addr, u16 value);
void lv1_pokew(u64 addr, u32 value);
void lv1_poked(u64 addr, u64 value);

#endif
