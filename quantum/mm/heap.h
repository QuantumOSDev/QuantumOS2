/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#ifndef __QUANTUM_HEAP_H
#define __QUANTUM_HEAP_H

#include <quantum/mm/kmmap.h>

#include <quantum/libs/stdio.h>
#include <quantum/libs/stdlib.h>

#define MAGIC_HEAP_PAGE_FREE { 0x50, 0x3D, 0xFF, 'F', 'R', 'E', 'E', 0x5F }
#define MAGIC_HEAP_PAGE_USED { 0x50, 0x3D, 0xFF, 'U', 'S', 'E', 'D', 0x5F }

typedef struct __heap_page_t {
    unsigned char         magic[8];
    unsigned long         size;
    void*                 ptr;
    struct __heap_page_t* next;
    struct __heap_page_t* last;
} heap_page_t;

typedef struct __heap_t {
    size_t mem_used;
    size_t page_len;

    kernel_memory_map_t* kmmap;
    heap_page_t*         first_page;
} heap_t;


void heap_init(kernel_memory_map_t* kmmap);

void heap_allocate(size_t n);

#endif /* __QUANTUM_HEAP_H */