#ifndef __LV2_PROCESS_H__
#define __LV2_PROCESS_H__

#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <lv2/lv2.h>
#include <lv2/interrupt.h>

#define VSH_PROCESS_NAME	"_main_vsh.self"

typedef struct
{
	void *unk_00; // 0
	char name[24]; // 8
	// ...
} __attribute__((packed)) UnkProcessStruct;

typedef struct _process_t
{
	void *syscall_table; 				// 0
	uint64_t unk_8[4]; 					// 8
	uint32_t pid; 						// 0x28
	int status; 						// 0x2C
	void *mem_object; 					// 0x30
	UnkProcessStruct *unk_38; 			// 0x38
	uint64_t unk_40; 					// 0x40
	void *first_thread; 				// 0x48 
	uint64_t unk_50; 					// 0x50
	uint64_t unk_58; 					// 0x58
	void *unk_60; 						// 0x60
	void *unk_68; 						// 0x68 vshprocess -> mios2_SPU_Service.elf
	void *unk_70; 						// 0x70 vshprocess -> mios2_SPU_Service.elf
	uint64_t unk_78; 					// 0x78
	uint64_t unk_80; 					// 0x80
	uint64_t unk_88[4]; 				// 0x88
	uint64_t unk_A8; 					// 0xA8  user address?
	struct _process_t *parent;  		// 0xB0
	struct _process_t *first_child;  	// 0xB8  
	struct _process_t *next_sibling; 	// 0xC0
	uint64_t num_children; 				// 0xC8
	void *unk_D0; 	 					// 0xD0
	uint64_t unk_D8; 					// 0xD8
	uint64_t unk_E0; 					// 0xE0
	uint64_t unk_E8; 					// 0xE8
	uint64_t unk_F0[2]; 				// 0xF0
	uint64_t unk_100; 					// 0x100
	uint64_t unk_108; 					// 0x108
	void *unk_110; 						// 0x110
	void *unk_118; 						// 0x118  vshprocess -> pointer to unk_D0
	uint64_t unk_120; 					// 0x120
	void *unk_128; 						// 0x128  only on vshprocess -> same as first_thread
	void *unk_130; 						// 0x130 only on vsh process -> same as first thread
	uint64_t unk_138;	 				// 0x138
	uint64_t unk_140[4]; 				// 0x140
	char *process_image; 				// 0x160
	void *unk_168; 						// 0x168
	uint64_t unk_170; 					// 0x170
	uint64_t unk_178; 					// 0x178
	uint64_t unk_180; 					// 0x180
	uint64_t unk_188[4]; 				// 0x188
	uint64_t unk_1A8; 					// 0x1A8
	uint64_t unk_1B0; 					// 0x1B0
	uint64_t unk_1B8; 					// 0x1B8
	uint64_t unk_1C0; 					// 0x1C0
	uint64_t unk_1C8; 					// 0x1C8
	uint64_t unk_1D0; 					// 0x1D0
	uint64_t unk_1D8; 					// 0x1D8
	uint64_t unk_1E0; 					// 0x1E0
	uint64_t unk_1E8[4]; 				// 0x1E8
	void *object_table; 				// 0x208 waiting for a better name...
	// ...?
	// 0x26C -> sdk version 32bits
} __attribute__((packed)) *process_t;

process_t get_current_process(void);

LV2_EXPORT int process_kill(process_t process);

static INLINE process_t get_current_process_critical(void)
{
	suspend_intr();
	process_t p = get_current_process();
	resume_intr();
	return p;
}

static INLINE void *get_current_process_object_table(void)
{
	suspend_intr();
	void *t = get_current_process()->object_table;
	resume_intr();	
	return t;
}

static INLINE char *get_process_name(process_t process)
{
	return process->unk_38->name;
}

static INLINE int is_vsh_process(process_t process)
{
	return (process && strcmp(get_process_name(process)+8, VSH_PROCESS_NAME) == 0);
}

#endif /* __LV2_PROCESS_H__ */
