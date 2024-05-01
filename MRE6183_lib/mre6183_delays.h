#ifndef __MRE6183_DELAYS_H			//Define to prevent recursive inclusion
#define __MRE6183_DELAYS_H

#include "stm32f10x_conf.h"

void Delay(__IO uint32_t nCount);
void TimerDelay_Init(void);
void delay_us(uint16_t delay_time_us);
void delay_ms(uint16_t delay_time_ms);
void TimerInterrupt_Init(uint16_t timer_freq);

#endif /*__MRE6183_DELAYS_H */
