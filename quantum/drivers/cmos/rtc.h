/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#ifndef __QUANTUMN_RTC_H
#define __QUANTUMN_RTC_H

typedef struct __rtc_time_t {
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day;
    unsigned char month;
    unsigned int  year;
} rtc_time_t;

rtc_time_t rtc_get_current_time();

rtc_time_t rtc_create_time(unsigned char second, unsigned char minute, unsigned char hour, 
                           unsigned char day,    unsigned char month,  unsigned int  year);

#endif /* __QUANTUMN_RTC_H */