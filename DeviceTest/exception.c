#include "stddef.h"
#include "aarch64.h"
#include "bcm283x.h"
#include "exception.h"
//#include "printk.h" // TODO : debug用

void (*irq_handler_cb[IRQNO_NUM])(void);

void exception_init(void)
{
    int i;

    for (i = 0; i < IRQNO_NUM; i++) {
        irq_handler_cb[i] = NULL;
    }

    extern void *vector_table;
    VBAR_EL1_WRITE((uint64_t)&vector_table);

    wr_word(CORE0_MICTL, 0x1U);
    wr_word(CORE1_MICTL, 0x2U);
    wr_word(CORE2_MICTL, 0x4U);
    wr_word(CORE3_MICTL, 0x8U);
    wr_word(CORE0_MBOX0_RC, 0xffffffffU);
    wr_word(CORE1_MBOX1_RC, 0xffffffffU);
    wr_word(CORE2_MBOX2_RC, 0xffffffffU);
    wr_word(CORE3_MBOX3_RC, 0xffffffffU);

    wr_word((DISABLE_IRQ_B), 0xffffffffU);
    wr_word((DISABLE_IRQ_1), 0xffffffffU);
    wr_word((DISABLE_IRQ_2), 0xffffffffU);
}

void exception_irq_handler(void)
{
//    // TODO : 下記はデバッグ用
//    prints("exception!\n");
//    printk("IRQ_B : %08x\n", *(int *)IRQ_PEND_B);
//    printk("IRQ_1 : %08x\n", *(int *)IRQ_PEND_1);
//    printk("IRQ_2 : %08x\n", *(int *)IRQ_PEND_2);

    // TODO : 割込みIDを判別して対応する割込み処理を呼ぶべき
    if (irq_handler_cb[IRQNO_AUX] != NULL) {
        irq_handler_cb[IRQNO_AUX]();
    }
}

void exception_int_ena(int no, void (*callback)(void))
{
    irq_handler_cb[no] = callback;

    if ((0 <= no) && (no < 32)) {
        // 0-31の割込みはNMIとしている
    } else if ((32 <= no) && (no < 64)) {
        wr_word(ENABLE_IRQ_B, (0x1 << (no - 32)));
    } else if ((64 <= no) && (no < 96)) {
        wr_word(ENABLE_IRQ_1, (0x1 << (no - 64)));
    } else if ((96 <= no) && (no < 128)) {
        wr_word(ENABLE_IRQ_2, (0x1 << (no - 96)));
    }
}

void exception_int_dis(int no)
{
    irq_handler_cb[no] = NULL;

    if ((0 <= no) && (no < 32)) {
        // 0-31の割込みはNMIとしている
    } else if ((32 <= no) && (no < 64)) {
        wr_word(DISABLE_IRQ_B, (0x1 << (no - 32)));
    } else if ((64 <= no) && (no < 96)) {                   
        wr_word(DISABLE_IRQ_1, (0x1 << (no - 64)));
    } else if ((96 <= no) && (no < 128)) {                  
        wr_word(DISABLE_IRQ_2, (0x1 << (no - 96)));
    }
}
