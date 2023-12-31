/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#ifndef __QUANTUM_VGA_DRIVER_H
#define __QUANTUM_VGA_DRIVER_H

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define SCREEN_SIZE_COLS 80
#define SCREEN_SIZE_ROWS 25
#define SCREEN_SIZE_BLEN 2

#define SCREEN_SIZE (SCREEN_SIZE_COLS * SCREEN_SIZE_ROWS * SCREEN_SIZE_BLEN)

#define ATTR_BYTE_BLK_ON_BLK 0x00   /* BLACK ON BLACK */
#define ATTR_BYTE_BLU_ON_BLK 0x01   /* BLUE ON BLACK */
#define ATTR_BYTE_GRN_ON_BLK 0x02   /* GREEN ON BLACK */
#define ATTR_BYTE_AQU_ON_BLK 0x03   /* AQUA ON BLACK */
#define ATTR_BYTE_RED_ON_BLK 0x04   /* RED ON BLACK */
#define ATTR_BYTE_MAG_ON_BLK 0x05   /* MAGENTA ON BLACK */
#define ATTR_BYTE_ORG_ON_BLK 0x06   /* ORANGE ON BLACK */
#define ATTR_BYTE_LGR_ON_BLK 0x07   /* L-GRAY ON BLACK */
#define ATTR_BYTE_DGR_ON_BLK 0x08   /* D-GRAY ON BLACK */
#define ATTR_BYTE_DBL_ON_BLK 0x09   /* DODGER-BLUE ON BLACK */

#define ATTR_BYTE_TEXT_COLOR_STD ATTR_BYTE_LGR_ON_BLK

static char *VGA_POINTER = (char*)0xb8000;

static int VGA_CURSOR = 0;

void vga_newline(void);
void vga_tab(void);
void vga_putchar(char c, unsigned short cl);
void vga_clear(void);
void vga_backspace(void);

#endif /* __QUANTUM_VGA_DRIVER_H */