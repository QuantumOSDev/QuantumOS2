/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/kernel.h>
#include <quantum/multiboot.h>

#include <quantum/drivers/pio.h>

#include <quantum/clib/stdlib.h>
#include <quantum/clib/stdio.h>

#include <quantum/arch/x86_64/idt.h>
#include <quantum/arch/x86_64/isr.h>
#include <quantum/arch/x86_64/pic.h>
#include <quantum/arch/x86_64/gdt.h>

idt_t idt_desc[IDT_DESCRIPTOR_COUNT];
idt_ptr_t idt;

void idt_set_entry(int index, unsigned int base, unsigned short seg, unsigned char flags)
{
    idt_t *current = &idt_desc[index];

    current->base_low         = (base & 0xFFFF);
    current->segment_selector = seg;
    current->zero             = 0;
    current->type             = (flags | 0x60);
    current->base_high        = (base >> 16) & 0xFFFF;
}

void idt_init(void)
{
    idt.base  = (unsigned int)idt_desc;
    idt.limit = 256 * sizeof(idt_t) - 1;

    pic_init();

    success_printf("idt_init", "setting idt entries\n");
    idt_set_entry(0, (unsigned int)exception_0, 0x08, 0x8E);
    idt_set_entry(1, (unsigned int)exception_1, 0x08, 0x8E);
    idt_set_entry(2, (unsigned int)exception_2, 0x08, 0x8E);
    idt_set_entry(3, (unsigned int)exception_3, 0x08, 0x8E);
    idt_set_entry(4, (unsigned int)exception_4, 0x08, 0x8E);
    idt_set_entry(5, (unsigned int)exception_5, 0x08, 0x8E);
    idt_set_entry(6, (unsigned int)exception_6, 0x08, 0x8E);
    idt_set_entry(7, (unsigned int)exception_7, 0x08, 0x8E);
    idt_set_entry(8, (unsigned int)exception_8, 0x08, 0x8E);
    idt_set_entry(9, (unsigned int)exception_9, 0x08, 0x8E);
    idt_set_entry(10, (unsigned int)exception_10, 0x08, 0x8E);
    idt_set_entry(11, (unsigned int)exception_11, 0x08, 0x8E);
    idt_set_entry(12, (unsigned int)exception_12, 0x08, 0x8E);
    idt_set_entry(13, (unsigned int)exception_13, 0x08, 0x8E);
    idt_set_entry(14, (unsigned int)exception_14, 0x08, 0x8E);
    idt_set_entry(15, (unsigned int)exception_15, 0x08, 0x8E);
    idt_set_entry(16, (unsigned int)exception_16, 0x08, 0x8E);
    idt_set_entry(17, (unsigned int)exception_17, 0x08, 0x8E);
    idt_set_entry(18, (unsigned int)exception_18, 0x08, 0x8E);
    idt_set_entry(19, (unsigned int)exception_19, 0x08, 0x8E);
    idt_set_entry(20, (unsigned int)exception_20, 0x08, 0x8E);
    idt_set_entry(21, (unsigned int)exception_21, 0x08, 0x8E);
    idt_set_entry(22, (unsigned int)exception_22, 0x08, 0x8E);
    idt_set_entry(23, (unsigned int)exception_23, 0x08, 0x8E);
    idt_set_entry(24, (unsigned int)exception_24, 0x08, 0x8E);
    idt_set_entry(25, (unsigned int)exception_25, 0x08, 0x8E);
    idt_set_entry(26, (unsigned int)exception_26, 0x08, 0x8E);
    idt_set_entry(27, (unsigned int)exception_27, 0x08, 0x8E);
    idt_set_entry(28, (unsigned int)exception_28, 0x08, 0x8E);
    idt_set_entry(29, (unsigned int)exception_29, 0x08, 0x8E);
    idt_set_entry(30, (unsigned int)exception_30, 0x08, 0x8E);
    idt_set_entry(31, (unsigned int)exception_31, 0x08, 0x8E);
    idt_set_entry(32, (unsigned int)irq_0, 0x08, 0x8E);
    idt_set_entry(33, (unsigned int)irq_1, 0x08, 0x8E);
    idt_set_entry(34, (unsigned int)irq_2, 0x08, 0x8E);
    idt_set_entry(35, (unsigned int)irq_3, 0x08, 0x8E);
    idt_set_entry(36, (unsigned int)irq_4, 0x08, 0x8E);
    idt_set_entry(37, (unsigned int)irq_5, 0x08, 0x8E);
    idt_set_entry(38, (unsigned int)irq_6, 0x08, 0x8E);
    idt_set_entry(39, (unsigned int)irq_7, 0x08, 0x8E);
    idt_set_entry(40, (unsigned int)irq_8, 0x08, 0x8E);
    idt_set_entry(41, (unsigned int)irq_9, 0x08, 0x8E);
    idt_set_entry(42, (unsigned int)irq_10, 0x08, 0x8E);
    idt_set_entry(43, (unsigned int)irq_11, 0x08, 0x8E);
    idt_set_entry(44, (unsigned int)irq_12, 0x08, 0x8E);
    idt_set_entry(45, (unsigned int)irq_13, 0x08, 0x8E);
    idt_set_entry(46, (unsigned int)irq_14, 0x08, 0x8E);
    idt_set_entry(47, (unsigned int)irq_15, 0x08, 0x8E);
    idt_set_entry(128, (unsigned int)exception_128, 0x08, 0x8E);

    load_idt((unsigned int)&idt);

    success_printf("idt_init", "successfully initialized idt\n");
    asm volatile ("sti");
}