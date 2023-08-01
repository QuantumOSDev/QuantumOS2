/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/drivers/cmos/cmos.h>
#include <quantum/drivers/cmos/rtc.h>

void rtc_no_update_in_progress()
{
    while (cmos_get_update_in_progress_flag())
        ;
}

rtc_time_t rtc_get_current_time()
{
    rtc_no_update_in_progress();

    rtc_time_t last_time = rtc_create_time(0, 0, 0, 0, 0, 0);
    rtc_time_t rtc_time  = rtc_create_time(cmos_get_register(0x00), 
                                           cmos_get_register(0x02),
                                           cmos_get_register(0x04),
                                           cmos_get_register(0x07),
                                           cmos_get_register(0x08),
                                           cmos_get_register(0x09));

    // TODO: add century register from ACPI    

    do 
    {
        last_time = rtc_time;

        rtc_no_update_in_progress();

        rtc_time.second = cmos_get_register(0x00);
        rtc_time.minute = cmos_get_register(0x02);
        rtc_time.hour   = cmos_get_register(0x04);
        rtc_time.day    = cmos_get_register(0x07);
        rtc_time.month  = cmos_get_register(0x08);
        rtc_time.year   = cmos_get_register(0x09);
    } 
    while ((last_time.second != rtc_time.second)  || 
           (last_time.minute != rtc_time.minute)  || 
           (last_time.hour   != rtc_time.hour  )  ||
           (last_time.day    != rtc_time.day   )  || 
           (last_time.month  != rtc_time.month )  || 
           (last_time.year   != rtc_time.year  ));

 
    if (!(cmos_get_register(0x0B) & 0x04)) 
    {
        rtc_time.second = (rtc_time.second & 0x0F) + ((rtc_time.second / 16) * 10);
        rtc_time.minute = (rtc_time.minute & 0x0F) + ((rtc_time.minute / 16) * 10);
        rtc_time.hour   = ((rtc_time.hour & 0x0F) + (((rtc_time.hour & 0x70) / 16) * 10) ) | (rtc_time.hour & 0x80);
        rtc_time.day    = (rtc_time.day & 0x0F) + ((rtc_time.day / 16) * 10);
        rtc_time.month  = (rtc_time.month & 0x0F) + ((rtc_time.month / 16) * 10);
        rtc_time.year   = (rtc_time.year & 0x0F) + ((rtc_time.year / 16) * 10);
    }

    return rtc_time;
}

rtc_time_t rtc_create_time(unsigned char second, unsigned char minute, unsigned char hour, 
                           unsigned char day,    unsigned char month,  unsigned int  year)
{
    return (rtc_time_t){ second, minute, hour, day, month, year };
}