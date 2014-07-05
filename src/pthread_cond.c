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
@ingroup PTHREAD_SYNC

@brief This implements pThreads conditional variables


For in-depth discussions about this component, see \ref
PTHREAD_SYNC

@see PTHREAD_SCHED
@see PTHREAD

*/

#include <pthread.h>
#include <errno.h>
#include <assert.h>

#include "implement_pthread.h"
#include "implement_tk.h"


//------1---------2---------3---------4---------5---------6---------7---------8
/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_cond_init.html

@todo Stubbed. Impl TBD
*/
int pthread_cond_init (pthread_cond_t * __cond, const pthread_condattr_t *attr){
   assert("Not implemented yet" == 0);
   _PTHREAD_NO_WARN_VAR(__cond);
   _PTHREAD_NO_WARN_VAR(attr);
   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_cond_destroy.html

@todo Stubbed. Impl TBD
*/
int pthread_cond_destroy (pthread_cond_t *__cond){
   assert (__cond->valid);
   assert("Not implemented yet" == 0);
   _PTHREAD_NO_WARN_VAR(__cond);
   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_cond_signal.html

@note BIG FAT NOTE. 1) If not taken yet = recursive. 2) Several listeners = release all => logic handled by phread_condval_attribute (TBD)
*/
int pthread_cond_signal (pthread_cond_t *__cond){
   int y;

   assert (__cond->valid);
   if (__cond->coop_mux == NULL) //No one is using the CV yet. No need to do anything
      return 0;

   // BIG FAT NOTE. 1) If not taken yet = recursive. 2) Several listeners = release all => logic handled by phread_condval_attribute (TBD)
   y = _mutex_unlock_primitive( &(__cond->cv_mux), BSINGLE );  //"Signal" to listener(s).
   if (__cond->cv_mux.owner == NULL){
      //No ownership has been passed either
      assert(__cond->cv_mux.blocked.numb == 0);
      //Make shure that next thread that grabs this CV will block
      __cond->cv_mux.owner = (pthread_t)1;
   }
   if (y)
      pthread_yield();


   //But the coop mutex is still taken. Release that too (note: order is important)
   pthread_mutex_unlock(__cond->coop_mux);  //have faith in that the impl of cond_wait will take the lock again *first thing* when it wakes up


   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_cond_broadcast.html
*/
int pthread_cond_broadcast (pthread_cond_t *__cond){
   int y1,y2;

   assert (__cond->valid);
   if (__cond->coop_mux == NULL) //No one is using the CV yet. No need to do anything
      return 0;

   y1 = _mutex_unlock_primitive( &(__cond->cv_mux), BCAST );  //"Signal" to listener(s).
   if (__cond->cv_mux.blocked.numb == 0){
      //No ownership has been passed either
      //Make shure that next thread that grabs this CV will block
      __cond->cv_mux.owner = (pthread_t)1;
   }

   //But the coop mutex is still taken. Release that too (note: order is important)
   //Also, dont BCAST this one. Let RDY threads compete for the lock. Those who fail will get
   //a chance when the thread released just before each, executes it's mutex_unlock
   y2 = _mutex_unlock_primitive(__cond->coop_mux, BSINGLE);  //have faith in that the impl of cond_wait will take the lock again *first thing* when it wakes up

   if (y1 || y2)
      pthread_yield();

   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_cond_wait.html
*/
int pthread_cond_wait (pthread_cond_t * __cond, pthread_mutex_t *mutex){
   int y;

   assert (__cond->valid);

   __cond->coop_mux = mutex;

   _mutex_unlock_primitive(__cond->coop_mux, BSINGLE);       //Make sure no thread is blocked, preventing the chance to release the CV
                                                             //Note: There must be no context switch between the line above and the next lock-line
   y = _mutex_lock_primitive(&(__cond->cv_mux));              //Would lock here always, even if noone owns the lock - due to init
   assert(y);
   __cond->cv_mux.linkOf    = _PBON_CONDVAR;
   __cond->cv_mux.link.cond = __cond;
   pthread_yield();

   pthread_mutex_lock(__cond->coop_mux);
   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_cond_timedwait.html

@todo Stubbed. Impl TBD
*/
int pthread_cond_timedwait (pthread_cond_t * __cond, pthread_mutex_t *mutex, const struct timespec *abstime){
   assert (__cond->valid);
   assert("Not implemented yet" == 0);
   _PTHREAD_NO_WARN_VAR(__cond);
   _PTHREAD_NO_WARN_VAR(mutex);
   _PTHREAD_NO_WARN_VAR(abstime);
   return 0;
}

//------1---------2---------3---------4---------5---------6---------7---------8


/*!
 *  @defgroup CVSLOG_pthread_cond_c pthread_cond_c
 *  @ingroup CVSLOG
 *  $Log: pthread_cond.c,v $
 *  Revision 1.4  2006-04-08 10:16:01  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.3.2.1  2006/04/03 20:07:27  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.3  2006/03/24 18:23:43  ambrmi09
 *  Another turn of cosmetics
 *
 *  Revision 1.2  2006/03/24 17:40:19  ambrmi09
 *  Cosmetic details
 *
 *  Revision 1.1  2006/03/24 11:22:56  ambrmi09
 *  - pThreads RW locks implemented (rough aproach - no usage error detection)
 *  - restructuring of the pThread src-files
 *
 *
 *******************************************************************/














