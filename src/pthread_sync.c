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

@brief POSIX 1003.1c API - pThread low level synchronisation primitives

The content of this source-file implement basic thread syncronisation. I.e. 
the common mechanism used by mutex, cond-vars rw-locks e.t.a,

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
unsigned long tk_pthread_sync( void ){
   return ERR_OK;       
}

unsigned long tk_pthread_sync_destruct( void ){
   return ERR_OK;
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
The mutex lock primitive.

This function will change the state in the scheduler, but it will not yield. 
Instead it returns a \b recommendation to yield or not.
*/
int _mutex_lock_primitive (pthread_mutex_t *mutex){
   int rc = 0;
   
   if (mutex->owner == NULL){
      mutex->owner = pthread_self();
      mutex->blocked.numb = 0;
   }else{
      pthread_t self;
      self = pthread_self();
      self->bOnId.kind = BON_PMUTEX;
      self->bOnId.entity.mutex = mutex;
      self->state |= QUEUE;

      mutex->linkOf=_PBON_NOLINK;
      mutex->link.dummy=NULL;
      mutex->blocked.thread[mutex->blocked.numb] = self;
      mutex->blocked.numb++;

      rc = 1;  //Schedule state has chanced, yield recommended      
   }
   return rc;
}

/*!
The mutex unlock primitive.

This function will change the state in the scheduler, but it will not yield. 
Instead it returns a \b recommendation to yield or not.
*/
int _mutex_unlock_primitive (pthread_mutex_t *mutex, bcast_t bcast){
   int rc = 0;
   pthread_t self = pthread_self();
   unsigned prio = TK_MAX_PRIO_LEVELS;
   int i;
/*
   if ( !pthread_equal(mutex->owner,self) )
      return 1;
*/
   if ( mutex->owner == NULL ){
      //refuse to use
      assert(mutex->blocked.numb == 0);
      return 0;
   }

   if (mutex->blocked.numb == 0){
      //No one to unblock
      mutex->owner = NULL;
      return 0;   
   }
   
   rc = 1;  //Schedule state has chanced, yield recomended      
   //One ore more blocked, yeild recomended.

   if (bcast){
      //Release all, let despatcher select in which order to run.
      for (i=0; i<mutex->blocked.numb;i++){
         mutex->blocked.thread[i]->bOnId.kind         =     BON_SCHED;
         mutex->blocked.thread[i]->bOnId.entity.tcb   =     NULL;
         mutex->blocked.thread[i]->state             &=    ~QUEUE;
         mutex->blocked.thread[i]->wakeupEvent        =     E_ITC;
      }
      mutex->blocked.numb=0;
      
   }else{
      pthread_t newOwner;   
      int j;
      
      for (i=0; i<mutex->blocked.numb;i++){
         //Find the one first in order with the highest prio (PRIO then FIFO)
         if (mutex->blocked.thread[i]->Prio < prio){
            j = i;
            prio = mutex->blocked.thread[i]->Prio;
         }
      }
      //The new owner is selected. Release him only, and adjust new mutext data.
      mutex->owner = newOwner = mutex->blocked.thread[j];

      //Compress the blocked list
      for (i=j; i<mutex->blocked.numb; i++){
         mutex->blocked.thread[i]=mutex->blocked.thread[i+1];
      }

      mutex->blocked.numb--;

      newOwner->bOnId.kind = BON_SCHED;
      newOwner->bOnId.entity.tcb = NULL;
      newOwner->state &= ~QUEUE;
      newOwner->wakeupEvent = E_ITC;
   }

   return rc;
}

//------1---------2---------3---------4---------5---------6---------7---------8

/** @defgroup PTHREAD_SYNC PTHREAD_SYNC: POSIX 1003.1c API - Thread synchronisation
@ingroup PTHREAD
@brief POSIX 1003.1c API - Thread synchronisation

Syncronisation between threads, i.e. 

- Mutexes and attribute handling thereof
- Event handling
- Timout event handling - special case of the above

<p><b>Go gack to</b> \ref COMPONENTS</p>

*/

  
/*!
 *  @defgroup CVSLOG_pthread_sync_c pthread_sync_c
 *  @ingroup CVSLOG
 *  $Log: pthread_sync.c,v $
 *  Revision 1.9  2006-04-08 10:16:01  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.8.2.1  2006/04/03 20:07:28  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.8  2006/03/24 18:23:45  ambrmi09
 *  Another turn of cosmetics
 *
 *  Revision 1.7  2006/03/24 11:22:56  ambrmi09
 *  - pThreads RW locks implemented (rough aproach - no usage error detection)
 *  - restructuring of the pThread src-files
 *
 *  Revision 1.6  2006/03/19 22:57:54  ambrmi09
 *  First naive implementation of a pthread mutex
 *
 *  Revision 1.5  2006/03/05 11:11:27  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.4  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.3  2006/02/20 19:17:14  ambrmi09
 *  - Made the errno variable thread specific (each thread has it's own)
 *  - Hid the details of using errno so that setting and reading it looks
 *    like using a normal variable
 *  - Extracted some stuff from tk.h that doesn't need to be public
 *  - Implemented perros and strerror including a storage with all the error
 *    strings (will go into NV ROM on a embedded system).
 *
 *  Revision 1.2  2006/02/20 15:22:01  ambrmi09
 *  Documentation stuff. No code changes.
 *
 *  Revision 1.1  2006/02/19 22:12:07  ambrmi09
 *  CO of missed files
 *
 *  
 *******************************************************************/
 













