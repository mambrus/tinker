// LPC2129 Vic Controller
// mtrojer@arrownordic.com
// $WCREV$
//! Vectored Interrupt Controller (VIC) Driver.
//! Defines VIC_* constants.
//! Defines struct vic_control
//! \page LPC2129_VIC lpc2129_vic.h 
//! \ingroup BSPINC

#ifndef _LPC2129_VIC_H
#define _LPC2129_VIC_H

#include "aeb_types.h"

#define VIC_CNTL_EN   0x20

#define VIC_CH_WDT    0
#define VIC_CH_TIMER0 4
#define VIC_CH_TIMER1 5
#define VIC_CH_UART0  6
#define VIC_CH_UART1  7
#define VIC_CH_PWM0   8
#define VIC_CH_I2C    9
#define VIC_CH_SPI0   10
#define VIC_CH_SPI1   11
#define VIC_CH_PLL    12
#define VIC_CH_RTC    13
#define VIC_CH_EINT0  14
#define VIC_CH_EINT1  15
#define VIC_CH_EINT2  16
#define VIC_CH_EINT3  17
#define VIC_CH_AD     18
#define VIC_CH_CAN    19
#define VIC_CH_CAN1TX 20
#define VIC_CH_CAN2TX 21
#define VIC_CH_CAN1RX 26
#define VIC_CH_CAN2RX 27

typedef struct
{
  UNS_32 vecchannel;
  UNS_8 vecaddr;           // 0-15
  void *func;
} vic_control;

//! \defgroup lpc2129_vic lpc2129_vic

//! Installs an ISR at a specific Vector Address.
//! @param *vc Pointer to initialized vic_control struct.
//!
//! Example usage:
//! <pre>
//!   vic_control vc;                  \n
//!   vc.vecaddr = 0;                  \n
//!   vc.vecchannel = VIC_CH_UART0;    \n
//!   vc.func = (void*)uart0_int_isr;  \n
//!   vic_install_isr(&vc);            \n
//! </pre>
//! \ingroup lpc2129_vic
void vic_install_isr(vic_control *vc);

//! Enable the interrupt for an installed ISR.
//! @param *vc Pointer to initialized vic_control struct.
//! \ingroup lpc2129_vic
void vic_enable_int(vic_control *vc);

//! Disable the interrupt for an installed ISR.
//! @param *vc Pointer to initialized vic_control struct.
//! \ingroup lpc2129_vic
void vic_disable_int(vic_control *vc);

//! Uninstall a previously installed ISR
//! @param *vc Pointer to initialized vic_control struct.
//! \ingroup lpc2129_vic
void vic_remove_isr(vic_control *vc);

#endif
