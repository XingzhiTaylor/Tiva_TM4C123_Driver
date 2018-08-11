## Overview
This driver is capable of UART setup and communications. Different from the previous two drivers, the UART driver APIs are broken down so the user has more flexibility. 

## API descriptions
There is no initialization or run time handler data structures. The use can pick any combination of parameters to initialize the UART module. 
APIs are implemented for every control registers. They are able to set a new parameter, get the current value of a parameter, check the status of flags, read and write data. For detailes descriptions, see `uart_driver.h`.

## Sample UART Apps
### App 1
This app enables UART0 module and communicates with PC using serial communication at a baud rate of 9600. The app uses polling, and checks for input from the PC in every loop. If the input is 'a', send a response string "hello" and turn on green LED. Otherwise, sens a response string "bye" and turn on red LED.
### App 2
This app is developed based on app 1 using the same baud rate and UART module. Interrupt is used in both receive and transmit to test the driver APIs. 
The program does nothing in the loop. When button 1 is pressed, the microcontroller will send response "Switch 1 pressed". When button 2 is pressed, the microcontroller will send response "Switch 2 pressed". If the signal is not recognizable, the microcontroller will send response "Oh Geez!!!!".
When the user types a character on the keyboard, the microcontroller will receive and check it. If the char is an upper case letter, green LED will light. If the char is a lower case letter, red LED will light. If the char is a number, blue LED will light. Otherwise, blue LED will blink. 
### App 3
This app is a test of the `DebugUartPrintf()` library. Simple srting, string with integer, string with floating point number, and string with string arguments are printed using `DebugUartPrintf()` to the PC.

## UART Operations
### Connection
There are two lines involved in UART transmission, simply Tx and Rx. Unlike synchronized protocols such as SPI and I2C, UART is asynchronized and there is no clock shared between two devices. The devices use oversampling to read data from data line. The oversampling frequency can be 8 times or 16 times of the internal clock frequency, and TM4C123 uses 16 times.
Some UART modules have two more pins, CTS (Clear to Send) and RTS (Request to send). The CTS of one device is connected to the RTS of the other device. 
### Baud Rate
Baud Rate is the rate at which the data is transmitted on the line and has the unit of Bps (bit per sec). Baud rate devisor is the parameter to set the baud rate in the UART module. Baud rate divisor is the number of internal sampling periods for which one bit of data stays on the line. The higher the baud rate, the shorter a bit of data stay on the line, and the smaller the baud rate divisor is.
In T4MC123, the oversampling rate is 16 times. Then, the baud rate divisor is given by:
```
divisor = Sysclk / (16 * baud rate)
```
The parameter has an integer part and a fraction part. They are set by two control registers.
The baud rate of two device **MUST MATCH** for them to talk. Otherwise wrong data will be received.
### Start and Stop Bit
In telegraph transmission, the idle state of the line is high to indicate the line is working. UART inherits this feature, and the idle state of a UART line is 1. 
There are synchronizations bit to make sure the other device knows what's happening. At the start of a transmission, the transmitter pulls the line down to 0 indicating the start. When the transmission is finished, the transmitter pulls the line up to 1, indicating the end. There might be one two end bits to make sure the receiver receives it.
### Data Transmission
Data length can be 5-8 bits. In UART, LSB is transmitted first.
### Parity Bit
Parity bit is a method to check data corruption, and the parity bit is added to the data bit. In even parity check, if the number of 1's in the data string is odd, the parity bit is set to 1 so that the number of 1's including parity bit is even. In odd parity check, if the number of 1's is even, the parity bit is set to 1 to make the total number of 1's odd. The parity bit can be enabled or disabled in the UART module.
### FIFO Operation
The data to transmit or received are stored in a FIFO before reaches the Tx/Rx logic. In Tiva TM4C123 MCU, there is hardware monitoring the filling status of the FIFO, setting corresponding flags in FR registers. Interrupt can also be generated. The software can monitor the flag or listen to the interrupt for the filling status of FIFO.
### Flags
There is a flag register that allows user to check for FIFO status. The flag register also contains the busy and clear to send flag for flow control.
### Errors
Below are the errors that might occur in a UART transmission. They can be checked and monitored using control registers and interrupts. 
- Overrun error: FIFO is full, resulting in data loss
- Break error: The receive signal is held LOW for more than a transmission length (start + data + parity + stop)
- Frame error: No valid stop bit is detected
- Parity error: Parity does not match the selscted type

### Work Flow
- The transmitter generate a start bit by pulling the line to low
- The transmitter sends 5-8 bit data
- If enabled, a parity bit is sent
- A stop bit is generated by the transmitter
- If more bytes to send, another start bit is generated

### Interrupts
In TM4C123 MCU, the following interrupts are available:
- Overrun error
- Break error
- Parity error
- Framing error
- Transmit: the filling level of Tx FIFO is lower than set
- Receive: the filling level of Rx FIFO is higher than set

