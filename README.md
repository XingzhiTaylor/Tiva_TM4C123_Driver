## Overview
This repository is my driver development project for Tiva Launchpad TM4C123GH6PM. The repository includes driver software for four functionaities
- GPIO
- SPI
- I2C
- UART

The driver software accesses the control registers and program them with given parameters. The driver is capable of initializing the module, and perform simple I/O. Simple applications that call the APIs to do I/O are also included in the repo. The description of each driver is **written separately in their own directories**.

## Source Code and Headers
### Drivers
The names of the driver header files and source code are named as follow:
#### GPIO
- gpio_driver.h
- gpio_driver.c

#### SPI
- spi_driver.h
- spi_driver.c

#### I2C
- i2c_driver.h
- i2c_driver.c

#### UART
- uart_driver.h
- uart_driver.c
- uart_debug.h
- uart_debug.lib

### Sample Apps
There are sample apps in the directory showing how the APIs work. The names of the sample app header files and source code are named as follow:
#### GPIO
- led.h
- led.c

#### SPI
- spi_app.h
- spi_master.c
- spi_slave.c
- simple_test.c

#### I2C
- i2c_app.h
- i2c_app.c
- simple_test.c

#### UART
- uart_app.h
- uart_app.c
- uart_app1.c
- uart_app2.c
- uart_app3.c
- simple_test.c

## Acknowledgement
In the development of the project, I referenced the Udemy course *Mastering Microcontroller with Embedded Driver Development* and *Tiva TM4C123GH6PM Microcontroller Data Sheet*. The Udemy course can be accessed using the following link
(https://www.udemy.com/mastering-microcontroller-with-peripheral-driver-development/learn/v4/content)
The drivers are still under development. More advanced functionalities are to be implemented in the software.
