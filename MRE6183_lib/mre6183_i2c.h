#ifndef __MRE6183_I2C_H			//Define to prevent recursive inclusion
#define __MRE6183_I2C_H

#include "stm32f10x_conf.h"
#include <stdbool.h>

void PeripheralInit_I2C1(void);
void clear_I2C1_BUSY_flag(void);
void send_8b_I2C1(uint8_t PeripheralAddress, uint8_t outByte);
void send_Xb_I2C1(uint8_t PeripheralAddress, uint8_t *buffer, uint32_t num_byte);
void send_16b_I2C1(uint8_t PeripheralAddress, uint16_t outData);
void receive_Xb_I2C1(uint8_t PeripheralAddress, uint8_t *buffer, uint32_t num_byte);
uint8_t receive_8b_I2C1(uint8_t PeripheralAddress);

#endif /*__MRE6183_I2C_H */
