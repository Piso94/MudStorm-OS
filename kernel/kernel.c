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
#include <paging.h>
#include <initrd.h>
#include <fs.h>
#include <commands.h>
#include <console.h>
#include <fpu.h>
#include <double.h>

extern uint32_t placement_address;

void _start(struct multiboot *mbd, size_t boot_magic)
{
	cls(); // Pulisce lo schermo
	
	/*
	 * Non serve a molto! :)
	 * printk("\t\t\t   VGA type: "); // Scrive a video
	 * if (detect_videotype()) // Se la funzione ritorna true
	 *	printk("MonoChrome\n"); // Scrive a video
	 * else // Altrimenti
	 * 	Log.d("Colour\n"); // Scrive a video
	 **/

	init_dbl_buffer();
	//_main();

	set_color(green);
	printk("Boot: 0x%x  [", &boot_magic);
	set_color(white);

	if (boot_magic != 0x2BADB002) // Se magic è diverso da
	{
		Log.e("FAIL"); // Scrive a video
		for (int i=0;i<999999900;i++);
		shutdown();
	}
	else
	{
		Log.i("OK");
	}
	
	Log.i("]");

	uint32_t initrd_location = *((uint32_t*)mbd->mods_addr);
	uint32_t initrd_end = *(uint32_t*)(mbd->mods_addr + 4);
	placement_address = initrd_end;

	gdt_install(); // Inizializza le GDT
	Log.i("\nGDT		[OK]");
   	idt_install(); // Inizializza gli IDT
	Log.i("\nIDT		[OK]");
   	isrs_install(); // Inizializza le ISR
	Log.i("\nISR		[OK]");
   	irq_install(); // Inizializza gli IRQ
	Log.i("\nIRQ		[OK]");
	timer_install(); // Inizializza il timer
	Log.i("\nTimer		[OK]"),
	keyboard_install(); // Inizializza la tastiera
	Log.i("\nTastiera	[OK]");
	mouse_install(); // Inizializza il mouse
	Log.i("\nMouse		[OK]");
	paging_install(); // Inizializza il paging
	Log.i("\nPaging		[OK]");
	fpu_install(); // Inizializzo l'fpu
	Log.i("\nFPU		[OK]");
	fs_root = init_initrd(initrd_location); // Inizializzo il filesystem
	Log.i("\nRAMFS		[OK]");

	set_color(green);
	size_t ram = (size_t)(((mbd->mem_lower + mbd->mem_upper) / 1024) + 1); // Prendo il valore della memoria "minore", la sommo con quella "maggiore", ottengo la memoria ram in KB, divido per 1024, ottengo la ram in MB meno 1, quindi sommo il risultato per 1!
	printk("\nRAM: %u MB	[OK]", ram); //Ok, funziona alla perfezione!
	set_color(white);
	asm volatile ("sti"); // Abilita gli interrupt
	Log.i("\nInterrupt	[OK]\n");

	runShell(); // Entra nella funzione
}
