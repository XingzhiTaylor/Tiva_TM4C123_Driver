## Overview
This driver is capable of basic I2C operations, including I2C master/slave initialization, I2C parameters setup, I2C communications, and interrupt handling. The user can setup master and slave on separate devices and communucate using these APIs. However, the functionality of repeated start and high speed mode is still being implemented.

## API Descriptions
#### Data Structures
Same as SPI, there is a `i2c_init_t` data structure that wraps up all the config parameters. The data structure includes master/slave selection, loopback enable, clock period, high speed enable, glitch filter setup, slave address setup, and slave dual address setup. 
There is a run time handler called `i2c_handle_t`. Same as the I2C data structure, it stores the I2C instance, init parameters, and I2C Tx/Rx data.
#### APIs
Here are some introduction to the APIs:
```
void i2c_init(i2c_handle_t *i2cx)
```
This is a init function. It sets the control register according to the parameters provided in the wrapper data structure.
```
RCC_I2Cx_CLK_ENABLE()
```
This function enables the clock of the corresponding I2C module. Configuring an I2C module before initializing its clock will result in a bus fault.
```
void i2c_master_tx(i2c_handle_t *i2cx)
void i2c_master_hs_tx(i2c_handle_t *i2cx)
void i2c_slave_tx(i2c_handle_t *i2cx)
```
These two APIs will transmit data from master and slave. `i2c_master_hs_tx` transmit data in high speed mode. These three function don't access the data register, and only enables corresponded interrupts. After enabling, a Tx/Rx interrupt will be raised when it is ok to Tx/Rx, and the interrupt handler will do the actual Tx/Rx.
```
void i2c_master_rx(i2c_handle_t *i2cx)
void i2c_slave_rx(i2c_handle_t *i2cx)
``` 
These two APIs receive data as master or slave. Same as the transmit API, they only enable interrupt and let the handler do the rest.
```
void i2c_irq_handler(i2c_handle_t *i2cx)
```
When an interrupt is generated, this handler is called. The handler will check the interrupt status register and run the corresponding subroutine.

## I2C Operations
### Connection
There are two lines involved in I2C transmission, SDA and SCL. Different from SPI, there can be more than one master device connected to the SDA and SCL line, and each of them can communicate with the slaves.
#### SCL
Similar to SPI, I2C is also a synchronized protocol. There is a clock line SCL on which the master provides its clock signal. 
#### SDA
This line conveys the data. Different from SPI, this line is bidirectional, which means data can be transmitted in both direction on this one line. Both SCL and SDA line need a pull up resistor. The pull-up resistor can be enabled in the I2C module or at the GPIO pin.
### Transmission Frame
Compared to SPI, the data frame of I2C is more complicated.
#### START and STOP Conditions
I2C defines two states indicating the start and stop of data transmission. When SCL is high and SDA transits from high to low, a START is generated. When SCL is high and SDA transits from low to high, a STOP is generated. After a START condition, data start to flow on SDA line and the bus is considered to be busy. After a STOP condition, no data will stay on SDA and the bus is free.
#### Slave Address
In I2C communication, each slave device has an own address, and the master locates the slave using its own address. In dual address mode, the slave can have a alternative address. The address is usually 7-bit, and 10-bit address is also allowed
#### Data Direction
Since there is only one data line, data direction is indicated in each transmision. There is a read/write bit in each transmission, where 1 indicates a master read and 0 indicates a master write.
#### ACK and NACK
I2C protocol allows master/slave to send acknowlegde signal to indicate whether it's ok to continue the transmission. In the ACK phase, if SDA is 0, an ACK signal is sent and the transmission continues. If SDA is 1, a NACK is sent and the transmission aborts.
#### Repeated Start
In some cases, the master need to read before write or write before read. In the transition from write to read, a STOP needs to be generated before the next START. However, since there are multiple slaves on the bus line, after a STOP is generated, the slave might be taken by another master before the next START is generated. 
I2C allows repeated START. Repeated START allows the master to generate the next START without generating a STOP. When the master wants to switch from write to read or the other way around, a repeated START is generated.
#### Arbitration
Is is possible that multiple masters place START on SDA at the same time. In this case, arbitration takes place. When multiple masters place START within minimum hold time, the first master places 1 on SDA while the others transmits 0 and wait until SDA becomes idle again.
#### Clock Stretching
Different from SPI where the slave has no control on clock, when the slave is unavailable to transmit any data for the master, it pulls the SCL line to low until it's available. This is called clock stretching.
### Work Flow
There are different phases in a complete transmission, and indication signals are sent in each phase.
#### START Phase
In the first clock cycle, a START condition is generated by the master to start the transmission
#### Address Phase
Slave address is sent by the master after a START condition.
##### 7-Bit Address
In 7-bit addressing mode, the slave address is transmitted on SDA in the next 7 clock cycles
##### 10-Bit Address
Some times 7-bit address space is not sufficient for the number of slaves, so 10-bit address may be used. In the case of 10-bit address, a sequence of 11110 is sent first indicating a 10-bit address, then A9 and A8 are sent. Since the 8th and 9th clock cycle is always the r/w and ACK phase, the rest 8 bits are transmitted from the 10th clock cycle.
#### Read and Write phase
The 8th clock cycle is always the r/w phase. In this clock cycle, SDA is 1 for master read and 0 for master write
#### ACK Phase
The 9th clock cycle is always the ACK phase. In this clock cycle, if SDA is 1, it is treated as a NACK, otherwise it is treated as an ACK.
#### Data Phase
##### Master Write
The master generated a read in the r/w clock cycle. If the slave generates an ACK, a byte is sent, then the slave generates an ACK again, another byte is sent. When no nore data is to sent to the slave, the master generates a STOP and the transmission terminates.
##### Master Read
The master generates a write in the r/w clock cycle. The slave sends an ACK, and then transmit data to the master. After receiving a byte from the slave, the master generates an ACK for the next byte. When the master doesn't want to receive more data, it generates a NACK, followed by a STOP confition.
### Interrupts
In Tiva TM4C123 microcontroller, the following interrupts are available
- Master transaction complete: Either Tx or Rx is completed
- Master arbitration lost
- Master error
- Slave data interrupt: data is received or required
- Slave start interrupt: a START is generated
- Slave stop interrupt: a STOP is generated