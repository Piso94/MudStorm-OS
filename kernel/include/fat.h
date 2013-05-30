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
#include "fsys.h"

typedef struct _dir
{
	uint8_t		filename[8];
	uint8_t		ext[3];
	uint8_t		attr;
	uint8_t		reserved;
	uint8_t		timecreated_ms;
	uint16_t	timecreated;
	uint16_t	datecreated;
	uint16_t	datelast_accessed;
	uint16_t	first_cluster_hibytes;
	uint16_t	last_mode_time;
	uint16_t	last_mode_data;
	uint16_t	firstcluster;
	uint16_t	filesize;
} directory_t, *p_directory_t;

typedef struct _mount
{
	size_t	numsectors;
	size_t	fatoffset;
	size_t	numrootentries;
	size_t	rootoffset;
	size_t	rootsize;
	size_t	fatsize;
	size_t	fatentrysize;
} mount_t, *p_mount_t;

extern file_t directory(const char* dir_name);
extern void read(p_file_t file, uint8_t *buff, size_t length);
extern file_t open(const char *name);
extern void fat_install();
extern void fat_mount();

#endif

