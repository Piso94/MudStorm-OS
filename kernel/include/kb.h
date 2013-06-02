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

#ifndef _KB_H_
#define _KB_H_

#include "stddef.h"

enum
{
ScrollLock = (uint8_t)0x01,
NumLock = (uint8_t)0x02,
CapsLock = (uint8_t)0x04
};

extern int kb_special(uint8_t key);
extern void update_leds(char led);
extern void flush();
extern char getchar_int();
extern int getchar();
extern void kbhit();
extern void gets(char *s);
extern void waitKey();

extern void keyboard_install();
extern void keyboard_uninstall();

#endif
