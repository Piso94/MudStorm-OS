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

void i(char* message)
{
	set_color(green);
	printk("%s", message);
	set_color(white);
}

void d(char* message)
{
	set_color(magenta);
	printk("%s", message);
	set_color(white);
}

void w(char* message)
{
	set_color(yellow + black);
	printk("%s", message);
	set_color(white);
}

void e(char* message)
{
	set_color(red);
	printk("%s", message);
	set_color(white);
}

void v(char* message)
{
	printk("%s", message);
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
