#include "stddef.h"
#include "baremetal.h"
#include "aarch64.h"
#include "bcm283x.h"
#include "exception.h"
#include "gpio.h"
#include "uart.h"
#include "printk.h"
#include "timer.h"
#include "i2c.h"
#include "i2s.h"
#include "wm8731.h"
#include "pwm.h"
#include "audio.h"
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

void main_init(void);
void audio_play(void);

int main(void)
{
    main_init();

    prints("hello world\n");

    audio_play();

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
    i2c_init(I2C_MSTR1, 0x1a);
    i2s_init(I2S_SLAV, 48000, 32);
    wm8731_init(WM8731_MSTR, 48000, 32);

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

//static uint32_t buf[48000 * 10 * 2];

void audio_play(void)
{
//    uint32_t l, r;
//    int count;
//    int i;

//    uint32_t buf_l[32], buf_r[32];

//    prints("init 1\n");
//    gpio_init();

//    prints("init 2\n");
//    i2c_init(I2C_MSTR1, 0x1a);
//    i2s_init(I2S_SLAV, 48000, 32);
//    wm8731_init(WM8731_MSTR, 48000, 32);
//
//    prints("init 3\n");
//    wm8713_active();
//
//
//    // wm8731 Line in -> Line out
//    count = 0;
//    for ( ; ; ) {
//        i2s_read(&l, &r);
//        if ((count % 48000) == 0) {
//            printk("%08x %08x\n", l, r);
//        }
//
//        i2s_write(&l, &r);
//
////        if (count < 48000 * 10) {
////            buf[count * 2 + 0] = l;
////            buf[count * 2 + 1] = r;
////        } else {
////            break;
////        }
//        count++;
//    }
//
////    for (i = 0; i < 48000 * 10 * 2; i++) {
////        printk("%08x\n", buf[i]);
////    }

//    prints("init 2\n");
//    gpio_init();
//    pwm_init(48000, 32);
//
//    // pwm out
//    count = 0;
//    for ( ; ; ) {
//        l = sin1k_48khz32bit[count % 48];
//        r = sin1k_48khz32bit[count % 48];
//
////        if ((count % 48000) == 32) {
////            printk("%08x %08x\n", l, r);
////        }
//
//        pwm_write(&l, &r);
//
//        count++;
//    }



    uint32_t inbuf_l[BUFFERING_SIZE], inbuf_r[BUFFERING_SIZE];
    uint32_t outbuf_l[BUFFERING_SIZE], outbuf_r[BUFFERING_SIZE];
    bool_t read = false;
    int i;
    int count = 0;

	prints("init 1\n");

    audio_open();

	prints("init 2\n");

    count = 0;
    for ( ; ; ) {
        read = audio_read_data(inbuf_l, inbuf_r);
        if (read == true) {
            for (i = 0; i < BUFFERING_SIZE; i++) {
                outbuf_l[i] = inbuf_l[i];
                outbuf_r[i] = inbuf_r[i];
            }
            audio_write_data(outbuf_l, outbuf_r);

            count = count + BUFFERING_SIZE;
            if ((count % 48000) == 0) {
                printk("%08x\n", inbuf_l[0]);
            }
        }
    }

    audio_close();

	for ( ; ; );
}
