#include "led.h"
#include "gpio_driver.h"

// This is changable
#define SW_PIN  GPIO_SW2_PIN

void led_init(void){
    // Declare a pin config object, and set it to the target pin
	gpio_pin_conf_t led_conf_t;
	led_conf_t.alternate = GPIO_PIN_DISABLE_AF;
	led_conf_t.io_type = GPIO_PIN_OUTPUT;
	led_conf_t.mode = GPIO_PIN_DIGITAL;
	led_conf_t.pupd = GPIO_PIN_NO_PUPD;
    // Enable the clock
	RCC_GPIOF_CLK_ENABLE();
    // Call the init api with the attibutes to init the pin
	led_conf_t.pin = LED_RED;
	gpio_init(GPIO_LED_PORT, &led_conf_t);
	
	led_conf_t.pin = LED_GREEN;
	gpio_init(GPIO_LED_PORT, &led_conf_t);
	
	led_conf_t.pin = LED_BLUE;
	gpio_init(GPIO_LED_PORT, &led_conf_t);
}

void led_turn_on(GPIOA_Type *GPIOx, uint16_t pin){
    // Write 1 to the pin to turn on
	gpio_write_to_pin(GPIO_LED_PORT, pin, 0x01);
}

void led_turn_off(GPIOA_Type *GPIOx, uint16_t pin){
    // Write 0 to the pin to turn off
	gpio_write_to_pin(GPIO_LED_PORT, pin, 0x00);
}

void led_toggle(GPIOA_Type *GPIOx, uint16_t pin){
    // Toggle the value in the register
	gpio_write_to_pin(GPIO_LED_PORT, pin, ((gpio_read_from_pin(GPIO_LED_PORT, pin)) ^ 0x01));
}

void sw_init(void){
	gpio_pin_conf_t sw_conf;
	sw_conf.pin = SW_PIN;
	sw_conf.alternate = GPIO_PIN_DISABLE_AF;
	sw_conf.io_type = GPIO_PIN_INPUT;
	sw_conf.mode = GPIO_PIN_DIGITAL;
	sw_conf.pupd = GPIO_PIN_PULL_UP;
	
	gpio_init(GPIO_SW_PORT, &sw_conf);
}

/*
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
    // Initialize LED
    led_init();
	// Initialize SW
	sw_init();
	// Enable and Initialize interrupt
	NVIC_enable_interrupt(GPIOF_IRQn);
	gpio_enable_interrupt(GPIO_SW_PORT, SW_PIN);
	gpio_configure_interrupt(GPIO_SW_PORT, GPIO_SW1_PIN, GPIO_INT_FALLING_EDGE);
    while(1){
			// Basic switching test
			led_on(GPIO_LED_PORT, LED_RED);
			delay(1);
			led_off(GPIO_LED_PORT, LED_RED);
			delay(1);
			led_on(GPIO_LED_PORT, LED_GREEN);
			delay(1);
			led_off(GPIO_LED_PORT, LED_GREEN);
			delay(1);
			led_on(GPIO_LED_PORT, LED_BLUE);
			delay(1);		
			led_off(GPIO_LED_PORT, LED_BLUE);
			delay(1);
			
			// Toggle test
			led_toggle(GPIO_LED_PORT, LED_RED);
			delay(1);
			led_toggle(GPIO_LED_PORT, LED_RED);
			delay(1);
			led_toggle(GPIO_LED_PORT, LED_GREEN);
			delay(1);
			led_toggle(GPIO_LED_PORT, LED_GREEN);
			delay(1);
			led_toggle(GPIO_LED_PORT, LED_BLUE);
			delay(1);		
			led_toggle(GPIO_LED_PORT, LED_BLUE);
			delay(1);		
    }
}

void GPIOF_Handler(void){
	// Clear the interrupt bits, otherwise interrupt will keep being sent to the processor
	gpio_clear_interrupt(GPIO_SW_PORT, SW_PIN);
	NVIC_clear_interrupt(GPIOF_IRQn);
	// Flash the red LED
	for(int i = 0; i < 4; i++){
		led_on(GPIO_LED_PORT, LED_RED);
		delay(1);
		led_off(GPIO_LED_PORT, LED_RED);
		delay(1);
	}
}
*/
