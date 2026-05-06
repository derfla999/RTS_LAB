/*
   Default linker script for MD407 (STM32F407)
   All code and data goes to RAM.
 */

/* Memory Spaces Definitions */
MEMORY
{
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 112K
}

SECTIONS
{
  /* the program code is stored in the .text section, which goes to RWM */
  .text : ALIGN(CONSTANT(COMMONPAGESIZE)) {
    *(.start_section)  /* startup code */
    *(.text)           /* remaining code */
    *(.text.*)
    *(.rodata)         /* read-only data (constants) */
    *(.rodata.*)
    *(.glue_7)
    *(.glue_7t)
  } >RAM

  /* This is the initialized data section */
  .data : ALIGN(CONSTANT(COMMONPAGESIZE)) {
    *(.data)
    *(.data.*)
  } >RAM

  /* This is the uninitialized data section */
  .bss : ALIGN(CONSTANT(COMMONPAGESIZE)) {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = . ;
    *(.bss)
    *(.bss.*)
    *(COMMON)
    /* This is used by the startup in order to initialize the .bss secion */
    _ebss = . ;
  } >RAM

  PROVIDE ( end = _ebss );
  PROVIDE ( _end = _ebss );
}

