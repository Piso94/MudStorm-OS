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

#include <stddef.h>
#include <fs.h>
#include <kmalloc.h>
#include <string.h>
#include <stdio.h>
#include <video.h>

fs_node_t *fs_root = 0;

uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
	if (node->read != 0)
		return node->read(node, offset, size, buffer);
	else
		return 0;
}

uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
	if (node->write != 0)
		return node->write(node, offset, size, buffer);
	else
		return 0;
}

void open_fs(fs_node_t *node, uint8_t read, uint8_t write)
{
	if (node->open != 0)
		return node->open(node);
}

void close_fs(fs_node_t *node)
{
	if (node->close != 0)
		return node->close(node);
}

struct dirent *readdir_fs(fs_node_t *node, uint32_t index)
{
	if (((node->flags & 0x7) == FS_DIRECTORY ) && (node->readdir != 0))
		return node->readdir(node, index);
	else
		return 0;	
}

fs_node_t *finddir_fs(fs_node_t *node, char *name)
{
	if (((node->flags & 0x7) == FS_DIRECTORY) && (node->finddir != 0))
		return node->finddir(node, name);
	else
		return 0;
}

size_t relocate_module(char *name, int size)
{
	fs_node_t *module_node = finddir_fs(fs_root, name);
	uint8_t module_buffer[size];
	size_t module_size = read_fs(module_node, 0, size, module_buffer);
	size_t module_ptr = (size_t)kmalloc(module_size);
	memcpy((void*)module_ptr, (void*)module_buffer, module_size);
	return module_ptr;
}

void file(char* name)
{
	set_color(WHITE);
	printk("%s", name);
}

void dir(char* name)
{
	set_color(BLUE);
	printk("%s", name);
	set_color(WHITE);
}
