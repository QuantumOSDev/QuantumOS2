/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#ifndef __QUANTUM_PIO_H
#define __QUANTUM_PIO_H

void outb(unsigned short port, unsigned char data);
void outs(unsigned short port, unsigned short data);
void outl(unsigned short port, unsigned int data);

unsigned char  inb(unsigned short port);
unsigned short ins(unsigned short port);
unsigned int   inl(unsigned short port);

void psleep(void);

#endif /* __QUANTUM_PIO_H */