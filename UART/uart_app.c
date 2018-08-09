#include "TM4C123GH6PM.h"
#include <stdbool.h>
#include "uart_driver.h"
#include "gpio_driver.h"
#include "led.h"
#include "uart_app.h"

void delay(unsigned long halfsecs);

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
	uart_gpio_conf.pin = UART_RX_PIN;
	gpio_init(GPIOA, &uart_gpio_conf);

  uart_gpio_conf.pin = UART_TX_PIN;
	gpio_init(GPIOA, &uart_gpio_conf);
}

/*****************************************************
 * This function initializes the UARTx module
 * Use system clock of 50 MHz
 * Baud rate is set to 9600, and BRD = 50,000,000 / (16 * 9600) = 325.52
 * Int = 325, frac = int(0.52 * 64 + 0.5) = 33
 * 8-bit data
 * 1 stop bit
 * No parity
 * No FIFO
 * No interrupt
 * No 9-bit address
 * ***************************************************/
void uart_init(void){
  // Enable UART clk
  RCC_UART_CLK_ENABLE(uint32_t,UART_NUMBER);

  UARTDisable(UART_MODULE);   // Disable the module before configuration
  uint16_t brd_int = 325;
  uint8_t brd_frac = 33;
  UARTBaudIntDivSet(UART_MODULE, brd_int); // Set the integer part
  UARTBaudFracDivSet(UART_MODULE, brd_frac); // Set the fraction part
  UARTWordWidthSet(UART_MODULE, UART_WLEN_8BIT); // 8-bit data
  // No parity
  // 1 stop bit
  // FIFO
  uint32_t lineControl = (!UART_SPS) | (UART_FEN) | (!UART_STP2) | (!UART_EPS) | (!UART_PEN) | (!UART_BRK);
  UARTLineControlSet(UART_MODULE, lineControl);
  UART9BitAdrSet(UART_MODULE, UART_DUMMY_ADDR, !UART_9BITEN); // Disable 9-bit address
  UARTEnable(UART_MODULE);
}

void UARTTransmit(UART0_Type* UARTx, uint8_t *tsm_buffer){
  while(UARTFlagCheck(UART_MODULE, UART_BUSY)); // Wait until not busy
  
  while(*tsm_buffer != 0){
    if (!UARTFlagCheck(UARTx, UART_TXFF)) { // Write data if not full
      UARTWriteByte(UART_MODULE, (*tsm_buffer));
    }
    tsm_buffer ++;
		delay(1);
  }
  
}


void UARTReceive(UART0_Type* UARTx, uint8_t *rcv_buffer){
  while(UARTFlagCheck(UARTx, UART_BUSY)); // Wait until not busy
  
  while(!UARTFlagCheck(UARTx, UART_RXFE)){  // Read data if not empty
    *rcv_buffer = UARTReadByte(UARTx);
    rcv_buffer ++;
  }

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
	led_init();
  uart_gpio_init();
  uart_init();
  NVIC_enable_interrupt(UART_IRQ_NUMBER);
  uint8_t data;
	uint8_t res0[] = "hello";
	uint8_t res1[] = "bye";

  while(1){
    UARTReceive(UART_MODULE, &data);
		if(data == 'a'){
			led_turn_off(GPIO_LED_PORT,LED_RED);
			led_turn_on(GPIO_LED_PORT,LED_GREEN);
			UARTTransmit(UART_MODULE, res0);
		} else {
			led_turn_off(GPIO_LED_PORT,LED_GREEN);
			led_turn_on(GPIO_LED_PORT,LED_RED);
			UARTTransmit(UART_MODULE, res1);
		}
		delay(2);
  }
}
