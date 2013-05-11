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

#include <mouse.h>
#include <io.h>
#include <intr/irq.h>
#include <stdio.h>
#include <video.h>

uint8_t mouse_cycle = 0;
uint8_t mouse_byte[3];

void mouse_handler(struct regs *a_r)
{
	switch (mouse_cycle)
	{
		case 0:
			mouse_byte[0] = inportb(0x60);
			if (mouse_byte[0] & 0x1)
			{
				//click_mouse(1);
			}
			/*
			 * Centrale, non lo gestisco
			 * if (mouse_byte[0] & 0x4)
			 * {
			 *	click_mouse(2);
			 * }
			 **/
			if (mouse_byte[0] & 0x2)
			{
				//click_mouse(3);
			}
			mouse_cycle++;
			break;
		case 1:
			mouse_byte[1] = inportb(0x60);
			mouse_cycle++;
			break;
		case 2:
			mouse_byte[2] = inportb(0x60);
			if ((mouse_byte[1] > 0) && (mouse_byte[2] > 0))
			{

			}
			mouse_cycle = 0;
			break;
	}
}

void mouse_wait(uint8_t a_type)
{
	uint32_t time_out = 100000;
	if (a_type == 0)
	{
		while (time_out--)
		{
			if ((inportb(0x64) & 1) == 1)
				return;
		}
		return;
	}
	else
	{
		while (time_out--)
		{
			if ((inportb(0x64) & 2) == 0)
				return;
		}
		return;
	}
}

void mouse_write(uint8_t a_write)
{
	mouse_wait(1);
	outportb(0x64, 0xD4);
	mouse_wait(1);
	outportb(0x60, a_write);
}

uint8_t mouse_read()
{
	mouse_wait(0);
	return inportb(0x60);
}

void mouse_install()
{
	uint8_t _status;

	mouse_wait(1);
	outportb(0x64, 0xA8);
	mouse_wait(1);
	outportb(0x64, 0x20);
	mouse_wait(0);
	_status = (inportb(0x60) | 2);
	mouse_wait(1);
	outportb(0x64, 0x60);
	mouse_wait(1);
	outportb(0x60, _status);

	mouse_write(0xF6);
	mouse_read();
	
	mouse_write(0xF4);
	mouse_read();

	irq_install_handler(12, mouse_handler);
}
