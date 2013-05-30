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
#include <fat.h>
#include <flp.h>
#include <fsys.h>

void cmd_read();

char *flp_type()
{
	uint8_t flp;
	outportb(0x70, 0x10);
	flp = inportb(0x71);
	uint8_t a = (flp >> 4);
	char *drive_type[6] = { "no floppy", "360kb 5.25", "1.2mb 5.25", "720kb 3.5", "1.44mb 3.5", "2.88mb 3.5" };
	
	return drive_type[a];
}

void _start(struct multiboot *mbd, size_t magic)
{
	cls(); // Pulisce lo schermo
	
	Log.i("Boot: 0x%x  ", &magic);

	if (magic != 0x2BADB002) // Se magic è diverso da ...
	{
		Log.e("[FAIL]\n");
		asm ("cli"); // Disabilita gli interrupt
		asm ("hlt"); // Ferma la CPU
	}
	else
	{
		Log.i("[OK]\n");
	}

	
	gdt_install(); // Inizializza le GDT
	Log.i("GDT\t\t[OK]");
   	idt_install(); // Inizializza gli IDT
	Log.i("\nIDT\t\t[OK]");
   	isrs_install(); // Inizializza le ISR
	Log.i("\nISR\t\t[OK]");
   	irq_install(); // Inizializza gli IRQ
	Log.i("\nIRQ\t\t[OK]");
	timer_install(); // Inizializza il timer
	Log.i("\nTimer\t\t[OK]");
	mouse_install(); // Inizializza il mouse
	Log.i("\nMouse\t\t[OK]");
	keyboard_install(); // Inizializza la tastiera
	Log.i("\nTastiera\t[OK]");
	enable_fpu(); // Abilita l'fpu
	init_fpu(); // Inizializza l'fpu
	Log.i("\nFPU\t\t[OK]");
	flp_set_working_drive(0); // Setto il drive floppy 0
	flp_install(6); // Inizializzo il driver del floppy
	Log.i("\nFloppy\t\t[%s]", flp_type());
	//fat_install(); // Inizializzo il filesystem FAT // TODO: Sistemare
	Log.e("\nFileSystem\t[JUMP]");
   	asm volatile ("sti"); // Abilita gli interrupt

	size_t ram = (size_t)(((mbd->mem_lower + mbd->mem_upper) / 1024) + 1); // Prendo il valore della memoria "minore", la sommo con quella "maggiore", ottengo la memoria ram in KB, divido per 1024, ottengo la ram in MB - 1, quindi sommo il risultato per 1!
	Log.i("\nRAM: %u MB\n", ram); //Ok, funziona!	
	//cmd_read();
	runShell(); // Entra nella funzione
}

void cmd_read() 
{
	char read[40] = { 0 };
	printk("Nome: ");
	scank("%s\n", read);

	file_t file = vol_openfile(read);

	if (file.flags == FS_INVALID) 
	{
		Log.w("File non valido!");
		return;
	}

	if (file.flags == FS_DIR) 
	{
		Log.w("Cartella!");
		return;
	}

	while (file.eof) 
	{
		uint8_t buf[512];
		vol_readfile(&file, buf, 512);

		for (int i=0; i<512; i++)
			putch(buf[i]);
	}
}
