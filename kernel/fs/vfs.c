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

#include <vfs.h>
#include <string.h>
#include <stdio.h>

#define DEVICE_MAX 26

p_file_sys_t filesys[DEVICE_MAX];

file_t vol_openfile(const char *name)
{
	if (name)
	{
		uint8_t device = 'a';
		char *filename = (char*)name;
		
		if (name[2] == ':')
		{
			device = name[1];
			filename += 3;
		}

		if (filesys[device - 'a'])
		{
			file_t file = filesys[device - 'a']->open(filename);
			file.device_id = device;
			return file;
		}
	}
	file_t file;
	file.flags = FS_INVALID;
	return file;
}

void vol_readfile(p_file_t file, uint8_t *buff, size_t length)
{
	if (file)
		if (filesys[file->device_id - 'a'])
			filesys[file->device_id - 'a']->read(file, buff, length);
}

void vol_closefile(p_file_t file)
{
	if (file)
		if (filesys[file->device_id - 'a'])
			filesys[file->device_id - 'a']->close(file);
}

void vol_register_fsys(p_file_sys_t fsys, size_t device_id)
{
	static int i = 0;
	if (i < DEVICE_MAX)
		if (fsys)
		{
			filesys[device_id] = fsys;
			i++;
		}
}

void vol_unregister_fsys(p_file_sys_t fsys)
{
	for (int i=0;i<DEVICE_MAX;i++)
		if (filesys[i] == fsys)
			filesys[i] = 0;	
}

void vol_unregister_fsys_id(size_t device_id)
{
	if (device_id < DEVICE_MAX)
		filesys[device_id] = 0;
}
