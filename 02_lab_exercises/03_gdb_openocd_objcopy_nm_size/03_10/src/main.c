/*
 * main.c
 *
 *  Created on: Nov 10, 2020
 *      Author: lbogdanov
 */
#include "main.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"

#ifdef __GNUC__
#define USED __attribute__((used))
#else
#define USED
#endif

const int USED uxTopUsedPriority = configMAX_PRIORITIES - 1;

void SysTick_Handler(void){
	osSystickHandler();
}

void led_thread_1(void const *argument){
	(void) argument;

	while(1){
		BSP_LED_Toggle(LED1);
		osDelay(200);
	}
}

void led_thread_2(void const *argument){
	(void) argument;

	while (1){
		BSP_LED_Toggle(LED2);
		osDelay(500);
	}
}

int main(void){
	SCB_EnableDCache();
	SCB_EnableICache();

	HAL_Init();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	clocks_init();
	button_init();
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);

	osThreadDef(led_1, led_thread_1, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	osThreadDef(led_2, led_thread_2, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);

	osThreadCreate(osThread(led_1), NULL);
	osThreadCreate(osThread(led_2), NULL);

	//Start the FreeRTOS scheduler.
	osKernelStart();

	while(1){ }

	return 0;
}
