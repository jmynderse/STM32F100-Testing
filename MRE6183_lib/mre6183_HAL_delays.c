#include "../MRE6183_lib/mre6183_HAL_delays.h"

void Delay(__IO uint32_t nCount) {
	for(; nCount != 0; nCount--);
}

void TimerDelay_Init(void)
{	
  // initialize the clock for TIM9
  __HAL_RCC_TIM9_CLK_ENABLE();
	
  // set the timer peripheral to use, can be any that aren't already in use
  h_delay_timer.Instance = TIM9;
	// set the prescaler to change the clock from the SystemCoreClock (24 MHz) to 1 MHz
  h_delay_timer.Init.Prescaler = (SystemCoreClock / 1000000) - 1;
	h_delay_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
  // set the period to max, we won't have any reason to hit it
	h_delay_timer.Init.Period = 65535;
  h_delay_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  h_delay_timer.Init.RepetitionCounter = 0x0000;
  h_delay_timer.Init.AutoReloadPreload = 0;
  // initialze the timer with the paramaters above
	HAL_TIM_Base_Init(&h_delay_timer);
  // start the timer
	HAL_TIM_Base_Start(&h_delay_timer);
}

void delay_us(uint16_t delay_time_us)
{
	h_delay_timer.Instance->CNT = 0;
	while(h_delay_timer.Instance->CNT < delay_time_us);
}

void delay_ms(uint16_t delay_time_ms)
{
	while(delay_time_ms > 0)
	{
		h_delay_timer.Instance->CNT = 0;
		delay_time_ms--;
		while(h_delay_timer.Instance->CNT < 1000);
	}
}

/*
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
*/
