#include "../MRE6183_lib/mre6183_LIS3DH.h"
#include "../MRE6183_lib/mre6183_uart.h"
#include "../MRE6183_lib/mre6183_delays.h"
#include <stdio.h>

void LIS3DH_init(void) {
	volatile uint8_t val = 0;
	volatile uint8_t val1, val2, val3, val4;
	char serial_string_out1[50];
	
	// initialize the sensor select pin
	SPI1_PeripheralInit_SlaveSelect();
	// deselect the sensor
	SPI1_SlaveDeselect();
	// initialze the SPI communication in master mode
	SPI1_PeripheralInit_Master(SPI_CPOL_High, SPI_CPHA_2Edge);

	// read the current values in CTRL_REG4
	//val = LIS3DH_read_command(LIS3DH_REG_WHOAMI);
	val1 = LIS3DH_read_command(LIS3DH_REG_CTRL1);
	val4 = LIS3DH_read_command(LIS3DH_REG_CTRL4);
	sprintf(serial_string_out1, "%02X \t\t %02X \r\n", val1, val4);
	UART_StringOut(serial_string_out1);	
	
	// set the rate to 400 Hz and enable X, Y, Z axes
	val = 0;
	val |= LIS3DH_DATARATE_400_HZ << 4;
	val |= LIS3DH_AXIS_X;
	val |= LIS3DH_AXIS_Y;
	val	|= LIS3DH_AXIS_Z;
	sprintf(serial_string_out1, "%02X \r\n", val);
	UART_StringOut(serial_string_out1);	
	LIS3DH_write_command(LIS3DH_REG_CTRL1, val);
	
//	// read the current values in CTRL_REG4
//	val = LIS3DH_read_command(LIS3DH_REG_WHOAMI);
//	val1 = LIS3DH_read_command(LIS3DH_REG_CTRL1);
//	val4 = LIS3DH_read_command(LIS3DH_REG_CTRL4);
//	sprintf(serial_string_out1, "%02X \t\t %02X \t\t %02X \r\n", val, val1, val4);
//	UART_StringOut(serial_string_out1);	


	// combine with the desired range
	val4 |= (LIS3DH_RANGE_4_G << 4);
	//val2 = val1 | (LIS3DH_RANGE_2_G << 4);
	//val3 = val1 | (LIS3DH_RANGE_4_G << 4);
	LIS3DH_write_command(LIS3DH_REG_CTRL4, val4);
	
	// read the current values in CTRL_REG4
	//val = LIS3DH_read_command(LIS3DH_REG_WHOAMI);
	val1 = LIS3DH_read_command(LIS3DH_REG_CTRL1);
	val4 = LIS3DH_read_command(LIS3DH_REG_CTRL4);
	sprintf(serial_string_out1, "%02X \t\t %02X \r\n", val1, val4);
	UART_StringOut(serial_string_out1);	

}

// send a command to the LIS3DH sensor
// the command will always be 2 bytes: 1 register and 1 value
void LIS3DH_write_command(uint8_t reg, uint8_t val) {
	uint16_t command;
	uint8_t RW, MS;
	// set the read/write bit - read = 1, write = 0
	RW = (0 << 7);
	// set the address incfement bit - constant = 0, increment = 1
	MS = (0 << 6);
	// combine the RW, MS, and register
	reg = (RW | MS) | reg;
	
	command = (reg << 8) | (val << 0);
	// enable SPI on the peripheral
	SPI1_SlaveSelect();
	SPI1_send_16b_Master(command);
	// disable SPI on the peripheral
	SPI1_SlaveDeselect();
}

// send a command to the LIS3DH sensor then read the response
// the command will always be 1 byte: a regiser
// the response will always be 1 byte: a value
// return data provided in the data array
uint8_t LIS3DH_read_command(uint8_t reg) {
	uint8_t RW, MS, raw_data, output_data;
	volatile uint16_t raw_data1, raw_data2;
	
	// set the read/write bit - read = 1, write = 0
	RW = (1 << 7);
	// set the address incfement bit - constant = 0, increment = 1
	MS = (0 << 6);
	// combine the RW, MS, and register
	reg = (RW | MS) | reg;
	
	// enable SPI on the peripheral
	SPI1_SlaveSelect();
	raw_data = SPI1_send_8b_receive_8b_Master(reg);
	// disable SPI on the peripheral
	SPI1_SlaveDeselect();
	raw_data1 = ~(0xFF00) & raw_data;
	output_data = (uint8_t) raw_data1;
	return output_data;
}

void LIS3DH_measure(float *Accel) {
	uint8_t data[6];
	int16_t raw_Accel[3];
//	char serial_string_out1[50];
	
	// 1. read high register
	// 2. read low register
	// 3. combine high and low registers into 16-bit value
	data[0] = LIS3DH_read_command(LIS3DH_REG_OUT_X_H);
	data[1] = LIS3DH_read_command(LIS3DH_REG_OUT_X_L);
	raw_Accel[0] = (data[0] << 8) | data[1];  // x-axis
	data[2] = LIS3DH_read_command(LIS3DH_REG_OUT_Y_H);
	data[3] = LIS3DH_read_command(LIS3DH_REG_OUT_Y_L);
	raw_Accel[1] = (data[2] << 8) | data[3];  // y-axis
	data[4] = LIS3DH_read_command(LIS3DH_REG_OUT_Z_H);
	data[5] = LIS3DH_read_command(LIS3DH_REG_OUT_Z_L);
	raw_Accel[2] = (data[4] << 8) | data[5];	// z-axis
	
	// Convert 16-but value using (default) full-scale value (2g)
	Accel[0] = raw_Accel[0] / 32767.0 * 2;
	Accel[1] = raw_Accel[1] / 32767.0 * 2;
	Accel[2] = raw_Accel[2] / 32767.0 * 2;
}
