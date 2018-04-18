#include "stddef.h"
#include "bcm283x.h"
#include "gpio.h"
#include "i2c.h"

#define I2C_MSTR0_BASE      0x3f205000
#define I2C_MSTR1_BASE      0x3f804000

#define I2C_C               0x00
#define I2C_S               0x04
#define I2C_DLEN            0x08
#define I2C_A               0x0c
#define I2C_FIFO            0x10
#define I2C_DIV             0x14
#define I2C_DEL             0x18
#define I2C_CLKT            0x1c

#define I2C_C_I2CEN_BIT     (0x1 << 15)
#define I2C_C_INTR_BIT      (0x1 << 10)
#define I2C_C_INTT_BIT      (0x1 << 9)
#define I2C_C_INTD_BIT      (0x1 << 8)
#define I2C_C_ST_BIT        (0x1 << 7)
#define I2C_C_CLEAR_SFT     4
#define I2C_C_CLEAR_MSK     (0x3 << 4)
#define I2C_C_CLEAR_CLEAR   (0x3 << 4)
#define I2C_C_READ_BIT      (0x1 << 0)

#define I2C_S_CLKT_BIT      (0x1 << 9)
#define I2C_S_ERR_BIT       (0x1 << 8)
#define I2C_S_RXF_BIT       (0x1 << 7)
#define I2C_S_TXE_BIT       (0x1 << 6)
#define I2C_S_RXD_BIT       (0x1 << 5)
#define I2C_S_TXD_BIT       (0x1 << 4)
#define I2C_S_RXR_BIT       (0x1 << 3)
#define I2C_S_TXW_BIT       (0x1 << 2)
#define I2C_S_DONE_BIT      (0x1 << 1)
#define I2C_S_TA_BIT        (0x1 << 0)

void i2c_init(int master_id, uint8_t slave_addr)
{
    uint64_t base = 0x0;
    int sda_gpio_id = 0;
    int sda_gpio_fsel = 0;
    int scl_gpio_id = 0;
    int scl_gpio_fsel = 0;

    /*
     *  I2CのMaster IDによってアクセスする対象を決める
     */
    switch (master_id) {
    case I2C_MSTR0:
        base = I2C_MSTR0_BASE;
        sda_gpio_id = 0;
        sda_gpio_fsel = GPIO_FSEL_0;
        scl_gpio_id = 1;
        scl_gpio_fsel = GPIO_FSEL_0;
        break;
    case I2C_MSTR1:
        base = I2C_MSTR1_BASE;
        sda_gpio_id = 2;
        sda_gpio_fsel = GPIO_FSEL_0;
        scl_gpio_id = 3;
        scl_gpio_fsel = GPIO_FSEL_0;
        break;
    default:
        return;
        break;
    }

    /*
     *  GPIOをI2Cに切り替える
     */
    gpio_fsel(sda_gpio_id, sda_gpio_fsel);
    gpio_fsel(scl_gpio_id, scl_gpio_fsel);

    /*
     *  I2Cを初期化する
     */
    // I2Cを有効にして，FIFOをクリアする
    wr_word(base + I2C_C, I2C_C_I2CEN_BIT | I2C_C_CLEAR_CLEAR);

    // I2C clockを100kHzに設定する
    // I2C clock = (i2c )core clock / DIV = 250MHz / 2500 = 100kHz
    // MEMO : 実測ではRPi3 i2cのcore clockと呼ばれるものは250MHzだった
    wr_word(base + I2C_DIV, 2500);

    // スレーブアドレスを設定する
    wr_word(base + I2C_A, slave_addr);
}

void i2c_read(int master_id, uint8_t* data, uint8_t len)
{
}

void i2c_write(int master_id, uint8_t* data, uint8_t len)
{
    uint64_t base = 0x0;
    uint32_t tmp;
    int i;

    switch (master_id) {
    case I2C_MSTR0:
        base = I2C_MSTR0_BASE;
        break;
    case I2C_MSTR1:
        base = I2C_MSTR1_BASE;
        break;
    default:
        return;
        break;
    }

    // 転送可能になるまで待つ
    while ((rd_word(base + I2C_S) & I2C_S_TA_BIT) == I2C_S_TA_BIT);

    // 書き込むデータ長を設定する
    wr_word(base + I2C_DLEN, len);

    // 転送モードを書き込みにする
    tmp = rd_word(base + I2C_C);
    wr_word(base + I2C_C, (tmp & ~I2C_C_READ_BIT));

    // ステータスをクリアにする
    wr_word(base + I2C_S, I2C_S_CLKT_BIT | I2C_S_ERR_BIT | I2C_S_DONE_BIT);

    // FIFOをクリアにして転送を開始する
    tmp = rd_word(base + I2C_C);
    wr_word(base + I2C_C, tmp | (I2C_C_ST_BIT | I2C_C_CLEAR_CLEAR));

    // FIFOレジスタにデータを書き込む
    for (i = 0; i < len; i++) {
        while ((rd_word(base + I2C_S) & I2C_S_TXD_BIT) == 0x0);
        wr_word(base + I2C_FIFO, data[i]);
    }

    // 転送完了するまで待つ
    while ((rd_word(base + I2C_S) & I2C_S_DONE_BIT) == 0x0);

    // エラー処理をする
    tmp = rd_word(I2C_S);
    if (tmp & I2C_S_CLKT_BIT) {
        wr_word(base + I2C_S, tmp | I2C_S_CLKT_BIT);
    }
    if (tmp & I2C_S_ERR_BIT) {
        wr_word(base + I2C_S, tmp | I2C_S_ERR_BIT);
    }
}
