/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#ifndef __QUANTUM_PIC_H
#define __QUANTUM_PIC_H

#define PIC1         0x20
#define PIC2         0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)
#define PIC_EOI      0x20

#define ICW1         0x11
#define ICW4_8086    0x01

void pic_init();

void pic_eoi(unsigned char irq);

#endif /* __QUANTUM_PIC_H */