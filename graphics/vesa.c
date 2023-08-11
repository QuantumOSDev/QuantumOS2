/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/graphics/vesa.h>

#include <quantum/graphics/unifont.h>
#include <quantum/graphics/color.h>

static vesa_graphics_t vesa_global_info;

vesa_graphics_t* vesa_init(multiboot_info_t* mb_info)
{
    vesa_global_info.fb_ptr = (void*)mb_info->framebuffer_addr;
    vesa_global_info.mb_ptr = (void*)mb_info;

    vesa_global_info.width   = mb_info->framebuffer_width;
    vesa_global_info.height  = mb_info->framebuffer_height;
    vesa_global_info.pitch   = mb_info->framebuffer_pitch;
    vesa_global_info.fb_size = vesa_global_info.width * 4 + vesa_global_info.height * vesa_global_info.pitch;

    return vesa_get_struct_info();
}

void vesa_put_pixel(int x, int y, color_t color)
{
    unsigned offset = x * 4 + y * ((multiboot_info_t*)vesa_global_info.mb_ptr)->framebuffer_pitch;

    ((unsigned char*)vesa_global_info.fb_ptr)[offset    ] = color.b;
    ((unsigned char*)vesa_global_info.fb_ptr)[offset + 1] = color.g;
    ((unsigned char*)vesa_global_info.fb_ptr)[offset + 2] = color.r;
}

void vesa_draw_rect(int x, int y, int w, int h, color_t color)
{
    for (int _x = 0; _x < w; _x++)
        for (int _y = 0; _y < h; _y++)
            vesa_put_pixel(x + _x, y + _y, color);
}

vesa_graphics_t* vesa_get_struct_info()
{
    return &vesa_global_info;
}

void vesa_draw_character(char c, int x, int y, color_t fg, 
                         color_t bg, unsigned char* __font_data)
{
    char* font_char = &font_data[c * FONT_WIDTH];

    for (int i = 1; i < FONT_WIDTH + 1; i++)
        for (int j = 0; j < FONT_HEIGHT + 1; j++)
            if (font_char[i] & (1 << (FONT_HEIGHT - j)))
                vesa_put_pixel(x+j, y+i, fg);
}

void vesa_clear()
{
    for (int i = 0; i < vesa_global_info.fb_size; i++)
        ((unsigned char*)vesa_global_info.fb_ptr)[i] = 0;
}