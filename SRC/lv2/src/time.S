#include <lv2/macros.h.S>
#include <lv2/symbols.h>

.global	get_time_seconds

get_time_seconds:

	stdu	%sp, -0x80(%sp)
	mflr	%r0
	std	%r0, 0x90(%sp)
	std	%rtoc, 0x28(%sp)
	MEM_BASE(%r0)
	LOAD_LABEL(%rtoc, %r0, TOC)
1: 
	mftb	%r9
	cmpwi	%r9, 0
	beq	1b
	ld	%r8, time_rtoc_entry_1(%rtoc)
	ld	%r7, time_rtoc_entry_2(%rtoc)
	ld	%r0, 0(%r8)
	ld	%r11, 0(%r7)
	add	%r9, %r9, %r0
	divd	%r3, %r9, %r11
	ld	%rtoc, 0x28(%sp)
	ld	%r0, 0x90(%sp)
	addi	%sp, %sp, 0x80
	mtlr	%r0
	blr
	

	
