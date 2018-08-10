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
  // Rx init
	uart_gpio_conf.pin = UART_RX_PIN;
	gpio_init(GPIOA, &uart_gpio_conf);
	// Tx init
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
 * Use FIFO
 * Rx Interrupt level 1/8
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
	// Rx Interrupt level 1/8
  uint32_t lineControl = (!UART_SPS) | (UART_FEN) | (!UART_STP2) | (!UART_EPS) | (!UART_PEN) | (!UART_BRK);
  UARTLineControlSet(UART_MODULE, lineControl);
	UARTRxIntFIFOLvlSet(UART_MODULE, UART_RX_1_8_FULL);
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
		// Wait for a little while before the next TX
		// Character might be lost if transmit too fast
		int count = 15384;
		while(count > 0){
			count --;
		}
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
	// Initialize LED, switches, and UART0
	led_init();
	sw_init();
  uart_gpio_init();
  uart_init();
	// Enable NVIC interrupts for GPIO and UART
  NVIC_enable_interrupt(UART_IRQ_NUMBER);
	NVIC_enable_interrupt(GPIOF_IRQn);
	// Enable UART receive interrupt
	UARTInterruptEnable(UART_MODULE, UART_RX_INT);
	// Enable GPIO switch interrupt
	gpio_enable_interrupt(GPIO_SW_PORT, GPIO_SW1_PIN);
	gpio_enable_interrupt(GPIO_SW_PORT, GPIO_SW2_PIN);
	// Config the sw to sense falling edge
	gpio_configure_interrupt(GPIO_SW_PORT, GPIO_SW1_PIN, GPIO_INT_FALLING_EDGE);
	gpio_configure_interrupt(GPIO_SW_PORT, GPIO_SW2_PIN, GPIO_INT_FALLING_EDGE);
	while(1){
		// Wait for something to happen
	}
}

void UART0_Handler(void){
	uint8_t data;
	UARTReceive(UART_MODULE, &data);
	if((data >= 48) && (data <= 57)){
		// This is a number, turn on blue LED
		led_turn_on(GPIO_LED_PORT, LED_BLUE);
		led_turn_off(GPIO_LED_PORT, LED_GREEN);
		led_turn_off(GPIO_LED_PORT, LED_RED);
	}else if((data >= 65) && (data <= 90)){
		// This is an upper case letter, turn on green LED
		led_turn_off(GPIO_LED_PORT, LED_BLUE);
		led_turn_on(GPIO_LED_PORT, LED_GREEN);
		led_turn_off(GPIO_LED_PORT, LED_RED);
	}else if((data >= 97) && (data <= 122)){
		// This is an upper case letter, turn on green LED
		led_turn_off(GPIO_LED_PORT, LED_BLUE);
		led_turn_off(GPIO_LED_PORT, LED_GREEN);
		led_turn_on(GPIO_LED_PORT, LED_RED);
	}else{
		// Otherwise, blink the blue LED
		led_toggle(GPIO_LED_PORT, LED_BLUE);
		delay(1);
		led_toggle(GPIO_LED_PORT, LED_BLUE);
		delay(1);
		led_toggle(GPIO_LED_PORT, LED_BLUE);
		delay(1);
		led_toggle(GPIO_LED_PORT, LED_BLUE);
	}
	// Clear the interrupts after finished
	UARTInterruptClear(UART_MODULE, UART_RX_INT);
	NVIC_clear_interrupt(UART_IRQ_NUMBER);
}

void GPIOF_Handler(void){
	// Different responses depending on inputs
	uint8_t res1[] = "Switch 1 pressed";
	uint8_t res2[] = "Switch 2 pressed";
	uint8_t res3[] = "Oh Geez!!!!";
	if(GPIO_SW_PORT->MIS & (1 << 4)){
		// SW 1 is pressed
		UARTTransmit(UART_MODULE, res1);
		gpio_clear_interrupt(GPIO_SW_PORT, GPIO_SW1_PIN);
	}else if(GPIO_SW_PORT->MIS & (1 << 0)){
		// SW 2 is pressed
		UARTTransmit(UART_MODULE, res2);
		gpio_clear_interrupt(GPIO_SW_PORT, GPIO_SW2_PIN);
	}else{
		// Unknown
		UARTTransmit(UART_MODULE, res3);	
	}
	// Clear the interrupts after finished
	NVIC_clear_interrupt(GPIOF_IRQn);
}
