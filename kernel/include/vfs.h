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

#ifndef _VFS_H_
#define _VFS_H_

#include "stddef.h"

typedef struct file
{
	char	name[32];
	size_t	flags;
	size_t	length;
	size_t	id;
	size_t	eof;
	size_t	position;
	size_t	currentcluster;
	size_t	device_id;
} file_t, *p_file_t;

typedef struct _file_system
{
	char		name[8];
	file_t		(*directory)	(const char *dir);
	void		(*mount)	();
	void		(*read)		(p_file_t file, uint8_t *buff, size_t length);
	void		(*close)	(p_file_t);
	file_t		(*open)		(const char *filename);
} file_sys_t, *p_file_sys_t;

#define FS_FILE	   0
#define FS_DIR	   1
#define FS_INVALID 2

extern file_t vol_openfile(const char *name);
extern void vol_readfile(p_file_t file, uint8_t *buff, size_t length);
extern void vol_closefile(p_file_t file);
extern void vol_register_fsys(p_file_sys_t fsys, size_t device_id);
extern void vol_unregister_fsys(p_file_sys_t fsys);
extern void vol_unregister_fsys_id(size_t device_id);

#endif

