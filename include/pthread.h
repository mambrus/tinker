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
#ifndef _PTHREAD_H
#define _PTHREAD_H

#if defined(__GNUC__)
#include <tinker/config.h>
#if (!defined(TK_COMP_PTHREAD) || TK_COMP_PTHREAD==0 ) && !defined(ECODES_ONLY)
#error "<pthread.h> belongs to a component that your build of TinKer didn't include. Please reconfigure and rebuild TinKer."
#endif
#define _PTHREAD_PRIOS TK_MAX_PRIO_LEVELS
#else
#define TK_MAX_THREADS 16
#endif

#include <time.h>
#include <sys/types.h>
#include <tk.h>

#define SCHED_OTHER 0
#define SCHED_FIFO 1
#define SCHED_RR 2

#define _PTHREAD_PRIOS TK_MAX_PRIO_LEVELS

#define _PTHREAD_NO_WARN_VAR(x) ((void)x)

#if (TK_MAX_PRIO_LEVELS != _PTHREAD_PRIOS)
#error You need to adjust the _PTHREAD_PRIOS *AND* initializers for arrays depending on this
#endif

typedef enum {
	_PBON_NOLINK = 0,
	_PBON_CONDVAR,
	_PBON_RWLOCK
} pbon_kind_t;

#define _PTHREAD_BLOCKED_INIT {0}
#define _PTHREADS_MAX_BLOCKED TK_MAX_THREADS

#define _PTHREAD_MUTEXATTR_DEFAULT {0,1}

#define PTHREAD_MUTEX_INITIALIZER { \
   NULL, \
   _PTHREAD_BLOCKED_INIT, \
   _PBON_NOLINK, \
   NULL, \
   _PTHREAD_MUTEXATTR_DEFAULT, \
   1 \
}

#define PTHREAD_CV_MUTEX_INITIALIZER { \
   (pthread_t)1, \
   _PTHREAD_BLOCKED_INIT, \
   _PBON_NOLINK, \
   NULL, \
   _PTHREAD_MUTEXATTR_DEFAULT, \
   1 \
}

#define _PTHREAD_CONDATTR_DEFAULT {0,1}

#define PTHREAD_COND_INITIALIZER { \
   NULL, \
   PTHREAD_CV_MUTEX_INITIALIZER, \
   _PTHREAD_CONDATTR_DEFAULT, \
   1 \
}

#define _PTHREAD_RWLOCKATTR_DEFAULT {0,1}

#if (_PTHREAD_PRIOS == 3 )
#define PTHREAD_RWLOCK_INITIALIZER {\
 0, \
 0, \
 0, \
 0, \
 PTHREAD_MUTEX_INITIALIZER, \
 PTHREAD_COND_INITIALIZER, \
 PTHREAD_COND_INITIALIZER, \
 _PTHREAD_RWLOCKATTR_DEFAULT, \
 {0,0,0}, \
 1 \
}

#elif (_PTHREAD_PRIOS == 5 )
#define PTHREAD_RWLOCK_INITIALIZER {\
 0, \
 0, \
 0, \
 0, \
 PTHREAD_MUTEX_INITIALIZER, \
 PTHREAD_COND_INITIALIZER, \
 PTHREAD_COND_INITIALIZER, \
 _PTHREAD_RWLOCKATTR_DEFAULT, \
 {0,0,0,0,0}, \
 1 \
}

#elif (_PTHREAD_PRIOS == 9 )
#define PTHREAD_RWLOCK_INITIALIZER {\
 0, \
 0, \
 0, \
 0, \
 PTHREAD_MUTEX_INITIALIZER, \
 PTHREAD_COND_INITIALIZER, \
 PTHREAD_COND_INITIALIZER, \
 _PTHREAD_RWLOCKATTR_DEFAULT, \
 {0,0,0,0,0,0,0,0,0}, \
 1 \
}

#elif (_PTHREAD_PRIOS == 16 )
#define PTHREAD_RWLOCK_INITIALIZER {\
 0, \
 0, \
 0, \
 0, \
 PTHREAD_MUTEX_INITIALIZER, \
 PTHREAD_COND_INITIALIZER, \
 PTHREAD_COND_INITIALIZER, \
 _PTHREAD_RWLOCKATTR_DEFAULT, \
 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
 1 \
}

#else
#error Number of supported priorities for pthread component is wrong. Supported number is 3,5,9 and 16
#endif
typedef struct _pthread_blocked_t_ {
	int numb;
	pthread_t thread[_PTHREADS_MAX_BLOCKED];
} _pthread_blocked_t;
struct pthread_mutexattr_t_ {
	int TBD_THIS_STRUCT;
	int valid;
};

struct pthread_mutex_t_ {
	pthread_t owner;
	_pthread_blocked_t blocked;
	pbon_kind_t linkOf;
	union {
		void *dummy;
		pthread_cond_t *cond;
		pthread_rwlock_t *rwl;
	} link;
	pthread_mutexattr_t attr;
	int valid;
};

struct pthread_condattr_t_ {
	int TBD_THIS_STRUCT;
	int valid;
};

struct pthread_cond_t_ {
	pthread_mutex_t *coop_mux;
	pthread_mutex_t cv_mux;
	pthread_condattr_t attr;
	int valid;
};

struct pthread_rwlockattr_t_ {
	int TBD_THIS_STRUCT;
	int valid;
};

struct pthread_rwlock_t_ {
	int readers_reading;
	int writers_writing;
	int blocked_writers;
	int blocked_readers;
	pthread_mutex_t mx_self;
	pthread_cond_t readers_lock;
	pthread_cond_t writers_lock;
	pthread_rwlockattr_t attr;
	int bwriters_prio[_PTHREAD_PRIOS];
	int valid;
};

struct sched_param {
	int sched_priority;
};
#define PTHREAD_ONCE_INIT { TK_FALSE, -1 }

struct pthread_once_t_ {
	int done;
	long started;

};

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

	int pthread_create_named_np(pthread_t * thread,
				    const pthread_attr_t * attr,
				    void *(*start_routine) (void *),
				    void *arg, char *threadName);
#define pthread_create(thread,attr,start_routine,arg) ( \
   (pthread_create_named_np( thread, attr, start_routine, arg,"*" #start_routine )) \
)

	pthread_t pthread_self(void);

	int pthread_equal(pthread_t t1, pthread_t t2);
	int pthread_once(pthread_once_t * once_control,
			 void (*init_routine) (void));

	int pthread_cancel(pthread_t);
	int pthread_join(pthread_t, void **);
	int pthread_detach(pthread_t);
	int pthread_yield(void);

	int pthread_setschedparam(pthread_t thread,
				  int policy, const struct sched_param *param);

	int pthread_getschedparam(pthread_t thread,
				  int *policy, struct sched_param *param);
	int pthread_attr_init(pthread_attr_t *);
	int pthread_attr_destroy(pthread_attr_t *);
	int pthread_attr_getdetachstate(const pthread_attr_t *, int *);
	int pthread_attr_getinheritsched(const pthread_attr_t *, int *);
	int pthread_attr_getschedparam(const pthread_attr_t *,
				       struct sched_param *);
	int pthread_attr_getschedpolicy(const pthread_attr_t *, int *);
	int pthread_attr_getscope(const pthread_attr_t *, int *);
	int pthread_attr_setdetachstate(pthread_attr_t *, int);
	int pthread_attr_setinheritsched(pthread_attr_t *, int);
	int pthread_attr_setschedparam(pthread_attr_t *,
				       const struct sched_param *);
	int pthread_attr_setschedpolicy(pthread_attr_t *, int);
	int pthread_attr_setscope(pthread_attr_t *, int);
	int pthread_attr_getstackaddr(const pthread_attr_t *, void **);
	int pthread_attr_setstackaddr(pthread_attr_t *, void *);
	int pthread_attr_getstacksize(const pthread_attr_t *, size_t *);
	int pthread_attr_setstacksize(pthread_attr_t *, size_t);

	int pthread_mutex_init(pthread_mutex_t * mutex,
			       const pthread_mutexattr_t * attr);
	int pthread_mutex_destroy(pthread_mutex_t * mutex);

	int pthread_mutex_trylock(pthread_mutex_t * __mutex);
	int pthread_mutex_lock(pthread_mutex_t * __mutex);
	int pthread_mutex_unlock(pthread_mutex_t * __mutex);

	int pthread_mutex_timedlock(pthread_mutex_t * mutex,
				    const struct timespec *abs_timeout);
	int pthread_cond_init(pthread_cond_t * __cond,
			      const pthread_condattr_t * attr);
	int pthread_cond_destroy(pthread_cond_t * __cond);
	int pthread_cond_signal(pthread_cond_t * __cond);
	int pthread_cond_broadcast(pthread_cond_t * __cond);
	int pthread_cond_wait(pthread_cond_t * __cond, pthread_mutex_t * mutex);
	int pthread_cond_timedwait(pthread_cond_t * __cond,
				   pthread_mutex_t * mutex,
				   const struct timespec *abstime);

	int pthread_condattr_init(pthread_condattr_t * __attr);
	int pthread_condattr_destroy(pthread_condattr_t * __attr);

	int pthread_rwlock_init(pthread_rwlock_t * __rwlock,
				const pthread_rwlockattr_t * attr);
	int pthread_rwlock_destroy(pthread_rwlock_t * __rwlock);
	int pthread_rwlock_rdlock(pthread_rwlock_t * __rwlock);
	int pthread_rwlock_tryrdlock(pthread_rwlock_t * __rwlock);
	int pthread_rwlock_timedrdlock(pthread_rwlock_t * __rwlock,
				       const struct timespec *abs_timeout);
	int pthread_rwlock_wrlock(pthread_rwlock_t * __rwlock);
	int pthread_rwlock_trywrlock(pthread_rwlock_t * __rwlock);
	int pthread_rwlock_timedwrlock(pthread_rwlock_t * __rwlock,
				       const struct timespec *abs_timeout);
	int pthread_rwlock_unlock(pthread_rwlock_t * __rwlock);

	int pthread_rwlockattr_init(pthread_rwlockattr_t * rwlockattr);
	int pthread_rwlockattr_destroy(pthread_rwlockattr_t * rwlockattr);
	unsigned long tk_pthread_sched(void);
	unsigned long tk_pthread_sched_destruct(void);
	unsigned long tk_pthread_sync(void);
	unsigned long tk_pthread_sync_destruct(void);
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#endif
