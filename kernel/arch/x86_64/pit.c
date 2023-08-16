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
#include <quantum/arch/x86_64/pit.h>

static unsigned long pit_ticks = 0;

void pit_interrupt_handler(registers_t* regs)
{
    pit_ticks++;
}

unsigned long pit_get_ticks()
{
    return pit_ticks;
}

void pit_set_frequency(int freq)
{
    int divisor = 1193180 / freq; 

    outb(PIT_MODE_COMMAND_REG,     0x36);
    outb(PIT_CHANNEL_O_DATA_PORT,  divisor & 0xFF);
    outb(PIT_CHANNEL_O_DATA_PORT,  divisor >> 8);
}

void pit_sleep(unsigned long time_ms)
{
    unsigned long af_ticks = pit_ticks + time_ms;

    while (pit_ticks < af_ticks)
        ;
}

void pit_init()
{
    pit_ticks = 0;
    asm volatile ("cli");

    isr_register_interrupt_handler(IRQ_BASE, pit_interrupt_handler);  
    pit_set_frequency(1000);

    asm volatile ("sti");

    success_printf("pit_init", "successfully initialized pit timer at 1000Hz\n");
}