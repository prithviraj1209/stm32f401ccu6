#ifndef I2C_STM32F401CCU6_H
#define I2C_STM32F401CCU6_H

#include "stm32f4xx.h"
#include <stdint.h>

// Function prototypes
void i2c_config(void);
void i2c_startSeq(void);
void i2c_stopSeq(void);
void i2c_write(uint8_t data);
void i2c_writeMul(uint8_t *data, uint8_t size);
void i2c_address(uint8_t address);

#endif // I2C_STM32F401CCU6_H