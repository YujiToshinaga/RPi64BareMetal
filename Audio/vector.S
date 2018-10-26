#include "baremetal.h"
#define MACRO_ONLY
#include "aarch64.h"
#include "bcm283x.h"
#include "exception.h"

    .align 11
    .section .vector, "ax"
    .global vector_table
    .type vector_table, %function
vector_table:
    b cur_sp0_sync_handler
    .balign 0x80
    b cur_sp0_irq_handler
    .balign 0x80
    b cur_sp0_fiq_handler
    .balign 0x80
    b cur_sp0_serr_handler
    .balign 0x80
    b cur_spx_sync_handler
    .balign 0x80
    b cur_spx_irq_handler
    .balign 0x80
    b cur_spx_fiq_handler
    .balign 0x80
    b cur_spx_serr_handler
    .balign 0x80
    b l64_sync_handler
    .balign 0x80
    b l64_irq_handler
    .balign 0x80
    b l64_fiq_handler
    .balign 0x80
    b l64_serr_handler
    .balign 0x80
    b l32_sync_handler
    .balign 0x80
    b l32_irq_handler
    .balign 0x80
    b l32_fiq_handler
    .balign 0x80
    b l32_serr_handler
    .balign 0x80

    .text
    .align 2
    .global cur_sp0_sync_handler
cur_sp0_sync_handler:
    b     cur_sp0_sync_handler

    .text
    .align 2
    .global cur_sp0_irq_handler
cur_sp0_irq_handler:
    b     cur_sp0_irq_handler

    .text
    .align 2
    .global cur_sp0_fiq_handler
cur_sp0_fiq_handler:
    b     cur_sp0_fiq_handler

    .text
    .align 2
    .global cur_sp0_serr_handler
cur_sp0_serr_handler:
    b     cur_sp0_serr_handler

    .text
    .align 2
    .global cur_spx_sync_handler
cur_spx_sync_handler:
    b     cur_spx_sync_handler
//    sub   sp, sp, #(16 * 10)
//    stp   x0, x1, [sp, #0x00]
//    stp   x2, x3, [sp, #0x10]
//    stp   x4, x5, [sp, #0x20]
//    stp   x6, x7, [sp, #0x30]
//    stp   x8, x9, [sp, #0x40]
//    stp   x10, x11, [sp, #0x50]
//    stp   x12, x13, [sp, #0x60]
//    stp   x14, x15, [sp, #0x70]
//    stp   x16, x17, [sp, #0x80]
//    stp   x18, x30, [sp, #0x90]
//    mrs   x0, elr_el1
//    mrs   x1, spsr_el1
//    stp   x0, x1, [sp, #-16]!
//    mov   x0, #(DAIF_CPULOCK AOR DAIF_ALWAYS_SET)
//    msr   daif, x0

    .text
    .align 2
    .global cur_spx_irq_handler
cur_spx_irq_handler:
    sub   sp, sp, #(16 * 10)
    stp   x0, x1, [sp, #0x00]
    stp   x2, x3, [sp, #0x10]
    stp   x4, x5, [sp, #0x20]
    stp   x6, x7, [sp, #0x30]
    stp   x8, x9, [sp, #0x40]
    stp   x10, x11, [sp, #0x50]
    stp   x12, x13, [sp, #0x60]
    stp   x14, x15, [sp, #0x70]
    stp   x16, x17, [sp, #0x80]
    stp   x18, x30, [sp, #0x90]

    mrs   x0, elr_el1
    mrs   x1, spsr_el1
    stp   x0, x1, [sp, #-16]!

    bl    exception_irq_handler

    ldp   x0, x1, [sp], #16
    msr   elr_el1, x0
    msr   spsr_el1, x1

    ldp   x18, x30, [sp, #0x90]
    ldp   x16, x17, [sp, #0x80]
    ldp   x14, x15, [sp, #0x70]
    ldp   x12, x13, [sp, #0x60]
    ldp   x10, x11, [sp, #0x50]
    ldp   x8, x9, [sp, #0x40]
    ldp   x6, x7, [sp, #0x30]
    ldp   x4, x5, [sp, #0x20]
    ldp   x2, x3, [sp, #0x10]
    ldp   x0, x1, [sp]
    add   sp, sp, #(16 * 10)
    eret

    .text
    .align 2
    .global cur_spx_fiq_handler
cur_spx_fiq_handler:
    b     cur_spx_fiq_handler

    .text
    .align 2
    .global cur_spx_serr_handler
cur_spx_serr_handler:
    b     cur_spx_serr_handler

    .text
    .align 2
    .global l64_sync_handler
l64_sync_handler:
    b     l64_sync_handler

    .text
    .align 2
    .global l64_irq_handler
l64_irq_handler:
    b     l64_irq_handler

    .text
    .align 2
    .global l64_fiq_handler
l64_fiq_handler:
    b     l64_fiq_handler

    .text
    .align 2
    .global l64_serr_handler
l64_serr_handler:
    b     l64_serr_handler

    .text
    .align 2
    .global l32_sync_handler
l32_sync_handler:
    b     l32_sync_handler

    .text
    .align 2
    .global l32_irq_handler
l32_irq_handler:
    b     l32_irq_handler

    .text
    .align 2
    .global l32_fiq_handler
l32_fiq_handler:
    b     l32_fiq_handler

    .text
    .align 2
    .global l32_serr_handler
l32_serr_handler:
    b     l32_serr_handler

