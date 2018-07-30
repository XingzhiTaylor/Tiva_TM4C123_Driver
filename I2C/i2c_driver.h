#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "TM4C123GH6PM.h"
#include <stdbool.h>

/****************************************************************************
 *                                                                          *
 *                  I2C Register Bit Definitions			                    	*
 *                                                                          *
 ****************************************************************************/
 
/***************Read-Only Master Control/Status (MCS) Register****************/
#define I2C_BUSY										( (uint32_t) 1 << 0 )
#define I2C_ERROR										( (uint32_t) 1 << 1 )
#define I2C_ADRACK									( (uint32_t) 1 << 2 )				// Acknowlegde address
#define I2C_DATACK									( (uint32_t) 1 << 3 )				// Acknowlegde data
#define I2C_ARBLST									( (uint32_t) 1 << 4 )				// Arbitration lost
#define I2C_IDLE										( (uint32_t) 1 << 5 )
#define I2C_BUSBSY									( (uint32_t) 1 << 6 )				// Bus busy
#define I2C_CLKTO										( (uint32_t) 1 << 7 )				// Clock timeout error

/***************Write-Only Master Control/Status (MCS) Register****************/
#define I2C_RUN											( (uint32_t) 1 << 0 )				// Enable I2C master
#define I2C_START										( (uint32_t) 1 << 1 )				// Generate START
#define I2C_STOP										( (uint32_t) 1 << 2 )				// Generate STOP
#define I2C_ACK											( (uint32_t) 1 << 3 )				// Acknowlegde enable
#define I2C_HS											( (uint32_t) 1 << 4 )				// High speed enable

/********************Master Time Period (MTPR) Register*********************/
#define I2C_TPR_HS									( (uint32_t) 1 << 7 )				// High speed enable

/****************************Master Interrupts*****************************/
#define I2C_INT_MASTER							( (uint32_t) 1 << 0 )				// Master Interrupt
#define I2C_INT_CLKTO								( (uint32_t) 1 << 1 )				// Clock timeout Interrupt

/********************Master Configuration (MCR) Register*********************/
#define I2C_LOOPBACK								( (uint32_t) 1 << 0 )				// Loopback mode enable
#define I2C_MASTER									( (uint32_t) 1 << 4 )				// Master mode enable
#define I2C_SLAVE										( (uint32_t) 1 << 5 )				// Slave mode enable
#define I2C_GLF_EN									( (uint32_t) 1 << 6 )				// Glitch filter enable

/********************Master Bus Monitor (MBMON) Register*********************/
#define I2C_MASTER_SCL							( (uint32_t) 1 << 0 )				// SCL signal
#define I2C_MASTER_SDA							( (uint32_t) 1 << 1 )				// SDA signal

/********************Master Configuration2 (MCR2) Register*********************/
#define I2C_GFPW_BYPASS							( (uint32_t) 0 << 4 )				// Bypass
#define I2C_GFPW_1_CLK							( (uint32_t) 1 << 4 )				// 1 clock wide
#define I2C_GFPW_2_CLK							( (uint32_t) 2 << 4 )				// 2 clocks wide
#define I2C_GFPW_3_CLK							( (uint32_t) 3 << 4 )				// 3 clocks wide
#define I2C_GFPW_4_CLK							( (uint32_t) 4 << 4 )				// 4 clocks wide
#define I2C_GFPW_8_CLK							( (uint32_t) 5 << 4 )				// 8 clocks wide
#define I2C_GFPW_16_CLK							( (uint32_t) 6 << 4 )				// 16 clocks wide
#define I2C_GFPW_31_CLK							( (uint32_t) 7 << 4 )				// 31 clocks wide

/******************Read-Only Slave Control/Status (SCSR)*********************/
#define I2C_RX_RQST									( (uint32_t) 1 << 0 )				// Receive request
#define I2C_TX_RQST									( (uint32_t) 1 << 1 )				// Transmit request
#define I2C_FST_BYT_RX							( (uint32_t) 1 << 2 )				// First byte receive
#define I2C_ADDR_MATCH							( (uint32_t) 1 << 3 )				// Enable I2C master

/******************Write-Only Slave Control/Status (SCSR)*********************/
#define I2C_SLAVE_ACTIVE						( (uint32_t) 1 << 0 )				// Acknowlegde enable

/*****************************Slave Interrupts******************************/
#define I2C_INT_DATA								( (uint32_t) 1 << 0 )				// Slave data interrupt
#define I2C_INT_START								( (uint32_t) 1 << 1 )				// Start condition interrupt
#define I2C_INT_STOP								( (uint32_t) 1 << 2 )				// Stop condition interrupt

/*****************************Slave Dual Address******************************/
#define I2C_OAR2_EN									( (uint32_t) 1 << 7 )				// Dual address enable
#define I2C_OAR2_DN									~( (uint32_t) 1 << 7 )			// Dual address disable

/*****************************Slave ACK Control******************************/
#define I2C_ACK_OEN									( (uint32_t) 1 << 0 )				// ACK Override enable
#define I2C_ACK_OVAL								( (uint32_t) 1 << 1 )				// ACK Override value

#define I2C_PC_HS										( (uint32_t) 1 << 0 ) 			// Peripheral Configuration high speed

#define I2C_INIT_UNSET							0
#define I2C_HNDL_UNSET							0

/****************************************************************************
 *                                                                          *
 *                  I2C Register Data Structures			                    	*
 *                                                                          *
 ****************************************************************************/
 
typedef struct {
	uint32_t Master;						// Master or Slave
	uint32_t Loopback;					// Loop back mode
	uint32_t Clock_period;			// Clock period
	uint32_t High_speed;				// High speed mode enable
	uint32_t Glitch_filter;			// Glitch filter enable
	uint32_t GF_width;					// Filter width
	uint32_t Slave_own_address;	// Address of slave. Not set if master is selected
	uint32_t Dual_address_en;		// Dual address for slave enable
	uint32_t Dual_address;			// Dual address of slave
} i2c_init_t;

typedef struct
{
	I2C0_Type		               *Instance;  /*!< I2C registers base address     */
	i2c_init_t                 Init;       /*!< I2C communication parameters   */
	uint32_t									 Slave_addr; /*!< Slave address to transmit data   */
	uint8_t                    *pBuffPtr;  /*!< Pointer to I2C transfer buffer */
	uint32_t                   XferSize;   /*!< I2C transfer size              */
	__IO uint32_t              XferCount;  /*!< I2C transfer counter           */
	uint32_t 									 ErrorCode;     
}i2c_handle_t;

/***************************Clock Init Macros*****************************/
#define RCC_I2C0_CLK_ENABLE()       volatile unsigned long delay0;\
                                    SYSCTL->RCGCI2C |= (0x01 << 0);\
                                    delay0 = SYSCTL->RCGCI2C;
#define RCC_I2C1_CLK_ENABLE()       volatile unsigned long delay1;\
                                    SYSCTL->RCGCI2C |= (0x01 << 1);\
                                    delay1 = SYSCTL->RCGCI2C;
#define RCC_I2C2_CLK_ENABLE()       volatile unsigned long delay2;\
                                    SYSCTL->RCGCI2C |= (0x01 << 2);\
                                    delay2 = SYSCTL->RCGCI2C;
#define RCC_I2C3_CLK_ENABLE()       volatile unsigned long delay3;\
                                    SYSCTL->RCGCI2C |= (0x01 << 3);\
                                    delay3 = SYSCTL->RCGCI2C;

/**
	* @brief  API used to do init the i2c module
	* @param  *i2cx : I2C module to initialize
  * @retval none
*/
void i2c_init(i2c_handle_t *i2cx);

/**
	* @brief  API used to do master data transmission 
	* @param  *I2Cx : Base address of the I2C  
  * @param  *buffer : pointer to the tx buffer 
  * @param  len : len of tx data
  * @retval none
	*/
void i2c_master_tx(i2c_handle_t *i2cx, uint8_t *tsm_buffer, uint32_t len);

/**
	* @brief  API used to do slave data transmission 
	* @param  *I2Cx : Base address of the I2C  
  * @param  *buffer : pointer to the tx buffer 
  * @param  len : len of tx data
  * @retval none
	*/
void i2c_slave_tx(i2c_handle_t *i2cx, uint8_t *tsm_buffer, uint32_t len);


/**
	* @brief  API used to do master data reception 
	* @param  *I2Cx : Base address of the I2C  
  * @param  *buffer : pointer to the rx buffer 
  * @param  len : len of rx data
  * @retval none
	*/
void i2c_master_rx(i2c_handle_t *i2cx, uint8_t *rcv_buffer, uint32_t len);


/**
	* @brief  API used to do slave data reception 
	* @param  *I2Cx : Base address of the I2C  
  * @param  *buffer : pointer to the rx buffer 
  * @param  len : len of rx data
  * @retval none
	*/
void i2c_slave_rx(i2c_handle_t *i2cx, uint8_t *rcv_buffer, uint32_t len);

/**
  * @brief  This function handles SPI interrupt request.
  * @param  spi_handle: pointer to a spi_handle_t structure that contains
  *                the configuration information for SPI module.
  * @retval none
  */
void i2c_irq_handler(i2c_handle_t *i2cx);

#endif