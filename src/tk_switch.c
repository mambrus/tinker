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
#include <tk.h>
#include "context.h"
#include "tk_tuning.h"
#include "implement_tk.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#define LDATA struct tcb_t_
#include <mlist/mlist.h>

extern struct tcb_t_ __tk_threadPool[TK_MAX_THREADS];
extern thid_t __tk_schedule[TK_MAX_PRIO_LEVELS][TK_MAX_THREADS_AT_PRIO];
extern struct stat_t __tk_roundrobin_prio_idxs[TK_MAX_PRIO_LEVELS];
extern thid_t __tk_thread_to_run;
extern thid_t __tk_active_thread;

static int npreempt = 0;
static int inpreempt = 0;
static int outpreempt = 0;
thid_t preemptlist[TK_MAX_THREADS];

void tk_preemplist(thid_t thid)
{
	npreempt++;
	preemptlist[inpreempt] = thid;
	inpreempt++;
	inpreempt = inpreempt % TK_MAX_THREADS;
}

void _tk_wakeup_timedout_threads(void)
{
	thid_t i=0;
	clock_t act_time_us;
	int _npreempt;
	struct node *n;

	act_time_us = tk_clock() * (1000000uL / CLOCKS_PER_SEC);
	for(
		n=mlist_head(tk_list_sleep);
		n;
		n=mlist_next(tk_list_sleep)
	) {
		assert(n->pl);


	//for (i = 0; i < TK_MAX_THREADS; i++) {
		TK_CLI();
		_npreempt = npreempt;
		TK_STI();

		if (_npreempt > 0) {
			return;
		}

		TK_CLI();
		if (__tk_threadPool[i].valid) {
			if (__tk_threadPool[i].state & SLEEP) {

				if (tk_difftime
				    (act_time_us,
				     __tk_threadPool[i].wakeuptime) >= 0) {

					__tk_threadPool[i].state =
					    (PROCSTATE) (__tk_threadPool
							 [i].state & ~_____QS_);
					__tk_threadPool[i].wakeupEvent =
					    E_TIMER;
				}
			}

			if ((__tk_threadPool[i].state & ZOMBI)
			    && (i != __tk_active_thread)) {

				if (_tk_try_detach_parent(i, TK_FALSE)) {

					tk_delete_thread(i);
				}

			}
		}
		TK_STI();
	}
}

void tk_yield(void)
{
	assert(__tk_IntFlagCntr == 0);

	TK_CLI();
	PUSHALL();
	TK_STI();

	_tk_wakeup_timedout_threads();

	TK_CLI();

	if (npreempt > 0) {
		assert(npreempt < 2);
		npreempt--;
		__tk_thread_to_run = preemptlist[outpreempt];
		outpreempt++;
		outpreempt = outpreempt % TK_MAX_THREADS;
	}

	__tk_thread_to_run = _tk_next_runable_thread();
	_tk_context_switch_to_thread(__tk_thread_to_run, __tk_active_thread);

	if (nThreads_ended || (nThreads_ended_skip_cntr>100)) {
		nThreads_ended=0;
		nThreads_ended_skip_cntr=0;
		_tk_finalize_dtors();
	} else
		nThreads_ended_skip_cntr++;
	POPALL();
	TK_STI();

}

void tk_yield_event(void)
{
	TK_CLI();
	PUSHALL();
	__tk_thread_to_run = _tk_next_runable_thread();
	_tk_context_switch_to_thread(__tk_thread_to_run, __tk_active_thread);
	POPALL();
	TK_STI();
}
