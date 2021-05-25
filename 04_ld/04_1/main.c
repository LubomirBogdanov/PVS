#include "autoconf.h"
#include "main.h"
#include "led.h"
#include "uart.h"
#include "printf.h"
#ifdef CONFIG_BUTTON_ENABLED
#include "button.h"
#endif


#define SCB_CPACR_CP10_CP11_EN	0xF00000

volatile uint32_t *scb_cpacr = (volatile uint32_t *)0xE000ED88;

extern double asm_rms(double dig3, double dig2, double dig1, double dig0);

void my_func_3(void){
	volatile int a = 3, b = 30, c;

	c = a + b;
}

void my_func_2(void){
	volatile int a = 2, b = 20, c;

	c = a + b;

	my_func_3();
}

void my_func_1(void){
	volatile int a = 1, b = 10, c;

	c = a + b;

	my_func_2();
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
	double val3 = 4.002503;
	double val2 = 4.233456;
	double val1 = 4.789317;
	double val0 = 4.220734;
	double my_result;
	volatile int i;
	char str[4] = {'A', 'B', 'C', '\0'};

	*scb_cpacr |= SCB_CPACR_CP10_CP11_EN; //Enable ARM's FPU

	led_init();
	uart_init(CONFIG_UART_BAUD_RATE);
#ifdef CONFIG_BUTTON_ENABLED
	button_init();
#endif

	my_result = asm_rms(val3, val2, val1, val0);

	//Expected output: 4.321294158
	printf("The result is --> %12.6f\n", my_result);

	my_func_1();

	while (1){
		led_set();
		for(i = 0; i < 50000; i++){ }
		led_clear();
		for(i = 0; i < 50000; i++){ }
	}
}

