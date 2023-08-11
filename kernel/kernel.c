/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <quantum/kernel.h>
#include <quantum/multiboot.h>

#include <quantum/graphics/color.h>
#include <quantum/graphics/unifont.h>
#include <quantum/graphics/print.h>
#include <quantum/graphics/vesa.h>

#include <quantum/drivers/cmos/rtc.h>
#include <quantum/drivers/vga.h>
#include <quantum/drivers/pio.h>

#include <quantum/libs/stdlib.h>
#include <quantum/libs/stdio.h>

#include <quantum/arch/x86_64/gdt.h>
#include <quantum/arch/x86_64/idt.h>
#include <quantum/arch/x86_64/isr.h>
#include <quantum/arch/x86_64/pic.h>
#include <quantum/arch/x86_64/pit.h>

void kernel_init(unsigned long magic, unsigned long addr)
{
    multiboot_info_t* mb_info = (multiboot_info_t*)addr;

    vesa_graphics_t* vesa_graphics = vesa_init(mb_info);

    print_init(color_create_rgb(250, 250, 250), color_create_rgb(24, 25, 26));
    
    success_printf("kernel_init", "Hello, World!\n");
    success_printf("kernel_init", "Quantum OS booted successfully with \"%s\" into %dx%d graphics\n", 
        (const char*)((void*)mb_info->boot_loader_name), vesa_graphics->width, vesa_graphics->height);

    rtc_time_t time = rtc_get_current_time();
    success_printf("rtc_get_current_time", "(UTC) %s%d:%s%d:%s%d %d%d:%s%d:%s%d\n",
        time.hour <= 9 ? "0" : "",
        time.hour, time.minute <= 9 ? "0" : "", 
        time.minute, time.second <= 9 ? "0" : "", 
        time.second, 
        20,
        time.year, 
        time.month <= 9 ? "0" : "", time.month, 
        time.day <= 9 ? "0" : "", time.day);
    
    gdt_init();
    idt_init();
    pit_init();

    printf("We would wait 100ms\n");
    pit_sleep(100);
    printf("After 100ms, wait 1s\n");
    pit_sleep(1000);
    printf("After 1s, wait 10s\n");
    pit_sleep(10000);
    printf("done\n");
}