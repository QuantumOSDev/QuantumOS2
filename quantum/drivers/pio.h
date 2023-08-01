#ifndef PIO_H
#define PIO_H

void outb(unsigned short port, unsigned char data);
void outs(unsigned short port, unsigned short data);
void outl(unsigned short port, unsigned int data);

unsigned char  inb(unsigned short port);
unsigned short ins(unsigned short port);
unsigned int   inl(unsigned short port);

void psleep(void);

#endif