#include "spi_driver.h"
#include "TM4C123GH6PM.h"

/**
	* @brief  API used to do initialize the given SPI device
	* @param  *spi_conf : SPI config data structure 
  * @retval none
	*/

void hal_spi_init(spi_conf_t *spi_conf);

/**
	* @brief  API used to do master data transmission 
	* @param  *SPIx : Base address of the SPI  
  * @param  *buffer : pointer to the tx buffer 
  * @param  len : len of tx data
  * @retval none
	*/
void hal_spi_master_tx(spi_handle_t *spi_handle,uint8_t *buffer, uint32_t len);

/**
	* @brief  API used to do slave data transmission 
	* @param  *SPIx : Base address of the SPI  
  * @param  *buffer : pointer to the tx buffer 
  * @param  len : len of tx data
  * @retval none
	*/
void hal_spi_slave_tx(spi_handle_t *spi_handle, uint8_t *rcv_buffer, uint32_t len);


/**
	* @brief  API used to do master data reception 
	* @param  *SPIx : Base address of the SPI  
  * @param  *buffer : pointer to the rx buffer 
  * @param  len : len of rx data
  * @retval none
	*/
void hal_spi_master_rx(spi_handle_t *spi_handle,uint8_t *buffer, uint32_t len);


/**
	* @brief  API used to do slave data reception 
	* @param  *SPIx : Base address of the SPI  
  * @param  *buffer : pointer to the rx buffer 
  * @param  len : len of rx data
  * @retval none
	*/
void hal_spi_slave_rx(spi_handle_t *spi_handle, uint8_t *rcv_buffer, uint32_t len);


/**
  * @brief  This function handles SPI interrupt request.
  * @param  spi_handle: pointer to a spi_handle_t structure that contains
  *                the configuration information for SPI module.
  * @retval none
  */
void hal_spi_irq_handler(spi_handle_t *spi_handle);
