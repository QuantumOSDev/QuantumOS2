/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/drivers/cmos/cmos.h>
#include <quantum/drivers/cmos/rtc.h>

#include <quantum/drivers/pio.h>

unsigned char cmos_get_update_in_progress_flag()
{
    outb(CMOS_ADDRESS, 0x0A);

    return (inb(CMOS_DATA) & 0x80);
}

unsigned char cmos_get_register(int _register)
{
    outb(CMOS_ADDRESS, _register);

    return (inb(CMOS_DATA));
}