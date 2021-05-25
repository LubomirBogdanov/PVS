#include <stdlib.h>
#include "autoconf.h"
#include "main.h"
#include "led.h"
#include "uart.h"
#include "printf.h"
#include "standard_rms.h"
#ifdef CONFIG_BUTTON_ENABLED
#include "button.h"
#endif

#define SCB_CPACR_CP10_CP11_EN	0xF00000

volatile uint32_t *scb_cpacr = (volatile uint32_t *)0xE000ED88;

unsigned long start_of_heap_get(void){
	extern char _sheap; /* Defined by the linker */
	return (unsigned long)&_sheap;
}

/*!
 * \brief The main entry of the application.
 * This function initializes a GPIO and a UART,
 * then outputs a message over the UART and
 * starts blinking a diode.
 *
 * \return None.
 */
int main(void){
	const uint32_t static_arr_1[10] = {0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777, 0x88888888, 0x99999999, 0xaaaaaaaa};
	const uint16_t static_arr_2[4] = {0xbaba, 0xdede, 0xcece, 0xfefe};
	const uint8_t static_arr_3[4] = {0xde, 0xad, 0xbe, 0xef};
	uint32_t *dynamic_arr_1;
	uint16_t *dynamic_arr_2;
	uint8_t *dynamic_arr_3;
	volatile int i;

	*scb_cpacr |= SCB_CPACR_CP10_CP11_EN; //Enable ARM's FPU

	led_init();
	uart_init(CONFIG_UART_BAUD_RATE);
#ifdef CONFIG_BUTTON_ENABLED
	button_init();
#endif

	printf("The start of the HEAP segment: 0x%08X\n", start_of_heap_get());

	dynamic_arr_1 = (uint32_t *)malloc(sizeof(uint32_t)*10);

	for(i = 0; i < 10; i++){
		dynamic_arr_1[i] = static_arr_1[i];
	}

	dynamic_arr_2 = (uint16_t *)malloc(sizeof(uint16_t)*4);

	for(i = 0; i < 4; i++){
		dynamic_arr_2[i] = static_arr_2[i];
	}

	printf("\n dynamic_arr_1[ ] = {\n");
	for(i = 0; i < 10; i++){
		printf("0x%08X\n", dynamic_arr_1[i]);
	}
	printf("}\n");

	printf("\n dynamic_arr_2[ ] = {\n");
	for(i = 0; i < 4; i++){
		printf("0x%04X\n", dynamic_arr_2[i]);
	}
	printf("}\n");

	free(dynamic_arr_1);

	dynamic_arr_3 = (uint8_t *)malloc(sizeof(uint16_t)*4);

	for(i = 0; i < 4; i++){
		dynamic_arr_3[i] = static_arr_3[i];
	}

	printf("\n dynamic_arr_3[ ] = {\n");
	for(i = 0; i < 4; i++){
		printf("0x%02X\n", dynamic_arr_3[i]);
	}
	printf("}\n");

	free(dynamic_arr_2);
	free(dynamic_arr_3);

	while (1){
		led_set();
		for(i = 0; i < 50000; i++){ }
		led_clear();
		for(i = 0; i < 50000; i++){ }
	}
}

