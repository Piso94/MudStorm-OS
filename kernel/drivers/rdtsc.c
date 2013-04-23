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

#include <rdtsc.h>
#include <io.h>
#include <delay.h>

qword currentcycles() 
{
    qword result; // Inizializzo la variabile "result" come "unsigned long long"
    asm volatile ("rdtsc" : "=A" (result)); // Passo la velocità(attuale) della CPU alla variabile "result"
    return result; // Ritorno il valore della variabile "result"
}

unsigned long cpu_speed()
{
	asm volatile ("cli"); // Disabilito gli interrupt
	outportb(0x43,0x34);   // set PIT channel 0 to single-shot mode
	outportb(0x40,0);
	outportb(0x40,0);      // program the counter will be 0x10000 - n after n ticks
	unsigned long stsc = currentcycles(); // Richiedo la velocità attuale della CPU
	for (int i=0x1000;i>0;i--); // Attendo 0x1000 (1s)
	unsigned long etsc = currentcycles(); // Richiedo la velocità attuale della CPU
	outportb(0x43,0x04);   // read PIT counter command ??
	byte lo = inportb(0x40);
	byte hi = inportb(0x40);
	asm volatile ("sti"); // Abilito gli interrupt

	unsigned long ticks = (0x10000 - (hi*256+lo));

	return ((etsc-stsc) * 1193180) / ticks; // Ritorno un valore unsigned long
}

void DelayS (int s)
{
	DelayMs(s * 3000); // Richiamo la funzione "DelayMs" e gli assegno il valore "s * 3000"
}

void DelayMs (int ms)
{
	qword t1 = currentcycles(); // Assegno alla variabile "t1" il valore di currentcycles()
	for(int i=0;i<10;i++); // Attendo 10 cicli
	qword t2 = currentcycles(); // Assegno alla variabile "t2" il valore di currentcycles()

	qword t = (t2 - t1) * ms; // Assegno alla variabile "t" il valore "(t2 - t1) * ms"

	for (qword i=0; i<t; i++); // Attendo fin quando "i" diventa minore di "t"
}
