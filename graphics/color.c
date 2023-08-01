/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/graphics/color.h>

color_t color_create_rgb (unsigned char r, unsigned char g, unsigned char b)
{
    return (color_t){ r, g, b, 255 };
}

color_t color_create_rgba(unsigned char r, unsigned char g, 
                          unsigned char b, unsigned char a)
{
    return (color_t){ r, g, b, a };
}