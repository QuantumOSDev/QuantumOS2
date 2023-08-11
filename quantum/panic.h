/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#ifndef __QUANTUM_PANIC_H
#define __QUANTUM_PANIC_H

void kernel_panic(char* func, const char* message, ...);

#endif /* __QUANTUM_PANIC_H */