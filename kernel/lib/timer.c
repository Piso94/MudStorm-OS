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

#include <timer.h>
#include <io.h>
#include <intr/irq.h>

volatile int PIC_ticks = 0; // Volatile to avoid infinite loops in optimization.

void PIC_handler(struct regs *r)
/// Callback from ISR 0 
{ 
	PIC_ticks++;            // Increment Tick count
} 

void wait(int ticks)
/// Hold the system until specified time is reached
{
    unsigned long eticks;

    eticks = PIC_ticks + ticks;
    while(PIC_ticks < eticks);
}

void timer_install()
/// Sets up the system clock by installing the timer handler into IRQ0
{
    unsigned long hz = 1000;                                // 1 tick / mSec
    asm("cli");                                             // Stop interrupts
    unsigned long divisor = 1193180 / hz;                   // Find the divisor
    outportb(0x43, 0x36);              // Tell PIC we're setting it's frequency
    unsigned char l = (unsigned char)(divisor & 0xFF);      // Bitmap Low byte
    unsigned char h = (unsigned char)(divisor >> 8) & 0xFF; // Bitmap High byte
    outportb(0x40, l);                                      // Send Low Byte!
    outportb(0x40, h);                                      // Send High Byte!
    irq_install_handler(0, PIC_handler);  // Install Pointer to the ISRS Table
    asm("sti"); // Restore interrupts
}
