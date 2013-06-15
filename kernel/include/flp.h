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

#ifndef _FLP_H_
#define _FLP_H_

#include "stddef.h"

extern void flp_install();
extern void flp_uninstall();

//! set current working drive
extern void flpydsk_set_working_drive (uint8_t drive);

//! get current working drive
extern uint8_t flpydsk_get_working_drive ();

//! read a sector
extern uint8_t* flpydsk_read_sector (int sectorLBA);

//! converts an LBA address to CHS
void flpydsk_lba_to_chs (int lba,int *head,int *track,int *sector);

#endif
