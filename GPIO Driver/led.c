#include "led.h"
#include "gpio_driver.h"

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
    // Test code
    led_init();
    while(1){
			led_on(GPIO_LED_PORT, LED_RED);
			delay(1);
			led_on(GPIO_LED_PORT, LED_GREEN);
			delay(1);
			led_on(GPIO_LED_PORT, LED_BLUE);
			delay(1);
			led_off(GPIO_LED_PORT, LED_RED);
			delay(1);	
			led_off(GPIO_LED_PORT, LED_GREEN);
			delay(1);
			led_off(GPIO_LED_PORT, LED_BLUE);
			delay(1);
    }
}
