// UART polled
// mtrojer@arrownordic.com
// $WCREV$

#include "aeb_types.h"
#include "aeb01.h"
#include "lpc21xx.h"
#include "lpc2129_uart.h"
#include "lpc2129_uart_polled.h"

void uart_polled_init(uart_control *uc)
{
  UNS_16 bauddivisor = VPB_CLOCK / (16*uc->baudrate);

  if (uc->port==0) {
    PINSEL0 = PINSEL0 | 0x00000005;
    UART0_LCR = uc->LCR;
    UART0_DLL = bauddivisor & 0xFF;
    UART0_DLM = bauddivisor >> 8;
    UART0_LCR = uc->LCR & (~LCR_DLAB_1);
  }

  if (uc->port==1) {
    PINSEL0 = PINSEL0 | 0x00050000;
    UART1_LCR = uc->LCR;
    UART1_DLL = bauddivisor & 0xFF;
    UART1_DLM = bauddivisor >> 8;
    UART1_LCR = uc->LCR & (~LCR_DLAB_1);
  }

  uc->status = UART_STATUS_OK;
}

UNS_8 uart_polled_putchar(uart_control *uc, UNS_8 ch)
{
  if (uc->status!=UART_STATUS_OK)
    return 0;
  
  if (uc->port==0) {
    if (ch == '\n')  {
      while (!(UART0_LSR & 0x20));
      UART0_THR = CR;                          /* output CR */
    }
    while (!(UART0_LSR & 0x20));
    return (UART0_THR = ch);
  }

  if (uc->port==1) {
    if (ch == '\n')  {
      while (!(UART1_LSR & 0x20));
      UART1_THR = CR;                          /* output CR */
    }
    while (!(UART1_LSR & 0x20));
    return (UART1_THR = ch);
  }
}

UNS_8 uart_polled_getchar(uart_control *uc)
{
  if (uc->status!=UART_STATUS_OK)
    return 0;

  if (uc->port==0) {
    while (!(UART0_LSR & 0x01));
    return (UART0_RBR);
  }

  if (uc->port==1) {
    while (!(UART1_LSR & 0x01));
    return (UART1_RBR);
  }
}
