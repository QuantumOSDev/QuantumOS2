/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#ifndef __QUANTUM_GDT_H
#define __QUANTUM_GDT_H

#define GDT_DESCRIPTOR_COUNT 8

typedef struct __tss_entry_t {
   unsigned int prev_tss;  
   unsigned int esp0;       
   unsigned int ss0;        
   unsigned int esp1;       
   unsigned int ss1;
   unsigned int esp2;
   unsigned int ss2;
   unsigned int cr3;
   unsigned int eip;
   unsigned int eflags;
   unsigned int eax;
   unsigned int ecx;
   unsigned int edx;
   unsigned int ebx;
   unsigned int esp;
   unsigned int ebp;
   unsigned int esi;
   unsigned int edi;
   unsigned int es;         
   unsigned int cs;
   unsigned int ss;         
   unsigned int ds;
   unsigned int fs;         
   unsigned int gs;
   unsigned int ldt;  
   unsigned short trap;
   unsigned short iomap_base;
} __attribute__((packed)) tss_entry_t;

typedef struct __gdt_t {
	unsigned short segment_limit;
	unsigned short base_low;
	unsigned char base_mid;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed)) gdt_t;

typedef struct __gdt_ptr_t {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed)) gdt_ptr_t;

void gdt_init();
void gdt_set_entry(int n, unsigned int base, unsigned int limit, unsigned int access, unsigned char gran);

void tts_write(int n, unsigned short ss0, unsigned int esp0);

extern void tss_flush();
extern void load_gdt(unsigned int __gdt_ptr);

#endif /* __QUANTUM_GDT_H */