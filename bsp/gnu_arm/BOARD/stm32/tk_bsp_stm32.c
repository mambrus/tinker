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
 

#include <tinker/config.h>

#include "../../tk_bsp.h"
#include "../../tk_systimer.h"
#include <../src/tk_tick.h>
#include <tinker/hixs.h>

/*
#include "lpc2129_uart.h"
#include "lpc2129_uart_polled.h"
#include "lpc2129_vic.h"
#include "lpc21xx.h"
*/

#include <string.h>
#include <assert.h>
#include <time.h>

#if (TK_SYSTEM==__SYS_HIXS__) || (TK_SYSTEM==__ANGEL_SWI__)
   #include <sys/stat.h>
   #include <sys/types.h>
   #include <sys/times.h>
   #include <sys/time.h>
#endif

#define DEV_STDIN  0
#define DEV_STDOUT 1
#define DEV_STDERR 2

#define MODE_READ    0
#define MODE_WRITE   4
#define MODE_APPEND  8

#define NAME_TELETYPE ":tt"

#if !defined(BOARD)
   #error BOARD needs to be defined
#endif

#if !defined(TK_SYSTEM)
   #error System needs to be defined
#endif

#if !defined(__SYS_DEFAULT__)
#error "Sanity chec failk"
#endif

#if (TK_SYSTEM == __SYS_HIXS__)
	extern struct hixs_t  hixs;
#else
	#error "System either not supported or provided"
#endif

int bsp_Open            (char *name, int mode, int len){
  /* 
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
   */
}

int bsp_Close           (/*TBD*/){}
int bsp_WriteC          (/*TBD*/){}
int bsp_Write0          (/*TBD*/){}

int bsp_Write           (int fh, char *ptr,int len){
   int i;
   /*
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
*/
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
	/*
	unsigned int TuS_low  = sys_mickey;           //Low  32 bit uS worth value
	long long    TuS_high = sys_mackey;           //High 32 bit uS worth value
	unsigned int cratio = MICKEYS_PER_SEC/CLOCKS_PER_SEC;
	
	long long Tcl = ((TuS_high << 32) + TuS_low ) / cratio;
	
	return Tcl; //Trunc it on purpose (best we can do anyway)
	*/
	static i = 0;
	return i++;
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
int tk_bsp_sysinit (void){
	//Hook up the lib (nobody else will) 
	//They allready have pre-set values by the linker so we need
	//to activly re-assign them

	hixs.close        = hixs.close;
	hixs.exit         = hixs.exit;
	hixs.execve       = hixs.execve;
	//hixs.fcntl        = ppc_fcntl;		// <- NOTE
	hixs.fork         = hixs.fork;
	hixs.fstat        = hixs.fstat;
	hixs.getpid       = hixs.getpid;
	hixs.gettimeofday = hixs.gettimeofday; 
	hixs.isatty       = hixs.isatty; 
	hixs.kill         = hixs.kill;
	hixs.link         = hixs.link;
	hixs.lseek        = hixs.lseek;
	//hixs.open         = ppc_open;		// <- NOTE
	//hixs.read         = ppc_read;		// <- NOTE
	//hixs.sbrk         = ppc_sbrk;		// <- NOTE use built in -it's OK
	hixs.settimeofday = hixs.settimeofday;
	hixs.stat         = hixs.stat;
	hixs.times        = bsp_Times;		// <- NOTE
	hixs.unlink       = hixs.unlink;
	hixs.wait         = hixs.wait;
	hixs.write        = bsp_Write;		// <- NOTE
	hixs.syscall_mon  = bsp_Syscall_mon;	// <- NOTE
}

