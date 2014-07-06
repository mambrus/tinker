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

int pthread_rwlock_rdlock(pthread_rwlock_t * __rwlock)
{
	int blocked_writers_granted = 0;
	int i;

	assert(__rwlock->valid);
	pthread_mutex_lock(&(__rwlock->mx_self));

	if (__rwlock->blocked_writers) {
		for (i = pthread_self()->Prio; i >= 0; i--) {
			blocked_writers_granted += __rwlock->bwriters_prio[i];
		}
	} else
		blocked_writers_granted = 0;

	while (__rwlock->writers_writing || blocked_writers_granted) {
		if (blocked_writers_granted && !__rwlock->writers_writing
		    && !__rwlock->readers_reading) {

			pthread_cond_signal(&(__rwlock->writers_lock));
		}

		__rwlock->blocked_readers++;
		pthread_cond_wait(&(__rwlock->readers_lock),
				  &(__rwlock->mx_self));
		__rwlock->blocked_readers--;

		if (__rwlock->blocked_writers) {
			for (i = pthread_self()->Prio; i < _PTHREAD_PRIOS; i++) {
				blocked_writers_granted +=
				    __rwlock->bwriters_prio[i];
			}
		} else
			blocked_writers_granted = 0;
	}
	__rwlock->readers_reading++;
	pthread_mutex_unlock(&(__rwlock->mx_self));
	return 0;
}

int pthread_rwlock_wrlock(pthread_rwlock_t * __rwlock)
{
	assert(__rwlock->valid);
	pthread_mutex_lock(&(__rwlock->mx_self));
	while (__rwlock->readers_reading || __rwlock->writers_writing) {

		__rwlock->blocked_writers++;
		__rwlock->bwriters_prio[pthread_self()->Prio]++;

		pthread_cond_wait(&(__rwlock->writers_lock),
				  &(__rwlock->mx_self));

		__rwlock->blocked_writers--;
		__rwlock->bwriters_prio[pthread_self()->Prio]--;
	}
	__rwlock->writers_writing++;
	assert(__rwlock->writers_writing < 2);
	pthread_mutex_unlock(&(__rwlock->mx_self));

	return 0;
}

int pthread_rwlock_unlock(pthread_rwlock_t * __rwlock)
{
	int blocked_writers_granted = 0;
	int i;

	assert(__rwlock->valid);

	pthread_mutex_lock(&(__rwlock->mx_self));

	if (!__rwlock->readers_reading && !__rwlock->writers_writing) {
		pthread_mutex_unlock(&(__rwlock->mx_self));
		return EINVAL;
	} else {
		assert(__rwlock->writers_writing < 2);

		if (__rwlock->writers_writing) {

			__rwlock->writers_writing = 0;
			if (__rwlock->blocked_writers) {

				if (__rwlock->blocked_writers) {
					for (i = pthread_self()->Prio; i >= 0;
					     i--) {
						blocked_writers_granted +=
						    __rwlock->bwriters_prio[i];
					}
				} else
					blocked_writers_granted = 0;

				if (blocked_writers_granted) {
					pthread_cond_signal(&
							    (__rwlock->
							     writers_lock));
				} else {

					if (__rwlock->blocked_readers) {
						pthread_cond_broadcast(&
								       (__rwlock->
									readers_lock));
					} else {

						pthread_cond_signal(&
								    (__rwlock->
								     writers_lock));
					}
				}

			} else {

				pthread_cond_broadcast(&
						       (__rwlock->
							readers_lock));
			}
		} else {

			__rwlock->readers_reading--;

			pthread_cond_signal(&(__rwlock->readers_lock));
		}
	}
	pthread_mutex_unlock(&(__rwlock->mx_self));
	return 0;
}

int pthread_rwlock_init(pthread_rwlock_t * __rwlock,
			const pthread_rwlockattr_t * attr)
{
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(__rwlock);
	_PTHREAD_NO_WARN_VAR(attr);
	return 0;
}

int pthread_rwlock_destroy(pthread_rwlock_t * __rwlock)
{
	assert(__rwlock->valid);
	assert("Not implemented yet" == 0);
	_PTHREAD_NO_WARN_VAR(__rwlock);
	return 0;
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t * __rwlock)
{
	assert("Not implemented yet" == NULL);
	_PTHREAD_NO_WARN_VAR(__rwlock);
	assert(__rwlock->valid);
	return 0;
}

int pthread_rwlock_timedrdlock(pthread_rwlock_t * __rwlock,
			       const struct timespec *abs_timeout)
{
	assert("Not implemented yet" == NULL);
	_PTHREAD_NO_WARN_VAR(__rwlock);
	_PTHREAD_NO_WARN_VAR(abs_timeout);
	assert(__rwlock->valid);
	return 0;
}

int pthread_rwlock_trywrlock(pthread_rwlock_t * __rwlock)
{
	assert("Not implemented yet" == NULL);
	_PTHREAD_NO_WARN_VAR(__rwlock);
	assert(__rwlock->valid);
	return 0;
}

int pthread_rwlock_timedwrlock(pthread_rwlock_t * __rwlock,
			       const struct timespec *abs_timeout)
{
	assert("Not implemented yet" == NULL);
	_PTHREAD_NO_WARN_VAR(__rwlock);
	_PTHREAD_NO_WARN_VAR(abs_timeout);
	assert(__rwlock->valid);
	return 0;
}
