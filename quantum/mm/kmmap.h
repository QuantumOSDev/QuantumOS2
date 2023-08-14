/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#ifndef __QUANTUM_KMMAP_H
#define __QUANTUM_KMMAP_H

#include <quantum/multiboot.h>

#include <quantum/libs/stdio.h>
#include <quantum/libs/stdlib.h>

typedef unsigned long u64;
typedef int i32;

extern unsigned char kernel_section_start;
extern unsigned char kernel_section_end;
extern unsigned char kernel_text_section_start;
extern unsigned char kernel_text_section_end;
extern unsigned char kernel_data_section_start;
extern unsigned char kernel_data_section_end;
extern unsigned char kernel_rodata_section_start;
extern unsigned char kernel_rodata_section_end;
extern unsigned char kernel_bss_section_start;
extern unsigned char kernel_bss_section_end;

typedef struct __kernel_memory_map_t {
    struct {
        unsigned int kernel_start;
        unsigned int kernel_end;
        unsigned int kernel_len;

        unsigned int text_start;
        unsigned int text_end;
        unsigned int text_len;

        unsigned int data_start;
        unsigned int data_end;
        unsigned int data_len;

        unsigned int rodata_start;
        unsigned int rodata_end;
        unsigned int rodata_len;

        unsigned int bss_start;
        unsigned int bss_end;
        unsigned int bss_len;
    } kernel;

    struct {
        unsigned int total_memory;
    } system;

    struct {
        unsigned long start;
        unsigned long end;
        unsigned long size;
    } available;
} kernel_memory_map_t;

typedef struct multiboot_memory_map_edited_t {
    unsigned int size;
    unsigned int addr_low;
    unsigned int addr_high;
    unsigned int len_low;
    unsigned int len_high;
    unsigned int type;
} multiboot_memory_map_edited_t;

int kmmap_get_kernel_memory_map(kernel_memory_map_t* map, multiboot_info_t* mb);

#endif /* __QUANTUM_KMMAP_H */
