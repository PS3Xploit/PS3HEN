#ifndef __LV2_PATCH_H__
#define __LV2_PATCH_H__

#include <lv2/lv2.h>
#include <lv2/symbols.h>

#define R0 		0
#define R1		1
#define R2		2
#define R3		3
#define R4		4
#define R5		5
#define R6		6
#define R7		7
#define R8		8
#define R9		9
#define R10		10
#define R11		11
#define R12		12
#define R13		13
#define R14		14
#define R15		15
#define R16		16
#define R17		17
#define R18		18
#define R19		19
#define R20		20
#define R21		21
#define R22		22
#define R23		23
#define R24		24
#define R25		25
#define R26		26
#define R27		27
#define R28		28
#define R29		29
#define R30		30
#define R31		31
#define SP		1
#define RTOC	2

#define MAKE_JUMP(addr, to) *(uint32_t *)(addr) = (0x12 << 26) | ((((to-(uint64_t)(addr))>>2)&0xFFFFFF) << 2)
#define MAKE_CALL(addr, to) *(uint32_t *)(addr) = (0x12 << 26) | ((((to-(uint64_t)(addr))>>2)&0xffffff) << 2) | 1

#define MAKE_JUMP_VALUE(addr, to) ((0x12 << 26) | ((((to-(uint64_t)(addr))>>2)&0xFFFFFF) << 2))
#define MAKE_CALL_VALUE(addr, to) ((0x12 << 26) | ((((to-(uint64_t)(addr))>>2)&0xFFFFFF) << 2)) | 1

#define B(A)	MAKE_JUMP_VALUE(0, A)
#define BL(A)	MAKE_CALL_VALUE(0, A)

#define BLR		0x4E800020
#define BLRL	0x4E800021
#define BCTR	0x4E800420
#define BCTRL	0x4E800421
#define NOP		0x60000000

#define SC		0x44000002
#define HVSC	0x44000022

#define MFSPR(SPR, RS) ((31 << 26) | (RS << 21) | (SPR << 16) | (339 << 1))
#define MFLR(RS) MFSPR(8, RS)
#define MFCTR(RS) MFSPR(9, RS)

#define MTSPR(SPR, RS) ((31 << 26) | (RS << 21) | (SPR << 16) | (467 << 1))
#define MTLR(RS) MTSPR(8, RS)
#define MTCTR(RS) MTSPR(9, RS)

#define OR(RA, RS, RB) (31 << 26) | (RS << 21) | (RA << 16) | (RB << 11) | (444 << 1)
#define ORI(RA, RS, UI) (24 << 26) | (RS << 21) | (RA << 16) | (UI&0xFFFF)
#define ORIS(RA, RS, UI) (25 << 26) | (RS << 21) | (RA << 16) | (UI&0xFFFF)
#define MR(RS, RA) OR(RS, RA, RA)

#define ADDI(RT, RA, SI) (14 << 26) | (RT << 21) | (RA << 16) | (SI&0xFFFF)
#define LI(RA, UI) ADDI(RA, 0, UI)

#define ADDIS(RT, RA, SI) (15 << 26) | (RT << 21) | (RA << 16) | (SI&0xFFFF)
#define LIS(RA, UI) ADDIS(RA, 0, UI)

#define LD(RT, DS, RA) (58 << 26) | (RT << 21) | (RA << 16) | ((DS >> 2) << 2)
#define STD(RS, DS, RA) (62 << 26) | (RS << 21) | (RA << 16) | ((DS >> 2) << 2)
#define STDU(RS, DS, RA) ((62 << 26) | (RS << 21) | (RA << 16) | ((DS >> 2) << 2) | 1)

#define LWZ(RT, D, RA) (32 << 26) | (RT << 21) | (RA << 16) | D
#define STW(RS, D, RA) (36 << 26) | (RS << 21) | (RA << 16) | D

/* DO NOT CHANGE */
#define DO_POSTCALL			0xFFFFC561
#define DO_POSTCALL_64		0xFFFFFFFFFFFFC561

#define LV2_PATCHED_FUNCTION	LV2_CONTEXT

#define LV2_HOOKED_FUNCTION(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	nop\n" \
"	nop\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_0(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	nop\n" \
"	nop\n" \
"	std 2, 40(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_1(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	nop\n" \
"	nop\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -20\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_2(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	nop\n" \
"	nop\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -24\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_3(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	nop\n" \
"	nop\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -28\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_4(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	nop\n" \
"	nop\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -32\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_5(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	nop\n" \
"	nop\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -36\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_6(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	std 8, 88(1)\n" \
"	nop\n" \
"	nop\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	ld 8, 88(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -40\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_7(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	std 8, 88(1)\n" \
"	std 9, 96(1)\n" \
"	nop\n" \
"	nop\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	ld 8, 88(1)\n" \
"	ld 9, 96(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -44\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_8(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	std 8, 88(1)\n" \
"	std 9, 96(1)\n" \
"	std 10, 104(1)\n" \
"	nop\n" \
"	nop\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	ld 8, 88(1)\n" \
"	ld 9, 96(1)\n" \
"	ld 10, 104(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -48\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_0(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	nop\n" \
"	nop\n" \
"	cmpwi 3, 0\n" \
"	bne 1f\n" \
"	std 2, 40(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"1:\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_1(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	nop\n" \
"	nop\n" \
"	cmpwi 3, 0\n" \
"	bne 1f\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -20\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"1:\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_2(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	nop\n" \
"	nop\n" \
"	cmpwi 3, 0\n" \
"	bne 1f\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -24\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"1:\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_3(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	nop\n" \
"	nop\n" \
"	cmpwi 3, 0\n" \
"	bne 1f\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -28\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"1:\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_4(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	nop\n" \
"	nop\n" \
"	cmpwi 3, 0\n" \
"	bne 1f\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -32\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"1:\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_5(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	nop\n" \
"	nop\n" \
"	cmpwi 3, 0\n" \
"	bne 1f\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -36\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"1:\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_6(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	std 8, 88(1)\n" \
"	nop\n" \
"	nop\n" \
"	cmpwi 3, 0\n" \
"	bne 1f\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	ld 8, 88(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -40\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"1:\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_7(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	std 8, 88(1)\n" \
"	std 9, 96(1)\n" \
"	nop\n" \
"	nop\n" \
"	cmpwi 3, 0\n" \
"	bne 1f\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	ld 8, 88(1)\n" \
"	ld 9, 96(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -44\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"1:\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_8(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	std 8, 88(1)\n" \
"	std 9, 96(1)\n" \
"	std 10, 104(1)\n" \
"	nop\n" \
"	nop\n" \
"	cmpwi 3, 0\n" \
"	bne 1f\n" \
"	std 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	ld 8, 88(1)\n" \
"	ld 9, 96(1)\n" \
"	ld 10, 104(1)\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -48\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"1:\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_POSTCALL_0(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_POSTCALL_1(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_POSTCALL_2(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_POSTCALL_3(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_POSTCALL_4(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_POSTCALL_5(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_POSTCALL_6(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	std 8, 88(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	ld 8, 88(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_POSTCALL_7(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	std 8, 88(1)\n" \
"	std 9, 96(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	ld 8, 88(1)\n" \
"	ld 9, 96(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_POSTCALL_8(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	std 8, 88(1)\n" \
"	std 9, 96(1)\n" \
"	std 10, 104(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	ld 8, 88(1)\n" \
"	ld 9, 96(1)\n" \
"	ld 10, 104(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_COND_POSTCALL_0(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	extsw 4, 3\n" \
"	cmpwi 4, -15007\n" \
"	bne +12\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_COND_POSTCALL_1(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	extsw 4, 3\n" \
"	cmpwi 4, -15007\n" \
"	bne +16\n" \
"	ld 3, 48(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_COND_POSTCALL_2(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	extsw 4, 3\n" \
"	cmpwi 4, -15007\n" \
"	bne +20\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_COND_POSTCALL_3(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	extsw 4, 3\n" \
"	cmpwi 4, -15007\n" \
"	bne +24\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_COND_POSTCALL_4(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	extsw 4, 3\n" \
"	cmpwi 4, -15007\n" \
"	bne +28\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_COND_POSTCALL_5(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	extsw 4, 3\n" \
"	cmpwi 4, -15007\n" \
"	bne +32\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_COND_POSTCALL_6(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	std 8, 88(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	extsw 4, 3\n" \
"	cmpwi 4, -15007\n" \
"	bne +36\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	ld 8, 88(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_COND_POSTCALL_7(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	std 8, 88(1)\n" \
"	std 9, 96(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	extsw 4, 3\n" \
"	cmpwi 4, -15007\n" \
"	bne +40\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	ld 8, 88(1)\n" \
"	ld 9, 96(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_HOOKED_FUNCTION_COND_POSTCALL_8(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	std 3, 48(1)\n" \
"	std 4, 56(1)\n" \
"	std 5, 64(1)\n" \
"	std 6, 72(1)\n" \
"	std 7, 80(1)\n" \
"	std 8, 88(1)\n" \
"	std 9, 96(1)\n" \
"	std 10, 104(1)\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	extsw 4, 3\n" \
"	cmpwi 4, -15007\n" \
"	bne +44\n" \
"	ld 3, 48(1)\n" \
"	ld 4, 56(1)\n" \
"	ld 5, 64(1)\n" \
"	ld 6, 72(1)\n" \
"	ld 7, 80(1)\n" \
"	ld 8, 88(1)\n" \
"	ld 9, 96(1)\n" \
"	ld 10, 104(1)\n" \
"	nop\n" \
"	nop\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_MULTIPLE_RETURN_2(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 4,  8(3)\n" \
"	ld 3, 0(3)\n" \
"	ld 2, 40(1)\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

#define LV2_MULTIPLE_RETURN_7(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -16\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 4,  8(3)\n" \
"	ld 5, 16(3)\n" \
"	ld 6, 24(3)\n" \
"	ld 7, 32(3)\n" \
"	ld 8, 40(3)\n" \
"	ld 9, 48(3)\n" \
"	ld 3, 0(3)\n" \
"	ld 2, 40(1)\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

typedef struct function_descriptor 
{
	void	*addr;
	void    *toc;	
} f_desc_t;

void 	clear_icache(void *addr, uint64_t size);
void 	*get_call_address(int level);
void	*get_patched_func_call_address(void);
void	*get_patched_func_call_address2(void);
uint64_t read_patched_func_saved_register(const int reg);
uint64_t read_patched_func_saved_register2(const int reg);
void	 write_patched_func_saved_register(const int reg, uint64_t val);
void	 write_patched_func_saved_register2(const int reg, uint64_t val);

static INLINE uint64_t get_patched_func_param(const int n)
{
	return read_patched_func_saved_register(n+2);
}

static INLINE uint64_t get_patched_func_param2(const int n)
{
	return read_patched_func_saved_register2(n+2);
}

static INLINE void set_patched_func_param(const int n, uint64_t val)
{
	write_patched_func_saved_register(n+2, val);
}

static INLINE void set_patched_func_param2(const int n, uint64_t val)
{
	write_patched_func_saved_register2(n+2, val);
}

static INLINE void *get_function_ptr(uint64_t addr, f_desc_t *f)
{
	f->addr = (void *)MKA(addr);
	f->toc = (void *)MKA(TOC);
	return f;
}

static INLINE void patch_call(uint64_t call_addr, void *newfunc)
{
	f_desc_t *f = (f_desc_t *)newfunc;
	MAKE_CALL(MKA(call_addr), (uint64_t)f->addr);
	clear_icache((void *)MKA(call_addr), 4);
}

static INLINE void change_function(uint64_t func_addr, void *newfunc)
{
	f_desc_t *f = (f_desc_t *)newfunc;
	MAKE_JUMP(MKA(func_addr), (uint64_t)f->addr);
	clear_icache((void *)MKA(func_addr), 4);
}

static INLINE void patch_jump(uint64_t jump_addr, void *newfunc) 
{ 
	change_function(jump_addr, newfunc); 
}

static INLINE void *hook_function(uint64_t func_addr, void *newfunc, f_desc_t *f)
{
	f_desc_t *g = (f_desc_t *)newfunc;
	uint32_t inst = *(uint32_t *)MKA(func_addr);
	uint32_t *orig_call = &((uint32_t *)g->addr)[19]; // WARNING: relies on HOOKED_FUNCTION not being changed
	
	orig_call[0] = inst;
	MAKE_JUMP(&orig_call[1], MKA(func_addr+4));
	clear_icache(orig_call, 8);
	change_function(func_addr, newfunc);
	f->addr = orig_call;
	f->toc = (void *)MKA(TOC);
	return f;
}

static INLINE void unhook_function(uint64_t func_addr, void *newfunc)
{
	f_desc_t *f = (f_desc_t *)newfunc;
	uint32_t *orig_call = &((uint32_t *)f->addr)[19]; // WARNING: relies on HOOKED_FUNCTION not being changed
	
	*(uint32_t *)MKA(func_addr) = orig_call[0];
	clear_icache((void *)MKA(func_addr), 4);
}

static INLINE void hook_function_with_precall(uint64_t func_addr, void *newfunc, unsigned int nparams)
{
	if (nparams > 8)
		nparams = 8;
	
	f_desc_t *f = (f_desc_t *)newfunc;
	uint32_t inst = *(uint32_t *)MKA(func_addr);
	uint32_t *orig_call = &((uint32_t *)f->addr)[2+nparams]; // WARNING: relies on HOOKED_FUNCTION_PRECALL_N not being changed	
	
	orig_call[0] = inst;
	MAKE_CALL(&orig_call[1], MKA(func_addr+4));
	clear_icache(orig_call, 8);
	change_function(func_addr, newfunc);
}

static INLINE void unhook_function_with_precall(uint64_t func_addr, void *newfunc, unsigned int nparams)
{
	if (nparams > 8)
		nparams = 8;
	
	f_desc_t *f = (f_desc_t *)newfunc;
	uint32_t *orig_call = &((uint32_t *)f->addr)[2+nparams]; // WARNING: relies on HOOKED_FUNCTION_PRECALL_N not being changed	
	
	*(uint32_t *)MKA(func_addr) = orig_call[0];
	clear_icache((void *)MKA(func_addr), 4);
}

static INLINE void hook_function_on_precall_success(uint64_t func_addr, void *newfunc, unsigned int nparams)
{
	hook_function_with_precall(func_addr, newfunc, nparams);
}

static INLINE void unhook_function_on_precall_success(uint64_t func_addr, void *newfunc, unsigned int nparams)
{
	unhook_function_with_precall(func_addr, newfunc, nparams);
}

static INLINE void hook_function_with_postcall(uint64_t func_addr, void *newfunc, unsigned int nparams)
{
	if (nparams > 8)
		nparams = 8;
	
	f_desc_t *f = (f_desc_t *)newfunc;
	uint32_t inst = *(uint32_t *)MKA(func_addr);
	uint32_t *orig_call = &((uint32_t *)f->addr)[16+(nparams*2)]; // WARNING: relies on HOOKED_FUNCTION_POST_CALL_N not being changed	
	
	orig_call[0] = inst;
	MAKE_CALL(&orig_call[1], MKA(func_addr+4));
	clear_icache(orig_call, 8);
	change_function(func_addr, newfunc);
}

static INLINE void unhook_function_with_postcall(uint64_t func_addr, void *newfunc, unsigned int nparams)
{
	if (nparams > 8)
		nparams = 8;
	
	f_desc_t *f = (f_desc_t *)newfunc;
	uint32_t *orig_call = &((uint32_t *)f->addr)[16+(nparams*2)]; // WARNING: relies on HOOKED_FUNCTION_POST_CALL_N not being changed	
	
	*(uint32_t *)MKA(func_addr) = orig_call[0];
	clear_icache((void *)MKA(func_addr), 4);	
}

static INLINE void hook_function_with_cond_postcall(uint64_t func_addr, void *newfunc, unsigned int nparams)
{
	if (nparams > 8)
		nparams = 8;
	
	f_desc_t *f = (f_desc_t *)newfunc;
	uint32_t inst = *(uint32_t *)MKA(func_addr);
	uint32_t *orig_call = &((uint32_t *)f->addr)[19+(nparams*2)]; // WARNING: relies on HOOKED_FUNCTION_COND_POST_CALL_N not being changed	
	
	orig_call[0] = inst;
	MAKE_CALL(&orig_call[1], MKA(func_addr+4));
	clear_icache(orig_call, 8);
	change_function(func_addr, newfunc);
}

static INLINE void unhook_function_with_cond_postcall(uint64_t func_addr, void *newfunc, unsigned int nparams)
{
	if (nparams > 8)
		nparams = 8;
	
	f_desc_t *f = (f_desc_t *)newfunc;
	uint32_t *orig_call = &((uint32_t *)f->addr)[19+(nparams*2)]; // WARNING: relies on HOOKED_FUNCTION_COND_POST_CALL_N not being changed	
	
	*(uint32_t *)MKA(func_addr) = orig_call[0];
	clear_icache((void *)MKA(func_addr), 4);
}

static INLINE void set_postcall_function(uint64_t postcall_func_addr, void *func, unsigned int nparams)
{	
	if (nparams > 8)
		nparams = 8;
	
	f_desc_t *f = (f_desc_t *)func;
	uint32_t *orig_call = &((uint32_t *)f->addr)[16+(nparams*2)]; // WARNING: relies on HOOKED_FUNCTION_POST_CALL_N not being changed	
	
	MAKE_CALL(&orig_call[1], MKA(postcall_func_addr+4));
	clear_icache(&orig_call[1], 4);	
}

/* Only to call pre/postcall hooked functions, not normal hooked or patched */
uint64_t call_hooked_function(void *func, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7);

#define call_hooked_function_0(func) call_hooked_function(func, 0, 0, 0, 0, 0, 0, 0)
#define call_hooked_function_1(func, a1) call_hooked_function(func, a1, 0, 0, 0, 0, 0, 0)
#define call_hooked_function_2(func, a1, a2) call_hooked_function(func, a1, a2, 0, 0, 0, 0, 0)
#define call_hooked_function_3(func, a1, a2, a3) call_hooked_function(func, a1, a2, a3, 0, 0, 0, 0)
#define call_hooked_function_4(func, a1, a2, a3, a4) call_hooked_function(func, a1, a2, a3, a4, 0, 0, 0)
#define call_hooked_function_5(func, a1, a2, a3, a4, a5) call_hooked_function(func, a1, a2, a3, a4, a5, 0, 0)
#define call_hooked_function_6(func, a1, a2, a3, a4, a5, a6) call_hooked_function(func, a1, a2, a3, a4, a5, a6, 0)
#define call_hooked_function_7(func, a1, a2, a3, a4, a5, a6, a7) call_hooked_function(func, a1, a2, a3, a4, a5, a6, a7)


#endif /* __LV2_PATCH_H__ */

