/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#ifndef __QUANTUM_PIT_H
#define __QUANTUM_PIT_H

#include <quantum/arch/x86_64/isr.h>

#define PIT_CHANNEL_O_DATA_PORT 0x40
#define PIT_CHANNEL_1_DATA_PORT 0x41
#define PIT_CHANNEL_2_DATA_PORT 0x42

#define PIT_MODE_COMMAND_REG    0x43

void pit_interrupt_handler(registers_t* regs);
void pit_set_frequency(int freq);

unsigned long pit_get_ticks();

void pit_sleep(unsigned long time_ms);

void pit_init();

#endif /* __QUANTUM_PIT_H */