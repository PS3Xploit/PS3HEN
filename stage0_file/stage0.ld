OUTPUT_FORMAT("elf64-powerpc")
OUTPUT_ARCH(powerpc:common64)

__base = 0x80000000007f0000;

ENTRY(_start)

PHDRS {
	stage1		PT_LOAD FLAGS(7);
}

SECTIONS {
	. = __base;
	__self_start = .;

	.start : { *(.start) } :stage1 = 0

	. = ALIGN(32);
	.text : {
		*(.text)
		*(.text.*)
	}
	.rodata : {
		*(.rodata)
		*(.rodata.*)
	}
	.data : {
		*(.data)
		*(.data.*)
		*(.sdata)
		*(.sdata.*)
	}
	.bss : {
		*(.sbss)
		*(.sbss.*)
		*(COMMON)
		*(.bss)
		*(.bss.*)
		LONG(0)
	}

	. = ALIGN(32);
	__toc = . + 0x8000;
	.toc : {
		*(.toc)
		*(.toc.*)
	}
	.opd : {
		*(.opd)
		*(.opd.*)
	}

	__self_end = .;
}





