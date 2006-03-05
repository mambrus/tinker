/***************************************************************************
 *   Copyright (C) 2006 by Michale Ambrus                                  *
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
@file
@ingroup SCHED

@brief Definitions that don't need to be public 

Definitions that don't need to be public for the \ref SCHED component.
Keeps all the internals out of th.h

For in-depth discussions about this component, see \ref
SCHED

*/



#ifndef _IMPLEMENT_TK_H
#define _IMPLEMENT_TK_H

/*!
@brief Defines the threads status (bit addressable)
Defines the threads status (bit addressable)
	- __T = TERM  = Process is waiting for one or more children to terminate
	- _S_ = SLEEP = Process is blocked on timer (sleeping)
	- Q__ = QUEUE = Process is blocked on queue or semaphore
   
@note the convenient naming. This is very practical when debugging the
dispatcher since symbolic names will easily be translated to
corresponding bits.
*/
typedef enum {
   READY    =0x0,     
   _______T =0x1,  
   ______S_ =0x2,  
   ______ST =0x3,
   _____Q__ =0x4,  
   _____Q_T =0x5,  
   _____QS_ =0x6,  
   _____QST =0x7,
   ZOMBIE   =0X80
}PROCSTATE;

/*!
@brief Defines on what the threads is blocked. 
Defines on what the threads is blocked. 
*/
typedef enum{TERM=1,SLEEP=2,QUEUE=4}STATEBITS;

/*!
@brief Wake-up events. 
Wake-up events (i.e. last reason to go ready)
*/
typedef enum{E_CHILDDEATH, E_TIMER, E_ITC, E_ITC2}wakeE_t;

/*!
Thread control block (TCB). This structure contains all
information the kernel needs to know about a thread.

@note the type for various stack pointers. This type is supplied by BSP
adaptions to cover certain architectures special aspects of a "stack".
In a 32bit "normal" CPU this is often a char*, but for some obscure MPU:s
like the C166 family, this is a much more complex structure.
*/

typedef struct tcb_t{
   unsigned int   Thid,Gid;             //!< Process ID and Parent ID (Gid)
   unsigned int   noChilds;            //!< Numb of procs this has created
   char           name[TK_THREAD_NAME_LEN+1]; //!< Name of the thread (+ 1 extra for byte terminating zero)
   BOOL           isInit;              //!< Memory for stack is allocated
   PROCSTATE      state;               //!< State of the process
   int            _errno_;               //!< Support of per thread errno
   stack_t        stack_begin;         //!< First address of stack memory
   stack_t        curr_sp;             //!< Current stackpointer of this thread
   size_t         stack_size;          //!< Size of stack
   unsigned long  stack_crc;           //!< Control value of integrity check
   clock_t        wakeuptime;          //!< When to wake up if sleeping
   wakeE_t        wakeupEvent;         //!< Helper variable mainly for ITC
   start_func_f   start_funct;         //!< Address of the threads entry function. Used ONLY for debugging purposes
   init_func_f    init_funct;          //!< Support of the pThread <em>"once"</em> concept.
   void          *prmtRetAddr;         //!< Preempted return adress - used in preempted mode.
   unsigned int   Prio,Idx;            //!< Helpers, prevent need of lookup
}tk_tcb_t;


typedef struct stat_t{
   unsigned short procs_at_prio;    //Used for optimizing sheduler.
   unsigned short curr_idx;
}prio_stat_t;




#endif /* _IMPLEMENT_PTHREAD_H */


/*!
 * @defgroup CVSLOG_implement_tk_h implement_tk_h
 * @ingroup CVSLOG
 *  $Log: implement_tk.h,v $
 *  Revision 1.4  2006-03-05 11:11:27  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.3  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  
 */

