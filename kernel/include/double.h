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

#ifndef _DOUBLE_H_
#define _DOUBLE_H_

#include "stddef.h"

#define SCREEN_W 800
#define SCREEN_H 1024

typedef struct strBITMAP
{
	size_t w;
	size_t h;
	uint8_t *data;
} bitmap;

typedef struct strRECT
{
	long x1;
	long y1;
	long x2;
	long y2;
} rect;

extern void init_dbl_buffer(void);
extern void update_screen(void);
extern void setpixel(bitmap *bmp, int x, int y, uint8_t color);
extern void drawrect(bitmap *bmp, uint16_t x, uint16_t y, 
			uint16_t x2, uint16_t y2, 
			uint8_t color);
void _main();


#endif

