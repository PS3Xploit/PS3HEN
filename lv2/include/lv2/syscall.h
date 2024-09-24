#ifndef __LV2_SYSCALL_H__
#define __LV2_SYSCALL_H__

#include <lv2/lv2.h>
#include <lv2/patch.h>

#define SYS_PROCESS_GETPID                                            1
#define SYS_PROCESS_WAIT_FOR_CHILD                                    2
#define SYS_PROCESS_GET_STATUS                                        4
#define SYS_PROCESS_DETACH_CHILD                                      5
#define SYS_PROCESS_GET_NUMBER_OF_OBJECT                             12
#define SYS_PROCESS_GET_ID                                           13
#define SYS_PROCESS_IS_SPU_LOCK_LINE_RESERVATION_ADDRESS             14
#define SYS_PROCESS_GETPPID                                          18
#define SYS_PROCESS_KILL                                             19
// 21 -> create_process
// 27 -> aother create process too, used for games, ps1 emu, etc
#define SYS_PROCESS_WAIT_FOR_CHILD2                                  23
#define SYS_PROCESS_GET_SDK_VERSION                                  25
#define SYS_PROCESS_GET_PARAMSFO				     				 30 /* Added */
#define SYS_PROCESS_GET_PPU_GUID                                     31
#define SYS_PPU_THREAD_YIELD                                         43
#define SYS_PPU_THREAD_JOIN                                          44
#define SYS_PPU_THREAD_DETACH                                        45
#define SYS_PPU_THREAD_GET_JOIN_STATE                                46
#define SYS_PPU_THREAD_SET_PRIORITY                                  47
#define SYS_PPU_THREAD_GET_PRIORITY                                  48
#define SYS_PPU_THREAD_GET_STACK_INFORMATION                         49
#define SYS_PPU_THREAD_RENAME                                        56
#define SYS_PPU_THREAD_RECOVER_PAGE_FAULT                            57
#define SYS_PPU_THREAD_GET_PAGE_FAULT_CONTEXT                        58
#define SYS_TRACE_ALLOCATE_BUFFER                                    67
#define SYS_TRACE_FREE_BUFFER                                        68
#define SYS_TRACE_CREATE2                                            69
#define SYS_TIMER_CREATE                                             70
#define SYS_TIMER_DESTROY                                            71
#define SYS_TIMER_GET_INFORMATION                                    72
#define _SYS_TIMER_START                                             73
#define SYS_TIMER_STOP                                               74
#define SYS_TIMER_CONNECT_EVENT_QUEUE                                75
#define SYS_TIMER_DISCONNECT_EVENT_QUEUE                             76
#define SYS_TRACE_CREATE2_IN_CBEPM                                   77
#define SYS_INTERRUPT_TAG_CREATE                                     80
#define SYS_INTERRUPT_TAG_DESTROY                                    81
#define SYS_EVENT_FLAG_CREATE                                        82
#define SYS_EVENT_FLAG_DESTROY                                       83
#define _SYS_INTERRUPT_THREAD_ESTABLISH                              84
#define SYS_EVENT_FLAG_WAIT                                          85
#define SYS_EVENT_FLAG_TRYWAIT                                       86
#define SYS_EVENT_FLAG_SET                                           87
#define SYS_INTERRUPT_THREAD_EOI                                     88
#define _SYS_INTERRUPT_THREAD_DISESTABLISH                           89
#define SYS_SEMAPHORE_CREATE                                         90
#define SYS_SEMAPHORE_DESTROY                                        91
#define SYS_SEMAPHORE_WAIT                                           92
#define SYS_SEMAPHORE_TRYWAIT                                        93
#define SYS_SEMAPHORE_POST                                           94
#define SYS_MUTEX_CREATE                                            100
#define SYS_MUTEX_DESTROY                                           101
#define SYS_MUTEX_LOCK                                              102
#define SYS_MUTEX_TRYLOCK                                           103
#define SYS_MUTEX_UNLOCK                                            104
#define SYS_COND_CREATE                                             105
#define SYS_COND_DESTROY                                            106
#define SYS_COND_WAIT                                               107
#define SYS_COND_SIGNAL                                             108
#define SYS_COND_SIGNAL_ALL                                         109
#define SYS_COND_SIGNAL_TO                                          110
#define SYS_SEMAPHORE_GET_VALUE                                     114
#define SYS_EVENT_FLAG_CLEAR                                        118
#define SYS_RWLOCK_CREATE                                           120
#define SYS_RWLOCK_DESTROY                                          121
#define SYS_RWLOCK_RLOCK                                            122
#define SYS_RWLOCK_TRYRLOCK                                         123
#define SYS_RWLOCK_RUNLOCK                                          124
#define SYS_RWLOCK_WLOCK                                            125
#define SYS_RWLOCK_WUNLOCK                                          127
#define SYS_EVENT_QUEUE_CREATE                                      128
#define SYS_EVENT_QUEUE_DESTROY                                     129
#define SYS_EVENT_QUEUE_RECEIVE                                     130
#define SYS_EVENT_QUEUE_TRYRECEIVE                                  131
#define SYS_EVENT_FLAG_CANCEL                                       132
#define SYS_EVENT_QUEUE_DRAIN                                       133
#define SYS_EVENT_PORT_CREATE                                       134
#define SYS_EVENT_PORT_DESTROY                                      135
#define SYS_EVENT_PORT_CONNECT_LOCAL                                136
#define SYS_EVENT_PORT_DISCONNECT                                   137
#define SYS_EVENT_PORT_SEND                                         138
#define SYS_EVENT_FLAG_GET                                          139
#define SYS_EVENT_PORT_CONNECT_IPC                                  140
#define SYS_TIMER_USLEEP                                            141
#define SYS_TIMER_SLEEP                                             142
#define SYS_TIME_GET_CURRENT_TIME                                   145
#define SYS_TIME_GET_TIMEBASE_FREQUENCY                             147
#define SYS_RWLOCK_TRYWLOCK                                         148
#define SYS_RAW_SPU_CREATE_INTERRUPT_TAG                            150
#define SYS_RAW_SPU_SET_INT_MASK                                    151
#define SYS_RAW_SPU_GET_INT_MASK                                    152
#define SYS_RAW_SPU_SET_INT_STAT                                    153
#define SYS_RAW_SPU_GET_INT_STAT                                    154
#define SYS_SPU_IMAGE_OPEN                                          156
#define SYS_RAW_SPU_CREATE                                          160
#define SYS_RAW_SPU_DESTROY                                         161
#define SYS_RAW_SPU_READ_PUINT_MB                                   163
#define SYS_SPU_THREAD_GET_EXIT_STATUS                              165
#define SYS_SPU_THREAD_SET_ARGUMENT                                 166
#define SYS_SPU_THREAD_GROUP_START_ON_EXIT                          167
#define SYS_SPU_INITIALIZE                                          169
#define SYS_SPU_THREAD_GROUP_CREATE                                 170
#define SYS_SPU_THREAD_GROUP_DESTROY                                171
#define SYS_SPU_THREAD_INITIALIZE                                   172
#define SYS_SPU_THREAD_GROUP_START                                  173
#define SYS_SPU_THREAD_GROUP_SUSPEND                                174
#define SYS_SPU_THREAD_GROUP_RESUME                                 175
#define SYS_SPU_THREAD_GROUP_YIELD                                  176
#define SYS_SPU_THREAD_GROUP_TERMINATE                              177
#define SYS_SPU_THREAD_GROUP_JOIN                                   178
#define SYS_SPU_THREAD_GROUP_SET_PRIORITY                           179
#define SYS_SPU_THREAD_GROUP_GET_PRIORITY                           180
#define SYS_SPU_THREAD_WRITE_LS                                     181
#define SYS_SPU_THREAD_READ_LS                                      182
#define SYS_SPU_THREAD_WRITE_SNR                                    184
#define SYS_SPU_THREAD_GROUP_CONNECT_EVENT                          185
#define SYS_SPU_THREAD_GROUP_DISCONNECT_EVENT                       186
#define SYS_SPU_THREAD_SET_SPU_CFG                                  187
#define SYS_SPU_THREAD_GET_SPU_CFG                                  188
#define SYS_SPU_THREAD_WRITE_SPU_MB                                 190
#define SYS_SPU_THREAD_CONNECT_EVENT                                191
#define SYS_SPU_THREAD_DISCONNECT_EVENT                             192
#define SYS_SPU_THREAD_BIND_QUEUE                                   193
#define SYS_SPU_THREAD_UNBIND_QUEUE                                 194
#define SYS_RAW_SPU_SET_SPU_CFG                                     196
#define SYS_RAW_SPU_GET_SPU_CFG                                     197
#define SYS_SPU_THREAD_RECOVER_PAGE_FAULT                           198
#define SYS_RAW_SPU_RECOVER_PAGE_FAULT                              199
#define SYS_SPU_THREAD_GROUP_SET_COOPERATIVE_VICTIMS                250
#define SYS_SPU_THREAD_GROUP_CONNECT_EVENT_ALL_THREADS              251
#define SYS_SPU_THREAD_GROUP_DISCONNECT_EVENT_ALL_THREADS           252
#define SYS_SPU_IMAGE_OPEN_BY_FD                                    260
#define SYS_VM_MEMORY_MAP                                           300
#define SYS_VM_UNMAP                                                301
#define SYS_VM_APPEND_MEMORY                                        302
#define SYS_VM_RETURN_MEMORY                                        303
#define SYS_VM_LOCK                                                 304
#define SYS_VM_UNLOCK                                               305
#define SYS_VM_TOUCH                                                306
#define SYS_VM_FLUSH                                                307
#define SYS_VM_INVALIDATE                                           308
#define SYS_VM_STORE                                                309
#define SYS_VM_SYNC                                                 310
#define SYS_VM_TEST                                                 311
#define SYS_VM_GET_STATISTICS                                       312
#define SYS_MEMORY_CONTAINER_CREATE                                 324
#define SYS_MEMORY_CONTAINER_DESTROY                                325
#define SYS_MMAPPER_ALLOCATE_FIXED_ADDRESS                          326
#define SYS_MMAPPER_ENABLE_PAGE_FAULT_NOTIFICATION                  327
#define SYS_MMAPPER_ALLOCATE_ADDRESS                                330
#define SYS_MMAPPER_FREE_ADDRESS                                    331
#define SYS_MMAPPER_CHANGE_ADDRESS_ACCESS_RIGHT                     336
#define SYS_MMAPPER_SEARCH_AND_MAP                                  337
#define SYS_MEMORY_CONTAINER_GET_SIZE                               343
#define SYS_MEMORY_ALLOCATE                                         348
#define SYS_MEMORY_FREE                                             349
#define SYS_MEMORY_ALLOCATE_FROM_CONTAINER                          350
#define SYS_MEMORY_GET_PAGE_ATTRIBUTE                               351
#define SYS_MEMORY_GET_USER_MEMORY_SIZE                             352
#define SYS_SM_GET_EXT_EVENT2					   					378 /* Added */
#define SYS_SM_SHUTDOWN						   					    379 /* Added */
#define SYS_SM_GET_PARAMS					    					380 /* Added */
#define SYS_GET_HW_CONFIG					    					393 /* Added */
#define SYS_CONSOLE_WRITE					    					398
#define SYS_TTY_READ                                                402
#define SYS_TTY_WRITE                                               403
#define SYS_OVERLAY_LOAD_MODULE                                     450
#define SYS_OVERLAY_UNLOAD_MODULE                                   451
#define SYS_OVERLAY_GET_MODULE_LIST                                 452
#define SYS_OVERLAY_GET_MODULE_INFO                                 453
#define SYS_OVERLAY_LOAD_MODULE_BY_FD                               454
#define SYS_OVERLAY_GET_MODULE_INFO2                                455
#define SYS_OVERLAY_GET_SDK_VERSION                                 456
#define _SYS_PRX_GET_MODULE_ID_BY_ADDRESS                           461
#define _SYS_PRX_LOAD_MODULE_BY_FD                                  463
#define _SYS_PRX_LOAD_MODULE_ON_MEMCONTAINER_BY_FD                  464
#define _SYS_PRX_LOAD_MODULE_LIST                                   465
#define _SYS_PRX_LOAD_MODULE_LIST_ON_MEMCONTAINER                   466
#define SYS_PRX_GET_PPU_GUID                                        467
#define _SYS_PRX_LOAD_MODULE                                        480
#define _SYS_PRX_START_MODULE                                       481
#define _SYS_PRX_STOP_MODULE                                        482
#define _SYS_PRX_UNLOAD_MODULE                                      483
#define _SYS_PRX_REGISTER_MODULE                                    484
#define _SYS_PRX_QUERY_MODULE                                       485
#define _SYS_PRX_REGISTER_LIBRARY                                   486
#define _SYS_PRX_UNREGISTER_LIBRARY                                 487
#define _SYS_PRX_LINK_LIBRARY                                       488
#define _SYS_PRX_UNLINK_LIBRARY                                     489
#define _SYS_PRX_QUERY_LIBRARY                                      490
#define _SYS_PRX_GET_MODULE_LIST                                    494
#define _SYS_PRX_GET_MODULE_INFO                                    495
#define _SYS_PRX_GET_MODULE_ID_BY_NAME                              496
#define _SYS_PRX_LOAD_MODULE_ON_MEMCONTAINER                        497
#define _SYS_PRX_START                                              498
#define _SYS_PRX_STOP                                               499
#define SYS_STORAGE_OPEN                                            600
#define SYS_STORAGE_CLOSE                                           601
#define SYS_STORAGE_READ                                            602
#define SYS_STORAGE_WRITE                                           603
#define SYS_STORAGE_SEND_DEVICE_COMMAND                             604
#define SYS_STORAGE_ASYNC_CONFIGURE                                 605
#define SYS_STORAGE_ASYNC_READ                                      606
#define SYS_STORAGE_ASYNC_WRITE                                     607
#define SYS_STORAGE_ASYNC_CANCEL                                    608
#define SYS_STORAGE_GET_DEVICE_INFO                                 609
#define SYS_STORAGE_GET_DEVICE_CONFIG                               610
#define SYS_STORAGE_REPORT_DEVICES                                  611
#define SYS_STORAGE_CONFIGURE_MEDIUM_EVENT                          612
#define SYS_STORAGE_SET_MEDIUM_POLLING_INTERVAL                     613
#define SYS_STORAGE_CREATE_REGION                                   614
#define SYS_STORAGE_DELETE_REGION                                   615
#define SYS_STORAGE_EXECUTE_DEVICE_COMMAND                          616
#define SYS_STORAGE_GET_REGION_ACL                                  617
#define SYS_STORAGE_SET_REGION_ACL                                  618
#define SYS_STORAGE_ASYNC_SEND_DEVICE_COMMAND                       619
#define SYS_IO_BUFFER_CREATE                                        624
#define SYS_IO_BUFFER_DESTROY                                       625
#define SYS_IO_BUFFER_ALLOCATE                                      626
#define SYS_IO_BUFFER_FREE                                          627
#define SYS_GPIO_SET                                                630
#define SYS_GPIO_GET                                                631
#define SYS_FSW_CONNECT_EVENT                                       633
#define SYS_FSW_DISCONNECT_EVENT                                    634
#define SYS_RSX_DEVICE_OPEN                                         666
#define SYS_RSX_DEVICE_CLOSE                                        667
#define SYS_RSX_MEMORY_ALLOCATE                                     668
#define SYS_RSX_MEMORY_FREE                                         669
#define SYS_RSX_CONTEXT_ALLOCATE                                    670
#define SYS_RSX_CONTEXT_FREE                                        671
#define SYS_RSX_CONTEXT_IOMAP                                       672
#define SYS_RSX_CONTEXT_IOUNMAP                                     673
#define SYS_RSX_CONTEXT_ATTRIBUTE                                   674
#define SYS_RSX_DEVICE_MAP                                          675
#define SYS_RSX_DEVICE_UNMAP                                        676
#define SYS_RSX_ATTRIBUTE                                           677
#define SYS_BDEMU_SEND_COMMAND                                      699
#define SYS_NET_ACCEPT						    					700 /* Added */
#define SYS_NET_BIND						    					701 /* Added */
#define SYS_NET_CONNECT						    					702 /* Added */
#define SYS_NET_LISTEN						    					706 /* Added */
#define SYS_NET_RECVFROM					    					707 /* Added */
#define SYS_NET_SENDTO						    					710 /* Added */
#define SYS_NET_SHUTDOWN					    					712 /* Added */
#define SYS_NET_SOCKET						    					713 /* Added */
#define SYS_NET_CLOSE						    					714 /* Added */
#define SYS_CELLFSOPEN						    					801 /* Added */
#define SYS_CELLFSREAD						    					802 /* Added */
#define SYS_CELLFSWRITE						    					803 /* Added */
#define SYS_CELLFSCLOSE						    					804 /* Added */
#define SYS_CELLFSOPENDIR					    					805 /* Added */
#define SYS_CELLFSSTAT						    					808 /* Added */
#define SYS_CELLFSFSTAT						    					809 /* Added */
#define SYS_CELLFSMKDIR						    					811 /* Added */
#define SYS_CELLFSRENAME					    					812 /* Added */
#define SYS_CELLFSUNLINK					    					814 /* Added */
#define SYS_CELLFSUTIME						    					815 /* Added */
#define SYS_CELLFSLSEEK						    					818 /* Added */
#define SYS_CELLFSNEWFS						    					836 /* Added */
#define SYS_CELLFSMOUNT						    					837 /* Added */
#define SYS_CELLFSUMOUNT					    					838 /* Added */
// 864. auth psx disc: 0x5004, 1. auth ps3 disc: 0x5007, in/out buffer of 0x18 bytes
// return to normal : 0x5004, 0x29
#define SYS_SS_ACCESS_CONTROL_ENGINE				    			871 /* Added */
#define SYS_SS_GET_OPEN_PSID                                        872
#define SYS_SS_GET_CACHE_OF_PRODUCT_MODE			    			873 /* Added */
#define SYS_SS_GET_CACHE_OF_FLASH_EXT_FLAG			    			874 /* Added */
#define SYS_SS_GET_BOOT_DEVICE					    				875 /* Added */
#define SYS_SS_DISC_ACCESS_CONTROL				    				876 /* Added */
#define SYS_SS_AD_SIGN						    					878 /* Added */
#define SYS_SS_MEDIA_ID						    					879 /* Added */
#define SYS_DECI3_OPEN                                              880
#define SYS_DECI3_CREATE_EVENT_PATH                                 881
#define SYS_DECI3_CLOSE                                             882
#define SYS_DECI3_SEND                                              883
#define SYS_DECI3_RECEIVE                                           884

#define MAX_NUM_OF_SYSTEM_CALLS                                    1024

#define LV2_SYSCALL	LV2_CONTEXT

#define LV2_SYSCALL2(ret, name, args) asm("" \
"	.section \".text\"\n" \
"	.align 2\n" \
"	.p2align 3,,7\n" \
"	.globl "#name"\n" \
"	.section \".opd\",\"aw\"\n" \
"	.align 3\n" \
#name": \n" \
"	.quad .L."#name",.TOC.@tocbase\n" \
"	.previous\n" \
"	.type   "#name", @function\n" \
".L."#name":\n" \
"	nop\n" \
"	nop\n" \
"	nop\n" \
"	nop\n" \
"	mflr 0\n" \
"	std 0, 32(1)\n" \
"	std 2, 40(1)\n" \
"	bl +4\n" \
"	li 0, 0\n" \
"	li 2, 0\n" \
"	oris 2, 2, __toc@h\n" \
"	ori 2, 2, __toc@l\n" \
"	oris 0, 0, .L."#name"@h\n" \
"	ori 0, 0, .L."#name"@l\n" \
"	subf 0, 0, 2\n" \
"	mflr 2\n" \
"	addi 2, 2, -32\n" \
"	add  2, 2, 0\n" \
"	bl .L._"#name"\n" \
"	ld 2, 40(1)\n" \
"	ld 0, 32(1)\n" \
"	mtlr 0\n" \
"	blr\n" \
"	.size "#name",.-.L."#name"\n"); \
ret name args; \
ret _##name args

/* Extends the kernel stack of the syscall in 4096 bytes. Can only be called once in a syscall */
LV2_EXPORT int extend_kstack(int unk);

static INLINE uint64_t get_syscall_address(int n)
{
	uint64_t **table = (uint64_t **)MKA(syscall_table_symbol);
	uint64_t *syscall = table[n];
	return syscall[0];
}

static INLINE void create_syscall(int n, void *func)
{
	uint64_t ***table = (uint64_t ***)MKA(syscall_table_symbol);
	uint64_t **syscall = table[n];
	f_desc_t *f = (f_desc_t *)func;	
	syscall[0] = f->addr;	
}

static INLINE void create_syscall2(int n, void *func)
{
	uint64_t **table = (uint64_t **)MKA(syscall_table_symbol);
	f_desc_t *f = (f_desc_t *)func;	
	uint64_t *opd = f->addr;
	
	opd[0] = ((uint64_t)opd) + 16;
	opd[1] = MKA(TOC);
	table[n] = opd;
}

#define patch_syscall	create_syscall

static INLINE void set_syscall_handler(void *syscall_handler)
{
	patch_call(syscall_call_offset, syscall_handler);
}


#endif /* __LV2_SYSCALL_H__ */
