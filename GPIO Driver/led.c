#include "led.h"

void led_init(void){
    // Declare a pin config object, and set it to the target pin
    // Enable the clock
    // Set the field in the data structure
    // Call the init api with the attibutes to init the pin
	
}

void led_on(GPIOA_Type *GPIOx, uint16_t pin){
    // Write 1 to the pin to turn on
}

void led_off(GPIOA_Type *GPIOx, uint16_t pin){
    // Write 0 to the pin to turn off
}

void led_toggle(GPIOA_Type *GPIOx, uint16_t pin){
    // Toggle the value in the register
}

int main(void) {
    // Test code
    // Some init

    while(1){

    }

    return 0;
}