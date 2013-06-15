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

/**
*	File
*/
typedef struct _FILE {

	char        name[32];
	unsigned    flags;
	unsigned    fileLength;
	unsigned    id;
	unsigned    eof;
	unsigned    position;
	unsigned    currentCluster;
	unsigned    deviceID;

}FILE, *PFILE;

/**
*	Filesystem interface
*/
typedef struct _FILE_SYSTEM {

	char Name [8];
	FILE               (*Directory)  (const char* DirectoryName);
	void	           (*Mount)      ();
	void               (*Read)       (PFILE file, unsigned char* Buffer, unsigned int Length);
	void	           (*Close)      (PFILE);
	FILE               (*Open)       (const char* FileName);

}FILESYSTEM, *PFILESYSTEM;

/**
*	File flags
*/
#define FS_FILE       0
#define FS_DIRECTORY  1
#define FS_INVALID    2

extern FILE volOpenFile (const char* fname);
extern void volReadFile (PFILE file, unsigned char* Buffer, unsigned int Length);
extern void volCloseFile (PFILE file);
extern void volRegisterFileSystem (PFILESYSTEM, unsigned int deviceID);
extern void volUnregisterFileSystem (PFILESYSTEM);
extern void volUnregisterFileSystemByID (unsigned int deviceID);

#endif
