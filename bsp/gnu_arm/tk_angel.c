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
 @brief
 
 This is the Angel generic backend for TinKer. Each trap will execute
 a corresponding  function specific for a certaing board or uC (or for you, 
 the application designer, to define and implement).
  
 */

#include "swi.h"
#include "tk_bsp.h"
#include "../src/context.h"

void Angel_Handler( void )__attribute__((interrupt("SWI")));

void Angel_Handler( void )
{
   int reason; //Reason of the call (see swich case below)
   void* arg;  //Arg can be anything - array, values e.t.a. All depends on the reason
   int *value;  //The return value to the caller
   
   //Pick up the Angel package. R0 is always intact, R2 *should* still be (prolog code seems OK)
   asm volatile ("mov %1, r1; mov %0, r0"
      : "=r" (reason), "=r" (arg) /* Outputs */
      : /* Inputs */
      : "r1", "memory"
	/* Clobbers all */);
	
	//Fix the SP so that normal ret can be used.
    asm volatile ( "ADD     LR, LR, #4" ); 
    
    GET_REG(r12, value);
    value -= 8;                //Now points at the pushed R0 
                               //(Prolog pushes R0,R1,R2,R3,R11,R12,LR,PC)

   switch (reason){

   case AngelSWI_Reason_Open            :
      {
         int  *block = (int*)arg;
         char *name  = (char*)   block[0];
         int   mode  =           block[1];
         int   len   =           block[2];
         
         *value = bsp_Open(name, mode, len);	
      }
      break;
//   case AngelSWI_Reason_Close           :break;
//   case AngelSWI_Reason_WriteC          :break;
//   case AngelSWI_Reason_Write0          :break;

   case AngelSWI_Reason_Write           :      
      {
         int   *block = (int*)arg;
         int   fh;
         char  *ptr;
         int   len;
         
         fh  =        block[0];
         ptr = (char*)block[1];
         len =        block[2];
         *value = bsp_Write(fh, ptr, len);
      }
      break;

//   case AngelSWI_Reason_Read            :break;
//   case AngelSWI_Reason_ReadC           :break;
//   case AngelSWI_Reason_IsTTY           :break;
//   case AngelSWI_Reason_Seek            :break;
//   case AngelSWI_Reason_FLen            :break;
//   case AngelSWI_Reason_TmpNam          :break;
//   case AngelSWI_Reason_Remove          :break;
//   case AngelSWI_Reason_Rename          :break;
     case AngelSWI_Reason_Clock           :
        *value = bsp_Clock();
        break;
//   case AngelSWI_Reason_Time            :break;
//   case AngelSWI_Reason_System          :break;
//   case AngelSWI_Reason_Errno           :break;
//   case AngelSWI_Reason_GetCmdLine      :break;
//   case AngelSWI_Reason_HeapInfo        :break;
//   case AngelSWI_Reason_EnterSVC        :break;
//   case AngelSWI_Reason_ReportException :break;

   default:
      //Should never happen. But in case it does, return a really wacky value
      //and let's hope the caller picks up the hint.
      *value = -1; /*EOF*/
   }
}

/*!
 * @defgroup CVSLOG_tk_angel_c tk_angel_c
 * @ingroup CVSLOG
 *  $Log: tk_angel.c,v $
 *  Revision 1.4  2006-11-27 22:29:22  ambrmi09
 *  Minor djustments completeing the move of some header files to public and due
 *  to some name clashed with user space naming conventions.
 *
 *  Revision 1.3  2006/09/13 18:29:30  ambrmi09
 *  Commited needed in repocitory
 *
 *  Revision 1.2  2006/04/08 10:15:55  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.1.2.1  2006/04/07 12:10:06  ambrmi09
 *  Skeleton for handling syscalls using the ARM Angel interface in place
 *
 *  Basic terminal I/O for gnu_arm (LPC2129) - only output so far (input requires
 *  blocking).
 *
 */


