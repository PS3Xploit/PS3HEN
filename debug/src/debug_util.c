#ifdef LV2

#include <lv2/modules.h>
#include <lv2/memory.h>
#include <lv2/libc.h>
#include <debug_util.h>

#define MAX_PRX	256

void debug_print_modules(process_t process)
{
	sys_prx_id_t *list;
	uint32_t *unk_list;
	uint32_t n, n2;
	
	list = alloc(sizeof(sys_prx_id_t)*MAX_PRX, 0x35);
	unk_list = alloc(sizeof(uint32_t)*MAX_PRX, 0x35);
	
	if (prx_get_module_list(process, list, unk_list, MAX_PRX, &n, &n2) == 0)
	{
		for (uint32_t i = 0; i < n; i++)
		{		
			sys_prx_module_info_t modinfo;
			sys_prx_segment_info_t text_segment;
		
			memset(&modinfo, 0, sizeof(modinfo));
			modinfo.segments_num = 1;
		
			if (prx_get_module_info(process, list[i], &modinfo, NULL, &text_segment) == 0)			
				printf("<%s> [%08lX]\n", modinfo.name, text_segment.base);			
		}
	}
	
	dealloc(list, 0x35);
	dealloc(unk_list, 0x35);
}

#endif

