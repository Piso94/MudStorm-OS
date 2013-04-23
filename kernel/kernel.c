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
#include <shell.h> 
#include <string.h>
#include <intr/gdt.h>
#include <intr/idt.h>
#include <intr/irq.h>
#include <intr/isrs.h>
#include <kb.h>
#include <mouse.h>
#include <timer.h>
#include <video.h>
#include <delay.h>
#include <stdio.h>
#include <io.h>
#include <log.h>

void _start(void *mbd, unsigned int magic)
{
	cls(); // Pulisco lo schermo
	
	printk("\t\t\t   VGA type: "); // Scrive a video
	if (detect_videotype()) // Se la funzione ritorna true
	{
		Log.v("MonoChrome\n"); // Scrive a video
	}
	else // Altrimenti
	{
		Log.d("Colour\n"); // Scrive a video
	}

	if (magic != 0x2BADB002) 
	{
		Log.e("\n\t\t\t\t Errore");
		Log.d("\n\t\t\t  Spegnere il Computer");
		asm ("cli");
		asm ("hlt");
	}

	gdt_install(); // Inizializzo le GDT
	Log.i("\nGDT Inzializzato");
   	idt_install(); // Inizializzo gli IDT
	Log.i("\nIDT Inzializzato");
   	isrs_install(); // Inizializzo le ISR
	Log.i("\nISR Inzializzato");
   	irq_install(); // Inizializzo gli IRQ
	Log.i("\nIRQ Inzializzato");
	timer_install(); // Inizializzo il timer
	Log.i("\nTimer Inzializzato");
	mouse_install(); // Inizializzo il mouse
	Log.i("\nMouse Inzializzato");
	keyboard_install(); // Inizializzo la tastiera
	Log.i("\nKeyboard Inzializzato");
   	asm volatile ("sti"); // Abilito gli interrupt
	Log.i("\nInterrupt Inzializzato");

	Log.i("\nAvvio Shell...\n");
	runShell(); // Esco da questa funzione ed entro in un'altra, contenuta nel file shell/shell.c
}
