// hello_launchpad.c
#include <stdint.h>
#include <stdlib.h>
#include "TM4C123GH6PM.h"



void spi_master_ini(void){
		uint8_t data = 'A';
		SYSCTL->RCGCSSI|=(1<<2);
	SSI2->DR=data;
		SYSCTL->RCGC2|=(1<<1);
		GPIOB->AFSEL|=(1<<4)|(1<<5)|(1<<6)|(1<<7);
		GPIOB->PCTL=0x22220000;
		GPIOB->DEN|=(1<<4)|(1<<5)|(1<<6)|(1<<7);
		GPIOB->PUR|=(1<<4)|(1<<5)|(1<<6)|(1<<7);
	SSI2->DR=data;
		SSI2->CR1=0;
	SSI2->DR=data;
		SSI2->CC=0;
	SSI2->DR=data;
		SSI2->CPSR=64;
	SSI2->DR=data;
		SSI2->CR0=0x7;
	SSI2->DR=data;
		SSI2->CR1|=(1<<1);
	SSI2->DR=data;
}

void send_byte(char data){
		SSI2->DR=data;
	while((SSI2->SR&(1<<0))==0);
}

void send_str(char *buffer){
  while(*buffer!=0){ 
  send_byte(*buffer);
		buffer++;
	}
}

int main(){
	
	SystemInit();
	spi_master_ini();
	send_str("Hello SPI \ncommunication");
		
}

