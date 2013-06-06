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

#include <cpuid.h>
#include <kheap.h>

void cpuid(int in, int a, int b, int c, int d)
{
	asm ("cpuid": "=a"(a), "=b"(b), "=c"(c), "=d"(d) : "a"(in));
}

char *cpu_vendor(void)
{
	uint32_t ebx = (uint32_t)kmalloc(sizeof(uint32_t));
	uint32_t unused = (uint32_t)kmalloc(sizeof(uint32_t));

	cpuid(0, unused, ebx, unused, unused);

	switch (ebx)
	{
		case 0x756e6547:
			return "Intel";
		case 0x68747541:
			return "AMD";
	}

	return "Sconosciuto";
}
