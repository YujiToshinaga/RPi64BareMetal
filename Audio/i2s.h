#ifndef I2S_H
#define I2S_H

extern void i2s_init(int sr, int bit);
//extern void i2s_play(void);
extern void i2s_read(uint32_t* val_l, uint32_t* val_r);
extern void i2s_write(uint32_t* val_l, uint32_t* val_r);

#endif /* I2S_H */
