// LPC2129 UART
// mtrojer@arrownordic.com
// $WCREV$
// Common include file for UART drivers.
//! Common include file for UART drivers.
//! Defines LCR_*, UART_* constants.
//! Defines struct uart_control
//! \page LPC2129_UART lpc2129_uart.h 
//! \ingroup BSPINC

#ifndef _LPC2129_UART_H
#define _LPC2129_UART_H

#include "aeb_types.h"

#define CR              0x0D

#define LCR_WLS_5BIT    (0x0 << 0)
#define LCR_WLS_6BIT    (0x1 << 0)
#define LCR_WLS_7BIT    (0x2 << 0)
#define LCR_WLS_8BIT    (0x3 << 0)
#define LCR_SBS_1BIT    (0x0 << 2)
#define LCR_SBS_2BIT    (0x1 << 2)
#define LCR_PE_0        (0x0 << 3)
#define LCR_PE_1        (0x1 << 3)
#define LCR_PS_ODD      (0x0 << 4)
#define LCR_PS_EVEN     (0x1 << 4)
#define LCR_PS_F1       (0x2 << 4)
#define LCR_PS_F0       (0x3 << 4)
#define LCR_BK_0        (0x0 << 6)    
#define LCR_BK_1        (0x1 << 6)
#define LCR_DLAB_0      (0x0 << 7)   
#define LCR_DLAB_1      (0x1 << 7)

#define UART_STATUS_NOT_STARTED 0x00
#define UART_STATUS_OK          0xFF

#define UART_ERR_NO_ERROR       0xFF
#define UART_ERR_NOT_INIT       0x01
#define UART_ERR_NO_CHAR_READY  0x02

typedef struct
{
  UNS_8 port;
  UNS_8 status;
  UNS_8 LCR;      // Line Control Register
  UNS_32 baudrate;
  UNS_8 vecaddr;  // which IRQ vecaddr to use (must be unique!)
} uart_control;


#endif

