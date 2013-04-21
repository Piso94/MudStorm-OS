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

#include <intr/gdt.h>

struct gdt_entry
    /// Define a GDT Table entry.
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char  base_middle;
    unsigned char  access;
    unsigned char  granularity;
    unsigned char  base_high;
} __attribute__((packed));

struct gdt_ptr
    /// Pointer for the CPU to access our GDT
{
    unsigned short limit;
    unsigned int   base;
} __attribute__((packed));

struct gdt_entry gdt[3];    // 3 Entry GDT; Null, Code, & Data
struct gdt_ptr gp;          // Pointer to that GDT

extern void _gdt_flush();    // Function in loader.asm 

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
    /// Set up a descriptor in the Table, it's cause we're lazy
{
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void gdt_install()
    /// Initiates our GDT with flat 32-bit addresses.
{
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;  // Set up the Pointer
    gp.base = (unsigned int)&gdt;                   // To the Table

    gdt_set_gate(0, 0, 0, 0, 0);                // 1st Descriptor is null
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code Segment, Kernel Mode
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data Segment, Kernel Mode

    _gdt_flush();                                // Tell CPU we have a new GDT
}
