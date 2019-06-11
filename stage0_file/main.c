#include <lv2/lv2.h>
#include <lv2/memory.h>
#include <lv2/io.h>
#include <lv2/libc.h>
#include <lv2/thread.h>
#include <lv2/patch.h>
#include <lv1/lv1.h>

//#define STAGE2_FILE_STAT	0x8e000050
//#define STAGE2_FILE_NREAD	0x8e000008
#define STAGE2_LOCATION		0x8a080000
#define SPRX_LOCATION		0x8a070000

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
//	f_desc_t f;
//	int (* func)(void);	
	HEN_CONFIG CONFIG;
	CONFIG.config_hdl=*(uint32_t *)0x8d000500;
	CONFIG.service_hdl1=*(uint32_t *)0x8D0FF050;
	CONFIG.service_hdl2=*(uint32_t *)0x8D0FF054;
	CONFIG.service_hdl3=*(uint32_t *)0x8D0FF058;
	
	#if defined (FIRMWARE_4_82DEX) ||  defined (FIRMWARE_4_84DEX)
	CONFIG.service_hdl4=*(uint32_t *)0x8D0FF05c;
	#else
	CONFIG.service_hdl4=0;
	#endif
	
	uint64_t read;
	int dst;
	if(cellFsOpen("/dev_hdd0/HENCONFIG",CELL_FS_O_WRONLY|CELL_FS_O_CREAT|CELL_FS_O_TRUNC, &dst, 0666, NULL, 0)==0)
	{
		cellFsWrite(dst, &CONFIG, sizeof(HEN_CONFIG), &read);
		cellFsClose(dst);
	}

//	CellFsStat *stat=(CellFsStat *)STAGE2_FILE_STAT;

	for (int i = 0; i < 128; i++)
	{
		uint64_t pte0 = *(uint64_t *)(MKA(0xf000000 | (i<<7)));
		uint64_t pte1 = *(uint64_t *)(MKA(0xf000000 | ((i<<7)+8)));
		
		lv1_write_htab_entry(0, i << 3, pte0, (pte1 & 0xff0000) | 0x190);
	}

	uint64_t size;
//	size=stat->st_size;
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
		uint32_t sce_bytes=0x53434500;
		*(uint32_t *)0x8a000000=sce_bytes; //hen check
		uint64_t header_len=*(uint64_t *)(SPRX_LOCATION+0x10);
		uint64_t data_len=*(uint64_t *)(SPRX_LOCATION+0x18);
		uint64_t size=header_len+data_len;
		if(cellFsOpen("/dev_hdd0/HENplugin.sprx",CELL_FS_O_WRONLY|CELL_FS_O_CREAT|CELL_FS_O_TRUNC, &dst, 0666, NULL, 0)==0)
		{
			cellFsWrite(dst, (void*)SPRX_LOCATION, size, &size);
			cellFsClose(dst);
		}
//		f.addr = stage2;	
//		f.toc = (void *)MKA(TOC);
//		func = (void *)&f;	
//		func();		
		return stage2;
	}

	return NULL;
}
