#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "TM4C123GH6PM.h"

/****************************************************************************
 *                                                                          *
 *                  Macros used for pin initialization                      *
 *                                                                          *
 ****************************************************************************/ 
/* GPIO mode setting values */
#define GPIO_PIN_ANALOG                     		( (uint32_t)0x00 )
#define GPIO_PIN_DIGITAL                    		( (uint32_t)0x01 )

/* GPIO alt func setting values */
#define GPIO_PIN_DISABLE_AF                   	( (uint32_t)0x01 )
// To be implemented: 16 alt func's

/* GPIO direction setting values */
#define GPIO_PIN_INPUT                     			( (uint32_t)0x00 )
#define GPIO_PIN_OUTPUT                    			( (uint32_t)0x01 )

/* GPIO pull up/down type selection values */
#define GPIO_PIN_NO_PUPD												( (uint32_t)0x00 )
#define GPIO_PIN_PULL_UP                        ( (uint32_t)0x01 )
#define GPIO_PIN_PULL_DOWN                      ( (uint32_t)0x02 )
#define GPIO_PIN_OPEN_DRAIN             				( (uint32_t)0x03 )

/* GPIO port address */
#define GPIO_PORT_A     GPIOA
#define GPIO_PORT_B     GPIOB
#define GPIO_PORT_C     GPIOC
#define GPIO_PORT_D     GPIOD
#define GPIO_PORT_E     GPIOE
#define GPIO_PORT_F     GPIOF
#define GPIO_PORT_G     GPIOG
#define GPIO_PORT_H     GPIOH
#define GPIO_PORT_I     GPIOI

/* Enable the clock for different ports using RCC registers */
// Check the user manual and implement them!
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

#endif
