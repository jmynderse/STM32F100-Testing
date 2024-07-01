#ifndef __MRE6183_SPI_H			//Define to prevent recursive inclusion
#define __MRE6183_SPI_H

#include "stm32f10x_conf.h"

void SPI1_PeripheralInit_Master(uint16_t CPOL, uint32_t CPHA);
void SPI1_PeripheralInit_SlaveSelect(void);
uint8_t SPI1_send_8b_receive_8b_Master(uint8_t outByte);
uint16_t SPI1_transfer_16b_Master(uint16_t outData);
void SPI1_send_16b_Master(uint16_t outData);
uint16_t SPI1receive_16b__Master(void);
void SPI1_SlaveSelect(void);
void SPI1_SlaveDeselect(void);

void SPI2_PeripheralInit_Master(void);

#endif /*__MRE6183_SPI_H */
