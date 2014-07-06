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
@ingroup PTHREAD_SCHED

@brief POSIX 1003.1c API - Scheduling and thread management

The content of this source-file implement thread management. I.e.
starting/stopping, setting attributes of the threads themselves.

For in-depth discussions about this component, see \ref
PTHREAD_SCHED

@see PTHREAD_SYNC
@see PTHREAD

*/
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

//------1---------2---------3---------4---------5---------6---------7---------8
unsigned long tk_pthread_sched_destruct(void)
{
	unsigned long rc = ERR_OK;

	rc = sm_delete(sem_once);

	return rc;

}

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
\returns 0 or a error code

Works exactlly as \ref pthread_create except that it provides TinKer with a name of the thread as well

@note This is not portable. Use the \ref pthread_create instead

@see http://www.opengroup.org/onlinepubs/009695399/functions/pthread_create.html

@todo Check if the stack concepts fit. Uncertain...
*/
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
		//NOTICE! Uncertain about this line.
		(*attr)->stackaddr = tk_tcb->stack_begin;

	} else {
		thid = tk_create_thread(threadName,
					TK_MAX_PRIO_LEVELS / 2,
					start_routine,
					arg, TK_NORMAL_STACK_SIZE);
	};
	//Types are the same, but avoid warnings
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

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Returns the id of the current thread

Do not compare thread id's (i.e. \ref pthread_t) directly to determine
if they are equal. On TinKer you can do that because it's a value based
type, but on other system pthread_t might be a reference. Use \ref
pthread_equal for this purpose to assure code is portable.

@warning Comparing without using \ref pthread_equal since it will not
render in a compilation error, either in situations where TinKer is the
kernel or in reference based \ref pthread_t kernel.

@see http://www.opengroup.org/onlinepubs/009695399/functions/pthread_self.html
@see http://developer.apple.com/documentation/Darwin/Reference/ManPages/man3/pthread_self.3.html#//apple_ref/doc/man/3/pthread_self
@see http://developer.apple.com/documentation/Darwin/Reference/ManPages/man3/pthread_equal.3.html#//apple_ref/doc/man/3/pthread_equal
*/
pthread_t pthread_self(void)
{
	return _tk_current_tcb();
}

/*!
@brief Compare thread id's with each other.

Compare thread id's with each other.

- API should do this
- You shouldn't

@see pthread_self
*/
int pthread_equal(pthread_t t1, pthread_t t2)
{
	return (t1->Thid == t2->Thid);
}

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
@see http://www.opengroup.org/onlinepubs/009695399/functions/pthread_once.html
*/
int pthread_once(pthread_once_t * once_control, void (*init_routine) (void)
    )
{
	int need2run = 0;
	unsigned long rc = ERR_OK;

	if (once_control == NULL) {
		return EINVAL;
	} else {
		//Proper way
		//--- Detect ---
		rc = sm_p(sem_once, WAIT, /*TK_FOREVER */ 1000);
		assert(rc == ERR_OK);
		if (once_control->started < 0) {
			need2run = 1;
			once_control->started++;
		}
		rc = sm_v(sem_once);
		assert(rc == ERR_OK);

		//--- Run if requred ---
		if (need2run) {

			init_routine();

			//--- Mark as finished ---
			rc = sm_p(sem_once, WAIT, /*TK_FOREVER */ 1000);
			assert(rc == ERR_OK);

			once_control->done = 1;
			rc = sm_v(sem_once);
			assert(rc == ERR_OK);
		}
	}

	return 0;
}

/*!
@todo: Very stubbed. Make this more complient!
*/
int pthread_cancel(pthread_t thread)
{
	assert(tk_delete_thread(thread->Thid) == TK_OK);
	return 0;
}

/*!
@todo: Very stubbed. Make this more complient!
*/
int pthread_yield(void)
{
	tk_yield();
	return 0;
}

/*!
@todo: tinker tcb needs a policy field. policy not handled.
*/
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

/** @defgroup PTHREAD_SCHED PTHREAD_SCHED: POSIX 1003.1c API - scheduling
@ingroup PTHREAD
@brief POSIX 1003.1c API - Scheduling and thread management

<em>*Documentation and implementation in progress*</em>

@todo The "once lock" is brutal. It will potentially lock on all accesses, even by threads involved with anuther once_control. Either replace with a rw_lock (TBD) or an array of once locks.

<p><b>Go gack to</b> \ref COMPONENTS</p>
*/
