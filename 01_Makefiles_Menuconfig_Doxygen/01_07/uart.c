#include "uart.h"

static volatile uint32_t *gpio_a_moder = (volatile uint32_t *)0x40020000;
static volatile uint32_t *gpio_a_otyper = (volatile uint32_t *)0x40020004;
static volatile uint32_t *gpio_afrh = (volatile uint32_t *)0x40020024;

static volatile uint32_t *apb2enr = (volatile uint32_t *)0x40023844;
static volatile uint32_t *ahb1enr = (volatile uint32_t *)0x40023830;

static volatile uint32_t *usart_1_cr1 = (volatile uint32_t *)0x40011000;
static volatile uint32_t *usart_1_brr = (volatile uint32_t *)0x4001100C;
static volatile uint32_t *usart_1_isr = (volatile uint32_t *)0x4001101C;
static volatile uint32_t *usart_1_tdr = (volatile uint32_t *)0x40011028;

void uart_init(uint32_t baud_rate){
	*ahb1enr |= RCC_AHB1ENR_GPIOA_EN;
	*gpio_a_moder |= GPIOA_MODER_AF_PA9; //PA9 - alternate function
	*gpio_a_moder |= GPIOA_MODER_AF_PA10; //PA10 - alternate function
	*gpio_a_otyper &= GPIOA_OTYPER_PA9; //PA9 - push-pull
	*gpio_afrh |= GPIOA_AF7_PA9; //PA9 = USART1_TX
	*gpio_afrh |= GPIOA_AF7_PA10; //PA10 = USART1_RX

	*apb2enr |= RCC_APB2_ENR_USART1_EN; //Enable USART1 clock
	*usart_1_cr1 |= (USART1_UE | USART1_RE | USART1_TE);

	//Upon reset: f_ck = APB2PRESC(/1) = AHBPRESC(/1) = HSI(16 MHz) => BRR = 16MHz / f_ck
	switch(baud_rate){
	case 9600:
		*usart_1_brr = 0x682;
		break;
	case 19200://ne
		*usart_1_brr = 0x341;
		break;
	case 38400:
		*usart_1_brr = 0x1A0;
		break;
	case 57600:
		*usart_1_brr = 0x115;
		break;
	case 115200:
		*usart_1_brr = 0x8A;
		break;
	}

	while(!(*usart_1_isr & USART1_REACK)){ }
	while(!(*usart_1_isr & USART1_TEACK)){ }
}

void uart_send_buff(uint8_t *buff, uint8_t buff_size){
	while(buff_size--){
		*usart_1_tdr = *buff++;
		while(!(*usart_1_isr & USART1_TXE)){ }
	}
}

void _putchar(char character){
	*usart_1_tdr = character;
	while(!(*usart_1_isr & USART1_TXE)){ }
}
