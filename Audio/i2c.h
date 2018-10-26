#ifndef I2C_H
#define I2C_H

#define I2C_MSTR0   0
#define I2C_MSTR1   1

extern void i2c_init(int master_id, uint8_t slave_addr);
extern void i2c_read(int master_id, uint8_t* data, uint8_t len);
extern void i2c_write(int master_id, uint8_t* data, uint8_t len);

#endif /* I2C_H */
