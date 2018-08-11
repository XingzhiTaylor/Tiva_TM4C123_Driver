## Overview
This is a library of a DIY `printf()` function. This function takes a string as argument, and print it on the PC using UART protocol.
## APIs
### void DebugUartStart(uint32_t baudRate) 
This function initialized the UART0 module with the given baud rate. First, it calls the `gpio_init()` function in the GPIO driver to initialize the UART functionality of the GPIO pin. Then, it calls the UART driver APIs to initailize the UART0 module. The baud rate divisor is calculated using the formula
```
divisor = Sysclk / (16 * baud rate)
```
And the integer part and fraction part are set. After the initialization, we are good to go
### void DebugUartPrintf(char *format, ...)
The print function takes a format string as an argument. Then, the string is formated using the built in VA APIs `va_start()` and `vsprintf()`. The formatted string is transmitted by the UART transmission API to the PC.
The point of this library is to test the UART API, so I didn't implement the string formatting feature.

The library is created using Keil 5 IDE. The libraries is used in UART sample app 3.
