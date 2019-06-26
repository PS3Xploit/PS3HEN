#include <lv2/macros.h.S>
#include <lv2/symbols.h>

#define CALL_NORET(addr) \
	MEM_BASE(%r0); \
	LOAD_LABEL(%r0, %r0, addr); \
	mtctr	%r0; \
	bctr;	

.global suspend_intr
.global resume_intr
.global ioctl_patched

ioctl_patched:
	mflr %r0
	std %r0, 0x10(%r1)
	stdu %r1, -0x80(%r1)
	std	%r31, 0x78(%r1)
	std	%r30, 0x70(%r1)
	std	%r29, 0x68(%r1)
	b 3f
	lis 	  %r31, 0x8000000000650000@highest   
	ori 	  %r31, %r31, 0x8000000000650000@higher  
	rldicr    %r31, %r31,32,31 
	oris      %r31, %r31,0x8000000000650000@h   
	ori       %r31, %r31,0x8000000000650000@l 
	lis 	  %r30, 0x8000000000700000@highest   
	ori 	  %r30, %r30, 0x8000000000700000@higher  
	rldicr    %r30, %r30,32,31 
	oris      %r30, %r30,0x8000000000700000@h   
	ori       %r30, %r30,0x8000000000700000@l 
	ld	%r29,0x18(%r28)
	cmpd cr7, %r29, %r30
	ble cr7, 2f
	mr	%r11, %r29
1:
	ld %r31, 0x78(%r1)
	ld %r30, 0x70(%r1)
	ld %r29, 0x68(%r1)
	addi %r1, %r1, 0x80
	ld %r0, 0x10(%r1)
	mtlr	%r0
	blr
2:
	cmpd cr7, %r29, %r31
	bge	cr7, 3f
	b 1b
3:
	ld %r31, 0x78(%r1)
	ld %r30, 0x70(%r1)
	ld %r29, 0x68(%r1)
	addi %r1, %r1, 0x80
	ld %r0, 0x10(%r1)
	ld %r3, -0x1e88(%r2)
	li %r5,0
	li %r6,0
	CALL_NORET(0x123fa0)

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

