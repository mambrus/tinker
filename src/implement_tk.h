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

#include "tk_tuning.h"
#include "stypes.h"

/*!
@brief Defines the threads status (bit addressable)
Defines the threads status (bit addressable)
	- ___T = TERM  = Process is waiting for one or more children to terminate
	- __S_ = SLEEP = Process is blocked on timer (sleeping)
	- _Q__ = QUEUE = Process is blocked on \b any kind of synchronisation primitive. I.e. semaphore, mutex, queue, pthread conditional variable e.t.a.
	- Z___ = ZOMBI = Process is but waiting for cleanup operation (usually idle) to remove it

@note the convenient naming. This is very practical when debugging the
dispatcher since symbolic names will easily be translated to
corresponding bits. This is actually the only \e real reason for this enum to
exist, since \ref STATEBITS does the same job.
*/
typedef enum {
	READY = 0x00,
	_______T = 0x01,
	______S_ = 0x02,
	______ST = 0x03,
	_____Q__ = 0x04,
	_____Q_T = 0x05,
	_____QS_ = 0x06,
	_____QST = 0x07,
	____Z___ = 0x08,
	____Z__T = 0x09,
	____Z_S_ = 0x0A,
	____Z_ST = 0x0B,
	____ZQ__ = 0x0C,
	____ZQ_T = 0x0D,
	____ZQS_ = 0x0E,
	____ZQST = 0x0F
} PROCSTATE;

#define TERM  _______T		//!< "Terminated" state (simplified PROCSTATE)
#define SLEEP ______S_		//!< "Sleeping" state (simplified PROCSTATE)
#define QUEUE _____Q__		//!< "Blocked" state (simplified PROCSTATE)
#define ZOMBI ____Z___		//!< "Zombi" state (simplified PROCSTATE)

/*!
@brief Defines what the threads is blocked on.
Defines on what the threads is blocked.

Note that the Q means it's blocked on a entity else than internal \ref
SCHED, which means that it's blocked on a synch entity of some kind.

I.e. anything else than another thread, cancelation or native timeout.
*/
//typedef enum{TERM_BIT=1,SLEEP_BIT=2,QUEUE_BIT=4,ZOMBI_BIT=8}STATEBITS;

/*!
@brief Wake-up events.
Wake-up events (i.e. last reason to go ready)
*/
typedef enum { E_NONE, E_CHILDDEATH, E_TIMER, E_ITC, E_ITC2 } wakeE_t;

/*
If PROCSTATE has Q flag set, bOnID in \ref tk_tcb_t will contain
information about any of the following synch entities:
 - TK_ITC entities
   - TK semaphore
   - TK queue
   - TK vqueue

 - Pthread entities
   - Mutexes
   - Conditional variables
   - Keys

 - POSIX 1b
   - Semaphores

 - Ptimer
*/
typedef enum {
	BON_SCHED = 0,		//!< Either other thread or self (native timer)
	BON_ITC,		//!< Any of the native ITC entities
	BON_PMUTEX,		//!< Pthread mutex
	BON_SEMAPHORE,		//!< POSIX 1b semaphore
	BON_PTIMER		//!< pTimer
} bon_sel_t;

/*!
Information about what the thread is blocked on

@note TinKer \ref SCHED doesn't actively use this info. It is for other blocking
packages so that they can have their own "place holder" in the TCB.

*/
typedef struct bon_t_ {
	bon_sel_t kind;
	union {
		struct tcb_t_ *tcb;
		struct itc_t_ *itc;
		struct pthread_mutex_t_ *mutex;
	} entity;
} bon_t;

/*!
Thread control block (TCB). This structure contains all
information the kernel needs to know about a thread.

@note the type for various stack pointers. This type is supplied by BSP
adoptions to cover certain architectures special aspects of a "stack".
In a 32bit "normal" CPU this is often a char*, but for some obscure MPU:s
like the C166 family, this is a much more complex structure.

@todo Insert a compile time check that prevents number of threads to be
larger than MAX_UNT/2 (i.e. negative int)
*/

typedef struct tcb_t_ {
	thid_t Thid, Gid;	//!< Process ID and Parent ID (Gid). A Gid of -1 would indicate a detached (parent-less) thread
	int noChilds;		//!< Numb of procs this has created
	char name[TK_THREAD_NAME_LEN + 1];	//!< Name of the thread (+ 1 extra for byte terminating zero)
	TK_BOOL valid;		//!< This TCB is active and contains valid info about a thread.
	PROCSTATE state;	//!< State of the process
   bon_t bOnId;		/*!< The ID of the \b main entity this thread is
   blocked on (either other threads-ID or ITC or ptimer-ID). If several
   entities are reason for blocking, only the main entity will be
   mentioned here.*/
	
   int _errno_;		//!< Support of per thread errno
	stack_t stack_begin;	//!< First address of stack memory       (This value will be static during execution)
	stack_t curr_sp;	//!< Current stack-pointer of this thread (This value will vary during execution)
	size_t stack_size;	//!< Size of stack
	unsigned long stack_crc;	//!< Control value of integrity check
	clock_t wakeuptime;	//!< When to wake up if sleeping
	wakeE_t wakeupEvent;	//!< Helper variable mainly for ITC
	void *retval;		//!< The return value of a thread. Either return code or value of \ref tk_threadexit()
/*
   //Obsolete stuff - kept for reference
   start_func_f   start_funct;         //!< Address of the threads entry function. Used ONLY for debugging purposes
   init_func_f    init_funct;          //!< Support of the pThread <em>"once"</em> concept.
   void          *prmtRetAddr;         //!< Preempted return address - used in preempted mode.
*/
	unsigned int Prio, Idx;	//!< Helpers, prevent need of lookup
} tk_tcb_t;

typedef struct stat_t {
	unsigned short procs_at_prio;	//Used for optimizing scheduler.
	unsigned short curr_idx;
} prio_stat_t;

//------1---------2---------3---------4---------5---------6---------7---------8
struct tcb_t_ *_tk_current_tcb(void);
struct tcb_t_ *_tk_specific_tcb(thid_t id);
void _tk_main(void);
void tk_trap(int ec);
int _tk_try_detach_parent(thid_t, int);
thid_t _tk_next_runable_thread(void);
void _tk_context_switch_to_thread(thid_t, thid_t);
//------1---------2---------3---------4---------5---------6---------7---------8

/* User-provided */
extern int root(void);

/*- public data **/

/*- private data **/
extern int __tk_IntFlagCntr;

#if (TK_HOWTO_CLOCK == TK_FNK_STUBBED)
clock_t clock_stubbed();
#define tk_clock()  clock_stubbed()
#else
#define tk_clock()  clock()
#endif

#if defined(TK_HOSTED)
/* We don't care it's fast enough */
#define tk_difftime difftime
#else
/* Light version of ANSI difftime with the only difference that it doesn't
   return a double, but a time_t instead. This guarantees correctness in
   both value and signed-ness as long as:
 *
 * 1: No casting in (a macro couldn't prevent this)
 * 2: Both Left-hand value and operands are of same size and all are signed.
 * 3: The difference is never larger than ((1/2 value-range) -1) for time_t
 * 4: Compiler can't figure out on beforehand that signed-overflow on any of
 *    it's operands can occur, or it has the right to optimize any
 *    evaluation to constantly TRUE or constantly FALSE (NEW).
 *
 * */
static time_t _tk_difftime(time_t t1, time_t t0)
{
	return t1 - t0;
}

#define tk_difftime _tk_difftime
#endif

#endif				/* _IMPLEMENT_TK_H */

