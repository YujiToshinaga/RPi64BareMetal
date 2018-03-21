#include "stddef.h"
#include "baremetal.h"
#include "aarch64.h"
#include "bcm283x.h"
#include "printk.h"
#include "serial.h"
#include "gpio.h"
#include "timer.h"
#include "xmodem.h"

#define SYNC_MAGIC 0x87654321

int sync = 0;

int main(void)
{
    mmap_t mm;
    int load_size = 0;
    uint64_t cur_tim;
    uint32_t tmp32;
    uint64_t tmp64;
    int i;

    // シリアルを有効化する
    if (prc_id() == 0) {
        serial_init();
    }

    // キャッシュ、MMUを有効化する
    cache_disable();

    SCTLR_EL1_WRITE(SCTLR_SA_BIT | SCTLR_A_BIT);
    ISB();

    CPUECTLR_EL1_READ(tmp64);
    tmp64 |= CPUECTLR_SMPEN_BIT;
    CPUECTLR_EL1_WRITE(tmp64);

    mmu_mmap_init();
    mm.pa   = LOADER_ADDR;
    mm.va   = mm.pa;
    mm.size = LOADER_SIZE;
    mm.attr = MEM_ATTR_NML_C;
    mm.ns   = MEM_NS_NONSECURE;
    mmu_mmap_add(&mm);
    mm.pa   = TMP_LOAD_ADDR;
    mm.va   = mm.pa;
    mm.size = TMP_LOAD_SIZE;
    mm.attr = MEM_ATTR_NML_C;
    mm.ns   = MEM_NS_NONSECURE;
    mmu_mmap_add(&mm);
    mm.pa   = LOAD_ADDR;
    mm.va   = mm.pa;
    mm.size = LOAD_SIZE;
    mm.attr = MEM_ATTR_SO;
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

    // バイナリロードを行う
    if (prc_id() == 0) {
        // ユーザ操作を待つ
        prints("Are you ready to load binary? (y)\n");
        while (serial_get() != 'y');
        prints("y\n");

        // 10秒待つ
        cur_tim = timer_get_syscount(); // [us]
        while (timer_get_syscount() < (cur_tim + 10 * 1000 * 1000));

        // XMODEM転送を行う
        xmodem_ready();
        load_size = xmodem_receiving((uint8_t *)TMP_LOAD_ADDR);
        printk("Received %d bytes\n", load_size);

        // キャッシュ領域から非キャッシュ領域へコピー
        for (i = 0; i < load_size; i = i + 4) {
            tmp32 = rd_word(TMP_LOAD_ADDR + i);
            wr_word(LOAD_ADDR + i, tmp32);
        }
        prints("Loading is completed\n");

        // 1秒待つ
        cur_tim = timer_get_syscount(); // [us]
        while (timer_get_syscount() < (cur_tim + 1 * 1000 * 1000));
    }

    // バリア同期する
    if (prc_id() == 0) {
        sync = SYNC_MAGIC;
    } else {
        while (sync != SYNC_MAGIC);
    }

    return 0;
}

