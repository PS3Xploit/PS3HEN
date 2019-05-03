
/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _VUART_H_
#define _VUART_H_

#define VUART_PARAM_TX_TRIGGER					0
#define VUART_PARAM_RX_TRIGGER					1
#define VUART_PARAM_INTERRUPT_MASK				2
#define VUART_PARAM_RX_BUFSIZE					3
#define VUART_PARAM_RX_BYTES					4
#define VUART_PARAM_TX_BUFSIZE					5
#define VUART_PARAM_TX_BYTES					6
#define VUART_PARAM_INTERRUPT_STATUS			7

#define VUART_INTERRUPT_MASK_TX					0x00000001
#define VUART_INTERRUPT_MASK_RX					0x00000002
#define VUART_INTERRUPT_MASK_DISCONNECT			0x00000004

#endif
