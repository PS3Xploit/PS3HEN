#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#define r0 0
#define r1 1
#define r2 2
#define r3 3
#define r4 4
#define r5 5
#define r6 6
#define r7 7
#define r8 8
#define r9 9
#define r10 10
#define r11 11
#define r12 12
#define r13 13
#define r14 14
#define r15 15
#define r16 16
#define r17 17
#define r18 18
#define r19 19
#define r20 20
#define r21 21
#define r22 22
#define r23 23
#define r24 24
#define r25 25
#define r26 26
#define r27 27
#define r28 28
#define r29 29
#define r30 30
#define r31 31

#define sp r1
#define toc r2

#define cr0 0
#define cr1 1
#define cr2 2
#define cr3 3
#define cr4 4
#define cr5 5
#define cr6 6
#define cr7 7

#define fr0 0
#define fr1 1
#define fr2 2
#define fr3 3
#define fr4 4
#define fr5 5
#define fr6 6
#define fr7 7
#define fr8 8
#define fr9 9
#define fr10 10
#define fr11 11
#define fr12 12
#define fr13 13
#define fr14 14
#define fr15 15
#define fr16 16
#define fr17 17
#define fr18 18
#define fr19 19
#define fr20 20
#define fr21 21
#define fr22 22
#define fr23 23
#define fr24 24
#define fr25 25
#define fr26 26
#define fr27 27
#define fr28 28
#define fr29 29
#define fr30 30
#define fr31 31

#define lv1call sc 1; extsw r3, r3


#define XGLUE(a,b) a##b
#define GLUE(a,b) XGLUE(a,b)

#define _GLOBAL(name) \
	.section ".text"; \
	.align 2 ; \
	.globl name; \
	.globl GLUE(.,name); \
	.section ".opd","aw"; \
name: \
	.quad GLUE(.,name); \
	.quad .TOC.@tocbase; \
	.quad 0; \
	.previous; \
	.type GLUE(.,name),@function; \
GLUE(.,name):

#endif

