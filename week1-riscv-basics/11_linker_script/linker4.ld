/* Minimal linker script for hello2.elf */

ENTRY(_start)

MEMORY {
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
  SRAM  (rwx) : ORIGIN = 0x10000000, LENGTH = 64K
}

SECTIONS {
  /* Place .text at 0x00000000 in FLASH */
  .text 0x0 : {
    *(.text*)
    *(.rodata*)
  } > FLASH

  /* Place .data at 0x10000000 in SRAM */
  .data 0x10000000 : {
    _data_start = .;
    *(.data*)
    _data_end = .;
  } > SRAM

  /* Place .bss immediately after .data in SRAM */
  .bss : {
    _bss_start = .;
    *(.bss*)
    *(COMMON)
    _bss_end = .;
  } > SRAM

  /* Set the top of the stack to end of SRAM */
  _stack_top = ORIGIN(SRAM) + LENGTH(SRAM);
}
