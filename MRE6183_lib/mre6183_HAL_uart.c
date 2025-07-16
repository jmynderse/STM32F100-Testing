#include "../MRE6183_lib/mre6183_HAL_uart.h"

void UART_Init(void)
{
	// create init structs
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	//UART_InitTypeDef USART_InitStructure = {0};
	
	// initialize clocks
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_USART2_CLK_ENABLE();
	
	// TX pin
	GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  //GPIO_InitStruct.Alternate = ;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	// RX pin
	GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  //GPIO_InitStruct.Alternate = ;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// configure UART using default values
	h_uart2.Instance = USART2;
  h_uart2.Init.BaudRate = 19200;
	h_uart2.Init.WordLength = UART_WORDLENGTH_8B;
  h_uart2.Init.StopBits = UART_STOPBITS_1;
  h_uart2.Init.Parity = UART_PARITY_NONE;
  h_uart2.Init.Mode = UART_MODE_RX | UART_MODE_TX;
  h_uart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	HAL_UART_Init(&h_uart2);
	
	// enable UART
	//HAL_UART_Cmd(h_uart2, ENABLE);
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

void UART_StringInEcho(char *buffer)
{
  uint16_t ii, serial_char_in;
  ii = 0;
  
  while (ii<UART_BUFFER_SIZE) {
    serial_char_in = UART_CharIn();
    if (serial_char_in == 0x0D) {
      // character is an enter/return
      break;
    }
    else
    {
    	UART_CharOut(serial_char_in);
      buffer[ii] = serial_char_in;
      ii++;
    }
  }
}

void UART_NumericStringInEcho(char *buffer)
{
  uint16_t ii, serial_char_in;
  ii = 0;
  
  while (ii<UART_BUFFER_SIZE) {
    serial_char_in = UART_CharIn();
    if (serial_char_in == 0x0D) {
      // character is an enter/return
      break;
    }
    else if ( (serial_char_in >= 0x30) && (serial_char_in <= 0x39) ) {
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
		// otherwise ignore it
  }
}
