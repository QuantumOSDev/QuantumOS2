/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/kernel.h>
#include <quantum/panic.h>

#include <quantum/graphics/color.h>
#include <quantum/graphics/unifont.h>
#include <quantum/graphics/print.h>
#include <quantum/graphics/vesa.h>

#include <quantum/libs/stdio.h>
#include <quantum/libs/string.h>
#include <quantum/libs/stdlib.h>

#include <quantum/arch/x86_64/pit.h>

static char __global_panic_buf[1024 * 5];

void kernel_panic(char* func, const char* message, ...)
{
    vesa_clear();
    print_t* print_struct = get_print_structure();
    print_struct->x = 10;
    print_struct->y = 10;
    
    print_struct->fg = color_create_rgb(255, 0, 0);

	va_list list;
    va_start(list, message);

    memset(__global_panic_buf, 0, 1024 * 5);
    svprintf(__global_panic_buf, message, list);

    va_end(list);

    printf("         .AMMMMMMMMMMA.\n");
    printf("       .AV. :::.:.:.::MA.\n");
    printf("      A' :..        : .:`A\n");
    printf("     A'..              . `A.\n");
    printf("    A' :.    :::::::::  : :`A\n");
    printf("    M  .    :::.:.:.:::  . .M\n");
    printf("    M  :   ::.:.....::.:   .M\n");
    printf("    V : :.::.:........:.:  :V\n");
    printf("   A  A:    ..:...:...:.   A A\n");
    printf("  .V  MA:.....:M.::.::. .:AM.M\n");
    printf(" A'  .VMMMMMMMMM:.:AMMMMMMMV: A\n");
    printf(":M .  .`VMMMMMMV.:A `VMMMMV .:M:\n");
    printf(" V.:.  ..`VMMMV.:AM..`VMV' .: V\n");
    printf("  V.  .:. .....:AMMA. . .:. .V\n");
    printf("   VMM...: ...:.MMMM.: .: MMV\n");
    printf("       `VM: . ..M.:M..:::M'\n");
    printf("         `M::. .:.... .::M\n");
    printf("          M:.  :. .... ..M\n");
    printf("          V:  M:. M. :M .V\n");
    printf("          `V.:M.. M. :M.V'\n\n");

    printf("KERNEL PANIC, WHAT HAVE YOU DONE!?\n");
    print_struct->fg = color_create_rgb(255, 255, 255);
    printf("[%d] %s: %s\n", pit_get_ticks(), func, __global_panic_buf);
    asm volatile("hlt");
}