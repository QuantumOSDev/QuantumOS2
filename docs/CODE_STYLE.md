# Code Style of QuantumOS project
In QuantumOS repository we use exactly the same code style so every file looks similar. Code style is defined below:

### Functions
So functions name in quantum are named like this:
```
<codebase>_<desc>
```
- *\<codebase>* - For example if we are writing code for usb drivers we just write **usb** at the beginning of function name.
- *\<desc>* - When our function initializes something we just write **init** or when it does print signature of something we write **print_signature**. 

Syntax of function look like this:
```c
/* Optional comment about this function */

void kernel_entry()
{
    return;
}
```
### Variables
Variables name are using **snake_case** exactly the same like functions but without the *\<codebase>* thing here is a simple example of defining variables
```c
char* example_variable_name = "Example string";
```
```c
multiboot_info_t mb_info = (multiboot_info_t*)addr;
```
```c
char** args = (char**)kcalloc(4, sizeof(char*));
```
**When value type isn't the same as variable type we should cast it always!**
### Headers
```c
/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Your name <youremail@email.com>
 */

#ifndef __QUANTUM_<FILENAME_IN_UPPER_SNAKE_CASE>_H
#define __QUANTUM_<FILENAME_IN_UPPER_SNAKE_CASE>_H

#include <quantum/core.h>

/* Definitions */

#endif /* __QUANTUM_<FILENAME_IN_UPPER_SNAKE_CASE>_H */
```
Here's an example of it for Kernel entry header create and edited by Solindek (for example)

```c
/*
 * QuantumOS Copyright (c) 2022-2023
 *  - SolindekDev <solindeklive.biznes@gmail.com>
 */

#ifndef __QUANTUM_KERNEL_ENTRY_H
#define __QUANTUM_KERNEL_ENTRY_H

#include <quantum/core.h>

void kernel_entry(unsigned long magic, unsigned long addr);

#endif /* __QUANTUM_KERNEL_ENTRY_H */
```
### If, While, For Statements 
When we have a if, while or for statement and inside of it there is only one expression we don't need to use brackets (**{}**), here's a example of it:
```c
void func()
{
    int a = 1;
    int b = 1;

    if (a == b)
        printf("a equals b");
}
```
Of course if we have couple of expressions inside we use the same brackets style as in function:
```c
void func()
{
    int a = 1;
    int b = 1;

    if (a == b)
    {
        printf("a equals b");
        b = 2;
    }
}
```