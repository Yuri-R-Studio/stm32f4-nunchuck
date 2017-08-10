/*
 * led.c
 *
 *  Created on: 8Aug.,2017
 *      Author: yuri
 */
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "led.h"


void led_task(void const * argument)
{
	//Simple blink led task
	for(;;){
		vTaskDelay(500);
		HAL_GPIO_TogglePin(GPIOD, Blue_Pin);
	}
}
