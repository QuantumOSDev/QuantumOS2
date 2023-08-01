#include <quantum/drivers/pio.h>

void outb(unsigned short port, unsigned char data)
{
    asm volatile ("outb %1, %0" :: "dN" (port), "a" (data));
}

void outs(unsigned short port, unsigned short data)
{
    asm volatile ("outw %1, %0" :: "dN" (port), "a" (data));
}

void outl(unsigned short port, unsigned int data)
{
    asm volatile ("outl %%eax, %%dx" :: "dN" (port), "a" (data));
}

unsigned char inb(unsigned short port)
{
    unsigned char result;

    asm volatile ("inb %1, %0" : "=a" (result) : "Nd" (port));

    return result;
}

unsigned short ins(unsigned short port)
{
    unsigned short result;

    asm volatile ("inw %1, %0" : "=a" (result) : "dN" (port));

    return result;
}

unsigned int inl(unsigned short port)
{
    unsigned int result;

    asm volatile ("inl %%dx, %%eax" : "=a" (result) : "dN" (port));

    return result;
}

void psleep(void)
{
    outb(0x80, 0);
}