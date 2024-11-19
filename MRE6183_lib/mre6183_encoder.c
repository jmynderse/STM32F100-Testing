#include "../MRE6183_lib/mre6183_encoder.h"

// this is a hack, but set the inital value of the full 
// rotations to -1 so that the initial IRQ will increment 
// back to zero
int32_t num_full_rotations = -1;

void Encoder_Configuration(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  //NVIC structure to set up NVIC controller
  NVIC_InitTypeDef NVIC_InitStructure;
	
  // initialize init structs
	GPIO_StructInit(&GPIO_InitStructure);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

  // enable clock on GPIOA and TIM1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
												 RCC_APB2Periph_TIM1, ENABLE);
	
	// choose PA8 and PA9 for encoder (TIM1_CH1, TIM1_CH2)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 2000-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	// configure the encoder
	TIM_EncoderInterfaceConfig(TIM1,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
  
  // configure event
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM16_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void Encoder_ResetValue(void) {
  TIM1->CNT = 0;
}

uint32_t Encoder_ReadValue(void) {
  return TIM1->CNT;
}

float Encoder_ReadValueRad(void) {
  return num_full_rotations*2*3.14 + TIM1->CNT*(3.14/1000);
}

void Encoder_Enable(void) {
  TIM_Cmd(TIM1, ENABLE);
}

void Encoder_Disable(void) {
  TIM_Cmd(TIM1, DISABLE);
}

void TIM1_UP_TIM16_IRQHandler(void) {
  // determine if the rotation was CW or CCW
  if (TIM1->CNT == 0) {
    num_full_rotations += 1;
  }
  else {
    num_full_rotations -= 1;
  }
  // clear the interrupt call
  TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}
