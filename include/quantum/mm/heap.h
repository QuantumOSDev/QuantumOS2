/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#ifndef __QUANTUM_HEAP_H
#define __QUANTUM_HEAP_H

#include <quantum/mm/kmmap.h>

#include <quantum/clib/stdio.h>
#include <quantum/clib/stdlib.h>

#define MAGIC_HEAP_PAGE { 0x50, 0x3D, 0xFF, 'Q', 'T', 'O', 'S', 0x5F }

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

heap_page_t* heap_get_page_by_addr(void* ptr);

void* heap_allocate(size_t size);
void  heap_free(void* ptr);

#endif /* __QUANTUM_HEAP_H */