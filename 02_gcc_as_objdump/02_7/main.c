#include "autoconf.h"
#include "main.h"
#include "led.h"
#include "uart.h"
#include "printf.h"

#if defined CONFIG_LED_BLINK_ITERATOR_50000
#define LED_BLINK 50000
#elif defined CONFIG_LED_BLINK_ITERATOR_200000
#define LED_BLINK 200000
#elif defined CONFIG_LED_BLINK_ITERATOR_400000
#define LED_BLINK 400000
#endif

/*!
 * \brief The main entry of the application.
 * This function initializes a GPIO and a UART,
 * then outputs a message over the UART and
 * starts blinking the diode.
 *
 * \return None.
 */
int main(void){
	double my_fpu_array_1[10] = {1.345239, 1.774125, 57865.412737, 0.850072, 16.142938, 4958.284851, 927.674384, 404.404121, 312.500056, 311.658473};
	double my_fpu_array_2[10] = {1.785993, 1.550390, 55000.328467, 0.851200, 19.372777, 4933.924562, 927.000993, 400.078463, 310.349112, 314.693125};
	double my_fpu_mean[10];
	volatile int i;

	led_init();
	uart_init(CONFIG_UART_BAUD_RATE);

	for(i = 0; i < 10; i++){
		my_fpu_mean[i] = (my_fpu_array_1[i] + my_fpu_array_2[i])/2;
	}

	//Expected output:
	//1.565616, 1.6622575, 56432.870602, 0.850636, 17.7578575, 4946.1047065, 927.3376885, 402.241292, 311.424584, 313.175799
	printf("\nmy_fpu_mean[] = {");
	for(i = 0; i < 10; i++){
		printf("%12.6f, ", my_fpu_mean[i]);
	}
	printf("}\n");


	while (1){
		led_set();
		for(i = 0; i < LED_BLINK; i++){ }
		led_clear();
		for(i = 0; i < LED_BLINK; i++){ }
	}
}

