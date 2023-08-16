/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */
 
#include <quantum/clib/stdlib.h>
#include <quantum/clib/string.h>
#include <quantum/clib/stdio.h>

#include <quantum/mm/heap.h>

int atoi(const char* str)
{
    int result = 0;
    int i = 0;
    if (*str == '-')
        i++;
    for (; str[i]; i++)
        result = result * 10 + str[i] - '0';
    if (*str == '-') 
        result /= -1;
    return result;
}

long atol(const char* str)
{
    long result = 0;
    for (int i = 0; str[i]; i++)
        result = result * 10 + str[i] - '0';
    return result;
}

char* itoa(int value, char* str, int base)
{
    int i = 0;
    int is_neg = 0;
    
    if (value == 0) 
    { 
        str[i++] = '0';
        str[i] = '\0';
        return NULL;
    } 

    if (value < 0 && base == 10) 
    {
        is_neg = 1;
        value = -value;
    }

    while (value != 0) 
    {
        int rem = value % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value = value / base;
    }
    if (is_neg)
        str[i++] = '-';
    str[i] = '\0';

    i = 0;
    int c, j;
    for (i = 0, j = strlen(str)-1; i < j; i++, j--) 
    {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }

    return str;
}

char* ltoa(long value, char* str, int base)
{
    long val;
    int is_neg;
    char buffer[33];
    char *pos;
    int digit;

    if (value < 0 && base == 10) 
    {
        is_neg = 1;
        val = -value;
    } 
    else 
    {
        is_neg = 0;
        val = value;
    }

    pos = &buffer[32];
    *pos = '\0';

    do 
    {
        digit = val % base;
        val = val / base;
        if (digit < 10)
            *--pos = (char)('0' + digit);
        else
            *--pos = (char)('a' + digit - 10);
    } while (val != 0L);

    if (is_neg) 
        *--pos = '-';

    memcpy(str, pos, &buffer[32] - pos + 1);
    return str;
}

char* ultoa(unsigned long value, char* str, int base)
{
    char buffer[33];
    char *pos;
    int digit;

    pos = &buffer[32];
    *pos = '\0';

    do 
    {
        digit = value % base;
        value = value / base;
        if (digit < 10) 
            *--pos = (char)('0' + digit);
        else 
            *--pos = (char)('a' + digit - 10);
    } while (value != 0L);

    memcpy(str, pos, &buffer[32] - pos + 1);
    return str;
}

void* malloc(size_t size)
{
    info_printf("malloc", "trying to allocate %u bytes inside of kernel\n", size);
    return heap_allocate(size);
}

void* calloc(size_t nmemb, size_t size)
{
    size_t final_size = (size_t)(nmemb * size);

    info_printf("calloc", "trying to allocate %u bytes inside of kernel\n", final_size);
    void* ptr = heap_allocate(final_size);
    memset(ptr, 0, final_size);

    return ptr;
}

void free(void* ptr)
{
    info_printf("free", "trying to unallocate ptr at 0x%ux\n", ptr);
    heap_free(ptr);
}

void* realloc(void* ptr, size_t size)
{
    /* If ptr is NULL then realloc is working like malloc */
    if (ptr == NULL)
    {
        error_printf("realloc", "ptr=NULL, realloc is now working like malloc\n");
        return malloc(size);
    }

    /* If size is 0 then realloc is working like free */
    if (size == 0) 
    {
        error_printf("realloc", "size=0, realloc is now working like free\n");
        free(ptr);
        return NULL;
    }

    void* new_ptr = malloc(size);

    if (new_ptr == NULL) 
    {
        error_printf("realloc", "couldn't allocate new_ptr\n");
        return NULL;
    }

    heap_page_t* page = heap_get_page_by_addr(ptr);

    if (page == NULL)
    {
        error_printf("heap_free", "didn't find page at 0x%ux\n", ptr);
        return NULL;
    }

    size_t old_size = page->size;
    size_t copy_size = (old_size < size) ? old_size : size;
    memcpy(new_ptr, ptr, copy_size);

    free(ptr);
    return new_ptr;
}