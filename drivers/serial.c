/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <quantum/drivers/serial.h>
#include <quantum/drivers/pio.h>

#include <quantum/libs/stdlib.h>
#include <quantum/libs/string.h>
#include <quantum/libs/stdio.h>

#ifndef SERIAL_PORT
# define SERIAL_PORT 0x3F8
#endif /* SERIAL_PORT */

static char __global_serial_printf_buf[1024 * 5];

void serial_putc(char c)
{
    outb(SERIAL_PORT, c);
}

void serial_print(char* str)
{
    for (int i = 0; i < strlen(str); i++)
        serial_putc(str[i]);
}

void serial_printf(const char* fmt, ...)
{
    va_list list;
    va_start(list, fmt);

    memset(__global_serial_printf_buf, 0, 1024 * 5);
    svprintf(__global_serial_printf_buf, fmt, list);
	serial_print(__global_serial_printf_buf);

    va_end(list);
}
