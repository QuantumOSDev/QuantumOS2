/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/clib/stdlib.h>
#include <quantum/clib/string.h>

size_t strlen(const char* s)
{
	size_t i = 0;

	while (s[i] != '\0')
		i++;

	return i;
}

char* strcpy(char* dest, const char* src)
{
	char* tmp_dest = dest;

	while (*dest != '\0')
		*dest++ = *src++;

	return tmp_dest;
}

char* strncpy(char* dest, const char* src, size_t n)
{
	char* tmp_dest = dest;

	for (int i = 0; i < n; i++)
	{
		if (*tmp_dest != 0)
			*tmp_dest = *src++;

		tmp_dest++;
	}

	return tmp_dest;
}

char* memcpy(void* dest, const void* src, size_t n)
{
	char *tmp_dest = dest;
	const char *s  = src;

	while (n--)
		*tmp_dest++ = *s++;

	return dest;
}

char* strcat(char* dest, const char* src)
{
	char *tmp_dest = dest;

	while (*dest)
		dest++;

	while (*dest != '\0')
		*dest++ = *src++;

	return tmp_dest;
}

char* strncat(char* dest, const char* src, size_t n)
{
	char *tmp_dest = dest;

	if (n)
	{
		while (*dest)
			dest++;

		while ((*dest++ = *src++) != 0)
		{
			if (--n == 0)
			{
				*dest = '\0';
				break;
			}
		}
	}

	return tmp_dest;
}

int strcmp(const char* s1, const char* s2)
{
	char c1, c2;

	while (1)
	{
		c1 = *s1++;
		c2 = *s2++;

		if (c1 != c2)
			return c1 < c2 ? -1 : 1;

		if (!c1)
			break;
	}

	return 0;
}

int strncmp(const char* s1, const char* s2, size_t n)
{
	char c1, c2;

	for (int i = 0; i < n; i++)
	{
		c1 = *s1++;
		c2 = *s2++;

		if (c1 != c2)
			return c1 < c2 ? -1 : 1;

		if (!c1)
			break;
	}

	return 0;
}

void* memset(void* ptr, int c, size_t n)
{
	char* tmp_ptr = ptr;

	for (int i = 0; i < n; i++)
		tmp_ptr[i] = c;

	return tmp_ptr;
}

void* memset16(unsigned short* ptr, unsigned short c, size_t n)
{
	unsigned short* tmp_ptr = ptr;

	for (int i = 0; i < n; i++)
		ptr[i] = c;

	return tmp_ptr;
}

void* memset32(unsigned int* ptr, unsigned int c, size_t n)
{
	unsigned int* tmp_ptr = ptr;

	for (int i = 0; i < n; i++)
		ptr[i] = c;

	return tmp_ptr;
}

void* memset64(unsigned long* ptr, unsigned long c, size_t n)
{
	unsigned long* tmp_ptr = ptr;

	for (int i = 0; i < n; i++)
		ptr[i] = c;

	return tmp_ptr;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
	size_t ofs = 0;
	int c1 = 0;

	while (ofs < n && !(c1 = ((unsigned char *)s1)[ofs] - ((unsigned char *)s2)[ofs]))
		ofs++;

	return c1;
}

static char* strtok_last_token_end = 0;
char* strtok(char* str, const char* delimiters) {
	char* begin;
	if(strtok_last_token_end && !str)
		begin = strtok_last_token_end+1;
	else
		begin = str;

	// Determine token begin
	while(*begin) {
		const char* delimiter = delimiters;
		unsigned char found = 0;
		while(*delimiter) {
			if(*begin!=*delimiter) {
				found = 1;
				break;
			}
			delimiter++;
		}
		if(found)
			break;
		begin++;
	}

	if(!(*begin)) {
		strtok_last_token_end = 0;
		return 0;
	}

	char* end = begin;
	while(*end) {
		const char* delimiter = delimiters;
		unsigned char found = 0;
		while(*delimiter) {
			if(*end==*delimiter) {
				found = 1;
				break;
			}
			delimiter++;
		}
		if(found) {
			*end = 0;
			break;
		}
		end++;
	}
	strtok_last_token_end = end;

	return begin;
}
