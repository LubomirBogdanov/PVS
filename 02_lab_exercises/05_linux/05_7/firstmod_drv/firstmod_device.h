#ifndef __FIRSTMOD_H
#define __FIRSTMOD_H

#define FIRSTMOD_NAME "my_first_led_module"

#define RCC_START_ADDRESS 0x40023800
#define RCC_END_ADDRESS   0x40023BFF

#define GPIOA_START_ADDRESS 0x40020000
#define GPIOA_END_ADDRESS   0x400203FF

//EXTI0 <- button
#define FIRSTMOD_DEV_IRQNUM   22

#endif
