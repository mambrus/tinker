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

#include <tinker/config.h>

#include "../../tk_bsp.h"
#include "../../tk_systimer.h"
#include <../src/tk_tick.h>

#include "lpc2129_uart.h"
#include "lpc2129_uart_polled.h"
#include "lpc2129_vic.h"
#include "lpc21xx.h"


#include <string.h>
#include <assert.h>
#include <time.h>

#if (TK_SYSTEM==__SYS_HIXS__) || (TK_SYSTEM==__ANGEL_SWI__)
   #include <sys/stat.h>
   #include <sys/types.h>
   #include <sys/times.h>
   #include <sys/time.h>
#endif

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

/* 
 This function needs working. Among others we need to read mickeys/mackes 
 several times to detect is IRQ has happened betweeb while reading the first
 and second part (potential wrap-around).
*/

int bsp_Clock           (){
	unsigned int TuS_low  = sys_mickey;           //Low  32 bit uS worth value
	long long    TuS_high = sys_mackey;           //High 32 bit uS worth value
	unsigned int cratio = MICKEYS_PER_SEC/CLOCKS_PER_SEC;
	
	long long Tcl = ((TuS_high << 32) + TuS_low ) / cratio;
	
	return Tcl; //Trunc it on purpose (best we can do anyway)	
}

int bsp_Time            (/*TBD*/){}
int bsp_System          (/*TBD*/){}
int bsp_Errno           (/*TBD*/){}
int bsp_GetCmdLine      (/*TBD*/){}
int bsp_HeapInfo        (/*TBD*/){}
int bsp_EnterSVC        (/*TBD*/){}
int bsp_ReportException (/*TBD*/){}


clock_t bsp_Times(struct tms *buf){
   (*buf).tms_utime  = (clock_t)bsp_Clock();
   buf->tms_stime  = 0;
   buf->tms_cutime = 0;
   buf->tms_cstime = 0;      
}

void (*bsp_syscall)(void);
void bsp_Syscall_mon(void *hix_syscall){
	bsp_syscall = hix_syscall;
};         

/*!

 @note
 ISR bindings are used at this lvls (important that other part knows of each 
 other's ISR's)
 
 */
int tk_bsp_sysinit        (void){   
   
   vic_global_enable_int();
   
   {  // Set up the terminal IO      
      
        
      #if !defined(__SYS_ANGEL_SWI__)
      #error "Sanity check"
      #endif
      
      //Hook up the lib (nobody else will)
      #if (TK_SYSTEM==__SYS_ANGEL_SWI__)
         extern void    initialise_monitor_handles _PARAMS ((void));
         initialise_monitor_handles(); 
      #elif (TK_SYSTEM==__SYS_HIXS__)
         extern int     (*hixs_close)        (int file);
         extern void    (*hixs_exit)         (int status);
         extern int     (*hixs_execve)       (char *name, char **argv, char **env);
         extern int     (*hixs_fork)         ();
         extern int     (*hixs_fstat)        (int file, struct stat *st);
         extern int     (*hixs_getpid)       ();
         extern int     (*hixs_gettimeofday) (struct timeval *tp, struct timezone *tzp);
         extern int     (*hixs_isatty)       (int file);
         extern int     (*hixs_kill)         (int pid, int sig);
         extern int     (*hixs_link)         (char *old, char *new);
         extern int     (*hixs_lseek)        (int file, int ptr, int dir);
         extern int     (*hixs_read)         (int file, char *ptr, int len);
         extern caddr_t (*hixs_sbrk)         (int incr);
         extern int     (*hixs_settimeofday) (const struct timeval *tp, const struct timezone *tzp);
         extern int     (*hixs_stat)         (char *file, struct stat *st);
         extern clock_t (*hixs_times)        (struct tms *buf);
         extern int     (*hixs_unlink)       (char *name);
         extern int     (*hixs_wait)         (int *status);
         extern int     (*hixs_write)        (int file, char *ptr, int len);
         extern void    (*hixs_syscall_mon)  (void *);         
         
         hixs_close        = hixs_close;        
         hixs_exit         = hixs_exit;
         hixs_execve       = hixs_execve;
         hixs_fork         = hixs_fork;
         hixs_fstat        = hixs_fstat;
         hixs_getpid       = hixs_getpid;
         hixs_gettimeofday = hixs_gettimeofday; 
         hixs_isatty       = hixs_isatty; 
         hixs_kill         = hixs_kill;
         hixs_link         = hixs_link;
         hixs_lseek        = hixs_lseek;
         hixs_read         = hixs_read;
         hixs_sbrk         = hixs_sbrk;
         hixs_settimeofday = hixs_settimeofday;
         hixs_stat         = hixs_stat;
         hixs_times        = bsp_Times;            // <-
         hixs_unlink       = hixs_unlink;
         hixs_wait         = hixs_wait;
         hixs_write        = bsp_Write;            // <--
         hixs_syscall_mon  = bsp_Syscall_mon;      // <--
         
      #else
         #error "System either not supported or provided"
      #endif
        
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
 *  Revision 1.6  2006-11-05 14:18:58  ambrmi09
 *  Build system and source modified to make better use of config.h
 *
 *  This file now contains information about how the kernel is configured
 *  and can be used by both application and kernel build (old solution only
 *  let kernel-buils know of these details).
 *
 *  This applies to both tk_tuning, component configuration among others.
 *  Use './configure --help' to see a full list. Note that  if a certain
 *  feature is not configured, the old tk_tuning will fill in the gaps.
 *  This is especially usefull when not using GNU build- and configure-
 *  tool-chain. Hopefully, we'll be able to get rid of tk_tuning.h in the
 *  future.
 *
 *  Revision 1.5  2006/10/02 18:38:01  ambrmi09
 *  Improved gdb-wrapper.ex to permit step into withot warning. This is
 *  however a workaround for something that must be a bug in the cross GDB
 *  used (target is halted at breakpoint even though GDB sends a warning).
 *
 *  This proves that the method is "workable", but it's a tedious work (is
 *  it worth it?). Same method coud be used for the other spep-exec comands.
 *
 *  Revision 1.4  2006/09/28 17:42:44  ambrmi09
 *  HIXS system integration for RM done. ARM now has two different system call API supported. This is mostly interesting from a transparency point of view, but also a good exersisze for bfin and ppc, which are not ported yet (since I'm not planning on implementing any other system integrations than HIXS from now on).
 *
 *  Revision 1.3  2006/09/13 18:29:31  ambrmi09
 *  Commited needed in repocitory
 *
 *  Revision 1.2  2006/04/08 10:15:56  ambrmi09
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

