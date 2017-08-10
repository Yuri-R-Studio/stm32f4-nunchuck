/*
 * led.h
 *
 *  Created on: 8Aug.,2017
 *      Author: yuri
 */

#ifndef LED_H_
#define LED_H_

#define Green_Pin GPIO_PIN_12
#define Green_GPIO_Port GPIOD
#define Orange_Pin GPIO_PIN_13
#define Orange_GPIO_Port GPIOD
#define Red_Pin GPIO_PIN_14
#define Red_GPIO_Port GPIOD
#define Blue_Pin GPIO_PIN_15
#define Blue_GPIO_Port GPIOD

void led_task(void const * argument);

#endif /* LED_H_ */
