#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "TM4C123GH6PM.h"

/****************************************************************************
 *                                                                          *
 *                  Macros used for interrupt setup		                    *
 *                                                                          *
 ****************************************************************************/ 
/* Set enable registers */
#define EN0										( *((volatile unsigned long *) 0xE000E100) )
#define EN1										( *((volatile unsigned long *) 0xE000E104) )
#define EN2										( *((volatile unsigned long *) 0xE000E108) )
#define EN3										( *((volatile unsigned long *) 0xE000E10C) )
#define EN4										( *((volatile unsigned long *) 0xE000E110) )

/* Clear enable registers */
#define DIS0									( *((volatile unsigned long *) 0xE000E180) )
#define DIS1									( *((volatile unsigned long *) 0xE000E184) )
#define DIS2									( *((volatile unsigned long *) 0xE000E188) )
#define DIS3									( *((volatile unsigned long *) 0xE000E18C) )
#define DIS4									( *((volatile unsigned long *) 0xE000E190) )
	
/* Set pending registers */
#define PEND0									( *((volatile unsigned long *) 0xE000E200) )
#define PEND1									( *((volatile unsigned long *) 0xE000E204) )
#define PEND2									( *((volatile unsigned long *) 0xE000E208) )
#define PEND3									( *((volatile unsigned long *) 0xE000E20C) )
#define PEND4									( *((volatile unsigned long *) 0xE000E210) )
	
/* Clear pending registers */
#define UNPEND0									( *((volatile unsigned long *) 0xE000E280) )
#define UNPEND1									( *((volatile unsigned long *) 0xE000E284) )
#define UNPEND2									( *((volatile unsigned long *) 0xE000E288) )
#define UNPEND3									( *((volatile unsigned long *) 0xE000E28C) )
#define UNPEND4									( *((volatile unsigned long *) 0xE000E290) )
	
/* IRQn range */
#define REG0_START								0
#define REG0_END								31
#define REG1_START								32
#define REG1_END								63
#define REG2_START								64
#define REG2_END								95
#define REG3_START								96
#define REG3_END								127
#define REG4_START							    128
#define REG4_END								131

/****************************************************************************
 *                                                                          *
 *                  Macros used for pin initialization                      *
 *                                                                          *
 ****************************************************************************/ 
/* GPIO mode setting values */
#define GPIO_PIN_ANALOG                     	( (uint32_t)0x00 )
#define GPIO_PIN_DIGITAL                    	( (uint32_t)0x01 )

/* GPIO alt func setting values */
#define GPIO_PIN_DISABLE_AF                   	( (uint32_t)0x01 )
// To be implemented: 16 alt func's

/* GPIO direction setting values */
#define GPIO_PIN_INPUT                     	    ( (uint32_t)0x00 )
#define GPIO_PIN_OUTPUT                    	    ( (uint32_t)0x01 )

/* GPIO pull up/down type selection values */
#define GPIO_PIN_NO_PUPD						( (uint32_t)0x00 )
#define GPIO_PIN_PULL_UP                        ( (uint32_t)0x01 )
#define GPIO_PIN_PULL_DOWN                      ( (uint32_t)0x02 )
#define GPIO_PIN_OPEN_DRAIN             		( (uint32_t)0x03 )

/* GPIO interrupt sense type selection values */
#define GPIO_INT_RISING_EDGE				    ( (uint32_t)0x00 )
#define GPIO_INT_FALLING_EDGE				    ( (uint32_t)0x01 )
#define GPIO_INT_BOTH_EDGES					    ( (uint32_t)0x02 )
#define GPIO_INT_LEVEL	                        ( (uint32_t)0x03 )

/****************************************************************************
 *                                                                          *
 *                  Macros used for clock initialization                    *
 *                                                                          *
 ****************************************************************************/ 
/* Enable the clock for different ports using RCC registers */
#define RCC_GPIOA_CLK_ENABLE()      volatile unsigned long delay;\
                                    SYSCTL->RCGC2 |= (0x01 << 0);\
                                    delay = SYSCTL->RCGC2;
#define RCC_GPIOB_CLK_ENABLE()      volatile unsigned long delay;\
                                    SYSCTL->RCGC2 |= (0x01 << 1);\
                                    delay = SYSCTL->RCGC2;
#define RCC_GPIOC_CLK_ENABLE()      volatile unsigned long delay;\
                                    SYSCTL->RCGC2 |= (0x01 << 2);\
                                    delay = SYSCTL->RCGC2;
#define RCC_GPIOD_CLK_ENABLE()      volatile unsigned long delay;\
                                    SYSCTL->RCGC2 |= (0x01 << 3);\
                                    delay = SYSCTL->RCGC2;
#define RCC_GPIOE_CLK_ENABLE()      volatile unsigned long delay;\
                                    SYSCTL->RCGC2 |= (0x01 << 4);\
                                    delay = SYSCTL->RCGC2;
#define RCC_GPIOF_CLK_ENABLE()      volatile unsigned long delay;\
                                    SYSCTL->RCGC2 |= (0x01 << 5);\
                                    delay = SYSCTL->RCGC2;
#define RCC_GPIOG_CLK_ENABLE()      volatile unsigned long delay;\
                                    SYSCTL->RCGC2 |= (0x01 << 6);\
                                    delay = SYSCTL->RCGC2;
#define RCC_GPIOH_CLK_ENABLE()      volatile unsigned long delay;\
                                    SYSCTL->RCGC2 |= (0x01 << 7);\
                                    delay = SYSCTL->RCGC2;
#define RCC_GPIOI_CLK_ENABLE()      volatile unsigned long delay;\
                                    SYSCTL->RCGC2 |= (0x01 << 8);\
                                    delay = SYSCTL->RCGC2;

typedef struct {
    uint32_t pin;
    uint32_t mode;
    uint32_t io_type;
    uint32_t pupd;
    uint32_t alternate;
} gpio_pin_conf_t;

/****************************************************************************
 *                                                                          *
 *                            Driver exposed API                            *
 *                                                                          *
 ****************************************************************************/ 

/*
 *brief: Unlocks the pin to enable configurations
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *return: none
*/
static void gpio_configure_pin_unlock(GPIOA_Type *GPIOx, uint16_t pin_no);

/*
 *brief: Configures the mode of a pin: input, output, alt or analogue mode
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: mode: mode to be configured, analog or digital
 *return: none
*/
static void gpio_configure_pin_mode(GPIOA_Type *GPIOx, uint16_t pin_no, uint32_t mode);

/*
 *brief: Configures the output type of a pin
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: op_type: I/O type to be configured
 *return: none
*/
static void gpio_configure_pin_iotype(GPIOA_Type *GPIOx, uint16_t pin_no, uint32_t io_type);

/*
 *brief: Configures the pull up/down register of a pin
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: pupd: pull up/down or open drain to be configured
 *return: none
*/
static void gpio_configure_pin_pupd(GPIOA_Type *GPIOx, uint16_t pin_no, uint32_t pupd);

/*
 *brief: Activate internal pull up or pull down resistors
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: alt_fun_value: alternate function to be configed with
 *return: none
*/
static void gpio_set_alt_function(GPIOA_Type *GPIOx, uint16_t pin_no, uint32_t alt_fun_value);

/*
 *brief: Read value from a pin
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *return: uint8_t: Value read
*/
uint8_t gpio_read_from_pin(GPIOA_Type *GPIOx, uint16_t pin_no);

/*
 *brief: Write value to a pin
 *param: *GPIOx: GPIO port base address
 *param: pin_no: GPIO pin #
 *param: val: The value to write to the pin
 *return: uint8_t: Value read
*/
void gpio_write_to_pin(GPIOA_Type *GPIOx, uint16_t pin_no, uint8_t val);

/*
 *brief: Configures a pin
 *param: *GPIOx: GPIO port base address
 *param: gpio_pin_conf: GPIO pin to configure
 *return: none
*/
void gpio_init(GPIOA_Type *GPIOx, gpio_pin_conf_t *gpio_pin_conf);

/*
 *brief  Configure the interrupt for a given pin number   
 *param  pin_no : GPIO pin number 
 *param  edge_sel   :  Triggering edge slection value of type "int_edge_sel_t"
 *return None
*/
void gpio_configure_interrupt(GPIOA_Type *GPIOx, uint16_t pin_no, uint32_t int_conf);

/*
 *brief  Enable the interrupt for a give pin number and irq number  
 *param  pin_no : GPIO pin number 
 *param  irq_no   :  irq_number to be enabled in NVIC 
 *return None
*/
void gpio_enable_interrupt(GPIOA_Type *GPIOx, uint16_t pin_no);

/*
 *brief  Clear the sticky interrupt pending bit if set 
 *param  pin_no : GPIO pin number 
 *return None
*/
void gpio_clear_interrupt(GPIOA_Type *GPIOx, uint16_t pin_no);

/*
 *param  irq_no   :  irq_number to be enabled in NVIC 
 *return None
*/
void NVIC_enable_interrupt(uint16_t irq_no);

/*  
 *param  irq_no   :  irq_number to be disabled in NVIC 
 *return None
*/
void NVIC_disable_interrupt(uint16_t irq_no);

/* 
 *param  irq_no   :  irq_number to be cleared in NVIC 
 *return None
*/
void NVIC_clear_interrupt(uint16_t irq_no);

#endif
