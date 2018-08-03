#include <TM4C123GH6PM.h>
#include <stdlib.h>
 
char error =0;  //global variable for error
void I2C0_ini_master(){
	SYSCTL->RCGCI2C|=0x1; //enabling clock for I2C0 module		 
	SYSCTL->RCGCGPIO|=0x2;//enabling clock for GPIOB
	while((SYSCTL->PRGPIO&0x2)==0){} //waiting until GPIO is ready 
	GPIOB->AFSEL=0xc;     //selecting for alternative functions
	GPIOB->ODR=0x08;      //enabling open drain for SDA line
	GPIOB->DEN=0x0c;      //enabling digtal function at PB2 and PB3
	GPIOB->PCTL=(GPIOB->PCTL&0xFFFF00FF)+0x3300;//PCTL value for I2C function
	I2C0->MCR=0x10;       //master mode
	I2C0->MTPR=0x7;       //100 kbps
}
 
void I2C0_write(char data,char addr){  //function for writing the slave address with data  
	while((I2C0->MCS&(1<<0))!=0){} //wait until I2C is not ready
	I2C0->MSA=(addr<<1)&0xFE;      //shifting the address for adding the R/W bit
	I2C0->MSA&=~0x01;              //0 means we want to perform write function
	I2C0->MDR=data&0xFF;           //putting data to the slave
	I2C0->MCS=0x7;                 //Start Run and Stop mode
	while((I2C0->MCS&(1<<0))!=0){} //wait until the I2C0 module is busy
if (((I2C0->MCS&(1<<1))==0))error=1;        //check for error
  
}
 
 
int main(){
	SystemInit(); 
	I2C0_ini_master(); //initializing in master mode
        I2C0_write('A',0x5);//writing the slave at address 0x5 with data A
return 0;
}
