#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

#define GPIOA_MODER_AF_PA9				0x80000
#define GPIOA_OTYPER_PA9 				0x200
#define GPIOA_MODER_AF_PA10				0x200000
#define GPIOA_AF7_PA9					0x70
#define GPIOA_AF7_PA10					0x700

#define RCC_AHB1ENR_GPIOA_EN			0x01
#define RCC_APB2_ENR_USART1_EN			0x10

#define USART1_UE						0x01
#define USART1_RE						0x04
#define USART1_TE						0x08
#define USART1_REACK					0x400000
#define USART1_TEACK					0x200000
#define USART1_TXE						0x80

void uart_init(uint32_t baud_rate);
void uart_send_buff(uint8_t *buff, uint8_t buff_size);
void _putchar(char character);

#endif
