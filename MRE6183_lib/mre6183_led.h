#ifndef __MRE6183_LED_H			//Define to prevent recursive inclusion
#define __MRE6183_LED_H

#include "stm32f10x.h"

void LED_PC8_Init(void);
void LED_PC9_Init(void);
void LEDToggle(uint32_t LEDn);

#endif /*__MRE6183_LED_H */
