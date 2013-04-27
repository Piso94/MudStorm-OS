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

#ifndef _KMALLOC_H_
#define _KMALLOC_H_

#include "stddef.h"

extern uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys);
extern uint32_t kmalloc_a(uint32_t sz);
extern uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);
extern uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys);
extern uint32_t kmalloc(uint32_t sz);

#endif
