#include <lv2/lv2.h>
#include <lv2/error.h>
#include <lv2/libc.h>
#include <lv2/thread.h>
#include <lv2/security.h>
#include <lv2/memory.h>
#include "common.h"
#include "cobra.h"
#include "config.h"
#include "storage_ext.h"
#include "crypto.h"

int sys_cobra_usb_command(uint8_t command, uint8_t bmRequestType, uint32_t addr, void *buf, uint16_t size)
{	
	DPRINTF("App is calling sys_cobra_usb_command: %02X\n", command);
	
	if (size > 4096)
		return EINVAL;
	
	return 0;	
}


