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

#include <types.h>
#include <console.h>
#include <video.h>
#include <stdio.h>
#include <version.h>
#include <io.h>

void shutdown()
{
	// Questa è l'istruzione vecchia che spegneva solo la macchina virtuale QEMU!
	//asm volatile ("outw %1, %0" : : "dN" ((unsigned short)0xB004), "a" ((unsigned short)0x2000));
	
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
	set_color(RED);
	printk("\t\t\t       MudStorm OS\n");
	printk("\t\t\t       __________\n"); 
  	printk("\t\t\t       |        |\n");
  	printk("\t\t\t       | |\134  /| |\n");
  	printk("\t\t\t       |_| \134/ |_|\n");
	printk("\t\t\t  %s:%s - %s:%s\n\n", "Kernel", KERNEL, "Shell", SHELL);
	set_color(WHITE);
}
