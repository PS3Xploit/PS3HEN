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
	mflr	%r0
	std 	%r0, 0x10(%sp)
	stdu 	%sp, -0x80(%sp)
	std		%r28, 0x78(%sp)
	std		%r27, 0x70(%sp)
	std		%r26, 0x68(%sp)
	
1:
	li %r4, 0
	lis %r4, 0x8000
	sldi %r4, %r4, 32
	oris %r4, %r4, 0x7e
	ori %r4, %r4, 0
	li %r3,0
	lis %r3, 0x8d00
	ori %r3, %r3, 0x500
	li %r5, 4
	CALL(copy_from_user_symbol)
	li %r3, 0
	lis %r3, 0x8000
	sldi %r3, %r3, 32
	oris %r3, %r3, 0x7e
	ld %r28, 0(%r3) #our config handle
	
	li %r4, 0
	lis %r4, 0x8000
	sldi %r4, %r4, 32
	oris %r4, %r4, 0x7e
	ori %r4, %r4, 0x10
	li %r3, 0
	lis %r3, 0x8d0f
	ori %r3, %r3, 0xf050
	li %r5, 4
	CALL(copy_from_user_symbol)
	li %r3, 0
	lis %r3, 0x8000
	sldi %r3, %r3, 32
	oris %r3, %r3, 0x7e
	ori %r3, %r3, 0x10
	mr %r27, %r3 #our services
	
	mr %r3, %r28
	lwz %r4, 0(%r27)
	stdu      %r1, -0xC0(%r1)
	std       %r27, 0x98(%r1)
	mr %r27, %r3
	li %r3, 0
	std       %r28, 0xA0(%r1)
	std       %r29, 0xA8(%r1)
	std       %r31, 0xB8(%r1)
	mr        %r28, %r4
	li %r26, 0
	lis %r26, 0x8000
	sldi %r26, %r26, 32
	oris %r26, %r26, 2f@h
	ori %r26, %r26, 2f@l
	std %r26, 0xd0(%r1)
	MEM_BASE(%r0)
	LOAD_LABEL(%r0, %r0, 0xa5ae4)
	mtctr	%r0
	bctrl

2:
	mr %r3, %r28
	lwz %r4, 4(%r27)
	stdu      %r1, -0xC0(%r1)
	std       %r27, 0x98(%r1)
	mr %r27, %r3
	li %r3, 0
	std       %r28, 0xA0(%r1)
	std       %r29, 0xA8(%r1)
	std       %r31, 0xB8(%r1)
	mr        %r28, %r4
	li %r26, 0
	lis %r26, 0x8000
	sldi %r26, %r26, 32
	oris %r26, %r26, 3f@h
	ori %r26, %r26, 3f@l
	std %r26, 0xd0(%r1)
	MEM_BASE(%r0)
	LOAD_LABEL(%r0, %r0, 0xa5ae4)
	mtctr	%r0
	bctrl

3:
	mr %r3, %r28
	lwz %r4, 8(%r27)
	stdu      %r1, -0xC0(%r1)
	std       %r27, 0x98(%r1)
	mr %r27, %r3
	li %r3, 0
	std       %r28, 0xA0(%r1)
	std       %r29, 0xA8(%r1)
	std       %r31, 0xB8(%r1)
	mr        %r28, %r4
	li %r26, 0
	lis %r26, 0x8000
	sldi %r26, %r26, 32
	oris %r26, %r26, 4f@h
	ori %r26, %r26, 4f@l
	std %r26, 0xd0(%r1)
	MEM_BASE(%r0)
	LOAD_LABEL(%r0, %r0, 0xa5ae4)
	mtctr	%r0
	bctrl
	
4:
	mr %r3, %r28
	lwz %r4, 0xc(%r27)
	stdu      %r1, -0xC0(%r1)
	std       %r27, 0x98(%r1)
	mr %r27, %r3
	li %r3, 0
	std       %r28, 0xA0(%r1)
	std       %r29, 0xA8(%r1)
	std       %r31, 0xB8(%r1)
	mr        %r28, %r4
	li %r26, 0
	lis %r26, 0x8000
	sldi %r26, %r26, 32
	oris %r26, %r26, 5f@h
	ori %r26, %r26, 5f@l
	std %r26, 0xd0(%r1)
	MEM_BASE(%r0)
	LOAD_LABEL(%r0, %r0, 0xa5ae4)
	mtctr	%r0
	bctrl
	
5:
	mr %r3, %r28
	lwz %r4, 0x10(%r27)
	stdu      %r1, -0xC0(%r1)
	std       %r27, 0x98(%r1)
	mr %r27, %r3
	li %r3, 0
	std       %r28, 0xA0(%r1)
	std       %r29, 0xA8(%r1)
	std       %r31, 0xB8(%r1)
	mr        %r28, %r4
	li %r26, 0
	lis %r26, 0x8000
	sldi %r26, %r26, 32
	oris %r26, %r26, 6f@h
	ori %r26, %r26, 6f@l
	std %r26, 0xd0(%r1)
	MEM_BASE(%r0)
	LOAD_LABEL(%r0, %r0, 0xa5ae4)
	mtctr	%r0
	bctrl

6:	
	mr %r3, %r28
	CALL(0xA6400)
	
	ld		%r28, 0x78(%sp)
	ld		%r27, 0x70(%sp)
	ld		%r26, 0x68(%sp)
	
7:
	std 	%rtoc, 0x28(%sp)
	clrrdi 	%rtoc, %rtoc, 32
	oris 	%rtoc, %rtoc, __toc@h
	ori 	%rtoc, %rtoc, __toc@l
	bl 	main
	ld	%rtoc, 0x28(%sp)
	addi 	%sp, %sp, 0x80
	ld 	%r0, 0x10(%sp)
	
	mfsprg0 %r9
	ld %r3, 0x68(%r9)
	ld %r3, 0(%r3)
	ld %r1, 0x50(%r3)
	addi %r1, %r1, 0xd40
	ld %r3, -0x1d60(%r2)
	li %r5,0
	li %r6,0
	CALL_NORET(0x129E28)


