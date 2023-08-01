/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#ifndef __QUANTUM_VESA_H
#define __QUANTUM_VESA_H

#include <quantum/multiboot.h>

#include <quantum/graphics/color.h>

typedef struct __vesa_graphics_t {
    void* fb_ptr;
    void* mb_ptr;

    long width, height;
    long pitch;
    long fb_size;
} vesa_graphics_t;

vesa_graphics_t* vesa_init(multiboot_info_t* mb_info);

void vesa_put_pixel(int x, int y, color_t color);

void vesa_draw_rect(int x, int y, int w, int h, color_t color);

void vesa_draw_character(char c, int x, int y, color_t fg, 
                         color_t bg, unsigned char* __font_data);

vesa_graphics_t* vesa_get_struct_info();

#endif /* __QUANTUM_VESA_H */