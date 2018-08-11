## Overview
This driver is capable of basic GPIO manipulations, including GPIO initialization, alternating function setup, data I/O, and interrupt handling. The sample app allows the user to control on-board LEDs with switches.

## API Descriptions
#### Data structures
A data structure `gpio_pin_conf_t` is defined in the header file to wrap all the init parameters. The user can set the fields in the data structure to config the pin. Depending on the application, some fields are not set. This can be done by setting the field to `GPIO_PIN_UNSET`.
#### APIs
Here are the introduction to some APIs
```void gpio_init(GPIOA_Type *GPIOx, gpio_pin_conf_t *gpio_pin_conf)```
This function initializes a GPIO pin in port `GPIOx` with the parameter provided by the user in `gpio_pin_conf`. 
```RCC_GPIOX_CLK_ENABLE()```
This function enables the clock of a GPIO port
```
void gpio_configure_interrupt(GPIOA_Type *GPIOx, uint16_t pin_no, uint32_t int_conf);
void gpio_enable_interrupt(GPIOA_Type *GPIOx, uint16_t pin_no);
void gpio_clear_interrupt(GPIOA_Type *GPIOx, uint16_t pin_no);
```
These three functions config and deal with the interrupts from the GPIO pin.
```
void NVIC_enable_interrupt(uint16_t irq_no);
void NVIC_disable_interrupt(uint16_t irq_no);
void NVIC_clear_interrupt(uint16_t irq_no);
```
These three functions config and deal with the interrupts from the NVIC.
```
uint8_t gpio_read_from_pin(GPIOA_Type *GPIOx, uint16_t pin_no);
void gpio_write_to_pin(GPIOA_Type *GPIOx, uint16_t pin_no, uint8_t val);
```
These two APIs perform r/w opertations

## GPIO Operations
Each GPIO port has its clock. The clock has to be enabled before any other operation. Failure to do this will cause a bus fault.
#### Data Control
###### Pin Direction
The GPIO pin hardware has a tri-state buffer, so we can configure the pin as either input or output. In the input mode, the pin is at a high-Z state \(also called floating state\). This state is neither high nor low, and the pin accepts signal from outside. In the output mode, the state of the pin is high or low, determined by the output signal. 
###### Pin Configurations
There are different hardware configurations for the pin. Usually there is a pull-up transistor that pulls the output to Vdd and a pull-down transistor that pulls the output down to 0. 
Voltage value close to Vdd is defined as HIGH, and volatge close to 0 is defined as LOW. The range of HIGH and LOW varies with microcontroller vendors. For voltage in between HIGH and LOW, they are floating state. Floating state may false trigger the transistors and cause error. To make sure the output is not floating, pull-up/down resistors are used. Pull-up resistors are connected to Vdd while pull-down resistors are connected to GND. Their values are chosen such that floating never occurs.
Another configuration is open drain, where nothing is attached to the drain of the CMOS. The open drain configuration can provide higher voltage and/or current, but the external circuit should provide a pull-up/down resistor. 
The output modes can be configured in control regusters.
###### Pin Data Register and Bit Specific Addressing
There is a data register that holds the data read from/written to the GPIO port. The data register has 8 bits for r/w, and bit n in the data register corresponds to pin n in the port. 
Sometimes we want to access single pin. Bit specific addressing provides a one-to-one mapping from the address bit to the pin. In Tiva microcontroller, bit 2 to 9 of the address are one-to-one mapped to the pin 0 to 7. If one wants to access one or more pins, he/she just needs to set the corresponding address bits to 1. Each GPIO port has a base address. By adding bit 2:9 to the base address, one can access a specific pin in a GPIO port.
Bit specific addressing is a good way of masking. It makes it easier to access single pins by virtually mapping single pin to single address. It also avoids touching the wrong pin by accident.
#### Interrupt Control
The GPIO can sense interrupts. The pin can be set to raise interrupt when
- Rising edge detected
- Falling edge detected
- Both edge detected
- Voltage level detected
The interrupt can be masked using interrupt mask register. Only unmasked interrupts can be received by the processor. The interrupt status before masking is provided in raw interrupt status register. The interrupt should be cleared using the interrupt clear register after being handled. Otherwise the proccessor will keep receiving the interrupt signal
#### Mode Control
The GPIO can be configured to different modes including SPI, I2C, UART, PWM, etc. This can be done by enabling alt func and writing correspoding alt func code to the port control register.
The pin can also produce analogue I/O by enabling analogue mode. This functionality is with the ADC module.
#### NVIC (Nested Vectored Interrupt Controller)
Each type of interrupt has its number. There is a vector interrupt table where the address of each interrupt service routine is stored. The NVIC finds the corresponding ISR according to the IRQ number and runs the ISR. The corresponding interrupt should be enabled in NVIC for the process to receive it. The interrupt handler should also be defined in the code as a subroutine.
