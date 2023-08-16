/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/multiboot.h>

#include <quantum/clib/stdio.h>
#include <quantum/clib/stdlib.h>

#include <quantum/mm/kmmap.h>
#include <quantum/mm/heap.h>

int kmmap_get_kernel_memory_map(kernel_memory_map_t* map, multiboot_info_t* mb)
{
    if (map == NULL) return -1;

    map->kernel.kernel_start =  (unsigned int)&kernel_section_start;
    map->kernel.kernel_end   =  (unsigned int)&kernel_section_end;
    map->kernel.kernel_len   = ((unsigned int)&kernel_section_end - (unsigned int)&kernel_section_start);
    map->kernel.text_start   =  (unsigned int)&kernel_text_section_start;
    map->kernel.text_end     =  (unsigned int)&kernel_text_section_end;
    map->kernel.text_len     = ((unsigned int)&kernel_text_section_end - (unsigned int)&kernel_text_section_start);
    map->kernel.data_start   =  (unsigned int)&kernel_data_section_start;
    map->kernel.data_end     =  (unsigned int)&kernel_data_section_end;
    map->kernel.data_len     = ((unsigned int)&kernel_data_section_end - (unsigned int)&kernel_data_section_start);
    map->kernel.rodata_start =  (unsigned int)&kernel_rodata_section_start;
    map->kernel.rodata_end   =  (unsigned int)&kernel_rodata_section_end;
    map->kernel.rodata_len   = ((unsigned int)&kernel_rodata_section_end - (unsigned int)&kernel_rodata_section_start);
    map->kernel.bss_start    =  (unsigned int)&kernel_bss_section_start;
    map->kernel.bss_end      =  (unsigned int)&kernel_bss_section_end;
    map->kernel.bss_len      = ((unsigned int)&kernel_bss_section_end - (unsigned int)&kernel_bss_section_start);

    map->system.total_memory = mb->mem_lower + mb->mem_upper;

    for (int  i = 0; i < mb->mmap_length; i += sizeof(multiboot_memory_map_edited_t))
    {
        multiboot_memory_map_edited_t* mmap = ((multiboot_memory_map_edited_t*)(mb->mmap_addr + i));

        if (mmap->type != MULTIBOOT_MEMORY_AVAILABLE) continue;

        if (mmap->addr_low == map->kernel.text_start)
        {
            map->available.start = map->kernel.kernel_end + 1024 * 1024;
            map->available.end   = mmap->addr_low + mmap->len_low;
            map->available.size  = map->available.end - map->available.start;

            return 0;
        }
    }

    return -1;
}