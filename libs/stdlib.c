/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */
 
#include <quantum/libs/stdlib.h>
#include <quantum/libs/string.h>
#include <quantum/libs/stdio.h>

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
        if (digit < 10) {
            *--pos = (char)('0' + digit);
        } else {
            *--pos = (char)('a' + digit - 10);
        } 
    } while (value != 0L);

    memcpy(str, pos, &buffer[32] - pos + 1);
    return str;
}

void* malloc(size_t size)
{
    info_printf("malloc", "trying to allocate %u bytes inside of kernel", size);
    return heap_allocate(size);
}

void* calloc(size_t nmemb, size_t size)
{
    size_t final_size = (size_t)(nmemb * size);

    info_printf("calloc", "trying to allocate %u bytes inside of kernel", final_size);
    void* ptr = heap_allocate(final_size);
    memset(ptr, 0, final_size);

    return ptr;
}