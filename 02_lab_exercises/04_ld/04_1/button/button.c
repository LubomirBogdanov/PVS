#include "button.h"
#include "printf.h"

#ifdef CONFIG_BUTTON_ENABLED

/*!
 * \var rcc_ahb1enr
 * \brief A variable that corresponds to the RCC_AHB1ENR register.
 */
 static volatile uint32_t *rcc_ahb1enr = (volatile uint32_t *)0x40023830;

 /*!
  * \var rcc_apb2enr
  * \brief A variable that corresponds to the RCC_APB2ENR register.
  */
  static volatile uint32_t *rcc_apb2enr = (volatile uint32_t *)0x40023844;

 /*!
  * \var syscfg_exticr0
  * \brief A variable that corresponds to the SYSCFG_EXTICR1 register.
  */
  static volatile uint32_t *syscfg_exticr1 = (volatile uint32_t *)0x40013808;

 /*!
  * \var exti_imr
  * \brief A variable that corresponds to the EXTI_IMR register.
  */
 static volatile uint32_t *exti_imr = (volatile uint32_t *)0x40013C00;

 /*!
  * \var exti_emr
  * \brief A variable that corresponds to the EXTI_EMR register.
  */
 static volatile uint32_t *exti_emr = (volatile uint32_t *)0x40013C04;

 /*!
  * \var exti_rtsr
  * \brief A variable that corresponds to the EXTI_RTSR register.
  */
 static volatile uint32_t *exti_rtsr = (volatile uint32_t *)0x40013C08;

 /*!
  * \var exti_ftsr
  * \brief A variable that corresponds to the EXTI_FTSR register.
  */
 static volatile uint32_t *exti_ftsr = (volatile uint32_t *)0x40013C0C;

 /*!
  * \var gpio_a_moder
  * \brief A variable that corresponds to the GPIOA_MODER register.
  */
 static volatile uint32_t *gpio_a_moder = (volatile uint32_t *)0x40020000;

 /*!
  * \var gpio_a_idr
  * \brief A variable that corresponds to the GPIOA_IDR register.
  */
 static volatile uint32_t *gpio_a_idr = (volatile uint32_t *)0x40020010;

 /*!
  * \var gpio_a_pupdr
  * \brief A variable that corresponds to the GPIOA_PUPDR register.
  */
 static volatile uint32_t *gpio_a_pupdr = (volatile uint32_t *)0x4002000C;

 /*!
  * \var nvic_iser0
  * \brief A variable that corresponds to the NVIC ISER (interrupt set-enable) register.
  */
 static volatile uint32_t *nvic_iser0 = (volatile uint32_t *)0xE000E100;

 /*!
  * \var nvic_icer0
  * \brief A variable that corresponds to the NVIC ICER (interrupt clear-enable) register.
  */
 static volatile uint32_t *nvic_icer0 = (volatile uint32_t *)0xE000E180;

 /*!
  * \var nvic_ipr0
  * \brief A variable that corresponds to the NVIC IPR (interrupt priority) register.
  */
 static volatile uint32_t *nvic_ipr1 = (volatile uint32_t *)0xE000E404;

 /*!
  * \var exti_pr
  * \brief A variable that corresponds to the EXTI (external interrupt/event controller)
  *  PR (pending) register.
  */
 static volatile uint32_t *exti_pr = (volatile uint32_t *)0x40013C14;

 /*!
  * \var scb_aircr
  * \brief A variable that corresponds to the SCB (system control block)
  *  AIRCR (application interrupt and reset control register) register.
  */
 static volatile uint32_t *scb_aircr = (volatile uint32_t *)0xE000ED0C;


 void exti0_line_interrupt_handler(void){
	 *exti_pr |= EXTI_PR_LINE0; //Bit cleared when written to 1!
	 printf("Hello!\n");
 }

 /*!
  * \brief This function initializes a single pin
  * from a GPIO port where a button is connected.
  *
  * To do an init, first the clock to the GPIO module
  * has to be enabled. Second, the respective GPIO has
  * to be configured as an INPUT. Third, the input
  * pull-down must be enabled (optional - the board
  * has external pull down).
  *
  * \return None.
  */
void button_init(void){
	*rcc_ahb1enr |= RCC_AHB1ENR_GPIOA_EN; //Enable GPIOA module
	*gpio_a_moder &= ~GPIOA_MODER_PA0; //PA0 - input
	*gpio_a_pupdr |= GPIOA_PUPDR_PA0; //PA0 - enable pull-down (not needed for this board)


	*rcc_apb2enr |= RCC_APB2ENR_SYSCFGEN; //Enable SYSCFG module
	*syscfg_exticr1 &= ~SYSCFG_EXTICR0; //EXTI0 is connected to PA0 pin
	*exti_imr |= EXTI_IMR_BIT0_MASK; //EXTI enable interrupt request for line 0
	*exti_emr &= ~EXTI_EMR_BIT0_MASK; //EXTI disable event request for line 0
	*exti_rtsr |= EXTI_RTSR_BIT0_MASK; //EXTI enable rising edge
	*exti_ftsr &= ~EXTI_FTSR_BIT0_MASK; //EXTI disable falling edge



	*scb_aircr = SCB_AIRCR_PRIO_GRP_4; //4 bits preemption priority, 0 bits for sub-priority

	*nvic_ipr1 = NVIC_IPR1_EXTI0; //Set the interrupt priority for EXTI Line0, preempt level 2, sub-priority = 0

	*nvic_iser0 |= NVIC_ISER0_EXTI0; //Enable interrupt on EXTI Line0, vector
	                                 //number 6 from the vector table

}

/*!
 * \brief This function sets a logic
 * high on the respective pin.
 *
 * \return 1 - the button has been pressed, 0 - the button
 * is released.
 */
uint8_t button_read(void){
	return *gpio_a_idr & GPIOA_IDR_PA0;
}

#endif
