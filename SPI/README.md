## Overview
This driver is capable of basic SPI operations, including SPI master/slave initialization, SPI control parameter setup, simple SPI communication, and interrupt handling. The user can set up master and slave on two separate devices and communicate using the APIs.

## API Descriptions
### Data Structures
Similar to the GPIO driver, there is a `spi_conf_t` data structure that wraps the config parameters. The config parameters include clock phase, clock polarity, clock prescaler, master/slave operation, data size, etc. There is also a handler data structure that handles the SPI module in the software excution. The runtime handler contains an instance of `spi_conf_t`, the base address of the SPI module, pointer to the data to transmit and receive. The runtime handler will initialize the module, read/write data from/to the module, and enable/disable interrupts.
### APIs
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
Similar to the transmit function, this function doesn't read the data register but only enables the interrupts. After the interrupt is enabled, a receive FIFO not empty is automatically generated, and data will be read from the data register by the interrupt handler.
```
void spi_irq_handler(spi_handle_t *spi_handle);
```
This function is where everything happens. After receiving the interrupt from NVIC, the handler will check the interrupt flags to determine which operation to run. It can send or receive data as a slave or master. The interrupt will be cleared after handling.

## SPI Operations
### Connection
#### CLK
SPI is a synchronized protocol, and a clock is provided. The clock is only provided by the master, and the slave r/w data following the master's clock. If the master wants to Tx/Rx data, it has to provide clock to the slave.
The data rate can be set in master configuration. In Tiva TM4C123 microcontroller, SPI clock rate can be configured as:
```
SPIClk = SysClk / (CPSDVSR * (1 + SCR))
```
Clock pre-divisor value (CPSDVSR) and SPI clock rate (SCR) can be written to corresponding registers.
#### MISO/MOSI
MISO stands for Master In Slave Out, and MOSI stands for Master Out Slave In. Data is transmitted on these two lines between the master and the slave.
### SPI Hardware
The SPI data is transmitted and received using shift register. Data is shifted one bit per clock cycle. In the transmitter, the data bit is shifted to the data line, and the receiver pushes this bit to its shift register. In Tiva microcontroller, the shift register is connected to a FIFO (First In First Out), and the FIFO is connected to the DR register. The User can r/w data by r/w the DR register. The data will then be poped from/pushed into Rx/Tx the FIFO. 
### Clock Phase and Clock Polarity
The slave or master samples the data line to determine what data is place on the line. The sampling follows the clock provided by the master, and clock phase and clock polarity determine when to sample.
There are two different clock phases:
- Phase 0: data is sampled on the leading edge of the clock
- Phase 1: data is sampled on the trailing edge of the clock
There are two different clock polarities:
- Polarity 0: Clock idle state is 0
- Polarity 1: Clock idle state is 1
The four combinations of clock phase and polarity forms four operating modes.
### Work Flow
- When data communication is going to happen, clock is provided on the clock line by the master
- Transmitter push data to DR register
- Data in the DR register goes into the Tx FIFO
- Data in transmit FIFO goes into the transmit logic
- Data transmit on MISO or MOSI line bit by bit
- Data sampled by the receiver according to the clock polarity and phase
- Data pushed into Rx FIFO of the receiver
- Receiver reads data from DR register 
### Interrupts 
Interrupt types vary from vendor to vendor. In Tiva TM4C123 microcontroller, the following interrupts are used:
- Transmit FIFO service: Tx FIFO is half full or less, time to push more data for Tx
- Receive FIFO service: Rx FIFO is half empty or less, time to read the transmitted data
- Receive FIFO time-out: Rx FIFO is not empty after 32 clock cycles, something might be wrong
- Receive Overrun: Overflow occurs in the Rx FIFO leading to data loss
- End of Transmission: When Tx FIFO is cleared, transmission finishes