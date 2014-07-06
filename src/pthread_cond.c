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

int pthread_cond_init(pthread_cond_t * __cond, const pthread_condattr_t * attr)
{
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(__cond);
	_PTHREAD_NO_WARN_VAR(attr);
	return 0;
}

int pthread_cond_destroy(pthread_cond_t * __cond)
{
	assert(__cond->valid);
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(__cond);
	return 0;
}

int pthread_cond_signal(pthread_cond_t * __cond)
{
	int y;

	assert(__cond->valid);
	if (__cond->coop_mux == NULL)
		return 0;

	y = _mutex_unlock_primitive(&(__cond->cv_mux), BSINGLE);
	if (__cond->cv_mux.owner == NULL) {

		assert(__cond->cv_mux.blocked.numb == 0);

		__cond->cv_mux.owner = (pthread_t) 1;
	}
	if (y)
		pthread_yield();

	pthread_mutex_unlock(__cond->coop_mux);

	return 0;
}

int pthread_cond_broadcast(pthread_cond_t * __cond)
{
	int y1, y2;

	assert(__cond->valid);
	if (__cond->coop_mux == NULL)
		return 0;

	y1 = _mutex_unlock_primitive(&(__cond->cv_mux), BCAST);
	if (__cond->cv_mux.blocked.numb == 0) {

		__cond->cv_mux.owner = (pthread_t) 1;
	}

	y2 = _mutex_unlock_primitive(__cond->coop_mux, BSINGLE);

	if (y1 || y2)
		pthread_yield();

	return 0;
}

int pthread_cond_wait(pthread_cond_t * __cond, pthread_mutex_t * mutex)
{
	int y;

	assert(__cond->valid);

	__cond->coop_mux = mutex;

	_mutex_unlock_primitive(__cond->coop_mux, BSINGLE);

	y = _mutex_lock_primitive(&(__cond->cv_mux));
	assert(y);
	__cond->cv_mux.linkOf = _PBON_CONDVAR;
	__cond->cv_mux.link.cond = __cond;
	pthread_yield();

	pthread_mutex_lock(__cond->coop_mux);
	return 0;
}

int pthread_cond_timedwait(pthread_cond_t * __cond, pthread_mutex_t * mutex,
			   const struct timespec *abstime)
{
	assert(__cond->valid);
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(__cond);
	_PTHREAD_NO_WARN_VAR(mutex);
	_PTHREAD_NO_WARN_VAR(abstime);
	return 0;
}
