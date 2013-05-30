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

#ifndef _ISRS_H_
#define _ISRS_H_

#include <stddef.h>

struct regs
{
    size_t gs, fs, es, ds;
    size_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    size_t int_no, err_code;
    size_t eip, cs, eflags, useresp, ss;    
};

extern void isrs_install();

#endif
