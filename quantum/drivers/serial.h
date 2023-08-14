/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#ifndef __QUANTUM_SERIAL_H
#define __QUANTUM_SERIAL_H

#define SERIAL_PORT 0x3F8

void serial_putc(char c);

void serial_print(char* str);
void serial_printf(const char* fmt, ...);

#endif /* __QUANTUM_SERIAL_H */