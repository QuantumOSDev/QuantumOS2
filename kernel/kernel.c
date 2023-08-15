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

#include <quantum/mm/kmmap.h>
#include <quantum/mm/heap.h>

void kernel_init(unsigned long magic, unsigned long addr)
{
    multiboot_info_t* mb_info = (multiboot_info_t*)addr;

    vesa_graphics_t* vesa_graphics = vesa_init(mb_info);

    print_init(color_create_rgb(250, 250, 250), color_create_rgb(0, 0, 0));
    
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

    kernel_memory_map_t kmmap;
    kmmap_get_kernel_memory_map(&kmmap, mb_info);

    heap_init(&kmmap);

    pit_init();
    keyboard_init();

    heap_allocate(3218903040);
    pit_sleep(1030);
    heap_allocate(0);
    pit_sleep(3030);
    heap_allocate(1024);

    // success_printf("kernel_init", "loading kernel console..");

    // pit_sleep(1000);

    // vesa_clear();
    // print_t* print_struct = get_print_structure();
    // print_struct->x = 10;
    // print_struct->y = 10;

    // while (1)
    // {
    //     printf("quauntumOS@root ~ ");

    //     char keysc;
    //     int  char_len;

    //     while ((keysc = keyboard_getchar()) != '\n')
    //     {
    //         if (keysc == '\b')
    //         {
    //             if (char_len != 0)
    //             {
    //                 insert_backspace();
    //                 char_len--;
    //             }

    //             continue;
    //         }

    //         printf("%c", keysc);
    //         char_len++;
    //     }

    //     printf("\n");
    // }
}
