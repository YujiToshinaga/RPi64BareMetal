#include "stddef.h"
#include "baremetal.h"
#include "aarch64.h"
#include "bcm283x.h"
#include "exception.h"
#include "gpio.h"
#include "uart.h"
#include "printk.h"
#include "timer.h"

void main_init(void);
void uart_int_handler(void);

int main(void)
{
    main_init();

    prints("hello world\n");

	exception_int_ena(IRQNO_AUX, uart_int_handler);
	uart_get_int_ena();

    prints("end\n");

    for ( ; ; );

    return 0;
}

void main_init(void)
{
    mmap_t mm;

    // 例外を初期化する
    exception_init();

    // IOを初期化する
    gpio_init();
    uart_init(UART_BAUDRATE);

    // キャッシュ、MMUを有効化する
    cache_disable();

    mmu_mmap_init();

    mm.pa   = SDRAM_ADDR;
    mm.va   = mm.pa;
    mm.size = SDRAM_SIZE;
    mm.attr = MEM_ATTR_NML_C;
    mm.ns   = MEM_NS_NONSECURE;
    mmu_mmap_add(&mm);
    mm.pa   = IO0_ADDR;
    mm.va   = mm.pa;
    mm.size = IO0_SIZE;
    mm.attr = MEM_ATTR_DEV;
    mm.ns   = MEM_NS_NONSECURE;
    mmu_mmap_add(&mm);
    mm.pa   = IO1_ADDR;
    mm.va   = mm.pa;
    mm.size = IO1_SIZE;
    mm.attr = MEM_ATTR_DEV;
    mm.ns   = MEM_NS_NONSECURE;
    mmu_mmap_add(&mm);

    mmu_init();
    cache_enable();
}

void uart_int_handler(void)
{
	printk("uart int handler\n");
	uart_get();
}
