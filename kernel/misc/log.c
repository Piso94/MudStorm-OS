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

#include <log.h>
#include <video.h>
#include <stdio.h>
#include <stdarg.h>

void i(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	set_color(green);
	print(0, format, args);
	set_color(white);
}

void d(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	set_color(magenta);
	print(0, format, args);
	set_color(white);
}

void w(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	set_color(yellow + black);
	print(0, format, args);
	set_color(white);
}

void e(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	set_color(red);
	print(0, format, args);
	set_color(white);
}

void v(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	print(0, format, args);
}

/*
 * Grazie a ZioCrocifisso (PieroTofy.it)
 * per avermi illuminato sull uso delle struct
 * con le funzioni!
 */
struct LOG Log =
{
	&i,
	&d,
	&w,
	&e,
	&v
};
