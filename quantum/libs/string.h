/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */
 
#ifndef __QUANTUM_STRING_H
#define __QUANTUM_STRING_H

#include <quantum/libs/stdlib.h>

size_t strlen(const char* s);

char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);

char* memcpy(void* dest, const void* src, size_t n);

char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t n);

int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);

void* memset(void* ptr, int c, size_t n);
void* memset16(unsigned short* ptr, unsigned short c, size_t n);
void* memset32(unsigned int* ptr, unsigned int c, size_t n);
void* memset64(unsigned long* ptr, unsigned long c, size_t n);

#endif /* __QUANTUM_STRING_H */