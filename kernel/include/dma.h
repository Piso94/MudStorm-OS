/*
 * MudStorm OS LittleHacker
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _DMA_H_
#define _DMA_H_

#include "stddef.h"

#define DMA_MAX_CHANNEL 16
#define DMA_CHANNEL_PER_DMAC 8

enum dma0_channel_io
{
	dma0_chan0_addr_reg	= 0,
	dma0_chan0_count_reg	= 1,
	dma0_chan1_addr_reg	= 2,
	dma0_chan1_count_reg	= 3,
	dma0_chan2_addr_reg	= 4,
	dma0_chan2_count_reg	= 5,
	dma0_chan3_addr_reg	= 6,
	dma0_chan3_count_reg	= 7,
};

enum dma0_io
{
	dma0_status_reg		= 0x08,
	dma0_command_reg	= 0x08,
	dma0_request_reg	= 0x09,
	dma0_chanmask_reg	= 0x0a,
	dma0_mode_reg		= 0x0b,
	dma0_clearbyte_flipflop_reg	= 0x0c,
	dma0_temp_reg		= 0x0d,
	dma0_master_clear_reg	= 0x0d,
	dma0_clear_mask_reg	= 0x0e,
	dma0_mask_reg		= 0x0f
};

enum dma1_channel_io
{
	dma1_chan4_addr_reg	= 0xc0,
	dma1_chan4_count_reg	= 0xc2,
	dma1_chan5_addr_reg	= 0xc4,
	dma1_chan5_count_reg	= 0xc6,
	dma1_chan6_addr_reg	= 0xc8,
	dma1_chan6_count_reg	= 0xca,
	dma1_chan7_addr_reg	= 0xcc,
	dma1_chan7_count_reg	= 0xce,
};

enum dma0_page_reg
{
	dma_page_extra0		 = 0x80,
	dma_page_chan2_addrbyte2 = 0x81,
	dma_page_chan3_addrbyte2 = 0x82,
	dma_page_chan1_addrbyte2 = 0x83,
	dma_page_extra1		 = 0x84,
	dma_page_extra2		 = 0x85,
	dma_page_extra3		 = 0x86,
	dma_page_chan6_addrbyte2 = 0x87,
	dma_page_chan7_addrbyte2 = 0x88,
	dma_page_chan5_addrbyte2 = 0x89,
	dma_page_extra4		 = 0x8c,
	dma_page_extra5		 = 0x8d,
	dma_page_extra6		 = 0x8e,
	dma_page_dram_refresh	 = 0x8f
};

enum dma1_io
{
	dma1_status_reg		= 0xd0,
	dma1_command_reg	= 0xd0,
	dma1_request_reg	= 0xd2,
	dma1_chanmask_reg	= 0xd4,
	dma1_mode_reg		= 0xd6,
	dma1_clearbyte_flipflop_reg = 0xd8,
	dma1_inter_reg		= 0xda,
	dma1_unmask_all_reg	= 0xdc,
	dma1_mask_reg		= 0xde
};

enum dma_mode_reg_mask
{
	dma_mode_mask_sel	= 3,
	dma_mdoe_mask_tra	= 0xc,
	dma_mode_self_test	= 0,
	dma_mode_read_transfer	= 4,
	dma_mode_write_transfer	= 5,
	dma_mode_mask_auto	= 0x10,
	dma_mode_mask_idec	= 0x20,	
	dma_mode_mask		= 0xc0,
	dma_mode_transfer_on_demand = 0,
	dma_mode_transfer_single = 0x40,
	dma_mode_transfer_block = 0x80,
	dma_mode_transfer_cascade =0xC0
};

enum dma_cmd_reg_mask
{
	dma_cmd_mask_memtomem	= 1,
	dma_cmd_mask_chan0addrhold = 2,
	dma_cmd_mask_enable	= 4,
	dma_cmd_mask_timing	= 8,
	dma_cmd_mask_priority	= 0x10,
	dma_cmd_mask_writesel	= 0x20,
	dma_cmd_mask_dreq	= 0x40,
	dma_cmd_mask_dack	= 0x80
};

extern void dma_set_mode(uint8_t ch, uint8_t mode);
extern void dma_set_read(uint8_t ch);
extern void dma_set_write();
extern void dma_set_address(uint8_t ch, uint8_t low, uint8_t high);
extern void dma_set_count(uint8_t ch, uint8_t low, uint8_t high);
extern void dma_mask_channel(uint8_t ch);
extern void dma_unmask_channel(uint8_t ch);
extern void dma_reset_flipflop(int dma);
extern void dma_reset(int dma);
extern void dma_set_external_page_register(uint8_t reg, uint8_t val);
extern void dma_unmask_all(int dma);
#endif

