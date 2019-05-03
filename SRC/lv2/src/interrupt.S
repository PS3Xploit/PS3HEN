#include <lv2/macros.h.S>
#include <lv2/symbols.h>

.global suspend_intr
.global resume_intr

suspend_intr:
	mfsprg0 %r9
	ld	%r3, 0x48(%r9)
	li	%r0, 2
	mtmsrd  %r0, 1
	blr
	
resume_intr:
	mfsprg0 %r9
	ld	%r3, 0x48(%r9)
	li	%r0, 0
	ori	%r0, %r0, 0x8002
	lwsync
	mtmsrd  %r0, 1
	blr
	
LV2_FUNCTION(spin_lock_irqsave_ex, spin_lock_irqsave_ex_symbol)
LV2_FUNCTION(spin_unlock_irqrestore_ex, spin_unlock_irqrestore_ex_symbol)

