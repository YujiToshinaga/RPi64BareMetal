#include "stddef.h"
#include "baremetal.h"
#include "aarch64.h"
#include "bcm283x.h"
#include "printk.h"
#include "serial.h"
#include "gpio.h"
#include "timer.h"

void measure(void);

int main(void)
{
    mmap_t mm;

    // シリアルを有効化する
    serial_init();

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

    // Hello Worldを表示する
    printk("hello world\n");

    // GPIOトグルを測定する
    measure();

    for ( ; ; );

    return 0;
}

void measure(void)
{
    int gpio_state;
    int last_gpio_state = GPIO_LOW;
    uint64_t syscount;
    int cnt;
//    int i;

    gpio_init();
    gpio_fsel(16, GPIO_FSEL_OUT);
    gpio_fsel(17, GPIO_FSEL_IN);

    gpio_set(16, GPIO_HIGH);

    cnt = 0;
    for ( ; ; ) {
        gpio_state = gpio_get(17);
        if (gpio_state != last_gpio_state) {
            syscount = timer_get_syscount();
            if (gpio_state == GPIO_HIGH) {
                printk("high %5d %16ld\n", cnt, syscount);
            } else {
                printk("low  %5d %16ld\n", cnt, syscount);
            }
            last_gpio_state = gpio_state;
            cnt++;
        }
//        for (i = 0; i < 1000000 ; i++);
    }
}
