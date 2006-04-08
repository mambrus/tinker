// UART polled
// mtrojer@arrownordic.com
// $WCREV$
//! Polled UART driver
//! \page LPC2129_UART_POLLED lpc2129_uart_polled.h 
//! \ingroup BSPINC

#ifndef _LPC2129_UART_POLLED_H
#define _LPC2129_UART_POLLED_H

#include "lpc2129_uart.h"

//! \defgroup lpc2129_uart_polled lpc2129_uart_polled

//! Initializes the UART peripheral
//! @param *uc Pointer to initialized uart_control struct.
//! Will update the status field.
//!
//! Example usage:
//! <pre>
//!   uart_control uart0;   \n
//!   uart0.port=0;         \n
//!   uart0.LCR = LCR_WLS_8BIT | LCR_SBS_1BIT | LCR_PE_0 | LCR_PS_ODD | LCR_BK_0 | LCR_DLAB_1;   \n
//!   uart0.baudrate = 115200;   \n
//! \n
//!   uart_int_init(&uart0);     \n
//! </pre>
//! \ingroup lpc2129_uart_polled
void  uart_polled_init(uart_control *uc);

//! Writes 8 bits to the UART peripheral
//! @param *uc Pointer to initialized uart_control struct.
//! You must call uart_polled_init() first.
//! @param ch Containing the 8 bits to be sent.
//! \ingroup lpc2129_uart_polled
UNS_8 uart_polled_putchar(uart_control *uc, UNS_8 ch);

//! Reads 8 bits from the UART periferal. Blocks until data is ready.
//! @param *uc Pointer to initialized uart_control struct.
//! @return Read character 
//! \ingroup lpc2129_uart_polled
UNS_8 uart_polled_getchar(uart_control *uc);

#endif
