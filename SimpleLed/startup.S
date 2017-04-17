#include "baremetal.h"

    .global	_entry

    .section .startup, "ax"

    // Entry point
    .type _entry, %function
_entry:

    // Go to sleep if not core0
//barrier:
//    mrs   x0, mpidr_el1
//    ldr   x1, =MPIDR_AFFINITY_MASK
//    and   x0, x0, x1
//    cmp   x0, 0x0
//    b.eq  core0
//
//sleep:
//    wfi
//    b     sleep
//
//core0:

    // Set stack pointer
stack:
    ldr   x0, =(_stack + STACK_SIZE)
    mov   sp, x0
    isb


    bl    main

panic:
    b     panic

