#include "TM4C123GH6PM.h"
#include "i2c_driver.h"

/**
  * @brief  This function handles I2C errors.
  * @param  i2c_handle: pointer to a i2c_handle_t structure that contains
  *                the configuration information for I2C module.
  * @retval none
  */
void i2c_exception_handler(i2c_handle_t *i2cx);

/**
  * @brief  This function checks I2C master errors.
  * @param  i2c_handle: pointer to a i2c_handle_t structure that contains
  *                the configuration information for I2C module.
  * @retval none
  */
void i2c_check_master_error(i2c_handle_t *i2cx){
	if(i2cx->Instance->MCS & I2C_ERROR){
		i2cx->ErrorCode = I2C_ERROR;  // Error code be modified
		i2c_exception_handler(i2cx);
	}
}

/**
  * @brief  This function sends I2C slave ACK response.
  * @param  i2c_handle: pointer to a i2c_handle_t structure that contains
  *                the configuration information for I2C module.
  * @retval none
  */
void i2c_slave_rx_ack(i2c_handle_t *i2cx){
	i2cx->Instance->SACKCTL |= 0x1;
}

/**
  * @brief  This function sends I2C slave NACK response.
  * @param  i2c_handle: pointer to a i2c_handle_t structure that contains
  *                the configuration information for I2C module.
  * @retval none
  */
void i2c_slave_rx_nack(i2c_handle_t *i2cx){
	i2cx->Instance->SACKCTL |= 0x3;
}

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
void i2c_master_tx(i2c_handle_t *i2cx){				// Repeated start functionality to be implemented
	// Write slave address to MSA
	i2cx->Instance->MSA = ( i2cx->Slave_addr << 1 );
	// Set bit 0 of MSA to 0 for transmit
	i2cx->Instance->MSA &= ~0x01;
	// Write data to MDR
	i2cx->Instance->MDR = (*i2cx->pBuffPtr);
	// Increment counter
	i2cx->pBuffPtr ++;
	// Update counter
	i2cx->XferCount ++;	
	// Wait until i2c bus is available
	while(i2cx->Instance->MCS & I2C_BUSBSY);
	
	
	// Start transmission
	if(i2cx->XferSize == 1){
		// If there is only one byte to send, send stop right after
		i2cx->Instance->MCS |= I2C_RUN;
		i2cx->Instance->MCS |= I2C_START;
		i2cx->Instance->MCS |= I2C_STOP;
		i2cx->Instance->MCS &= ~I2C_HS;
		// Wait until finish
		while(i2cx->Instance->MCS & I2C_BUSY);
		// Check for errors
		i2c_check_master_error(i2cx);
		return;
	}
	
	
	// Start transmission
	i2cx->Instance->MCS |= I2C_RUN;
	i2cx->Instance->MCS |= I2C_START;
	i2cx->Instance->MCS &= ~I2C_STOP;
	i2cx->Instance->MCS &= ~I2C_HS;
	
	
	while(i2cx->XferCount != i2cx->XferSize){
		// Wait for the transmission of the previous byte
		while(i2cx->Instance->MCS & I2C_BUSY);
				
		// Check for error
		if(i2cx->Instance->MCS & I2C_ERROR){
			// Check for arbitration lost
			if(i2cx->Instance->MCS & I2C_ARBLST){
				// In the case of arbitration lost, run error service immediately
				i2cx->ErrorCode = I2C_ARBLST;
				i2c_exception_handler(i2cx);
			}
			// Stop transmission, stop I2C
			i2cx->Instance->MCS &= ~I2C_RUN;
			i2cx->Instance->MCS &= ~I2C_START;
			i2cx->Instance->MCS |= I2C_STOP;
			i2cx->Instance->MCS &= ~I2C_HS;
			// Enter error service
			i2cx->ErrorCode = I2C_ERROR;  // Error code be modified
			i2c_exception_handler(i2cx);
		}
				
		// Write the next data byte to MDR
		i2cx->Instance->MDR = (*i2cx->pBuffPtr);
		// Increment counter
		i2cx->pBuffPtr ++;
		// Update counter
		i2cx->XferCount ++;
				
		if(i2cx->XferCount != i2cx->XferSize){
			// Transmit not yet finished. Keep transmitting
			i2cx->Instance->MCS |= I2C_RUN;
			i2cx->Instance->MCS &= ~I2C_START;
			i2cx->Instance->MCS &= ~I2C_STOP;
			i2cx->Instance->MCS &= ~I2C_HS;
		} else {
			// Transmit finished. Break from the loop
			// Send stop
			i2cx->Instance->MCS |= I2C_RUN;
			i2cx->Instance->MCS &= ~I2C_START;
			i2cx->Instance->MCS |= I2C_STOP;
			i2cx->Instance->MCS &= ~I2C_HS;
			break;
		}
	}
	// Wait until finish
	while(i2cx->Instance->MCS & I2C_BUSY);
	// Error check
	i2c_check_master_error(i2cx);
}

/**
	* @brief  API used to do master data transmission in high speed mode
	* @param  *I2Cx : Base address of the I2C  
  * @param  *buffer : pointer to the tx buffer 
  * @param  len : len of tx data
  * @retval none
	*/
void i2c_master_hs_tx(i2c_handle_t *i2cx){
	// Write slave address to MSA
	i2cx->Instance->MSA = ( i2cx->Slave_addr << 1 );
	// Set bit 0 of MSA to 0 for transmit
	i2cx->Instance->MSA &= ~0x01;
	// HS mode setup
	i2cx->Instance->MCS |= I2C_RUN;
	i2cx->Instance->MCS |= I2C_START;
	i2cx->Instance->MCS &= ~I2C_STOP;
	i2cx->Instance->MCS &= ~I2C_ACK;
	i2cx->Instance->MCS |= I2C_HS;
	// Wait until i2c is available
	while(i2cx->Instance->MCS & I2C_BUSY);
	// Error check
	if(i2cx->Instance->MCS & I2C_ERROR) return;
	
	i2c_master_tx(i2cx);
}
		

/**
	* @brief  API used to do slave data transmission 
	* @param  *I2Cx : Base address of the I2C  
  * @param  *buffer : pointer to the tx buffer 
  * @param  len : len of tx data
  * @retval none
	*/
void i2c_slave_tx(i2c_handle_t *i2cx){
	while(i2cx->Instance->SCSR & I2C_TX_RQST){
		// Write data to data register
		i2cx->Instance->SDR = (*i2cx->pBuffPtr);
		// Increment pointer
		i2cx->pBuffPtr ++;
		// Update counter
		i2cx->XferCount ++;
		if(i2cx->XferCount == i2cx->XferSize){
			break;
		}
	}
}


/**
	* @brief  API used to do master data reception 
	* @param  *I2Cx : Base address of the I2C  
  * @param  *buffer : pointer to the rx buffer 
  * @param  len : len of rx data
  * @retval none
	*/
void i2c_master_rx(i2c_handle_t *i2cx){
	// Write slave address to MSA
	i2cx->Instance->MSA = ( i2cx->Slave_addr << 1 );
	// Set bit 0 of MSA to 1 for receive
	i2cx->Instance->MSA |= 0x01;
	// Wait until i2c bus is available
	while(i2cx->Instance->MCS & I2C_BUSBSY);	
	
	if(i2cx->XferSize == 1){
		// If there is only one byte to send, send stop right after
		i2cx->Instance->MCS |= I2C_RUN;
		i2cx->Instance->MCS |= I2C_START;
		i2cx->Instance->MCS |= I2C_STOP;
		i2cx->Instance->MCS |= I2C_ACK;
		i2cx->Instance->MCS &= ~I2C_HS;
		// Wait until finish
		while(i2cx->Instance->MCS & I2C_BUSY);
		// Check for errors
		if(i2cx->Instance->MCS & I2C_ERROR){
			i2cx->ErrorCode = I2C_ERROR;  // Error code be modified
			i2c_exception_handler(i2cx);
		} else {
			// Read the next data byte from MDR
			(*i2cx->pBuffPtr) = i2cx->Instance->MDR;
		}
		return;
	}
	
	// Start receiving
	i2cx->Instance->MCS |= I2C_RUN;
	i2cx->Instance->MCS |= I2C_START;
	i2cx->Instance->MCS &= ~I2C_STOP;
	i2cx->Instance->MCS |= I2C_ACK;
	i2cx->Instance->MCS &= ~I2C_HS;
	
	
	while(i2cx->XferCount != i2cx->XferSize){
		// Wait for the receiving of the previous byte
		while(i2cx->Instance->MCS & I2C_BUSY);
		
		// Check for error
		if(i2cx->Instance->MCS & I2C_ERROR){
			// Check for arbitration lost
			if(i2cx->Instance->MCS & I2C_ARBLST){
				// In the case of arbitration lost, run error service immediately
				i2cx->ErrorCode = I2C_ARBLST;
				i2c_exception_handler(i2cx);
			}
			// Stop receiving, stop I2C
			i2cx->Instance->MCS &= ~I2C_RUN;
			i2cx->Instance->MCS &= ~I2C_START;
			i2cx->Instance->MCS |= I2C_STOP;
			i2cx->Instance->MCS &= ~I2C_HS;
			// Enter error service
			i2cx->ErrorCode = I2C_ERROR;  // Error code to be modified
			i2c_exception_handler(i2cx);
		}
		
		// Read the next data byte from MDR
		(*i2cx->pBuffPtr) = i2cx->Instance->MDR;
		// Increment counter
		i2cx->pBuffPtr ++;
		// Update counter
		i2cx->XferCount ++;
		
		if(i2cx->XferCount != i2cx->XferSize - 1){
			// Receive not yet finished. Keep receiving
			i2cx->Instance->MCS |= I2C_RUN;
			i2cx->Instance->MCS &= ~I2C_START;
			i2cx->Instance->MCS &= ~I2C_STOP;
			i2cx->Instance->MCS |= I2C_ACK;
			i2cx->Instance->MCS &= ~I2C_HS;
		} else {
			// Receive finished. Break from the loop
			// Send stop
			i2cx->Instance->MCS |= I2C_RUN;
			i2cx->Instance->MCS &= ~I2C_START;
			i2cx->Instance->MCS |= I2C_STOP;
			i2cx->Instance->MCS &= ~I2C_ACK;
			i2cx->Instance->MCS &= ~I2C_HS;
			break;
		}
	}
	// Wait until finish
	while(i2cx->Instance->MCS & I2C_BUSY);
	// Error check
	// Check for errors
	if(i2cx->Instance->MCS & I2C_ERROR){
		i2cx->ErrorCode = I2C_ERROR;  // Error code be modified
		i2c_exception_handler(i2cx);
	} else {
		// Read the next data byte from MDR
		(*i2cx->pBuffPtr) = i2cx->Instance->MDR;
	}
}


/**
	* @brief  API used to do slave data reception 
	* @param  *I2Cx : Base address of the I2C  
  * @retval none
	*/
void i2c_slave_rx(i2c_handle_t *i2cx){
	while(i2cx->Instance->SCSR & I2C_RX_RQST){
		// Read data from data register
		(*i2cx->pBuffPtr) = i2cx->Instance->SDR;
		// Increment pointer
		i2cx->pBuffPtr ++;
		// Update counter
		i2cx->XferCount ++;
	}
	// To be implemented: NACK condition
	/*
	if(condition){
		i2c_slave_rx_ack(i2cx);
	} else {
		i2c_slave_rx_nack(i2cx);
	}
	*/
}

/**
  * @brief  This function unmasks I2C slave interrupt request.
  * @param  i2c_handle: pointer to a i2c_handle_t structure that contains
  *                the configuration information for I2C module.
  * @retval none
  */
void i2c_slave_int_enable(i2c_handle_t *i2cx){
	i2cx->Instance->SIMR = 0x7;
}

/**
  * @brief  This function handles I2C interrupt request.
  * @param  i2c_handle: pointer to a i2c_handle_t structure that contains
  *                the configuration information for I2C module.
  * @retval none
  */
void i2c_irq_handler(i2c_handle_t *i2cx){
	if(i2cx->Instance->SMIS & (I2C_INT_DATA | I2C_INT_START)){
		// Response when there is data interrupt after start
		if(i2cx->Instance->SCSR & I2C_RX_RQST){
			i2c_slave_rx(i2cx);
		} else if(i2cx->Instance->SCSR & I2C_TX_RQST) {
			i2c_slave_tx(i2cx);
		} else {
			// To be implemented
		}
	}
	
	if(i2cx->Instance->SMIS & I2C_INT_STOP){
		// Stop detected, everything is finished, clear interrupt
		i2cx->Instance->SICR |= 0x7;
	}
}
		
