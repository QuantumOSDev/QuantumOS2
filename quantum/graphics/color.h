/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#ifndef __QUANTUM_COLOR_H
#define __QUANTUM_COLOR_H

typedef struct __color_t {
    unsigned char r, g, b, a;
} color_t;

color_t color_create_rgb (unsigned char r, unsigned char g, unsigned char b);
color_t color_create_rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

#endif /* __QUANTUM_COLOR_H */