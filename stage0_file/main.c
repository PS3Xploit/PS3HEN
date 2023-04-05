#include <lv2/lv2.h>
#include <lv2/memory.h>
#include <lv2/io.h>
#include <lv2/libc.h>
#include <lv2/thread.h>
#include <lv2/patch.h>
#include <lv2/syscall.h>
#include <lv1/lv1.h>

#define STAGE2_LOCATION		0x8a080000
#define SPRX_LOCATION		0x8a070000
#define SYS_EVENT_QUEUE_DESTROY_FORCE	1

void syscall_unregister_service(void);

typedef struct hen_config
{
	uint32_t config_hdl;
	uint32_t service_hdl1;
	uint32_t service_hdl2;
	uint32_t service_hdl3;
	uint32_t service_hdl4; //only on dex atm
} HEN_CONFIG;

void *main(void)
{
	void *stage2 = NULL;
	f_desc_t f;	
	cellFsUnlink("/dev_hdd0/HENplugin.sprx");	
	#if defined(FIRMWARE_4_82DEX) || defined (FIRMWARE_4_84DEX)
	HEN_CONFIG CONFIG;
	CONFIG.config_hdl=*(uint32_t *)0x8d000500;
	CONFIG.service_hdl1=*(uint32_t *)0x8D0FF050;
	CONFIG.service_hdl2=*(uint32_t *)0x8D0FF054;
	CONFIG.service_hdl3=*(uint32_t *)0x8D0FF058;
	CONFIG.service_hdl4=*(uint32_t *)0x8D0FF05c;
	typedef uint32_t sys_event_queue_t;
	sys_event_queue_t to_be_destroyed=*(uint32_t *)0x8D0FF060;

	if(CONFIG.service_hdl1)
	{
		f.addr=syscall_unregister_service;
		f.toc=(void*)MKA(TOC);
		int (*unregister_service)(uint32_t config_hdl,uint32_t service_hdl)=(void*)&f;
		unregister_service(CONFIG.config_hdl,CONFIG.service_hdl1);
		unregister_service(CONFIG.config_hdl,CONFIG.service_hdl2);
		unregister_service(CONFIG.config_hdl,CONFIG.service_hdl3);
		unregister_service(CONFIG.config_hdl,CONFIG.service_hdl4); 
		
		
		f.addr=(void*)MKA(get_syscall_address(0x205));
		int(*config_close)(uint32_t hdl)=(void*)&f;
		config_close(CONFIG.config_hdl);
		
		f.addr=(void*)MKA(get_syscall_address(0x81));
		int(*event_queue_destroy_user)(sys_event_queue_t equeu_id, int mode)=(void*)&f;
		event_queue_destroy_user(to_be_destroyed,SYS_EVENT_QUEUE_DESTROY_FORCE);
	}
	#endif
	
	uint64_t sc_null = *(uint64_t *)MKA(syscall_table_symbol);
	uint64_t sc_8 = *(uint64_t *)((MKA(syscall_table_symbol))+8*8);
	if(sc_8!=sc_null)
	{
		uint32_t sce_bytes=0x53434500;
		*(uint32_t *)0x8a000000=sce_bytes; //hen check
		return NULL;
	}
	
	int dst;

	for (int i = 0; i < 127; i++)
	{
		uint64_t pte0 = *(uint64_t *)(MKA(0xf000000 | (i<<7)));
		uint64_t pte1 = *(uint64_t *)(MKA(0xf000000 | ((i<<7)+8)));
		
		lv1_write_htab_entry(0, i << 3, pte0, (pte1 & 0xff0000) | 0x190);
	}

	uint64_t size;
	size=*(uint64_t *)(STAGE2_LOCATION-8);
	if(size) // Thanks to @aldostools, this will prevent hang if binary does not have stage2
	{
		stage2=alloc(size,0x27);
		if (stage2)
		{		
			memcpy(stage2,(void *)STAGE2_LOCATION,size);
		}
	}
	
	if (stage2)
	{
		uint64_t header_len=*(uint64_t *)(SPRX_LOCATION+0x10);
		uint64_t data_len=*(uint64_t *)(SPRX_LOCATION+0x18);
		uint64_t size=header_len+data_len;
		uint64_t size1;
		for(uint8_t retry = 0; retry < 10; retry++)
		{
			timer_usleep(100000);
			if(cellFsOpen("/dev_hdd0/HENplugin.sprx",CELL_FS_O_WRONLY|CELL_FS_O_CREAT|CELL_FS_O_TRUNC, &dst, 0666, NULL, 0)==0)
			{
				cellFsWrite(dst, (void*)SPRX_LOCATION, size, &size1);
				cellFsClose(dst);
				uint32_t sce_bytes=0x53434500;
				*(uint32_t *)0x8a000000=sce_bytes; //hen check
				timer_usleep(200000);
				if(size1>=size)
					return stage2;
				else
					break;
			}
		}
	}

	return NULL;
}
