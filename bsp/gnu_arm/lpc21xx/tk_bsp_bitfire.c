/***************************************************************************
 *   Copyright (C) 2006 by Michael Ambrus                                  *
 *   michael.ambrus@maquet.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
 /*!
 @brief Syscalls implemented for the Bitfire board
  
 Bitfire is a starterkit from Arrow electronics. You should be able to
 use this as a template for any LPC2129 based HW.
 
 */

#include "../tk_bsp.h"
#include "../tk_systimer.h"

#include "lpc2129_uart.h"
#include "lpc2129_uart_polled.h"
#include "lpc2129_vic.h"
#include "lpc21xx.h"

#include <string.h>
#include <assert.h>


static uart_control uc0;
static uart_control uc1;

#define DEV_STDIN  0
#define DEV_STDOUT 1
#define DEV_STDERR 2

#define MODE_READ    0
#define MODE_WRITE   4
#define MODE_APPEND  8

#define NAME_TELETYPE ":tt"

int bsp_Open            (char *name, int mode, int len){
   
   if ( strncmp(name,NAME_TELETYPE,len) == 0 ){  //Lib wants handles for std- in/out/err
      if (mode == MODE_READ)
         return DEV_STDIN;
      if (mode == MODE_WRITE)
         return DEV_STDOUT;
      if (mode == MODE_APPEND)
         return DEV_STDERR;
   }
   //If you reach here, bad FF is requested. Try output something usefull.
   assert("Open error: Bad filename requested." == 0);
}

int bsp_Close           (/*TBD*/){}
int bsp_WriteC          (/*TBD*/){}
int bsp_Write0          (/*TBD*/){}

int bsp_Write           (int fh, char *ptr,int len){
   int i;
   
   switch (fh){
   case DEV_STDOUT :
      for (i=0; i<len; i++)
         uart_polled_putchar(&uc0,ptr[i]);
      break;
   case DEV_STDERR :
      for (i=0; i<len; i++)
         uart_polled_putchar(&uc1,ptr[i]);
      break;
   default:
      assert("Write error: Bad filehandle" == 0);
   }

   //Return #chars not written (i.e. 0)   
   return 0;
} 

int bsp_Read            (/*TBD*/){}
int bsp_ReadC           (/*TBD*/){}
int bsp_IsTTY           (/*TBD*/){}
int bsp_Seek            (/*TBD*/){}
int bsp_FLen            (/*TBD*/){}
int bsp_TmpNam          (/*TBD*/){}
int bsp_Remove          (/*TBD*/){}
int bsp_Rename          (/*TBD*/){}
int bsp_Clock           (/*TBD*/){}
int bsp_Time            (/*TBD*/){}
int bsp_System          (/*TBD*/){}
int bsp_Errno           (/*TBD*/){}
int bsp_GetCmdLine      (/*TBD*/){}
int bsp_HeapInfo        (/*TBD*/){}
int bsp_EnterSVC        (/*TBD*/){}
int bsp_ReportException (/*TBD*/){}

/*!

 @note
 ISR bindings are used at this lvls (important that other part knows of each 
 other's ISR's)
 
 */
int tk_bsp_sysinit        (void){   
   
   {  // Set up the terminal IO      
      extern void    initialise_monitor_handles _PARAMS ((void));
        
      //Hook up the lib (nobody else will)
      initialise_monitor_handles();
        
      //Initialize the UART:s for std- in/out/err
      uc0.port = 0;
      uc0.LCR = LCR_WLS_8BIT | LCR_SBS_1BIT | LCR_PE_0 | LCR_PS_ODD | LCR_BK_0 | LCR_DLAB_1;
      uc0.baudrate = 115200;
      uart_polled_init(&uc0);
      
      uc1.port = 1;
      uc1.LCR = LCR_WLS_8BIT | LCR_SBS_1BIT | LCR_PE_0 | LCR_PS_ODD | LCR_BK_0 | LCR_DLAB_1;
      uc1.baudrate = 115200;
      uart_polled_init(&uc1);
   }
   
   {  // Set up system timer
      vic_control vc;
      
      //Showing our ISR intentions
      vc.func       = systimer_Handler;
      vc.vecaddr    = VICVectAddr7;              
      vc.vecchannel = VIC_CH_TIMER0;
      
      systimer_init(&vc);
      
      
   }
   
}



/*!
 * @defgroup CVSLOG_tk_bsp_bitfire.c tk_bsp_bitfire.c
 * @ingroup CVSLOG
 *  $Log: tk_bsp_bitfire.c,v $
 *  Revision 1.2  2006-04-08 10:15:56  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.1.2.2  2006/04/08 09:59:02  ambrmi09
 *  In preparation to join with main trunk.
 *
 *  Revision 1.1.2.1  2006/04/07 12:10:07  ambrmi09
 *  Skeleton for handling syscalls using the ARM Angel interface in place
 *
 *  Basic terminal I/O for gnu_arm (LPC2129) - only output so far (input requires
 *  blocking).
 *
 */