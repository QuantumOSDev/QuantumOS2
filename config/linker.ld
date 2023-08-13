ENTRY(_start)
 
SECTIONS
{
	kernel_section_start = .;
	. = 1M;
 
	.text : {
		kernel_text_section_start = .;
		*(.multiboot)
		*(.text)
        . = ALIGN(4096);
		kernel_text_section_end = .;
	}
 
	.rodata : {
        kernel_rodata_section_start = .;
        *(.rodata)
        kernel_rodata_section_end = .;
    }

	.data : {
		kernel_data_section_start = .;
		*(.data)
        . = ALIGN(4096);
        kernel_data_section_end = .;
	}
 
	.bss : {
        kernel_bss_section_start = .;
        bss = .; _bss = .; bss = .;
        *(.bss)
        . = ALIGN(4096);
        kernel_bss_section_end = .;
    }

    end = .; _end = .; end = .;
    kernel_section_end = .;
}
