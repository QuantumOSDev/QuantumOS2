/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */
 
#include <quantum/libs/stdio.h>
#include <quantum/libs/stdarg.h>
#include <quantum/libs/string.h>

#include <quantum/graphics/print.h>

#include <quantum/arch/x86_64/pit.h>

static char __global_printf_buf[1024 * 5];

void printf(const char* fmt, ...)
{    
	va_list list;
    va_start(list, fmt);

    memset(__global_printf_buf, 0, 1024 * 5);
    svprintf(__global_printf_buf, fmt, list);
	print(__global_printf_buf);

    va_end(list);
}

void
success_printf(char* func_name, const char* fmt, ...)
{
	print_t* print_struct = get_print_structure();
	print_struct->fg = color_create_rgb(48, 199, 93);
	printf("%s", func_name);
	print_struct->fg = color_create_rgb(250, 250, 250);
	printf(": ");

	va_list list;
    va_start(list, fmt);

    memset(__global_printf_buf, 0, 1024 * 5);
    svprintf(__global_printf_buf, fmt, list);
	print(__global_printf_buf);

    va_end(list);
}

void sprintf(char* buf, const char* fmt, ...)
{
    va_list list;
    va_start(list, fmt);

    svprintf(buf, fmt, list);

    va_end(list);
}

void svprintf(char* buffer, const char* fmt, va_list arg)
{
    char buf[64];

	int           __i32 = 0;
	long 	      __i64 = 0;
	unsigned long __u32 = 0;
	char*         __str = NULL;
	int 	      __base = 10;

	for (int i = 0; i < strlen((char*)fmt); i++) 
	{
		char curr_char = fmt[i];
		if (curr_char == '%')
		{
			i++;
			curr_char = fmt[i];

			if (curr_char == '\0') break;

			switch (curr_char) 
			{
				case 'd':
				case 'D':
				case 'i':
				case 'I':
					__i32 = va_arg(arg, int);
					itoa(__i32, buf, 10);
					for (int j = 0; j < strlen(buf); j++)
						*buffer++ = buf[j];
					memset(buf, 0, 64);
					break;
				case 'u':
				case 'U':
					__u32 = va_arg(arg, unsigned long);

					i++;
					curr_char = fmt[i];
					if (curr_char == 'x')
						__base = 16;
					else if (curr_char == 'o')
						__base = 8;
					else if (curr_char == 'b')
						__base = 2;

					ultoa(__u32, buf, __base);
					__base = 10;
					for (int j = 0; j < strlen(buf); j++)
						*buffer++ = buf[j];
					memset(buf, 0, 64);
					break;
				case 'o':
				case 'O':
					__i32 = va_arg(arg, int);
					itoa(__i32, buf, 8);
					for (int j = 0; j < strlen(buf); j++)
						*buffer++ = buf[j];
					memset(buf, 0, 64);
					break;
				case 'x':
				case 'X':
					__i32 = va_arg(arg, int);
					itoa(__i32, buf, 16);
					for (int j = 0; j < strlen(buf); j++)
						*buffer++ = buf[j];
					memset(buf, 0, 64);
					break;
				case 'b':
				case 'B':
					__i32 = va_arg(arg, int);
					itoa(__i32, buf, 2);
					for (int j = 0; j < strlen(buf); j++)
						*buffer++ = buf[j];
					memset(buf, 0, 64);
					break;
				case 'c':
				case 'C':
					__i32 = va_arg(arg, int);
					*buffer++ = (char)__i32;
					break;
				case 's':
				case 'S':
					__str = va_arg(arg, char*);
					for (int j = 0; j < strlen(__str); j++)
						*buffer++ = __str[j];
					break;
				case 'f':
				case 'F':
					// TODO: float to string
					break;
				case 'l':
				case 'L':
					__i64 = va_arg(arg, long);
					ultoa(__i64, buf, 10);
					for (int j = 0; j < strlen(buf); j++)
						*buffer++ = buf[j];
					memset(buf, 0, 64);
					break;
				case '%':
					*buffer++ = curr_char;
					break;
				case 'p':
				case 'P':
					// Convert pointer to string address in hexadecimal
					break;
				default:
					*buffer++ = curr_char;
					break;
			}
			// Format
		} 
		else
		{
			*buffer++ = curr_char;
		}
	}

	*buffer++ = '\0';
}