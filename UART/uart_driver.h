#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "TM4C123GH6PM.h"

/****************************************************************************
 *                                                                          *
 *                  Macros used for UART module setip	                    	*
 *                                                                          *
 ****************************************************************************/ 
 
/* UART Receive Status/Error Clear (RSR/ECR) */
#define UART_OVERRUN_ERR										( (uint32_t) 1 << 3 )
#define UART_BREAK_ERR											( (uint32_t) 1 << 2 )
#define UART_PARITY_ERR								`			( (uint32_t) 1 << 1 )
#define UART_FRAMING_ERR										( (uint32_t) 1 << 0 )
#define UART_ERR_CLEAR											( (uint8_t) 0xFF )

/* UART Flag (FR) */
#define UART_TXFE														( (uint32_t) 1 << 7 )		// Transmit FIFO empty
#define UART_RXFF														( (uint32_t) 1 << 6 )		// Receive FIFO full
#define UART_TXFF												`		( (uint32_t) 1 << 5 )		// Transmit FIFO full
#define UART_RXFE														( (uint32_t) 1 << 4 )		// Receive FIFO empty
#define UART_BUSY														( (uint32_t) 1 << 3 )		// UART busy
#define UART_CTS														( (uint32_t) 1 << 0 )		// Clear to send

/* UART Line Control (LCRH) */
#define UART_SPS														( (uint32_t) 1 << 7 )		// Stick parity set
#define UART_FEN														( (uint32_t) 1 << 4 )		// Enable FIFO 
#define UART_STP2												`		( (uint32_t) 1 << 3 )		// Two stop bits select
#define UART_EPS														( (uint32_t) 1 << 2 )		// Even parity select
#define UART_PEN														( (uint32_t) 1 << 1 )		// Parity enable
#define UART_BRK														( (uint32_t) 1 << 0 )		// Send break

#define UART_WLEN_5BIT											( (uint32_t) 1 << 0 ) 	// 5 bit word length
#define UART_WLEN_6BIT											( (uint32_t) 1 << 1 )		// 6 bit word length
#define UART_WLEN_7BIT											( (uint32_t) 1 << 2 )		// 7 bit word length
#define UART_WLEN_8BIT											( (uint32_t) 1 << 3 )		// 8 bit word length

/* UART Control (LCRH) */
#define UART_CTSEN													( (uint32_t) 1 << 15 )		// Enable Clear to send
#define UART_RTSEN													( (uint32_t) 1 << 14 )		// Enable Request to Send 
#define UART_RTS												`		( (uint32_t) 1 << 11 )		// Request to Send
#define UART_RXE														( (uint32_t) 1 << 9 )		// Receive enable
#define UART_TXE														( (uint32_t) 1 << 8 )		// Transmit enable
#define UART_LBE														( (uint32_t) 1 << 7 )		// Loopback enable
#define UART_HSE														( (uint32_t) 1 << 5 )		// High-speed enable
#define UART_EOT														( (uint32_t) 1 << 4 )		// End of transmission
#define UART_SMART											`		( (uint32_t) 1 << 3 )		// ISO 7816 smart card support
#define UART_SIRLP													( (uint32_t) 1 << 2 )		// SIR low power mode
#define UART_SIREN													( (uint32_t) 1 << 1 )		// SIR enable
#define UART_ENABLE													( (uint32_t) 1 << 0 )		// UART enable





#endif
