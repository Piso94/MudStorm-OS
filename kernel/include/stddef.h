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

#ifndef _STDDEF_H_
#define _STDDEF_H_

// Colour
enum
{
	black = 0,
	blue = 1,
	green = 2,
	cyan = 3,
	red = 4,
	magenta = 5,
	brown = 6,
	grey = 7,
	dark_grey = 8,
	bright_blue = 9,
	bright_green = 10,
	bright_cyan = 11,
	bright_red = 12,
	bright_magenta = 13,
	yellow = 14,
	white = 15
};

// Type
#define NULL ((void*)0)
#define EOF -1

typedef unsigned int bool;
enum
{
	true = 1,
	false = 0
};

typedef unsigned int size_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed long int32_t;
typedef signed long long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;

typedef unsigned short int word;

#endif
