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

typedef struct _BIOS_PARAMATER_BLOCK {

	uint8_t			OEMName[8];
	uint16_t		BytesPerSector;
	uint8_t			SectorsPerCluster;
	uint16_t		ReservedSectors;
	uint8_t			NumberOfFats;
	uint16_t		NumDirEntries;
	uint16_t		NumSectors;
	uint8_t			Media;
	uint16_t		SectorsPerFat;
	uint16_t		SectorsPerTrack;
	uint16_t		HeadsPerCyl;
	unsigned		HiddenSectors;
	unsigned		LongSectors;

}BIOSPARAMATERBLOCK, *PBIOSPARAMATERBLOCK;

/**
*	bios paramater block extended attributes
*/
typedef struct _BIOS_PARAMATER_BLOCK_EXT
{
	unsigned			SectorsPerFat32;
	uint16_t			Flags;
	uint16_t			Version;
	unsigned			RootCluster;
	uint16_t			InfoCluster;
	uint16_t			BackupBoot;
	uint16_t			Reserved[6];

} BIOSPARAMATERBLOCKEXT, *PBIOSPARAMATERBLOCKEXT;

/**
*	boot sector
*/
typedef struct _BOOT_SECTOR
{
	uint8_t			Ignore[3];
	BIOSPARAMATERBLOCK	Bpb;
	BIOSPARAMATERBLOCKEXT	BpbExt;
	uint8_t			Filler[448];

} BOOTSECTOR, *PBOOTSECTOR;

#endif
