#ifndef __LV2_MODULES_H__
#define __LV2_MODULES_H__

#include <lv2/lv2.h>
#include <lv2/process.h>
#include <lv2/memory.h>
#include <lv2/object.h>

typedef int32_t sys_prx_id_t;

typedef struct
{
	uint64_t base;	/* segment base */
	uint64_t filesz;/* segment size */
	uint64_t memsz;	/* segment size */
	uint64_t index;	/* segment index */
	uint64_t type;	/* segment type == ELF type */
} __attribute__((packed)) sys_prx_segment_info_t;

typedef struct
{
	uint64_t size; /* struct size, ignored in kernel version of the function */
	char name[30];
	char version[2];
	uint32_t modattribute;
	uint32_t start_entry;
	uint32_t stop_entry;
	uint32_t all_segments_num;
	uint32_t filename; /* User: user pointer to receive filename; Kernel: ignored */
	uint32_t filename_size;
	uint32_t segments; /* User: user pointer to receive segments; Kernel: ignored */
	uint32_t segments_num;
} __attribute__((packed)) sys_prx_module_info_t;

typedef struct
{
	void *dummy;
} *module_t;

// unk -> pass 0x10000
// unk2 -> pass buffer of at least 0x58 bytes
LV2_EXPORT int authenticate_program_segment(uint64_t unk, void *sce_header, void *inbuf, uint16_t segnum, void *out, uint32_t pid, void *unk2, uint8_t *npdrm_keys);

LV2_EXPORT sys_prx_id_t prx_load_module(process_t process, sys_memory_container_t mem_container, uint64_t flags, char *path);
LV2_EXPORT int prx_start_module(sys_prx_id_t id, process_t process, uint64_t flags, void *meminfo);
LV2_EXPORT int prx_stop_module(sys_prx_id_t id, process_t process, uint64_t flags, void *meminfo);
LV2_EXPORT int prx_unload_module(sys_prx_id_t id, process_t process);

LV2_EXPORT int prx_get_module_info(process_t process, sys_prx_id_t id, sys_prx_module_info_t *modinfo, char *filename, sys_prx_segment_info_t *segments);
LV2_EXPORT sys_prx_id_t prx_get_module_id_by_address(process_t process, void *addr);
LV2_EXPORT sys_prx_id_t prx_get_module_id_by_name(process_t process, char *name);
// unk_list -> pass a buffer of 4*max_count size
LV2_EXPORT int prx_get_module_list(process_t process, sys_prx_id_t *list, uint32_t *unk_list, uint32_t max_count, uint32_t *num, uint32_t *num2);

LV2_EXPORT int open_prx_object(process_t process, sys_prx_id_t id, module_t *module, object_handle_t *handle);
LV2_EXPORT int close_prx_object(process_t process, object_handle_t handle);

LV2_EXPORT int lock_prx_mutex(void);
LV2_EXPORT int unlock_prx_mutex(void);

// Custom functions / helpers
// Name must be 30 characters at least!
int prx_get_module_name_by_address(process_t process, void *addr, char *name);
int prx_start_module_with_thread(sys_prx_id_t id, process_t process, uint64_t flags, uint64_t arg);
int prx_stop_module_with_thread(sys_prx_id_t id, process_t process, uint64_t flags, uint64_t arg);

#endif /* __LV2_MODULES_H__ */
