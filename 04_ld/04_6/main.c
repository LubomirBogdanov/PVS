#include <string.h>
#include "stm32f7xx_hal.h"
#include "stm32f769i_discovery.h"
#include "standard_rms.h"

UART_HandleTypeDef uart_init_struct;

void systick_handler(void){
	HAL_IncTick();
}

/*!
  *\brief  System Clock Configuration
  *
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            PLL_R                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * \param  None
  * \return None
  */
void hal_clocks_init(void){
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef  ret = HAL_OK;

  __HAL_RCC_PWR_CLK_ENABLE();

  //The voltage scaling allows optimizing the power consumption when the device is
  //clocked below the maximum system frequency, to update the voltage scaling value
  //regarding system frequency refer to product datasheet.
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  //Enable HSE Oscillator and activate PLL with HSE as source
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 7;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK){
    while(1) { }
  }

  //Activate the OverDrive to reach the 216 MHz Frequency
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK){
    while(1) { }
  }

  //Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK){
    while(1) { }
  }
}

void exti0_line_interrupt_handler(void){
  HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if (GPIO_Pin == GPIO_PIN_0){
    BSP_LED_Toggle(LED2);
  }
}

void hal_button_init(void){
	GPIO_InitTypeDef gpio_config;

	BSP_LED_Init(LED2);
	BSP_LED_Init(LED1);

	// Configure PC.13 pin as input floating
	gpio_config.Mode = GPIO_MODE_IT_RISING;
	gpio_config.Pull = GPIO_NOPULL;
	gpio_config.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &gpio_config);
	// Enable and set EXTI line 0 Interrupt to the lowest priority
	HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart){
  GPIO_InitTypeDef gpio_init_struct;

  // Enable GPIO TX/RX clock
  __GPIOA_CLK_ENABLE();

  // Enable USART1 clock
  __USART1_CLK_ENABLE();

  //UART TX GPIO pin configuration
  gpio_init_struct.Pin = GPIO_PIN_9;
  gpio_init_struct.Mode = GPIO_MODE_AF_PP;
  gpio_init_struct.Pull = GPIO_PULLUP;
  gpio_init_struct.Speed = GPIO_SPEED_HIGH;
  gpio_init_struct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOA, &gpio_init_struct);

  /* UART RX GPIO pin configuration  */
  gpio_init_struct.Pin = GPIO_PIN_10;
  HAL_GPIO_Init(GPIOA, &gpio_init_struct);
}

void hal_uart_init(void){
	uart_init_struct.Instance = USART1;
	uart_init_struct.Init.BaudRate = 9600;
	uart_init_struct.Init.WordLength = UART_WORDLENGTH_8B;
	uart_init_struct.Init.StopBits = UART_STOPBITS_1;
	uart_init_struct.Init.Parity = UART_PARITY_NONE;
	uart_init_struct.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart_init_struct.Init.Mode = UART_MODE_TX_RX;

	HAL_UART_Init(&uart_init_struct);
}

int main(void){
	double val3 = 4.002503;
	double val2 = 4.233456;
	double val1 = 4.789317;
	double val0 = 4.220734;
	double my_result;
	char *my_msg = "HAL example using GPIO and UART\n";
	volatile int i;
	GPIO_InitTypeDef gpio_config;

    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  //Enable ARM's FPU

	SCB_EnableDCache();
	SCB_EnableICache();

	HAL_Init();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	hal_clocks_init();
	hal_button_init();
	hal_uart_init();

	//Expected output: 4.321294158
	//Use debugger to inspect the "my_result" variable
	my_result = standard_rms(val3, val2, val1, val0);

	HAL_UART_Transmit(&uart_init_struct, (uint8_t*)my_msg, strlen(my_msg), 100);

	while (1){
		BSP_LED_Toggle(LED1);
		HAL_Delay(50);
	}
}
