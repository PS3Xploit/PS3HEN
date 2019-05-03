#ifndef __LV1_PATCH_H__
#define __LV1_PATCH_H__

#include <lv1/lv1.h>

#ifdef LV2

#include <lv2/lv2.h>
#include <lv2/libc.h>

#define HV_BASE	0x8000000014000000ULL  
#define HV_SIZE	0x01000000

#endif

#ifdef USE_LV1_PEEK_POKE

static INLINE u8 lv1_peekb(u64 addr)
{
	u64 val;	
	lv1_cfw_peek(addr, &val);	
	return (val>>56);
}

static INLINE u16 lv1_peekh(u64 addr)
{
	u64 val;	
	lv1_cfw_peek(addr, &val);	
	return (val>>48);
}

static INLINE u32 lv1_peekw(u64 addr)
{
	u64 val;	
	lv1_cfw_peek(addr, &val);	
	return (val>>32);
}

static INLINE u64 lv1_peekd(u64 addr)
{
	u64 val;	
	lv1_cfw_peek(addr, &val);	
	return val;
}

static INLINE void lv1_pokeb(u64 addr, u8 value)
{
	u64 val;
	
	lv1_cfw_peek(addr, &val);
	val = (val&0xFFFFFFFFFFFFFFULL) | ((u64)value << 56);
	
	lv1_cfw_poke(addr, val);
}

static INLINE void lv1_pokeh(u64 addr, u16 value)
{
	u64 val;
	
	lv1_cfw_peek(addr, &val);
	val = (val&0xFFFFFFFFFFFFULL) | ((u64)value << 48);
	
	lv1_cfw_poke(addr, val);
}

static INLINE void lv1_pokew(u64 addr, u32 value)
{
	u64 val;
	
	lv1_cfw_peek(addr, &val);
	val = (val&0xFFFFFFFFULL) | ((u64)value << 32);
	
	lv1_cfw_poke(addr, val);
}

static INLINE void lv1_poked(u64 addr, u64 value)
{
	lv1_cfw_poke(addr, value);
}

#else

static INLINE u8 lv1_peekb(u64 addr)
{
	return *(u8 *)(HV_BASE+addr);
}

static INLINE u16 lv1_peekh(u64 addr)
{
	return *(u16 *)(HV_BASE+addr);
}

static INLINE u32 lv1_peekw(u64 addr)
{
	return *(u32 *)(HV_BASE+addr);
}

static INLINE u64 lv1_peekd(u64 addr)
{
	return *(u64 *)(HV_BASE+addr);
}

static INLINE void lv1_pokeb(u64 addr, u8 value)
{
	*(u8 *)(HV_BASE+addr) = value;
}

static INLINE void lv1_pokeh(u64 addr, u16 value)
{
	*(u16 *)(HV_BASE+addr) = value;
}

static INLINE void lv1_pokew(u64 addr, u32 value)
{
	*(u32 *)(HV_BASE+addr) = value;
}

static INLINE void lv1_poked(u64 addr, u64 value)
{
	*(u64 *)(HV_BASE+addr) = value;
}

#endif

#endif

