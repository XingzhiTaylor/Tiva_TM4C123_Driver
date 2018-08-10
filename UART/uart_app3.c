#include "uart_debug.h"

void delay(unsigned long halfsecs){
  unsigned long count;
  
  while(halfsecs > 0 ) { // repeat while there are still halfsecs to delay
    count = 1538460; // 400000*0.5/0.13 that it takes 0.13 sec to count down to zero
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    halfsecs--;
  }
}

int main(void){
	DebugUartStart(DEBUG_UART_BAUD_115200);
	while(1){
		DebugUartPrintf("Hello World!");
		delay(2);
		DebugUartPrintf("%d + %d = %d", 1, 1, 2);
		delay(2);
		DebugUartPrintf("%.2f + %.3f = %.4f", 0.1, 0.21, 0.310);
		delay(2);
		DebugUartPrintf("%s and %s are my favourite.", "Apple", "pear");
		delay(2);
	}
}