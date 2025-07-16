#ifndef __MRE6183_I2C_H			//Define to prevent recursive inclusion
#define __MRE6183_I2C_H

#include "stm32f10x.h"
#include <stdbool.h>

void I2C1_PeripheralInit(void);
void I2C1_clear_BUSY_flag(void);
void I2C1_send_Xb(uint8_t PeripheralAddress, uint8_t *buffer, uint32_t num_byte);
void I2C1_receive_Xb(uint8_t PeripheralAddress, uint8_t *buffer, uint32_t num_byte);

#endif /*__MRE6183_I2C_H */
