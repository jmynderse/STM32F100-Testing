#include "../MRE6183_lib/mre6183_i2c.h"

void I2C1_PeripheralInit(void) {
	// create init structs
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	// initialize init structs
	GPIO_StructInit(&GPIO_InitStructure);
	I2C_StructInit(&I2C_InitStructure);
	
	// initialize GPIO clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	/* I2C1 SDA and SCL configuration */
	// SCL - PB6
	// SDA - PB7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// initialize the I2C1 clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);

	// reset and then un-reset I2C1 (might help with the stuck busy flag)
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	
	// configure I2C using default values
	I2C_InitStructure.I2C_ClockSpeed = 5000;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStructure);
	
	// enable I2C
	I2C_Cmd(I2C1, ENABLE);
}

// This function comes from the microcontroller errata document. In my testing,
// the stuck BUSY flag that I experienced seems to have actually been caused by a
// bad peripheral address
void I2C1_clear_BUSY_flag(void) {
	// create init structs
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	// initialize init structs
	GPIO_StructInit(&GPIO_InitStructure);
	I2C_StructInit(&I2C_InitStructure);

	//	The I2C analog filters embedded in the I2C I/Os may be tied to low level,
	//	whereas SCL and SDA lines are kept at high level. This can occur after
	//	an MCU power-on reset, or during ESD stress. Consequently, the I2C BUSY
	//	flag is set, and the I2C cannot enter master mode (START condition cannot
	//	be sent). The I2C BUSY flag cannot be cleared by the SWRST control bit,
	//	nor by a peripheral or a system reset. BUSY bit is cleared under reset,
	//	but it is set high again as soon as the reset is released, because the
	//	analog filter output is still at low level. This issue occurs randomly.
	
	// 1. Disable the I2C peripheral by clearing the PE bit in I2Cx_CR1 register.
	I2C_Cmd(I2C1, DISABLE);
	// 2. Configure the SCL and SDA I/Os as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
	// 3. Check SCL and SDA High level in GPIOx_IDR.
	while(!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_6));
	while(!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_7));
	// 4. Configure the SDA I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	// 5. Check SDA Low level in GPIOx_IDR.
	while(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_7));
	// 6. Configure the SCL I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	// 7. Check SCL Low level in GPIOx_IDR.
	while(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_6));
	// 8. Configure the SCL I/O as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	// 9. Check SCL High level in GPIOx_IDR.
	while(!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_6));
	// 10. Configure the SDA I/O as General Purpose Output Open-Drain , High level (Write 1 to GPIOx_ODR).
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	// 11. Check SDA High level in GPIOx_IDR.
	while(!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_7));
	// 12. Configure the SCL and SDA I/Os as Alternate function Open-Drain.
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// 13. Set SWRST bit in I2Cx_CR1 register.
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
	// 14. Clear SWRST bit in I2Cx_CR1 register.
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	// 15. Enable the I2C peripheral by setting the PE bit in I2Cx_CR1 register.
	// configure I2C using default values
	I2C_InitStructure.I2C_ClockSpeed = 5000;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1, ENABLE);
}

// send function
// taken from "Discovering the STM32 Microcontroller" by Geoffrey Brown
// https://legacy.cs.indiana.edu/~geobrown/book.pdf
//
// PeripheralAddress - this is specific to your peripheral
// buffer - pointer to array of bytes that contain the data to send
// num_byte - how many bytes are you sending
void I2C1_send_Xb(uint8_t PeripheralAddress, uint8_t *buffer, uint32_t num_byte) {
	if (!num_byte) {
		return;
	}
	
	// wait until the I2C is not busy
	while(I2C_GetFlagStatus(I2C1 , I2C_FLAG_BUSY));
	
	// Initiate Start Sequence
	I2C_GenerateSTART(I2C1 , ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	// Send Address
	I2C_Send7bitAddress(I2C1, PeripheralAddress << 1, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	// Send Data
	I2C_SendData(I2C1, *buffer++);
	while (--num_byte) {
		//while(!I2C_CheckEvent(I2C1 , I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		while(!I2C_GetFlagStatus(I2C1 , I2C_FLAG_BTF));
		I2C_SendData(I2C1, *buffer++);
	}
	//while(!I2C_CheckEvent(I2C1 , I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	while(!I2C_GetFlagStatus(I2C1 , I2C_FLAG_BTF));

	// Initiate Stop
	I2C_GenerateSTOP(I2C1, ENABLE);
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF));
}

// receive function
// taken from "Discovering the STM32 Microcontroller" by Geoffrey Brown
// https://legacy.cs.indiana.edu/~geobrown/book.pdf
//
// PeripheralAddress - this is specific to your peripheral
// buffer - pointer to array of bytes that will be filled by the receive operation
// num_byte - how many bytes are you receiving
void I2C1_receive_Xb(uint8_t PeripheralAddress, uint8_t *buffer, uint32_t num_byte) {
	if (!num_byte) {
		return;
	}
	
	// wait until the I2C is not busy
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	// Enable Acknowledgment, clear POS flag
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);

	// Intiate Start Sequence (wait for EV5)
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	// Send Address
	I2C_Send7bitAddress(I2C1, PeripheralAddress << 1, I2C_Direction_Receiver);
	while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR));
	
	if (num_byte == 1) {
		// Clear Ack bit
		I2C_AcknowledgeConfig(I2C1 , DISABLE);
		// EV6_1 -- must be atomic -- Clear ADDR , generate STOP
		__disable_irq ();
		(void) I2C1->SR2;
		I2C_GenerateSTOP(I2C1 ,ENABLE);
		__enable_irq ();
		// Receive data EV7
		while(! I2C_GetFlagStatus(I2C1 , I2C_FLAG_RXNE));
		*buffer++ = I2C_ReceiveData(I2C1);
	}
	else if (num_byte == 2) {
		// Set POS flag
		I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Next);
		// EV6_1 -- must be atomic and in this order
		__disable_irq ();
		(void) I2C1->SR2; // Clear ADDR flag
		I2C_AcknowledgeConfig(I2C1, DISABLE); // Clear Ack bit
		__enable_irq ();
		// EV7_3 -- Wait for BTF , program stop , read data twice
		while(! I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF));
		__disable_irq ();
		I2C_GenerateSTOP(I2C1, ENABLE);
		*buffer++ = I2C1 ->DR;
		__enable_irq ();
		*buffer++ = I2C1 ->DR;
	}
	else {
		(void) I2C1->SR2; // Clear ADDR flag
		while (num_byte-- != 3) {
			// EV7 -- cannot guarantee 1 transfer completion time,
			// wait for BTF instead of RXNE
			while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF));
			*buffer++ = I2C_ReceiveData(I2C1);
		}
		while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF));
		// EV7_2 -- Figure 1 has an error , doesn 't read N-2 !
		I2C_AcknowledgeConfig(I2C1 , DISABLE); // clear ack bit
		__disable_irq ();
		*buffer++ = I2C_ReceiveData(I2C1); // receive byte N-2
		I2C_GenerateSTOP(I2C1, ENABLE); // program stop
		__enable_irq ();
		*buffer++ = I2C_ReceiveData(I2C1); // receive byte N-1
		// wait for byte N
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
		*buffer++ = I2C_ReceiveData(I2C1);
		num_byte = 0;
	}
	
	// Wait for stop
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF));
	
	return;
}
