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

#include <intr/irq.h>
#include <intr/idt.h>
#include <intr/isrs.h>
#include <io.h>

/// IRQ or Interrupt Request line, are 16 lines that are hardware interrupts
/// Controlled by the PIC, Since these are hardware, they are a bit different
/// Than software (we have to acknowledge the interrupt so the chip knows we 
/// Got it. So they have their own functions to handle that.
extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

void *irq_routines[16] =
    /// This is a jump vector with addresses to functions to handle the irq's
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void irq_install_handler(int irq, void (*handler)(struct regs *r))
    /// This installs an IRQ handler that will be run when an IRQ is triggered
{
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
    /// This uninstalls an IRQ handler function.
{
    irq_routines[irq] = 0;
}

void irq_remap(void)
    /// BIOS maps the IRQ's in such a way they conflict with the CPU exceptions
    /// We need to remap the PIC so this doesn't happen.
{
    outportb(0x20, 0x11);    
    outportb(0xA0, 0x11);    
    outportb(0x21, 0x20);    
    outportb(0xA1, 0x28);   
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);  
    outportb(0xA1, 0x0);
}

void irq_install()
    /// This installs the IRQ subsytem and allows us to use them
{
    irq_remap();

    // Master
    idt_set_gate(32, (unsigned)_irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned)_irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned)_irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned)_irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned)_irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned)_irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned)_irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned)_irq7, 0x08, 0x8E);
    // Slave
    idt_set_gate(40, (unsigned)_irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned)_irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned)_irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned)_irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned)_irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned)_irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned)_irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned)_irq15, 0x08, 0x8E);
}

void _irq_handler(struct regs *r)
{
    void (*handler)(struct regs *r);        // Void Pointer Function Call

    handler = irq_routines[r->int_no - 32]; // Figure out what IRQ was Called
    if (handler)                            // Does the IRQ have a handler?
    {
        handler(r);                         // Run it then.
    }

    if (r->int_no >= 40)                    // Was this call from the slave PIC?
    {
        outportb(0xA0, 0x20);               // Send an EOI to it
    }

    outportb(0x20, 0x20);                   // Send an EOI to the master PIC.
}
