/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */
 
#ifndef __QUANTUM__STDIO_H
#define __QUANTUM__STDIO_H

#include <quantum/libs/stdarg.h>

void printf(const char* fmt, ...);

void sprintf(char* buf, const char* fmt, ...);

void svprintf(char* buffer, const char* fmt, va_list arg);

void success_printf(char* func_name, const char* fmt, ...);

#endif /* __QUANTUM__STDIO_H */