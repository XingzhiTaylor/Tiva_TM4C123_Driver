#ifndef SPI_APP_H
#define SPI_APP_H

#include "TM4C123GH6PM.h"

#define SSI0_PORT					GPIOA
#define SSI1_PORT					GPIOD

#define SSI0_IRQ_NBR				7
#define SSI1_IRQ_NBR				34

#define GPIOA_PIN_2					2
#define GPIOA_PIN_3					3
#define GPIOA_PIN_4					4
#define GPIOA_PIN_5					5

#define SPI_CLK							GPIOA_PIN_2
#define SPI_FSS							GPIOA_PIN_3
#define SPI_RX							GPIOA_PIN_4
#define SPI_TX							GPIOA_PIN_5

#define GPIO_LED_PORT					GPIOF
#define GPIO_SW_PORT					GPIOF

#define GPIOF_PIN_0 					0
#define GPIOF_PIN_1 					1
#define GPIOF_PIN_2						2
#define GPIOF_PIN_3 					3
#define GPIOF_PIN_4 					4

#define GPIO_SW2_PIN 				GPIOF_PIN_0
#define LED_RED		 					GPIOF_PIN_1
#define LED_BLUE 						GPIOF_PIN_2
#define LED_GREEN	 				  GPIOF_PIN_3
#define GPIO_SW1_PIN 				GPIOF_PIN_4

#endif
