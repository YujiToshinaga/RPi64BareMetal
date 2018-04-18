#include "stddef.h"
#include "bcm283x.h"
#include "gpio.h"
#include "pwm.h"

/*
 *  Clock Manager
 */
#define CM_PWMCTL               0x3f1010a0
#define CM_PWMDIV               0x3f1010a4
#define CM_PASSWD               0x5a000000

#define CM_PWMCTL_INT           (0x0 << 9)
#define CM_PWMCTL_1MASH         (0x1 << 9)
#define CM_PWMCTL_2MASH         (0x2 << 9)
#define CM_PWMCTL_3MASH         (0x3 << 9)
#define CM_PWMCTL_FLIP_BIT      (0x1 << 8)
#define CM_PWMCTL_BUSY_BIT      (0x1 << 7)
#define CM_PWMCTL_KILL_BIT      (0x1 << 5)
#define CM_PWMCTL_ENAB_BIT      (0x1 << 4)
#define CM_PWMCTL_SRC_GND       (0 << 0)
#define CM_PWMCTL_SRC_OSC       (1 << 0)
#define CM_PWMCTL_SRC_PLLA      (4 << 0)
#define CM_PWMCTL_SRC_PLLC      (5 << 0)
#define CM_PWMCTL_SRC_PLLD      (6 << 0)
#define CM_PWMDIV_DIVI_SFT      12
#define CM_PWMDIV_DIVI_MSK      (0xfff << 12)
#define CM_PWMDIV_DIVF_SFT      0
#define CM_PWMDIV_DIVF_MSK      (0xfff << 0)

#define CM_RXSEX_BIT            (0x1 << 23)
#define CM_RXF_BIT              (0x1 << 22)
#define CM_TXE_BIT              (0x1 << 21)
#define CM_RXD_BIT              (0x1 << 20)
#define CM_TXD_BIT              (0x1 << 19)
#define CM_RXR_BIT              (0x1 << 18)
#define CM_TXW_BIT              (0x1 << 17)
#define CM_RXERR_BIT            (0x1 << 16)

/*
 *  PWM
 */
#define PWM_CTL                 0x3f20c000
#define PWM_STA                 0x3f20c004
#define PWM_DMAC                0x3f20c008
#define PWM_RNG1                0x3f20c010
#define PWM_DAT1                0x3f20c014
#define PWM_FIF1                0x3f20c018
#define PWM_RNG2                0x3f20c020
#define PWM_DAT2                0x3f20c024

#define PWM_CTL_MSEN2_BIT       (0x1 << 15)
#define PWM_CTL_USEF2_BIT       (0x1 << 13)
#define PWM_CTL_POLA2_BIT       (0x1 << 12)
#define PWM_CTL_SBIT2_BIT       (0x1 << 11)
#define PWM_CTL_RPTL2_BIT       (0x1 << 10)
#define PWM_CTL_MODE2_BIT       (0x1 << 9)
#define PWM_CTL_PWEN2_BIT       (0x1 << 8)
#define PWM_CTL_MSEN1_BIT       (0x1 << 7)
#define PWM_CTL_CLRF1_BIT       (0x1 << 6)
#define PWM_CTL_USEF1_BIT       (0x1 << 5)
#define PWM_CTL_POLA1_BIT       (0x1 << 4)
#define PWM_CTL_SBIT1_BIT       (0x1 << 3)
#define PWM_CTL_RPTL1_BIT       (0x1 << 2)
#define PWM_CTL_MODE1_BIT       (0x1 << 1)
#define PWM_CTL_PWEN1_BIT       (0x1 << 0)

#define PWM_STA_STA4_BIT        (0x1 << 12)
#define PWM_STA_STA3_BIT        (0x1 << 11)
#define PWM_STA_STA2_BIT        (0x1 << 10)
#define PWM_STA_STA1_BIT        (0x1 << 9)
#define PWM_STA_BERR_BIT        (0x1 << 8)
#define PWM_STA_GAPO4_BIT       (0x1 << 7)
#define PWM_STA_GAPO3_BIT       (0x1 << 6)
#define PWM_STA_GAPO2_BIT       (0x1 << 5)
#define PWM_STA_GAPO1_BIT       (0x1 << 4)
#define PWM_STA_RERR1_BIT       (0x1 << 3)
#define PWM_STA_WERR1_BIT       (0x1 << 2)
#define PWM_STA_EMPT1_BIT       (0x1 << 1)
#define PWM_STA_FULL1_BIT       (0x1 << 0)

void pwm_init(int sr, int bit)
{
    /*
     *  GPIOをPWMに切り替え
     */
    gpio_fsel(40, GPIO_FSEL_0);
    gpio_fsel(45, GPIO_FSEL_0);

    /*
    *  Clock Manager Audio Clocks Control
    */
    // oscillator 19.2Mhz
    // PLLC 1000Mhz
    // PLLD 500Mhz
    //   250MHz = 500MHz / 2 => DIVI = 2, DIVF = 0
    wr_word(CM_PWMCTL, CM_PASSWD |
            CM_PWMCTL_KILL_BIT | CM_PWMCTL_SRC_PLLD);
    wr_word(CM_PWMDIV, CM_PASSWD |
            (2 << CM_PWMDIV_DIVI_SFT) | (0 << CM_PWMDIV_DIVF_SFT));
    wr_word(CM_PWMCTL, CM_PASSWD |
            CM_PWMCTL_INT | CM_PWMCTL_ENAB_BIT | CM_PWMCTL_SRC_PLLD);

    /*
    *  PWMを初期化
    */
    wr_word(PWM_RNG1, 250000000 / sr);
    wr_word(PWM_RNG2, 250000000 / sr);
    wr_word(PWM_CTL, PWM_CTL_USEF2_BIT | PWM_CTL_PWEN2_BIT |
            PWM_CTL_CLRF1_BIT | PWM_CTL_USEF1_BIT | PWM_CTL_PWEN1_BIT);
}

void pwm_write(uint32_t* val_l, uint32_t* val_r)
{
    while ((rd_word(PWM_STA) & PWM_STA_FULL1_BIT) == PWM_STA_FULL1_BIT);
    wr_word(PWM_FIF1,
            ((*val_l >> (32 - 11)) + (1 << (11 - 1))) & ((1 << 11) - 1));
    while ((rd_word(PWM_STA) & PWM_STA_FULL1_BIT) == PWM_STA_FULL1_BIT);
    wr_word(PWM_FIF1,
            ((*val_l >> (32 - 11)) + (1 << (11 - 1))) & ((1 << 11) - 1));
}

