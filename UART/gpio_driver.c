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

/*
 *brief  Configure the interrupt for a given pin number
 *param: *GPIOx: GPIO port base address
 *param  pin_no : GPIO pin number 
 *param  int_conf   :  interrupt type
 *return None
*/
void gpio_configure_interrupt(GPIOA_Type *GPIOx, uint16_t pin_no, uint32_t int_conf){
	// Choose sense GPIOx->IS
	// Set event accordingly
	if(int_conf == GPIO_INT_LEVEL){
		GPIOx->IS |= (0x01 << pin_no);
	} else {
		GPIOx->IS &= ~(0x01 << pin_no);
		switch(int_conf){
			case GPIO_INT_RISING_EDGE:
				GPIOx->IEV |= (0x01 << pin_no);
				break;
			case GPIO_INT_FALLING_EDGE:
				GPIOx->IEV &= ~(0x01 << pin_no);
				break;
			case GPIO_INT_BOTH_EDGES:
				GPIOx->IBE |= (0x01 << pin_no);
				break;
		}
	}
}

/*
 *brief  Enable the interrupt for a give pin number and irq number  
 *param  pin_no : GPIO pin number 
 *param  irq_no   :  irq_number to be enabled in NVIC 
 *return None
*/
void gpio_enable_interrupt(GPIOA_Type *GPIOx, uint16_t pin_no){
	// Enable interrupt for the pin
	GPIOx->IM |= (0x01 << pin_no);
}

/**
 *brief  Clear the sticky interrupt pending bit if set 
 *param  pin_no : GPIO pin number 
 *return None
*/
void gpio_clear_interrupt(GPIOA_Type *GPIOx, uint16_t pin_no){
	if((GPIOx->MIS & (0x01 << pin_no)) >> pin_no){
		// If the mask interrupt status is 1, we need to clear it
		// Check MIS instead of RIS (raw int status) since mis is after masking
		// Level trigger is automatically cleared if signal is below level
		GPIOx->ICR |= (0x01 << pin_no);
	}
}

/*
 *param  irq_no   :  irq_number to be enabled in NVIC 
 *return None
*/
void NVIC_enable_interrupt(uint16_t irq_no){
	uint32_t irq_pos = (irq_no % 32);
	// Enable the corresponding interrupt in NVIC
	if(irq_no <= REG0_END){
		EN0 |= (0x01 << irq_pos);
	} else if (irq_no >= REG1_START && irq_no <= REG1_END) {
		EN1 |= (0x01 << irq_pos);
	} else if (irq_no >= REG2_START && irq_no <= REG2_END) {
		EN2 |= (0x01 << irq_pos);
	} else if (irq_no >= REG3_START && irq_no <= REG3_END) {
		EN3 |= (0x01 << irq_pos);
	} else if (irq_no >= REG4_START && irq_no <= REG4_END) {
		EN4 |= (0x01 << irq_pos);
	}
}

/* 
 *param  irq_no   :  irq_number to be disabled in NVIC 
 *return None
*/
void NVIC_disable_interrupt(uint16_t irq_no){
	uint32_t irq_pos = (irq_no % 32);
	// Enable the corresponding interrupt in NVIC
	if(irq_no <= REG0_END){
		DIS0 |= (0x01 << irq_pos);
	} else if (irq_no >= REG1_START && irq_no <= REG1_END) {
		DIS1 |= (0x01 << irq_pos);
	} else if (irq_no >= REG2_START && irq_no <= REG2_END) {
		DIS2 |= (0x01 << irq_pos);
	} else if (irq_no >= REG3_START && irq_no <= REG3_END) {
		DIS3 |= (0x01 << irq_pos);
	} else if (irq_no >= REG4_START && irq_no <= REG4_END) {
		DIS4 |= (0x01 << irq_pos);
	}
}

/*
 *param  irq_no   :  irq_number to be cleared in NVIC 
 *return None
*/
void NVIC_clear_interrupt(uint16_t irq_no){
	uint32_t irq_pos = (irq_no % 32);
	if(irq_no <= REG0_END){
		if((PEND0 & (0x01 << irq_pos)) >> irq_pos){
			UNPEND0 |= (0x01 << irq_pos);
		}
	} else if (irq_no >= REG1_START && irq_no <= REG1_END) {
		if((PEND1 & (0x01 << irq_pos)) >> irq_pos){
			UNPEND1 |= (0x01 << irq_pos);
		}
	} else if (irq_no >= REG2_START && irq_no <= REG2_END) {
		if((PEND2 & (0x01 << irq_pos)) >> irq_pos){
			UNPEND2 |= (0x01 << irq_pos);
		}
	} else if (irq_no >= REG3_START && irq_no <= REG3_END) {
		if((PEND3 & (0x01 << irq_pos)) >> irq_pos){
			UNPEND3 |= (0x01 << irq_pos);
		}
	} else if (irq_no >= REG4_START && irq_no <= REG4_END) {
		if((PEND4 & (0x01 << irq_pos)) >> irq_pos){
			UNPEND4 |= (0x01 << irq_pos);
		}
	}
}
