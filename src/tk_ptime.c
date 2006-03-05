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
 

/*- include files **/
#include <tk.h>
#include <tk_ipc.h>
#include <tk_sysqueues.h>
#include <tk_ptime.h>
#include <kernel/src/tk_hwclock.h>
#include <tk_hwsys.h>
#include <string.h>
#include <errno.h>



/*- local definitions **/
#define TK_MAX_PTIMERS TK_MAX_THREADS //!< Makes no sense to have more timer than threads

#define HWClkID CLK1 //hmm, doesn't work as intended

/* default settings */

/*- external functions **/

/*- external data **/

/*- internal functions **/

/*void insertInPendingList(ptimer_t *timer);*/
/*void deleteFromPendingList(ptimer_t *timer);*/
unsigned int timerdeamon(void *inpar );


/*- public data **/

/*- private data **/

/*- public functions **/

/*- private functions **/

/*- private data **/

/*
ptimer_t timer_pool[TK_MAX_PTIMERS]; //!< Memory pool to avoid usage of malloc

unsigned int currentIdx;             //!< Index to next empty slot
*/

ptimer_t *pendingTimers;             /*!< Sorted linked list to pending times. 
                                          The first one is also pending 
                                          awaitinng triggering from 
                                          timerEvent */

HWclock_stats_t HWclock_stats;        /*!< The quality statistics of the fireing 
                                          mechanism the lower level provides. 
                                          This value must be asked for and
                                          will not containg valid values 
                                          until then.*/
                                          
//Pubplic API
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Creates and initializes the ptime component 
*/
//#define tk_setHWclock_pCLK1(ticks) ( GPT1_vLoadTmr(GPT1_TIMER_3, ticks) )

unsigned long tk_ptime( void ){
   int i;
   HWtick_t hw_ticks;

   pendingTimers = NULL;   
   
   /*
   for (i=0;i<TK_MAX_PTIMERS; i++){
      strncpy(&timer_pool[i].name,"ZOMB",4);
      timer_pool[i].tid         = i;
      timer_pool[i].prev        = NULL;
      timer_pool[i].active      = NO;
      timer_pool[i].numBlocked  = 0;
      timer_pool[i].expTime     = 0;
      timer_pool[i].neededCnts  = 0;
      timer_pool[i].count       = 0;
      timer_pool[i].next        = NULL;
   }
   */
   tk_getHWclock_Quality(	CLK2, &HWclock_stats );
   tk_disarmHWclock(        CLK2 );
   tk_getHWclock(           CLK2, &hw_ticks );
   hw_ticks=HWclock_stats.maxPebbles;
   tk_setHWclock(           CLK2, hw_ticks );
   tk_armHWclock(           CLK2 );

   tk_create_thread("TIME",0,timerdeamon,1,0x600);
   return ERR_OK;
}

/*!
Destroys the ptime component. Any pending threads are released.

@todo: Release threads

@todo: Define timeout codes. timeout_exp, timer_deleted e.t.a.

*/
unsigned long  tk_ptime_destruct( void ){
   int i;

   /*
   todo's here
   */
   
   /*
   for (i=0;i<TK_MAX_PTIMERS; i++){
      strncpy(&timer_pool[i].name,"ZOMB",4);
      timer_pool[i].tid         = i;
      timer_pool[i].prev        = NULL;
      timer_pool[i].active      = NO;
      timer_pool[i].numBlocked  = 0;
      timer_pool[i].expTime     = 0;
      timer_pool[i].neededCnts  = 0;
      timer_pool[i].count       = 0;
      timer_pool[i].next        = NULL;
   }
   */
   return ERR_OK;
}

/*!
Creates (and starts) a timer that will expire <b>at</b> the absolute time 
you set.

*/
unsigned long tk_ptimeevent_at( 
   unsigned int     *tid,     /*!< out: The identity of the timer. Use this id for 
                                   any related operations (for example to 
                                   block on). */
   time_t           *absTime  /*!< in: A absolute time_t defining the event. 
                                   This can be of any lengt or resolution 
                                   supported by you system and the time_t 
                                   struture. @note This is also be the sorting 
                                   criteria/key for the list of pending timers 
                                   */
){
   /*!  Reurns ptime return code */
   /*
   TK_CLI();
   if (timer_pool[currentIdx].active){
      TK_STI();
      return (ERR_NO_MORE_TIMERS);
   }
   
  //Continue with stuff
  
  //Stuff finished - safe to reactivate event sources
  TK_STI();
  */
}

/*!
Creates (and starts) a timer that will expire <b>in</b> the relative time counting from now.


*/
unsigned long tk_ptimeevent_in( 
   unsigned int     *tid,     /*!< out: The identity of the timer. Use this id for 
                                   any related operations (for example to 
                                   block on). */
   time_t           *absTime  /*!< in: A relative time_t defining the event. 
                                   This can be of any lengt or resolution 
                                   supported by you system and the time_t 
                                   struture. */
){                            /*!  Reurns ptime return code */
}

/*!
Destroys a pending timer including cancelling the event. In case any threads are
pending on the timer, those will be released but also given an reason different
from the "normal" case.

This must be a valid timer created by any of the set functions and in must 
not have expired yet. If any these pre-requisits are not met the function
will return an error.

@note To prevent logic missmatch in case a timer is fireing an even while 
removing it's data. The trigger will be attempted to be disarmed first, then if 
that is succesfull interrupts will be disabled during the rest of the operation 
completes.
*/

unsigned long tk_ptimer_destroy( 
   unsigned int  tid          /*!< The identity of the timer.*/
){
  /*
  TK_CLI();
  if (!timer_pool[tid].active){
     TK_STI();
     return(ERR_UNDEF_PTIMER);
  }
  
  //Continue with stuff
  //Stuff finished - safe to reactivate event sources
  TK_STI();
  */
}

//glue API
//------1---------2---------3---------4---------5---------6---------7---------8

/*!
@ingroup kernel_glue

@brief Gives a way to enter specific timers

This gets the current threads internal timer structure (timer_t). Function is
"public" but is <b>not</b> ment to be used by application developers, rather as
a way for different layers of TinKer to interact.

@note <b>For TinKer internal use only</b>

*/
ptimer_t *_tk_ptimer( unsigned int tid ){
   /*
   return(&timer_pool[tid]);
   */
   return tid;
}

//module internal API
//------1---------2---------3---------4---------5---------6---------7---------8

/*!
@brief Internal thread that handles pending timer even requests

On a very low leve (or HW level) the system can only provide <b>one</b>
real time-out event. And even worse: it's limited in either lengt or 
resolution (i.e. a compromize between the two) AND it can wake up 
only one pending thread. 

This manager thread on the other hand, makes it possibe to handle all the
mentioed drawbacks. A time-out event can be of very high resolution AND
destined to happen very far in the future, and when it expieres it can
wake up any number of threads pending on it. Also, you can have several
pending timers pending at the same ptimer event.

Notice that we dont want to be to dependant of any other clocks, which 
is an <b> architectural design issue </b>. I used to think two HW clocks were
nesessarey, but I've desided that that aproach would be to resky since it's 
more complex. Using only the the same clock HW as for all the pending timers
might introduce a higher in accurancy, but I estimate that those errors will 
be handleble. In general this is due to a combination of latency within this 
componens critical parts, tinteger truncation and the frequency of both 
pending timer timeouts and on the fClock used (i.e. the interrupt frecuency). 
In general one can say that the more often the deamon is awaken, the bigger 
will the dirift (and inheritly the inaccurency) be.

Design wise one can argue which parts of the service should be handled in 
this thread. For the time being I tend to let it do as little as possible 
since it will be running on highest priority. The issue is whether it should
organize the pending timer list or not (sort list on each newly started timer 
e.t.a.) as opposed to "share" the timer list with any thread whishing to start 
or cancel a new timer. In the latter case some sort of synchronization must 
be used. However I have a feeling that normal semaphores might not be the right
technique for that.

@note <b>For TinKer internal use only</b> This function is not declared 
public in a headerfile and should never be accessed from outside this module.

@todo Introduce a component area for each component. Where both the adressed 
issue, the solution and the quirks and considerations are described.

*/
unsigned int timerdeamon(void *inpar ){
   unsigned long msg_buf[4];   

   printf("Timer deamon started. Preemtive hi-res timer events now possible\n");
   while (1){
      q_receive(tk_sys_queues[Q_HW_TIMER_EVENT],WAIT,0,msg_buf);
      printf("Timer deamon: %d\n",msg_buf[THWP_EVENT_ID]);
      /*
      THWP_EVENT_ID
      THWP_TIMER_ID                                      
      THWP_RTIME                
      THWP_LATCY
      
      ET_TIMEOUT     
      ET_TIMEOUT_P   
      ET_RELOAD_NEW  
      ET_CANCELLED
      */
      switch (msg_buf[THWP_EVENT_ID]){
      case ET_TIMEOUT: 
         break;
      case ET_TIMEOUT_P: 
         break;
      case ET_RELOAD_NEW: 
         break;
      case ET_CANCELLED: 
         break;
      default:
         printf("tk_ptime: Error - we really need to polish the error handling...\n");
         tk_exit(1);
      };

   }

}

//------1---------2---------3---------4---------5---------6---------7---------8
/** @defgroup PTIMER PTIMER: Pre-emptive timers for TinKer.
@ingroup COMPONENTS 
@brief This package support pre-emptive timer events for TinKer

The main idea behind this component is... APA

@dot
digraph ptime_main {
   node [
      shape=record, 
      style=filled, 
      fillcolor=yellow, 
      fontname=Helvetica, 
      nojustify="true", 
      fontsize=10.0 
   ];
   
   edge [
      dir=both, 
      color="blue:red", 
      fontname=Helvetica,  
      nojustify="true", 
      fontsize=10.0 
   ];
   
   graph [
      rankdir = "TB", 
      fontname=Helvetica,  
      nojustify="true", 
      fontsize=10.0    
   ]; 
    
   tmr_array [ orientation=73.0, label="{\
      <a0> T#0 st | \
      <a1> T#1 st | \
      <a2> T#2 st | \
      <a3> T#3 st | \
      <a..> .. |\
      <aN> T#N st }"];

   tmr_ele0 [label="{<L> L |<tmr_ref> T#0 |<R> R}"]
   tmr_ele1 [label="{<L> L |<tmr_ref> T#1 |<R> R}"]
   tmr_ele2 [label="{<L> L |<tmr_ref> T#2 |<R> R}"]   
   tmr_ele3 [label="{<L> L |<tmr_ref> T#3 |<R> R}"]


   tmr_ele0:tmr_ref:w -> tmr_array:a0:w ;  
   tmr_ele1:tmr_ref:w -> tmr_array:a1:w ;
   tmr_ele2:tmr_ref:w -> tmr_array:a2:w ;
   tmr_ele3:tmr_ref:w -> tmr_array:a3:w ;



   head -> tmr_ele2:L:w;
   tmr_ele2:R:e -> tmr_ele1:L:w;     
   tmr_ele1:R:e -> tmr_ele3:L:w;
   tmr_ele3:R:e -> tmr_ele0:L:w;
   tmr_ele0:R:e -> tail;

   //head [shape=box, orientation=7.0];
   head [shape=box];
   tail [hape=box];

   HWtimer [
      shape=circle, 
      fillcolor=lightcoral, 
      fontcolor=blue
   ];
   
   head -> HWtimer [
      label="Arm with\n pebbles", 
      arrowtail=none, 
      color=red, 
      fontcolor=red, 
      style="bold"
   ]
   
   HWtimer -> head [
      label="Timeout\n event", 
      arrowtail=none, 
      color=blue, 
      fontcolor=blue, 
      shape=vee, 
      style=dashed
   ]
}
@enddot

<p><b>Go gack to</b> \ref COMPONENTS</p>

*/


  
/*! 
 * @defgroup CVSLOG_tk_ptime_c tk_ptime_c
 * @ingroup CVSLOG
 *  $Log: tk_ptime.c,v $
 *  Revision 1.8  2006-03-05 11:11:28  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.7  2006/02/22 13:05:47  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.6  2006/02/20 15:22:01  ambrmi09
 *  Documentation stuff. No code changes.
 *
 *  Revision 1.5  2006/02/19 12:44:33  ambrmi09
 *  - Documented ITC
 *  - Started to build up the structure for the \ref PTHREAD component
 *
 *  Revision 1.4  2006/02/16 15:11:00  ambrmi09
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
 *  Revision 1.3  2006/02/02 16:25:02  ambrmi09
 *  Minor syntax errors fixed
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
