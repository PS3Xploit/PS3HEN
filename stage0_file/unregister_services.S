#include <lv2/macros.h.S>
#include <lv2/symbols.h>

#define CALL(addr) \
	MEM_BASE(%r0); \
	LOAD_LABEL(%r0, %r0, addr); \
	mtctr	%r0; \
	bctrl;
	
.global syscall_unregister_service

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

syscall_unregister_service:

stdu      %r1, -0xC0(%r1)
mflr      %r0
std       %r27, 0x98(%r1)
mr        %r27, %r3
li        %r3, 0
std       %r28, 0xA0(%r1)
std       %r29, 0xA8(%r1)
std       %r31, 0xB8(%r1)
std       %r0, 0xD0(%r1)
mr        %r28, %r4
#CALL        (extend_kstack_symbol)
CALL(suspend_intr)# (ppu_sched_obj)
CALL(rsx_syscall_check)
mr        %r29, %r3
clrldi    %r28, %r28, 32
CALL(resume_intr)# (ppu_sched_obj)
ld        %r31, 0x208(%r29) # load cur_proc_obj.obj_list
clrldi    %r4, %r27, 32
li        %r29, 0
addi      %r5, %r1, 0x70
addi      %r6, %r1, 0x78
li        %r7, 0x41
mr        %r3, %r31
std       %r29, 0x70(%r1)
std       %r29, 0x78(%r1)
CALL(open_kernel_object_symbol)
addi      %r5, %r1, 0x80
addi      %r6, %r1, 0x88
li        %r7, 0x43      # SYS_SERVICE_OBJECT
li        %r8, 0
mr        %r4, %r28
mr        %r3, %r31
std       %r29, 0x88(%r1)
std       %r29, 0x80(%r1)
CALL(open_shared_kernel_object_symbol) # (cur_proc_obj.obj_list, , , , SYS_SERVICE_OBJECT, 0)
cmpwi     cr7, %r3, 0
mr        %r29, %r3
beq       cr7, 2f
ld        %r0, 0xD0(%r1)
extsw     %r3, %r29
ld        %r27, 0x98(%r1)
ld        %r28, 0xA0(%r1)
mtlr      %r0
ld        %r29, 0xA8(%r1)
ld        %r31, 0xB8(%r1)
addi      %r1, %r1, 0xC0
blr

2:
ld        %r27, 0x80(%r1)
CALL(new_medium_listener_object_symbol)
mr        %r4, %r27
CALL(unk_func2_unregister_service)
mr        %r29, %r3
mr        %r3, %r27
cmpwi     cr7, %r29, 0
beq       cr7, 3f
mr        %r3, %r31
ld        %r4, 0x88(%r1)
CALL(close_kernel_object_handle_symbol) # (cur_proc_obj.obj_list,
ld        %r0, 0xD0(%r1)
ld        %r27, 0x98(%r1)
extsw     %r3, %r29
ld        %r28, 0xA0(%r1)
ld        %r29, 0xA8(%r1)
mtlr      %r0
ld        %r31, 0xB8(%r1)
addi      %r1, %r1, 0xC0
blr

3:
CALL(unk_func3_unregister_service)
cmpdi     cr7, %r27, 0
mr        %r3, %r27
li        %r4, 0x1D
bne       cr7, 5f

4:
mr        %r4, %r28
mr        %r3, %r31
CALL(destroy_shared_kernel_object_symbol)
ld        %r4, 0x88(%r1)
mr        %r29, %r3
mr        %r3, %r31
CALL(close_kernel_object_handle_symbol)
mr        %r3, %r31
ld        %r4, 0x78(%r1)
CALL(destroy_kernel_object_symbol)
ld        %r0, 0xD0(%r1)
ld        %r27, 0x98(%r1)
extsw     %r3, %r29
ld        %r28, 0xA0(%r1)
ld        %r29, 0xA8(%r1)
mtlr      %r0
ld        %r31, 0xB8(%r1)
addi      %r1, %r1, 0xC0
blr

5:
CALL(dealloc_symbol)
b         4b


