/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */
 
#ifndef __QUANTUM_STDLIB_H
#define __QUANTUM_STDLIB_H

#ifndef __SIZE_T
# define __SIZE_T
# define size_t unsigned long int
#endif /* __SIZE_T */

#ifndef NULL
# define NULL ((void*)0)
#endif /* NULL */

int atoi(const char* str);
long atol(const char* str);

char* itoa(int value, char* str, int base);
char* ltoa(long value, char* str, int base);
char* ultoa(unsigned long value, char* str, int base);

void* realloc(void* ptr, size_t size);
void* malloc (size_t size);
void* calloc (size_t nmemb, size_t size);

void free(void* ptr);

#endif /* __QUANTUM_STDLIB_H */