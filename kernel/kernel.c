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

size_t ram;

void _start(struct multiboot *mbd, size_t magic)
{
	cls(); // Pulisce lo schermo

	if (magic != 0x2BADB002) // Se magic è diverso da ...
	{
		Log.e("Boot [FAIL]");
		delay_ms(500); // Attendo 1/2 secondo
		asm ("cli"); // Disabilita gli interrupt
		asm ("hlt"); // Ferma la CPU
	}
	else
	{
		Log.i("Boot [Ok]\n\n");
		delay_ms(500); // Attendo 1/2 secondo
		cls(); // Pulisce lo schermo
	}
	
	gdt_install(); // Inizializza le GDT
   	idt_install(); // Inizializza gli IDT
   	isrs_install(); // Inizializza le ISR
   	irq_install(); // Inizializza gli IRQ
	timer_install(); // Inizializza il timer
	mouse_install(); // Inizializza il mouse
	keyboard_install(); // Inizializza la tastiera
	enable_fpu(); // Abilita l'fpu
	init_fpu(); // Inizializza l'fpu

   	asm volatile ("sti"); // Abilita gli interrupt

	ram = (mbd->mem_lower + mbd->mem_upper) / 1024 + 1; 
	/*
	 * Prendo il valore della memoria "minore", 
         * la sommo con quella "maggiore", 
         * ottengo la memoria ram in KB, divido per 1024, 
         * ottengo la ram in MB - 1, 
         * quindi sommo il risultato per 1!
         **/

	runShell(); // Entra nella funzione
}
