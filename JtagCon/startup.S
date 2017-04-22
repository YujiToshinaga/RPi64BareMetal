#include "baremetal.h"
#include "aarch64.h"

    .global	_entry

    .section .startup, "ax"

    // Entry point
    .type _entry, %function
_entry:

    // Go to sleep if not core0
core_barrier:
    mrs   x0, mpidr_el1
    ldr   x1, =MPIDR_AFFINITY_MASK
    and   x0, x0, x1
    cmp   x0, 0x0
    b.eq  core0
sleep:
//    wfi
    b     sleep
core0:

    // JTAG configuration
jtagcfg:
jtagcfg_gpfsel2:            // JTAG configuration GPFSEL2
    ldr   x2, =0x3f200008
    ldr   w0, [x2]          // Load
    mov   w1, #7		        // FSEL22 <= ALT4
    lsl   w1, w1, #6
    mvn   w1, w1
    and   w0, w0, w1
    mov   w1, #3
    lsl   w1, w1, #6
    orr   w0, w0, w1
    mov   w1, #7		        // FSEL23 <= ALT4
    lsl   w1, w1, #9
    mvn   w1, w1
    and   w0, w0, w1
    mov   w1, #3
    lsl   w1, w1, #9
    orr   w0, w0, w1
    mov   w1, #7		        // FSEL24 <= ALT4
    lsl   w1, w1, #12
    mvn   w1, w1
    and   w0, w0, w1
    mov   w1, #3
    lsl   w1, w1, #12
    orr   w0, w0, w1
    mov   w1, #7		        // FSEL25 <= ALT4
    lsl   w1, w1, #15
    mvn   w1, w1
    and   w0, w0, w1
    mov   w1, #3
    lsl   w1, w1, #15
    orr   w0, w0, w1
    mov   w1, #7		        // FSEL26 <= ALT4
    lsl   w1, w1, #18
    mvn   w1, w1
    and   w0, w0, w1
    mov   w1, #3
    lsl   w1, w1, #18
    orr   w0, w0, w1
    mov   w1, #7		        // FSEL27 <= ALT4
    lsl   w1, w1, #21
    mvn   w1, w1
    and   w0, w0, w1
    mov   w1, #3
    lsl   w1, w1, #21
    orr   w0, w0, w1
    str   w0, [x2]          // Store
    dmb   sy

jtagcfg_gppud:              // JTAG configuration GPPUD
    ldr   x2, =0x3f200094
    mov   w0, #0
    str   w0, [x2]
    dmb   sy
    mov   x1, #150
jtagcfg_gppud_loop:
    nop
    sub   x1, x1, #1
    cmp   x1, #0
    bne   jtagcfg_gppud_loop

jtagcfg_gppudclk0:          // JTAG configuration GPPUDCLK0
    ldr   x2, =0x3f200098
    ldr   w0, =0x0FC00000		// Set GPIO22-27
    str   w0, [x2]
    dmb   sy
    mov   x1, #150
jtagcfg_gppudclk0_loop:
    nop
    sub   x1, x1, #1
    cmp   x1, #0
    bne   jtagcfg_gppudclk0_loop

jtagcfg_clear_gppudclk0:    // JTAG configuration clear GPPUDCLK0
    ldr   x2, =0x3f200098
    mov   w0, #0            // All clear
    str   w0, [x2]
    dmb   sy
    mov   x1, #150
jtagcfg_clear_gppudclk0_loop:
    nop
    sub   x1, x1, #1
    cmp   x1, #0
    bne   jtagcfg_clear_gppudclk0_loop

    // Set stack pointer
set_stack:
    ldr   x0, =(_stack + STACK_SIZE)
    mov   sp, x0
    isb

    // Jump main
    bl    main

panic:
    b     panic

