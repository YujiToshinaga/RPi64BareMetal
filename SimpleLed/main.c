#include "baremetal.h"

#define GPFSEL1 0x3F200004
#define GPSET0  0x3F20001C

int main(void)
{
  *(volatile unsigned int *)GPFSEL1 = 0x01 << (3 * 6);

  *(volatile unsigned int *)GPSET0 = 0x01 << 16;

  for ( ; ; );

  return 0;
}
