#include "spi_driver.h"
#include "TM4C123GH6PM.h"

 /**
	* @brief  Enables the SPI device   
	* @param  *SPIx : Base address of the SPI  
	* @retval None
	*/
void hal_spi_enable(SSI0_Type *SPIx){
	if( !(SPIx->CR1 & ( (uint32_t) 1 << 2 )) )
		SPIx->CR1 |= ( (uint32_t) 1 << 2 ); // Enable SPI for settings
}

 /**
	* @brief  Disables the SPI device   
	* @param  *SPIx : Base address of the SPI  
	* @retval None
	*/
void hal_spi_disable(SSI0_Type *SPIx){
	if( (SPIx->CR1 & ( (uint32_t) 1 << 2 )) )
		SPIx->CR1 &= ~( (uint32_t) 1 << 2 ); // Disable SPI for settings
}

/**
	* @brief  API used to do initialize the given SPI device
	* @param  *spi_handle->Init : SPI config data structure 
  * @retval none
	*/

void spi_init(spi_handle_t *spi_handle){
	spi_handle->Instance->CR1 &= ~( (uint32_t) 1 << 2 ); // Disable SPI for settings
	
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
	spi_handle->Instance->CR0 |= ( (uint32_t) (spi_handle->Init.SCR) << 8 );
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
	if(spi_handle->Init.Clock == SPI_SYS_CLK) {
		spi_handle->Instance->CC &= SPI_SYS_CLK;
	} else {
		spi_handle->Instance->CC |= SPI_PIOSC;
	}
	
	// Clock prescaler
	// SSIClk = SysClk / (CPSDVSR * (1 + SCR))
	spi_handle->Instance->CPSR |= spi_handle->Init.Clock_prescale;
}



/**
	* @brief  API used to do master data transmission 
	* @param  *SPIx : Base address of the SPI  
  * @param  *buffer : pointer to the tx buffer 
  * @param  len : len of tx data
  * @retval none
	*/
void spi_master_tx(spi_handle_t *spi_handle,uint8_t *buffer, uint32_t len);

/**
	* @brief  API used to do slave data transmission 
	* @param  *SPIx : Base address of the SPI  
  * @param  *buffer : pointer to the tx buffer 
  * @param  len : len of tx data
  * @retval none
	*/
void spi_slave_tx(spi_handle_t *spi_handle, uint8_t *rcv_buffer, uint32_t len);


/**
	* @brief  API used to do master data reception 
	* @param  *SPIx : Base address of the SPI  
  * @param  *buffer : pointer to the rx buffer 
  * @param  len : len of rx data
  * @retval none
	*/
void spi_master_rx(spi_handle_t *spi_handle,uint8_t *buffer, uint32_t len);


/**
	* @brief  API used to do slave data reception 
	* @param  *SPIx : Base address of the SPI  
  * @param  *buffer : pointer to the rx buffer 
  * @param  len : len of rx data
  * @retval none
	*/
void spi_slave_rx(spi_handle_t *spi_handle, uint8_t *rcv_buffer, uint32_t len);


/**
  * @brief  This function handles SPI interrupt request.
  * @param  spi_handle: pointer to a spi_handle_t structure that contains
  *                the configuration information for SPI module.
  * @retval none
  */
void hal_spi_irq_handler(spi_handle_t *spi_handle);
