#include "baremetal.h"
#define MACRO_ONLY
#include "bcm283x.h"

OUTPUT_FORMAT(elf64-littleaarch64)
OUTPUT_ARCH(aarch64)
ENTRY(_entry)

MEMORY {
  RAM (rwx): ORIGIN = SDRAM_ADDR, LENGTH = SDRAM_SIZE
}

SECTIONS
{
  . = LOADER_ADDR;

  .startup . : {
    __STARTUP_START__ = .;
    startup.o(.startup)
    __STARTUP_END__ = .;
  } > RAM

  .text . : {
    __TEXT_START__ = .;
    *(.text*)
    __TEXT_END__ = .;
  } > RAM

  .rodata . : {
    __RODATA_START__ = .;
    *(.rodata*)
    __RODATA_END__ = .;
  } > RAM

  .data : ALIGN(16) {
    __DATA_START__ = .;
    *(.data*)
    __DATA_END__ = .;
  } > RAM

  .bss : ALIGN(16) {
    __BSS_START__ = .;
    *(.bss*)
    *(COMMON) /* TODO */
    __BSS_END__ = .;
  } > RAM

  .stack (NOLOAD) : ALIGN(16) {
    __STACK_START__ = .;
    *(.stack)
    __STACK_END__ = .;
  } > RAM
}

