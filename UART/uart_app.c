#include "TM4C123GH6PM.h"
#include <stdbool.h>
#include "uart_driver.h"
#include "gpio_driver.h"
#include "led.h"

#define UARTx                   UART0
#define UART_MODULE             0

void uart_gpio_init(void) {
  // Declare a pin config object, and set it to the target pin
	gpio_pin_conf_t uart_gpio_conf;
	uart_gpio_conf.enable_alt = GPIO_PIN_ENABLE_AF;
	uart_gpio_conf.alternate = 0x1;
	uart_gpio_conf.io_type = GPIO_PIN_UNSET;
	uart_gpio_conf.mode = GPIO_PIN_DIGITAL;
	uart_gpio_conf.pupd = GPIO_PIN_NO_PUPD;
  // Enable the clock
	RCC_GPIOA_CLK_ENABLE();
  // Master init
	uart_gpio_conf.pin = 0;
	gpio_init(GPIOA, &uart_gpio_conf);

  uart_gpio_conf.pin = 1;
	gpio_init(GPIOA, &uart_gpio_conf);
}

/*****************************************************
 * This function initializes the UARTx module
 * Use system clock of 20 MHz
 * Baud rate is set to 115200, and BRD = 20,000,000 / (16 * 115200) = 10.8507
 * Int = 10, frac = int(0.8507 * 64 + 0.5) = 54
 * 8-bit data
 * 1 stop bit
 * No parity
 * No FIFO
 * No interrupt
 * No 9-bit address
 * ***************************************************/
void uart_init(void){
  // Enable UART clk
  RCC_UART_CLK_ENABLE(UART_MODULE);

  UARTDisable(UARTx);   // Disable the module before configuration
  uint16_t brd_int = 10;
  uint8_t brd_frac = 54;
  UARTBaudIntDivSet(UARTx, brd_int); // Set the integer part
  UARTBaudFracDivSet(UARTx, brd_frac); // Set the fraction part
  UARTWordWidthSet(UARTx, UART_WLEN_8BIT); // 8-bit data
  // No parity
  // 1 stop bit
  // No FIFO
  uint32_t lineControl = (!UART_SPS) | (!UART_FEN) | (!UART_STP2) | (!UART_EPS) | (!UART_PEN) | (!UART_BRK);
  UARTLineControlSet(UARTx, lineControl);
  UART9BitAdrSet(UARTx, 0x0, !UART_9BITEN); // Disable 9-bit address
  UARTEnable(UARTx);
}

void UARTTransmit(UART0_Type *UARTx, uint8_t *tsm_buffer){
  while(UARTFlagCheck(UARTx, UART_BUSY)); // Wait until not busy
  
  while(tsm_buffer != 0){
    //if (!UARTFlagCheck(UARTx, UART_TXFF)) { // Write data if not full
      UARTWriteByte(UARTx, (*tsm_buffer));
    //}
    tsm_buffer ++;
  }
  
}


void UARTReceive(UART0_Type *UARTx, uint8_t *rcv_buffer){
  while(UARTFlagCheck(UARTx, UART_BUSY)); // Wait until not busy
  
  //while(!UARTFlagCheck(UARTx, UART_RXFE)){  // Read data if not empty
    *rcv_buffer = UARTReadByte(UARTx);
    //rcv_buffer ++;
  //}

}


void delay(unsigned long halfsecs){
  unsigned long count;
  
  while(halfsecs > 0 ) { // repeat while there are still halfsecs to delay
    count = 1538460; // 400000*0.5/0.13 that it takes 0.13 sec to count down to zero
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    halfsecs--;
  }
}

int main(void) {
  uart_gpio_init();
  uart_init();
  NVIC_enable_interrupt(5);
  uint8_t *data;

  while(1){
    UARTReceive(UARTx, data);
  }
}
