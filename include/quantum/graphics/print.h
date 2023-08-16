/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#ifndef __QUANTUM_PRINT_H
#define __QUANTUM_PRINT_H

#include <quantum/graphics/color.h>

typedef struct __print_t {
    color_t fg, bg;
    int x, y;
} print_t;

void print_init(color_t fg, color_t bg);

void print(const char* msg);

void putchar(char c);

print_t* get_print_structure();

#endif /* __QUANTUM_PRINT_H */