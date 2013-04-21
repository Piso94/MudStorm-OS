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

#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <types.h>
#include <intr/isrs.h>

extern byte mouse_x;
extern byte mouse_y;

void mouse_handler(struct regs *a_r);
void mouse_wait(byte a_type);
void mouse_write(byte a_write);
byte mouse_read();
void mouse_install();

#endif
