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

#ifndef _FS_H_
#define _FS_H_

#include "stddef.h"

#define FS_FILE		0x01
#define FS_DIRECTORY	0x02
#define FS_CHARDEVICE	0x03
#define FS_BLOCKDEVICE	0x04
#define FS_PIPE		0x05
#define FS_SYMLINK	0x06
#define FS_MOUNTPOINT	0x08

struct fs_node;

typedef uint32_t (*read_type_t)(struct fs_node*, uint32_t, uint32_t, uint8_t*);
typedef uint32_t (*write_type_t)(struct fs_node*, uint32_t, uint32_t,uint8_t*);
typedef void (*open_type_t)(struct fs_node*);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent * (*readdir_type_t)(struct fs_node*, uint32_t);
typedef struct fs_node * (*finddir_type_t)(struct fs_node*, char *name);

typedef struct fs_node
{
	char name[128];
	uint32_t mask;
	uint32_t uid;
	uint32_t gid;
	uint32_t flags;
	uint32_t length;
	uint32_t inode;
	uint32_t impl;
	read_type_t read;
	write_type_t write;
	open_type_t open;
	close_type_t close;
	readdir_type_t readdir;
	finddir_type_t finddir;
	struct fs_node *ptr;
} fs_node_t;

struct dirent
{
	char name[128];
	uint32_t ino;
};

extern fs_node_t *fs_root;

uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);

void open_fs(fs_node_t *node, uint8_t read, uint8_t write);
void close_fs(fs_node_t *node);

struct dirent *readdir_fs(fs_node_t *node, uint32_t index);
fs_node_t *finddir_fs(fs_node_t *node, char *name);

size_t relocate_module(char *name, int size);

void file(char* name);
void dir(char* name);

#endif
