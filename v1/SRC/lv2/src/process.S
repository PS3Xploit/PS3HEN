#include <lv2/macros.h.S>
#include <lv2/symbols.h>

.global get_current_process

get_current_process:
	mfsprg0 %r3
	lhz	%r0, 0xC0(%r3)
	cmpwi   cr7, %r0, 3
	beq	cr7, loc_0
	ld	%r3, 0xB0(%r3)
	blr
loc_0:                               
	mfsprg0 %r9
	ld	%r11, 0x68(%r9)
	li	%r3, 0
	ld	%r11, 0(%r11)
	cmpdi	cr7, %r11, 0
	beqlr	cr7
	ld	%r3, 0x80(%r11)
	blr

LV2_FUNCTION(process_kill, process_kill_symbol)

