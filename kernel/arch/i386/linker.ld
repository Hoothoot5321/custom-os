/* The bootloader will look at this image and start execution at the symbol
   designated at the entry point. */
ENTRY(_start)

VIRTUAL_OFFSET = 0xC0000000;

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 1M;

	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	_kernel_start = .;

	.multiboot.data : {
		*(.multiboot.data)
	}
	
	.multiboot.text : {
		*(.multiboot.text)
	}

	. += VIRTUAL_OFFSET;
	.text  ALIGN(4K): AT (ADDR (.text) - VIRTUAL_OFFSET)
	{
		*(.text)
	}


	/* Read-only data. */
	.rodata ALIGN(4K) : AT (ADDR (.rodata) - VIRTUAL_OFFSET)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data  ALIGN(4K) : AT (ADDR (.data) - VIRTUAL_OFFSET)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss  ALIGN(4K) : AT (ADDR (.bss) - VIRTUAL_OFFSET)
	{
		*(COMMON)
		*(.bss)
		*(.stack)
	}
	
	_kernel_end = .;
	PROVIDE(_kernel_end = .);

}
