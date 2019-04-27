/*  gelic.h - definitons for the debug.h - printf message logging via Lv-1 Ethernet

Copyright (C) 2010  Hector Martin "marcan" <hector@marcansoft.com

This code is licensed to you under the terms of the GNU GPL, version 2;
see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

Portions taken from Linux (drivers/net/ps3_gelic_net.h):

 *  PS3 Platfom gelic definitions.
 *
 * Copyright (C) 2010 Hector Martin "marcan" <hector@marcansoft.com>
 *
 * Copyright (C) 2007 Sony Computer Entertainment Inc.
 * Copyright 2006, 2007 Sony Corporation.
 *
 * This file is based on: spider_net.h
 *
 * (C) Copyright IBM Corp. 2005
 *
 * Authors : Utz Bacher <utz.bacher@de.ibm.com>
 *           Jens Osterkamp <Jens.Osterkamp@de.ibm.com>
*/

#ifndef GELIC_H
#define GELIC_H

#include <stdint.h>


 /* RX descriptor data_status bits */
enum gelic_descr_rx_status 
{
	GELIC_DESCR_RXDMADU		= 0x80000000, /* destination MAC addr unknown */
	GELIC_DESCR_RXLSTFBF	= 0x40000000, /* last frame buffer            */
	GELIC_DESCR_RXIPCHK		= 0x20000000, /* IP checksum performed        */
	GELIC_DESCR_RXTCPCHK	= 0x10000000, /* TCP/UDP checksup performed   */
	GELIC_DESCR_RXWTPKT		= 0x00C00000, /*
									       * wakeup trigger packet
									       * 01: Magic Packet (TM)
									       * 10: ARP packet
									       * 11: Multicast MAC addr
									       */
	GELIC_DESCR_RXVLNPKT	= 0x00200000,  /* VLAN packet */
	/* bit 20..16 reserved */
	GELIC_DESCR_RXRRECNUM	= 0x0000ff00,  /* reception receipt number */
	/* bit 7..0 reserved */
};

#define GELIC_DESCR_DATA_STATUS_CHK_MASK	\
	(GELIC_DESCR_RXIPCHK | GELIC_DESCR_RXTCPCHK)

 /* TX descriptor data_status bits */
enum gelic_descr_tx_status 
{
	GELIC_DESCR_TX_TAIL	= 0x00000001, /* gelic treated this
								       * descriptor was end of
								       * a tx frame
								       */
};

/* RX descriptor data error bits */
enum gelic_descr_rx_error 
{
	/* bit 31 reserved */
	GELIC_DESCR_RXALNERR	= 0x40000000, /* alignement error 10/100M */
	GELIC_DESCR_RXOVERERR	= 0x20000000, /* oversize error */
	GELIC_DESCR_RXRNTERR	= 0x10000000, /* Runt error */
	GELIC_DESCR_RXIPCHKERR	= 0x08000000, /* IP checksum  error */
	GELIC_DESCR_RXTCPCHKERR	= 0x04000000, /* TCP/UDP checksum  error */
	GELIC_DESCR_RXDRPPKT	= 0x00100000, /* drop packet */
	GELIC_DESCR_RXIPFMTERR	= 0x00080000, /* IP packet format error */
	/* bit 18 reserved */
	GELIC_DESCR_RXDATAERR	= 0x00020000, /* IP packet format error */
	GELIC_DESCR_RXCALERR	= 0x00010000, /* cariier extension length
									       * error */
	GELIC_DESCR_RXCREXERR	= 0x00008000, /* carrier extention error */
	GELIC_DESCR_RXMLTCST	= 0x00004000, /* multicast address frame */
	/* bit 13..0 reserved */
};

#define GELIC_DESCR_DATA_ERROR_CHK_MASK		\
	(GELIC_DESCR_RXIPCHKERR | GELIC_DESCR_RXTCPCHKERR)

/* DMA command and status (RX and TX)*/
enum gelic_descr_dma_status 
{
	GELIC_DESCR_DMA_COMPLETE            = 0x00000000, /* used in tx */
	GELIC_DESCR_DMA_BUFFER_FULL         = 0x00000000, /* used in rx */
	GELIC_DESCR_DMA_RESPONSE_ERROR      = 0x10000000, /* used in rx, tx */
	GELIC_DESCR_DMA_PROTECTION_ERROR    = 0x20000000, /* used in rx, tx */
	GELIC_DESCR_DMA_FRAME_END           = 0x40000000, /* used in rx */
	GELIC_DESCR_DMA_FORCE_END           = 0x50000000, /* used in rx, tx */
	GELIC_DESCR_DMA_CARDOWNED           = 0xa0000000, /* used in rx, tx */
	GELIC_DESCR_DMA_NOT_IN_USE          = 0xb0000000, /* any other value */
};

#define GELIC_DESCR_DMA_STAT_MASK	(0xf0000000)

/* tx descriptor command and status */
enum gelic_descr_tx_dma_status 
{
	/* [19] */
	GELIC_DESCR_TX_DMA_IKE			= 0x00080000, /* IPSEC off */
	/* [18] */
	GELIC_DESCR_TX_DMA_FRAME_TAIL	= 0x00040000, /* last descriptor of
											       * the packet
											       */
	/* [17..16] */
	GELIC_DESCR_TX_DMA_TCP_CHKSUM	= 0x00020000, /* TCP packet */
	GELIC_DESCR_TX_DMA_UDP_CHKSUM	= 0x00030000, /* UDP packet */
	GELIC_DESCR_TX_DMA_NO_CHKSUM	= 0x00000000, /* no checksum */

	/* [1] */
	GELIC_DESCR_TX_DMA_CHAIN_END	= 0x00000002, /* DMA terminated
											       * due to chain end
											       */
};

#define GELIC_DESCR_DMA_CMD_NO_CHKSUM	\
	(GELIC_DESCR_DMA_CARDOWNED | GELIC_DESCR_TX_DMA_IKE | \
	GELIC_DESCR_TX_DMA_NO_CHKSUM)

#define GELIC_DESCR_DMA_CMD_TCP_CHKSUM	\
	(GELIC_DESCR_DMA_CARDOWNED | GELIC_DESCR_TX_DMA_IKE | \
	GELIC_DESCR_TX_DMA_TCP_CHKSUM)

#define GELIC_DESCR_DMA_CMD_UDP_CHKSUM	\
	(GELIC_DESCR_DMA_CARDOWNED | GELIC_DESCR_TX_DMA_IKE | \
	GELIC_DESCR_TX_DMA_UDP_CHKSUM)

enum gelic_descr_rx_dma_status 
{
	/* [ 1 ] */
	GELIC_DESCR_RX_DMA_CHAIN_END	= 0x00000002, /* DMA terminated
											       * due to chain end
											       */
};

/* for lv1_net_control */
enum gelic_lv1_net_control_code 
{
	GELIC_LV1_GET_MAC_ADDRESS		= 1,
	GELIC_LV1_GET_ETH_PORT_STATUS	= 2,
	GELIC_LV1_SET_NEGOTIATION_MODE	= 3,
	GELIC_LV1_GET_VLAN_ID			= 4,
	GELIC_LV1_SET_WOL				= 5,
	GELIC_LV1_GET_CHANNEL			= 6,
	GELIC_LV1_POST_WLAN_CMD			= 9,
	GELIC_LV1_GET_WLAN_CMD_RESULT	= 10,
	GELIC_LV1_GET_WLAN_EVENT		= 11,
};

/* for GELIC_LV1_WOL_MAGIC_PACKET */
enum gelic_lv1_wol_mp_arg 
{
	GELIC_LV1_WOL_MP_DISABLE	= 0,
	GELIC_LV1_WOL_MP_ENABLE		= 1,
};

/* for GELIC_LV1_WOL_{ADD,DELETE}_MATCH_ADDR */
enum gelic_lv1_wol_match_arg 
{
	GELIC_LV1_WOL_MATCH_INDIVIDUAL	= 0,
	GELIC_LV1_WOL_MATCH_ALL			= 1,
};

/* status returened from GET_ETH_PORT_STATUS */
enum gelic_lv1_ether_port_status 
{
	GELIC_LV1_ETHER_LINK_UP		= 0x0000000000000001L,
	GELIC_LV1_ETHER_FULL_DUPLEX	= 0x0000000000000002L,
	GELIC_LV1_ETHER_AUTO_NEG	= 0x0000000000000004L,

	GELIC_LV1_ETHER_SPEED_10	= 0x0000000000000010L,
	GELIC_LV1_ETHER_SPEED_100	= 0x0000000000000020L,
	GELIC_LV1_ETHER_SPEED_1000	= 0x0000000000000040L,
	GELIC_LV1_ETHER_SPEED_MASK	= 0x0000000000000070L,
};

enum gelic_lv1_vlan_index 
{
	/* for outgoing packets */
	GELIC_LV1_VLAN_TX_ETHERNET_0	= 0x0000000000000002L,
	GELIC_LV1_VLAN_TX_WIRELESS		= 0x0000000000000003L,

	/* for incoming packets */
	GELIC_LV1_VLAN_RX_ETHERNET_0	= 0x0000000000000012L,
	GELIC_LV1_VLAN_RX_WIRELESS		= 0x0000000000000013L,
};

enum gelic_lv1_phy 
{
	GELIC_LV1_PHY_ETHERNET_0		= 0x0000000000000002L,
};

/* size of hardware part of gelic descriptor */
#define GELIC_DESCR_SIZE	(32)

enum gelic_port_type 
{
	GELIC_PORT_ETHERNET_0	= 0,
	GELIC_PORT_WIRELESS	= 1,
	GELIC_PORT_MAX
};

struct gelic_descr 
{
	/* as defined by the hardware */
	u32 buf_addr;
	u32 buf_size;
	u32 next_descr_addr;
	u32 dmac_cmd_status;
	u32 result_size;
	u32 valid_size;	/* all zeroes for tx */
	u32 data_status;
	u32 data_error;	/* all zeroes for tx */
} __attribute__((aligned(32)));

#endif
