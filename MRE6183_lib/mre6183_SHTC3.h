#ifndef __MRE6183_SHTC3_H			//Define to prevent recursive inclusion
#define __MRE6183_SHTC3_H

#include "stm32f10x_conf.h"
#include "../MRE6183_lib/mre6183_i2c.h"

#define SHTC3_DEFAULT_ADDR 0x70 /**< SHTC3 I2C Address */
#define SHTC3_NORMAL_MEAS_TFIRST_STRETCH                                       \
  0x7CA2 /**< Normal measurement, temp first with Clock Stretch Enabled */
#define SHTC3_LOWPOW_MEAS_TFIRST_STRETCH                                       \
  0x6458 /**< Low power measurement, temp first with Clock Stretch Enabled */
#define SHTC3_NORMAL_MEAS_HFIRST_STRETCH                                       \
  0x5C24 /**< Normal measurement, hum first with Clock Stretch Enabled */
#define SHTC3_LOWPOW_MEAS_HFIRST_STRETCH                                       \
  0x44DE /**< Low power measurement, hum first with Clock Stretch Enabled */

#define SHTC3_NORMAL_MEAS_TFIRST                                               \
  0x7866 /**< Normal measurement, temp first with Clock Stretch disabled */
#define SHTC3_LOWPOW_MEAS_TFIRST                                               \
  0x609C /**< Low power measurement, temp first with Clock Stretch disabled */
#define SHTC3_NORMAL_MEAS_HFIRST                                               \
  0x58E0 /**< Normal measurement, hum first with Clock Stretch disabled */
#define SHTC3_LOWPOW_MEAS_HFIRST                                               \
  0x401A /**< Low power measurement, hum first with Clock Stretch disabled */

#define SHTC3_READID 0xEFC8    /**< Read Out of ID Register */
#define SHTC3_SOFTRESET 0x805D /**< Soft Reset */
#define SHTC3_SLEEP 0xB098     /**< Enter sleep mode */
#define SHTC3_WAKEUP 0x3517    /**< Wakeup mode */

//typedef struct {
//	int _sensorID;
//	float _temperature, _humidity;
//} SHTC3;

void SHTC3_init(void);
void SHTC3_write_command(uint16_t command);
void SHTC3_read_command(uint16_t command, uint8_t *data, uint32_t num_byte);
void SHTC3_measure(float *Temp_RH);

//uint16_t SHTC3_readID(SHTC3 *sensor) {
	
//}

#endif /*__MRE6183_SHTC3_H */
