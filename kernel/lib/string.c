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
#include <string.h>

uint16_t *memsetw(uint16_t *dest, uint16_t val, size_t count)
{
    	uint16_t *temp = (uint16_t*)dest;
    	for( ; count != 0; count--) *temp++ = val;
    	return dest;
}

void *memcpy(void *dst, const void *src, size_t len)
    /// Copy block of memory from src to dst, len bytes long
{
    char *sp = (char*) src;
    char *dp = (char*) dst;
    for( ; len != 0; len--) *dp++ = *sp++;
	return dst;
}

void *memset(void *dst, int val, size_t len)
{
	uint8_t *tmp = (uint8_t*)dst;
	for( ;len != 0;len--, tmp[len] = val);
	return dst;
}

void memmov (void *dst, const void *src, const size_t bytes)
{
    for (size_t i=0; i<bytes; i++) 
    {
        ((char *)dst)[i] = ((char *)src)[i];
    }
}

void strcat (char* dst, const char* src)
{
	int dst_len = strlen(dst), src_len = strlen(src);

	for (int i=0; i<(dst_len+src_len); i++)
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + src_len] = '\0';
}

int strlen(const char *str) 
{
	int i = 0;
	while(str[i] != '\0') 
	{
		++i;
	}
	return i;
}

int strcmp(const char *str1, const char *str2)
{
	int res = 0;
	while (!(res = *(uint8_t*)str1 - *(uint8_t*)str2) && *str2)
		++str1, ++str2;
	if (res < 0)
		res = -1;
	if (res > 0)
		res = 1;
	return res;
}

int strncmp (const char *dst, const char *src, const size_t bytes) 
{
    int result = 0;

    for (size_t i=0; i<bytes; i++) 
    {
        if (dst[i] < src[i]) 
	{
            result = -1;
            break;
        } 
	else if (dst[i] > src[i]) 
	{
            result = 1;
            break;
        }
    }

    return result;
}

char *strncpy (char *dst, register const char *src, register size_t n)
{
  if (n != 0) {
	char *d = dst;
	const char *s = src;
	do {
		if ((*d++ = *s++) == 0) {
			while (--n != 0)
				*d++ = '\0';
				break;
			}
		} while (--n != 0);
	}
	return dst;
 }

char *strcpy(char *dst, const char *src)
{
	char *_s1 = dst;
	while ((*dst++ = *src++));
	return _s1;
}

char *strtok(register char *s, register const char *delim)
{
	register char *spanp;
	register int c, sc;
	char *tok;
	static char *last;


	if (s == NULL && (s = last) == NULL)
		return (NULL);

	/*
	 * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
	 */
cont:
	c = *s++;
	for (spanp = (char *)delim; (sc = *spanp++) != 0;) 
	{
		if (c == sc)
			goto cont;
	}

	if (c == 0) 
	{
		/* no non-delimiter characters */
		last = NULL;
		return (NULL);
	}
	tok = s - 1;

	/*
	 * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	 * Note that delim must have one NUL; we stop if we see that, too.
	 */
	for ( ; ; ) 
	{
		c = *s++;
		spanp = (char *)delim;
		do 
		{
			if ((sc = *spanp++) == c) 
			{
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				last = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}

char *strchr(char *str, int ch)
{
	do
	{
		if (*str == ch)
			return (char*)str;
	} while (*str++);
	return 0;
}

int stoi(char *str)
{
  int result;
  int puiss;

  result = 0;
  puiss = 1;
  while (('-' == (*str)) || ((*str) == '+'))
    {
      if (*str == '-')
        puiss = puiss * -1;
      str++;
    }
  while ((*str >= '0') && (*str <= '9'))
    {
      result = (result * 10) + ((*str) - '0');
      str++;
    }
  return (result * puiss);
}

int isdigit(int c) 
{
  if (c >= 48 && c <= 57)
    return 1;
  return 0;
}

char* toupper (char *src)
{
	int len = strlen(src);
    
    for (int i=0; i < len; i++)
        if (*(src + i) >= 'a' && *(src + i) <= 'z')
            *(src + i) -= ('a' - 'A');
    
    return src;
}

char* tolower (char *src)
{
	int len = strlen(src);
    
    for (int i=0; i < len; i++)
        if (*(src + i) >= 'A' && *(src + i) <= 'Z')
            *(src + i) -= ('A' - 'a');
    
    return src;
}

bool isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\12');
}
