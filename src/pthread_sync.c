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
#include <pthread.h>
#include <errno.h>
#include <assert.h>

#include "implement_pthread.h"
#include "implement_tk.h"

unsigned long tk_pthread_sync(void)
{
	return ERR_OK;
}

unsigned long tk_pthread_sync_destruct(void)
{
	return ERR_OK;
}

int _mutex_lock_primitive(pthread_mutex_t * mutex)
{
	int rc = 0;

	if (mutex->owner == NULL) {
		mutex->owner = pthread_self();
		mutex->blocked.numb = 0;
	} else {
		pthread_t self;
		self = pthread_self();
		self->bOnId.kind = BON_PMUTEX;
		self->bOnId.entity.mutex = mutex;
		self->state = (PROCSTATE) (self->state | QUEUE);

		mutex->linkOf = _PBON_NOLINK;
		mutex->link.dummy = NULL;
		mutex->blocked.thread[mutex->blocked.numb] = self;
		mutex->blocked.numb++;

		rc = 1;
	}
	return rc;
}

int _mutex_unlock_primitive(pthread_mutex_t * mutex, bcast_t bcast)
{
	int rc = 0;
	unsigned prio = TK_MAX_PRIO_LEVELS;
	int i;

	if (mutex->owner == NULL) {

		assert(mutex->blocked.numb == 0);
		return 0;
	}

	if (mutex->blocked.numb == 0) {

		mutex->owner = NULL;
		return 0;
	}

	rc = 1;

	if (bcast) {

		for (i = 0; i < mutex->blocked.numb; i++) {
			mutex->blocked.thread[i]->bOnId.kind = BON_SCHED;
			mutex->blocked.thread[i]->bOnId.entity.tcb = NULL;
			mutex->blocked.thread[i]->state =
			    (PROCSTATE) (mutex->blocked.thread[i]->
					 state & ~QUEUE);
			mutex->blocked.thread[i]->wakeupEvent = E_ITC;
		}
		mutex->blocked.numb = 0;

	} else {
		pthread_t newOwner;
		int j;

		for (i = 0; i < mutex->blocked.numb; i++) {

			if (mutex->blocked.thread[i]->Prio < prio) {
				j = i;
				prio = mutex->blocked.thread[i]->Prio;
			}
		}

		mutex->owner = newOwner = mutex->blocked.thread[j];

		for (i = j; i < mutex->blocked.numb; i++) {
			mutex->blocked.thread[i] = mutex->blocked.thread[i + 1];
		}

		mutex->blocked.numb--;

		newOwner->bOnId.kind = BON_SCHED;
		newOwner->bOnId.entity.tcb = NULL;
		newOwner->state = (PROCSTATE) (newOwner->state & ~QUEUE);
		newOwner->wakeupEvent = E_ITC;
	}

	return rc;
}
