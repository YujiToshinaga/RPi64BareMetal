#ifndef BAREMETAL_H
#define BAREMETAL_H

#define STACK_SIZE      0x00020000          // 128KB
#define STACK_CORE_SIZE (STACK_SIZE / 4)
#define STACK_ALIGN     4                   // 16byte align

#define LOADER_ADDR     0x00000000
#define LOADER_SIZE     0x00080000
#define TMP_LOAD_ADDR   0x00080000
#define TMP_LOAD_SIZE   0x00080000
#define LOAD_ADDR       0x00100000
#define LOAD_SIZE       0x00080000          // 512KB

#endif /* BAREMETAL_H */

