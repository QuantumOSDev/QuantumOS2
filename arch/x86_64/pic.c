/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/kernel.h>
#include <quantum/multiboot.h>

#include <quantum/drivers/pio.h>

#include <quantum/libs/stdlib.h>
#include <quantum/libs/stdio.h>

#include <quantum/arch/x86_64/idt.h>
#include <quantum/arch/x86_64/isr.h>
#include <quantum/arch/x86_64/pic.h>
#include <quantum/arch/x86_64/gdt.h>

void pic_init()
{
    unsigned char a1, a2;

    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1);
    outb(PIC2_COMMAND, ICW1);

    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);

    outb(PIC1_DATA, 4);
    outb(PIC2_DATA, 2);

    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);

    success_printf("pic_init", "successfully initialized pic\n");
}

void pic_eoi(unsigned char irq)
{
    if (irq >= 0x28)
        outb(PIC2, PIC_EOI);

    outb(PIC1, PIC_EOI);
}