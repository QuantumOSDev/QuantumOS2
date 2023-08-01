/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#ifndef __QUANTUMN_CMOS_H
#define __QUANTUMN_CMOS_H

#define CMOS_ADDRESS 0x70
#define CMOS_DATA    0x71

unsigned char cmos_get_update_in_progress_flag();

unsigned char cmos_get_register(int _register);

#endif /* __QUANTUMN_CMOS_H */