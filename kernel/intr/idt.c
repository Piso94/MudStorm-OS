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

#include <intr/idt.h>
#include <string.h>

extern void _idt_load();     // From 'loader.S', Loads the IDT to the CPU

struct idt_entry
    /// IDT table entry so the CPU knows how to handle a specific interrupt
{
    uint16_t base_lo;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr
    /// Pointer to the Interrupt Vector Table to give to the CPU
{
    uint16_t limit;
    size_t   base;
} __attribute__((packed));

/***
Create a static table of 256 interrupts, this will be used as the vector table
That the CPU uses to handle the ISR's, also the CPU takes the pointer in a very
Specific format, so we need an instance of that too.
***/
struct idt_entry idt[256];
struct idt_ptr idtp;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
    /// Set an interrupt's entry in the table
{
    /* The interrupt routine's base address */
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    /* The segment or 'selector' that this IDT entry will use
    *  is set here, along with any access flags */
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install()
    /// Create and install the IDT
{
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1; // Set up the pointer
    idtp.base = (size_t)&idt;                     // Base of pointer...
    memset(&idt, 0, sizeof(struct idt_entry) * 256);    // Clear IDT to all 0's
    _idt_load();                                         // Load it to the CPU.
}
