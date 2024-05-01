#include "../MRE6183_lib/mre6183_led.h"

void LED_PC8_Init(void) {
	// create init structs
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// initialize init structs
  GPIO_StructInit(&GPIO_InitStructure);
	
	// enable the clock for GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	// setup gpio for led on port C pin 8 as output, push-pull, and 2 MHz
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void LED_PC9_Init(void) {
	// create init structs
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// initialize init structs
  GPIO_StructInit(&GPIO_InitStructure);
	
	// enable the clock for GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	// setup gpio for led on port C pin 9 as output, push-pull, and 2 MHz
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

// call function with LEDToggle(GPIO_Pin_8) or LEDToggle(GPIO_Pin_9)
void LEDToggle(uint32_t LEDn) {
	//read previous state
	if(GPIO_ReadOutputDataBit(GPIOC, LEDn)) {
		GPIO_ResetBits(GPIOC, LEDn); //set to zero
	}
	else {
	  GPIO_SetBits(GPIOC, LEDn); //set to one
	}
}
