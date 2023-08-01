/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/graphics/print.h>
#include <quantum/graphics/color.h>
#include <quantum/graphics/unifont.h>
#include <quantum/graphics/vesa.h>

static print_t print_info;

void print_init(color_t fg, color_t bg)
{
    print_info.x = 10;
    print_info.y = 10;

    print_info.fg = color_create_rgb(250, 250, 250);
    print_info.bg = color_create_rgb(  0,   0,   0);
}

void print(const char* msg)
{
    int i = 0;

    while (msg[i] != '\0')
    {
        putchar(msg[i]);
        i++;
    }
}

void insert_newline() 
{
    print_info.x = 10;
    print_info.y += 22;

    if (print_info.y >= 720)
    {
        vesa_clear();
        print_info.y = 0;
    }
}

void insert_backspace()
{
    print_info.x = print_info.x - 9;

    vesa_draw_character(' ', print_info.x, print_info.y, print_info.fg, print_info.bg, font_data);
}

void insert_tab() 
{
    for (int i = 0; i < 4; i++)
        putchar(' ');
}

void putchar(char c)
{
    if (c == '\n' || c == '\r')
        insert_newline();
    else if (c == '\b')
        insert_backspace();
    else if (c == '\t')
        insert_tab();
    else
    {
        vesa_draw_character(c, print_info.x, print_info.y, print_info.fg, print_info.bg, font_data);
        print_info.x += 9;

        if (print_info.x >= 1280)
            insert_newline();
    }
}

print_t* get_print_structure()
{
    return &print_info;
}