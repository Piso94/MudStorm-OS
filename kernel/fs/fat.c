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

#include <fat.h>
#include <bpb.h>
#include <string.h>
#include <flp.h>
#include <io.h>
#include <stdio.h>
#include <timer.h>
#include <kheap.h>

#define SECTOR_SIZE 512

file_sys_t sys_fat;
mount_t mount_info;

uint8_t FAT [SECTOR_SIZE * 2];

void todos_filename(const char *name, char *fname, size_t length)
{
	size_t i = 0;

	if (length > 11)
		return;

	if (!fname || !name)
		return;
	
	memset(fname, ' ', length);

	for (i=0;i<(strlen((char*)name) - 1) && i < length;i++)
	{
		if (name[i] == '.' || i == 8)
			break;
		fname[i] = *toupper((char*)&name[i]);
	}

	if (name[i] == '.')
		for (int k=0;k<3;k++)
		{
			++i;
			if (name[i])
				fname[k + 8] = name[i];
		}

	for (i=0;i<3;i++)
		fname[i + 8] = *toupper((char*)&fname[i + 8]);
}

file_t directory(const char *dir_name)
{
	file_t	file;
	uint8_t *buff;
	p_directory_t dir;
	
	char dos_filename[11];
	todos_filename(dir_name, dos_filename, 11);
	dos_filename[11] = 0;

	for (int sector=0;sector<14;sector++)
	{
		buff = flp_read_sector(mount_info.rootoffset + sector);
		dir = (p_directory_t)buff;
		
		for (int i=0;i<16;i++)
		{
			char name[11];
			memcpy(name, dir->filename, 11);
			name[11] = 0;
			
			if (!strcmp(dos_filename, name))
			{
				strcpy(file.name, dir_name);
				file.id = 0;
				file.currentcluster = dir->firstcluster;
				file.length = dir->filesize;
				file.eof = 0;
				file.length = dir->filesize;

				if (dir->attr == 0x10)
					file.flags = FS_DIR;
				else
					file.flags = FS_FILE;

				return file;
			}
			dir++;
		}
	}

	file.flags = FS_INVALID;
	return file;
}

void read(p_file_t file, uint8_t *buff, size_t length)
{
	if (file)
	{
		size_t ph_sector = ((32 + (file->currentcluster)) - 1);
		uint8_t *sector = (uint8_t*)flp_read_sector(ph_sector);
		
		memcpy(buff, sector, 512);

		size_t fat_offset = (file->currentcluster + (file->currentcluster / 2));
		size_t fat_sector = (1 + (fat_offset / SECTOR_SIZE));
		size_t entry_offset = (fat_offset % SECTOR_SIZE);
		
		sector = (uint8_t*)flp_read_sector(fat_sector);
		memcpy(FAT, sector, 512);

		sector = (uint8_t*)flp_read_sector(fat_sector + 1);
		memcpy((FAT + SECTOR_SIZE), sector, 512);

		uint16_t next_cluster = *(uint16_t*)&FAT[entry_offset];
		
		if (file->currentcluster & 0x0001)
			next_cluster >>= 4;
		else
			next_cluster &= 0x0FFF;

		if (next_cluster >= 0xff8)
		{
			file->eof = -1;
			return;
		}

		if (next_cluster == 0)
		{
			file->eof = -1;
			return;
		}
		delay_ms(5);

		file->currentcluster = next_cluster;
	}
}

void fat_close(p_file_t file)
{
	if (file)
		file->flags = FS_INVALID;
}

file_t opendir(file_t file, const char *filename)
{
	file_t file1;
	
	char dos_filename[11];
	todos_filename(filename, dos_filename, 11);
	dos_filename[11] = 0;

	while (!file.eof)
	{
		uint8_t buff[512];
		read(&file1, buff, 512);
		
		p_directory_t pdir = (p_directory_t)buff;

		for (size_t i=0;i<16;i++)
		{
			char name[11];
			memcpy(name, pdir->filename, 11);
			name[11] = 0;

			if (!strcmp(name, dos_filename))
			{
				strcpy(file1.name, filename);
				file1.id = 0;
				file1.currentcluster = pdir->firstcluster;
				file1.length = pdir->filesize;
				file1.eof = 0;
				file1.length = pdir->filesize;
				
				if (pdir->attr == 0x10)
					file1.flags = FS_DIR;
				else
					file1.flags = FS_FILE;
				return file1;
			}
			pdir++;
		}
	}
	file1.flags = FS_INVALID;
	return file1;
}

file_t open(const char* filename)
{
	file_t curdir;
	char *p = 0;
	bool rootdir = true;
	char *path = (char*)filename;
	
	p = strchr(path, '/');
	if (!p)
	{
		curdir = directory(path);
		if (curdir.flags == FS_FILE)
			return curdir;

		file_t ret;
		ret.flags = FS_INVALID;
		return ret;
	}
	p++;
	
	while (p)
	{
		char pathname[16];
		int i = 0;

		for (i=0;i<16;i++)
		{
			if (p[i] == '/' || p[i] == 0)
				break;

			pathname[i] = p[i];
		}
		pathname[i] = 0;

		if (rootdir)
		{
			curdir = directory(pathname);
			rootdir = false;
		}
		else
		{
			curdir = opendir(curdir, pathname);
		}

		if (curdir.flags == FS_INVALID)
			break;

		if (curdir.flags == FS_FILE)
			return curdir;

		p = strchr(p + 1, '/');
		if (p)
			p++;
	}
	file_t ret;
	ret.flags = FS_INVALID;
	return ret;
}

void fat_mount()
{
	p_bs_t bootsector;
	uint8_t *sector;

	sector = flp_read_sector(0);

	for (int i=0;i<sizeof(p_bs_t);i++)
		((uint8_t*)&bootsector)[i] = sector[i];

	mount_info.numsectors = bootsector->_bpb.numsectors;
	mount_info.fatoffset  = 1;
	mount_info.fatsize    = bootsector->_bpb.sectorsperfat;
	mount_info.fatentrysize = 8;
	mount_info.numrootentries = bootsector->_bpb.numdirentries;
	mount_info.rootoffset = (bootsector->_bpb.numberoffats * bootsector->_bpb.sectorsperfat) + 1;
	mount_info.rootsize   = (bootsector->_bpb.numdirentries * 32) / bootsector->_bpb.bytespersector + 0.5f;
}

void fat_install()
{
	strcpy(sys_fat.name, "FAT12");
	sys_fat.directory = directory;
	sys_fat.mount     = fat_mount;
	sys_fat.open      = open;
	sys_fat.read      = read;
	sys_fat.close     = fat_close;

	vol_register_fsys(&sys_fat, 0);
	fat_mount();
}
