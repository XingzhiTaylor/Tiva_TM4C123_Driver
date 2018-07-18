#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include "TM4C123GH6PM.h"

/****************************************************************************
 *                                                                          *
 *                  SPI Register Bit Definitions			                    	*
 *                                                                          *
 ****************************************************************************/
 
/*********************Bit Definitions for CR0 Register**********************/
#define SPI_FIRST_CLOCK_TRANS 																	0
#define SPI_SECOND_CLOCK_TRANS 																	1

#define SPI_CLOCK_POL_LOW 																			0
#define SPI_CLOCK_POL_HIGH 																			1

#define SPI_FREE_SCL_SPI																		 0x00
#define SPI_TI_SYN_SRL																			 0x01
#define SPI_MICROWIRE																				 0x02

/*********************Bit Definitions for CR1 Register**********************/
#define SPI_FIFO_HLF_FULL																				0
#define SPI_EOT_ENABLE																					1

#define SPI_SLV_OUTPUT_ENABLE																		0
#define SPI_SLV_OUTPUT_DISABLE																	1

#define SPI_MASTER_SEL																					0
#define SPI_SLAVE_SEL																						1

#define SPI_DISABLE																							0
#define SPI_ENABLE																							1

#define SPI_NORMAL																							0
#define SPI_LOOPBACK																						1

/*********************Bit Definitions for ISR Register*********************/
#define SPI_BUSY 														( (uint32_t) 1 << 4 )
#define SPI_FIFO_FULL												( (uint32_t) 1 << 3 )
#define SPI_FIFO_NOT_EMPTY									( (uint32_t) 1 << 2 )
#define SPI_FIFO_NOT_FULL										( (uint32_t) 1 << 1 )	
#define SPI_FIFO_EMPTY											( (uint32_t) 1 << 0 )	

/*********************Bit Definitions for ICC Register*********************/
#define SPI_SYS_CLK													 									0x0
#define SPI_PIOSC														 									0x5

/***************************Clock Init Macros*****************************/
#define RCC_SSI0_CLK_ENABLE()       volatile unsigned long delay;\
                                    SYSCTL->RCGCSSI |= (0x01 << 0);\
                                    delay = SYSCTL->RCGCSSI;
#define RCC_SSI1_CLK_ENABLE()       volatile unsigned long delay;\
                                    SYSCTL->RCGCSSI |= (0x01 << 1);\
                                    delay = SYSCTL->RCGCSSI;
#define RCC_SSI2_CLK_ENABLE()       volatile unsigned long delay;\
                                    SYSCTL->RCGCSSI |= (0x01 << 2);\
                                    delay = SYSCTL->RCGCSSI;
#define RCC_SSI3_CLK_ENABLE()       volatile unsigned long delay;\
                                    SYSCTL->RCGCSSI |= (0x01 << 3);\
                                    delay = SYSCTL->RCGCSSI;

/*************************SPI Data Structures*****************************/
typedef struct {
	uint32_t Clock_phase;
	uint32_t Clock_polarity;
	uint32_t Frame_format;
	uint32_t Mode;							// Slave or master
	uint32_t EOT_enable;
	uint32_t Loopback;
	uint32_t Clock;
	uint32_t Clock_prescale;		// CPSR Register
	uint32_t Clock_scaler;      // SCR value in CR0 register
} spi_conf_t;

#endif
