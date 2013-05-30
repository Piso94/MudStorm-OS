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
#include <console.h>
#include <video.h>
#include <stdio.h>
#include <version.h>
#include <io.h>
#include <log.h>

void shutdown()
{
	// Questa è quella nuova che spegne tutte le VM, ma non quelle reali!
	asm volatile ("cli");
  	for ( ; ; )
  	{
	      // Funziona per QEMU e Bochs
	      outportw (0xB004, 0x2000);
	
	      // Magic shutdown per Bochs e QEMU
	      for (const char *s = "Shutdown"; *s; ++s)
		        outportb (0x8900, *s);

	      // Magic code per VMWare
	      asm volatile ("cli; hlt");
	}
}

void reboot()
{
	int good = 0x02;
	while (good & 0x02)
	{
	    	good = inportb(0x64);
	    	outportb(0x64, 0xFE);
	}
	asm volatile ("hlt");
}

void logo()
{
	// Nuovo Logo
	Log.d("\t\t\t       __________\n"); 
  	Log.d("\t\t\t       |        |\n");
  	Log.d("\t\t\t       | |\134  /| |\n");
  	Log.d("\t\t\t       |_| \134/ |_|\n");
	Log.d("\t\t\t       MudStorm OS\n");
}
