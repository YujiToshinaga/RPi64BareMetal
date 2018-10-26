#ifndef GPIO_H
#define GPIO_H

#define GPIO_HIGH       0
#define GPIO_LOW        1

#define GPIO_FSEL_IN    0
#define GPIO_FSEL_OUT   1
#define GPIO_FSEL_0     2
#define GPIO_FSEL_1     3
#define GPIO_FSEL_2     4
#define GPIO_FSEL_3     5
#define GPIO_FSEL_4     6
#define GPIO_FSEL_5     7

#define GPIO_GPFSEL0            0x3f200000
#define GPIO_GPFSEL1            0x3f200004
#define GPIO_GPFSEL2            0x3f200008
#define GPIO_GPFSEL3            0x3f20000C
#define GPIO_GPFSEL4            0x3f200010
#define GPIO_GPFSEL5            0x3f200014
#define GPIO_GPSET0             0x3f20001C
#define GPIO_GPSET1             0x3f200020
#define GPIO_GPCLR0             0x3f200028
#define GPIO_GPCLR1             0x3f20002C
#define GPIO_GPLEV0             0x3f200034
#define GPIO_GPLEV1             0x3f200038
#define GPIO_GPEDS0             0x3f200040
#define GPIO_GPEDS1             0x3f200044
#define GPIO_GPREN0             0x3f20004C
#define GPIO_GPREN1             0x3f200050
#define GPIO_GPFEN0             0x3f200058
#define GPIO_GPFEN1             0x3f20005C
#define GPIO_GPHEN0             0x3f200064
#define GPIO_GPHEN1             0x3f200068
#define GPIO_GPLEN0             0x3f200070
#define GPIO_GPLEN1             0x3f200074
#define GPIO_GPAREN0            0x3f20007C
#define GPIO_GPAREN1            0x3f200080
#define GPIO_GPAFEN0            0x3f200088
#define GPIO_GPAFEN1            0x3f20008C
#define GPIO_GPPUD              0x3f200094
#define GPIO_GPPUDCLK0          0x3f200098
#define GPIO_GPPUDCLK1          0x3f20009C

#ifndef MACRO_ONLY

extern void gpio_init(void);
extern void gpio_fsel(int id, int fsel);
extern void gpio_set(int id, int set);
extern int gpio_get(int id);

#endif  /* MACRO_ONLY */

#endif /* GPIO_H */
