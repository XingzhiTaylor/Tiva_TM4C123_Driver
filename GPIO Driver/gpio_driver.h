#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "TM4C123GH6PM.h"

/*
 *brief: Configures the mode of a pin: input, output, alt or analogue mode
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: mode: mode to be configured
 *return: none
*/
static void hal_gpio_configure_pin_mode(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t mode){

}

/*
 *brief: Configures the output type of a pin
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: op_type: output type to be configured
 *return: none
*/
static void hal_gpio_configure_pin_otype(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t op_type){

}

/*
 *brief: Configures the speed of a pin
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: speed: speed to be configured
 *return: none
*/
static void hal_gpio_configure_pin_speed(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t speed){

}

/*
 *brief: Activate internal pull up or pull down resistors
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: pupd: specify the resistor to activate
 *return: none
*/
static void hal_gpio_configure_pin_pupd(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t pupd){

}

/*
 *brief: Activate internal pull up or pull down resistors
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: alt_fun_value: alternate function to be configed with
 *return: none
*/
static void hal_gpio_set_alt_function(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t alt_fun_value){

}

/*
 *brief: Read value from a pin
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *return: uint8_t: Value read
*/
uint8_t hal_gpio_read_from_pin(GPIO_TypeDef *GPIOx, uint16_t pin_no) {

}

/*
 *brief: Write value to a pin
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: val: The value to write to the pin
 *return: uint8_t: Value read
*/
void hal_gpio_write_to_pin(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint8_t val) {

}














#endif