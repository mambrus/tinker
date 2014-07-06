/***************************************************************************
 *   Copyright (C) 20006, Michael Ambrus                                   *
 *   michael.ambrus@maquet.com                                             *
 *   Copyright (C) 2014, Michael Ambrus                                    *
 *   michael.ambrus@gmail.com                                              *
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

@brief Tinker inner-most \e"guts"

The contents in this file concerns context switching. This is a kernels most
critical area where it's extremely venerable.

Two types of threats exist:

- Interrupts - Certain context switching techniques are sensitive for
  interrupts (but not, not all).

- Primitiveness - the functions in this file do not like to be preempted

@note This is a mandatory file as part of the \ref SCHED component

For in-depth discussions about this component, see \ref
SCHED

@see COMPONENTS
*/

#include <tk.h>
#include "context.h"
#include "tk_tuning.h"
#include "implement_tk.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

extern struct tcb_t_ __tk_threadPool[TK_MAX_THREADS];
extern thid_t __tk_schedule[TK_MAX_PRIO_LEVELS][TK_MAX_THREADS_AT_PRIO];
extern struct stat_t __tk_roundrobin_prio_idxs[TK_MAX_PRIO_LEVELS];
extern thid_t __tk_thread_to_run;
extern thid_t __tk_active_thread;

static int npreempt = 0;
static int inpreempt = 0;
static int outpreempt = 0;
thid_t preemptlist[TK_MAX_THREADS];

/*!
Called by _ny versions of itc. A special ready-list for preemptable threads
*/
void tk_preemplist(thid_t thid)
{
	npreempt++;
	preemptlist[inpreempt] = thid;
	inpreempt++;
	inpreempt = inpreempt % TK_MAX_THREADS;
}

/*!
@brief create a "virtual" timeout event (whenever possible).

Iterates though the whole schedule, trying to find sleeping threads
who's timeout time has expired and flag them as \ref READY.

@see PROCSTATE

In non-preemptive mode there is no way to wake up a sleeping thread any
other way. Therefore \b detection thereof must be done.

This is done mainly by the idle thread (it has nothing else to do
anyway) and in all the context-swappable points (i.e. in principle in
most of the public functions belonging to \ref SCHED ).

There those points are is easiest to find out by tracing the \ref
tk_yield function.

This concept is a heritage from the non-preemptive concept, but waking
up sleeping threads this way is still done. This will also be kept in in
the \ref idle thread even when \ref PTIMER package is finished.

When \ref PTIMER package is finished, the \ref tk_yield might change
behaviour, but the \ref idle will still not (to keep compatibility
towards the old \ref ITC). Note that timeout handling for \ref PTHREAD
is planned to be handled by \ref PTIMER.

Timing using this concept was (and still is) mainly intended for simple
blocking timeouts in \ref ITC and was never intended for high-precision
time keeping (even though that works quite well also).

*/
void _tk_wakeup_timedout_threads(void)
{
	thid_t i;
	clock_t act_time_us;
	int _npreempt;

	//act_time = tk_clock();
	act_time_us = tk_clock() * (1000000uL / CLOCKS_PER_SEC);
	//Note: The following is not just a sanity check, the list might be
	//fragmented.Explains also why the whole table (pool) has to be
	//traversed, and not just [0..procs_in_use]
	for (i = 0; i < TK_MAX_THREADS; i++) {
		TK_CLI();
		_npreempt = npreempt;
		TK_STI();

		if (_npreempt > 0) {
			return;
		}

		TK_CLI();
		if (__tk_threadPool[i].valid) {
			if (__tk_threadPool[i].state & SLEEP) {	//This one is sleeping. Time to wake him up?
				//if ( act_time >= __tk_threadPool[i].wakeuptime ){
				//if ( (signed long)(act_time - __tk_threadPool[i].wakeuptime) >= 0 ){
				if (tk_difftime
				    (act_time_us,
				     __tk_threadPool[i].wakeuptime) >= 0) {
					/*Release queues also (but not the TERM bit) */
					__tk_threadPool[i].state =
					    (PROCSTATE) (__tk_threadPool[i].
							 state & ~_____QS_);
					__tk_threadPool[i].wakeupEvent =
					    E_TIMER;
				}
			}
			//This thread wants to die (zombied). But make sure someone else is doing it
			//(otherwise we would remove our own stack, which is both ugly and dangerous)
			if ((__tk_threadPool[i].state & ZOMBI)
			    && (i != __tk_active_thread)) {

				//We should not really need to do the following line. Should
				//have been done in any canceling mechanism of the thread.
				//Note and commented call saved for future reference.

				//_tk_detach_children(i);

				if (_tk_try_detach_parent(i, TK_FALSE)) {
					//Finally, it should now be safe to send the zombied thread to Nirvana
					tk_delete_thread(i);
				}
				//else: The parent is either blocking on a brother or sister OR
				//neither, but might consider blocking later. I.e. we have to stay
				//zombied until it decides (i.e. either joins us, dies or someone detaches us).
			}
		}
		TK_STI();
	}
}

/*!
Dispatch or yield to another thread that has more "right" to run.

I.e. <b>search</b> for another potential thread that is now in \e "run-able"
state and that has higher priority.

Also: In non-preemptive mode (and as a secondary duty), it creates simulated
timeout-events on all threads currently sleeping, but who's timeout has
expired (i.e. search for expired timeouts and change state to READY if needed).

@note In TinKer lingo we use the words
- \e "sleeping" - for tread blocked on timeout
- \e "blocked" - for thread blocked for any other reason

The concept of passively sleeping threads but actively awaken by the
kernel (in idle thread or any dispatch point) is a key-concept in
non-preemptable mode. In this mode the kernel is constantly searching
for who's ready to run. This is normally done in a specially
dedicated - the \ref _tk_idle "idle thread" (i.e. the lowest priority
thread).

@note This is the function to put here and there in you application if
you run a non-preemptable version of TinKer. Any execution will not
be context switched until you either execute this function or run any
other kernel function. <b>Any other TinKer kernel function is also a
context switching point.</b>. If you don't want that behaviour you have
to use the non-yielded version of that function (i.e. with suffix -ny)
<p>

@note to use this function in <b>preemptable mode</b> you must make sure
that there is no prefix-postfix stuff happening on the stack, neither
before the first PUSHALL nor after the last POPALL.

*/
void tk_yield(void)
{
	assert(__tk_IntFlagCntr == 0);

	TK_CLI();
	PUSHALL();
	TK_STI();

	_tk_wakeup_timedout_threads();

	TK_CLI();

	//Do not premit interrupts between the following two. Proc statuses
	//(i.e. thread statuses) frozen in time.
	if (npreempt > 0) {
		assert(npreempt < 2);
		npreempt--;
		__tk_thread_to_run = preemptlist[outpreempt];
		outpreempt++;
		outpreempt = outpreempt % TK_MAX_THREADS;
	}

	__tk_thread_to_run = _tk_next_runable_thread();
	_tk_context_switch_to_thread(__tk_thread_to_run, __tk_active_thread);

	POPALL();
	TK_STI();

}

/*!
Dispatch or \e yield to another thread that has more "right" to run.

This is basically identical with tk_yield(), but it doesn't try to
search for threads to wake up that are sleeping on non-preempting
timers. This function is meant to be used for preemptable scheduling
and to be called from an event source (i.e. ISR).

@see void tk_yield( void )

*/
void tk_yield_event(void)
{
	TK_CLI();
	PUSHALL();
	__tk_thread_to_run = _tk_next_runable_thread();
	_tk_context_switch_to_thread(__tk_thread_to_run, __tk_active_thread);
	POPALL();
	TK_STI();
}
