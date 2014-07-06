/***************************************************************************
 *   Copyright (C) 2005,2006 by Michael Ambrus                             *
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
#include <tk_itc.h>
#include <tk_sysqueues.h>
#include <tk_ptime.h>
#include <kernel/src/tk_hwclock.h>
#include <tk_hwsys.h>
#include <string.h>
#include <errno.h>

/*- local definitions **/
#define TK_MAX_PTIMERS TK_MAX_THREADS	//!< Makes no sense to have more timer than threads

#define HWClkID CLK1		//hmm, doesn't work as intended

/* default settings */

/*- external functions **/

/*- external data **/

/*- internal functions **/

/*void insertInPendingList(ptimer_t *timer);*/
/*void deleteFromPendingList(ptimer_t *timer);*/
void *timerdeamon(void *inpar);

/*- public data **/

/*- private data **/

/*- public functions **/

/*- private functions **/

/*- private data **/

/*
ptimer_t timer_pool[TK_MAX_PTIMERS]; //!< Memory pool to avoid usage of malloc

unsigned int currentIdx;             //!< Index to next empty slot
*/

ptimer_t *pendingTimers;	/*!< Sorted linked list to pending times.
				   The first one is also pending
				   awaiting triggering from
				   timerEvent */

HWclock_stats_t HWclock_stats;	/*!< The quality statistics of the firing
				   mechanism the lower level provides.
				   This value must be asked for and
				   will not contain valid values
				   until then. */

//Pubplic API
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Creates and initializes the ptime component
*/
//#define tk_setHWclock_pCLK1(ticks) ( GPT1_vLoadTmr(GPT1_TIMER_3, ticks) )

unsigned long tk_ptime(void)
{
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
	tk_getHWclock_Quality(CLK2, &HWclock_stats);
	tk_disarmHWclock(CLK2);
	tk_getHWclock(CLK2, &hw_ticks);
	hw_ticks = HWclock_stats.maxPebbles;
	tk_setHWclock(CLK2, hw_ticks);
	tk_armHWclock(CLK2);

	tk_create_thread("TIME", 0, timerdeamon, 1, 0x600);
	return ERR_OK;
}

/*!
Destroys the ptime component. Any pending threads are released.

@todo: Release threads

@todo: Define timeout codes. timeout_exp, timer_deleted e.t.a.

*/
unsigned long tk_ptime_destruct(void)
{
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
unsigned long tk_ptimeevent_at(unsigned int *tid,	/*!< out: The identity of the timer. Use this id for
							   any related operations (for example to
							   block on). */
			       time_t * absTime	/*!< in: A absolute time_t defining the event.
						   This can be of any length or resolution
						   supported by you system and the time_t
						   struture. @note This is also be the sorting
						   criteria/key for the list of pending timers
						 */
    )
{
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
unsigned long tk_ptimeevent_in(unsigned int *tid,	/*!< out: The identity of the timer. Use this id for
							   any related operations (for example to
							   block on). */
			       time_t * absTime	/*!< in: A relative time_t defining the event.
						   This can be of any lengt or resolution
						   supported by you system and the time_t
						   struture. */
    )
{				/*!  Returns ptime return code */
}

/*!
Destroys a pending timer including cancelling the event. In case any threads are
pending on the timer, those will be released but also given an reason different
from the "normal" case.

This must be a valid timer created by any of the set functions and in must
not have expired yet. If any these pre-requisites are not met the function
will return an error.

@note To prevent logic mismatch in case a timer is firing an even while
removing it's data. The trigger will be attempted to be disarmed first, then if
that is successful interrupts will be disabled during the rest of the operation
completes.
*/

unsigned long tk_ptimer_destroy(unsigned int tid	/*!< The identity of the timer. */
    )
{
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
"public" but is <b>not</b> meant to be used by application developers, rather as
a way for different layers of TinKer to interact.

@note <b>For TinKer internal use only</b>

*/
ptimer_t *_tk_ptimer(unsigned int tid)
{
	/*
	   return(&timer_pool[tid]);
	 */
	return tid;
}

//module internal API
//------1---------2---------3---------4---------5---------6---------7---------8

/*!
@brief Internal thread that handles pending timer even requests

On a very low level (or HW level) the system can only provide <b>one</b>
real time-out event. And even worse: it's limited in either length or
resolution (i.e. a compromise between the two) AND it can wake up
only one pending thread.

This manager thread on the other hand, makes it possible to handle all the
mentioned drawbacks. A time-out event can be of very high resolution AND
destined to happen very far in the future, and when it expires it can
wake up any number of threads pending on it. Also, you can have several
pending timers pending at the same ptimer event.

Notice that we don't want to be to dependant of any other clocks, which
is an <b> architectural design issue </b>. I used to think two HW clocks were
necessary, but I've decided that that approach would be to risky since it's
more complex. Using only the same clock HW as for all the pending timers
might introduce a higher in accuracy, but I estimate that those errors will
be handleable. In general this is due to a combination of latency within this
component critical parts, integer truncation and the frequency of both
pending timer timeouts and on the fClock used (i.e. the interrupt frequency).
In general one can say that the more often the daemon is awaken, the bigger
will the drift (and inherently the inaccuracy) be.

Design wise one can argue which parts of the service should be handled in
this thread. For the time being I tend to let it do as little as possible
since it will be running on highest priority. The issue is whether it should
organize the pending timer list or not (sort list on each newly started timer
e.t.a.) as opposed to "share" the timer list with any thread wishing to start
or cancel a new timer. In the latter case some sort of synchronization must
be used. However I have a feeling that normal semaphores might not be the right
technique for that.

@note <b>For TinKer internal use only</b> This function is not declared
public in a header-file and should never be accessed from outside this module.

@todo Introduce a component area for each component. Where both the addressed
issue, the solution and the quirks and considerations are described.

*/
void *timerdeamon(void *inpar)
{
	unsigned long msg_buf[4];

	printk(("Timer deamon started. Preemtive hi-res timer events now possible\n"));
	while (1) {
		q_receive(tk_sys_queues[Q_HW_TIMER_EVENT], WAIT, 0, msg_buf);
		printk(("Timer deamon: %d\n", msg_buf[THWP_EVENT_ID]));
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
		switch (msg_buf[THWP_EVENT_ID]) {
		case ET_TIMEOUT:
			break;
		case ET_TIMEOUT_P:
			break;
		case ET_RELOAD_NEW:
			break;
		case ET_CANCELLED:
			break;
		default:
			printk(("tk_ptime: Error - we really need to polish the error handling...\n"));
			tk_exit(TC_UNKNOWN);
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
