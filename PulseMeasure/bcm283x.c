#include "stddef.h"
#include "bcm283x.h"

/*
 *  プロセッサINDEX（0オリジン）の取得
 */
uint32_t prc_index(void)
{
    uint64_t mpidr;
    uint32_t index;

    asm volatile("mrs %0, mpidr_el1":"=r"(mpidr));
    index = (uint32_t)mpidr & 0x000000ff;
    return index;
}
