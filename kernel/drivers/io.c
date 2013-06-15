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

#include <stddef.h>
#include <io.h>

uint8_t inportb(uint16_t _port)
{
	uint8_t rv;
	asm volatile ("inb %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}

uint16_t inportw(uint16_t _port)
{
	uint16_t result;
	asm volatile  ("in %%dx, %%ax" : "=a" (result) : "d" (_port));
	return result;
}


void inportsm(uint16_t port, uint8_t *data, uint32_t size)
{
	asm volatile ("rep insw" : "+D" (data), "+c" (size) : "d" (port) : "memory");
}

uint16_t inports(uint16_t _port)
{
	uint16_t rv;
	asm volatile ("inw %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}


void outportb(uint16_t _port, uint8_t _data)
{
	asm volatile ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void outportw(uint16_t _port, uint16_t _data)
{
	asm volatile ("outw %1, %0" : : "dN" (_port), "a" (_data));
}

void outports(uint16_t _port, uint16_t _data)
{
	asm volatile ("outw %1, %0" : : "dN" (_port), "a" (_data));
}

void outportsm(uint16_t port, uint8_t *data, uint32_t size)
{
	asm volatile ("rep outsw" : "+S" (data), "+c" (size) : "d" (port));
}
