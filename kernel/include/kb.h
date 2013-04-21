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

#include <types.h>

#define ScrollLock (unsigned char)0x01
#define NumLock (unsigned char)0x02
#define CapsLock (unsigned char)0x04

extern int kb_special(unsigned char key);
extern void UpdateLeds(char led);
extern void FlushBuffer();
extern char getchar_int();
extern int getchar();
extern void visible(bool rix);
extern void gets(char *s);
extern void waitKey();

extern void keyboard_install();

#endif
