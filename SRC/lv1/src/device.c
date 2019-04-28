/*  device.c - lv1 device functions

Copyright (C) 2010  Hector Martin "marcan" <hector@marcansoft.com

This code is licensed to you under the terms of the GNU GPL, version 2;
see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include <stdio.h>
#include <lv1/lv1.h>

#if defined(LV2)
#include <lv2/lv2.h>
#include <lv2/memory.h>
#endif

#define printf(...)

int map_dma_mem(int bus_id, int dev_id, void *start, size_t len, u64 *r_bus_addr)
{
	s64 result;
	u64 real_addr = (u64)start;
	real_addr &= ~0x8000000000000000ULL;
	u64 real_end = real_addr + len;
	u64 map_start = real_addr & ~0xfff;
	u64 map_end = (real_end + 0xfff) & ~0xfff;
	u64 bus_addr;

	u64 flags = 0xf800000000000000UL;

	result = lv1_allocate_device_dma_region(bus_id, dev_id, map_end - map_start, 12, 0, &bus_addr);
	if (result)
		return result;
	
	result = lv1_map_device_dma_region(bus_id, dev_id, map_start, bus_addr, map_end - map_start, flags);
	if (result) 
	{
		lv1_free_device_dma_region(bus_id, dev_id, bus_addr);
		return result;
	}

	*r_bus_addr = bus_addr + real_addr - map_start;
	return 0;
}

int unmap_dma_mem(int bus_id, int dev_id, u64 bus_addr, size_t len)
{
	s64 result;
	u64 real_bus_addr;

	real_bus_addr = bus_addr & ~0xfff;
	len += bus_addr - real_bus_addr;
	len = (len + 0xfff) & ~0xfff;

	result = lv1_unmap_device_dma_region(bus_id, dev_id, real_bus_addr, len);
	if (result)
		return result;

	return lv1_free_device_dma_region(bus_id, dev_id, real_bus_addr);
}

int find_device_by_type(int type, int index, int *pbus_id, int *pdev_id, int *pirq)
{
	u64 v2;
	u64 bus_ndx;
	s64 result;

	printf("Locating device with type %d and index %d...\n", type, index);
	for (bus_ndx=0; bus_ndx<10; bus_ndx++) {
		u64 bus_id=0, bus_type=0, num_dev=0;

		result = lv1_get_repository_node_value(PS3_LPAR_ID_PME, FIELD_FIRST("bus",bus_ndx),
											   FIELD("type",0), 0, 0, &bus_type, &v2);
		if (result)
			continue;
		result = lv1_get_repository_node_value(PS3_LPAR_ID_PME, FIELD_FIRST("bus",bus_ndx),
											   FIELD("id",0), 0, 0, &bus_id, &v2);
		if (result)
			continue;
		result = lv1_get_repository_node_value(PS3_LPAR_ID_PME, FIELD_FIRST("bus",bus_ndx),
											   FIELD("num_dev",0), 0, 0, &num_dev, &v2);
		if (result)
			continue;

		printf("Bus #%ld id %ld type %ld num_dev %ld\n", bus_ndx, bus_id, bus_type, num_dev);
		u64 dev_ndx;
		for (dev_ndx=0; dev_ndx<num_dev; dev_ndx++) 
		{
			s64 dev_id=0, dev_type=0, dev_intr=0;

			result = lv1_get_repository_node_value(PS3_LPAR_ID_PME, FIELD_FIRST("bus",bus_ndx),
												   FIELD("dev",dev_ndx), FIELD("id",0), 0, (u64*)&dev_id, &v2);
			if (result)
				dev_id = -1;
			result = lv1_get_repository_node_value(PS3_LPAR_ID_PME, FIELD_FIRST("bus",bus_ndx),
												   FIELD("dev",dev_ndx), FIELD("type",0), 0, (u64*)&dev_type, &v2);
			if (result)
				dev_type = -1;
			result = lv1_get_repository_node_value(PS3_LPAR_ID_PME, FIELD_FIRST("bus",bus_ndx),
												   FIELD("dev",dev_ndx), FIELD("intr",0), 0, (u64*)&dev_intr, &v2);
			if (result)
				dev_intr = -1;

			printf("- Dev #%ld id %ld type %ld intr %ld\n", dev_ndx, dev_id, dev_type, dev_intr);
			
			if (dev_type == type) 
			{
				if (index) 
				{
					index--;
					continue;
				}

				printf("Device found: bus #%ld id %ld type %ld, dev #%ld id %ld type %ld intr %ld\n",
					   bus_ndx, bus_id, bus_type, dev_ndx, dev_id, dev_type, dev_intr);
				if (pbus_id)
					*pbus_id = bus_id;
				if (pdev_id)
					*pdev_id = dev_id;
				if (pirq)
					*pirq = dev_intr;
				return 0;
			}
		}
	}

	printf("Device not found\n");
	return -1;
}
