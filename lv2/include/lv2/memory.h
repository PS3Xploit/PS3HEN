#ifndef __LV2_MEMORY_H__
#define __LV2_MEMORY_H__

#include <lv2/lv2.h>
#include <lv2/process.h>

#define KB(n) (1024*n)
#define MB(n) (1048576*n)
#define GB(n) (1073741824*n)

#define MEMORY_PAGE_SIZE_4K		0x100
#define MEMORY_PAGE_SIZE_64K	0x200
#define MEMORY_PAGE_SIZE_1M		0x400

typedef uint32_t sys_memory_container_t;

LV2_EXPORT void *alloc(int size, uint64_t flags);

LV2_EXPORT void *dealloc(void *ptr, uint64_t flags);
LV2_EXPORT int copy_to_user(void *src, void *user_dst, int size);
LV2_EXPORT int copy_from_user(void *user_src, void *dst, int size);
LV2_EXPORT int copy_to_process(process_t process, void *src, void *foreign_dst, int size);
LV2_EXPORT int copy_from_process(process_t process, void *foreign_src, void *dst, int size);
LV2_EXPORT int process_write_memory(process_t process, void *destination, void *source, size_t size, int flag);
LV2_EXPORT int process_read_memory(process_t process, void *destination, void *source, size_t size);

LV2_EXPORT int kernel_ea_to_lpar_addr(void *ea_addr, uint64_t *lpar_addr);
LV2_EXPORT int process_ea_to_lpar_addr_ex(void *mem_object, void *ea_addr, uint64_t *lpar_addr);
LV2_EXPORT int set_pte(void *unk, void *unk2, uint64_t va, uint64_t lpar, uint32_t rpn, uint64_t slot);

LV2_EXPORT int page_allocate(process_t process, uint64_t size, uint64_t flags, uint64_t page_size, void **page_addr);
LV2_EXPORT int page_free(process_t process, void *page_addr, uint64_t flags);
LV2_EXPORT int page_export_to_proc(process_t process, void *page_addr, uint64_t flags, void **process_page_addr);
LV2_EXPORT int page_unexport_from_proc(process_t process, void *process_page_addr);

static INLINE int page_allocate_auto(process_t process, uint64_t size, uint64_t flags, void **page_addr)
{
	uint64_t page_size;
	
	if (size >= 0x100000)
	{
		size = (size+0xFFFFF) & ~0xFFFFF;
		page_size = MEMORY_PAGE_SIZE_1M;
	}
	else if (size >= 0x10000)
	{
		size = (size+0xFFFF) & ~0xFFFF; 
		page_size = MEMORY_PAGE_SIZE_64K;
	}
	else
	{
		if (size > 0x1000)		
			size = (size+0xFFF) & ~0xFFF;	
		else		
			size = 0x1000;		
		
		page_size = MEMORY_PAGE_SIZE_4K;
	}
	
	return page_allocate(process, size, flags, page_size, page_addr);
}

static INLINE int process_ea_to_lpar_addr(process_t process, void *ea_addr, uint64_t *lpar_addr)
{
	return process_ea_to_lpar_addr_ex(process->mem_object, ea_addr, lpar_addr);
}

static INLINE void *get_secure_user_ptr(void *ptr)
{
	return (void *)(((uint64_t)ptr)&0xFFFFFFFF);
}

static INLINE void get_slb(uint64_t entry, uint64_t *esid, uint64_t *vsid)
{
	uint64_t _esid, _vsid;
	
	__asm__ __volatile__(
        "slbmfev %0,%2\n"
        "slbmfee %1,%2\n"
        :"=&r"(_vsid), "=&r"(_esid)
        :"r"(entry)
        :"memory");
	
	*esid = _esid;
	*vsid = _vsid;
}

#endif /* __LV2_MEMORY_H__ */
