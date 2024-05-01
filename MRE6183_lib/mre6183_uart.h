#ifndef __MRE6183_UART_H			//Define to prevent recursive inclusion
#define __MRE6183_UART_H

#include "stm32f10x_conf.h"
#include <string.h>

#define UART_BUFFER_SIZE 50

void UART_Init(void);
void UART_CharOut(char serial_char_out);
void UART_StringOut(char serial_string_out[UART_BUFFER_SIZE]);

#endif /*__MRE6183_UART_H */
