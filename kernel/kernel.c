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

#include <boot.h>
#include <stddef.h>
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
#include <stdio.h>
#include <io.h>
#include <log.h>
#include <kmalloc.h>

extern uint32_t placement_address;

void _start(struct multiboot *mbd, unsigned int magic)
{

	/*
	 * Per filesystem! Li tengo, magari possono tornare utili :)
	 * uint32_t initrd_location = *((uint32_t*)mbd->mods_addr);
	 * uint32_t initrd_end = *(uint32_t*)(mbd->mods_addr+4);
	 **/

	cls(); // Pulisce lo schermo
	
	printk("\t\t\t   VGA type: "); // Scrive a video
	if (detect_videotype()) // Se la funzione ritorna true
		Log.v("MonoChrome\n"); // Scrive a video
	else // Altrimenti
		Log.d("Colour\n"); // Scrive a video

	if (magic != 0x2BADB002) // Se magic è diverso da
	{
		Log.e("\n\t\t\t\t Errore"); // Scrive a video
		Log.d("\n\t\t\t  Spegnere il Computer"); // Scrive a video
		asm ("cli"); // Disabilita gli interrupt
		asm ("hlt"); // Ferma la CPU
	}

	gdt_install(); // Inizializza le GDT
   	idt_install(); // Inizializza gli IDT
   	isrs_install(); // Inizializza le ISR
   	irq_install(); // Inizializza gli IRQ
	timer_install(); // Inizializza il timer
	mouse_install(); // Inizializza il mouse
	keyboard_install(); // Inizializza la tastiera
   	asm volatile ("sti"); // Abilita gli interrupt

	runShell(); // Entra nella funzione
}
