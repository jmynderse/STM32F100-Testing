#include "stm32f10x_conf.h"
#include "../MRE6183_lib/mre6183_led.h"
#include "../MRE6183_lib/mre6183_delays.h"
#include "../MRE6183_lib/mre6183_uart.h"
#include "../MRE6183_lib/mre6183_spi.h"
#include "../MRE6183_lib/mre6183_i2c.h"
#include "../MRE6183_lib/mre6183_SHTC3.h"
#include "../MRE6183_lib/mre6183_LIS3DH.h"
#include <stdio.h>
#include <string.h>

uint16_t numRead16b = 0;
float temp = 0;

bool testSPI = 1;
bool testI2C = 0;
bool testUART = 0;
char serial_char_out;
char serial_string_out1[UART_BUFFER_SIZE];

int main(void) {
//	uint8_t numRead8b = 0;
//	uint16_t numRead16b = 0;
//	uint8_t data[3];
//	uint16_t received_data;
	float Temp_RH[2];
	float Accel[3];
	
	LED_PC8_Init();
	LED_PC9_Init();
	UART_Init();
	TimerDelay_Init();
	
	if (testSPI) {
		// initialize the sensor
		LIS3DH_init();
	}
	
	LEDToggle(GPIO_Pin_8);
	
	// initialization using I2C
	// this is very specific to the SHTC3 temperature and relative humidity sensor used in my testing
	if (testI2C) {
		SHTC3_init();
	}
	
	while(1) {
		// LED on
		LEDToggle(GPIO_Pin_9);
		
		// are we testing SPI communication?
		if(testSPI) {
			LIS3DH_measure(Accel);
			sprintf(serial_string_out1, "X: %0.2f \t\t Y: %0.2f \t\t Z: %0.2f \r\n", Accel[0], Accel[1], Accel[2]);
			UART_StringOut(serial_string_out1);
			delay_ms(500);
		}

		// are we testing I2C communication?
		if(testI2C) {
			// collect a temperate and relative humidity measurement
			SHTC3_measure(Temp_RH);
			// output the results to the UART
			sprintf(serial_string_out1, "Temp: %0.2f \t\t Humidity: %0.2f \r\n", Temp_RH[0], Temp_RH[1]);
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


