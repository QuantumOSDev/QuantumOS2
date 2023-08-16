/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/kernel.h>
#include <quantum/panic.h>
#include <quantum/multiboot.h>

#include <quantum/drivers/pio.h>

#include <quantum/clib/stdlib.h>
#include <quantum/clib/stdio.h>

#include <quantum/arch/x86_64/idt.h>
#include <quantum/arch/x86_64/isr.h>
#include <quantum/arch/x86_64/pic.h>
#include <quantum/arch/x86_64/gdt.h>

isr_handler_t interrupt_handlers[INTERRUPT_HANDLERS_COUNT];

char *exception_messages[32] = {
   "Division By Zero",
   "Debug",
   "Non Maskable Interrupt",
   "Breakpoint",
   "Overflow",
   "BOUND Range Exceeded",
   "Invalid Opcode",
   "Device Not Available (No Math Coprocessor)",
   "Double Fault",
   "Coprocessor Segment Overrun",
   "Invalid TSS",
   "Segment Not Present",
   "Stack-Segment Fault",
   "General Protection",
   "Page Fault",
   "Unknown Interrupt (intel reserved)",
   "x87 FPU Floating-Point Error (Math Fault)",
   "Alignment Check",
   "Machine Check",
   "SIMD Floating-Point Exception",
   "Virtualization Exception",
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

void isr_register_interrupt_handler(int index, isr_handler_t handler)
{
    if (index < INTERRUPT_HANDLERS_COUNT)
        interrupt_handlers[index] = handler;
}

void isr_end_interrupt(int index)
{
    pic_eoi((unsigned char)index);
}

void isr_irq_handler(registers_t* regs)
{
    if (interrupt_handlers[regs->int_no] != NULL)
        ((isr_handler_t)(interrupt_handlers[regs->int_no]))(regs);

    pic_eoi((unsigned char) regs->int_no);
}

void isr_exception_handler(registers_t regs)
{
    if (regs.int_no < 32)
    {
        kernel_panic("isr_exception_handler", "isr error occured (0x%x): %s",
            regs.int_no, exception_messages[regs.int_no]);
        asm volatile ("hlt");
    }

    if (interrupt_handlers[regs.int_no] != NULL)
        ((isr_handler_t)(interrupt_handlers[regs.int_no]))(&regs);
}