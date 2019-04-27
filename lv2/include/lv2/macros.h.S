#ifndef __MACRO_H_S__
#define __MACRO_H_S__

#include "symbols.h"

#define MEM_BASE(dest) \
	li dest, 1; \
	rldicr dest, dest, 63, 0;
	
#define LOAD_LABEL(dest, source, address) \
	oris	dest, source, (address)@h; \
	ori	dest, dest, (address)@l;
	
#define LV2_FUNCTION(name, address) \
	.section .name.text, "ax"; \
	.global name; \
	.align 3; \
	name:; \
	.quad	0x8000000000000000 | address, 0x8000000000000000 | TOC; \
	.previous; 

	
#define LV2_FUNCTION2(name, address) \
	.section .name.text, "ax"; \
	.global name; \
	name:; \
	stdu	%sp, -0x80(%sp); \
	mflr	%r0; \
	std	%r0, 0x90(%sp); \
	std	%rtoc, 0x28(%sp); \
	MEM_BASE(%r0); \
	LOAD_LABEL(%rtoc, %r0, address); \
	mtctr	%rtoc; \
	LOAD_LABEL(%rtoc, %r0, TOC); \
	bctrl; \
	ld	%rtoc, 0x28(%sp); \
	ld	%r0, 0x90(%sp); \
	addi	%sp, %sp, 0x80; \
	mtlr	%r0; \
	blr; \
	.previous;
	
#define LV2_FUNCTION3(name, address) \
	.section .name.text, "ax"; \
	.global name; \
	name:; \
	std	%rtoc, 0x28(%sp); \
	stdu	%sp, -0x70(%sp); \
	mflr	%r0; \
	std	%r0, 0x80(%sp); \
	MEM_BASE(%r0); \
	LOAD_LABEL(%rtoc, %r0, address); \
	mtctr	%rtoc; \
	LOAD_LABEL(%rtoc, %r0, TOC); \
	bctrl; \
	ld	%r0, 0x80(%sp); \
	mtlr	%r0; \
	addi	%sp, %sp, 0x70; \
	ld	%rtoc, 0x28(%sp); \
	blr; \
	.previous;

#endif /* __MACRO_H_S__ */

