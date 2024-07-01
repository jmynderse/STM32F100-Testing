#include "../MRE6183_lib/mre6183_spi.h"

// Code taken from: https://learnbuildshare.wordpress.com/about/stm32/using-spi-as-master/

// Setup SPI1 pins A4/SS, A5/SCK, A6/MISO, A7/MOSI on port A
// Hardware slave selectis not used. That is indicated in the settings
void SPI1_PeripheralInit_Master(uint16_t CPOL, uint32_t CPHA) {
	// create init structs
  GPIO_InitTypeDef GPIO_InitDef;
  SPI_InitTypeDef SPI_InitDef;
  // initialize init structs
  GPIO_StructInit(&GPIO_InitDef);
  SPI_StructInit(&SPI_InitDef);

	// initialize clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);

	// do not initialize A4/SS because a software SS will be used

	// initialize A5/SCK alternate function push-pull (50 MHz)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitDef);

	// initialize A6/MISO input pull-up (50 MHz)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitDef);

	// initialize A7/MOSI alternate function push-pull (50 MHz)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitDef);

	// initialize SPI master
	// for slave, no need to define SPI_BaudRatePrescaler
	SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitDef.SPI_Mode = SPI_Mode_Master;
	SPI_InitDef.SPI_DataSize = SPI_DataSize_16b;     // 8-bit transactions
	SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;    // MSB first
	SPI_InitDef.SPI_CPOL = SPI_CPOL_High;            // CPOL = 1, clock idle high
	SPI_InitDef.SPI_CPHA = SPI_CPHA_2Edge;          // CPHA = 1
	SPI_InitDef.SPI_NSS = SPI_NSS_Soft;             // use software SS
	// SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // APB1 24/64 = 0.375 MHz
	SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // APB1 24/8 = 3 MHz
	// SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // APB1 24/2 = 12 MHz
	SPI_InitDef.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitDef);

	// enable SPI
	SPI_Cmd(SPI1, ENABLE);
}

void SPI1_PeripheralInit_Master_ReadOnly_16b(void) {
	// create init structs 
	GPIO_InitTypeDef GPIO_InitDef;
	SPI_InitTypeDef SPI_InitDef;
	// initialize init structs
	GPIO_StructInit(&GPIO_InitDef);
	SPI_StructInit(&SPI_InitDef);

	// initialize clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);

	// do not initialize A4/SS because a software SS will be used

	// initialize A5/SCK alternate function push-pull (50 MHz)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitDef);

	// initialize A6/MISO input pull-up (50 MHz)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitDef);

	// initialize A7/MOSI alternate function push-pull (50 MHz)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitDef);

	// initialize SPI master
	// for slave, no need to define SPI_BaudRatePrescaler
	SPI_InitDef.SPI_Direction = SPI_Direction_1Line_Rx;
	SPI_InitDef.SPI_Mode = SPI_Mode_Master;
	SPI_InitDef.SPI_DataSize = SPI_DataSize_16b;     // 8-bit transactions
	SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;    // MSB first
	SPI_InitDef.SPI_CPOL = SPI_CPOL_Low;            // CPOL = 0, clock idle low
	SPI_InitDef.SPI_CPHA = SPI_CPHA_2Edge;          // CPHA = 0, read on clock rising edge
	SPI_InitDef.SPI_NSS = SPI_NSS_Soft;             // use software SS
	SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // APB1 24/64 = 0.375 MHz
	// SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // APB1 24/8 = 3 MHz
	// SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // APB1 24/2 = 12 MHz
	SPI_InitDef.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitDef);

	// enable SPI
	SPI_Cmd(SPI1, ENABLE);
}

// Setup slave select, output, A4 on port A
void SPI1_PeripheralInit_SlaveSelect(void) {
	GPIO_InitTypeDef GPIO_InitDef;  // GPIO init
	GPIO_StructInit(&GPIO_InitDef); // initialize init struct

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // initialize clock

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_4;                 // GPIO pin 3
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_Out_PP;  // GPIO mode output push-pull
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;  // GPIO port output speed, 2 MHz
	GPIO_Init(GPIOA, &GPIO_InitDef);    // initialize pin on GPIOA port
}

void SPI1_SlaveSelect(void) {
	// reset (to zero) the SS pin to start communication
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

void SPI1_SlaveDeselect(void) {
	// set (to one) the SS pin to end communication
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}


// Transfer a byte over SPI1  A4/SS, A5/SCK, A6/MISO, A7/MOSI
uint8_t SPI1_send_8b_receive_8b_Master(uint8_t outByte){
	volatile uint16_t outTwoByte, inTwoByte;
	outTwoByte = outByte << 8;

	// Approach 1, from Brown's book
	// SPI_I2S_SendData(SPI1, outByte); // send
	// while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	// return SPI_I2S_ReceiveData(SPI1); // read received

	// Approach 2,
	// from http://www.lxtronic.com/index.php/basic-spi-simple-read-write
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1, outTwoByte);                                                        // send
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	inTwoByte = SPI_I2S_ReceiveData(SPI1);
	return inTwoByte;                                                   // read received
}

uint16_t SPI1_transfer_16b_Master(uint16_t outData){
	// Approach 1, from Brown's book
	// SPI_I2S_SendData(SPI1, outByte); // send
	// while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	// return SPI_I2S_ReceiveData(SPI1); // read received

	// Approach 2,
	// from http://www.lxtronic.com/index.php/basic-spi-simple-read-write
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1, outData);                                                        // send
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	return SPI_I2S_ReceiveData(SPI1);                                                   // read received
}

// Transfer a byte over SPI1  A4/SS, A5/SCK, A6/MISO, A7/MOSI
uint16_t SPI1_receive_16b_Master(void) {
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	return SPI_I2S_ReceiveData(SPI1);                                                   // read received
}

void SPI1_send_16b_Master(uint16_t outData){
	// Approach 1, from Brown's book
	// SPI_I2S_SendData(SPI1, outByte); // send
	// while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	// return SPI_I2S_ReceiveData(SPI1); // read received

	// Approach 2,
	// from http://www.lxtronic.com/index.php/basic-spi-simple-read-write
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI1, outData);                                                        // send
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));
	return;
}

// Setup SPI2 pins B12/SS, B13/SCK, B14/MISO, B15/MOSI on port B
void SPI2_PeripheralInit_Master(void) {
	// create init structs
	GPIO_InitTypeDef GPIO_InitDef;
	SPI_InitTypeDef SPI_InitDef;
	// initialize init structs
	GPIO_StructInit(&GPIO_InitDef);
	SPI_StructInit(&SPI_InitDef);

	// initialize clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); // this is on a different bus

	// do not initialize B12/SS because a software SS will be used

	// initialize B13/SCK alternate function push-pull (10 MHz)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitDef);

	// initialize B14/MISO input pull-up (10 MHz)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitDef);

	// initialize B15/MOSI alternate function push-pull (10 MHz)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitDef);

	//  initialize SPI master
	// for slave, no need to define SPI_BaudRatePrescaler
	SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitDef.SPI_Mode = SPI_Mode_Master;
	SPI_InitDef.SPI_DataSize = SPI_DataSize_8b;     // 8-bit transactions
	SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;    // MSB first
	SPI_InitDef.SPI_CPOL = SPI_CPOL_Low;            // CPOL = 0, clock idle low
	SPI_InitDef.SPI_CPHA = SPI_CPHA_2Edge;          // CPHA = 1
	SPI_InitDef.SPI_NSS = SPI_NSS_Soft;             // use software SS
	// SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // APB1 24/64 = 0.375 MHz
	SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // APB1 24/8 = 3 MHz
	// SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // APB1 24/2 = 12 MHz
	SPI_InitDef.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitDef);

	// enable SPI
	SPI_Cmd(SPI2, ENABLE);
}
