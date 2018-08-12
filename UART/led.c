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
	sw_conf.pin = GPIO_SW1_PIN;
	sw_conf.alternate = GPIO_PIN_DISABLE_AF;
	sw_conf.io_type = GPIO_PIN_INPUT;
	sw_conf.mode = GPIO_PIN_DIGITAL;
	sw_conf.pupd = GPIO_PIN_PULL_UP;
	
	gpio_init(GPIO_SW_PORT, &sw_conf);
	
	sw_conf.pin = GPIO_SW2_PIN;
	gpio_init(GPIO_SW_PORT, &sw_conf);
}
