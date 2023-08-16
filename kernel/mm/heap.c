/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <quantum/mm/kmmap.h>
#include <quantum/mm/heap.h>

#include <quantum/clib/stdlib.h>
#include <quantum/clib/string.h>
#include <quantum/clib/stdio.h>

#ifndef MAGIC_HEAP_PAGE
# define MAGIC_HEAP_PAGE { 0x50, 0x3D, 0xFF, 'Q', 'T', 'O', 'S', 0x5F }
#endif /* MAGIC_HEAP_PAGE */

static heap_t* heap_allocator;

void heap_init(kernel_memory_map_t* kmmap)
{
    void* memory_start = (void*)kmmap->available.start;

    heap_allocator             = memory_start;
    heap_allocator->mem_used   = sizeof(heap_t) + sizeof(heap_page_t);
    heap_allocator->page_len   = 1;
    heap_allocator->kmmap      = kmmap;
    heap_allocator->first_page = memory_start + sizeof(heap_t);

    heap_page_t* first_page = heap_allocator->first_page;
    first_page->size  = 0;
    first_page->ptr   = heap_allocator->first_page + sizeof(heap_page_t);
    first_page->next  = NULL;
    first_page->last  = NULL;

    memcpy(first_page->magic, (unsigned char[8])MAGIC_HEAP_PAGE, sizeof(unsigned char) * 8);
    info_printf("heap_init", "allocated psychical memory heap that is capable of allocating %u bytes\n", heap_allocator->kmmap->available.size);
}

heap_page_t* heap_get_empty_page()
{
    heap_page_t* curr_page = heap_allocator->first_page;

    while (curr_page->next != NULL)
        curr_page = curr_page->next;

    return curr_page;
}

void* heap_allocate(size_t size) 
{
    if ((heap_allocator->mem_used + size) > heap_allocator->kmmap->available.size)
    {
        error_printf("heap_allocate", "not enough memory to allocate %u %u bytes\n", size);
        return NULL;   
    }

    if (size == 0)
    {
        error_printf("heap_allocate", "invalid amount of bytes to allocate\n", size);
        return NULL;   
    }

    heap_page_t* parent_page = heap_get_empty_page();
    heap_page_t* child_page  = parent_page->next;

    child_page       = (heap_page_t*)(heap_allocator->kmmap->available.start + heap_allocator->mem_used);
    child_page->size = size;
    child_page->ptr  = (void*)heap_allocator->kmmap->available.start + heap_allocator->mem_used + size + sizeof(heap_page_t);
    child_page->next = NULL;
    child_page->last = parent_page;

    memcpy(child_page->magic, (unsigned char[8])MAGIC_HEAP_PAGE, sizeof(unsigned char) * 8);

    heap_allocator->mem_used += (size + sizeof(heap_page_t));
    heap_allocator->page_len += 1;
    
    info_printf("heap_allocate", "allocated new heap page with size of %u bytes at 0x%ux\n",
                size, (unsigned long)child_page->ptr);
    info_printf("heap_allocate", "heap_allocator->mem_used=%u, heap_allocator->page_len=%u\n", 
                heap_allocator->mem_used, heap_allocator->page_len);

    return (void*)child_page->ptr;
}

heap_page_t* heap_get_page_by_addr(void* ptr)
{
    if (ptr < (void*)heap_allocator->kmmap->available.start)
    {
        error_printf("heap_get_page_by_addr", "given ptr 0x%xu is not correct\n", ptr);
        return NULL;
    }

    unsigned char magic[8] = MAGIC_HEAP_PAGE;
    heap_page_t* page      = (heap_page_t*)ptr; 

    if (memcmp((const void*)magic, (const void*)page, sizeof(heap_page_t)) == 0)
    {
        error_printf("heap_get_page_by_addr", "at the given ptr 0x%ux magic code is not correct\n", ptr);
        return NULL;
    }

    info_printf("heap_get_page_by_addr", "found heap page at 0x%ux\n", ptr);
    return page;
}

void heap_free(void* ptr)
{
    heap_page_t* free_page = heap_get_page_by_addr(ptr - sizeof(heap_page_t));

    if (free_page == NULL)
    {
        error_printf("heap_free", "didn't find page at 0x%ux\n", ptr);
        return;
    }

    info_printf("heap_free", "unallocating memory at 0x%ux of size %u\n", ptr, free_page->size);

    heap_allocator->mem_used -= free_page->size;
    heap_allocator->page_len -= 1;

    memset(free_page, 0, sizeof(heap_page_t));
    return;
}