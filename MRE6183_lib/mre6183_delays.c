#include "../MRE6183_lib/mre6183_delays.h"

void Delay(__IO uint32_t nCount) {
	for(; nCount != 0; nCount--);
}

void TimerDelay_Init(void)
{
	// create init structs
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	// initialize init structs
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	// initialize the clock for TIM7
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	
	// set the prescaler to change the clock from the SystemCoreClock (24 MHz) to 1 MHz
  TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000000) - 1;
	// set the period to maximum (16-bit)
	TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	
	TIM_Cmd(TIM7, ENABLE);
}

void delay_us(uint16_t delay_time_us)
{
	TIM7->CNT = 0;
	while(TIM7->CNT < delay_time_us);
}

void delay_ms(uint16_t delay_time_ms)
{
	while(delay_time_ms > 0)
	{
		TIM7->CNT = 0;
		delay_time_ms--;
		while(TIM7->CNT < 1000);
	}
}


void TimerInterrupt_Init(uint16_t timer_freq)
{
	// create init structs
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// initialize init structs (not finding an equivalent for NVIC)
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	// initialize the clock for TIM6
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	// set the prescaler to change the clock from the SystemCoreClock (24 MHz) to the defined sample rate
  TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / timer_freq / 2000) - 1;
	// set the period to maximum (16-bit)
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM6, ENABLE);
}
