#include <lv1/lv1.h>

u64 lv1_badwdsd_peek64(u64 addr)
{
    asm volatile("mflr 0");
    asm volatile("addi 1, 1, -16");
    asm volatile("std 0, 0(1)");

    asm volatile("addi 1, 1, -128");
    asm volatile("li 11, 34");
    asm volatile("sc 1");
    asm volatile("addi 1, 1, 128");

    asm volatile("ld 0, 0(1)");
    asm volatile("addi 1, 1, 16");
    asm volatile("mtlr 0");

    asm volatile("blr");
}

u32 lv1_test_peekpoke()
{
    return (lv1_badwdsd_poke64(0, 0) == 0) ? 1 : 0;
}

#if 0

u8 lv1_peekb(u64 addr)
{
    if (!lv1_test_peekpoke())
        return 0;

    u64 val;
    val = lv1_badwdsd_peek64(addr);
    return (val >> 56);
}

u16 lv1_peekh(u64 addr)
{
    if (!lv1_test_peekpoke())
        return 0;

    u64 val;
    val = lv1_badwdsd_peek64(addr);
    return (val >> 48);
}

u32 lv1_peekw(u64 addr)
{
    if (!lv1_test_peekpoke())
        return 0;

    u32 val;
    val = (u32)lv1_badwdsd_peek32(addr);
    return val;
}

#endif

u64 lv1_peekd(u64 addr)
{
    if (!lv1_test_peekpoke())
        return 0;

    u64 val;
    val = lv1_badwdsd_peek64(addr);
    return val;
}

#if 0

void lv1_pokeb(u64 addr, u8 value)
{
    if (!lv1_test_peekpoke())
        return;

    u64 val;

    val = lv1_badwdsd_peek64(addr);
    val = (val & 0xFFFFFFFFFFFFFFULL) | ((u64)value << 56);

    lv1_badwdsd_poke64(addr, val);
}

void lv1_pokeh(u64 addr, u16 value)
{
    if (!lv1_test_peekpoke())
        return;

    u64 val;

    val = lv1_badwdsd_peek64(addr);
    val = (val & 0xFFFFFFFFFFFFULL) | ((u64)value << 48);

    lv1_badwdsd_poke64(addr, val);
}

void lv1_pokew(u64 addr, u32 value)
{
    if (!lv1_test_peekpoke())
        return;
    
    lv1_badwdsd_poke32(addr, value);
}

#endif

void lv1_poked(u64 addr, u64 value)
{
    if (!lv1_test_peekpoke())
        return;

    lv1_badwdsd_poke64(addr, value);
}
