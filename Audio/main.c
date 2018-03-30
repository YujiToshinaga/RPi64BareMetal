#include "stddef.h"
#include "baremetal.h"
#include "aarch64.h"
#include "bcm283x.h"
#include "printk.h"
#include "serial.h"
#include "gpio.h"
#include "timer.h"
#include "i2c.h"
#include "i2s.h"
#include "wm8731.h"
#include "xmodem.h"

uint32_t sin1k_48khz32bit[48] = {
    0X00000000, 0X01abb54e, 0X03501927, 0X04e5fa21,
    0X06666666, 0X07caca19, 0X090d0c29, 0X0a27a904,
    0X0b15cac3, 0X0bd35e53, 0X0c5d2550, 0X0cb0c43b,
    0X0ccccccd, 0X0cb0c43b, 0X0c5d2550, 0X0bd35e53,
    0X0b15cac3, 0X0a27a904, 0X090d0c29, 0X07caca19,
    0X06666666, 0X04e5fa21, 0X03501927, 0X01abb54e,
    0X00000000, 0Xfe544ab2, 0Xfcafe6d9, 0Xfb1a05df,
    0Xf999999a, 0Xf83535e7, 0Xf6f2f3d7, 0Xf5d856fc,
    0Xf4ea353d, 0Xf42ca1ad, 0Xf3a2dab0, 0Xf34f3bc5,
    0Xf3333333, 0Xf34f3bc5, 0Xf3a2dab0, 0Xf42ca1ad,
    0Xf4ea353d, 0Xf5d856fc, 0Xf6f2f3d7, 0Xf83535e7,
    0Xf999999a, 0Xfb1a05df, 0Xfcafe6d9, 0Xfe544ab2,
};

void memory_init(void);
void audio_play(void);

int main(void)
{
    // シリアルを有効化する
    serial_init();

    prints("hello world\n");

    memory_init();

    audio_play();

    prints("end\n");

    for ( ; ; );

    return 0;
}

void memory_init(void)
{
    mmap_t mm;
    uint64_t tmp64;

    // キャッシュ、MMUを有効化する
    cache_disable();

    SCTLR_EL1_READ(tmp64);
    tmp64 |= (SCTLR_SA_BIT | SCTLR_A_BIT);
    SCTLR_EL1_WRITE(tmp64);
    ISB();

    CPUECTLR_EL1_READ(tmp64);
    tmp64 |= CPUECTLR_SMPEN_BIT;
    CPUECTLR_EL1_WRITE(tmp64);
    ISB();

    mmu_mmap_init();

    mm.pa   = MAIN_ADDR;
    mm.va   = mm.pa;
    mm.size = MAIN_SIZE;
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

//static uint32_t buf[48000 * 10 * 2];

void audio_play(void)
{
    uint32_t l, r;
    int count;
//    int i;

    prints("init 1\n");
    gpio_init();
    i2c_init(I2C_MSTR1, 0x1a);
    i2s_init(I2S_SLAV, 48000, 32);

    prints("init 2\n");
    wm8731_init(WM8731_MSTR, 48000, 32);

    prints("init 3\n");
    wm8713_active();

    count = 0;
    for ( ; ; ) {
        i2s_read(&l, &r);
        if ((count % 48000) == 0) {
            printk("%08x %08x\n", l, r);
        }

        i2s_write(&l, &r);

//        if (count < 48000 * 10) {
//            buf[count * 2 + 0] = l;
//            buf[count * 2 + 1] = r;
//        } else {
//            break;
//        }
        count++;
    }

//    for (i = 0; i < 48000 * 10 * 2; i++) {
//        printk("%08x\n", buf[i]);
//    }
}
