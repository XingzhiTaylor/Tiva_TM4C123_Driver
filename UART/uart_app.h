#ifndef UART_APP_H
#define UART_APP_H

#include "TM4C123GH6PM.h"
#include "uart_driver.h"
#include "gpio_driver.h"

#define UART_MODULE             UART0
#define UART_NUMBER             0
#define UART_DUMMY_ADDR					0x0
#define UART_IRQ_NUMBER					5
#define UART_ALT_FUN						0x1
#define GPIO_PA_1								1
#define GPIO_PA_0								0
#define GPIOF_IRQ_NUMBER				30
#define UART_TX_PIN							GPIO_PA_1
#define UART_RX_PIN							GPIO_PA_0


#endif
