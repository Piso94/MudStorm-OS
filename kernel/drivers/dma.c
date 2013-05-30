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

#include <dma.h>
#include <io.h>

void dma_mask_channel(uint8_t ch)
{
	if (ch <= 4)
		outportb(dma0_chanmask_reg, (1 << (ch - 1)));
	else
		outportb(dma1_chanmask_reg, (1 << (ch - 5)));
}

void dma_unmask_channel(uint8_t ch)
{
	if (ch <= 4)
		outportb(dma0_chanmask_reg, ch);
	else
		outportb(dma1_chanmask_reg, ch);
}

void dma_unmask_all(int dma)
{
	outportb(dma1_unmask_all_reg, 0xff);
}

void dma_reset(int dma)
{
	outportb(dma0_temp_reg, 0xff);
}

void dma_reset_flipflop(int dma)
{
	if (dma < 2)
		return;

	outportb((dma==0) ? dma0_clearbyte_flipflop_reg : dma1_clearbyte_flipflop_reg, 0xff);
}



void dma_set_address(uint8_t ch, uint8_t low, uint8_t high)
{
	if (ch > 8)
		return;

	uint16_t port = 0;
	switch (ch)
	{
		case 0:
			port = dma0_chan0_addr_reg;
			break;
		case 1:
			port = dma0_chan1_addr_reg;
			break;
		case 2:
			port = dma0_chan2_addr_reg;
			break;
		case 3:
			port = dma0_chan3_addr_reg;
			break;
		case 4:
			port = dma1_chan4_addr_reg;
			break;
		case 5:
			port = dma1_chan5_addr_reg;
			break;
		case 6:
			port = dma1_chan6_addr_reg;
			break;
		case 7:
			port = dma1_chan7_addr_reg;
			break;
	}
	outportb(port, low);
	outportb(port, high);
}

void dma_set_count(uint8_t ch, uint8_t low, uint8_t high)
{
	if (ch > 8)
		return;

	uint16_t port = 0;
	switch (ch)
	{
		case 0:
			port = dma0_chan0_count_reg;
			break;
		case 1:
			port = dma0_chan1_count_reg;
			break;
		case 2:
			port = dma0_chan2_count_reg;
			break;
		case 3:
			port = dma0_chan3_count_reg;
			break;
		case 4:
			port = dma1_chan4_count_reg;
			break;
		case 5:
			port = dma1_chan5_count_reg;
			break;
		case 6:
			port = dma1_chan6_count_reg;
			break;
		case 7:
			port = dma1_chan7_count_reg;
			break;
	}
	outportb(port, low);
	outportb(port, high);
}

void dma_set_mode(uint8_t ch, uint8_t mode)
{
	int dma = (ch < 4) ? 0 : 1;
	int chan = (dma == 0) ? ch : ch - 4;

	dma_mask_channel(ch);
	outportb((ch < 4) ? (dma0_mode_reg) : dma1_mode_reg, chan | mode);
	dma_unmask_all(dma);
}

void dma_set_read(uint8_t ch)
{
	dma_set_mode(ch, dma_mode_write_transfer | dma_mode_transfer_single);
}

void dma_set_write()
{
	outportb(0x0a, 0x06);
	outportb(0x0b, 0x5A);
	outportb(0x0a, 0x02);
}

void dma_set_external_page_register(uint8_t reg, uint8_t val)
{
	if (reg > 14)
		return;
	
	uint16_t port = 0;
	switch (reg)
	{
		case 1:
			port = dma_page_chan1_addrbyte2;
			break;
		case 2:
			port = dma_page_chan2_addrbyte2;
			break;
		case 3:
			port = dma_page_chan3_addrbyte2;
			break;
		case 4:
			return;
		case 5:
			port = dma_page_chan5_addrbyte2;
			break;
		case 6:
			port = dma_page_chan6_addrbyte2;
			break;
		case 7:
			port = dma_page_chan7_addrbyte2;
			break;
	}
	outportb(port, val);
}
