#ifndef BAREMETAL_H
#define BAREMETAL_H

#define STACK_SIZE      0x00020000          // 128KB
#define STACK_CORE_SIZE (STACK_SIZE / 4)
#define STACK_ALIGN     4                   // 16byte align

#define START_ADDR      0x00000000
#define MAIN_ADDR       0x00000000
#define MAIN_SIZE       0x01000000          // 1MB

#endif /* BAREMETAL_H */

