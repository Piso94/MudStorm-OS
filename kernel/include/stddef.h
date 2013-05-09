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

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define GREY 7
#define DARK_GREY 8
#define BRIGHT_BLUE 9
#define BRIGHT_GREEN 10
#define BRIGHT_CYAN 11
#define BRIGHT_RED 12
#define BRIGHT_MAGENTA 13
#define YELLOW 14
#define WHITE 15

 // Type

#define NULL ((void*)0)

#define EOF (-1)

#define true 1
#define false 0
#define TRUE 1
#define FALSE 0

#define UINT32_MAX  (0xffffffff)

typedef int bool;

typedef unsigned int size_t;

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned long dword;
typedef unsigned long long qword;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed long int32_t;
typedef signed long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long uint64_t;

#endif
