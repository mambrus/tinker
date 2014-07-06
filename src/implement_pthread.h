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

@brief Definitions that don't need to be public

Definitions that don't need to be public for the \ref PTHREAD component.
Keeps all the internals out of pthread.h

The following structures are the most important reason for this file to exist:

\code
pthread_t_
pthread_attr_t_
pthread_once_t_
pthread_key_t_
pthread_mutex_t_
pthread_mutexattr_t_
pthread_cond_t_
pthread_condattr_t_
\endcode

For in-depth discussions about this component, see \ref
PTHREAD

@see PTHREAD
@see COMPONENTS

@see http://www.freepascal.org/docs-html/rtl/unixtype/index-3.html
*/

#ifndef _IMPLEMENT_PTHREAD_H
#define _IMPLEMENT_PTHREAD_H
#include "stypes.h"
#include "tk_tuning.h"

#ifndef TK_MAX_THREADS
#error TK_MAX_THREADS not defined. Did you --enable-max_threads=<val> properly?
#endif

/*!
@brief Tinker ID mapping.

@note that we don't allow access to the \ref tk_tcb_t directly.

@see http://www.freepascal.org/docs-html/rtl/unixtype/pthread_t.html
*/

/*
struct pthread_t_ {
   thid_t thid;  //!< Thinker thread identity
};
*/

/*!
Describes the thread attributes. It should be considered an opaque
record, the names of the fields can change anytime. Use the appropriate
functions to set the thread attributes.

@see http://www.freepascal.org/docs-html/rtl/unixtype/pthread_attr_t.html
*/
struct pthread_attr_t_ {
	unsigned long valid;	//
	stack_t stackaddr;	// Used to be a vioid*
	size_t stacksize;	//
	int detachstate;	//
	int priority;		//
};

/*!
TBD

@see http://www.freepascal.org/docs-html/rtl/unixtype/pthread_key_t.html
*/
struct pthread_key_t_ {
	int TBD_THIS_STRUCT;
};

typedef enum { BSINGLE = 0, BCAST } bcast_t;

int _mutex_lock_primitive(pthread_mutex_t * mutex);
int _mutex_unlock_primitive(pthread_mutex_t * mutex, bcast_t bcast);

//#include <semaphore.h>
//typedef enum {
///*
//   * This enumeration represents the state of the thread;
//   * The thread is still "alive" if the numeric value of the
//   * state is greater or equal "PThreadStateRunning".
//   */
//PThreadStateInitial = 0,      /* Thread not running                   */
//PThreadStateRunning,          /* Thread alive & kicking               */
//PThreadStateSuspended,        /* Thread alive but suspended           */
//PThreadStateCanceling,        /* Thread alive but and is              */
//                              /* in the process of terminating        */
//                              /* due to a cancellation request        */
//PThreadStateException,        /* Thread alive but exiting             */
//                              /* due to an exception                  */
//PThreadStateLast
//}
//PThreadState;
//
//
//typedef enum {
///*
//   * This enumeration represents the reason why a thread has
//   * terminated/is terminating.
//   */
//PThreadDemisePeaceful = 0,    /* Death due natural causes     */
//PThreadDemiseCancelled,       /* Death due to user cancel     */
//PThreadDemiseException,       /* Death due to unhandled       */
//                              /* exception                    */
//PThreadDemiseNotDead  /* I'm not dead!                */
//}
//PThreadDemise;
//
//struct pthread_t_ {
//DWORD thread;
//HANDLE threadH;
//PThreadState state;
//PThreadDemise demise;
//void *exitStatus;
//void *parms;
//int ptErrno;
//int detachState;
//int cancelState;
//int cancelType;
//HANDLE cancelEvent;
//#if HAVE_SIGSET_T
//sigset_t sigmask;
//#endif /* HAVE_SIGSET_T */
//int implicit:1;
//void *keys;
//};
//
//
///*
//* Special value to mark attribute objects as valid.
//*/
//#define _PTHREAD_ATTR_VALID ((unsigned long) 0xC4C0FFEE)
//
//struct pthread_attr_t_ {
//unsigned long valid;
//void *stackaddr;
//size_t stacksize;
//int detachstate;
//int priority;
//#if HAVE_SIGSET_T
//sigset_t sigmask;
//#endif /* HAVE_SIGSET_T */
//};
//
//
///*
//* ====================
//* ====================
//* Mutexes and Condition Variables
//* ====================
//* ====================
//*/
//
//#define _PTHREAD_OBJECT_AUTO_INIT ((void *) -1)
//#define _PTHREAD_OBJECT_INVALID   NULL
//
//struct pthread_mutex_t_ {
//HANDLE mutex;
//CRITICAL_SECTION cs;
//};
//
//
//struct pthread_mutexattr_t_ {
//int pshared;
//int forcecs;
//};
//
//
//struct pthread_key_t_ {
//DWORD key;
//void (*destructor) (void *);
//pthread_mutex_t threadsLock;
//void *threads;
//};
//
//
//typedef struct ThreadParms ThreadParms;
//typedef struct ThreadKeyAssoc ThreadKeyAssoc;
//
//struct ThreadParms {
//pthread_t tid;
//void *(*start) (void *);
//void *arg;
//};
//
//
//struct pthread_cond_t_ {
//long waiters;                       /* # waiting threads             */
//pthread_mutex_t waitersLock;        /* Mutex that guards access to
//                                      waiter count                  */
//sem_t sema;                         /* Queue up threads waiting for the
//                                      condition to become signaled  */
//HANDLE waitersDone;                 /* An auto reset event used by the
//                                      broadcast/signal thread to wait
//                                      for the waiting thread(s) to wake
//                                      up and get a chance at the
//                                      semaphore                     */
//int wasBroadcast;                   /* keeps track if we are signaling
//                                      or broadcasting               */
//};
//
//
//struct pthread_condattr_t_ {
//int pshared;
//};
//
//
//struct ThreadKeyAssoc {
///*
//   * Purpose:
//   *      This structure creates an association between a
//   *      thread and a key.
//   *      It is used to implement the implicit invocation
//   *      of a user defined destroy routine for thread
//   *      specific data registered by a user upon exiting a
//   *      thread.
//   *
//   * Attributes:
//   *      lock
//   *              protects access to the rest of the structure
//   *
//   *      thread
//   *              reference to the thread that owns the association.
//   *              As long as this is not NULL, the association remains
//   *              referenced by the pthread_t.
//   *
//   *      key
//   *              reference to the key that owns the association.
//   *              As long as this is not NULL, the association remains
//   *              referenced by the pthread_key_t.
//   *
//   *      nextKey
//   *              The pthread_t->keys attribute is the head of a
//   *              chain of associations that runs through the nextKey
//   *              link. This chain provides the 1 to many relationship
//   *              between a pthread_t and all pthread_key_t on which
//   *              it called pthread_setspecific.
//   *
//   *      nextThread
//   *              The pthread_key_t->threads attribute is the head of
//   *              a chain of assoctiations that runs through the
//   *              nextThreads link. This chain provides the 1 to many
//   *              relationship between a pthread_key_t and all the
//   *              PThreads that have called pthread_setspecific for
//   *              this pthread_key_t.
//   *
//   *
//   * Notes:
//   *      1)      As long as one of the attributes, thread or key, is
//   *              not NULL, the association is being referenced; once
//   *              both are NULL, the association must be released.
//   *
//   *      2)      Under WIN32, an association is only created by
//   *              pthread_setspecific if the user provided a
//   *              destroyRoutine when they created the key.
//   *
//   *
//   */
//pthread_mutex_t lock;
//pthread_t thread;
//pthread_key_t key;
//ThreadKeyAssoc *nextKey;
//ThreadKeyAssoc *nextThread;
//};
//
//
//#ifdef _MSC_VER
///*
//* --------------------------------------------------------------
//* MAKE_SOFTWARE_EXCEPTION
//*      This macro constructs a software exception code following
//*      the same format as the standard Win32 error codes as defined
//*      in WINERROR.H
//*  Values are 32 bit values layed out as follows:
//*
//*   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//*  +---+-+-+-----------------------+-------------------------------+
//*  |Sev|C|R|     Facility          |               Code            |
//*  +---+-+-+-----------------------+-------------------------------+
//*
//* Severity Values:
//*/
//#define SE_SUCCESS              0x00
//#define SE_INFORMATION                0x01
//#define SE_WARNING              0x02
//#define SE_ERROR                0x03
//

#define MAKE_SOFTWARE_EXCEPTION( _severity, _facility, _exception ) \
( (DWORD) ( ( (_severity) << 30 ) |	/* Severity code	*/ \
	   ( 1 << 29 )	|		/* MS=0, User=1		*/ \
	   ( 0 << 28 )	|		/* Reserved		*/ \
	   ( (_facility) << 16 ) |	/* Facility Code	*/ \
	   ( (_exception) <<  0 )	/* Exception Code	*/ \
	   ) )

///*
//* We choose one specific Facility/Error code combination to
//* identify our software exceptions vs. WIN32 exceptions.
//* We store our actual component and error code within
//* the optional information array.
//*/
/*
#define EXCEPTION_PTHREAD_SERVICES	\
   MAKE_SOFTWARE_EXCEPTION( SE_ERROR, \
			      PTHREAD_SERVICES_FACILITY, \
			      PTHREAD_SERVICES_ERROR )

*/
//#define PTHREAD_SERVICES_FACILITY             0xBAD
//#define PTHREAD_SERVICES_ERROR                        0xDEED
//
//#else
//
//#ifdef __cplusplus
//
//class Pthread_exception {};
//
//#else /* __cplusplus */
//
//#warning File __FILE__, Line __LINE__: Cancellation not supported under C.
//
//#endif /* __cplusplus */
//
//#endif /* _MSC_VER */
//
///* Function pointer to TryEnterCriticalSection if it exists; otherwise NULL */
//extern TK_BOOL (WINAPI *_pthread_try_enter_critical_section)(LPCRITICAL_SECTION);
//
///* Declared in global.c */
//extern int _pthread_processInitialized;
//extern pthread_key_t _pthread_selfThreadKey;
//extern pthread_key_t _pthread_cleanupKey;
//extern CRITICAL_SECTION _pthread_mutex_test_init_lock;
//extern CRITICAL_SECTION _pthread_cond_test_init_lock;
//
//
//#ifdef __cplusplus
//extern "C" {
//#endif /* __cplusplus */
//
///*
//* =====================
//* =====================
//* Forward Declarations
//* =====================
//* =====================
//*/
//int _pthread_processInitialize (void);
//
//void _pthread_processTerminate (void);
//
//void _pthread_threadDestroy (pthread_t tid);
//
//void _pthread_cleanupStack (void);
//
//void *_pthread_threadStart (ThreadParms * threadParms);
//
//void _pthread_callUserDestroyRoutines (pthread_t thread);
//
//int _pthread_tkAssocCreate (ThreadKeyAssoc ** assocP,
//                         pthread_t thread,
//                         pthread_key_t key);
//
//void _pthread_tkAssocDestroy (ThreadKeyAssoc * assoc);
//
//int _pthread_sem_timedwait (sem_t * sem,
//                         const struct timespec * abstime);
//
//#ifdef __cplusplus
//}
//#endif /* __cplusplus */
//
//
///*
//* Check for old and new versions of cygwin. See the FAQ file:
//*
//* Question 1 - How do I get pthreads-win32 to link under Cygwin or Mingw32?
//*
//* Patch by Anders Norlander <anorland@hem2.passagen.se>
//*/
//#if defined(__CYGWIN32__) || defined(__CYGWIN__)
//
///*
//* Macro uses args so we can cast start_proc to LPTHREAD_START_ROUTINE
//* in order to avoid warnings because of return type
//*/
//
/*
#define _beginthreadex(security, \
		      stack_size, \
		      start_proc, \
		      arg, \
		      flags, \
		      pid) \
      CreateThread(security, \
		   stack_size, \
		   (LPTHREAD_START_ROUTINE) start_proc, \
		   arg, \
		   flags, \
		   pid)
*/
//#define _endthreadex ExitThread
//
//#endif /* __CYGWIN32__ || __CYGWIN__ */

#endif				/* _IMPLEMENT_PTHREAD_H */

