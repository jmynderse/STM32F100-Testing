#include "stm32f4xx.h"
#include "../MRE6183_lib/mre6183_HAL_basics.h"
#include "../MRE6183_lib/mre6183_HAL_led.h"
#include "../MRE6183_lib/mre6183_HAL_delays.h"

int main(void) {

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	/* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  LED_Init();
  TimerDelay_Init();
	
	while(1) {
		LED_Toggle();
    delay_ms(500);
	}
}
