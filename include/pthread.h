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
@ingroup PTHREAD

@brief Common headerfile for POSIX 1003.1c (pthreads)

TBD

For in-depth discussions about this component, see \ref
PTHREAD

@see PTHREAD
@see COMPONENTS
*/


#ifndef _PTHREAD_H
#define _PTHREAD_H

#if defined(__GNUC__)
	#include <tinker/config.h>
	#if (!defined(TK_COMP_PTHREAD) || TK_COMP_PTHREAD==0 ) && !defined(ECODES_ONLY)
	#error "pthread.h" belongs to a component that your build of TinKer didn't include. Please reconfigure and rebuild TinKer.
	#endif
	#define _PTHREAD_PRIOS TK_MAX_PRIO_LEVELS
#else
	#define TK_MAX_THREADS 16
#endif

#ifndef TK_MAX_THREADS
	#error TK_MAX_THREADS not defined. Did you --enable-max_threads=<val> properly?
#endif

//#include <sched.h>
#include <time.h>
#include <sys/types.h>
#include <tk.h>

#if defined(TINKER)
   #define main(x,y) tk_root() 
#endif

#define SCHED_OTHER 0    //!< not used by tinker
#define SCHED_FIFO  1
#define SCHED_RR    2

//#define _PTHREAD_PRIOS 0x10  //!< Match this with TK_MAX_PRIO_LEVELS. Intentionally not set equal sice some static arrays init needs adjustment. see \ref PTHREAD_RWLOCK_INITIALIZER further down in this file. Easy to fix, but haven't had time yet... For now, just kae sure it will build on a 16 priority system only.

#define _PTHREAD_PRIOS TK_MAX_PRIO_LEVELS

#define _PTHREAD_NO_WARN_VAR(x) ((void)x)  //!< Used in stubbed functions to avoid lots of warnings

#if (TK_MAX_PRIO_LEVELS != _PTHREAD_PRIOS)
   #error You need to adjust the _PTHREAD_PRIOS *AND* initializers for arrays depending on this
#endif

typedef enum {
   _PBON_NOLINK=0,      //!< Blocked on mutex
   _PBON_CONDVAR,       //!< Blocked on a conditional
   _PBON_RWLOCK         //!< Blocked on a RW lock
}pbon_kind_t; 


//------1---------2---------3---------4---------5---------6---------7---------8
#define _PTHREAD_BLOCKED_INIT {0}
#define _PTHREADS_MAX_BLOCKED TK_MAX_THREADS
//------1---------2---------3---------4---------5---------6---------7---------8
#define _PTHREAD_MUTEXATTR_DEFAULT {0,1}

#define PTHREAD_MUTEX_INITIALIZER { \
   NULL,                            \
   _PTHREAD_BLOCKED_INIT,           \
   _PBON_NOLINK,                    \
   NULL,                            \
   _PTHREAD_MUTEXATTR_DEFAULT,      \
   1                                \
}

#define PTHREAD_CV_MUTEX_INITIALIZER { \
   (pthread_t)1,/*Makes sure will block on first take. Value to low to ever be a valid adress*/ \
   _PTHREAD_BLOCKED_INIT,           \
   _PBON_NOLINK,                    \
   NULL,                            \
   _PTHREAD_MUTEXATTR_DEFAULT,      \
   1                                \
}
//------1---------2---------3---------4---------5---------6---------7---------8
#define _PTHREAD_CONDATTR_DEFAULT {0,1}

#define PTHREAD_COND_INITIALIZER {  \
   NULL,                            \
   PTHREAD_CV_MUTEX_INITIALIZER,    \
   _PTHREAD_CONDATTR_DEFAULT,       \
   1                                \
}

//------1---------2---------3---------4---------5---------6---------7---------8
#define _PTHREAD_RWLOCKATTR_DEFAULT {0,1}

/*!
RW lock structure static initializer
@Note: Needs to mach number of prios in system 
*/
#if (_PTHREAD_PRIOS == 3 )
#define PTHREAD_RWLOCK_INITIALIZER {\
	0,				\
	0,				\
	0,				\
	0,				\
	PTHREAD_MUTEX_INITIALIZER,	\
	PTHREAD_COND_INITIALIZER,	\
	PTHREAD_COND_INITIALIZER,	\
	_PTHREAD_RWLOCKATTR_DEFAULT,	\
	{0,0,0},			\
	1				\
}


#elif  (_PTHREAD_PRIOS == 5 )
#define PTHREAD_RWLOCK_INITIALIZER {\
	0,				\
	0,				\
	0,				\
	0,				\
	PTHREAD_MUTEX_INITIALIZER,	\
	PTHREAD_COND_INITIALIZER,	\
	PTHREAD_COND_INITIALIZER,	\
	_PTHREAD_RWLOCKATTR_DEFAULT,	\
	{0,0,0,0,0},			\
	1				\
}


#elif (_PTHREAD_PRIOS == 9 )
#define PTHREAD_RWLOCK_INITIALIZER {\
	0,				\
	0,				\
	0,				\
	0,				\
	PTHREAD_MUTEX_INITIALIZER,	\
	PTHREAD_COND_INITIALIZER,	\
	PTHREAD_COND_INITIALIZER,	\
	_PTHREAD_RWLOCKATTR_DEFAULT,	\
	{0,0,0,0,0,0,0,0,0},		\
	1				\
}


#elif (_PTHREAD_PRIOS == 16 )
#define PTHREAD_RWLOCK_INITIALIZER {\
	0,				\
	0,				\
	0,				\
	0,				\
	PTHREAD_MUTEX_INITIALIZER,	\
	PTHREAD_COND_INITIALIZER,	\
	PTHREAD_COND_INITIALIZER,	\
	_PTHREAD_RWLOCKATTR_DEFAULT,	\
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},\
	1				\
}


#else
	#Error Number of supported priorities for pthread component is wrong. Supported number is 3,5,9 and 16
#endif


//------1---------2---------3---------4---------5---------6---------7---------8
/*!
A list of blocked threads. This is to speed up release (instead of letting the dispatcher finding who should be released).

Currently inplemented as a table. Consider this struct "opaque", i.e. implemettation might change in future.

@note The table has to come last in the struct for static initialization to work properly.
*/
typedef struct _pthread_blocked_t_{
   int numb;
   pthread_t thread[_PTHREADS_MAX_BLOCKED ];
}_pthread_blocked_t;

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
TBD
 
@see http://www.freepascal.org/docs-html/rtl/unixtype/pthread_mutexattr_t.html
*/

struct pthread_mutexattr_t_ {
   int TBD_THIS_STRUCT;
   int                  valid;    //!< equals 1 if been initialized (sanity check)
};

/*!
TBD
 
@see http://www.freepascal.org/docs-html/rtl/unixtype/pthread_mutex_t.html
*/                      
struct pthread_mutex_t_ {
   pthread_t            owner;    //!< Who (i.e. which thread) has claimed ownership of this mutex
   _pthread_blocked_t   blocked;  //!< Information about blocked threads (blocklist on this mutex)
   pbon_kind_t          linkOf;   //!< Information about linked resource
   union{
      void             *dummy;
      pthread_cond_t   *cond;
      pthread_rwlock_t *rwl;      
   }link;
   pthread_mutexattr_t  attr;     //!< Attributes of this mutex   
   int                  valid;    //!< equals 1 if been initialized (sanity check)
};
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
TBD
 
@see http://www.freepascal.org/docs-html/rtl/unixtype/pthread_condattr_t.html
*/
struct pthread_condattr_t_ {
   int TBD_THIS_STRUCT;
   int                  valid;    //!< equals 1 if been initialized (sanity check)
};

/*!
TBD
 
@see http://www.freepascal.org/docs-html/rtl/unixtype/pthread_cond_t.html
*/
struct pthread_cond_t_ {
   pthread_mutex_t        *coop_mux;
   pthread_mutex_t         cv_mux;
   pthread_condattr_t      attr;
   int                     valid;    //!< equals 1 if been initialized (sanity check)
};
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
TBD
 
@see http://www.freepascal.org/docs-html/rtl/unixtype/pthread_condattr_t.html
*/
struct pthread_rwlockattr_t_{
   int TBD_THIS_STRUCT;
   int                  valid;    //!< equals 1 if been initialized (sanity check)
};

/*!
TBD
 
@see http://www.freepascal.org/docs-html/rtl/unixtype/pthread_cond_t.html
*/
struct pthread_rwlock_t_ {
   int                     readers_reading;
   int                     writers_writing;
   int                     blocked_writers;
   int                     blocked_readers;
   pthread_mutex_t         mx_self;
   pthread_cond_t          readers_lock;   
   pthread_cond_t          writers_lock;   
   pthread_rwlockattr_t    attr;
   int                     bwriters_prio[_PTHREAD_PRIOS];
   int                     valid;    //!< equals 1 if been initialized (sanity check)
};


//------1---------2---------3---------4---------5---------6---------7---------8
struct sched_param {
  int sched_priority;
};
//------1---------2---------3---------4---------5---------6---------7---------8

/*!
Describes the thread attributes. It should be considered an opaque
record, the names of the fields can change anytime. Use the appropriate
functions to set the thread attributes.
 
@see http://www.freepascal.org/docs-html/rtl/unixtype/pthread_attr_t.html
*/

/*! 
@brief Once Key

Once Key. Threads needing to run \ref pthread_once must pass a reference
to a shared variable (\ref pthread_once_t_ *once_control) that will
control weather init function shall be run or not.

@note How you this way can have several init functions and/or execute
one more than once if needed.

*/
#define PTHREAD_ONCE_INIT       { TK_FALSE, -1 }  


struct pthread_once_t_ {
  int done;                 //!< indicates if user function executed 
  long started;             /*!< First thread to increment this value
                                 to zero executes the user function */
};

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif



int pthread_create_named_np (
   pthread_t               *thread,
   const pthread_attr_t    *attr,
   void *(*start_routine)  (void *),
   void *                  arg,
   char                    *threadName
);
/*
Compacted form for for TiNa
int pthread_create_named_np (pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)  (void *), void * arg, char *threadName);
*/

/*
Implemented as a macro to be able to provide TinKer with a thread-name.

Otherwise it behaves exactlly identical to the function standard specification.

@see http://www.opengroup.org/onlinepubs/009695399/functions/pthread_create.html
*/
#define pthread_create(      \
   thread,                   \
   attr,                     \
   start_routine,            \
   arg                       \
) (                          \
   (pthread_create_named_np( thread, attr, start_routine, arg,"*" #start_routine ))   \
)

pthread_t pthread_self (void);

int pthread_equal(pthread_t t1, pthread_t t2); 
int pthread_once (pthread_once_t *once_control, void (*init_routine) (void));

int pthread_cancel   (pthread_t);
int pthread_join     (pthread_t, void**);
int pthread_detach   (pthread_t);
int pthread_yield    (void);

int pthread_setschedparam (pthread_t thread,
			   int policy,
			   const struct sched_param *param);

int pthread_getschedparam (pthread_t thread,
			   int *policy,
			   struct sched_param *param);

/*
Compacted form for TiNa
int pthread_setschedparam (pthread_t thread, int policy, const struct sched_param *param);
int pthread_getschedparam (pthread_t thread, int *policy, struct sched_param *param);
*/


/* Attributes */

int pthread_attr_init (pthread_attr_t *);
int pthread_attr_destroy (pthread_attr_t *);
int pthread_attr_getdetachstate (const pthread_attr_t *, int *);
int pthread_attr_getinheritsched (const pthread_attr_t *, int *);
int pthread_attr_getschedparam (const pthread_attr_t *, struct sched_param *);
int pthread_attr_getschedpolicy (const pthread_attr_t *, int *);
int pthread_attr_getscope (const pthread_attr_t *, int *);
int pthread_attr_setdetachstate (pthread_attr_t *, int);
int pthread_attr_setinheritsched (pthread_attr_t *, int);
int pthread_attr_setschedparam (pthread_attr_t *, const struct sched_param *);
int pthread_attr_setschedpolicy (pthread_attr_t *, int);
int pthread_attr_setscope (pthread_attr_t *, int);
int pthread_attr_getstackaddr (const pthread_attr_t *, void **);
int pthread_attr_setstackaddr (pthread_attr_t *, void *);
int pthread_attr_getstacksize (const pthread_attr_t *, size_t *);
int pthread_attr_setstacksize (pthread_attr_t *, size_t);



/*!
@name Mutex creation destruction
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_mutex_init.html
*/
//@{
int pthread_mutex_init (pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
//@}
/*!
@name Mutex usage
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_mutex_lock.html
*/
//@{
int pthread_mutex_trylock (pthread_mutex_t *__mutex);
int pthread_mutex_lock (pthread_mutex_t *__mutex);
int pthread_mutex_unlock (pthread_mutex_t *__mutex);
//@}

/*!
@name Mutex "advanced realtime" usage
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_mutex_timedlock.html
*/
//@{
int pthread_mutex_timedlock(pthread_mutex_t *mutex,const struct timespec *abs_timeout);
//@}

/*
int pthread_mutexattr_init (pthread_mutexattr_t *__attr);
int pthread_mutexattr_destroy (pthread_mutexattr_t *__attr);
int pthread_mutexattr_getpshared (__const pthread_mutexattr_t *
int pthread_mutexattr_setpshared (pthread_mutexattr_t *__attr,
int pthread_mutexattr_settype (pthread_mutexattr_t *__attr, int __kind)
int pthread_mutexattr_gettype (__const pthread_mutexattr_t *__restrict
*/

//------1---------2---------3---------4---------5---------6---------7---------8
int pthread_cond_init (pthread_cond_t * __cond, const pthread_condattr_t *attr);
int pthread_cond_destroy (pthread_cond_t *__cond);
int pthread_cond_signal (pthread_cond_t *__cond);
int pthread_cond_broadcast (pthread_cond_t *__cond);
int pthread_cond_wait (pthread_cond_t * __cond, pthread_mutex_t *mutex);
int pthread_cond_timedwait (pthread_cond_t * __cond, pthread_mutex_t *mutex, const struct timespec *abstime);
//------1---------2---------3---------4---------5---------6---------7---------8
int pthread_condattr_init (pthread_condattr_t *__attr);
int pthread_condattr_destroy (pthread_condattr_t *__attr);

//->int pthread_condattr_getpshared (__const pthread_condattr_t *attr, int *pshared);
//->int pthread_condattr_setpshared (pthread_condattr_t *__attr, int pshared);
//------1---------2---------3---------4---------5---------6---------7---------8

int pthread_rwlock_init (pthread_rwlock_t *__rwlock, const pthread_rwlockattr_t *attr);
int pthread_rwlock_destroy (pthread_rwlock_t *__rwlock);
int pthread_rwlock_rdlock (pthread_rwlock_t *__rwlock);
int pthread_rwlock_tryrdlock (pthread_rwlock_t *__rwlock);
int pthread_rwlock_timedrdlock (pthread_rwlock_t * __rwlock, const struct timespec *abs_timeout); 
int pthread_rwlock_wrlock (pthread_rwlock_t *__rwlock);
int pthread_rwlock_trywrlock (pthread_rwlock_t *__rwlock);
int pthread_rwlock_timedwrlock (pthread_rwlock_t * __rwlock, const struct timespec *abs_timeout); 
int pthread_rwlock_unlock (pthread_rwlock_t *__rwlock);
//------1---------2---------3---------4---------5---------6---------7---------8
int pthread_rwlockattr_init (pthread_rwlockattr_t *rwlockattr);
int pthread_rwlockattr_destroy (pthread_rwlockattr_t *rwlockattr);

//->int pthread_rwlockattr_getpshared (const pthread_rwlockattr_t *attr,int *pshared);
//->int pthread_rwlockattr_setpshared (pthread_rwlockattr_t *attr, int pshared);

//------1---------2---------3---------4---------5---------6---------7---------8

/*!
@name Creation and destruction of this component

Use these functions only at boot, and shut-down.

@see COMPONENTS
*/
//@{

unsigned long tk_pthread_sched( void );
unsigned long tk_pthread_sched_destruct( void );
unsigned long tk_pthread_sync( void );
unsigned long tk_pthread_sync_destruct( void );

//@}



/*
The ones not commeted out is implemented by TinKer
*/

// int pthread_create (
// pthread_t pthread_self (void);
// int pthread_equal (pthread_t __thread1, pthread_t __thread2);
// void pthread_exit (void *__retval)
// int pthread_join (pthread_t __th, void **__thread_return);
// int pthread_detach (pthread_t __th);
// int pthread_attr_init (pthread_attr_t *__attr);
// int pthread_attr_destroy (pthread_attr_t *__attr);
// int pthread_attr_setdetachstate (pthread_attr_t *__attr,
// int pthread_attr_getdetachstate (__const pthread_attr_t *__attr,
// int pthread_attr_setschedparam (pthread_attr_t *__restrict __attr,
// int pthread_attr_getschedparam (__const pthread_attr_t *__restrict
// int pthread_attr_setschedpolicy (pthread_attr_t *__attr, int __policy)
// int pthread_attr_getschedpolicy (__const pthread_attr_t *__restrict
// int pthread_attr_setinheritsched (pthread_attr_t *__attr,
// int pthread_attr_getinheritsched (__const pthread_attr_t *__restrict
// int pthread_attr_setscope (pthread_attr_t *__attr, int __scope)
// int pthread_attr_getscope (__const pthread_attr_t *__restrict __attr,
// int pthread_attr_setguardsize (pthread_attr_t *__attr,
// int pthread_attr_getguardsize (__const pthread_attr_t *__restrict
// int pthread_attr_setstackaddr (pthread_attr_t *__attr,
// int pthread_attr_getstackaddr (__const pthread_attr_t *__restrict
// int pthread_attr_setstack (pthread_attr_t *__attr, void *__stackaddr,
// int pthread_attr_getstack (__const pthread_attr_t *__restrict __attr,
// int pthread_attr_setstacksize (pthread_attr_t *__attr,
// int pthread_attr_getstacksize (__const pthread_attr_t *__restrict
// int pthread_getattr_np (pthread_t __th, pthread_attr_t *__attr);
// int pthread_setschedparam (pthread_t __target_thread, int __policy,
// int pthread_getschedparam (pthread_t __target_thread,
// int pthread_getconcurrency (void);
// int pthread_setconcurrency (int __level);
// int pthread_spin_init (pthread_spinlock_t *__lock, int __pshared)
// int pthread_spin_destroy (pthread_spinlock_t *__lock);
// int pthread_spin_lock (pthread_spinlock_t *__lock);
// int pthread_spin_trylock (pthread_spinlock_t *__lock);
// int pthread_spin_unlock (pthread_spinlock_t *__lock);
// int pthread_barrier_init (pthread_barrier_t *__restrict __barrier,
// int pthread_barrier_destroy (pthread_barrier_t *__barrier);
// int pthread_barrierattr_init (pthread_barrierattr_t *__attr);
// int pthread_barrierattr_destroy (pthread_barrierattr_t *__attr);
// int pthread_barrierattr_getpshared (__const pthread_barrierattr_t *
// int pthread_barrierattr_setpshared (pthread_barrierattr_t *__attr,
// int pthread_barrier_wait (pthread_barrier_t *__barrier);
// int pthread_key_create (pthread_key_t *__key,
// int pthread_key_delete (pthread_key_t __key);
// int pthread_setspecific (pthread_key_t __key,
// void *pthread_getspecific (pthread_key_t __key);
// variable initialized to PTHREAD_ONCE_INIT.  */
// int pthread_once (pthread_once_t *__once_control,
// int pthread_setcancelstate (int __state, int *__oldstate);
// int pthread_setcanceltype (int __type, int *__oldtype);
//>>>int pthread_cancel (pthread_t __thread);
// void pthread_testcancel (void);
// void _pthread_cleanup_push (struct _pthread_cleanup_buffer *__buffer,
// void _pthread_cleanup_pop (struct _pthread_cleanup_buffer *__buffer,
// void _pthread_cleanup_push_defer (struct _pthread_cleanup_buffer *__buffer,
// void _pthread_cleanup_pop_restore (struct _pthread_cleanup_buffer *__buffer,
// int pthread_getcpuclockid (pthread_t __thread_id,
// int pthread_atfork (void (*__prepare) (void),
// void pthread_kill_other_threads_np (void);


///*! Cleanup buffers */
//struct _pthread_cleanup_buffer
//{
//  void (*__routine) (void *);             /*!< Function to call.  */
//  void *__arg;                            /*!< Its argument.  */
//  int __canceltype;                       /*!< Saved cancellation type. */
//  struct _pthread_cleanup_buffer *__prev; /*!< Chaining of cleanup functions.  */
//};
//
///* Function for handling threads.  */
//
//extern int pthread_create (
//   pthread_t *__thread,
//   __const pthread_attr_t *__restrict __attr,
//   void *(*__start_routine) (void *),
//   void *__restrict __arg) __THROW;
//
///* Obtain the identifier of the current thread.  */
//extern pthread_t pthread_self (void);
//
///* Compare two thread identifiers.  */
//extern int pthread_equal (pthread_t __thread1, pthread_t __thread2);
//
///* Terminate calling thread.  */
//extern void pthread_exit (void *__retval)
//     __attribute__ ((__noreturn__));
//
///* Make calling thread wait for termination of the thread TH.  The
//   exit status of the thread is stored in *THREAD_RETURN, if THREAD_RETURN
//   is not NULL.  */
//extern int pthread_join (pthread_t __th, void **__thread_return);
//
///* Indicate that the thread TH is never to be joined with PTHREAD_JOIN.
//   The resources of TH will therefore be freed immediately when it
//   terminates, instead of waiting for another thread to perform PTHREAD_JOIN
//   on it.  */
//extern int pthread_detach (pthread_t __th);
//
//
///* Functions for handling attributes.  */
//
///* Initialize thread attribute *ATTR with default attributes
//   (detachstate is PTHREAD_JOINABLE, scheduling policy is SCHED_OTHER,
//    no user-provided stack).  */
//extern int pthread_attr_init (pthread_attr_t *__attr);
//
///* Destroy thread attribute *ATTR.  */
//extern int pthread_attr_destroy (pthread_attr_t *__attr);
//
///* Set the `detachstate' attribute in *ATTR according to DETACHSTATE.  */
//extern int pthread_attr_setdetachstate (pthread_attr_t *__attr,
//               int __detachstate);
//
///* Return in *DETACHSTATE the `detachstate' attribute in *ATTR.  */
//extern int pthread_attr_getdetachstate (__const pthread_attr_t *__attr,
//               int *__detachstate);
//
///* Set scheduling parameters (priority, etc) in *ATTR according to PARAM.  */
//extern int pthread_attr_setschedparam (pthread_attr_t *__restrict __attr,
//                   __const struct sched_param *__restrict
//                   __param);
//
///* Return in *PARAM the scheduling parameters of *ATTR.  */
//extern int pthread_attr_getschedparam (__const pthread_attr_t *__restrict
//                   __attr,
//                   struct sched_param *__restrict __param)
//    ;
//
///*! Set scheduling policy in *ATTR according to POLICY.  */
//extern int pthread_attr_setschedpolicy (pthread_attr_t *__attr, int __policy)
//    ;
//
///*! Return in *POLICY the scheduling policy of *ATTR.  */
//extern int pthread_attr_getschedpolicy (__const pthread_attr_t *__restrict
//               __attr, int *__restrict __policy)
//    ;
//
///*! Set scheduling inheritance mode in *ATTR according to INHERIT.  */
//extern int pthread_attr_setinheritsched (pthread_attr_t *__attr,
//                int __inherit);
//
///*! Return in *INHERIT the scheduling inheritance mode of *ATTR.  */
//extern int pthread_attr_getinheritsched (__const pthread_attr_t *__restrict
//                __attr, int *__restrict __inherit)
//    ;
//
///*! Set scheduling contention scope in *ATTR according to SCOPE.  */
//extern int pthread_attr_setscope (pthread_attr_t *__attr, int __scope)
//    ;
//
///*! Return in *SCOPE the scheduling contention scope of *ATTR.  */
//extern int pthread_attr_getscope (__const pthread_attr_t *__restrict __attr,
//              int *__restrict __scope);
//
//#ifdef __USE_UNIX98
///*! Set the size of the guard area at the bottom of the thread.  */
//extern int pthread_attr_setguardsize (pthread_attr_t *__attr,
//                  size_t __guardsize);
//
///*! Get the size of the guard area at the bottom of the thread.  */
//extern int pthread_attr_getguardsize (__const pthread_attr_t *__restrict
//                  __attr, size_t *__restrict __guardsize)
//    ;
//#endif
//
///*! Set the starting address of the stack of the thread to be created.
//   Depending on whether the stack grows up or down the value must either
//   be higher or lower than all the address in the memory block.  The
//   minimal size of the block must be PTHREAD_STACK_SIZE.  */
//extern int pthread_attr_setstackaddr (pthread_attr_t *__attr,
//                  void *__stackaddr);
//
///*! Return the previously set address for the stack.  */
//extern int pthread_attr_getstackaddr (__const pthread_attr_t *__restrict
//                  __attr, void **__restrict __stackaddr)
//    ;
//
//#ifdef __USE_XOPEN2K
///*! The following two interfaces are intended to replace the last two.  They
//   require setting the address as well as the size since only setting the
//   address will make the implementation on some architectures impossible.  */
//extern int pthread_attr_setstack (pthread_attr_t *__attr, void *__stackaddr,
//              size_t __stacksize);
//
///*! Return the previously set address for the stack.  */
//extern int pthread_attr_getstack (__const pthread_attr_t *__restrict __attr,
//              void **__restrict __stackaddr,
//              size_t *__restrict __stacksize);
//#endif
//
///*! Add information about the minimum stack size needed for the thread
//   to be started.  This size must never be less than PTHREAD_STACK_SIZE
//   and must also not exceed the system limits.  */
//extern int pthread_attr_setstacksize (pthread_attr_t *__attr,
//                  size_t __stacksize);
//
///*! Return the currently used minimal stack size.  */
//extern int pthread_attr_getstacksize (__const pthread_attr_t *__restrict
//                  __attr, size_t *__restrict __stacksize)
//    ;
//
//#ifdef __USE_GNU
///*! Get thread attributes corresponding to the already running thread TH.  */
//extern int pthread_getattr_np (pthread_t __th, pthread_attr_t *__attr);
//#endif
//
///*! Functions for scheduling control.  */
//
///*! Set the scheduling parameters for TARGET_THREAD according to POLICY
//   and *PARAM.  */
//extern int pthread_setschedparam (pthread_t __target_thread, int __policy,
//              __const struct sched_param *__param)
//    ;
//
///*! Return in *POLICY and *PARAM the scheduling parameters for TARGET_THREAD.  */
//extern int pthread_getschedparam (pthread_t __target_thread,
//              int *__restrict __policy,
//              struct sched_param *__restrict __param)
//    ;
//
//#ifdef __USE_UNIX98
///*! Determine level of concurrency.  */
//extern int pthread_getconcurrency (void);
//
///*! Set new concurrency level to LEVEL.  */
//extern int pthread_setconcurrency (int __level);
//#endif
//
//#ifdef __USE_GNU
///*! Yield the processor to another thread or process.
//   This function is similar to the POSIX `sched_yield' function but
//   might be differently implemented in the case of a m-on-n thread
//   implementation.  */
//extern int pthread_yield (void);
//#endif
//
///*! Functions for mutex handling.  */
//
///*! Initialize MUTEX using attributes in *MUTEX_ATTR, or use the
//   default values if later is NULL.  */
//extern int pthread_mutex_init (pthread_mutex_t *__restrict __mutex,
//                __const pthread_mutexattr_t *__restrict
//                __mutex_attr);
//
///*! Destroy MUTEX.  */
//extern int pthread_mutex_destroy (pthread_mutex_t *__mutex);
//
///*! Try to lock MUTEX.  */
//extern int pthread_mutex_trylock (pthread_mutex_t *__mutex);
//
///*! Wait until lock for MUTEX becomes available and lock it.  */
//extern int pthread_mutex_lock (pthread_mutex_t *__mutex);
//
//#ifdef __USE_XOPEN2K
///*! Wait until lock becomes available, or specified time passes. */
//extern int pthread_mutex_timedlock (pthread_mutex_t *__restrict __mutex,
//                __const struct timespec *__restrict
//                __abstime);
//#endif
//
///*! Unlock MUTEX.  */
//extern int pthread_mutex_unlock (pthread_mutex_t *__mutex);
//
//
///*! Functions for handling mutex attributes.  */
//
///*! Initialize mutex attribute object ATTR with default attributes
//   (kind is PTHREAD_MUTEX_TIMED_NP).  */
//extern int pthread_mutexattr_init (pthread_mutexattr_t *__attr);
//
///*! Destroy mutex attribute object ATTR.  */
//extern int pthread_mutexattr_destroy (pthread_mutexattr_t *__attr);
//
///*! Get the process-shared flag of the mutex attribute ATTR.  */
//extern int pthread_mutexattr_getpshared (__const pthread_mutexattr_t *
//                __restrict __attr,
//                int *__restrict __pshared);
//
///*! Set the process-shared flag of the mutex attribute ATTR.  */
//extern int pthread_mutexattr_setpshared (pthread_mutexattr_t *__attr,
//                int __pshared);
//
//#ifdef __USE_UNIX98
///*! Set the mutex kind attribute in *ATTR to KIND (either PTHREAD_MUTEX_NORMAL,
//   PTHREAD_MUTEX_RECURSIVE, PTHREAD_MUTEX_ERRORCHECK, or
//   PTHREAD_MUTEX_DEFAULT).  */
//extern int pthread_mutexattr_settype (pthread_mutexattr_t *__attr, int __kind)
//    ;
//
///*! Return in *KIND the mutex kind attribute in *ATTR.  */
//extern int pthread_mutexattr_gettype (__const pthread_mutexattr_t *__restrict
//                  __attr, int *__restrict __kind);
//#endif
//
//
///* Functions for handling conditional variables.  */
//
///*! Initialize condition variable COND using attributes ATTR, or use
//   the default values if later is NULL.  */
//extern int pthread_cond_init (pthread_cond_t *__restrict __cond,
//               __const pthread_condattr_t *__restrict
//               __cond_attr);
//
///*! Destroy condition variable COND.  */
//extern int pthread_cond_destroy (pthread_cond_t *__cond);
//
///*! Wake up one thread waiting for condition variable COND.  */
//extern int pthread_cond_signal (pthread_cond_t *__cond);
//
///*! Wake up all threads waiting for condition variables COND.  */
//extern int pthread_cond_broadcast (pthread_cond_t *__cond);
//
///*! Wait for condition variable COND to be signaled or broadcast.
//   MUTEX is assumed to be locked before.  */
//extern int pthread_cond_wait (pthread_cond_t *__restrict __cond,
//               pthread_mutex_t *__restrict __mutex);
//
///*! Wait for condition variable COND to be signaled or broadcast until
//   ABSTIME.  MUTEX is assumed to be locked before.  ABSTIME is an
//   absolute time specification; zero is the beginning of the epoch
//   (00:00:00 GMT, January 1, 1970).  */
//extern int pthread_cond_timedwait (pthread_cond_t *__restrict __cond,
//               pthread_mutex_t *__restrict __mutex,
//               __const struct timespec *__restrict
//               __abstime);
//
///*! Functions for handling condition variable attributes.  */
//
///*! Initialize condition variable attribute ATTR.  */
//extern int pthread_condattr_init (pthread_condattr_t *__attr);
//
///*! Destroy condition variable attribute ATTR.  */
//extern int pthread_condattr_destroy (pthread_condattr_t *__attr);
//
///*! Get the process-shared flag of the condition variable attribute ATTR.  */
//extern int pthread_condattr_getpshared (__const pthread_condattr_t *
//               __restrict __attr,
//               int *__restrict __pshared);
//
///*! Set the process-shared flag of the condition variable attribute ATTR.  */
//extern int pthread_condattr_setpshared (pthread_condattr_t *__attr,
//               int __pshared);
//
//
//#ifdef __USE_UNIX98
///*! Functions for handling read-write locks.  */
//
///*! Initialize read-write lock RWLOCK using attributes ATTR, or use
//   the default values if later is NULL.  */
//extern int pthread_rwlock_init (pthread_rwlock_t *__restrict __rwlock,
//            __const pthread_rwlockattr_t *__restrict
//            __attr);
//
///*! Destroy read-write lock RWLOCK.  */
//extern int pthread_rwlock_destroy (pthread_rwlock_t *__rwlock);
//
///*! Acquire read lock for RWLOCK.  */
//extern int pthread_rwlock_rdlock (pthread_rwlock_t *__rwlock);
//
///*! Try to acquire read lock for RWLOCK.  */
//extern int pthread_rwlock_tryrdlock (pthread_rwlock_t *__rwlock);
//
//#ifdef __USE_XOPEN2K
///*! Try to acquire read lock for RWLOCK or return after specfied time.  */
//extern int pthread_rwlock_timedrdlock (pthread_rwlock_t *__restrict __rwlock,
//                   __const struct timespec *__restrict
//                   __abstime);
//#endif
//
///*! Acquire write lock for RWLOCK.  */
//extern int pthread_rwlock_wrlock (pthread_rwlock_t *__rwlock);
//
///*! Try to acquire write lock for RWLOCK.  */
//extern int pthread_rwlock_trywrlock (pthread_rwlock_t *__rwlock);
//
//#ifdef __USE_XOPEN2K
///*! Try to acquire write lock for RWLOCK or return after specfied time.  */
//extern int pthread_rwlock_timedwrlock (pthread_rwlock_t *__restrict __rwlock,
//                   __const struct timespec *__restrict
//                   __abstime);
//#endif
//
///*! Unlock RWLOCK.  */
//extern int pthread_rwlock_unlock (pthread_rwlock_t *__rwlock);
//
//
///*! Functions for handling read-write lock attributes.  */
//
///*! Initialize attribute object ATTR with default values.  */
//extern int pthread_rwlockattr_init (pthread_rwlockattr_t *__attr);
//
///*! Destroy attribute object ATTR.  */
//extern int pthread_rwlockattr_destroy (pthread_rwlockattr_t *__attr);
//
///*! Return current setting of process-shared attribute of ATTR in PSHARED.  */
//extern int pthread_rwlockattr_getpshared (__const pthread_rwlockattr_t *
//                 __restrict __attr,
//                 int *__restrict __pshared);
//
///*! Set process-shared attribute of ATTR to PSHARED.  */
//extern int pthread_rwlockattr_setpshared (pthread_rwlockattr_t *__attr,
//                 int __pshared);
//
///*! Return current setting of reader/writer preference.  */
//extern int pthread_rwlockattr_getkind_np (__const pthread_rwlockattr_t *__attr,
//                 int *__pref);
//
///*! Set reader/write preference.  */
//extern int pthread_rwlockattr_setkind_np (pthread_rwlockattr_t *__attr,
//                 int __pref);
//#endif
//
//#ifdef __USE_XOPEN2K
///*! The IEEE Std. 1003.1j-2000 introduces functions to implement
//   spinlocks.  */
//
///*! Initialize the spinlock LOCK.  If PSHARED is nonzero the spinlock can
//   be shared between different processes.  */
//extern int pthread_spin_init (pthread_spinlock_t *__lock, int __pshared)
//    ;
//
///*! Destroy the spinlock LOCK.  */
//extern int pthread_spin_destroy (pthread_spinlock_t *__lock);
//
///*! Wait until spinlock LOCK is retrieved.  */
//extern int pthread_spin_lock (pthread_spinlock_t *__lock);
//
///*! Try to lock spinlock LOCK.  */
//extern int pthread_spin_trylock (pthread_spinlock_t *__lock);
//
///*! Release spinlock LOCK.  */
//extern int pthread_spin_unlock (pthread_spinlock_t *__lock);
//
//
///*! Barriers are a also a new feature in 1003.1j-2000. */
//
//extern int pthread_barrier_init (pthread_barrier_t *__restrict __barrier,
//             __const pthread_barrierattr_t *__restrict
//             __attr, unsigned int __count);
//
//extern int pthread_barrier_destroy (pthread_barrier_t *__barrier);
//
//extern int pthread_barrierattr_init (pthread_barrierattr_t *__attr);
//
//extern int pthread_barrierattr_destroy (pthread_barrierattr_t *__attr);
//
//extern int pthread_barrierattr_getpshared (__const pthread_barrierattr_t *
//                  __restrict __attr,
//                  int *__restrict __pshared);
//
//extern int pthread_barrierattr_setpshared (pthread_barrierattr_t *__attr,
//                  int __pshared);
//
//extern int pthread_barrier_wait (pthread_barrier_t *__barrier);
//#endif
//
//
///*! Functions for handling thread-specific data.  */
//
///*! Create a key value identifying a location in the thread-specific
//   data area.  Each thread maintains a distinct thread-specific data
//   area.  DESTR_FUNCTION, if non-NULL, is called with the value
//   associated to that key when the key is destroyed.
//   DESTR_FUNCTION is not called if the value associated is NULL when
//   the key is destroyed.  */
//extern int pthread_key_create (pthread_key_t *__key,
//                void (*__destr_function) (void *));
//
///*! Destroy KEY.  */
//extern int pthread_key_delete (pthread_key_t __key);
//
///*! Store POINTER in the thread-specific data slot identified by KEY. */
//extern int pthread_setspecific (pthread_key_t __key,
//            __const void *__pointer);
//
///*! Return current value of the thread-specific data slot identified by KEY.  */
//extern void *pthread_getspecific (pthread_key_t __key);
//
//
///*! Functions for handling initialization.  */
//
///*! Guarantee that the initialization function INIT_ROUTINE will be called
//   only once, even if pthread_once is executed several times with the
//   same ONCE_CONTROL argument. ONCE_CONTROL must point to a static or
//   extern variable initialized to PTHREAD_ONCE_INIT.  */
//extern int pthread_once (pthread_once_t *__once_control,
//          void (*__init_routine) (void));
//
//
///*! Functions for handling cancellation.  */
//
///*! Set cancelability state of current thread to STATE, returning old
//   state in *OLDSTATE if OLDSTATE is not NULL.  */
//extern int pthread_setcancelstate (int __state, int *__oldstate);
//
///*! Set cancellation state of current thread to TYPE, returning the old
//   type in *OLDTYPE if OLDTYPE is not NULL.  */
//extern int pthread_setcanceltype (int __type, int *__oldtype);
//
///*! Cancel THREAD immediately or at the next possibility.  */
//extern int pthread_cancel (pthread_t __thread);
//
///*! Test for pending cancellation for the current thread and terminate
//   the thread as per pthread_exit(PTHREAD_CANCELED) if it has been
//   cancelled.  */
//extern void pthread_testcancel (void);
//
//
///*! Install a cleanup handler: ROUTINE will be called with arguments ARG
//   when the thread is cancelled or calls pthread_exit.  ROUTINE will also
//   be called with arguments ARG when the matching pthread_cleanup_pop
//   is executed with non-zero EXECUTE argument.
//   pthread_cleanup_push and pthread_cleanup_pop are macros and must always
//   be used in matching pairs at the same nesting level of braces. */
//
/*
#define pthread_cleanup_push(routine,arg) \
  { struct _pthread_cleanup_buffer _buffer;                 \
    _pthread_cleanup_push (&_buffer, (routine), (arg));
*/
/*
extern void _pthread_cleanup_push (struct _pthread_cleanup_buffer *__buffer,
               void (*__routine) (void *),
               void *__arg);
*/
/*! Remove a cleanup handler installed by the matching pthread_cleanup_push.
   If EXECUTE is non-zero, the handler function is called. */
/*
#define pthread_cleanup_pop(execute) \
    _pthread_cleanup_pop (&_buffer, (execute)); }
*/
/*
extern void _pthread_cleanup_pop (struct _pthread_cleanup_buffer *__buffer,
              int __execute);
*/
/*! Install a cleanup handler as pthread_cleanup_push does, but also
   saves the current cancellation type and set it to deferred cancellation.  */
/*
#ifdef __USE_GNU
# define pthread_cleanup_push_defer_np(routine,arg) \
  { struct _pthread_cleanup_buffer _buffer;                 \
    _pthread_cleanup_push_defer (&_buffer, (routine), (arg));
*/
/*
extern void _pthread_cleanup_push_defer (struct _pthread_cleanup_buffer *__buffer,
                void (*__routine) (void *),
                void *__arg);
*/
/*! Remove a cleanup handler as pthread_cleanup_pop does, but also
   restores the cancellation type that was in effect when the matching
   pthread_cleanup_push_defer was called.  */

/*
# define pthread_cleanup_pop_restore_np(execute) \
  _pthread_cleanup_pop_restore (&_buffer, (execute)); }

extern void _pthread_cleanup_pop_restore (struct _pthread_cleanup_buffer *__buffer,
                 int __execute);
#endif
*/

//#ifdef __USE_XOPEN2K
///*! Get ID of CPU-time clock for thread THREAD_ID.  */
//extern int pthread_getcpuclockid (pthread_t __thread_id,
//              clockid_t *__clock_id);
//#endif
//
//
///*! Functions for handling signals.  */
//#include <bits/sigthread.h>
//
//
///*! Functions for handling process creation and process execution.  */
//
///*! Install handlers to be called when a new process is created with FORK.
//   The PREPARE handler is called in the parent process just before performing
//   FORK. The PARENT handler is called in the parent process just after FORK.
//   The CHILD handler is called in the child process.  Each of the three
//   handlers can be NULL, meaning that no handler needs to be called at that
//   point.
//   PTHREAD_ATFORK can be called several times, in which case the PREPARE
//   handlers are called in LIFO order (last added with PTHREAD_ATFORK,
//   first called before FORK), and the PARENT and CHILD handlers are called
//   in FIFO (first added, first called).  */
//
//extern int pthread_atfork (void (*__prepare) (void),
//            void (*__parent) (void),
//            void (*__child) (void));
//
///*! Terminate all threads in the program except the calling process.
//   Should be called just before invoking one of the exec*() functions.  */
//
//extern void pthread_kill_other_threads_np (void);
//

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif




#endif   /* pthread.h */


//------1---------2---------3---------4---------5---------6---------7---------8
/** @defgroup PTHREAD PTHREAD: POSIX 1003.1c API
@ingroup COMPONENTS
@brief POSIX 1003.1c API

<em>*Documentation and implementation in progress*</em>

Very good references to the pThread API
@see http://developer.apple.com/documentation/Darwin/Reference/ManPages/man3/pthread.3.html
@see http://www.opengroup.org/onlinepubs/009695399/idx/threads.html

@note The following types shall be defined as described in <sys/types.h> :

\code
pthread_attr_t
pthread_barrier_t
pthread_barrierattr_t
pthread_cond_t
pthread_condattr_t
pthread_key_t
pthread_mutex_t
pthread_mutexattr_t
pthread_once_t
pthread_rwlock_t
pthread_rwlockattr_t
pthread_spinlock_t
pthread_t
\endcode

<p><b>Go gack to</b> \ref COMPONENTS</p>

*/

/*!
 * @defgroup CVSLOG_pthread_h pthread_h
 * @ingroup CVSLOG
 *  $Log: pthread.h,v $
 *  Revision 1.23  2007-03-23 20:27:23  ambrmi09
 *  1) Reorganization of ITC into several smaller files
 *  2) Component pthread now supports 3,5,9 and 16 priorities
 *
 *  Revision 1.22  2007-03-08 23:28:35  ambrmi09
 *  Minor changes made in TinKer headers for TiNa code ceneration to work
 *
 *  Revision 1.21  2006-12-12 10:57:05  ambrmi09
 *  This adresses the second part of #1609064
 *
 *  Revision 1.20  2006/12/11 14:41:52  ambrmi09
 *  Solves #1609064 (part1)
 *
 *  Revision 1.19  2006/11/27 22:29:22  ambrmi09
 *  Minor djustments completeing the move of some header files to public and due
 *  to some name clashed with user space naming conventions.
 *
 *  Revision 1.18  2006/11/05 19:06:03  ambrmi09
 *  Buildsystem adjusted to permit configuration of components.
 *  Now when component is enabled it will also be included in the build
 *  (instead of just sanity-tested in the source files).
 *
 *  Also a feature for application sanity is assed. When a header-file is
 *  included in the application, a check against the component it belongs
 *  to will be performed. That way user don't need to rely on run-time
 *  checks and can get feedback much earlier.
 *
 *  Revision 1.17  2006/04/08 10:15:57  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.16.2.1  2006/04/03 20:07:22  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.16  2006/03/24 18:23:43  ambrmi09
 *  Another turn of cosmetics
 *
 *  Revision 1.15  2006/03/24 17:40:18  ambrmi09
 *  Cosmetic details
 *
 *  Revision 1.14  2006/03/24 11:22:54  ambrmi09
 *  - pThreads RW locks implemented (rough aproach - no usage error detection)
 *  - restructuring of the pThread src-files
 *
 *  Revision 1.13  2006/03/19 22:57:53  ambrmi09
 *  First naive implementation of a pthread mutex
 *
 *  Revision 1.12  2006/03/19 12:44:35  ambrmi09
 *  Got rid of many compilation warnings. MSVC amd GCC actually gompiles
 *  without one single warning (yay!). Be aware that ther was a lot of
 *  comparisons between signed/unsigned in ITC. Fetts a bit shaky...
 *
 *  Revision 1.11  2006/03/17 14:18:42  ambrmi09
 *  Finalized pThreads and RT gueues for GNU build-chain
 *
 *  Revision 1.10  2006/03/17 12:20:02  ambrmi09
 *  Major uppdate (5 days hard work)
 *
 *  - Finally tied up all loose ends in the concept. Threads are now
 *  joinable
 *
 *  - Corrected one error: compacting scheduele while cancelling a
 *  threads
 *
 *  - Several new API, mainly concerned with cancelation (corrsp pThread
 *  also)
 *
 *  - Found a nasty bug while creating threads in threads for XC167. TOS is
 *  really a patchy solution ;( This one had to do with the compiler
 *  being fooled by the inline assembly and optimized something that was not
 *  optimizable (saving stack segment got wacked).
 *
 *  - Designed a concurrent qsort test-app. This is good for showing
 *  boss-worker model. Number of threads recoed on XC167 was 50 and on MSVS
 *  more than 150! Interesting to notice was that TinKer creation and
 *  cancelation for threads was much faster than Windows own (20-30 times
 *  faster).
 *
 *  - A MSVC workspace for pThreads-Win32. Good for testing apps
 *  transparency.
 *
 *  - Increased memory on XC167 (phyCore HW). now 32k for stacks and 16k for
 *  malloc. We still lack RAM that is not deployed (pHycore has
 *  128k + 256k physical RAM memory i think). Maximum for
 *  stack is 64k however (type of pointers determine this). If memory is
 *  increased further, we get a TRAP_B saying bad memory interface. Typical
 *  error for config memory issues in DaVe.
 *
 *  Revision 1.9  2006/03/11 14:37:48  ambrmi09
 *  - Replaced printf with printk in in-depth parts of the kernel. This is
 *  to make porting easier since printk can then be mapped to whatever
 *  counsole output ability there is (including none if there isn't any).
 *
 *  - Conditionals for: 1) time ISR latency and 2) clock systimer faliure
 *  introduced. This is because debugging involves stopping the program but
 *  not the clock HW, which will trigger the "trap" as soon as resuming the
 *  program after a BP stop (or step). I.e. inhibit those part's when
 *  debugging (which is probably most of the time). Remeber to re-enable for
 *  "release" version of any application.
 *
 *  - Working on getting rid of all the compilation warnings.
 *
 *  - Detected a new serious bug. If an extra malloc is not executed
 *  *before* the first thread is created that requires a steck  (i.e. the
 *  idle tread sice root allready has a stack), that thread will fail with
 *  an illegal OP-code trap. This has been traced to be due to a faulty
 *  malloc and/or possibly a memory alignement problem. The first block
 *  allocated, will be about 6 positions to high up in the memory map, which
 *  means that sthe total block will not really fit. If that block is the
 *  stack of a thread, those positions will be either the context or the
 *  return adress of that thread (which is bad). I'm concerned about not
 *  detecting this error before, which leads me to believe that this
 *  actually is an alignement issue in malloc and it's anly pure chance
 *  wheather the bug will manifest or not. This is a problem related
 *  to the Keil_XC167 target only.
 *
 *  Revision 1.8  2006/03/05 11:11:24  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.7  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  
 */



