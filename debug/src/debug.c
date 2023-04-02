/*  debug.c - printf message logging via Lv-1 Ethernet

Copyright (C) 2010  Hector Martin "marcan" <hector@marcansoft.com

This code is licensed to you under the terms of the GNU GPL, version 2;
see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <debug.h>
#include <lv1/lv1.h>
#include <lv1/gelic.h>
#include <lv1/device.h>

#ifdef PS2EMU

#include <ps2emu/libc.h>
#include <ps2emu/symbols.h>
#include <ps2emu/patch.h>

#elif defined(LV2)

#include <lv2/patch.h>
#include <lv2/syscall.h>
#include <lv2/interrupt.h>
#include <lv2/libc.h>
#include <lv2/symbols.h>

#endif
#include "printf.h"

#undef printf

#define DEBUG_PORT 18194

static int bus_id;
static int dev_id;

uint64_t *ttywrite_sc;
static u64 bus_addr;

struct ethhdr 
{
	uint8_t dest[6];
	uint8_t src[6];
	uint16_t type;
} __attribute__((packed));

struct vlantag 
{
	uint16_t vlan;
	uint16_t subtype;
} __attribute__((packed));

struct iphdr 
{
	uint8_t ver_len;
	uint8_t dscp_ecn;
	uint16_t total_length;
	uint16_t ident;
	uint16_t frag_off_flags;
	uint8_t ttl;
	uint8_t proto;
	uint16_t checksum;
	uint32_t src;
	uint32_t dest;
} __attribute__((packed));

struct udphdr 
{
	uint16_t src;
	uint16_t dest;
	uint16_t len;
	uint16_t checksum;
} __attribute__((packed));

static struct ethhdr *h_eth;
static struct vlantag *h_vlan;
static struct iphdr *h_ip;
static struct udphdr *h_udp;

static char *pmsg;

#define MAX_MESSAGE_SIZE 1000

struct debug_block 
{
	volatile struct gelic_descr descr;
	uint8_t pkt[1520];
} __attribute__((packed));

struct debug_block dbg_ __attribute__((aligned(32)));

#define dbg (&dbg_)

static int debug_initialized = 0;
static int header_size;

#ifdef PS2EMU

void debug_install(void)
{
#ifdef PS2HWEMU
	change_function(printf_symbol, debug_printf);
#endif
}

void debug_uninstall(void)
{
}

#elif defined(LV2)

LV2_SYSCALL2(int, ttyWrite, (int channel, const char* message, int length, int* written))
{
	debug_print(message, length);
	if(written)
		*written=length;
	
	#ifdef DEBUG
	f_desc_t f;
	f.addr=(void*)ttywrite_sc;
	f.toc=(void*)MKA(TOC);
	int (*func)(int, const char *, int, int *)=(void *)&f;
    return func(channel, message, length, written);
	#endif
	
	return 0;
}

LV2_SYSCALL2(int, consoleWrite, (const char* message, int length))
{
	debug_print(message, length);			
	return 0;
}

void debug_install(void)
{
	suspend_intr();
	change_function(printf_symbol, debug_printf);
	change_function(printfnull_symbol, debug_printf);
	create_syscall2(SYS_TTY_WRITE, ttyWrite);
	create_syscall2(SYS_CONSOLE_WRITE, consoleWrite);
	resume_intr();
}

void debug_hook()
{
	change_function(printf_symbol, debug_printf);
	change_function(printfnull_symbol, debug_printf);
}

void debug_uninstall(void)
{
	suspend_intr();
	*(uint32_t *)MKA(printf_symbol)=0xF821FF51;
	clear_icache((void *)MKA(printf_symbol),4);		
	*(uint32_t *)MKA(printfnull_symbol)=0x38600000;
	clear_icache((void *)MKA(printfnull_symbol),4);
	resume_intr();
}

#endif

int64_t debug_init(void)
{
	uint64_t **table = (uint64_t **)MKA(syscall_table_symbol);
	f_desc_t *f = (f_desc_t *)table[SYS_TTY_WRITE];
	ttywrite_sc = (uint64_t *)f->addr;
	
	
	s64 result;
	u64 v2;

	result = find_device_by_type(DEV_TYPE_GELIC, 0, &bus_id, &dev_id, NULL);
	if (result)
		return result;

	result = map_dma_mem(bus_id, dev_id, dbg, sizeof(struct debug_block), &bus_addr);
	if (result)
		return result;

	memset(dbg, 0, sizeof(struct debug_block));

	dbg->descr.buf_addr = bus_addr + offsetof(struct debug_block, pkt);

	u64 mac;
	result = lv1_net_control(bus_id, dev_id, GELIC_LV1_GET_MAC_ADDRESS, 0, 0, 0, &mac, &v2);

	if (result)
		return result;

	mac <<= 16;

	h_eth = (struct ethhdr*)dbg->pkt;
	
	memset(&h_eth->dest, 0xff, 6);
	memcpy(&h_eth->src, &mac, 6);

	header_size = sizeof(struct ethhdr);

	u64 vlan_id;
	result = lv1_net_control(bus_id, dev_id, GELIC_LV1_GET_VLAN_ID, \
							 GELIC_LV1_VLAN_TX_ETHERNET_0, 0, 0, &vlan_id, &v2);
	if (result == 0) 
	{
		h_eth->type = 0x8100;

		header_size += sizeof(struct vlantag);
		h_vlan = (struct vlantag*)(h_eth+1);
		h_vlan->vlan = vlan_id;
		h_vlan->subtype = 0x0800;
		h_ip = (struct iphdr*)(h_vlan+1);
	} 
	else 
	{
		h_eth->type = 0x0800;
		h_ip = (struct iphdr*)(h_eth+1);
	}

	header_size += sizeof(struct iphdr);
	h_ip->ver_len = 0x45;
	h_ip->ttl = 10;
	h_ip->proto = 0x11;
	h_ip->src = 0x00000000;
	h_ip->dest = 0xffffffff;

	header_size += sizeof(struct udphdr);
	h_udp = (struct udphdr*)(h_ip+1);
	h_udp->src = DEBUG_PORT;
	h_udp->dest = DEBUG_PORT;

	pmsg = (char*)(h_udp+1);

	debug_initialized = 1;

	return 0;
}

int debug_end(void)
{
	lv1_net_stop_tx_dma(bus_id, dev_id, 0);
	return unmap_dma_mem(bus_id, dev_id, bus_addr, sizeof(struct debug_block));
}

int64_t debug_print(const char* buffer, size_t msgsize)
{
	if (msgsize > MAX_MESSAGE_SIZE)
		msgsize = MAX_MESSAGE_SIZE;

	if (buffer)
		memcpy(pmsg, buffer, msgsize);

	dbg->descr.buf_size = header_size + msgsize;
	h_ip->total_length = msgsize + sizeof(struct udphdr) + sizeof(struct iphdr);
	h_udp->len = msgsize + sizeof(struct udphdr);

	h_ip->checksum = 0;
	u32 sum = 0;
	u16 *p = (u16*)h_ip;
	int i;

	for (i=0; i<5; i++)
		sum += *p++;

	h_ip->checksum = ~(sum + (sum>>16));

	dbg->descr.dmac_cmd_status = GELIC_DESCR_DMA_CMD_NO_CHKSUM | GELIC_DESCR_TX_DMA_FRAME_TAIL;
	dbg->descr.result_size = 0;
	dbg->descr.data_status = 0;

	int ret = lv1_net_start_tx_dma(bus_id, dev_id, bus_addr, 0);

	if (ret)
		return ret;

	while ((dbg->descr.dmac_cmd_status & GELIC_DESCR_DMA_STAT_MASK) == GELIC_DESCR_DMA_CARDOWNED);
	return 0;
}

#ifdef PS2EMU
#ifdef PS2HWEMU
	PS2EMU_PATCHED_FUNCTION(int64_t, debug_printf, (const char *fmt, ...))
#else
//#define _debug_printf debug_printf
	int64_t _debug_printf(const char* fmt, ...)
#endif /* PS2HWEMU */
#elif defined(LV2)
	LV2_PATCHED_FUNCTION(int64_t,  debug_printf, (const char* fmt, ...))
#else
	int64_t debug_printf(const char* fmt, ...)
#endif
{
	va_list ap;

	if (!debug_initialized)
		return -1;

	va_start(ap, fmt);
	size_t msgsize = _vsnprintf(pmsg, MAX_MESSAGE_SIZE, fmt, ap);
	va_end(ap);

	return debug_print(NULL, msgsize);
}

void abort(void)
{
	_debug_printf("abort() called! Panicking.\n");
	lv1_panic(0);
	while (1);
}

void fatal(const char *msg)
{
	_debug_printf("FATAL: %s\n", msg);
	_debug_printf("Panicking.\n");
	lv1_panic(0);
	while (1);
}

void debug_print_hex(void *buf, uint64_t size)
{
	uint8_t *buf8 = (uint8_t *)buf;
	
	for (uint64_t i = 0; i < size; i++)
	{
		_debug_printf("%02X ", buf8[i]);
		
		if ((i&0xF) == 0xF || i == (size-1))
			_debug_printf("\n");
	}
}

void debug_print_hex_c(void *buf, uint64_t size)
{
	uint8_t *buf8 = (uint8_t *)buf;
	
	for (uint64_t i = 0; i < size; i++)
	{
		_debug_printf("0x%02X, ", buf8[i]);
		
		if ((i&0xF) == 0xF || i == (size-1))
			_debug_printf("\n");
	}
}


