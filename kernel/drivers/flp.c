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
#include <io.h>
#include <timer.h>
#include <intr/irq.h>

enum FLPYDSK_IO {

	FLPYDSK_DOR		=	0x3f2,
	FLPYDSK_MSR		=	0x3f4,
	FLPYDSK_FIFO	=	0x3f5,
	FLPYDSK_CTRL	=	0x3f7
};

/**
*	Bits 0-4 of command byte. Please see chapter for additional commands
*/

enum FLPYDSK_CMD {

	FDC_CMD_READ_TRACK	=	2,
	FDC_CMD_SPECIFY		=	3,
	FDC_CMD_CHECK_STAT	=	4,
	FDC_CMD_WRITE_SECT	=	5,
	FDC_CMD_READ_SECT	=	6,
	FDC_CMD_CALIBRATE	=	7,
	FDC_CMD_CHECK_INT	=	8,
	FDC_CMD_FORMAT_TRACK=	0xd,
	FDC_CMD_SEEK		=	0xf
};

/**
*	Additional command masks. Can be masked with above commands
*/

enum FLPYDSK_CMD_EXT {

	FDC_CMD_EXT_SKIP		=	0x20,	//00100000
	FDC_CMD_EXT_DENSITY		=	0x40,	//01000000
	FDC_CMD_EXT_MULTITRACK	=	0x80	//10000000
};

/*
**	Digital Output Register
*/

enum FLPYDSK_DOR_MASK {

	FLPYDSK_DOR_MASK_DRIVE0			=	0,	//00000000	= here for completeness sake
	FLPYDSK_DOR_MASK_DRIVE1			=	1,	//00000001
	FLPYDSK_DOR_MASK_DRIVE2			=	2,	//00000010
	FLPYDSK_DOR_MASK_DRIVE3			=	3,	//00000011
	FLPYDSK_DOR_MASK_RESET			=	4,	//00000100
	FLPYDSK_DOR_MASK_DMA			=	8,	//00001000
	FLPYDSK_DOR_MASK_DRIVE0_MOTOR	=	16,	//00010000
	FLPYDSK_DOR_MASK_DRIVE1_MOTOR	=	32,	//00100000
	FLPYDSK_DOR_MASK_DRIVE2_MOTOR	=	64,	//01000000
	FLPYDSK_DOR_MASK_DRIVE3_MOTOR	=	128	//10000000
};

/**
*	Main Status Register
*/

enum FLPYDSK_MSR_MASK {

	FLPYDSK_MSR_MASK_DRIVE1_POS_MODE	=	1,	//00000001
	FLPYDSK_MSR_MASK_DRIVE2_POS_MODE	=	2,	//00000010
	FLPYDSK_MSR_MASK_DRIVE3_POS_MODE	=	4,	//00000100
	FLPYDSK_MSR_MASK_DRIVE4_POS_MODE	=	8,	//00001000
	FLPYDSK_MSR_MASK_BUSY				=	16,	//00010000
	FLPYDSK_MSR_MASK_DMA				=	32,	//00100000
	FLPYDSK_MSR_MASK_DATAIO				=	64, //01000000
	FLPYDSK_MSR_MASK_DATAREG			=	128	//10000000
};

/**
*	Controller Status Port 0
*/

enum FLPYDSK_ST0_MASK {

	FLPYDSK_ST0_MASK_DRIVE0		=	0,		//00000000	=	for completness sake
	FLPYDSK_ST0_MASK_DRIVE1		=	1,		//00000001
	FLPYDSK_ST0_MASK_DRIVE2		=	2,		//00000010
	FLPYDSK_ST0_MASK_DRIVE3		=	3,		//00000011
	FLPYDSK_ST0_MASK_HEADACTIVE	=	4,		//00000100
	FLPYDSK_ST0_MASK_NOTREADY	=	8,		//00001000
	FLPYDSK_ST0_MASK_UNITCHECK	=	16,		//00010000
	FLPYDSK_ST0_MASK_SEEKEND	=	32,		//00100000
	FLPYDSK_ST0_MASK_INTCODE	=	64		//11000000
};

/*
** LPYDSK_ST0_MASK_INTCODE types
*/

enum FLPYDSK_ST0_INTCODE_TYP {

	FLPYDSK_ST0_TYP_NORMAL		=	0,
	FLPYDSK_ST0_TYP_ABNORMAL_ERR=	1,
	FLPYDSK_ST0_TYP_INVALID_ERR	=	2,
	FLPYDSK_ST0_TYP_NOTREADY	=	3
};

/**
*	GAP 3 sizes
*/

enum FLPYDSK_GAP3_LENGTH {

	FLPYDSK_GAP3_LENGTH_STD = 42,
	FLPYDSK_GAP3_LENGTH_5_14= 32,
	FLPYDSK_GAP3_LENGTH_3_5= 27
};

/*
**	Formula: 2^sector_number * 128, where ^ denotes "to the power of"
*/

enum FLPYDSK_SECTOR_DTL {

	FLPYDSK_SECTOR_DTL_128	=	0,
	FLPYDSK_SECTOR_DTL_256	=	1,
	FLPYDSK_SECTOR_DTL_512	=	2,
	FLPYDSK_SECTOR_DTL_1024	=	4
};

const int flp_sectors_per_track = 18;
const int dma_buffer = 0x1000;
const int fdc_dma_channel = 2;

static uint8_t current_drive = 0;
volatile uint8_t flp_disk_irq = 0;

void flp_initialize_dma()
{
	outportb (0x0a,0x06);	//mask dma channel 2
	outportb (0xd8,0xff);	//reset master flip-flop
	outportb (0x04, 0);     //address=0x1000 
	outportb (0x04, 0x10);
	outportb (0xd8, 0xff);  //reset master flip-flop
	outportb (0x05, 0xff);  //count to 0x23ff (number of bytes in a 3.5" floppy disk track)
	outportb (0x05, 0x23);
	outportb (0x80, 0);     //external page register = 0
	outportb (0x0a, 0x02);  //unmask dma channel 2
}

void flp_read_dma()
{
	outportb (0x0a, 0x06); //mask dma channel 2
	outportb (0x0b, 0x56); //single transfer, address increment, autoinit, read, channel 2
	outportb (0x0a, 0x02); //unmask dma channel 2
}

void flp_write_dma()
{
	outportb (0x0a, 0x06); //mask dma channel 2
	outportb (0x0b, 0x5a); //single transfer, address increment, autoinit, write, channel 2
	outportb (0x0a, 0x02); //unmask dma channel 2
}

uint8_t flp_read_status()
{
	return inportb(FLPYDSK_MSR);
}

void flp_write_dor(uint8_t val)
{
	outportb(FLPYDSK_DOR, val);
}

void flp_send_cmd(uint8_t cmd)
{
	for (int i=0;i<500;i++)
		if (flp_read_status() & FLPYDSK_MSR_MASK_DATAREG)
			return outportb(FLPYDSK_FIFO, cmd);
}

uint8_t flp_read_data()
{
	for (int i=0;i<500;i++)
		if (flp_read_status() & FLPYDSK_MSR_MASK_DATAREG)
			return inportb(FLPYDSK_FIFO);
	return 0;
}

void flp_write_ccr(uint8_t val)
{
	outportb(FLPYDSK_CTRL, val);
}

void flp_wait_irq()
{
	while (!flp_disk_irq)
		flp_disk_irq = 0;
}

void flpdisk_irq()
{
	asm ("cli");
	flp_disk_irq = 1;
	asm ("sti");
}

void flp_check_int(size_t* st0, size_t* cyl)
{
	flp_send_cmd(FDC_CMD_CHECK_INT);

	*st0 = flp_read_status();
	*cyl = flp_read_status();
}

void flp_ctrl_motor(bool b)
{
	if (current_drive > 3)
		return;

	uint8_t motor = 0;
	
	switch (current_drive)
	{
		case 0:
			motor = FLPYDSK_DOR_MASK_DRIVE0_MOTOR;
			break;
		case 1:
			motor = FLPYDSK_DOR_MASK_DRIVE1_MOTOR;
			break;
		case 2:
			motor = FLPYDSK_DOR_MASK_DRIVE2_MOTOR;
			break;
		case 3:
			motor = FLPYDSK_DOR_MASK_DRIVE3_MOTOR;
			break;
	}

	if (b)
		flp_write_dor((uint8_t)(current_drive | motor | FLPYDSK_DOR_MASK_RESET | FLPYDSK_DOR_MASK_DMA));
	else
		flp_write_dor(FLPYDSK_DOR_MASK_RESET);

	delay_ms(20);
}

void flp_drive_data(uint8_t stepr, uint8_t loadt, uint8_t unloadt, bool dma)
{
	uint8_t data = 0;

	flp_send_cmd(FDC_CMD_SPECIFY);
	
	data = ((stepr & 0xf) << 4) | (unloadt & 0xf);
		flp_send_cmd(data);
	data = ((loadt << 1) | ((dma) ? 0 : 1));
		flp_send_cmd(data);
}

int flp_calibrate(uint8_t drive)
{
	size_t st0, cyl;
	
	if (drive >= 4)
		return -2;
	
	flp_ctrl_motor(true);
	
	for (int i=0;i<10;i++)
	{
		flp_send_cmd(FDC_CMD_CALIBRATE);
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
	flp_write_dor(FLPYDSK_DOR_MASK_RESET | FLPYDSK_DOR_MASK_DMA);
}

void flp_reset()
{
	size_t st0, cyl;
	
	flp_disable_ctrl();
	flp_enable_ctrl();
	flp_wait_irq();

	for (int i=0;i<4;i++)
		flp_check_int(&st0, &cyl);
	
	flp_write_ccr(0);

	flp_drive_data(3, 16, 240, true);
	flp_calibrate(current_drive);
}

void flp_read_sector_imp(uint8_t head, uint8_t track, uint8_t sector)
{
	size_t st0, cyl;
	
	flp_initialize_dma();
	flp_read_dma();
	flp_send_cmd(FDC_CMD_READ_SECT | FDC_CMD_EXT_MULTITRACK | FDC_CMD_EXT_SKIP | FDC_CMD_EXT_DENSITY);
	flp_send_cmd(head << 2 | current_drive);
	flp_send_cmd(track);
	flp_send_cmd(head);
	flp_send_cmd(sector);
	flp_send_cmd(FLPYDSK_SECTOR_DTL_512);
	flp_send_cmd(((sector + 1) >= flp_sectors_per_track) ? flp_sectors_per_track : sector + 1);
	flp_send_cmd(FLPYDSK_GAP3_LENGTH_3_5);
	flp_send_cmd(0xff);
	
	flp_wait_irq();
	
	for (int i=0;i<7;i++)
		flp_read_data();
	
	flp_check_int(&st0, &cyl);
}

int flp_seek(uint8_t cyl, uint8_t head)
{
	size_t st0, cyl1;
	
	if (current_drive >= 4)
		return -1;

	for (int i=0;i<10;i++)
	{
		flp_send_cmd(FDC_CMD_SEEK);
		flp_send_cmd((head) << 2 | current_drive);
		flp_send_cmd(cyl);
		
		flp_wait_irq();
		flp_check_int(&st0, &cyl1);
		
		if (cyl1 == cyl)
			return 0;
	}
	
	return -1;
}

void flp_lba_to_chs(int lba, int *head, int *track, int *sector)
{
	*head = (lba % (flp_sectors_per_track * 2)) / (flp_sectors_per_track);
   	*track = lba / (flp_sectors_per_track * 2);
   	*sector = lba % flp_sectors_per_track + 1;
}

void flp_install()
{
	irq_install_handler(6, flpdisk_irq);
	flp_initialize_dma();
	flp_reset();
	flp_drive_data(13, 1, 0xf, true);
}

void flp_uninstall()
{
	irq_uninstall_handler(6);
}

void flp_set_working_drive(uint8_t drive)
{
	if (drive >= 4)
		return;
	
	current_drive = drive;
}

uint8_t flp_get_working_drive()
{
	return current_drive;
}

uint8_t* flp_read_sector(int sector_lba)
{
	if (current_drive >= 4)
		return (uint8_t*)-1;
	
	int head = 0, track = 0, sector = 1;
	flp_lba_to_chs(sector_lba, &head, &track, &sector);

	flp_ctrl_motor(true);
	if (flp_seek((uint8_t)track, (uint8_t)head) != 0)
		return 0;
	
	flp_read_sector_imp((uint8_t)head, (uint8_t)track, (uint8_t)sector);
	flp_ctrl_motor(false);
	
	return (uint8_t*)dma_buffer;
}
