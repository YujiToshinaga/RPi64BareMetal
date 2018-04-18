#ifndef UART_H
#define UART_H

#define AUX_IRQ                 0x3f215000 
#define AUX_ENABLES             0x3f215004 
#define MU_IO                   0x3f215040 
#define MU_IER                  0x3f215044 
#define MU_IIR                  0x3f215048 
#define MU_LCR                  0x3f21504c 
#define MU_MCR                  0x3f215050 
#define MU_LSR                  0x3f215054 
#define MU_MSR                  0x3f215058 
#define MU_SCRATCH              0x3f21505c 
#define MU_CNTL                 0x3f215060 
#define MU_STAT                 0x3f215064 
#define MU_BAUD                 0x3f215068 

#define AUX_ENABLES_MU_EN_BIT   (0x1 << 0)
#define AUX_ENABLES_SPI1_EN_BIT (0x1 << 1)
#define AUX_ENABLES_SPI2_EN_BIT (0x1 << 2)
#define MU_IO_DATA_SHIFT        0
#define MU_IO_DATA_MASK         (0xff << 0)
#define MU_IER_EN_TX_INT_BIT    (0x1 << 1)
#define MU_IER_EN_RX_INT_BIT    (0x1 << 0)
#define MU_IIR_FIFO_CLR         (0x3 << 1)
#define MU_LCR_DS_8BIT          (0x3 << 0)
#define MU_LSR_TX_IDLE_BIT      (0x1 << 6)
#define MU_LSR_TX_EMPTY_BIT     (0x1 << 5)
#define MU_LSR_RX_OVR_BIT       (0x1 << 1)
#define MU_LSR_DATA_RDY_BIT     (0x1 << 0)
#define MU_CNTL_TX_EN_BIT       (0x1 << 1)
#define MU_CNTL_RX_EN_BIT       (0x1 << 0)

#define UART_BAUDRATE           115200

#ifndef MACRO_ONLY

extern void uart_init(int baudrate);
extern bool_t uart_getready(void);
extern bool_t uart_putready(void);
extern void uart_put(char c);
extern char uart_get(void);
extern void uart_pol_put(char c);
extern char uart_pol_get(void);
extern void uart_put_int_ena(void);
extern void uart_put_int_dis(void);
extern void uart_get_int_ena(void);
extern void uart_get_int_dis(void);

#endif  /* MACRO_ONLY */

#endif /* UART_H */
