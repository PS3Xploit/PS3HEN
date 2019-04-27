/*  device.h - lv1 device functions

Copyright (C) 2010  Hector Martin "marcan" <hector@marcansoft.com

This code is licensed to you under the terms of the GNU GPL, version 2;
see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#ifndef DEVICE_H
#define DEVICE_H

#include <stdio.h>
#include <lv1/lv1.h>

enum 
{
	DEV_TYPE_STOR_DISK = 0,
	DEV_TYPE_GELIC = 3,
	DEV_TYPE_USB = 4,
	DEV_TYPE_STOR_ROM = 5,
	DEV_TYPE_SB_GPIO = 6,
	DEV_TYPE_STOR_FLASH = 14,
};

int map_dma_mem(int bus_id, int dev_id, void *start, size_t len, u64 *bus_addr);
int unmap_dma_mem(int bus_id, int dev_id, u64 bus_addr, size_t len);

int find_device_by_type(int type, int index, int *bus_id, int *dev_id, int *irq);

#endif
