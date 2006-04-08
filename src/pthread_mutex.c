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

@brief This implements pThreads mutexes


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
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_mutex_init.html
@todo Stubbed. Impl TBD
*/
int pthread_mutex_init (
   pthread_mutex_t *mutex,
   const pthread_mutexattr_t *attr
){
   assert (mutex->valid);
   assert("Not implemented yet" == 0); 
   _PTHREAD_NO_WARN_VAR(mutex);
   _PTHREAD_NO_WARN_VAR(attr);
   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_mutex_destroy.html
@todo Stubbed. Impl TBD
*/
int pthread_mutex_destroy(pthread_mutex_t *mutex){
   assert (mutex->valid);
   assert("Not implemented yet" == 0); 
   _PTHREAD_NO_WARN_VAR(mutex);
   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_mutex_trylock.html

@todo Stubbed. Impl TBD
*/
int pthread_mutex_trylock (pthread_mutex_t *mutex){
   assert (mutex->valid);
   assert("Not implemented yet" == 0);
   _PTHREAD_NO_WARN_VAR(mutex); 
   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_mutex_lock.html
*/
int pthread_mutex_lock (pthread_mutex_t *mutex){

   assert (mutex->valid);
   if ( _mutex_lock_primitive(mutex) ) //State in schedule has changed. Let dispatcher determine who should really run
      pthread_yield();
   
   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_mutex_unlock.html
*/
int pthread_mutex_unlock (pthread_mutex_t *mutex){

   assert (mutex->valid);
   if ( _mutex_unlock_primitive(mutex, BSINGLE) ) //State in schedule has changed. Let dispatcher determine who should really run
      pthread_yield();

   return 0;
}


/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_mutex_timedlock.html

@todo Stubbed. Impl TBD
*/
int pthread_mutex_timedlock(
   pthread_mutex_t *mutex,
   const struct timespec *abs_timeout
){
   assert (mutex->valid);
   assert("Not implemented yet" == 0);
   _PTHREAD_NO_WARN_VAR(mutex);
   _PTHREAD_NO_WARN_VAR(abs_timeout); 
   return 0;
}

//------1---------2---------3---------4---------5---------6---------7---------8

  
/*!
 *  @defgroup CVSLOG_pthread_mutex_c pthread_mutex_c
 *  @ingroup CVSLOG
 *  $Log: pthread_mutex.c,v $
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
 













