#include "TM4C123GH6PM.h"
#include "i2c_driver.h"

/**
	* @brief  API used to do init the i2c module
	* @param  *i2cx : I2C module to initialize
  * @retval none
*/
void i2c_init(i2c_handle_t *i2cx){
	if(i2cx->Init.Master == I2C_MASTER){
		// The I2C module is configured as master.
		// Choose master
		i2cx->Instance->MCR |= I2C_MASTER;
		// Set clock period
		i2cx->Instance->MTPR |= i2cx->Init.Clock_period;
	} else if(i2cx->Init.Master == I2C_SLAVE){
		// The I2C module is configured as slave
		// Choose slave
		i2cx->Instance->MCR |= I2C_SLAVE;
		// Enable slave operation
		i2cx->Instance->SCSR |= I2C_SLAVE_ACTIVE;
		// Set own address of the slave
		i2cx->Instance->SOAR = i2cx->Init.Slave_own_address;
		if(i2cx->Init.Dual_address_en){
			// If dual address is needed, enable the functionality
			// Write the alternative address
			i2cx->Instance->SOAR2 = i2cx->Init.Dual_address;
			i2cx->Instance->SOAR2 |= I2C_OAR2_EN;
		} else {
			// Disable dual address if not needed
			i2cx->Instance->SOAR2 &= I2C_OAR2_DN;
		}
	}
	if(i2cx->Init.High_speed){
		// High speed mode config
		if(i2cx->Instance->PP & I2C_PC_HS){
			// If the interface is capable of high speed mode, enable it
			i2cx->Instance->PC |= I2C_PC_HS;
		} else {
			// If the interface is not capable of high speed mode, return
			return;
		}
		if(i2cx->Init.Master == I2C_MASTER){
			// If the module is master, set the master control register to enable high speed
			i2cx->Instance->MTPR |= I2C_TPR_HS;
			i2cx->Instance->MCS |= I2C_HS;
		}
	}
}
		
			

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