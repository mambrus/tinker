/***************************************************************************
 *   Copyright (C) 1998, Michael Ambrus                                    *
 *   Copyright (C) 2005,2006, Michael Ambrus                               *
 *   michael.ambrus@maquet.com                                             *
 *   Copyright (C) 2014 by Michael Ambrus                                  *
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

#include <tk.h>
#include "context.h"
#include "tk_tuning.h"
#include "implement_tk.h"

#ifndef printk
#error "Tinker needs a printk to be implemented for this target"
#endif
#if defined(TK_COMP_KMEM) && (TK_COMP_KMEM==1)
#include <tk_mem.h>
#endif

#if defined(TK_COMP_ITC) && (TK_COMP_ITC==1)
#include <tk_itc.h>
#include <tk_sysqueues.h>
#endif

#if defined(TK_COMP_PTIME) && (TK_COMP_PTIMER==1)
#ifndef TK_COMP_PTHREAD
#error Component PTIMER is dependant of PTHREAD
#endif
#ifndef TK_COMP_POSIX_RT
#error Component PTIMER is dependant of POSIX_RT
#endif
#include <tk_ptimer.h>
#endif

#if defined(TK_COMP_PTHREAD) && (TK_COMP_PTHREAD==1)
#ifndef TK_COMP_ITC
#error Component POSIX_RT is dependant of ITC
#endif
#include <pthread.h>
#endif
#undef main

#if defined(TK_COMP_POSIX_RT) && (TK_COMP_POSIX_RT==1)
#ifndef TK_COMP_PTHREAD
#error Component POSIX_RT is dependant of PTHREAD
#endif
#ifndef TK_COMP_ITC
#error Component POSIX_RT is dependant of ITC
#endif
#include <semaphore.h>
#include <mqueue.h>
#endif

#if defined (TK_SYSTEM) && (TK_SYSTEM == __SYS_HIXS__)
#include <tinker/hixs.h>
#endif

#if defined(TK_COMP_FILESYS) && (TK_COMP_FILESYS==1)
#if (!(TK_SYSTEM == __SYS_HIXS__))
#error Component TK_COMP_FILESYS is only available for HIXS adapted GNU targets
#endif
#include <filesys/filesys.h>
#endif

#ifndef MARKUP_STACKS_ON_START
#if TK_HOSTED
#define MARKUP_STACKS_ON_START __tk_no
#else
#define MARKUP_STACKS_ON_START __tk_yes
#endif
#endif

#define LDATA struct tcb_t_
//struct tcb_t_ __tk_threadPool[TK_MAX_THREADS] = { {0} };
#include <mlist/mlist.h>

void *_tk_destructor(void *retval);
void *_tk_idle(void *foo);
void _tk_detach_children(thid_t thid);

void _tk_half_switch(thid_t, thid_t);
void _tk_constructor(start_func_f, void *, thid_t);
void _tk_construct_thread_scope(start_func_f, void *, thid_t);

int __tk_IntFlagCntr;
struct tcb_t_ __tk_threadPool[TK_MAX_THREADS] = { {0} };

thid_t __tk_schedule[TK_MAX_PRIO_LEVELS][TK_MAX_THREADS_AT_PRIO];
struct stat_t __tk_roundrobin_prio_idxs[TK_MAX_PRIO_LEVELS];

thid_t __tk_active_thread;

#define active_thread __tk_active_thread
thid_t __tk_thread_to_run = 0;
thid_t __tk_procs_in_use = 0;
thid_t __tk_proc_idx;

static thid_t idle_Thid;
init_func_f boot_hook;

void _b_hook(void *caller)
{
	if (boot_hook != 0)
		boot_hook(caller);
}

void *_tk_idle(void *foo)
{
	TK_NO_WARN_ARG(foo);
	while (TK_TRUE) {
		tk_yield();
	}
}

void tk_delete_kernel(void)
{
	tk_delete_thread(idle_Thid);
}

thid_t tk_thread_id(void)
{
	return (__tk_active_thread);
}

tk_tcb_t *_tk_current_tcb(void)
{
	return (&__tk_threadPool[__tk_active_thread]);
}

tk_tcb_t *_tk_specific_tcb(thid_t id)
{
	return (&__tk_threadPool[id]);
}

int *_tk_errno()
{
	return &(__tk_threadPool[__tk_active_thread]._errno_);
}

static const char testPatt[] = "TinKer is Real-Time!";
void tk_create_kernel(void)
{
	int i, j;
	char *testArea;
	__mlist_init();
	assert(mlist_opencreate(sizeof(struct tcb_t_*), NULL, &tk_list_sleep)==0);
	assert(mlist_opencreate(sizeof(struct tcb_t_*), NULL, &tk_list_die)==0);

	TK_NO_WARN_ARG(testArea);

	for (i = 0; i < TK_MAX_THREADS; i++) {
		__tk_threadPool[i].state = ZOMBI;
		__tk_threadPool[i].bOnId.kind = BON_SCHED;
		__tk_threadPool[i].valid = TK_FALSE;
		__tk_threadPool[i].wakeupEvent = E_NONE;
	}

	__tk_threadPool[0].state = READY;
	__tk_threadPool[0].valid = TK_TRUE;
	strcpy(__tk_threadPool[0].name, "tk_root");
	__tk_procs_in_use = 1;
	__tk_proc_idx = 0;

	for (i = 0; i < TK_MAX_PRIO_LEVELS; i++) {

		__tk_roundrobin_prio_idxs[i].procs_at_prio = 0;
		__tk_roundrobin_prio_idxs[i].curr_idx = 0;
		for (j = 0; j < TK_MAX_THREADS_AT_PRIO; j++) {

			__tk_schedule[i][j] = 0;
		}
	}

	idle_Thid =
	    tk_create_thread("idle", TK_MAX_PRIO_LEVELS - 1, _tk_idle,
			     (void *)NULL, TK_MINIMUM_STACK_SIZE);

	__tk_threadPool[__tk_threadPool[idle_Thid].Gid].noChilds--;

	__tk_threadPool[idle_Thid].Gid = __tk_threadPool[idle_Thid].Thid;

	__tk_roundrobin_prio_idxs[0].procs_at_prio = 1;
	__tk_schedule[0][0] = 0;

}

void *_tk_destructor(void *retval)
{

#ifdef DEBUG
	printk(("Dieing thread [id=%u] is returning! Return value is: 0x%lX\n\n",
		(unsigned int)__tk_active_thread,
		(unsigned long)retval));
#endif

	__tk_threadPool[__tk_active_thread].state = ZOMBI;
	__tk_threadPool[__tk_active_thread].retval = (void *)retval;
	_tk_detach_children(__tk_active_thread);
	mlist_add(tk_list_die, &__tk_threadPool[__tk_active_thread]);
	nThreads_ended++;

	while (TK_TRUE) {
		tk_yield();
#ifdef DEBUG
		printk(("Error: Dead thread [id=%u] still waiting for Nirvana!\n",
			(unsigned int)__tk_active_thread));
#endif
		assert("Code should never reach this point" == NULL);
	}

}

void _tk_finalize_dtors(void){
	struct node *n;

	for(
		n=mlist_head(tk_list_die);
		n;
		n=mlist_next(tk_list_die)
	) {
		assert(n->pl);
		assert(n->pl->state == ZOMBI);
		assert(n->pl->Thid != __tk_active_thread);

		if (_tk_try_detach_parent(n->pl->Thid, TK_FALSE)) {
			tk_delete_thread(n->pl->Thid);
			mlist_del_curr(tk_list_die);
		}

	}
}

int tk_delete_thread(thid_t Thid)
{
	unsigned int Prio, Idx, i;
	int bury_it = TK_TRUE;

	if (__tk_threadPool[Thid].valid == TK_FALSE) {

		return (TK_ERROR);
	}

	if (!(__tk_threadPool[Thid].state & ZOMBI)) {

		__tk_threadPool[Thid].state = ZOMBI;
		_tk_detach_children(Thid);
		bury_it = _tk_try_detach_parent(Thid, TK_FALSE);
	}

	if (bury_it) {
		Prio = __tk_threadPool[Thid].Prio;
		Idx = __tk_threadPool[Thid].Idx;
		assert(__tk_schedule[Prio][Idx] == Thid);
		__tk_procs_in_use--;

		for (i = Idx; i < __tk_roundrobin_prio_idxs[Prio].procs_at_prio;
		     i++) {
			__tk_schedule[Prio][i] = __tk_schedule[Prio][i + 1];
			__tk_threadPool[__tk_schedule[Prio][i]].Idx = i;
		}

		__tk_roundrobin_prio_idxs[Prio].procs_at_prio--;
		if (__tk_roundrobin_prio_idxs[Prio].curr_idx >=
		    __tk_roundrobin_prio_idxs[Prio].procs_at_prio) {
			__tk_roundrobin_prio_idxs[Prio].curr_idx = 0;
		}

		stalloc_free(__tk_threadPool[Thid].stack.bos);

		__tk_threadPool[Thid].valid = TK_FALSE;
	}

	return (TK_OK);
}

int tk_detach(thid_t Thid)
{
	if (__tk_threadPool[Thid].valid != TK_TRUE)
		return ESRCH;

	if (__tk_threadPool[Thid].Gid != -1)
		return EINVAL;

	_tk_try_detach_parent(Thid, TK_TRUE);
	return 0;
}

thid_t tk_create_thread(char *name,
			unsigned int prio,
			start_func_f f, void *inpar, size_t stack_size)
{

	thid_t slot_idx = __tk_roundrobin_prio_idxs[prio].procs_at_prio;
	unsigned int error = 0;
	thid_t valid_proc_idx;

	if (__tk_procs_in_use >= TK_MAX_THREADS) {
		printk(("tk: Error - Total amount of threads would exceed limit\n"));
		error |= TC_MAX_THREADS;
	}

	if (prio >= TK_MAX_PRIO_LEVELS) {
		printk(("tk: Error - Chosen priority too high\n"));
		error |= TC_MAX_PRIO_LEVELS;
	}

	if (TK_MAX_THREADS_AT_PRIO <=
	    (__tk_roundrobin_prio_idxs[prio].procs_at_prio)) {
		printk(("tk: Error - To many threads at this prio\n"));
		error |= TC_THREADS_AT_PRIO;
	}
	if (error) {
		tk_exit(error);
	}

	do {
		__tk_proc_idx++;
		__tk_proc_idx %= TK_MAX_THREADS;
	} while (__tk_threadPool[__tk_proc_idx].valid == TK_TRUE);

#if TK_THREAD_NAME_LEN
	if (strlen(name) < TK_THREAD_NAME_LEN)
		strncpy(__tk_threadPool[__tk_proc_idx].name, name,
			TK_THREAD_NAME_LEN);
	else {
		printk(("tk: Error - Thread-name to long\n"));
		tk_exit(TC_NAME_LEN);
	}
#endif
	__tk_threadPool[__tk_proc_idx].name[TK_THREAD_NAME_LEN] = 0;

	if ((__tk_threadPool[__tk_proc_idx].stack.bos =
	     (char *)stalloc(stack_size)) == NULL) {
		printk(("tk: Error - Can't create thread (can't allocate memory for stack)\n"));
		tk_exit(TC_NOMEM);
	}

	__tk_threadPool[__tk_proc_idx].valid = TK_TRUE;
	__tk_threadPool[__tk_proc_idx].state = READY;
	__tk_threadPool[__tk_proc_idx].bOnId.kind = BON_SCHED;
	__tk_threadPool[__tk_proc_idx].bOnId.entity.tcb = NULL;
	__tk_threadPool[__tk_proc_idx].Thid = __tk_proc_idx;
	__tk_threadPool[__tk_proc_idx].Gid = __tk_active_thread;
	__tk_threadPool[__tk_proc_idx].noChilds = 0;
	__tk_threadPool[__tk_proc_idx]._errno_ = 0;
	__tk_threadPool[__tk_proc_idx].Prio = prio;
	__tk_threadPool[__tk_proc_idx].Idx = slot_idx;

	__tk_threadPool[__tk_proc_idx].wakeupEvent = E_NONE;
	__tk_threadPool[__tk_proc_idx].retval = inpar;

	__tk_threadPool[__tk_active_thread].noChilds++;

	__tk_threadPool[__tk_proc_idx].stack.stack_size = stack_size;
#if STACK_SMASH
	__tk_threadPool[__tk_proc_idx].stack.stack_crc = 0;
#endif				//STACK_SMASH

#if MARKUP_STACKS_ON_START
#if defined (HAVE_MEMSET)
#include <string.h>
	memset(__tk_threadPool[__tk_proc_idx].stack.bos,
	       (unsigned char)__tk_proc_idx, stack_size - 8);
#else
	for (i = 0; i < stack_size; i++)
		__tk_threadPool[__tk_proc_idx].stack.bos[i] =
		    (unsigned char)(__tk_proc_idx % sizeof(unsigned char));
#endif
#endif

	__tk_procs_in_use++;
	valid_proc_idx = __tk_proc_idx;
	_tk_construct_thread_scope(f, inpar, __tk_proc_idx);

	slot_idx = __tk_roundrobin_prio_idxs[prio].procs_at_prio;
	__tk_threadPool[__tk_proc_idx].Idx = slot_idx;

	__tk_schedule[prio][slot_idx] = __tk_proc_idx;

	__tk_roundrobin_prio_idxs[prio].procs_at_prio++;

	return valid_proc_idx;
}

int tk_join(thid_t PID, void **value_ptr)
{
	if (__tk_threadPool[PID].Gid != __tk_active_thread)
		return EINVAL;

	if (__tk_threadPool[PID].valid != TK_TRUE)
		return ESRCH;

	if (__tk_threadPool[PID].state & ZOMBI) {
		if (value_ptr != NULL) {
			*value_ptr = __tk_threadPool[PID].retval;
		}

		_tk_try_detach_parent(PID, TK_TRUE);

	} else {

		__tk_threadPool[__tk_active_thread].bOnId.kind = BON_SCHED;
		__tk_threadPool[__tk_active_thread].bOnId.entity.tcb =
		    &__tk_threadPool[PID];
		__tk_threadPool[__tk_active_thread].state =
		    (PROCSTATE) (__tk_threadPool[__tk_active_thread].state |
				 TERM);
		tk_yield();
		assert(__tk_threadPool[__tk_active_thread].wakeupEvent ==
		       E_CHILDDEATH);
		if (value_ptr != NULL) {

			*value_ptr = __tk_threadPool[__tk_active_thread].retval;
		}
	}

	return ERR_OK;
}

void tk_msleep(unsigned int time_ms)
{

	clock_t act_time_us;
	clock_t wkp_time_us;
	unsigned long in_us;

	in_us = time_ms * 1000uL;
	act_time_us = tk_clock() * (1000000uL / CLOCKS_PER_SEC);
	wkp_time_us = act_time_us + in_us;

	__tk_threadPool[__tk_active_thread].wakeuptime = wkp_time_us;
	__tk_threadPool[__tk_active_thread].state =
	    (PROCSTATE) (__tk_threadPool[__tk_active_thread].state | SLEEP);

	tk_yield();
}

int tk_change_prio(thid_t tid, int newPrio)
{
	int y;
	int oldPrio;
	thid_t oldIdx, newIdx;
	int error = 0;
	int i;

	if (__tk_threadPool[tid].valid != TK_TRUE) {
		printk(("tk: Error - Invalid TCB detected\n"));
		error |= TC_TCB_INVALID;
	}

	if (newPrio >= TK_MAX_PRIO_LEVELS) {
		printk(("tk: Error - Chosen priority too high\n"));
		error |= TC_MAX_PRIO_LEVELS;
	}

	if (TK_MAX_THREADS_AT_PRIO <=
	    (__tk_roundrobin_prio_idxs[newPrio].procs_at_prio)) {
		printk(("tk: Error - To many threads at this prio\n"));
		error |= TC_THREADS_AT_PRIO;
	}
	if (error) {
		tk_exit(error);
	}

	oldPrio = __tk_threadPool[tid].Prio;
	oldIdx = __tk_threadPool[tid].Idx;

	if (oldPrio == newPrio)
		return 0;

	if (__tk_threadPool[tid].state != READY)
		y = 0;
	else {
		if (newPrio > oldPrio)
			y = 1;
		else if (__tk_active_thread == tid)
			y = 1;
		else
			y = 0;
	}

	for (i = oldIdx; i < __tk_roundrobin_prio_idxs[oldPrio].procs_at_prio;
	     i++) {
		__tk_schedule[oldPrio][i] = __tk_schedule[oldPrio][i + 1];
		__tk_threadPool[__tk_schedule[oldPrio][i]].Idx = i;
	}
	__tk_roundrobin_prio_idxs[oldPrio].procs_at_prio--;
	if (__tk_roundrobin_prio_idxs[oldPrio].curr_idx >=
	    __tk_roundrobin_prio_idxs[oldPrio].procs_at_prio) {
		__tk_roundrobin_prio_idxs[oldPrio].curr_idx = 0;
	}

	newIdx = __tk_roundrobin_prio_idxs[newPrio].procs_at_prio;

	__tk_schedule[newPrio][newIdx] = tid;
	__tk_roundrobin_prio_idxs[newPrio].procs_at_prio++;

	__tk_threadPool[tid].Prio = newPrio;
	__tk_threadPool[tid].Idx = newIdx;

	return y;

}

void _tk_detach_children(thid_t thid)
{
	thid_t j;

	if (__tk_threadPool[thid].noChilds > 0) {
		for (j = 0; (__tk_threadPool[thid].noChilds > 0)
		     && (j < TK_MAX_THREADS); j++) {
			if (__tk_threadPool[j].valid
			    && __tk_threadPool[j].Gid == thid) {

				__tk_threadPool[j].Gid = -1;
				__tk_threadPool[thid].noChilds--;
			}
		}
	}
}

int _tk_try_detach_parent(thid_t thid, int force)
{
	int succeded_unblocking_parent = TK_FALSE;

	if (__tk_threadPool[thid].Gid == -1)
		return TK_TRUE;

	assert(__tk_threadPool[__tk_threadPool[thid].Gid].valid);

	if ((__tk_threadPool[__tk_threadPool[thid].Gid].state & TERM) &&
	    __tk_threadPool[__tk_threadPool[thid].Gid].bOnId.entity.tcb ==
	    &__tk_threadPool[thid]) {
		__tk_threadPool[__tk_threadPool[thid].Gid].state =
		    (PROCSTATE) (__tk_threadPool
				 [__tk_threadPool[thid].Gid].state & ~_______T);
		__tk_threadPool[__tk_threadPool[thid].Gid].wakeupEvent =
		    E_CHILDDEATH;

		if (__tk_threadPool[__tk_threadPool[thid].Gid].state == READY) {
			__tk_threadPool[__tk_threadPool[thid].Gid].bOnId.
			    entity.tcb = NULL;
			__tk_threadPool[__tk_threadPool[thid].Gid].bOnId.kind =
			    BON_SCHED;
		} else {
			__tk_threadPool[__tk_threadPool[thid].Gid].bOnId.
			    entity.tcb = &__tk_threadPool[thid];
			__tk_threadPool[__tk_threadPool[thid].Gid].bOnId.kind =
			    BON_SCHED;
		}

		succeded_unblocking_parent = TK_TRUE;

		__tk_threadPool[__tk_threadPool[thid].Gid].retval =
		    __tk_threadPool[thid].retval;

	} else
		succeded_unblocking_parent = TK_FALSE;

	if ((succeded_unblocking_parent == TK_TRUE) || (force == TK_TRUE)) {
		__tk_threadPool[__tk_threadPool[thid].Gid].noChilds--;
		assert(__tk_threadPool[__tk_threadPool[thid].Gid].noChilds >=
		       0);
		__tk_threadPool[thid].Gid = -1;
		return TK_TRUE;
	}

	return TK_FALSE;
}

thid_t _tk_next_runable_thread()
{
	int idx, prio, nbTry, loop, p_at_p;
	thid_t return_Thid;
	TK_BOOL found;
#ifndef NDEBUG
	thid_t last_thid = (thid_t) - 1;
#endif

	return_Thid = idle_Thid;
	found = TK_FALSE;

	for (prio = 0; prio < TK_MAX_PRIO_LEVELS && !found; prio++) {
		p_at_p = __tk_roundrobin_prio_idxs[prio].procs_at_prio;

		if (p_at_p != 0) {

			idx = (__tk_roundrobin_prio_idxs[prio].curr_idx);

			nbTry = __tk_roundrobin_prio_idxs[prio].procs_at_prio;
			for (loop = 0; loop < nbTry && !found; loop++) {

#ifndef NDEBUG
				assert(__tk_threadPool
				       [__tk_schedule[prio][idx]].valid);
				assert(__tk_threadPool
				       [__tk_schedule[prio][idx]].Thid !=
				       last_thid);
				last_thid = __tk_threadPool[__tk_schedule[prio]
							    [idx]].Thid;
#endif
				if (__tk_threadPool
				    [__tk_schedule[prio][idx]].state == READY) {
					return_Thid = __tk_schedule[prio][idx];
					found = TK_TRUE;
				}

				idx++;
				idx %= p_at_p;
			}
		}
	}
	return return_Thid;
}

#if JUMPER_BASED
#define _SELF 0
void _tk_context_switch_to_thread(thid_t RID, thid_t SID)
{
	unsigned int pid;
#if STACK_DEBUG
	__tk_threadPool[SID].stack.free =
	    (void *)__tk_threadPool[SID].stack.stack_size -
	    __tk_threadPool[SID].stack.sp;

	__tk_threadPool[SID].stack.inuse =
	    __tk_threadPool[SID].stack.stack_size -
	    __tk_threadPool[SID].stack.free;
#endif				//STACK_DEBUG

	PUSH_CPU_GETCUR_STACK(__tk_threadPool[SID].stack, pid);

	if (pid == _SELF) {
		__tk_active_thread = RID;
		CHANGE_STACK_POP_CPU(__tk_threadPool[RID].stack, pid);
	}

}

void _tk_half_switch(thid_t RID, thid_t SID)
{
	unsigned int pid;
#if STACK_DEBUG
	__tk_threadPool[SID].stack.free =
	    (void *)__tk_threadPool[SID].stack.stack_size -
	    __tk_threadPool[SID].stack.sp;

	__tk_threadPool[SID].stack.inuse =
	    __tk_threadPool[SID].stack.stack_size -
	    __tk_threadPool[SID].stack.free;
#endif				//STACK_DEBUG

	PUSH_CPU_GETCUR_STACK(__tk_threadPool[SID].stack, pid);

	if (pid == _SELF) {
		__tk_active_thread = RID;
		CHANGE_STACK_POP_CPU(__tk_threadPool[SID].stack, pid);
	}

}

#undef _SELF
#else
void _tk_context_switch_to_thread(thid_t RID, thid_t SID)
{
	static char *tmp_stackp;
	static unsigned int cswTEMP;

	TK_NO_WARN_ARG(cswTEMP);

	PUSH_CPU_GETCUR_STACK(tmp_stackp, cswTEMP);

	__tk_threadPool[SID].stack.sp = tmp_stackp;

	tmp_stackp = STACK_PTR(__tk_threadPool[RID].stack.sp);

	__tk_active_thread = RID;
	CHANGE_STACK_POP_CPU(tmp_stackp, cswTEMP);

}

void _tk_half_switch(thid_t RID, thid_t SID)
{
	static char *tmp_stackp;
	static unsigned int cswTEMP;

	TK_NO_WARN_ARG(cswTEMP);

	PUSH_CPU_GETCUR_STACK(tmp_stackp, cswTEMP);

	__tk_threadPool[SID].stack.sp = tmp_stackp;
	tmp_stackp = STACK_PTR(__tk_threadPool[SID].stack.sp);

	__tk_active_thread = RID;
	CHANGE_STACK_POP_CPU(tmp_stackp, cswTEMP);

}
#endif
void _tk_constructor(start_func_f f, void *inpar, thid_t id)
{
	void *retval;
	tk_yield();

	retval = f(inpar);

	_tk_destructor(retval);

}

void _tk_construct_thread_scope(start_func_f f, void *inpar, thid_t id)
{
	TK_CLI();

	static start_func_f t_f;
	static void *t_inpar;
	static thid_t t_id;

	static char *aSP;
	static unsigned int TEMP;

	TK_NO_WARN_ARG(TEMP);

	t_f = f;
	t_inpar = inpar;
	t_id = id;

	__tk_threadPool[t_id].stack.sp =
	    __tk_threadPool[t_id].stack.bos +
	    __tk_threadPool[t_id].stack.stack_size - EXTRA_MARGIN;

	PUSHALL();

	_tk_half_switch(t_id, __tk_active_thread);

	if (__tk_active_thread != t_id) {
		POPALL();
		return;
	}

	aSP = __tk_threadPool[t_id].stack.sp;
	CHANGE_STACK(aSP, TEMP);

	TK_STI();
	_tk_constructor(t_f, t_inpar, t_id);

}

#if defined(TK_COMP_FILESYS) && TK_COMP_FILESYS

#include <fcntl.h>

int set_fflags(int desc, int flags)
{
	int oldflags = fcntl(desc, F_GETFL, 0);

	if (oldflags == -1)
		return -1;

	oldflags |= O_NONBLOCK;

	return fcntl(desc, F_SETFL, oldflags);
}

#endif

#ifdef BOOT_BSP_STUB

int _tk_bsp_sysinit(void)
{
	return 0;
}
#else
#define _tk_bsp_sysinit tk_bsp_sysinit
#endif

#define _MSTR(x) \
    #x

#define MSTR(x) \
    _MSTR(x)

void _tk_main(void)
{
	_b_hook(_tk_main);
#if defined (TK_SYSTEM) && (TK_SYSTEM == __SYS_HIXS__)
	_syscall_mon(_tk_main);
	extern struct hixs_t hixs;
	hixs.exit = tk_exit;
#endif
	_tk_bsp_sysinit();
	_b_hook(_tk_bsp_sysinit);

	printk(("BSP initialized\n"));

#if defined(__GNUC__)
	printk(("TinKer running target : %s\n", MSTR(TK_ALIAS_HOST)));
	printk(("GCC version           : %s\n", MSTR(TK_GCC_VERSION)));
	printk(("Main architecture     : %s\n", MSTR(__TK_ARCH_up__)));
	printk(("CPU variant           : %s\n", MSTR(__TK_DCPU_up__)));
	printk(("Board                 : %s\n", MSTR(__TK_BOARD_up__)));
	printk(("System                : %s\n", MSTR(__TK_SYSTEM_up__)));
	printk(("Scheduling method     : %s\n", MSTR(TK_DISPATCH)));
#endif

	_b_hook(tk_create_kernel);
	tk_create_kernel();
	printk(("TinKer kernel created\n"));

	printk(("ANSI timing constants:\n"));
#if defined( __C166__ )
	printk(("CLK_TCK=[%ld], CLOCKS_PER_SEC=[%ld]\n", CLK_TCK,
		CLOCKS_PER_SEC));
#else
	printk(("CLK_TCK=[%d], CLOCKS_PER_SEC=[%d]\n", CLK_TCK,
		CLOCKS_PER_SEC));
#endif
#if defined(TK_COMP_KMEM) && TK_COMP_KMEM
	_b_hook(tk_mem);
	assert(tk_mem() == ERR_OK);
#endif

#if defined(TK_COMP_ITC) && TK_COMP_ITC
	_b_hook(tk_itc);
	assert(tk_itc() == ERR_OK);

	_b_hook(_tk_create_system_queues);
	assert(_tk_create_system_queues() == 0);
#if defined(TK_COMP_PTIMER) && TK_COMP_PTIMER
	_b_hook(tk_ptime);
	assert(tk_ptime() == ERR_OK);
#endif
#if defined(TK_COMP_PTHREAD) && TK_COMP_PTHREAD
	_b_hook(tk_pthread_sched);
	assert(tk_pthread_sched() == ERR_OK);
	_b_hook(tk_pthread_sync);
	assert(tk_pthread_sync() == ERR_OK);
#if defined(TK_COMP_POSIX_RT) && TK_COMP_POSIX_RT

#endif
#endif
#endif

#if defined(TK_COMP_FILESYS) && TK_COMP_FILESYS
	_b_hook(fs_init);
	printk(("File-system starting...\n"));
	assert(fs_init() == ERR_OK);

	fprintf(stderr, "File-system initialized! (1)\n");
	printk(("File-system initialized! (2)\n"));
#endif

	tk_root();

#if defined(TK_COMP_FILESYS) && TK_COMP_FILESYS
	_b_hook(fs_fini);
	printk(("File-system shutting down...\n"));
	assert(fs_fini() == ERR_OK);
	fprintf(stderr, "File-system terminated!\n");
#endif

#if defined(TK_COMP_ITC) && TK_COMP_ITC
#if defined(TK_COMP_PTHREAD) && TK_COMP_PTHREAD
#if defined(TK_COMP_POSIX_RT) && TK_COMP_POSIX_RT

#endif
	_b_hook(tk_pthread_sched_destruct);
	assert(tk_pthread_sched_destruct() == ERR_OK);
	_b_hook(tk_pthread_sync_destruct);
	assert(tk_pthread_sync_destruct() == ERR_OK);
#endif
	_b_hook(tk_itc_destruct);
	assert(tk_itc_destruct() == ERR_OK);
#if defined(TK_COMP_PTIMER) && TK_COMP_PTIMER
	_b_hook(tk_ptime_destruct);
	assert(tk_ptime_destruct() == ERR_OK);
#endif
#endif

#if defined(TK_COMP_KMEM) && TK_COMP_KMEM
	_b_hook(tk_mem_destruct);
	assert(tk_mem_destruct() == ERR_OK);
#endif
	_b_hook(tk_delete_kernel);
	tk_delete_kernel();
	printk(("Kernel deleted\n"));
}

#if (defined(_WIN32) && defined(_MSC_VER)) \
 || defined(__BORLANDC__) || defined(__BCPLUSPLUS__) \
 || defined(__CYGWIN32__) || defined(__CYGWIN__) || defined(__GNUC__)
int main(int argc, char **argv);
int main(int argc, char **argv)
{
	_b_hook(main);
	_tk_main();
	_b_hook(NULL);
	TRAP(0);
	return 0;
}
#endif
