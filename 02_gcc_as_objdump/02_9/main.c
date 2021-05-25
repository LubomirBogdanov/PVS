#include "autoconf.h"
#include "main.h"
#include "led.h"
#include "uart.h"
#include "printf.h"

#define SCB_CPACR_CP10_CP11_EN	0xF00000

volatile uint32_t *scb_cpacr = (volatile uint32_t *)0xE000ED88;

/*!
 * \brief The main entry of the application.
 * This function initializes a GPIO and a UART,
 * then outputs a message over the UART and
 * starts blinking the diode.
 *
 * \return None.
 */
int main(void){
	float my_fpu_array_1[10] = {4.00, 16.00, 3.504, 350.768, 1.14, 1256.13, 4096.00, 56.1212, 98.111, 311.256};
	float my_sqrt[10];
	volatile int i;

	*scb_cpacr |= SCB_CPACR_CP10_CP11_EN; //Enable ARM's FPU

	led_init();
	uart_init(CONFIG_UART_BAUD_RATE);

	for(i = 0; i < 10; i++){
... Place in-line assembly here ...
	}

	//Expected output:
	//2, 4, 1.871897433, 18.72880135, 1.067707825, 35.441924327, 64, 7.491408412, 9.905099697, 17.64244881
	printf("\nmy_sqrt[] = {");
	for(i = 0; i < 10; i++){
		printf("%12.6f, ", my_sqrt[i]);
	}
	printf("}\n");


	while (1){
		led_set();
		for(i = 0; i < 50000; i++){ }
		led_clear();
		for(i = 0; i < 50000; i++){ }
	}
}

