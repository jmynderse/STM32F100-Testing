#ifndef __MRE6183_PWM_H			//Define to prevent recursive inclusion
#define __MRE6183_PWM_H

#include "stm32f10x.h"

void PWM_Configuration(void);
void PWM_SetValue(uint32_t PWM_value);
void PWM_Enable(void);
void PWM_Disable(void);

#endif /*__MRE6183_PWM_H */
