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

#ifndef _STDIO_H_
#define _STDIO_H_

#include "stdarg.h"
#include "stddef.h"

void printchar(char **str, int c);
int prints(char **out, const char *string, int width, int pad);
int printi(char **out, int i, int b, int sg, int width, int pad, int letbase);
int print(char **out, const char *format, va_list args);
extern int printk(const char *format, ...);
extern int scank(const char *format, ...);

#endif
