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

#include <malloc.h>

char* g_heapstart, *g_heapend;
#define _malloc_heap_start g_heapstart
#define _malloc_heap_end g_heapend

char *_brkval;
struct _freelist *_flp;

void *malloc(size_t len)
{
	struct _freelist *fp1, *fp2;
	char *cp;
	size_t s, avail;

	if (len < sizeof(struct _freelist) - sizeof(size_t))
	{
		len = sizeof(struct _freelist) - sizeof(size_t);
	}

	for (s = 0, fp1 = _flp, fp2 = 0;fp1;fp2 = fp1, fp1 = fp1->nx)
	{
		if (fp1->sz == len)
		{
			if (fp2)
			{
				fp2->nx = fp1->nx;
			}
			else
			{
				_flp = fp1->nx;
			}
			return &(fp1->nx);
		}
		if (fp1->sz > len)
		{
			if ((s == 0) || (fp1->sz < s))
			{
				s = fp1->sz;
			}
		}
	}
	if (s)
	{
		if (s - len < sizeof(struct _freelist))
		{
                        len = s;
		}

                for (fp1 = _flp, fp2 = 0;
                     fp1;
                     fp2 = fp1, fp1 = fp1->nx)
		{
                        if (fp1->sz == s) 
			{
                                if (len == s) 
				{
                                        if (fp2)
					{
                                                fp2->nx = fp1->nx;
					}
                                        else
					{
                                                _flp = fp1->nx;
					}
                                        return &(fp1->nx);
                                }

                                cp = (char *)fp1;
                                s -= len;
                                cp += s;
                                fp2 = (struct _freelist *)cp;
                                fp2->sz = len;
                                fp1->sz = s - sizeof(size_t);
                                return &(fp2->nx);
                        }
                }
	}
	if (_brkval == 0)
	{
                _brkval = _malloc_heap_start;
	}
        	cp = _malloc_heap_end;
#if 0
        if (cp == 0)
                cp = STACK_POINTER() - __malloc_margin;
#endif
        avail = cp - _brkval;

        if (avail >= len && avail >= len + sizeof(size_t)) 
	{
                fp1 = (struct _freelist *)_brkval;
                _brkval += len + sizeof(size_t);
                fp1->sz = len;
                return &(fp1->nx);
        }

        return 0;
}

void free(void *p)
{
	struct _freelist *fp1, *fp2, *fpnew;
        char *cp1, *cp2, *cpnew;

        if (p == 0)
                return;

        cpnew = p;
        cpnew -= sizeof(size_t);
        fpnew = (struct _freelist *)cpnew;
        fpnew->nx = 0;

        if (_flp == 0) {
                _flp = fpnew;
                return;
        }

        for (fp1 = _flp, fp2 = 0;
             fp1;
             fp2 = fp1, fp1 = fp1->nx) 
	{
                if (fp1 < fpnew)
                        continue;
                cp1 = (char *)fp1;
                fpnew->nx = fp1;
                if ((char *)&(fpnew->nx) + fpnew->sz == cp1) 
		{
                        fpnew->sz += fp1->sz + sizeof(size_t);
                        fpnew->nx = fp1->nx;
                }
                if (fp2 == 0) 
		{
                        _flp = fpnew;
                        return;
                }
                break;
        }

        fp2->nx = fpnew;
        cp2 = (char *)&(fp2->nx);
        if (cp2 + fp2->sz == cpnew) 
	{
                fp2->sz += fpnew->sz + sizeof(size_t);
                fp2->nx = fpnew->nx;
        }
}
