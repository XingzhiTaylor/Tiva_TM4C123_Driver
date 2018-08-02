#include "TM4C123GH6PM.h"
#include "spi_driver.h"
#include "gpio_driver.h"
#include "spi_app.h"

spi_handle_t spi_handle;

/****************** LED init and utils ******************/
void led_init(void){
  // Declare a pin config object, and set it to the target pin
	gpio_pin_conf_t led_conf;
	led_conf.enable_alt = GPIO_PIN_DISABLE_AF;
	led_conf.io_type = GPIO_PIN_OUTPUT;
	led_conf.mode = GPIO_PIN_DIGITAL;
	led_conf.pupd = GPIO_PIN_NO_PUPD;
  // Enable the clock
	RCC_GPIOF_CLK_ENABLE();
  // Call the init api with the attibutes to init the pin
	led_conf.pin = LED_RED;
	gpio_init(GPIO_LED_PORT, &led_conf);
	
	led_conf.pin = LED_GREEN;
	gpio_init(GPIO_LED_PORT, &led_conf);
	
	led_conf.pin = LED_BLUE;
	gpio_init(GPIO_LED_PORT, &led_conf);
}

void led_on(GPIOA_Type *GPIOx, uint16_t pin){
	// Write 1 to the pin to turn on
	gpio_write_to_pin(GPIO_LED_PORT, pin, 0x01);
}

void led_off(GPIOA_Type *GPIOx, uint16_t pin){
	// Write 0 to the pin to turn off
	gpio_write_to_pin(GPIO_LED_PORT, pin, 0x00);
}

void led_toggle(GPIOA_Type *GPIOx, uint16_t pin){
	// Toggle the value in the register
	gpio_write_to_pin(GPIO_LED_PORT, pin, ((gpio_read_from_pin(GPIO_LED_PORT, pin)) ^ 0x01));
}

/****************** SPI GPIO and SPI functionalities init ******************/
void spi_gpio_init(void){
  // Declare a pin config object, and set it to the target pin
	gpio_pin_conf_t spi_gpio_conf;
	spi_gpio_conf.enable_alt = GPIO_PIN_ENABLE_AF;
	spi_gpio_conf.alternate = 0x2;
	spi_gpio_conf.io_type = GPIO_PIN_UNSET;
	spi_gpio_conf.mode = GPIO_PIN_DIGITAL;
	spi_gpio_conf.pupd = GPIO_PIN_PULL_UP;
  // Enable the clock
	RCC_GPIOA_CLK_ENABLE();
  // Master init
	spi_gpio_conf.pin = SPI_CLK;
	gpio_init(SSI0_PORT, &spi_gpio_conf);
	
	spi_gpio_conf.pin = SPI_FSS;
	gpio_init(SSI0_PORT, &spi_gpio_conf);
	
	spi_gpio_conf.pin = SPI_RX;
	gpio_init(SSI0_PORT, &spi_gpio_conf);
	
	spi_gpio_conf.pin = SPI_TX;
	gpio_init(SSI0_PORT, &spi_gpio_conf);
}

void spi_func_init(void){
	// Set up the slave device
	RCC_SSI0_CLK_ENABLE();
	spi_conf_t spi_slave_conf;
	spi_handle.Init = spi_slave_conf;
	spi_handle.Init.Clock_phase = SPI_FIRST_CLOCK_TRANS;
	spi_handle.Init.Clock_polarity = SPI_CLOCK_POL_LOW;
	spi_handle.Init.Frame_format = SPI_FREE_SCL_SPI;
	spi_handle.Init.Data_size = 0x7; // Write 7 for 8-bit data
	spi_handle.Init.Loopback = SPI_NORMAL;
	spi_handle.Init.Mode = SPI_SLAVE_SEL;
	
	spi_handle.Instance = SSI0;
	spi_init(&spi_handle);
	spi_enable(spi_handle.Instance);
}

/****************** LED blink helper functions ******************/
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

// Pattern A: three short blinks 
void patternA (uint16_t colour) {
	led_on(GPIO_LED_PORT, colour);
	delay(1);
	led_off(GPIO_LED_PORT, colour);
	delay(1);
	led_on(GPIO_LED_PORT, colour);
	delay(1);
	led_off(GPIO_LED_PORT, colour);
	delay(1);
	led_on(GPIO_LED_PORT, colour);
	delay(1);
	led_off(GPIO_LED_PORT, colour);
	delay(1);
}

// Pattern B: three long blinks 
void patternB (uint16_t colour) {
	led_on(GPIO_LED_PORT, colour);
	delay(2);
	led_off(GPIO_LED_PORT, colour);
	delay(1);
	led_on(GPIO_LED_PORT, colour);
	delay(2);
	led_off(GPIO_LED_PORT, colour);
	delay(1);
	led_on(GPIO_LED_PORT, colour);
	delay(2);
	led_off(GPIO_LED_PORT, colour);
	delay(1);
}

// Pattern C: long short long 
void patternC (uint16_t colour) {
	led_on(GPIO_LED_PORT, colour);
	delay(2);
	led_off(GPIO_LED_PORT, colour);
	delay(1);
	led_on(GPIO_LED_PORT, colour);
	delay(1);
	led_off(GPIO_LED_PORT, colour);
	delay(1);
	led_on(GPIO_LED_PORT, colour);
	delay(2);
	led_off(GPIO_LED_PORT, colour);
	delay(1);
}

void blink_pattern(uint8_t* signal, uint16_t colour){
	// Blink the corresponding pattern. If invalid, blink white
	uint8_t *ptr = signal;
	for(int i = 0; i < 3; i++){
		switch(*ptr){
			case 'A':
				patternA(colour);
				break;
			case 'B':
				patternB(colour);
				break;
			case 'C':
				patternB(colour);
				break;
			default:
				led_on(GPIO_LED_PORT, LED_BLUE);
				led_on(GPIO_LED_PORT, LED_RED);
				led_on(GPIO_LED_PORT, LED_GREEN);
				delay(2);
				led_off(GPIO_LED_PORT, LED_BLUE);
				led_off(GPIO_LED_PORT, LED_RED);
				led_off(GPIO_LED_PORT, LED_GREEN);
		}
		ptr++;
	}
}

int main(void){
	// Set up
	// Set up the LED GPIO pins
	led_init();
	// Set up SPI pins
	spi_gpio_init();
	// Set up SPI functionalities
	spi_func_init();
	
	NVIC_enable_interrupt(SSI0_IRQ_NBR);
	
	while(1){
		// Receive signal from master
		uint8_t *slave_receive;
		while(spi_status_check(&spi_handle, SPI_BUSY));
		spi_slave_rx(&spi_handle, slave_receive, 3);
		while(spi_status_check(&spi_handle, SPI_BUSY));
		// Slave blinks signal
		blink_pattern(slave_receive, LED_GREEN);		
	
		uint8_t slave_signal[3] = {'C', 'A', 'C'};
		blink_pattern(slave_signal, LED_GREEN);
		// Slave transmits signal
		while(spi_status_check(&spi_handle, SPI_BUSY));
		spi_slave_tx(&spi_handle, slave_signal, 3);
		while(spi_status_check(&spi_handle, SPI_BUSY));			
	}
}

// ISR
void SSI0_Handler(){
	// Call the SSI int handler
	spi_irq_handler(&spi_handle);
	// Clear the interrupt
	NVIC_clear_interrupt(SSI0_IRQ_NBR);
}
