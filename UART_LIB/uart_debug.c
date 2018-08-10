#include "TM4C123GH6PM.h"
#include "gpio_driver.h"
#include "uart_driver.h"
#include "uart_debug.h"
#include <stdio.h>
#include <stdarg.h>


void BaudRateDevisor(uint32_t baudRate, uint16_t *brd_int, uint8_t *brd_frac){
	// Assume 50MHz clock
	double whole = (double)50000000/(16 * (double)baudRate);
	*brd_int = (uint16_t)whole;
	double frac = whole - ((uint8_t)whole);
	*brd_frac = (uint8_t)(frac * 64 + 0.5);
}

void UARTTransmit(UART0_Type* UARTx, uint8_t *tsm_buffer){
  while(UARTFlagCheck(UARTx, UART_BUSY)); // Wait until not busy
  
  while(*tsm_buffer != 0){
    if (!UARTFlagCheck(UARTx, UART_TXFF)) { // Write data if not full
      UARTWriteByte(UARTx, (*tsm_buffer));
    }
    tsm_buffer ++;
		// Wait for a little while before the next TX
		// Character might be lost if transmit too fast
		int count = 15384;
		while(count > 0){
			count --;
		}
  }
  
}

void DebugUartStart(uint32_t baudRate){
	// GPIO pin init
	// Declare a pin config object, and set it to the target pin
	gpio_pin_conf_t uart_gpio_conf;
	uart_gpio_conf.enable_alt = GPIO_PIN_ENABLE_AF;
	uart_gpio_conf.alternate = UART_ALT_FUN;
	uart_gpio_conf.io_type = GPIO_PIN_UNSET;
	uart_gpio_conf.mode = GPIO_PIN_DIGITAL;
	uart_gpio_conf.pupd = GPIO_PIN_NO_PUPD;
  // Enable the clock
	RCC_GPIOA_CLK_ENABLE();
  // Rx init
	uart_gpio_conf.pin = DEBUG_UART_RX_PIN;
	gpio_init(GPIOA, &uart_gpio_conf);
	// Tx init
  uart_gpio_conf.pin = DEBUG_UART_TX_PIN;
	gpio_init(GPIOA, &uart_gpio_conf);
  // Enable UART clk
  RCC_UART_CLK_ENABLE(uint32_t,DEBUG_UART_NUMBER);

	
	// UART module init
  UARTDisable(DEBUG_UART_MODULE);   // Disable the module before configuration
  uint16_t brd_int;
  uint8_t brd_frac;
	BaudRateDevisor(baudRate, &brd_int, &brd_frac);	// Calculate the baud rate divisor
  UARTBaudIntDivSet(DEBUG_UART_MODULE, brd_int); // Set the integer part
  UARTBaudFracDivSet(DEBUG_UART_MODULE, brd_frac); // Set the fraction part
  UARTWordWidthSet(DEBUG_UART_MODULE, UART_WLEN_8BIT); // 8-bit data
  // No parity
  // 1 stop bit
  // FIFO
  uint32_t lineControl = (!UART_SPS) | (UART_FEN) | (!UART_STP2) | (!UART_EPS) | (!UART_PEN) | (!UART_BRK);
  UARTLineControlSet(DEBUG_UART_MODULE, lineControl);
  UARTEnable(DEBUG_UART_MODULE);
}

void DebugUartPrintf(char *format, ...){
	char str[80];
	char *tsm_buffer;
	
	/*Extract the the argument list using VA apis */
	va_list args;
	va_start(args, format);
	vsprintf(str, format,args);
	tsm_buffer=str;
	
	UARTTransmit(DEBUG_UART_MODULE, tsm_buffer);
	va_end(args);
}
