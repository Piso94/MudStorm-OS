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
#include <kheap.h>
#include <commands.h>
#include <fpu.h>
#include <cpuid.h>
#include <flp.h>
#include <fat.h>

void _start(struct multiboot *mbd, size_t magic)
{
	cls(); // Pulisce lo schermo
	
	Log.i("Boot: 0x%x  ", &magic);

	if (magic != 0x2BADB002) // Se magic è diverso da ...
	{
		Log.e("[FAIL]");
		asm ("cli"); // Disabilita gli interrupt
		asm ("hlt"); // Ferma la CPU
	}
	else
	{
		Log.i("[Ok]\n\n");
	}
	
	gdt_install(); // Inizializza le GDT
	Log.i("GDT\t\t[Ok]");
   	idt_install(); // Inizializza gli IDT
	Log.i("\nIDT\t\t[Ok]");
   	isrs_install(); // Inizializza le ISR
	Log.i("\nISR\t\t[Ok]");
   	irq_install(); // Inizializza gli IRQ
	Log.i("\nIRQ\t\t[Ok]");
	timer_install(); // Inizializza il timer
	Log.i("\nTimer\t\t[Ok]");
	mouse_install(); // Inizializza il mouse
	Log.i("\nMouse\t\t[Ok]");
	keyboard_install(); // Inizializza la tastiera
	Log.i("\nTastiera\t[Ok]");
	enable_fpu(); // Abilita l'fpu
	init_fpu(); // Inizializza l'fpu
	Log.i("\nFPU\t\t[Ok]");
	flp_install();
	Log.i("\nFloppy\t\t[Ok]");
	fat_install();
	Log.i("\nFAT\t\t[Ok]\n\n");

   	asm volatile ("sti"); // Abilita gli interrupt

	size_t ram;

	ram = (mbd->mem_lower + mbd->mem_upper) / 1024 + 1; 
	/*
	 * Prendo il valore della memoria "minore", 
         * la sommo con quella "maggiore", 
         * ottengo la memoria ram in KB, divido per 1024, 
         * ottengo la ram in MB - 1, 
         * quindi sommo il risultato per 1!
         **/
	Log.i("CPU: %s\n", cpu_vendor());
	Log.i("RAM: %u MB\n", ram);
	
	runShell(); // Entra nella funzione
}
