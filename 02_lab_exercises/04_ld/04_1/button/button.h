#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>
#include "autoconf.h"

#ifdef CONFIG_BUTTON_ENABLED

/*!
 * \def RCC_AHB1ENR_GPIOA_EN
 *
 * \brief A bitmask for the GPIO_EN bit.
 */
#define RCC_AHB1ENR_GPIOA_EN	0x01

/*!
 * \def GPIOA_MODER_PA0
 *
 * \brief A bitmask for the MODER_PA0 bits.
 */
#define GPIOA_MODER_PA0			0x03

/*!
 * \def GPIOA_IDR_PA0
 *
 * \brief A bitmask for the IDR_PA0 bit.
 */
#define GPIOA_IDR_PA0			0x01

/*!
 * \def GPIOA_PUPDR_PA0
 *
 * \brief A bitmask for the PUPDR_PA0 bits.
 */
#define GPIOA_PUPDR_PA0			0x02

/*!
 * \def NVIC_ISER0_EXTI0
 *
 * \brief A bitmask for the EXTI0 interrupt set enable (in NVIC module).
 */
#define NVIC_ISER0_EXTI0		0x40

/*!
 * \def NVIC_ICER0_EXTI0
 *
 * \brief A bitmask for the EXTI0 interrupt clear enable (in NVIC module).
 */
#define NVIC_ICER0_EXTI0		0x40

/*!
 * \def NVIC_IPR1_EXTI0
 *
 * \brief A value for the highest priority of the
 * EXTI0 interrupt line.
 */
#define NVIC_IPR1_EXTI0			0x00200000

/*!
 * \def EXTI_PR_LINE0
 *
 * \brief A bitmask for the EXTI0 interrupt line (in EXTI module).
 */
#define EXTI_PR_LINE0			0x01UL

/*!
 * \def EXTI_PR_LINE0
 *
 * \brief A bitmask to configure priority group bits from
 * the AIRCR register in the SCB module. Currently
 * 4 bits are selected for preemption priority
 * and 0 bits for subpriority.
 */
#define SCB_AIRCR_PRIO_GRP_4	0xFA050300

/*!
 * \def RCC_APB2ENR_SYSCFGEN
 *
 *
 * \brief A bitmask for the RCC APB2ENR register to enable the clock to the SYSCFG module.
 */
#define RCC_APB2ENR_SYSCFGEN	0x4000UL

/*!
 * \def SYSCFG_EXTICR0
 *
 * \brief A bitmask for the SYSCFG EXTICR0 register to connect EXTI0 line to PA0 pin.
 */
#define SYSCFG_EXTICR0			0x0000000FUL

/*!
 * \def EXTI_IMR_BIT0_MASK
 *
 * \brief A bitmask for the EXTI IMR register to make the specific interrupt request NOT masked.
 */
#define EXTI_IMR_BIT0_MASK		0x0000001FUL

/*!
 * \def EXTI_EMR_BIT0_MASK
 *
 * \brief A bitmask for the EXTI EMR register to make the specific event request NOT masked.
 */
#define EXTI_EMR_BIT0_MASK		0x0000001FUL

/*!
 * \def EXTI_RTSR_BIT0_MASK
 *
 * \brief A bitmask for the EXTI RTSR register to make the specific trigger on rising edge.
 */
#define EXTI_RTSR_BIT0_MASK		0x0000001FUL

/*!
 * \def EXTI_FTSR_BIT0_MASK
 *
 * \brief A bitmask for the EXTI FTSR register to make the specific trigger on falling edge.
 */
#define EXTI_FTSR_BIT0_MASK		0x0000001FUL

void button_init(void);
uint8_t button_read(void);

#endif

#endif
