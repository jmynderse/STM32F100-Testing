#include "../MRE6183_lib/mre6183_pwm.h"

/* PWM Configuration - assume PC8 */
void PWM_Configuration(void) {
	// create init structures
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	// initialize init structs
	GPIO_StructInit(&GPIO_InitStructure);

	/* Set the desired PWM frequency in Hz */
	uint32_t PWMFreq = 1000;
	/* Set the desired PWM max value (100%) */
	uint32_t PWMMaxValue = 1000;
	
	// enable clock on TIM3
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	// enable clock on GPIOC and AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	// setup GPIO for PC8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / PWMFreq / PWMMaxValue) -1;  
  TIM_TimeBaseStructure.TIM_Period = PWMMaxValue-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Channel 1 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
}


