ENTRY(loader)   /*the name of the entry label*/

SECTIONS {
    . = 0x00100000;

    .text ALIGN (0x1000) : 
    {       /*align the 4kb*/
        *(.text)
    }

    .rodata ALIGN (0x1000) : {
        *(.rodata*)
    }

    .data ALIGN (0x1000) : {
        *(.data)
    }

    .bss ALIGN (0x1000) : {
        *(.COMMON)
        *(.bss)
    }
}