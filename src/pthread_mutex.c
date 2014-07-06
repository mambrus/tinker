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

int pthread_mutex_init(pthread_mutex_t * mutex,
		       const pthread_mutexattr_t * attr)
{
	assert(mutex->valid);
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(mutex);
	_PTHREAD_NO_WARN_VAR(attr);
	return 0;
}

int pthread_mutex_destroy(pthread_mutex_t * mutex)
{
	assert(mutex->valid);
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(mutex);
	return 0;
}

int pthread_mutex_trylock(pthread_mutex_t * mutex)
{
	assert(mutex->valid);
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(mutex);
	return 0;
}

int pthread_mutex_lock(pthread_mutex_t * mutex)
{

	assert(mutex->valid);
	if (_mutex_lock_primitive(mutex))
		pthread_yield();

	return 0;
}

int pthread_mutex_unlock(pthread_mutex_t * mutex)
{

	assert(mutex->valid);
	if (_mutex_unlock_primitive(mutex, BSINGLE))
		pthread_yield();

	return 0;
}

int pthread_mutex_timedlock(pthread_mutex_t * mutex,
			    const struct timespec *abs_timeout)
{
	assert(mutex->valid);
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(mutex);
	_PTHREAD_NO_WARN_VAR(abs_timeout);
	return 0;
}
