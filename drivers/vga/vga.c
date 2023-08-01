#include <quantum/drivers/vga.h>
#include <quantum/drivers/pio.h>

void vga_set_cursor(int offset)
{
    offset /= 2;

    outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    outb(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
    outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    outb(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

int vga_get_cursor(void)
{
    outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);

    int offset = inb(VGA_DATA_REGISTER) << 8;
    
    outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    
    offset += inb(VGA_DATA_REGISTER);
    
    return offset * 2;
}

int vga_row_from_offset(int offset)
{
    return offset / (2 * SCREEN_SIZE_COLS);
}

int vga_line_from_offset(int col, int row)
{
    return 2 * (row * SCREEN_SIZE_COLS + col);
}


void vga_newline(void)
{
    VGA_CURSOR = vga_line_from_offset(0, vga_row_from_offset(VGA_CURSOR) + 1);
}

void vga_tab(void)
{
    int offset = (SCREEN_SIZE_COLS * SCREEN_SIZE_BLEN) - (VGA_CURSOR % SCREEN_SIZE_COLS);

    if (offset < 4)
    {
        int free = (4 - offset);

        vga_newline();

        VGA_CURSOR = VGA_CURSOR + free;

        return;
    }

    VGA_CURSOR = (VGA_CURSOR + 4);
}

void vga_putchar(char c, unsigned short cl)
{
    if (c == '\0')
    {
        return;
    }

    if (VGA_CURSOR >= SCREEN_SIZE)
    {
        vga_clear();
    }

    if (c == '\n')
    {
        vga_newline();

        return;
    }

    if (c == '\t')
    {
        vga_tab();

        return;
    }

    VGA_POINTER[VGA_CURSOR]     = c;
    VGA_POINTER[VGA_CURSOR + 1] = cl;

    VGA_CURSOR = (VGA_CURSOR + 2);
}

void vga_clear(void)
{
    VGA_CURSOR = 0;

    unsigned int iter = VGA_CURSOR;

    while (iter < SCREEN_SIZE)
    {
        vga_putchar(' ', ATTR_BYTE_BLK_ON_BLK);

        iter = VGA_CURSOR;
    }

    VGA_CURSOR = 0;
}

void vga_backspace(void)
{
    VGA_CURSOR = (VGA_CURSOR - 2);

	VGA_POINTER[VGA_CURSOR] = ' ';
	VGA_POINTER[VGA_CURSOR + 1] = ATTR_BYTE_BLK_ON_BLK;
}