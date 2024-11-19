#ifndef __MRE6183_ENCODER_H			//Define to prevent recursive inclusion
#define __MRE6183_ENCODER_H

#include "stm32f10x.h"

// extern int32_t num_full_rotations;

void Encoder_Configuration(void);
void Encoder_ResetValue(void);
uint32_t Encoder_ReadValue(void);
float Encoder_ReadValueRad(void);
void Encoder_Enable(void);
void Encoder_Disable(void);

#endif /*__MRE6183_ENCODER_H */
