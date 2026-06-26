/*============================================================================*/
/* STM32F103C8.ld — Linker script for STM32F103C8 (Cortex-M3, 64KB/20KB)     */
/* Matches memory map from Keil scatter-file Objects/test.sct:                */
/*   IROM:  0x08000000, size 0x10000 (64 KB)                                  */
/*   IRAM:  0x20000000, size 0x05000 (20 KB)                                  */
/*============================================================================*/

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
}

/* Top of stack = top of RAM */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* Interrupt vector table — must be first in flash */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    /* Program code and read-only data */
    .text :
    {
        . = ALIGN(4);
        *(.text*)
        *(.glue_7)
        *(.glue_7t)
        . = ALIGN(4);
    } > FLASH

    .rodata :
    {
        . = ALIGN(4);
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    /* Constructor/destructor tables */
    .preinit_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array*))
        PROVIDE_HIDDEN(__preinit_array_end = .);
    } > FLASH

    .init_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array*))
        PROVIDE_HIDDEN(__init_array_end = .);
    } > FLASH

    .fini_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array*))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } > FLASH

    /* Initialized data — in flash for load, in RAM for runtime */
    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    /* Zero-initialized data */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } > RAM

    /* Heap and stack */
    ._heap_stack :
    {
        . = ALIGN(4);
        PROVIDE(end = .);
        . = . + 0x400;  /* 1 KB heap */
        . = . + 0x400;  /* 1 KB stack */
        . = ALIGN(4);
    } > RAM

}
