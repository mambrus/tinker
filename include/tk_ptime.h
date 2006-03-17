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
@ingroup PTIMER

@brief XXX

This is XXX

For in-depth discussions XXX \ref
PTIMER

@see PTIMER

*/
   

 
#ifndef TK_PTIMER_H
#define TK_PTIMER_H

/*- include files **/
//#include <time.h>

#include <tk_itc.h> //last in error chain


/*- public definitions **/

/*COMPONENT PTIMER*/                   

enum PTIMER_ERROR_CODES{

ERR_UNDEF_PTIMER=ERR_ITC_SENTINEL   ,//!< @brief No such or invalid ptimer 
ERR_TIME_NEG               ,//!< @brief Trying to set event for time that's passed 
ERR_TIMER_DELETED          ,//!< @brief The timer (i.e. the requested event) has been deleted 
ERR_NO_MORE_TIMERS         ,//!< @brief You try to exeed the maximum number of pending timers

ERR_PTIMER_SENTINEL
};


/*!
@name Protocol macros

Macros involved with the protocol between HW clock and service thread

*/
//@{

#define ET_TIMEOUT       0   /*!< Timeout event finished - timer all completed.
                                  */
#define ET_TIMEOUT_P     1   /*!< Timeout event part - timer partly completed.
                                  HWclock_ needs reloading */
#define ET_RELOAD_NEW    2   /*!< Current timer was disrupted by a new timer that
                                  expires earlier than the pending one. 
                                  THWP_RTIME will contain the remaining bullets */
#define ET_CANCELLED     3   /*!< Current timer was cancelled.                                  
                                  THWP_RTIME will contain the remaining bullets */

//@}



/*! This index enumeration determines that each index in the array passed from 
    the HW ISR to the driver means. */
typedef enum{
   THWP_EVENT_ID, /*!< Reason why the service thread is awaken (see the ET_XXX
                       macros*/
   THWP_TIMER_ID, /*!< The event relates to this timer number (used for sanity 
                       checks for cases where interrupts happen during charging 
                       of the HWclock_) */
   THWP_RTIME,    /*!< Time that remains. Bullets remaing in HWclock_ - This field 
                       is very important and will always contain a valid value 
                       (remaining in shift register) no matter the event 
                       reason. The value is used by the service thread to use 
                       to update it's own internal record of time. The value 
                       tells the service how much time was left until the next 
                       <b>expected</b> event. 0 is a special case meaning that 
                       the event has arrived as expected in time. */
   THWP_LATCY,    /*!< Extra field that can be used for compensating latency 
                       (for targets where ISR can measure it) */
   THWP_PROT_SZ   /*!< Internal - Use to determine needed sizes for array using 
                       these indexes can be determined by name */
}e_timerHW_protIdx;

/*- local definitions **/

/*- Error codes **/

//#define ERR_OK             	0x00 /*!< No error */
//#define ERR_UNDEF_PTIMER   	0x02 /*!< No such or invalid ptimer */
//#define ERR_TIME_NEG       	0x03 /*!< Trying to set event for time that's passed */
//#define ERR_TIMER_DELETED  	0x04 /*!< The timer (i.e. the requested event) has been deleted */
//#define ERR_NO_MORE_TIMERS 	0x06 /*!< You try to exeed the maximum number of pending timers */


/*!
TinKers ptimer_t structure. This structure contains all
information the kernel needs to know about pre-emptable timers (ptimers).
*/
typedef struct ptimer_s{
   char              name[4];    //!< Name of the timer
   unsigned int      tid;        //!< Identity of this timer
   struct ptimer_s  *prev;       //!< Previous in list
   unsigned int      active;     //!< If this timer is active or not @note: <b>internal use only </b>
   unsigned int      numBlocked; //!< Number of blocked threads on this timer
   time_t            expTime;    //!< Expire time
   unsigned int      neededCnts; //!< Number of iterations needed to complete
   unsigned int      count;      //!< Completed this number of iterations
   struct ptimer_s  *next;       //!< Next in list
}ptimer_t;



/*- default settings **/

/*- external functions **/

/*- external data **/

/*- internal functions **/

/*- public data **/

/*- private data **/

/*- public functions **/

/*!
@name Creation and destruction of this component

Use these functions only at boot, and shut-down.

@see COMPONENTS
*/
//@{
unsigned long  tk_ptime          ( void );
unsigned long  tk_ptime_destruct ( void );
//@}

/*!
@name API for createing and destoy timers 

These functions are involved with creating and destroying timers
*/
//@{
unsigned long  tk_ptimeevent_at ( thid_t *tid, time_t *absTime );
unsigned long  tk_ptimeevent_in ( thid_t *tid, time_t *relTime );
unsigned long  tk_ptimer_destroy( thid_t  tid );
//@}

/*!
@name API using the service

These functions are involved with creating and destroying timers
*/
//@{
unsigned long  tk_ptimer_block  ( thid_t  tid );
unsigned long  tk_ptimer_awake  ( thid_t  tid );
unsigned long  tk_ptimer_sleep  ( thid_t  tid, time_t *relTime );
//@}

/*- private functions **/

//------1---------2---------3---------4---------5---------6---------7---------8


#endif

  
/*! 
 * @defgroup CVSLOG_tk_ptime_h tk_ptime_h
 * @ingroup CVSLOG
 *
 *  $Log: tk_ptime.h,v $
 *  Revision 1.11  2006-03-17 12:20:03  ambrmi09
 *  Major uppdate (5 days hard work)
 *
 *  - Finally tied up all loose ends in the concept. Threads are now
 *  joinable
 *
 *  - Corrected one error: compacting scheduele while cancelling a
 *  threads
 *
 *  - Several new API, mainly concerned with cancelation (corrsp pThread
 *  also)
 *
 *  - Found a nasty bug while creating threads in threads for XC167. TOS is
 *  really a patchy solution ;( This one had to do with the compiler
 *  being fooled by the inline assembly and optimized something that was not
 *  optimizable (saving stack segment got wacked).
 *
 *  - Designed a concurrent qsort test-app. This is good for showing
 *  boss-worker model. Number of threads recoed on XC167 was 50 and on MSVS
 *  more than 150! Interesting to notice was that TinKer creation and
 *  cancelation for threads was much faster than Windows own (20-30 times
 *  faster).
 *
 *  - A MSVC workspace for pThreads-Win32. Good for testing apps
 *  transparency.
 *
 *  - Increased memory on XC167 (phyCore HW). now 32k for stacks and 16k for
 *  malloc. We still lack RAM that is not deployed (pHycore has
 *  128k + 256k physical RAM memory i think). Maximum for
 *  stack is 64k however (type of pointers determine this). If memory is
 *  increased further, we get a TRAP_B saying bad memory interface. Typical
 *  error for config memory issues in DaVe.
 *
 *  Revision 1.10  2006/03/12 15:08:54  ambrmi09
 *  - Adjusted the source to accomodate the new file structure.
 *
 *  - All build environments uppdated and verified except BC5. For this one
 *  we stumbled across the header-file issue that I've been fearing. Seems
 *  we need to take care of that one after all.
 *
 *  @note The malloc bug still not solved.
 *
 *  Revision 1.9  2006/03/05 11:11:26  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.8  2006/03/02 14:05:49  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.7  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.6  2006/02/20 15:22:00  ambrmi09
 *  Documentation stuff. No code changes.
 *
 *  Revision 1.5  2006/02/19 12:44:33  ambrmi09
 *  - Documented ITC
 *  - Started to build up the structure for the \ref PTHREAD component
 *
 *  Revision 1.4  2006/02/17 21:17:26  ambrmi09
 *  More doc structure - this time for the new PTHREAD's components
 *
 *  Revision 1.3  2006/02/16 15:11:00  ambrmi09
 *  Introduced a new component for better and safer useage of the heap.
 *  Package is called \red KMEM and the files are tk_mem.c and tk_mem.h (so
 *  far).
 *
 *  Started to take care of the long needed issue with error codes and
 *  better error handling. Introduced errno.h to begin with, whitch is part
 *  of the package \ref kernel_reimpl_ansi. Its not a good solution yet,
 *  since both kernel and ANSI codes are in the same file we have to invent
 *  a way to omit the ANSI defines when a tool-chain that has errno.h is
 *  used.
 *
 *  Revision 1.2  2006/02/02 15:51:02  ambrmi09
 *  A lot of thought has been invested into the new PTIME component. Had to
 *  change things even in the systime parts (integrated in the SHEDUL
 *  component) to make it more generic. Think this will be really nice when
 *  it's ready, but has been a long road to get PTIME running (and I'm
 *  still not there).
 *
 *  Revision 1.1  2005/12/04 15:48:52  ambrmi09
 *  API for ne pre-emptable timers in place. Implementing this will be a
 *  hard but fun "nut" to crack. ptime has the potential of comming
 *  very close to the high-res timers that POSIX 1003.1c define and is a
 *  good pointer whether pthreads is a good next step or not for TinKer.
 *
 *  
 *******************************************************************/









