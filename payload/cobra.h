#ifndef __COBRA_H__
#define __COBRA_H__

int cobra_usb_command(uint8_t command, uint8_t bmRequestType, uint32_t addr, void *buf, uint16_t size);

/* Syscalls */
int sys_cobra_usb_command(uint8_t command, uint8_t bmRequestType, uint32_t addr, void *buf, uint16_t size);

#endif /* __COBRA_H__ */

