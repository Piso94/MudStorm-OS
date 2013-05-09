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

byte mouse_cycle = 0;
byte mouse_byte[3];
byte mouse_x = 0;
byte mouse_y = 0;

void mouse_handler(struct regs *a_r)
{
	switch (mouse_cycle)
	{
		case 0:
			mouse_byte[0] = inportb(0x60);
			mouse_cycle++;
			break;
		case 1:
			mouse_byte[1] = inportb(0x60);
			mouse_cycle++;
			break;
		case 2:
			mouse_byte[2] = inportb(0x60);
			mouse_x = mouse_byte[1];
			mouse_y = mouse_byte[2];
			mouse_cycle = 0;
			break;
	}
}

void mouse_wait(byte a_type)
{
	dword time_out = 100000;
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

void mouse_write(byte a_write)
{
	mouse_wait(1);
	outportb(0x64, 0xD4);
	mouse_wait(1);
	outportb(0x60, a_write);
}

byte mouse_read()
{
	mouse_wait(0);
	return inportb(0x60);
}

void mouse_install()
{
	byte _status;

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
