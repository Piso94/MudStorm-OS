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

#include <intr/isrs.h>
#include <intr/idt.h>
#include <stdio.h>
#include <kb.h>
#include <log.h>

/***
Intel has reserved the first 32 interrupts for system use, for things like
Overflow and divide by zero errors. These are displayed here.
***/
extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();


void isrs_install()
    /// Install The interrupts into the IDT, There are 4 Arguments:
    ///     - The Interrupt Number
    ///     - The Pointer to the function
    ///     - Selector
    ///     - Access Flags (0x8E = Ring 0, Present entry)
{
    // The first 32 Entries are intel exceptions
    idt_set_gate(0, (size_t)_isr0, 0x08, 0x8E);
    idt_set_gate(1, (size_t)_isr1, 0x08, 0x8E);
    idt_set_gate(2, (size_t)_isr2, 0x08, 0x8E);
    idt_set_gate(3, (size_t)_isr3, 0x08, 0x8E);
    idt_set_gate(4, (size_t)_isr4, 0x08, 0x8E);
    idt_set_gate(5, (size_t)_isr5, 0x08, 0x8E);
    idt_set_gate(6, (size_t)_isr6, 0x08, 0x8E);
    idt_set_gate(7, (size_t)_isr7, 0x08, 0x8E);

    idt_set_gate(8, (size_t)_isr8, 0x08, 0x8E);
    idt_set_gate(9, (size_t)_isr9, 0x08, 0x8E);
    idt_set_gate(10, (size_t)_isr10, 0x08, 0x8E);
    idt_set_gate(11, (size_t)_isr11, 0x08, 0x8E);
    idt_set_gate(12, (size_t)_isr12, 0x08, 0x8E);
    idt_set_gate(13, (size_t)_isr13, 0x08, 0x8E);
    idt_set_gate(14, (size_t)_isr14, 0x08, 0x8E);
    idt_set_gate(15, (size_t)_isr15, 0x08, 0x8E);

    idt_set_gate(16, (size_t)_isr16, 0x08, 0x8E);
    idt_set_gate(17, (size_t)_isr17, 0x08, 0x8E);
    idt_set_gate(18, (size_t)_isr18, 0x08, 0x8E);
    idt_set_gate(19, (size_t)_isr19, 0x08, 0x8E);
    idt_set_gate(20, (size_t)_isr20, 0x08, 0x8E);
    idt_set_gate(21, (size_t)_isr21, 0x08, 0x8E);
    idt_set_gate(22, (size_t)_isr22, 0x08, 0x8E);
    idt_set_gate(23, (size_t)_isr23, 0x08, 0x8E);

    idt_set_gate(24, (size_t)_isr24, 0x08, 0x8E);
    idt_set_gate(25, (size_t)_isr25, 0x08, 0x8E);
    idt_set_gate(26, (size_t)_isr26, 0x08, 0x8E);
    idt_set_gate(27, (size_t)_isr27, 0x08, 0x8E);
    idt_set_gate(28, (size_t)_isr28, 0x08, 0x8E);
    idt_set_gate(29, (size_t)_isr29, 0x08, 0x8E);
    idt_set_gate(30, (size_t)_isr30, 0x08, 0x8E);
    idt_set_gate(31, (size_t)_isr31, 0x08, 0x8E);
}

char *exception_messages[] =
    /// Just A fancy Array of charecters here
    /// We use this to identify the intel Exceptions to the kernel
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};


void _fault_handler(struct regs *r)
    /// This is where the interrupts are handled, From here, they're sent to a
    /// Jump table that runs the interrupt specific function.
    /* TODO: Add A jump table for software interrupts */
{
    if (r->int_no < 32)
    {
	   Log.e("\n\t\t\t\t  KERNEL PANIC :/");
	   printk("\n\t\t\t\t%s", exception_messages[r->int_no]);
	   update_leds(CapsLock);
	   update_leds(NumLock);
	   asm ("cli");
	   asm ("hlt");
    }
}
