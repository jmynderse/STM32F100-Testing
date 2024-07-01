#include "../MRE6183_lib/mre6183_SHTC3.h"

// I started to write this as a sensor structure and gave up
//void SHTC3_init(SHTC3 *sensor) {
//	sensor->_sensorID = 0x03C1;
//}

// send a command to the SHTC3 sensor
// the command will always be 2 bytes
void SHTC3_write_command(uint16_t command) {
	uint8_t command_bytes[3];
	
	command_bytes[0] = (command >> 8) & 0xFF;
	command_bytes[1] = (command >> 0) & 0xFF;
	I2C1_send_Xb(SHTC3_DEFAULT_ADDR, command_bytes, 2);
}

// send a command to the SHTC3 sensor then read the response
// the command will always be 2 bytes
// the response may vary in size
// return data provided in the data array
void SHTC3_read_command(uint16_t command, uint8_t *data, uint32_t num_byte) {
	uint8_t command_bytes[3];
	
	command_bytes[0] = (command >> 8) & 0xFF;
	command_bytes[1] = (command >> 0) & 0xFF;
	I2C1_send_Xb(SHTC3_DEFAULT_ADDR, command_bytes, 2);
	I2C1_receive_Xb(SHTC3_DEFAULT_ADDR, data, num_byte);
}

void SHTC3_init(void) {
		uint8_t data[3];
	
		// initialize the I2C peripheral
		I2C1_PeripheralInit();
	
		// SHTC3-specific startup procedure: reset, wake-up, verify ID, sleep
		SHTC3_write_command(SHTC3_SOFTRESET);
		SHTC3_write_command(SHTC3_WAKEUP);
		SHTC3_read_command(SHTC3_READID, data, 3);
		// using the ID number display to validate against the logic analyzer
		//sprintf(serial_string_out1, "Received: %02x \t %02x \t %02x \r\n", data[0], data[1], data[2]);
		//UART_StringOut(serial_string_out1);
		SHTC3_write_command(SHTC3_SLEEP);
}


// complete a measurement with the SHTC3 sensor
// the process is specified in the datasheet: wake-up, measure, sleep
// measurement data is 6 bytes
// 2 bytes of temp, 1 byte of temp checksum
// 2 bytes of RH, 1 byte of RH checksum
// conversion provided in the datasheet
void SHTC3_measure(float *Temp_RH) {
	uint8_t data[6];
	uint16_t raw_Temp;
	uint16_t raw_RH;
	
	SHTC3_write_command(SHTC3_WAKEUP);
	SHTC3_read_command(SHTC3_NORMAL_MEAS_TFIRST_STRETCH, data, 6);
	SHTC3_write_command(SHTC3_SLEEP);
	
	raw_Temp = (data[0] << 8) | data[1];
	raw_RH = (data[3] << 8) | data[4];
	
	Temp_RH[0] = -45 + 175*raw_Temp/65536;
	Temp_RH[1] = 100*raw_RH/65536;
}
