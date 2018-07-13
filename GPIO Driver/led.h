#ifndef __LED_H
#define __LED_H

#include "gpio_driver.h"
#include "TM4C123GH6PM.h"

#define EXTIx_IRQn 						EXTI0_IRQn
#define EXTIx_IRQHandler      EXTI0_IRQHandler

#define GPIO_BUTTON_PIN				0
#define GPIO_LED_PORT					GPIOF

#define GPIOF_PIN_1 					1
#define GPIOF_PIN_2						2
#define GPIOF_PIN_3 					3

#define LED_RED		 						GPIOF_PIN_1
#define LED_BLUE 							GPIOF_PIN_2
#define LED_GREEN	 				    GPIOF_PIN_3

void led_init(void);

void led_turn_on(GPIOA_Type *GPIOx, uint16_t pin);

void led_turn_off(GPIOA_Type *GPIOx, uint16_t pin);

void led_toggle(GPIOA_Type *GPIOx, uint16_t pin);

#endif
