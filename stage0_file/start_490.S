	.text
	.section .start,"ax",@progbits
	.align 3

	.global _start
	
#include <lv2/macros.h.S>
#include <lv2/symbols.h>

#define CALL_NORET(addr) \
	MEM_BASE(%r0); \
	LOAD_LABEL(%r0, %r0, addr); \
	mtctr	%r0; \
	bctr;		
	
#define CALL(addr) \
	MEM_BASE(%r0); \
	LOAD_LABEL(%r0, %r0, addr); \
	mtctr	%r0; \
	bctrl;
	
_start:
	mfsprg0 %r9
	ld %r3, 0x68(%r9)
	ld %r3, 0(%r3)
	ld %r1, 0x50(%r3)
	addi %r1, %r1, 0xd40

	mflr	%r0
	std 	%r0, 0x10(%sp)
	stdu 	%sp, -0x80(%sp)
	std 	%rtoc, 0x28(%sp)
	clrrdi 	%rtoc, %rtoc, 32
	oris 	%rtoc, %rtoc, __toc@h
	ori 	%rtoc, %rtoc, __toc@l
	bl 	main
	cmpdi cr7, %r3,0
	bne cr7,2f
	ld	%rtoc, 0x28(%sp)
	addi 	%sp, %sp, 0x80
	ld 	%r0, 0x10(%sp)
	ld %r3, -0x1e88(%r2)
	li %r5,0
	li %r6,0
	CALL_NORET(0x123f98)

2:

	ld	%rtoc, 0x28(%sp)
	addi 	%sp, %sp, 0x80
	ld 	%r0, 0x10(%sp)
	mtctr %r3
	bctr

