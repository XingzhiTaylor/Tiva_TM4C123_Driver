#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "TM4C123GH6PM.h"
#include <stdbool.h>

/****************************************************************************
 *                                                                          *
 *                  Macros used for UART module setup	                    	*
 *                                                                          *
 ****************************************************************************/ 
 
/* UART Receive Status/Error Clear (RSR/ECR) */
#define UART_OVERRUN_ERR										( (uint32_t) 1 << 3 )
#define UART_BREAK_ERR											( (uint32_t) 1 << 2 )
#define UART_PARITY_ERR											( (uint32_t) 1 << 1 )
#define UART_FRAMING_ERR										( (uint32_t) 1 << 0 )
#define UART_ERR_CLEAR											( (uint8_t) 0xFF )

/* UART Flag (FR) */
#define UART_TXFE														( (uint32_t) 1 << 7 )		// Transmit FIFO empty
#define UART_RXFF														( (uint32_t) 1 << 6 )		// Receive FIFO full
#define UART_TXFF														( (uint32_t) 1 << 5 )		// Transmit FIFO full
#define UART_RXFE														( (uint32_t) 1 << 4 )		// Receive FIFO empty
#define UART_BUSY														( (uint32_t) 1 << 3 )		// UART busy
#define UART_CTS														( (uint32_t) 1 << 0 )		// Clear to send

/* UART Line Control (LCRH) */
#define UART_SPS														( (uint32_t) 1 << 7 )		// Stick parity set
#define UART_FEN														( (uint32_t) 1 << 4 )		// Enable FIFO 
#define UART_STP2														( (uint32_t) 1 << 3 )		// Two stop bits select
#define UART_EPS														( (uint32_t) 1 << 2 )		// Even parity select
#define UART_PEN														( (uint32_t) 1 << 1 )		// Parity enable
#define UART_BRK														( (uint32_t) 1 << 0 )		// Send break

#define UART_WLEN_5BIT											( (uint32_t) 0x0 ) 	// 5 bit word length
#define UART_WLEN_6BIT											( (uint32_t) 0x1 )		// 6 bit word length
#define UART_WLEN_7BIT											( (uint32_t) 0x2 )		// 7 bit word length
#define UART_WLEN_8BIT											( (uint32_t) 0x3 )		// 8 bit word length

/* UART Control (CTL) */
#define UART_CTSEN													( (uint32_t) 1 << 15 )		// Enable Clear to send
#define UART_RTSEN													( (uint32_t) 1 << 14 )		// Enable Request to Send 
#define UART_RTS														( (uint32_t) 1 << 11 )		// Request to Send
#define UART_RXE														( (uint32_t) 1 << 9 )		// Receive enable
#define UART_TXE														( (uint32_t) 1 << 8 )		// Transmit enable
#define UART_LBE														( (uint32_t) 1 << 7 )		// Loopback enable
#define UART_HSE														( (uint32_t) 1 << 5 )		// High-speed enable
#define UART_EOT														( (uint32_t) 1 << 4 )		// End of transmission
#define UART_SMART													( (uint32_t) 1 << 3 )		// ISO 7816 smart card support
#define UART_SIRLP													( (uint32_t) 1 << 2 )		// SIR low power mode
#define UART_SIREN													( (uint32_t) 1 << 1 )		// SIR enable
#define UART_ENABLE													( (uint32_t) 1 << 0 )		// UART enable

/* UART FIFO Level Select (IFLS) */
#define UART_RX_7_8_FULL										( (uint32_t) 0x4 )		// Rx FIFO 7/8 full
#define UART_RX_3_4_FULL										( (uint32_t) 0x3 )		// Rx FIFO 3/4 full
#define UART_RX_1_2_FULL										( (uint32_t) 0x2 )		// Rx FIFO 1/2 full
#define UART_RX_1_4_FULL										( (uint32_t) 0x1 )		// Rx FIFO 1/4 full
#define UART_RX_1_8_FULL										( (uint32_t) 0x0 )		// Rx FIFO 1/8 full

#define UART_TX_7_8_EMPTY										( (uint32_t) 0x0 )		// Rx FIFO 7/8 full
#define UART_TX_3_4_EMPTY										( (uint32_t) 0x1 )		// Rx FIFO 3/4 full
#define UART_TX_1_2_EMPTY										( (uint32_t) 0x2 )		// Rx FIFO 1/2 full
#define UART_TX_1_4_EMPTY										( (uint32_t) 0x3 )		// Rx FIFO 1/4 full
#define UART_TX_1_8_EMPTY										( (uint32_t) 0x4 )		// Rx FIFO 1/8 full

/* UART Interrupts */
#define UART_9BIT                           ( (uint32_t) 1 << 12 ) // 9-bit mode
#define UART_OE_INT                         ( (uint32_t) 1 << 10 ) // Overrun error interrupt
#define UART_BE_INT                         ( (uint32_t) 1 << 9 )  // Break error interrupt
#define UART_PE_INT                         ( (uint32_t) 1 << 8 )  // Parity error interrupt
#define UART_FE_INT                         ( (uint32_t) 1 << 7 )  // Frame error interrupt
#define UART_RT_INT                         ( (uint32_t) 1 << 6 )  // Receive time-out error interrupt
#define UART_TX_INT                         ( (uint32_t) 1 << 5 )  // Transmit interrupt
#define UART_RX_INT                         ( (uint32_t) 1 << 4 )  // Receive interrupt
#define UART_CTS_INT                        ( (uint32_t) 1 << 1 )  // Clear to send interrupt

#define UART_9BITEN                         ( (uint32_t) 1 << 15 ) // 9-bit address enable

/* UART Peripheral Properties */
#define UART_9BIT_SUP                       ( (uint32_t) 1 << 1 ) // 9-bit support
#define UART_SMART_SUP                      ( (uint32_t) 1 << 0 ) // Smart card support

/* UART Clock Configuration */
#define UART_SYS_CLK                        ( (uint32_t) 0x0 ) // System clock
#define UART_PIOSC                          ( (uint32_t) 0x5 ) // PIOSC

/***************************Clock Init Macros*****************************/
#define RCC_UART_CLK_ENABLE(uint32_t, n)      volatile unsigned long delay0;\
                                              SYSCTL->RCGCUART |= (0x01 << n);\
                                              delay0 = SYSCTL->RCGCUART;

/****************************************************************************
 *                                                                          *
 *                          Driver Exposed APIs     	                    	*
 *                                                                          *
 ****************************************************************************/ 

/*
 * @brief: Write a byte of data to the data register
 * @param: *UARTx: Base address of the UART module
 * @param: data: The byte of data to send
 * @return: None
 */
void UARTWriteByte(UART0_Type *UARTx, uint8_t data);

/*
 * @brief: Read a byte of data from the data register
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
uint8_t UARTReadByte(UART0_Type *UARTx);

/*
 * @brief: Check for receive error
 * @param: *UARTx: Base address of the UART module
 * @param: error: The receive error to check
 * @return: Whether the specified error has occured
 */
bool UARTReceiveErrorCheck(UART0_Type *UARTx, uint32_t error);

/*
 * @brief: Check if the receive error has been cleared
 * @param: *UARTx: Base address of the UART module
 * @param: error: The receive error to check
 * @return: Whether the specified error has been cleared
 */
bool UARTReceiveErrorCleared(UART0_Type *UARTx, uint32_t error);

/*
 * @brief: Clear all receive errors
 * @param: *UARTx: Base address of the UART module
 * @param: error: The receive error to clear
 * @return: None
 */
void UARTClearReceiveError(UART0_Type *UARTx, uint32_t error);

/*
 * @brief: Check the UART flags
 * @param: *UARTx: Base address of the UART module
 * @param: flag: The flag to check
 * @return: Whether the specified flag is set
 */
bool UARTFlagCheck(UART0_Type *UARTx, uint32_t flag);

/*
 * @brief: Set the IrDA low power dividor
 * @param: *UARTx: Base address of the UART module
 * @param: divisor: The divisor to set
 * @return: None
 */
void UARTIrDALPDivSet(UART0_Type *UARTx, uint8_t divisor);

/*
 * @brief: Get the IrDA low power dividor
 * @param: *UARTx: Base address of the UART module
 * @return: The IrDA low power dividor
 */
uint8_t UARTIrDALPDivGet(UART0_Type *UARTx);

/*
 * @brief: Set the integer baud rate dividor
 * @param: *UARTx: Base address of the UART module
 * @param: divisor: The divisor to set
 * @return: None
 */
void UARTBaudIntDivSet(UART0_Type *UARTx, uint16_t divisor);

/*
 * @brief: Get the integer baud rate dividor
 * @param: *UARTx: Base address of the UART module
 * @return: The integer baud rate dividor
 */
uint16_t UARTBaudIntDivGet(UART0_Type *UARTx);

/*
 * @brief: Set the fractional baud rate dividor
 * @param: *UARTx: Base address of the UART module
 * @param: divisor: The divisor to set
 * @return: None
 */
void UARTBaudFracDivSet(UART0_Type *UARTx, uint8_t divisor);

/*
 * @brief: Get the fractional baud rate dividor
 * @param: *UARTx: Base address of the UART module
 * @return: The fractional baud rate dividor
 */
uint8_t UARTBaudFracDivGet(UART0_Type *UARTx);

/*
 * @brief: Set the line control parameters
 * @param: *UARTx: Base address of the UART module
 * @param: lineControl: The line control param to set
 * @return: None
 */
void UARTLineControlSet(UART0_Type *UARTx, uint32_t lineControl);

/*
 * @brief: Set the word width
 * @param: *UARTx: Base address of the UART module
 * @param: width: The word width to set
 * @return: None
 */
void UARTWordWidthSet(UART0_Type *UARTx, uint32_t width);

/*
 * @brief: Get the word width
 * @param: *UARTx: Base address of the UART module
 * @return: Word width
 */
uint32_t UARTWordWidthGet(UART0_Type *UARTx);

/*
 * @brief: Enable clear to send
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTClearToSendEnable(UART0_Type *UARTx);

/*
 * @brief: Disable clear to send
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTClearToSendDisable(UART0_Type *UARTx);

/*
 * @brief: Enable request to send
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTReqToSendEnable(UART0_Type *UARTx);

/*
 * @brief: Disable request to send
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTReqToSendDisable(UART0_Type *UARTx);


/*
 * @brief: Enable request to send
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTReqToSendSet(UART0_Type *UARTx);

/*
 * @brief: Disable request to send
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTReqToSendClear(UART0_Type *UARTx);

/*
 * @brief: Enable receive
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTReceiveEnable(UART0_Type *UARTx);

/*
 * @brief: Disable receive
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTReceiveDisable(UART0_Type *UARTx);

/*
 * @brief: Enable transmit
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTTransmitEnable(UART0_Type *UARTx);

/*
 * @brief: Disable transmit
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTTransmitDisable(UART0_Type *UARTx);


/*
 * @brief: Enable loop back
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTLoopBackEnable(UART0_Type *UARTx);

/*
 * @brief: Disable loop back
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTLoopBackDisable(UART0_Type *UARTx);

/*
 * @brief: Enable high speed
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTHighSpeedEnable(UART0_Type *UARTx);

/*
 * @brief: Disable high speed
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTHighSpeedDisable(UART0_Type *UARTx);

/*
 * @brief: Enable EOT
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTEndOfTxEnable(UART0_Type *UARTx);

/*
 * @brief: Disable EOT
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTEndOfTxDisable(UART0_Type *UARTx);


/*
 * @brief: Enable smart card support
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTSmartEnable(UART0_Type *UARTx);

/*
 * @brief: Disable smart card support
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTSmartDisable(UART0_Type *UARTx);


/*
 * @brief: Enable SIR low power
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTSIRLowPwrEnable(UART0_Type *UARTx);

/*
 * @brief: Disable SIR low power
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTSIRLowPwrEDisable(UART0_Type *UARTx);

/*
 * @brief: Enable SIR
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTSIREnable(UART0_Type *UARTx);

/*
 * @brief: Disable SIR
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTSIRDisable(UART0_Type *UARTx);

/*
 * @brief: Enable UART
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTEnable(UART0_Type *UARTx);

/*
 * @brief: Disable UART
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
void UARTDisable(UART0_Type *UARTx);

/*
 * @brief: Set the Tx FIFO interrupt level
 * @param: *UARTx: Base address of the UART module
 * @param: IFL: The Tx FIFO interrupt level to set
 * @return: None
 */
void UARTTxIntFIFOLvlSet(UART0_Type *UARTx, uint32_t IFL);

/*
 * @brief: Get the Tx FIFO interrupt level
 * @param: *UARTx: Base address of the UART module
 * @return: Tx FIFO interrupt level
 */
uint32_t UARTTxIntFIFOLvlGet(UART0_Type *UARTx);

/*
 * @brief: Set the Rx FIFO interrupt level
 * @param: *UARTx: Base address of the UART module
 * @param: IFL: The Tx FIFO interrupt level to set
 * @return: None
 */
void UARTRxIntFIFOLvlSet(UART0_Type *UARTx, uint32_t IFL);

/*
 * @brief: Get the Rx FIFO interrupt level
 * @param: *UARTx: Base address of the UART module
 * @return: Tx FIFO interrupt level
 */
uint32_t UARTRxIntFIFOLvlGet(UART0_Type *UARTx);

/*
 * @brief: Enable UART interrupts
 * @param: *UARTx: Base address of the UART module
 * @param: interrupt: Interrupt to enable
 * @return: None
 */
void UARTInterruptEnable(UART0_Type *UARTx, uint32_t interrupt);

/*
 * @brief: Disable UART interrupts
 * @param: *UARTx: Base address of the UART module
 * @param: interrupt: Interrupt to disable
 * @return: None
 */
void UARTInterruptDisable(UART0_Type *UARTx, uint32_t interrupt);

/*
 * @brief: Get UART raw interrupt status
 * @param: *UARTx: Base address of the UART module
 * @param: interrupt: Raw Interrupt status to check
 * @return: Whether the interrupt is set
 */
bool UARTRawInterruptGet(UART0_Type *UARTx, uint32_t interrupt);

/*
 * @brief: Get UART masked interrupt status
 * @param: *UARTx: Base address of the UART module
 * @param: interrupt: Masked Interrupt status to check
 * @return: Whether the nterrupt is set
 */
bool UARTMaskedInterruptGet(UART0_Type *UARTx, uint32_t interrupt);

/*
 * @brief: Clear UART interrupts
 * @param: *UARTx: Base address of the UART module
 * @param: interrupt: Interrupt to clear
 * @return: None
 */
void UARTInterruptClear(UART0_Type *UARTx, uint32_t interrupt);

/*
 * @brief: Set DMA control
 * @param: *UARTx: Base address of the UART module
 * @param: param: DMA params to set
 * @return: None
 */
void UARTDMASet(UART0_Type *UARTx, uint32_t param);

/*
 * @brief: Set 9-bit address
 * @param: *UARTx: Base address of the UART module
 * @param: addr: address to set
 * @param: enable: whether to enable 9-bit addr
 * @return: None
 */
void UART9BitAdrSet(UART0_Type *UARTx, uint8_t addr, bool enable);

/*
 * @brief: Get 9-bit address
 * @param: *UARTx: Base address of the UART module
 * @return: The 9-bit address
 */
uint8_t UART9BitAdrGet(UART0_Type *UARTx);

/*
 * @brief: Mask 9-bit address
 * @param: *UARTx: Base address of the UART module
 * @param: mask: address mask
 * @return: None
 */
void UART9BitAdrMask(UART0_Type *UARTx, uint8_t mask);

/*
 * @brief: Set peripheral properties
 * @param: *UARTx: Base address of the UART module
 * @param: prop: properties to set
 * @return: None
 */
void UARTPeriPropSet(UART0_Type *UARTx, uint32_t prop);

/*
 * @brief: Config clock
 * @param: *UARTx: Base address of the UART module
 * @param: clock: clock to use
 * @return: None
 */
void UARTClkConfig(UART0_Type *UARTx, uint8_t clock);

/*
 * @brief: Initialize UART
 * @param: *UARTx: Base address of the UART module
 * @return: None
 */
//void UARTInit(UART0_Type *UARTx);

/*
 * @brief: Initialize UART
 * @param: *UARTx: Base address of the UART module
 * @param: tsm_buffer: pointer to data buffer
 * @return: None
 */
//void UARTTransmit(UART0_Type *UARTx, uint8_t *tsm_buffer);

/*
 * @brief: Initialize UART
 * @param: *UARTx: Base address of the UART module
 * @param: rcv_buffer: receive buffer
 * @return: None
 */
//void UARTReceive(UART0_Type *UARTx, uint8_t *rcv_buffer);

#endif
