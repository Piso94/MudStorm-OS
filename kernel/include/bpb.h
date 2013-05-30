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

#ifndef _BPB_H_
#define _BPB_H_

#include "stddef.h"

typedef struct _bios_paramater
{
	uint8_t		oemname[8];
	uint16_t	bytespersector;
	uint8_t		sectorspercluster;
	uint16_t	reservedsectors;
	uint8_t		numberoffats;
	uint16_t	numdirentries;
	uint16_t	numsectors;
	uint8_t		media;
	uint16_t	sectorsperfat;
	uint16_t	sectorspertrack;
	uint16_t	headspercyl;
	size_t	hiddensectors;
	size_t	longsectors;
} bpb_t, *p_bpb_t;

typedef struct _bios_paramater_ext
{
	size_t	sectorsperfat32;
	uint16_t	flags;
	uint16_t	version;
	size_t	rootcluster;
	uint16_t	infocluster;
	uint16_t	backupboot;
	uint16_t	reserved[6];
} bpb_ext_t, *p_bpb_ext_t;

typedef struct _boot_sector
{
	uint8_t		ignore[3];
	bpb_t		_bpb;
	bpb_ext_t	bpbext;
	uint8_t		filler[448];
} bs_t, *p_bs_t;

#endif

