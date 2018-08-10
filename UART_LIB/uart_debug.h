#ifndef UART_DEBUG_H
#define UART_DEBUG_H

#include "TM4C123GH6PM.h"
#include "uart_driver.h"
#include <stdbool.h>

#define DEBUG_UART_MODULE				UART0
#define DEBUG_UART_NUMBER				0
#define UART_ALT_FUN						0x1

#define GPIO_PA_1								1
#define GPIO_PA_0								0
#define DEBUG_UART_TX_PIN				GPIO_PA_1
#define DEBUG_UART_RX_PIN				GPIO_PA_0

#define DEBUG_UART_BAUD_4800		4800
#define DEBUG_UART_BAUD_9600		9600
#define DEBUG_UART_BAUD_19200		19200
#define DEBUG_UART_BAUD_38400		38400
#define DEBUG_UART_BAUD_57200		57200
#define DEBUG_UART_BAUD_115200	115200

#define SYS_CLK_RATE						50000000

void DebugUartStart(uint32_t baudRate);
void DebugUartPrintf(char *format, ...);


#endif
