#ifndef __LED_H
#define __LED_H

#include "gpio_driver.h"
#include "TM4C123GH6PM.h"

#define GPIO_LED_PORT					GPIOF
#define GPIO_SW_PORT					GPIOF

#define GPIOF_PIN_0 					0
#define GPIOF_PIN_1 					1
#define GPIOF_PIN_2						2
#define GPIOF_PIN_3 					3
#define GPIOF_PIN_4 					4

#define GPIO_SW2_PIN 					GPIOF_PIN_0
#define LED_RED		 						GPIOF_PIN_1
#define LED_BLUE 							GPIOF_PIN_2
#define LED_GREEN	 				    GPIOF_PIN_3
#define GPIO_SW1_PIN 					GPIOF_PIN_4

void led_init(void);

void sw_init(void);

void led_turn_on(GPIOA_Type *GPIOx, uint16_t pin);

void led_turn_off(GPIOA_Type *GPIOx, uint16_t pin);

void led_toggle(GPIOA_Type *GPIOx, uint16_t pin);

#endif
