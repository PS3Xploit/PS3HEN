#ifndef _STOR_H_
#define _STOR_H_

#include <lv1/lv1.h>

#define LV1_FLASH_DEV_ID	1
#define LV1_BDVD_DEV_ID		2

typedef struct 
{
	u64 dev_id;
	u64 block_size;
	void *dma;
	u64 dma_lpar_addr;
	u64 dma_size_log;
	u32 dma_bus_addr;
} lv1_stor_wrapper_var;

int lv1_stor_wrapper_open(u64 dev_id, void *dma_buf, u64 dma_buf_lpar_addr, u64 dma_size_log, lv1_stor_wrapper_var *stor_var);

int lv1_stor_wrapper_close(lv1_stor_wrapper_var *stor_var);

int lv1_stor_wrapper_read(lv1_stor_wrapper_var *stor_var, u64 region_id, u64 start_sector, u64 num_sectors, u64 flags, void *data);



#endif
