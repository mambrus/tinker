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
#include <tk_itc.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>
#include <tk_itc.h>
#include "implement_pthread.h"
#include "implement_tk.h"

unsigned long sem_once;

unsigned long tk_pthread_sched(void)
{
	unsigned long rc = ERR_OK;

	rc = sm_create("SOnc", 1, FIFO, &sem_once);

	return rc;
}

unsigned long tk_pthread_sched_destruct(void)
{
	unsigned long rc = ERR_OK;

	rc = sm_delete(sem_once);

	return rc;

}

int pthread_create_named_np(pthread_t * thread,
			    const pthread_attr_t * attr,
			    void *(*start_routine) (void *),
			    void *arg, char *threadName)
{
	thid_t thid;
	struct tcb_t_ *tk_tcb;

	if (attr != NULL) {
		thid = tk_create_thread(threadName,
					(*attr)->priority,
					start_routine, arg, (*attr)->stacksize);
		tk_tcb = _tk_specific_tcb(thid);

		(*attr)->stackaddr = tk_tcb->stack;

	} else {
		thid = tk_create_thread(threadName,
					TK_MAX_PRIO_LEVELS / 2,
					start_routine,
					arg, TK_NORMAL_STACK_SIZE);
	};

	*thread = (pthread_t) _tk_specific_tcb(thid);

	return 0;
}

int pthread_join(pthread_t thread, void **arg_return)
{
	return tk_join(thread->Thid, arg_return);
}

int pthread_detach(pthread_t thread)
{
	return tk_detach(thread->Thid);
}

pthread_t pthread_self(void)
{
	return _tk_current_tcb();
}

int pthread_equal(pthread_t t1, pthread_t t2)
{
	return (t1->Thid == t2->Thid);
}

int pthread_once(pthread_once_t * once_control, void (*init_routine) (void)
    )
{
	int need2run = 0;
	unsigned long rc = ERR_OK;

	if (once_control == NULL) {
		return EINVAL;
	} else {

		rc = sm_p(sem_once, WAIT, 1000);
		assert(rc == ERR_OK);
		if (once_control->started < 0) {
			need2run = 1;
			once_control->started++;
		}
		rc = sm_v(sem_once);
		assert(rc == ERR_OK);

		if (need2run) {

			init_routine();

			rc = sm_p(sem_once, WAIT, 1000);
			assert(rc == ERR_OK);

			once_control->done = 1;
			rc = sm_v(sem_once);
			assert(rc == ERR_OK);
		}
	}

	return 0;
}

int pthread_cancel(pthread_t thread)
{
	assert(tk_delete_thread(thread->Thid) == TK_OK);
	return 0;
}

int pthread_yield(void)
{
	tk_yield();
	return 0;
}

int pthread_setschedparam(pthread_t thread,
			  int policy, const struct sched_param *param)
{
	int y = 0;
	y = tk_change_prio(thread->Thid, param->sched_priority);
	_PTHREAD_NO_WARN_VAR(policy);

	if (y)
		tk_yield();

	return 0;
}

int pthread_getschedparam(pthread_t thread,
			  int *policy, struct sched_param *param)
{
	param->sched_priority = thread->Prio;
	_PTHREAD_NO_WARN_VAR(policy);
	return 0;
}
