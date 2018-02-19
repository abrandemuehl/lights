MEMORY {
  /* FLASH (rx) : ORIGIN = 0x08000000,  LENGTH = 256K */
  /* RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 32K */
  FLASH (rx) : ORIGIN = 0x08000000,  LENGTH = 64K
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 8K
}

/* End of 32K RAM */
_estack = ORIGIN(RAM) + LENGTH(RAM); /* Used by startup.s */

ENTRY(Reset_Handler)

SECTIONS {
  .int_vector_table : {
    . = ALIGN(4); /* Align the start of the section to 4 */
    *(.int_vector_table)
    . = ALIGN(4); /* Align the end of the section to 4 */
  } > FLASH
  .text : {
    . = ALIGN(4); /* Align the start of the section to 4 */
    *(.text)
    *(.rodata)
    *(.rodata*)

    /* C data constructors and destructors */
    KEEP(*(.init))
    KEEP(*(.fini))
    . = ALIGN(4); /* Align the end of the section to 4 */
  } > FLASH

  /* ARM unwinding things */
  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .data : { /* .data section for all static data */
    . = ALIGN(4); /* Align the start of the section to 4 */
    *(.data)
    *(.data*)
    . = ALIGN(4); /* Align the end of the section to 4 */
  } > RAM AT >FLASH /* Put in both FLASH and RAM */
  .bss : { /* .bss section for zero initialized data */
    . = ALIGN(4); /* Align the start of the section to 4 */
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    . = ALIGN(4); /* Align the end of the section to 4 */
    __bss_end__ = .;
  } > RAM AT >FLASH /* Put in both FLASH and RAM */

  /* Define the heap */
  .heap : {
    end = .;
    KEEP(*(.heap))
    __heap_end__ = .;
    __HeapLimit__ = __heap_end__;
  } >RAM AT >FLASH
}
