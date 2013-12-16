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

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

extern void help();
extern void clear();
extern void eco();
extern void cowsay();
extern void date();
extern void bee();
extern void debug();
extern void logos();
extern void _reboot();
extern void _halt();
extern void ls();
extern void cd();

extern char* argv; // Inzializzo una variabile "char*" con nome "argv" con valore "NULL"

#endif
