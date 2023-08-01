/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <quantum/kernel.h>
#include <quantum/multiboot.h>

#include <quantum/graphics/color.h>
#include <quantum/graphics/fonts.h>
#include <quantum/graphics/vesa.h>

#include <quantum/drivers/vga.h>
#include <quantum/drivers/pio.h>

void kernel_init(unsigned long magic, unsigned long addr)
{
    multiboot_info_t* mb_info = (multiboot_info_t*)addr;

    vesa_graphics_t* vesa_graphics = vesa_init(mb_info);

    print_init(color_create_rgb(250, 250, 250), color_create_rgb(0, 0, 0));
    print("hello");
}