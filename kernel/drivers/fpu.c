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

#include <fpu.h>

void set_fpu_cw(const uint16_t cw)
{
	asm volatile ("fldcw %0" :: "m"(cw));
}

void enable_fpu(void)
{
	asm volatile ("clts");
	size_t t;
	asm volatile ("mov %%cr4, %0" : "=r"(t));
	t |= 3 << 9;
	asm volatile ("mov %0, %%cr4" :: "r"(t));
}

void disable_fpu(void)
{
	size_t t;
	asm volatile ("mov %%cr0, %0" : "=r"(t));
	t |= 1 << 3;
	asm volatile ("mov %0, %%cr0" :: "r"(t));
}

void init_fpu(void)
{
	asm volatile ("fninit");
	set_fpu_cw(0x37F);
}

