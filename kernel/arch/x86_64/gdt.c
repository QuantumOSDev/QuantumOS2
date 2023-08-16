/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/arch/x86_64/gdt.h>

#include <quantum/clib/string.h>
#include <quantum/clib/stdio.h>

gdt_t gdt_desc[GDT_DESCRIPTOR_COUNT];
gdt_ptr_t gdt_ptr;

tss_entry_t tss_entry;

void gdt_init()
{
    gdt_ptr.limit = sizeof(gdt_desc) - 1;
	gdt_ptr.base  = (unsigned int)gdt_desc;

	gdt_set_entry(0, 0, 0, 0, 0);
	gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
	tts_write(5, 0x10, 0x0);

	load_gdt((unsigned int)&gdt_ptr);
	tss_flush();
}

void gdt_set_entry(int n, unsigned int base, unsigned int limit, unsigned int access, unsigned char gran)
{
    success_printf("gdt_set_entry", "n=%d, base=%ux, limit=%ux, access=%ux, gran=%ux\n",
                n, base, limit, access, gran);

	gdt_t* current_gdt = &gdt_desc[n];

	current_gdt->segment_limit = (limit & 0xFFFF);
	current_gdt->base_low	   = (base & 0xFFFF);
	current_gdt->base_mid	   = (base >> 16) & 0xFF;
	current_gdt->access		   = access;
	current_gdt->granularity   = (limit >> 16) & 0x0F;
	current_gdt->granularity   = (current_gdt->granularity | (gran & 0xF0));
	current_gdt->base_high	   = (base >> 24 & 0xFF);
}

void tts_write(int n, unsigned short ss0, unsigned int esp0)
{
    unsigned int base = (unsigned int)&tss_entry;
    unsigned int limit = base + sizeof(tss_entry_t);

    gdt_set_entry(n, base, limit, 0xE9, 0x00);
    memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0  = ss0;  
    tss_entry.esp0 = esp0; 

    tss_entry.cs   = 0x0b;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;

    success_printf("tts_write", "n=%d, ss0=%ux, esp0=%ux, cs=0x0b\n",
               	 n, ss0, esp0);
}