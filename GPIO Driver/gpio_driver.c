#include "gpio_driver.h"
#include "TM4C123GH6PM.h"

/*
 *brief: Unlocks the pin to enable configurations
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *return: none
*/
static void gpio_configure_pin_unlock(GPIOA_Type *GPIOx, uint16_t pin_no){
    GPIOx->LOCK = 0x4C4F434B; // Unlock the commit register
    GPIOx->CR |= (0x01 << pin_no); // Write 0x01 to the corresponding bit in commit register
}

/*
 *brief: Configures the mode of a pin: input, output, alt or analogue mode
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: mode: mode to be configured
 *return: none
*/
static void gpio_configure_pin_mode(GPIOA_Type *GPIOx, uint16_t pin_no, uint32_t mode){
    if(mode == GPIO_PIN_DIGITAL) {
        GPIOx->AMSEL = 0x00; //Disable analog mode
        GPIOx->DEN |= (0X01 << pin_no); // Enable digital I/O for the pin
    } else {
        // To be implemented
    }
}

/*
 *brief: Configures the direction of a pin
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: io_type: I/O direction to be configured
 *return: none
*/
static void gpio_configure_pin_iotype(GPIOA_Type *GPIOx, uint16_t pin_no, uint32_t io_type){
    if(io_type == GPIO_PIN_OUTPUT){
        GPIOx->DIR |= (0x01 << pin_no); // Set the bit to 1
    } else {
        GPIOx->DIR &= ~(0x01 << pin_no); // Clear the bit as 0
    }
}

/*
 *brief: Configures the pull up/down register of a pin
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: pupd: pull up/down or open drain to be configured
 *return: none
*/
static void gpio_configure_pin_pupd(GPIOA_Type *GPIOx, uint16_t pin_no, uint32_t pupd){
    if(pupd == GPIO_PIN_PULL_UP){
        GPIOx->PUR |= (0x01 << pin_no); // Set the pull up register bit to 1
    } else if(pupd == GPIO_PIN_PULL_DOWN) {
        GPIOx->PDR |= (0x01 << pin_no); // Set the pull down register bit to 1
    } else if (pupd == GPIO_PIN_OPEN_DRAIN) {
        GPIOx->ODR |= (0x01 << pin_no); // Set the open drain register bit to 1
    }
}

/*
 *brief: Activate internal pull up or pull down resistors
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: alt_fun_value: alternate function to be configed with
 *return: none
*/
static void gpio_set_alt_function(GPIOA_Type *GPIOx, uint16_t pin_no, uint32_t alt_fun_value){
    GPIOx->PCTL |= (alt_fun_value << (4 * pin_no)); // Choose alt func	
}

/*
 *brief: Read value from a pin
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *return: uint8_t: Value read
*/
uint8_t gpio_read_from_pin(GPIOA_Type *GPIOx, uint16_t pin_no) {
    uint8_t data;
    data = ((GPIOx->DATA & (0x01 << pin_no)) >> pin_no);
    return data;
}

/*
 *brief: Write value to a pin
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: val: The value to write to the pin
 *return: uint8_t: Value read
*/
void gpio_write_to_pin(GPIOA_Type *GPIOx, uint16_t pin_no, uint8_t val) {
	if(val) {
		GPIOx->DATA |= (0x01 << pin_no);
	} else {
		GPIOx->DATA &= ~(0x01 << pin_no);
	}
	
}

/*
 *brief: Configures a pin
 *param: *GPIOx: GPIO port base address
 *param: gpio_pin_conf: GPIO pin to configure
 *return: none
*/
void gpio_init(GPIOA_Type *GPIOx, gpio_pin_conf_t *gpio_pin_conf){
    //unlock
    gpio_configure_pin_unlock(GPIOx, gpio_pin_conf->pin);
    //alternate func
    gpio_set_alt_function(GPIOx, gpio_pin_conf->pin, gpio_pin_conf->alternate);
    //mode
    gpio_configure_pin_mode(GPIOx, gpio_pin_conf->pin, gpio_pin_conf->mode);
    //dir
    gpio_configure_pin_iotype(GPIOx, gpio_pin_conf->pin, gpio_pin_conf->io_type);
    //enable pupd
    gpio_configure_pin_pupd(GPIOx, gpio_pin_conf->pin, gpio_pin_conf->pupd);
}
