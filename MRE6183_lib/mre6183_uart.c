#include "../MRE6183_lib/mre6183_uart.h"

void UART_Init(void)
{
	// create init structs
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	// initialize init structs
  GPIO_StructInit(&GPIO_InitStructure);
  USART_StructInit(&USART_InitStructure);
	
	// initialize clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	// TX pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	// RX pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// configure UART using default values
	USART_InitStructure.USART_BaudRate = 19200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &USART_InitStructure);
	
	// enable UART
	USART_Cmd(USART2, ENABLE);
}

void UART_CharOut(char serial_char_out)
{
	// delay until the transmit data register is ready
	while( USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
	// send the character
	USART_SendData(USART2, serial_char_out);
	// delay until the transmission is done
	while( USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET );
}

void UART_StringOut(char serial_string_out[UART_BUFFER_SIZE])
{
	uint8_t ii;
	for (ii=0; ii<strlen(serial_string_out); ii++)
	{
		UART_CharOut(serial_string_out[ii]);
	}
}

void UART_LineBreak(void)
{
	UART_CharOut('\r');
	UART_CharOut('\n');
}

char UART_CharIn(void)
{
	uint16_t serial_char_in;
  
  // delay until the received data register is not empty
	while( USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET )
	// receive the character
  serial_char_in = USART_ReceiveData(USART2);
	// return the character
  return serial_char_in;
}

char UART_CharInEcho(void)
{
	uint16_t serial_char_in;
  
  // delay until the received data register is not empty
	while( USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET )
	// receive the character
  serial_char_in = USART_ReceiveData(USART2);
  // echo the received character
  UART_CharOut(serial_char_in);
	// return the character
  return serial_char_in;
}


void UART_NumericalStringInEcho(char *buffer)
{
  uint16_t ii, serial_char_in;
  ii = 0;
  
  while (ii<UART_BUFFER_SIZE) {
    serial_char_in = UART_CharIn();
    if ( (serial_char_in >= 0x30) && (serial_char_in <= 0x39) ) {
      // character is a number between 0 and 9
      UART_CharOut(serial_char_in);
      buffer[ii] = serial_char_in;
      ii++;
    }
    else if (serial_char_in == 0x2E) {
      // character is a period
      UART_CharOut(serial_char_in);
      buffer[ii] = serial_char_in;
      ii++;
    }
    else if (serial_char_in == 0x0D) {
      // character is an enter/return
      break;
    }
  }
}
