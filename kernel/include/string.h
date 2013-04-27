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

#ifndef _STRING_H_
#define _STRING_H_

#include "stddef.h"

typedef char* string;

extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
extern void *memcpy(void *dst, const void *src, size_t len);
extern void memset(void *dst, int val, size_t len);
extern void memmov (void *dst, const void *src, const unsigned int bytes);
extern void strcat (char *dst, const char *src);
extern int strlen(const char *str);
extern int strcmp(const char *str1, const char *str2);
extern int strncmp (const char *dst, const char *src, const unsigned int bytes);
extern char *strncpy (char *dst, register const char *src, register size_t n);
extern char *strcpy(char *dst, const char *src);
extern char *strtok(register char *s, register const char *delim);
extern int stoi(char *str);
extern int isdigit(int c);
extern char* toupper (char *src);
extern char* tolower (char *src);
extern bool isspace(char c);

#endif
