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

#ifndef _INITRD_H_
#define _INITRD_H_

#include "stddef.h"
#include "fs.h"

typedef struct
{
	uint32_t nfiles;
} initrd_header_t;

typedef struct
{
	uint8_t magic;
	int8_t name[64];
	uint32_t offset;
	uint32_t length;
} initrd_file_header_t;

fs_node_t *init_initrd(uint32_t location);

#endif
