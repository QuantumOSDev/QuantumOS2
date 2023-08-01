/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <quantum/kernel.h>

#include <quantum/drivers/vga.h>
#include <quantum/drivers/pio.h>

void kernel_init(unsigned long magic, unsigned long addr)
{
    vga_putchar('e', ATTR_BYTE_GRN_ON_BLK);

    for (;;)
    {
        
    }
}