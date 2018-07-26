#include "spi_driver.h"
#include "TM4C123GH6PM.h"
#include <stdbool.h>

/***********************Initializations*************************/
 /**
	* @brief  Enables the SPI device   
	* @param  *SPIx : Base address of the SPI  
	* @retval None
	*/
void spi_enable(SSI0_Type *SPIx){
	SPIx->CR1 |= ( (uint32_t) 1 << 1 ); // Enable SPI for settings
}

 /**
	* @brief  Disables the SPI device   
	* @param  *SPIx : Base address of the SPI  
	* @retval None
	*/
void spi_disable(SSI0_Type *SPIx){
	SPIx->CR1 &= ~( (uint32_t) 1 << 1 ); // Disable SPI for settings
}

/**
	* @brief  API used to do initialize the given SPI device
	* @param  *spi_handle->Init : SPI config data structure 
  * @retval none
	*/

void spi_init(spi_handle_t *spi_handle){
	spi_disable(spi_handle->Instance);  // Disable SPI for settings
	
	// Set clock phase
	if(spi_handle->Init.Clock_phase == SPI_SECOND_CLOCK_TRANS){
		spi_handle->Instance->CR0 |= ( (uint32_t) 1 << 7 );
	} else {
		spi_handle->Instance->CR0 &= ~( (uint32_t) 1 << 7 );
	}
	
	// Set clock polarity
	if(spi_handle->Init.Clock_polarity == SPI_CLOCK_POL_HIGH){
		spi_handle->Instance->CR0 |= ( (uint32_t) 1 << 6 );
	} else {
		spi_handle->Instance->CR0 &= ~( (uint32_t) 1 << 6 );
	}
	
	// Set clock SCR, frame format, and transmit data size
	if(spi_handle->Init.Mode == SPI_MASTER_SEL) {
		spi_handle->Instance->CR0 |= ( (uint32_t) (spi_handle->Init.SCR) << 8 );
	}
	spi_handle->Instance->CR0 |= ( (uint32_t) (spi_handle->Init.Frame_format) << 4 );
	spi_handle->Instance->CR0 |= ( (uint32_t) (spi_handle->Init.Data_size) << 0 );
	
	// Choose master/slave mode
	if(spi_handle->Init.Mode == SPI_SLAVE_SEL) {
		spi_handle->Instance->CR1 |= ( (uint32_t) 1 << 2 ); // Slave mode
		if(spi_handle->Init.Slave_out_enable) {
			spi_handle->Instance->CR1 |= ( (uint32_t) 1 << 3 ); // Slave output enable
		} else {
			spi_handle->Instance->CR1 &= ~( (uint32_t) 1 << 3 ); // Slave output disable
		}
	} else {
		spi_handle->Instance->CR1 &= ~( (uint32_t) 1 << 2 ); // Master mode
	}
	
	// Loopback mode setting
	if(spi_handle->Init.Loopback) {
		spi_handle->Instance->CR1 |= ( (uint32_t) 1 << 0 ); // Loopback mode
	} else {
		spi_handle->Instance->CR1 &= ~( (uint32_t) 1 << 0 ); // Normal mode
	}
	
	// Choose a clock
	// Set clock prescaler
	// SSIClk = SysClk / (CPSDVSR * (1 + SCR))
	if(spi_handle->Init.Mode == SPI_MASTER_SEL){
		if(spi_handle->Init.Clock == SPI_SYS_CLK) {
			spi_handle->Instance->CC &= SPI_SYS_CLK;
		} else {
			spi_handle->Instance->CC |= SPI_PIOSC;
		}	
		spi_handle->Instance->CPSR |= spi_handle->Init.Clock_prescale;
	}
}

/***********************Transmit/Receive*************************/

/**
	* @brief  API used to do check the spi status
	* @param  *spi_conf : SPI config data structure 
	* @param  *status_to_check : status to check
  * @retval none
*/
bool spi_status_check(spi_handle_t *spi_handle, spi_status status_to_check){
	return (spi_handle->Instance->SR & status_to_check) ? true : false;
}

/**
	* @brief  API used to do master data transmission 
	* @param  *SPIx : Base address of the SPI  
  * @param  *buffer : pointer to the tx buffer 
  * @param  len : len of tx data
  * @retval none
	*/
void spi_master_tx(spi_handle_t *spi_handle, uint8_t *tsm_buffer, uint32_t len){
	spi_handle->pTxBuffPtr = tsm_buffer;
	spi_handle->TxXferSize = len;
	spi_handle->TxXferCount = 0;
	// Enable Tx interrupt
	spi_handle->Instance->IM |= ( (uint32_t) 1 << 3);
}

/**
	* @brief  API used to do slave data transmission 
	* @param  *SPIx : Base address of the SPI  
  * @param  *buffer : pointer to the tx buffer 
  * @param  len : len of tx data
  * @retval none
	*/
void spi_slave_tx(spi_handle_t *spi_handle, uint8_t *tsm_buffer, uint32_t len){
	spi_handle->pTxBuffPtr = tsm_buffer;
	spi_handle->TxXferSize = len;
	spi_handle->TxXferCount = 0;
	// Enable Tx interrupt
	spi_handle->Instance->IM |= ( (uint32_t) 1 << 3);
}


/**
	* @brief  API used to do master data reception 
	* @param  *SPIx : Base address of the SPI  
  * @param  *buffer : pointer to the rx buffer 
  * @param  len : len of rx data
  * @retval none
	*/
void spi_master_rx(spi_handle_t *spi_handle, uint8_t *rcv_buffer, uint32_t len){
	spi_handle->pRxBuffPtr = rcv_buffer;
	spi_handle->RxXferSize = len;
	spi_handle->RxXferCount = 0;
	
	// Clear the data register
	uint16_t val1 = spi_handle->Instance->DR;
	uint16_t val2 = spi_handle->Instance->DR;
	uint16_t val3 = spi_handle->Instance->DR;
	uint16_t val4 = spi_handle->Instance->DR;
	uint16_t val5 = spi_handle->Instance->DR;
	uint16_t val6 = spi_handle->Instance->DR;
	// Enable Rx interrupt
	spi_handle->Instance->IM |= ( (uint32_t) 1 << 2);
}

/**
	* @brief  API used to do slave data reception 
	* @param  *SPIx : Base address of the SPI  
  * @param  *buffer : pointer to the rx buffer 
  * @param  len : len of rx data
  * @retval none
	*/
void spi_slave_rx(spi_handle_t *spi_handle, uint8_t *rcv_buffer, uint32_t len){
	spi_handle->pRxBuffPtr = rcv_buffer;
	spi_handle->RxXferSize = len;
	spi_handle->RxXferCount = 0;
	
	// Clear the data register
	uint16_t val = spi_handle->Instance->DR;
	// Enable Rx interrupt
	spi_handle->Instance->IM |= ( (uint32_t) 1 << 2);
}

/***********************Interrupt handlers*************************/

/**
	* @brief  API used to do set EOT 
	* @param  *SPIx : Base address of the SPI  
  * @retval none
	*/
void spi_master_eot_set(spi_handle_t *spi_handle){
	spi_handle->Instance->CR1 |= ( (uint32_t) 0x01 << 4);
}

/**
	* @brief  API used to do clear EOT 
	* @param  *SPIx : Base address of the SPI  
  * @retval none
	*/
void spi_master_eot_clear(spi_handle_t *spi_handle){
	spi_handle->Instance->CR1 &= ~( (uint32_t) 0x01 << 4);
}

/**
  * @brief  This function starts Tx process after receiving interrupt
  * @param  spi_handle: pointer to a spi_handle_t structure that contains
  *                the configuration information for SPI module.
  * @retval none
  */
static void spi_tx_handler(spi_handle_t *spi_handle){
	spi_handle->Instance->DR = (*spi_handle->pTxBuffPtr); // Send the current byte
	spi_handle->pTxBuffPtr ++; // Send next byte
	spi_handle->TxXferCount ++;
	
	if(spi_handle->TxXferCount == spi_handle->TxXferSize){
		// Transmission fnnished. Disable interrupt
		spi_handle->Instance->IM &= ~( (uint32_t) 1 << 3);
	}
}

/**
  * @brief  This function starts Rx process after receiving interrupt
  * @param  spi_handle: pointer to a spi_handle_t structure that contains
  *                the configuration information for SPI module.
  * @retval none
  */
static void spi_rx_handler(spi_handle_t *spi_handle){
	*spi_handle->pRxBuffPtr = spi_handle->Instance->DR; // Read the current byte
	spi_handle->pRxBuffPtr ++; // Read next byte
	spi_handle->RxXferCount ++;
	
	if(spi_handle->RxXferCount == spi_handle->RxXferSize){
		// Transmission fnnished. Disable interrupt
		spi_handle->RxXferException = RECEIVE_SUCCEEDED;
		spi_handle->Instance->IM &= ~( (uint32_t) 1 << 2);
	}
}

/**
  * @brief  This function handles receive time-out interrupt
  * @param  spi_handle: pointer to a spi_handle_t structure that contains
  *                the configuration information for SPI module.
  * @retval none
  */
static void spi_rt_handler(spi_handle_t *spi_handle){
	spi_handle->RxXferException = RECEIVE_TIMEOUT; // Set exception flag
	spi_handle->Instance->ICR |= 0x02; // Clear the interrupt
}

/**
  * @brief  This function handles receive overflow interrupt
  * @param  spi_handle: pointer to a spi_handle_t structure that contains
  *                the configuration information for SPI module.
  * @retval none
  */
static void spi_ror_handler(spi_handle_t *spi_handle){
	spi_handle->RxXferException = RECEIVE_OVERRUN; // Set exception flag
	spi_handle->Instance->ICR |= 0x01; // Clear the interrupt
}

/**
  * @brief  This function decodes the interrupt request and call corresponding handler.
  * @param  spi_handle: pointer to a spi_handle_t structure that contains
  *                the configuration information for SPI module.
  * @retval none
  */
void spi_irq_handler(spi_handle_t *spi_handle){
	uint32_t int_status = spi_handle->Instance->MIS;
	if(int_status & SPI_TXMIS){
		spi_tx_handler(spi_handle);
	}
	
	if(int_status & SPI_RXMIS){
		spi_rx_handler(spi_handle);
	}
	
	if(int_status & SPI_RTMIS){
		spi_rt_handler(spi_handle);
	}
	
	if(int_status & SPI_RTMIS){
		spi_ror_handler(spi_handle);
	}
}
