/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/kernel.h>
#include <quantum/panic.h>

#include <quantum/graphics/color.h>
#include <quantum/graphics/unifont.h>
#include <quantum/graphics/print.h>
#include <quantum/graphics/vesa.h>

#include <quantum/libs/stdio.h>
#include <quantum/libs/string.h>
#include <quantum/libs/stdlib.h>
#include <quantum/libs/stdbool.h>

#include <quantum/arch/x86_64/isr.h>
#include <quantum/arch/x86_64/pit.h>

#include <quantum/drivers/keyboard.h>
#include <quantum/drivers/pio.h>

static char keyboard_map[128] = KEYBOARD_MAP_ENG;

static bool keyboard_shift_pressed = false;
static bool keyboard_caps_lock     = false;

static char keyboard_current_char  = 0;
static char keyboard_current_sc    = 0;

void keyboard_set_key_map(char* key_map)
{
    memcpy(keyboard_map, key_map, 128);
}

char* keyboard_get_key_map()
{
    return keyboard_map;
}

char keyboard_alternate(char ch)
{
    switch (ch)
    {
        case '`' :  return '~' ;
        case '1' :  return '!' ;
        case '2' :  return '@' ;
        case '3' :  return '#' ;
        case '4' :  return '$' ;
        case '5' :  return '%' ;
        case '6' :  return '^' ;
        case '7' :  return '&' ;
        case '8' :  return '*' ;
        case '9' :  return '(' ;
        case '0' :  return ')' ;
        case '-' :  return '_' ;
        case '=' :  return '+' ;
        case '[' :  return '{' ;
        case ']' :  return '}' ;
        case '\\':  return '|' ;
        case ';' :  return ':' ;
        case '\'':  return '\"';
        case ',' :  return '<' ;
        case '.' :  return '>' ;
        case '/' :  return '?' ;

        default: return ch;
    }
}

char keyboard_getsc()
{
    int scancode;
    int i;

    for (i = 1000; i > 0; i++)
    {
        if ((inb(KEYBOARD_STATUS_PORT) & KEYBOARD_READY) == 0)
            continue;

        scancode = inb(KEYBOARD_DATA_PORT);
        break;
    }

    if (i > 0)
        return scancode;

    return 0;
}

char keyboard_getchar()
{
    while (keyboard_current_char <= 0)
        ;

    char copy_key_char = keyboard_current_char;
    keyboard_current_char = 0;

    return copy_key_char;
}

char keyboard_getchar_no_wait()
{
    if (!keyboard_current_char <= 0)
        return '\0';

    char copy_key_char = keyboard_current_char;
    keyboard_current_char = 0;

    return copy_key_char;
}

char* keyboard_getchar_until(char c)
{
    // static char result[1024];
    // char input = keyboard_getchar();

    // memset(result, 0, 1024);

    // while (input != c)
    // {
    //     if (input == '\b')
    //     {
    //         if (strlen(result) > 0)
    //         {
    //             printf("\b");
    //             result[strlen(result) - 1] = '\0';

    //             input = keyboard_getchar();
    //             continue;
    //         }

    //         input = keyboard_getchar();
    //         continue;
    //     }

    //     printf("%c", input);
    //     strcat(result, (const char[]){input, 0});
    //     input = keyboard_getchar();
    // }

    // return (char*)result;
}

static void keyboard_handler(registers_t *regs)
{
    keyboard_current_char = 0;
    int scancode = keyboard_getsc();

    if (scancode & KEYBOARD_KEY_UP)
    {
        switch (scancode)
        {
            case -86:
            {
                keyboard_shift_pressed = false;
                break;
            }

            case -74:
            {
                keyboard_shift_pressed = false;
                break;
            }

            default:
            {
                break;
            }
        }
    }
    else
    {
        switch (scancode)
        {
            case KEYBOARD_CAPS_LOCK_SC:
            {
                if (keyboard_caps_lock == false)
                {
                    keyboard_caps_lock = true;
                }
                else
                {
                    keyboard_caps_lock = false;
                }
                break;
            }

            case KEYBOARD_ENTER_SC:
            {
                keyboard_current_char = '\n';
                break;
            }

            case KEYBOARD_TAB_SC:
            {
                keyboard_current_char = '\t';
                break;
            }

            case KEYBOARD_LSHIFT_SC:
            {
                keyboard_shift_pressed = true;
                break;
            }

            case KEYBOARD_RSHIFT_SC:
            {
                keyboard_shift_pressed = true;
                break;
            }

            default:
            {
                keyboard_current_char = keyboard_map[scancode];

                if (keyboard_caps_lock)
                {
                    if (keyboard_shift_pressed)
                    {
                        keyboard_current_char = keyboard_alternate(keyboard_current_char);
                    }
                    else
                    {
                        if (((keyboard_current_char >= 'A') && (keyboard_current_char <= 'Z')) || ((keyboard_current_char >= 'a') && (keyboard_current_char <= 'z')))
                        {
                            keyboard_current_char = keyboard_current_char - 32;
                        }
                        else
                        {
                            keyboard_current_char = keyboard_alternate(keyboard_current_char);
                        }
                    }
                }
                else
                {
                    if (keyboard_shift_pressed)
                    {
                        if (((keyboard_current_char >= 'A') && (keyboard_current_char <= 'Z')) || ((keyboard_current_char >= 'a') && (keyboard_current_char <= 'z')))
                        {
                            keyboard_current_char = keyboard_current_char - 32;
                        }
                        else
                        {
                            keyboard_current_char = keyboard_alternate(keyboard_current_char);
                        }
                    }
                    else
                    {
                        keyboard_current_char = keyboard_map[scancode];
                    }
                }
                break;
            }
        }
    }
}

void keyboard_init()
{
    success_printf("keyboard_init", "successfully initalized keyboard drivers\n");
    isr_register_interrupt_handler(IRQ_BASE + 1, keyboard_handler);
}
