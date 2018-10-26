#include "stddef.h"
#include "bcm283x.h"
#include "gpio.h"
#include "uart.h"

void uart_init(int baudrate)
{
    uint32_t baud;

    gpio_fsel(14, GPIO_FSEL_5);
    gpio_fsel(15, GPIO_FSEL_5);

    baud = (250000000 / (baudrate * 8)) - 1;

    wr_word(AUX_ENABLES, rd_word(AUX_ENABLES) | AUX_ENABLES_MU_EN_BIT);
    wr_word(MU_CNTL, 0);
    wr_word(MU_IER, 0);
    wr_word(MU_IIR, MU_IIR_FIFO_CLR);
    wr_word(MU_LCR, MU_LCR_DS_8BIT);
    wr_word(MU_MCR, 0);
    wr_word(MU_BAUD, baud);
    wr_word(MU_CNTL, MU_CNTL_TX_EN_BIT | MU_CNTL_RX_EN_BIT);
}

bool_t uart_getready(void)
{
    bool_t ready = false;

    if ((rd_word(MU_LSR) & MU_LSR_DATA_RDY_BIT) == MU_LSR_DATA_RDY_BIT) {
        ready = true;
    } else {
        ready = false;
    }
    return ready;
}

bool_t uart_putready(void)
{
    bool_t ready = false;

    if ((rd_word(MU_LSR) & MU_LSR_TX_EMPTY_BIT) == MU_LSR_TX_EMPTY_BIT) {
        ready = true;
    } else {
        ready = false;
    }
    return ready;
}

void uart_put(char c)
{
    wr_word(MU_IO, c);
}

char uart_get(void)
{
    return ((char)(rd_word(MU_IO) & MU_IO_DATA_MASK));
}

void uart_pol_put(char c)
{
    while ((rd_word(MU_LSR) & MU_LSR_TX_EMPTY_BIT) != MU_LSR_TX_EMPTY_BIT);
    wr_word(MU_IO, c);
}

char uart_pol_get(void)
{
    while ((rd_word(MU_LSR) & MU_LSR_DATA_RDY_BIT) != MU_LSR_DATA_RDY_BIT);
    return ((char)(rd_word(MU_IO) & MU_IO_DATA_MASK));
}

void uart_put_int_ena(void)
{
    wr_word(MU_IER, rd_word(MU_IER) | MU_IER_EN_TX_INT_BIT);
}

void uart_put_int_dis(void)
{
    wr_word(MU_IER, rd_word(MU_IER) & ~MU_IER_EN_TX_INT_BIT);
}

void uart_get_int_ena(void)
{
    wr_word(MU_IER, rd_word(MU_IER) | MU_IER_EN_RX_INT_BIT);
}

void uart_get_int_dis(void)
{
    wr_word(MU_IER, rd_word(MU_IER) & ~MU_IER_EN_RX_INT_BIT);
}
