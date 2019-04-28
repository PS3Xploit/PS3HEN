#include <lv1/lv1.h>
#include <lv1/device.h>
#include <lv1/mm.h>
#include <lv1/stor.h>

#ifdef LV2
#include <lv2/libc.h>
#endif

#include <string.h>


#define STOR_BUS_ID							4


int lv1_stor_wrapper_open(u64 dev_id, void *dma_buf, u64 dma_buf_lpar_addr, u64 dma_size_log, lv1_stor_wrapper_var *stor_var)
{
	u64 dma_bus_addr, v2;
	int result;

	result = map_dma_mem(STOR_BUS_ID, dev_id, (void *)dma_buf_lpar_addr, (1<<dma_size_log), &dma_bus_addr);	
	if (result != 0)
		return result;	
	
	result = lv1_get_repository_node_value(PS3_LPAR_ID_PME, FIELD_FIRST("bus", STOR_BUS_ID), FIELD("dev", dev_id), FIELD("blk_size", 0), 0, &stor_var->block_size, &v2);
	if (result != 0)
	{
		unmap_dma_mem(STOR_BUS_ID, dev_id, dma_bus_addr, dma_size_log);		
		return result;
	}
	
	stor_var->dev_id = dev_id;
	stor_var->dma = dma_buf;
	stor_var->dma_lpar_addr = dma_buf_lpar_addr;
	stor_var->dma_size_log = dma_size_log;
	stor_var->dma_bus_addr = dma_bus_addr;
	
	return 0;
}

int lv1_stor_wrapper_close(lv1_stor_wrapper_var *stor_var)
{
	return unmap_dma_mem(STOR_BUS_ID, stor_var->dev_id, stor_var->dma_bus_addr, stor_var->dma_size_log);	
}

int lv1_stor_wrapper_read(lv1_stor_wrapper_var *stor_var, u64 region_id, u64 start_sector, u64 num_sectors, u64 flags, void *data)
{
	u64 tag, status;
	int result;

	result = lv1_storage_read(stor_var->dev_id, region_id, start_sector, num_sectors,
		flags, stor_var->dma_lpar_addr, &tag);
	if (result != 0)
		return result;

	for (;;)
	{
		result = lv1_storage_check_async_status(stor_var->dev_id, tag, &status);
		if (result != 0)
			continue;

		if (status == 0)
			break;
	}

	memcpy(data, stor_var->dma, stor_var->block_size * num_sectors);	

	return 0;
}


