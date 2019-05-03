#include "assembly.h"

.text
.globl	.ttyWrite
.ttyWrite:
	li      r11, 403
	mflr    r0
	std     r0, 0x10(sp)
	sc
	ld      r0, 0x10(sp)
	mtlr    r0
	blr

