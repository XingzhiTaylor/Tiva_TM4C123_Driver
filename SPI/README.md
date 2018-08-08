## Overview
This driver is capable of basic SPI operations, including SPI master/slave initialization, SPI control parameter setup, simple SPI communication, and interrupt handling. The user can set up master and slave on two separate devices and communicate using the APIs.

## API Descriptions
#### Data Structures
Similar to the GPIO driver, there is a `spi_conf_t` data structure that wraps the config parameters. The config parameters include clock phase, clock polarity, clock prescaler, master/slave operation, data size, etc. There is also a handler data structure that handles the SPI module in the software excution. The runtime handler contains an instance of `spi_conf_t`, the base address of the SPI module, pointer to the data to transmit and receive. The runtime handler will initialize the module, read/write data from/to the module, and enable/disable interrupts.
#### APIs
Here are the introduction to some APIs:
```
void spi_init(spi_handle_t *spi_handle)
```
This function takes the init data structure and config the module following the parameters provided.
```
RCC_SSIx_CLK_ENABLE()
```
This function enables the clock of the SSI module.
```
void spi_master_tx(spi_handle_t *spi_handle, uint8_t *tsm_buffer, uint32_t len);
void spi_slave_tx(spi_handle_t *spi_handle, uint8_t *tsm_buffer, uint32_t len);
```
These two functions can send data SPI data to the slave/master as a master/slave. In the implememntation, data is not written to the data register in this function, only transmit interrupt is enabled. After enabling the interrupt, a transmit FIFO empty interrupt will be automatically generated, and the interrupt handler will do the rest 
```
void spi_master_rx(spi_handle_t *spi_handle, uint8_t *rcv_buffer, uint32_t len);
void spi_slave_rx(spi_handle_t *spi_handle, uint8_t *rcv_buffer, uint32_t len);
```
Similar to the transmit function, this function doesn't read the data register but only enables the interrupts. After the interrupt is enabled, a receive FIFO not empty is automatically generated, and data will be read from the data register in the interrupt handler.
```
void spi_irq_handler(spi_handle_t *spi_handle);
```
This function is where everything happens. After receiving the interrupt from NVIC, the handler will check the interrupt flags to determine which operation to run. It can send or receive data as a slave or master. The interrupt will be cleared after handling.

## SPI Operations

