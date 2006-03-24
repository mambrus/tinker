/***************************************************************************
 *   Copyright (C) 2006 by Michale Ambrus                                  *
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

@brief This implements pThreads RW locks. 

This is a text-book implementation of pThreads RW locks

@note RW locks did \b not used to be part of the POSIX 1003.1c (pThread) 
standard. Please make sure that you will not get into portability problems 
if you plan to any older implementation of the library later.

The RWL implementation is based entirely on cond vars and mutexes and is 
actual not dependant of TinKer. 

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
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_rwlock_rdlock.html
*/
int pthread_rwlock_rdlock (pthread_rwlock_t *__rwlock){
   int blocked_writers_granted = 0;
   int i;

   assert (__rwlock->valid);
   pthread_mutex_lock(&(__rwlock->mx_self));
   
   //Calculate number of blocked writers having more right to run (i.e. with prio equal or above my own)
   if (__rwlock->blocked_writers){
      for (i=pthread_self()->Prio; i>= 0; i--){
         blocked_writers_granted += __rwlock->bwriters_prio[i];
      }
   }else 
      blocked_writers_granted = 0;

   while ( __rwlock->writers_writing || blocked_writers_granted){
      if (blocked_writers_granted && !__rwlock->writers_writing && !__rwlock->readers_reading){
         //No one would wake this writer up (one or several), so we have to do it (special case).
         pthread_cond_signal(&(__rwlock->writers_lock));
      }
   
      __rwlock->blocked_readers++; //Note: This is a lie for a short while *if* thread is actually not blocking on CV below. Doesn't matter for logic since safe by mutex, but might confuse you while debugging
      pthread_cond_wait(&(__rwlock->readers_lock),&(__rwlock->mx_self));
      __rwlock->blocked_readers--;
      
      //Calculate number of blocked writers again - status might have changed
      if (__rwlock->blocked_writers){
         for (i=pthread_self()->Prio; i<_PTHREAD_PRIOS;i++){
            blocked_writers_granted += __rwlock->bwriters_prio[i];
         }
      }else 
         blocked_writers_granted = 0;
   }
   __rwlock->readers_reading++;
   pthread_mutex_unlock(&(__rwlock->mx_self));
   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_rwlock_wrlock.html
*/
int pthread_rwlock_wrlock (pthread_rwlock_t *__rwlock){
   assert (__rwlock->valid);
   pthread_mutex_lock(&(__rwlock->mx_self));
   while ( __rwlock->readers_reading  || __rwlock->writers_writing ){
      //I.e. I'm going to block - outer mutex keeps this without a question. i.e. RW variables are safe to update
      __rwlock->blocked_writers++; //Note: This is a lie for a short while *if* thread is actually not blocking on CV below. Doesn't matter for logic since safe by mutex, but might confuse you while debugging
      __rwlock->bwriters_prio[pthread_self()->Prio]++;
      
      pthread_cond_wait(&(__rwlock->writers_lock),&(__rwlock->mx_self));
      
      __rwlock->blocked_writers--; //Still safe to update - still holding mutex
      __rwlock->bwriters_prio[pthread_self()->Prio]--;
   }
   __rwlock->writers_writing++;
   assert(__rwlock->writers_writing<2);
   pthread_mutex_unlock(&(__rwlock->mx_self));

   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_rwlock_unlock.html
*/
int pthread_rwlock_unlock (pthread_rwlock_t *__rwlock){
   int blocked_writers_granted = 0;
   int i;
   
   assert (__rwlock->valid);   
   
   pthread_mutex_lock(&(__rwlock->mx_self));
   
   if ( !__rwlock->readers_reading && !__rwlock->writers_writing){
      pthread_mutex_unlock(&(__rwlock->mx_self));
      return EINVAL;
   }else{
      assert(__rwlock->writers_writing<2);
   
      if (__rwlock->writers_writing){
         //Since there can only be one writer writing, it means it must be me (and that I'm a writer)
         __rwlock->writers_writing = 0;
         if (__rwlock->blocked_writers){
            //I've blocked at least one other writer - 
            //standard says to release blocked writes before blocked readers if of same prio or higher
            //Calculate number of blocked writers having more right to run (i.e. with prio equal or above my own)
            if (__rwlock->blocked_writers){
               for (i=pthread_self()->Prio; i>= 0; i--){
                  blocked_writers_granted += __rwlock->bwriters_prio[i];
               }
            }else 
               blocked_writers_granted = 0;

           
           if (blocked_writers_granted){
              pthread_cond_signal(&(__rwlock->writers_lock));
           }else{
           //Writer(s) are of lower prio than my self, so we don't know fore sure if the should run
           //Instead release readers

           //Note: The blocked writer(/writers) might in fact be of lower prio than 
           //any blocked reader, so we'll break the standard, but *only for one 
           //iteration* - It will sort it self out during next read lock

              if (__rwlock->blocked_readers){
                 pthread_cond_broadcast(&(__rwlock->readers_lock));
              }else{
                 //No readers to release, so we need to release the blocked writer(/writes) anyway                 
                 pthread_cond_signal(&(__rwlock->writers_lock));
              }
           }
            
         }else{
            //Only readers are blocked (might in fact be no readers at all but we don't care)
            //Release'em all and let them compete about the lock (schedule will deside who tries first - i.e. who will in fact win).
            pthread_cond_broadcast(&(__rwlock->readers_lock));
         }
      }else{
         //Since I'm not a writer, I must be a reader 
         __rwlock->readers_reading--;
         //It's not possible for me to have blocked other readers - but there might be a writer to release
         pthread_cond_signal(&(__rwlock->readers_lock));
      }
   }
   pthread_mutex_unlock(&(__rwlock->mx_self));
   return 0;
}

//------1---------2---------3---------4---------5---------6---------7---------8
//------1---------2---------3---------4---------5---------6---------7---------8

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_rwlock_init.html

@todo Stubbed. Impl TBD
*/
int pthread_rwlock_init (pthread_rwlock_t *__rwlock, const pthread_rwlockattr_t *attr){
   assert("Not implemented yet" == 0); 
   _PTHREAD_NO_WARN_VAR(__rwlock);
   _PTHREAD_NO_WARN_VAR(attr);
   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_rwlock_destroy.html

@todo Stubbed. Impl TBD
*/
int pthread_rwlock_destroy (pthread_rwlock_t *__rwlock){
   assert (__rwlock->valid);
   assert("Not implemented yet" == 0);
   _PTHREAD_NO_WARN_VAR(__rwlock); 
   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_rwlock_tryrdlock.html

@todo Stubbed. Impl TBD
*/
int pthread_rwlock_tryrdlock (pthread_rwlock_t *__rwlock){
   assert("Not implemented yet" == NULL);
   _PTHREAD_NO_WARN_VAR(__rwlock);
   assert (__rwlock->valid);
   return 0;
}


/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_rwlock_timedrdlock.html

@todo Stubbed. Impl TBD
*/
int pthread_rwlock_timedrdlock (pthread_rwlock_t * __rwlock, const struct timespec *abs_timeout){
   assert("Not implemented yet" == NULL);
   _PTHREAD_NO_WARN_VAR(__rwlock);
   _PTHREAD_NO_WARN_VAR(abs_timeout);
   assert (__rwlock->valid);
   return 0;
}

 

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_rwlock_wrlock.html

@todo Stubbed. Impl TBD
*/
int pthread_rwlock_trywrlock (pthread_rwlock_t *__rwlock){
   assert("Not implemented yet" == NULL);
   _PTHREAD_NO_WARN_VAR(__rwlock);
   assert (__rwlock->valid);
   return 0;
}


/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_rwlock_timedwrlock.html

@todo Stubbed. Impl TBD
*/
int pthread_rwlock_timedwrlock (pthread_rwlock_t * __rwlock, const struct timespec *abs_timeout){
   assert("Not implemented yet" == NULL);
   _PTHREAD_NO_WARN_VAR(__rwlock);
   _PTHREAD_NO_WARN_VAR(abs_timeout);
   assert (__rwlock->valid);
   return 0;
}

//------1---------2---------3---------4---------5---------6---------7---------8

  
/*!
 *  @defgroup CVSLOG_pthread_rwlock_c pthread_rwlock_c
 *  @ingroup CVSLOG
 *  $Log: pthread_rwlock.c,v $
 *  Revision 1.3  2006-03-24 18:23:44  ambrmi09
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
 













