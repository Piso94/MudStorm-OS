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

#include <flp.h>
#include <dma.h>
#include <io.h>
#include <timer.h>
#include <stdio.h>
#include <intr/irq.h>

enum flp_io
{
	flp_dor		= 0x3f2,
	flp_msr		= 0x3f4,
	flp_fifo	= 0x3f5,
	flp_ctrl	= 0x3f7
};

enum flp_cmd
{
	flp_cmd_read_track	= 2,
	flp_cmd_specify		= 3,
	flp_cmd_check_stat	= 4,
	flp_cmd_write_sect	= 5,
	flp_cmd_read_sect	= 6,
	flp_cmd_calibrate	= 7,
	flp_cmd_check_int	= 8,
	flp_cmd_format_track	= 0xd,
	flp_cmd_seek		= 0xf
};

enum flp_cmd_ext
{
	fdc_cmd_ext_skip	= 0x20,
	fdc_cmd_ext_density	= 0x40,
	fdc_cmd_ext_multitrack	= 0x80
};

enum flp_dor_mask
{
	flp_dor_mask_drive0	= 0,
	flp_dor_mask_drive1	= 1,
	flp_dor_mask_drive2	= 2,
	flp_dor_mask_drive3	= 3,
	flp_dor_mask_reset	= 4,
	flp_dor_mask_dma	= 8,
	
	flp_dor_mask_drive0_motor	= 16,
	flp_dor_mask_drive1_motor	= 32,
	flp_dor_mask_drive2_motor	= 64,
	flp_dor_mask_drive3_motor	= 128
};

enum flp_msr_mask
{
	flp_msr_mask_drive1_pos_mode	= 1,
	flp_msr_mask_drive2_pos_mode	= 2,
	flp_msr_mask_drive3_pos_mode	= 4,
	flp_msr_mask_drive4_pos_mode	= 8,
	flp_msr_mask_busy	= 16,
	flp_msr_mask_dma	= 32,
	flp_msr_mask_dataio	= 64,
	flp_msr_mask_datareg	= 128
};

enum flp_st0_mask
{
	flp_st0_mask_drive0	= 0,
	flp_st0_mask_drive1	= 1,
	flp_st0_mask_drive2	= 2,
	flp_st0_mask_drive3	= 3,
	flp_st0_mask_head	= 4,
	flp_st0_mask_notready	= 8,
	flp_st0_mask_unitcheck	= 16,
	flp_st0_mask_seekend	= 32,
	flp_st0_intcode		= 64
};

enum flp_st0_intcode_type
{
	flp_st0_type_normal	= 0,
	flp_st0_type_abnormal	= 1,
	flp_st0_type_invalid	= 2,
	flp_st0_type_notread	= 3
};

enum flp_gap3_length
{
	flp_gap3_length_std	= 42,
	flp_gap3_length_5_14	= 32,
	flp_gap3_length_3_5	= 27
};

enum flp_sector_dtl
{
	flp_sector_dtl_128	= 0,
	flp_sector_dtl_256	= 1,
	flp_sector_dtl_512	= 2,
	flp_sector_dtl_1024	= 4
};

const int flp_irq = 6;
const int flp_sector_per_track = 18;
const int dma_buffer = 0x1000;
const int fdc_dma_channel = 2;

static uint8_t current_drive = 0;
volatile uint8_t flp_disk_irq = 0;

bool dma_initialize_flp(uint8_t *buff, size_t length)
{
	union
	{
		uint8_t byte[4]; // Low[0], Middle[1], High[2]
		uint32_t l;
	} a, c;

	a.l = (size_t)buff;
	c.l = (size_t)length - 1;
	
	if ((a.l >> 24) || (c.l >> 16) || (((a.l & 0xffff) + c.l) >> 16))
	{
		printk("\nDMA Errore!");
		return false;
	}

	dma_reset(1);
	dma_mask_channel(fdc_dma_channel);
	dma_reset_flipflop(1);
	dma_set_address(fdc_dma_channel, a.byte[0], a.byte[1]);
	dma_reset_flipflop(1);
	dma_set_count(fdc_dma_channel, c.byte[0], c.byte[1]);
	dma_set_read(fdc_dma_channel);
	dma_unmask_all(1);

	return true;
}

uint8_t flp_read_status()
{
	return inportb(flp_msr);
}

void flp_write_dor(uint8_t val)
{
	outportb(flp_dor, val);
}

void flp_send_cmd(uint8_t cmd)
{
	for (int i=0;i<500;i++)
		if (flp_read_status() & flp_msr_mask_datareg)
			return outportb(flp_fifo, cmd);
}

uint8_t flp_read_data()
{
	for (int i=0;i<500;i++)
		if (flp_read_status() & flp_msr_mask_datareg)
			return inportb(flp_fifo);
	return 0;
}

void flp_write_ccr(uint8_t val)
{
	outportb(flp_ctrl, val);
}

inline void flp_wait_irq()
{
	while (flp_disk_irq == 0)
		;
	flp_disk_irq = 0;
}

void flp_handler()
{
	asm ("pusha");
	asm ("cli");
	flp_disk_irq = 1;
	asm ("sti");
	asm ("popa");
}

void flp_check_int(size_t *st0, size_t *cyl)
{
	flp_send_cmd(flp_cmd_check_int);
	
	*st0 = flp_read_data();
	*cyl = flp_read_data();
}

void flp_ctrl_motor (bool status)
{
	if (current_drive > 3)
		return;

	uint8_t motor = 0;

	switch (current_drive)
	{
		case 0:
			motor = flp_dor_mask_drive0_motor;
			break;
		case 1:
			motor = flp_dor_mask_drive1_motor;
			break;
		case 2:
			motor = flp_dor_mask_drive2_motor;
			break;
		case 3:
			motor = flp_dor_mask_drive3_motor;
			break;
	}
	
	if (status)
		flp_write_dor((uint8_t)(current_drive | motor | flp_dor_mask_reset | flp_dor_mask_dma));
	else
		flp_write_dor(flp_dor_mask_reset);

	delay_ms(5);
}

void flp_drive_data(uint8_t stepr, uint8_t loadt, uint8_t unloadt, bool dma)
{
	uint8_t data = 0;

	flp_send_cmd(flp_cmd_specify);
	data = (((stepr & 0xf) << 4) | (unloadt & 0xf));
	flp_send_cmd(data);
	data = ((loadt << 1) | ((dma) ? 0 : 1));
	flp_send_cmd(data);
}

int flp_calibrate (uint8_t drive)
{
	size_t st0, cyl;
	
	if (drive >= 4)
		return -2;
	
	flp_ctrl_motor(true);
	
	for (int i=0;i<10;i++)
	{
		flp_send_cmd(flp_cmd_calibrate);
		flp_send_cmd(drive);
		flp_wait_irq();
		flp_check_int(&st0, &cyl);
		
		if (!cyl)
		{
			flp_ctrl_motor(false);
			return 0;
		}
	}

	flp_ctrl_motor(false);
	return -1;
}

void flp_disable_ctrl()
{
	flp_write_dor(0);
}

void flp_enable_ctrl()
{
	flp_write_dor(flp_dor_mask_reset | flp_dor_mask_dma);
}

void flp_reset()
{
	size_t st0, cyl;
	
	flp_disable_ctrl();
	flp_enable_ctrl();
	flp_wait_irq();
	
	for(int i=0;i<4;i++)
		flp_check_int(&st0, &cyl);
	
	flp_write_ccr(0);

	flp_drive_data(3, 16, 240, true);

	flp_calibrate(current_drive);
}

void flp_read_sector_imp(uint8_t head, uint8_t track, uint8_t sector)
{
	size_t st0, cyl;
	
	if (!dma_initialize_flp((uint8_t*)dma_buffer, 512))
	{
		printk("\nDMA Buffer: Errore!");
		return;
	}

	dma_set_read(fdc_dma_channel);

	flp_send_cmd(flp_cmd_read_sect | fdc_cmd_ext_multitrack | fdc_cmd_ext_skip | fdc_cmd_ext_density);
	flp_send_cmd(head << 2 | current_drive);
	flp_send_cmd(track);
	flp_send_cmd(head);
	flp_send_cmd(sector);
	flp_send_cmd(flp_sector_dtl_512);
	flp_send_cmd(((sector + 1) >= flp_sector_per_track) ? flp_sector_per_track : (sector + 1));
	flp_send_cmd(flp_gap3_length_3_5);
	flp_send_cmd(0xff);

	flp_wait_irq();

	for(int j=0;j<7;j++)
		flp_read_data();

	flp_check_int(&st0, &cyl);
}

int flp_seek(uint8_t cyl, uint8_t head)
{
	size_t st0, cyl0;

	if (current_drive >= 4)
		return -1;
	
	for (int i=0;i<10;i++)
	{
		flp_send_cmd(flp_cmd_seek);
		flp_send_cmd((head << 2) | current_drive);
		flp_send_cmd(cyl);
		
		flp_wait_irq();
		flp_check_int(&st0, &cyl0);

		if (cyl0 == cyl)
			return 0;
	}
	
	return -1;
}

void flp_lba_to_chs(int lba, int *head, int *track, int *sector)
{
	*head = (lba % (flp_sector_per_track * 2)) / flp_sector_per_track;
	*track = lba / (flp_sector_per_track * 2);
	*sector = lba % (flp_sector_per_track + 1);
}

void flp_install()
{
	irq_install_handler(6, flp_handler);
	flp_reset();
	flp_drive_data(13, 1, 0xf, true);
}

void flp_uninstall()
{
	irq_uninstall_handler(6);
}

void flp_set_working_drive(uint8_t drive)
{
	if (drive < 4)
		current_drive = drive;
}

uint8_t flp_get_working_drive()
{
	return current_drive;
}

uint8_t *flp_read_sector(int sector_lba)
{
	if (current_drive >= 4)
	{
		printk("\nFloppy: Errore Drive!\n");
		return (uint8_t*)-1;
	}
	
	int head = 0, track = 0, sector = 1;
	
	flp_lba_to_chs(sector_lba, &head, &track, &sector);

	flp_ctrl_motor(true);

	if (flp_seek((uint8_t)track, (uint8_t)head) != 0)
	{
		printk("\nFloppy: Errore Seek!\n");
		return (uint8_t*)-1;
	}

	flp_read_sector_imp((uint8_t)head, (uint8_t)track, (uint8_t)sector);
	flp_ctrl_motor(false);

	return (uint8_t*)dma_buffer;
}

void flp_write_sector_imp(uint8_t head, uint8_t track, uint8_t sector)
{
	size_t st0, cyl;
	
	dma_set_write();
	
	flp_send_cmd(flp_cmd_write_sect | fdc_cmd_ext_multitrack | fdc_cmd_ext_density);
	flp_send_cmd(head << 2 | current_drive);
	flp_send_cmd(track);
	flp_send_cmd(head);
	flp_send_cmd(sector);
	flp_send_cmd(flp_sector_dtl_512);
	flp_send_cmd(flp_sector_per_track);
	flp_send_cmd(flp_gap3_length_3_5);
	flp_send_cmd(0xFF);

	flp_wait_irq();
	
	for (int i=0;i<7;i++)
		flp_read_data();

	flp_check_int(&st0, &cyl);
}

uint8_t *flp_write_sector(int sector_lba)
{
	if (current_drive >= 4)
	{
		printk("\nFloppy: Errore Drive!\n");
		return (uint8_t*)-1;
	}

	int head = 0, track = 0, sector = 1;
	flp_lba_to_chs(sector_lba, &head, &track, &sector);
	flp_ctrl_motor(true);
	
	if (flp_seek((uint8_t)track, (uint8_t)head) != 0)
	{
		printk("\nFloppy: Errore Seek!\n");
		return (uint8_t*)-1;
	}

	flp_write_sector_imp((uint8_t)head, (uint8_t)track, (uint8_t)sector);
	flp_ctrl_motor(false);
	
	return (uint8_t*)0;
}
