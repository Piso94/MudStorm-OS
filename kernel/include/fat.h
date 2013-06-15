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

#ifndef _FAT_H_
#define _FAT_H_

#include "stddef.h"
#include "vfs.h"

/**
*	Directory entry
*/
typedef struct _DIRECTORY
{
	uint8_t   Filename[8];
	uint8_t   Ext[3];
	uint8_t   Attrib;
	uint8_t   Reserved;
	uint8_t   TimeCreatedMs;
	uint16_t  TimeCreated;
	uint16_t  DateCreated;
	uint16_t  DateLastAccessed;
	uint16_t  FirstClusterHiBytes;
	uint16_t  LastModTime;
	uint16_t  LastModDate;
	uint16_t  FirstCluster;
	unsigned  FileSize;

} DIRECTORY, *PDIRECTORY;

/**
*	Filesystem mount info
*/
typedef struct _MOUNT_INFO
{
	unsigned numSectors;
	unsigned fatOffset;
	unsigned numRootEntries;
	unsigned rootOffset;
	unsigned rootSize;
	unsigned fatSize;
	unsigned fatEntrySize;

} MOUNT_INFO, *PMOUNT_INFO;

extern FILE fsysFatDirectory (const char* DirectoryName);
extern void fsysFatRead(PFILE file, unsigned char* Buffer, unsigned int Length);
extern FILE fsysFatOpen (const char* FileName);
extern void fsysFatInitialize ();

extern void fat_install();
extern void fat_uninstall();

#endif
