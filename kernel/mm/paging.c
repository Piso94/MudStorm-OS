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

#include <paging.h>

extern size_t end;

void enable_paging()
{
	size_t page_aligned_end = (end & 0xFFFFF000) + 0x1000;
	size_t *page_directory = (size_t*)page_aligned_end;
	uint32_t addr = 0;
	size_t *first_page_table = page_directory + 0x1000;

	for (size_t i=0; i<1024; i++)
	{
		page_directory[i] = 0 | 2;
	}

	for (size_t i=0; i<1024; i++)
	{
		page_directory[i] = addr | 3;
		addr = addr + 4096;
	}
	
	page_directory[0] = (size_t)first_page_table;
	page_directory[0] |= 3;
	
	asm volatile("mov %0, %%cr3":: "b"(page_directory));
	
	size_t cr0;
	asm volatile("mov %%cr0, %0": "=b"(cr0));
	cr0 |= 0x8000000;
	asm volatile("mov %0, %%cr0":: "b"(cr0));
}
