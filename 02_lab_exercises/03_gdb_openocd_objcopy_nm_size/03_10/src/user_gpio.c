/*
 * user_gpio.c
 *
 *  Created on: Nov 10, 2020
 *      Author: lbogdanov
 */
#include "user_gpio.h"

void EXTI0_IRQHandler(void){
  HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if (GPIO_Pin == GPIO_PIN_0){
    BSP_LED_Toggle(LED2);
  }
}

void button_init(void){
	GPIO_InitTypeDef gpio_config;

	BSP_LED_Init(LED2);

	/* Configure PC.13 pin as input floating */
	gpio_config.Mode = GPIO_MODE_IT_RISING;
	gpio_config.Pull = GPIO_NOPULL;
	gpio_config.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &gpio_config);
	/* Enable and set EXTI line 0 Interrupt to the lowest priority */
	HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

