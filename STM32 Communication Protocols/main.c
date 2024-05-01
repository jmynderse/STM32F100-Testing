#include "stm32f10x_conf.h"
#include "../MRE6183_lib/mre6183_led.h"
#include "../MRE6183_lib/mre6183_delays.h"
#include "../MRE6183_lib/mre6183_uart.h"
#include "../MRE6183_lib/mre6183_spi.h"
#include "../MRE6183_lib/mre6183_i2c.h"
#include "mre6183_SHTC3.h"
#include <stdio.h>
#include <string.h>

uint16_t numRead16b = 0;
float temp = 0;

bool testSPI = 0;
bool testI2C = 1;
bool testUART = 0;
char serial_char_out;
char serial_string_out1[UART_BUFFER_SIZE];

int main(void) {
//	uint8_t numRead8b = 0;
//	uint16_t numRead16b = 0;
	uint8_t data[3];
	uint16_t received_data;
	float Temp_RH[2];
	
	LED_PC8_Init();
	LED_PC9_Init();
	UART_Init();
	TimerDelay_Init();
	
	if (testSPI) {
		PeripheralInit_SPI_SlaveSelect();
		SPI_SlaveDeselect();
		PeripheralInit_SPI1_Master();
	}
	
	LEDToggle(GPIO_Pin_8);
	
	// initialization using I2C
	// this is very specific to the SHTC3 temperature and relative humidity sensor used in my testing
	if (testI2C) {
		// output some debugging text to the UART
		sprintf(serial_string_out1, "\r\n");
		UART_StringOut(serial_string_out1);
		sprintf(serial_string_out1, "Initializing SHTC3... \r\n");
		UART_StringOut(serial_string_out1);
		
		// initialize the I2C peripheral
		PeripheralInit_I2C1();
		
		// SHTC3-specific startup procedure: reset, wake-up, verify ID, sleep
		SHTC3_write_command(SHTC3_SOFTRESET);
		SHTC3_write_command(SHTC3_WAKEUP);
		SHTC3_read_command(SHTC3_READID, data, 3);
		// using the ID number display to validate against the logic analyzer
		sprintf(serial_string_out1, "Received: %02x \t %02x \t %02x \r\n", data[0], data[1], data[2]);
		UART_StringOut(serial_string_out1);
		SHTC3_write_command(SHTC3_SLEEP);

		// output some debugging text to the UART
		sprintf(serial_string_out1, "SHTC3 going to sleep...\r\n");
		UART_StringOut(serial_string_out1);
		sprintf(serial_string_out1, "\r\n");
	}
	
	while(1) {
		// LED on
		LEDToggle(GPIO_Pin_9);
		
		// are we testing SPI communication?
		if(testSPI) {
			// enable SPI on the peripheral
			SPI_SlaveSelect();
			// transfer first byte
			numRead16b = transfer_16b_SPI1_Master(0xFFFF);
			
			temp = (float) (numRead16b >> 2) / 4;
			
			//numRead8b = transfer_8b_SPI1_Master(0xFF);
			// alternatively, just read two byte from peripheral
			// numRead16b = receive_16b_SPI1_Master();
			// disable SPI on the peripheral
			SPI_SlaveDeselect();
			// sprintf(serial_string_out1, "Value: %u \r\n", numRead8b);
			// UART_StringOut(serial_string_out1);
			// delay_ms(500);
		}

		// are we testing I2C communication?
		if(testI2C) {
			// collect a temperate and relative humidity measurement
			SHTC3_measure(Temp_RH);
			// output the results to the UART
			sprintf(serial_string_out1, "Temp: %0.2f \r\n", Temp_RH[0]);
			UART_StringOut(serial_string_out1);
		}
		
		// are we testing UART communication?
		if(testUART) {
			// send characters by UART, time the delay
			UART_CharOut('A');
			UART_CharOut('B');
			UART_CharOut('\r');
			UART_CharOut('\n');
			delay_ms(500);
			sprintf(serial_string_out1, "%u us\r\n", TIM4->CNT);
			UART_StringOut(serial_string_out1);
		}
		
		// LED off
		LEDToggle(GPIO_Pin_9);
		//delay_ms(1000);
	}
}


