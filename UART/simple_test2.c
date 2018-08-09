#include "TM4C123.h" // Device header
#include <stdint.h>
 
char UART1Rx(void); 
void delayMs(int n);
 
int main(void) 
{
 char c;
 
 SYSCTL->RCGCUART |= 2; /* provide clock to UART1 */
 SYSCTL->RCGCGPIO |= 2; /* enable clock to PORTA */
 SYSCTL->RCGCGPIO |= 0x20; /* enable clock to PORTF */
 
 /* UART1 initialization */
 UART1->CTL = 0; /* disable UART1 */
 UART1->IBRD = 325; /* 16MHz/16=1MHz, 1MHz/104=9600 baud rate */
 UART1->FBRD = 33; /* fraction part, see Example 4-4 */
 UART1->CC = 0; /* use system clock */
 UART1->LCRH = 0x60; /* 8-bit, no parity, 1-stop bit, no FIFO */
 UART1->CTL = 0x301; /* enable UART1, TXE, RXE */
 
 /* UART1 TX0 and RX0 use PA0 and PA1. Set them up. */
 GPIOB->DEN = 0x03; /* Make PA0 and PA1 as digital */
 GPIOB->AFSEL = 0x03; /* Use PA0,PA1 alternate function */
 GPIOB->PCTL = 0x11; /* configure PA0 and PA1 for UART */
 
 GPIOF->DIR = 0x0E; /* configure Port F to control the LEDs */
 GPIOF->DEN = 0x0E;
 GPIOF->DATA = 0;
 
 for(;;)
 {
 c = UART1Rx(); /* get a character from UART */
 GPIOF->DATA = c << 1; /* shift left and write it to LEDs */ 
 } 
} /* UART1 Receive */ /* This function waits until a character is received then returns it. */ 
char UART1Rx(void) { 
	char c; while((UART1->FR & 0x10) != 0); /* wait until the buffer is not empty */
 c = UART1->DR; /* read the received data */
 return c; /* and return it */
}
 
/* Append delay functions and SystemInit() here */
void delayMs(int n){
int i,j; 
 
 for(i=0;i<n;i++){
 for(j=0;j<3180;j++)
 {}
 }
}