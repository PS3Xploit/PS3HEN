.text
	.section .start,"ax",@progbits
	.align 3

	.global _start

_start:

	mflr	%r0
	bl	1f
1:
	mflr 	%r8
	lis 	%r4, 1b@ha
	addi 	%r4, %r4, 1b@l
	subf. 	%r8, %r4, %r8
	beq 	4f
	
	li	%r6, 0
	oris	%r6, %r6, 0xBAAD
	ori	%r6, %r6, 0xCAFE	

	lis 	%r4, __reloc_start@ha
	addi 	%r4, %r4, __reloc_start@l
	lis 	%r5, __reloc_end@ha
	addi 	%r5, %r5, __reloc_end@l
	subf. 	%r5, %r4, %r5
	beq 	4f
	srdi. 	%r5, %r5, 3
	mtctr 	%r5
	add 	%r4, %r4, %r8

2:
	ld 	%r5, 0(%r4)
	srdi 	%r7, %r5, 32	
	cmpw 	%r7, %r6
	bne 	3f	
	rldicl 	%r5, %r5, 0, 32
	add 	%r5, %r5, %r8
	std 	%r5, 0(%r4)	
	
3:
	addi 	%r4, %r4, 8
	bdnz 	2b	

4:
	std 	%r0, 0x10(%sp)
	stdu 	%sp, -0x80(%sp)
	std	%rtoc, 0x28(%sp)
	
	lis 	%r4, __toc@ha
	addi 	%r4, %r4, __toc@l
	add 	%rtoc, %r4, %r8	
	bl	main	
	
	ld	%rtoc, 0x28(%sp)
	addi 	%sp, %sp, 0x80
	ld 	%r0, 0x10(%sp)	
	mtlr 	%r0
	li 	%r8, 0
	blr	
	



