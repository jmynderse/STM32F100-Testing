#include "stm32f10x_conf.h"

void PeripheralInit_SPI1_Master(void);
void PeripheralInit_SPI_SlaveSelect(void);
uint8_t transfer_8b_SPI1_Master(uint8_t outByte);
uint16_t transfer_16b_SPI1_Master(uint16_t outData);
uint16_t receive_16b_SPI1_Master(void);
void PeripheralInit_SPI2_Master(void);
uint8_t transfer_8b_SPI2_Master(uint8_t outByte);
void SPI_SlaveSelect(void);
void SPI_SlaveDeselect(void);
