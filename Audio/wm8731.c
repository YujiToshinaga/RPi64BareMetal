#include "stddef.h"
#include "bcm283x.h"
#include "gpio.h"
#include "i2c.h"
#include "i2s.h"
#include "wm8731.h"

#define WM8731_LLINEIN      0x00
#define WM8731_RLINEIN      0x01
#define WM8731_LHPOUT       0x02
#define WM8731_RHPOUT       0x03
#define WM8731_ANACTRL      0x04
#define WM8731_DIGCTRL      0x05
#define WM8731_POWER        0x06
#define WM8731_DIGIF        0x07
#define WM8731_SAMPLE       0x08
#define WM8731_ACTCTRL      0x09
#define WM8731_RESET        0x0f

#define WM8731_LINEIN_INVOL(x)      ((((x) * 2 + 69) / 3) << 0)
#define WM8731_LINEIN_INMUTE_BIT    (0x1 << 7)
#define WM8731_LINEIN_LRINBOTH_BIT  (0x1 << 8)

#define WM8731_HPOUT_HPVOL(x)       ((((x) + 121) / 1) << 0)
#define WM8731_HPOUT_HPVOL_MUTE     (0x0 << 0)
#define WM8731_HPOUT_ZCEN_BIT       (0x1 << 7)
#define WM8731_HPOUT_LRHPBOTH_BIT   (0x1 << 8)

#define WM8731_ANACTRL_MICBOOST_BIT (0x1 << 0)
#define WM8731_ANACTRL_MUTEMIC_BIT  (0x1 << 1)
#define WM8731_ANACTRL_INSEL_BIT    (0x1 << 2)
#define WM8731_ANACTRL_BYPASS_BIT   (0x1 << 3)
#define WM8731_ANACTRL_DACSEL_BIT   (0x1 << 4)
#define WM8731_ANACTRL_SIDETONE_BIT (0x1 << 5)
#define WM8731_ANACTRL_SIDEATT_M15  (0x3 << 6)
#define WM8731_ANACTRL_SIDEATT_M12  (0x2 << 6)
#define WM8731_ANACTRL_SIDEATT_M9   (0x1 << 6)
#define WM8731_ANACTRL_SIDEATT_0    (0x0 << 6)

#define WM8731_DIGCTRL_ADCHPD_BIT   (0x1 << 0)
#define WM8731_DIGCTRL_DEEMP_48     (0x3 << 1)
#define WM8731_DIGCTRL_DEEMP_44     (0x2 << 1)
#define WM8731_DIGCTRL_DEEMP_32     (0x1 << 1)
#define WM8731_DIGCTRL_DEEMP_DIS    (0x0 << 1)
#define WM8731_DIGCTRL_DACMU_BIT    (0x1 << 3)
#define WM8731_DIGCTRL_HPOR_BIT     (0x1 << 4)

#define WM8731_POWER_LINEINPD_BIT   (0x1 << 0)
#define WM8731_POWER_MICPD_BIT      (0x1 << 1)
#define WM8731_POWER_ADCPD_BIT      (0x1 << 2)
#define WM8731_POWER_DACPD_BIT      (0x1 << 3)
#define WM8731_POWER_OUTPD_BIT      (0x1 << 4)
#define WM8731_POWER_OSCPD_BIT      (0x1 << 5)
#define WM8731_POWER_CLKOUTPD_BIT   (0x1 << 6)
#define WM8731_POWER_POWEROFF_BIT   (0x1 << 7)

#define WM8731_DIGIF_FORMAT_DSP     (0x3 << 0)
#define WM8731_DIGIF_FORMAT_I2S     (0x2 << 0)
#define WM8731_DIGIF_FORMAT_LEFT    (0x1 << 0)
#define WM8731_DIGIF_FORMAT_RIGHT   (0x0 << 0)
#define WM8731_DIGIF_IWL_32         (0x3 << 2)
#define WM8731_DIGIF_IWL_24         (0x2 << 2)
#define WM8731_DIGIF_IWL_20         (0x1 << 2)
#define WM8731_DIGIF_IWL_16         (0x0 << 2)
#define WM8731_DIGIF_LRP_BIT        (0x1 << 4)
#define WM8731_DIGIF_LRSWAP_BIT     (0x1 << 5)
#define WM8731_DIGIF_MS_BIT         (0x1 << 6)
#define WM8731_DIGIF_BCLKINV_BIT    (0x1 << 7)

#define WM8731_SAMPLE_USB_BIT       (0x1 << 0)
#define WM8731_SAMPLE_BOSR_BIT      (0x1 << 1)
#define WM8731_SAMPLE_SR_48         (0x0 << 2)
#define WM8731_SAMPLE_SR_8          (0x3 << 2)
#define WM8731_SAMPLE_SR_32         (0x6 << 2)
#define WM8731_SAMPLE_SR_96         (0x7 << 2)
#define WM8731_SAMPLE_SR_44         (0x8 << 2)
#define WM8731_SAMPLE_SR_88         (0xf << 2)
#define WM8731_SAMPLE_CLKDIV2       (0x1 << 6)

#define WM8731_ACTCTRL_ACTIVE_BIT   (0x1 << 0)

#define WM8731_RESET_RESET          (0x00 << 0)

void wm8731_write(uint32_t addr, uint32_t data)
{
    uint8_t tmp[2];

    tmp[0] = 0x0;
    tmp[0] = tmp[0] | (uint8_t)((addr & 0x0000007f) << 1);
    tmp[0] = tmp[0] | (uint8_t)((data & 0x00000100) >> 8);
    tmp[1] = 0x0;
    tmp[1] = tmp[1] | (uint8_t)(data & 0x000000ff);
    i2c_write(I2C_MSTR1, tmp, 2);
}

uint32_t wm8731_read(uint32_t addr)
{
    return 0;
}

void wm8731_init(int master_mode, int sr, int bit)
{
    uint32_t data;

    // Reset register
    //    00000000を書き込むことでリセットされる
    wm8731_write(WM8731_RESET, WM8731_RESET_RESET);

    // Left line in
    // Rigth line in
    //    4:0   Line input vol                          : 0dB
    //    7     Mute to ADC                             : disable
    //    8     LとRのVol/Muteの同期をとる              : enable
    data = 0x0;
    data = data | WM8731_LINEIN_INVOL(0);
    data = data | WM8731_LINEIN_LRINBOTH_BIT;
    wm8731_write(WM8731_LLINEIN, data);
    wm8731_write(WM8731_RLINEIN, data);

    // Left headphone out
    // Right headphone out
    //    6:0   HP Vol                                  : 0dB
    //    7     Zero cross detect                       : disable 
    //    8     LとRのVol/Mute/Zero crossの同期をとる   : enable
    data = 0x0;
    data = data | WM8731_HPOUT_HPVOL(0);
    data = data | WM8731_HPOUT_LRHPBOTH_BIT;
    wm8731_write(WM8731_LHPOUT, data);
    wm8731_write(WM8731_RHPOUT, data);

    // Analog audio path control
    //    0     mic input level boost                   : disable
    //    1     mic input mute                          : disable
    //    2     mic/line input select to ADC            : line
    //    3     bypass                                  : disable
    //    4     dac select                              : select dac
    //    5     side tone                               : disable
    //    7:6   side tone attenuation                   : -6dB
    data = 0x0;
    data = data | WM8731_ANACTRL_DACSEL_BIT;
    data = data | WM8731_ANACTRL_MUTEMIC_BIT;
    wm8731_write(WM8731_ANACTRL, data);

    // Digital audio path control
    //    0     ADC high pass filter                    : disable
    //    2:1   de-enphasis                             : disable
    //    3     DAC soft mute                           : disable
    //    4     dc offset when high pass filter         : clear
    data = 0x0;
//    data = data | WM8731_DIGCTRL_HPOR_BIT; // debug
    wm8731_write(WM8731_DIGCTRL, data);

    // Power down control
    //    各機能のPower downを無効にするには00000000にする
    data = 0x0;
    data = data | WM8731_POWER_MICPD_BIT;
    wm8731_write(WM8731_POWER, data);

    // Digital interface
    //    1:0   Audio tmp Format Select                 : I2S
    //    3:2   Input Audio tmp Bit                     : 32bits
    //    4     
    //    5
    //    6     Master Slave Mode Control               : Master Mode / Slave Mode
    //    7
    if (master_mode == WM8731_MSTR) {
        switch (bit) {
        case 32:
            data = 0x0;
            data = data | WM8731_DIGIF_FORMAT_I2S;
            data = data | WM8731_DIGIF_IWL_32;
            data = data | WM8731_DIGIF_MS_BIT;
            wm8731_write(WM8731_DIGIF, data);
            break;
        case 24:
            data = 0x0;
            data = data | WM8731_DIGIF_FORMAT_I2S;
            data = data | WM8731_DIGIF_IWL_24;
            data = data | WM8731_DIGIF_MS_BIT;
            wm8731_write(WM8731_DIGIF, data);
            break;
        case 20:
            break;
        case 16:
            break;
        default:
            break;
        }
    } else {
        switch (bit) {
        case 32:
            data = 0x0;
            data = data | WM8731_DIGIF_FORMAT_I2S;
            data = data | WM8731_DIGIF_IWL_32;
            wm8731_write(WM8731_DIGIF, data);
            break;
        case 24:
            data = 0x0;
            data = data | WM8731_DIGIF_FORMAT_I2S;
            data = data | WM8731_DIGIF_IWL_24;
            wm8731_write(WM8731_DIGIF, data);
            break;
        case 20:
            break;
        case 16:
            break;
        default:
            break;
        }
    }

    // Sampling control
    //    0     Mode select                             : USB mode
    //    1     BOSR                                    : 272fs
    //    5:2   Sampling Rate
    //    6     CLKDIV2                                 : Clock is MCLK
    switch (sr) {
    case 8000:
        break;
    case 32000:
        break;
    case 44100:
        break;
    case 48000:
        data = 0x0;
        data = data | WM8731_SAMPLE_USB_BIT;
        data = data | WM8731_SAMPLE_SR_48;
        wm8731_write(WM8731_SAMPLE, data);
        break;
    case 96000:
        break;
    default:
        break;
    }
}

void wm8713_active(void)
{
    uint32_t data;

    // Activate control
    //    0     Active
    data = 0x0;
    data = data | WM8731_ACTCTRL_ACTIVE_BIT;
    wm8731_write(WM8731_ACTCTRL, data);
}

void wm8713_inactive(void)
{
    uint32_t data;

    // Activate control
    //    0     Active
    data = 0x0;
    wm8731_write(WM8731_ACTCTRL, data);
}

