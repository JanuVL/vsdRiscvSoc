OUTPUT_ARCH(riscv)
ENTRY(main)
SECTIONS
{
    . = 0x80000000;
    .text : {
        *(.text.start)
        *(.text)
    }
    .data : ALIGN(4) {
        *(.data)
    }
    .bss : ALIGN(4) {
        *(.bss)
    }
    _end = .;
}
