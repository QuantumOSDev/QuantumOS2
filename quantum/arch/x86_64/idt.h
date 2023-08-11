/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#ifndef __QUANTUM_IDT_H
#define __QUANTUM_IDT_H

#define IDT_DESCRIPTOR_COUNT 256

typedef struct __idt_t {
    unsigned short base_low;
    unsigned short segment_selector;
    unsigned char zero;
    unsigned char type;
    unsigned short base_high;
} __attribute__((packed)) idt_t;

typedef struct __idt_ptr_t {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idt_ptr_t;

extern void load_idt(unsigned int idt_ptr);

void idt_set_entry(int index, unsigned int base, unsigned short seg, unsigned char flags);
void idt_init();

#endif /* __QUANTUM_IDT_H */