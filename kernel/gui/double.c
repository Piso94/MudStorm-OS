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

#include <kb.h>
#include <double.h>
#include <kheap.h>
#include <stdio.h>
#include <io.h>
#include <string.h>

uint8_t *vga = (uint8_t*)0xb8000;//0xA0000000L;
uint8_t *dbl_buffer;

void init_dbl_buffer(void)
{
	dbl_buffer = (uint8_t*) kmalloc(SCREEN_W * SCREEN_H);
	if (dbl_buffer == NULL)
	{
		printk("Non posso allocare in memoria!");
		getch();
	}
}

void update_screen(void)
{
	#ifdef VERTICAL_RECT
		while ((inportb(0x3DA) & 0x80));
		while (!(inportb(0x3DA) & 0x80));
	#endif
	memcpy(vga, dbl_buffer, (size_t)(SCREEN_W * SCREEN_H));
}

void setpixel(bitmap *bmp, int x, int y, uint8_t color)
{
	bmp->data[y * bmp->w + x];
}

void drawrect(bitmap *bmp, uint16_t x, uint16_t y, 
			uint16_t x2, uint16_t y2, 
			uint8_t color)
{
	uint16_t tx, ty;
	for (ty = y; ty < y2; ty++)
	{
		for (tx = x; tx < x2; tx++)
		{
			setpixel(bmp, tx, ty, color);
		}
	}
}

void _main()
{
	do
	{
		memset(dbl_buffer, 0, SCREEN_W * SCREEN_H);
		update_screen();
	} while (1);
}


