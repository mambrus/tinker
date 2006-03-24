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
@ingroup PTHREAD_SCHED

@brief POSIX 1003.1c API - Scheduling and thread management

The content of this source-file implement thread management. I.e.
starting/stopping, setting attributes of the threads themselves.


For in-depth discussions about this component, see \ref
PTHREAD_SCHED

@see PTHREAD_SYNC
@see PTHREAD

*/
#include <tk_itc.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>
#include <tk_itc.h>
#include "implement_pthread.h"
#include "implement_tk.h"

unsigned long sem_once;

unsigned long tk_pthread_sched( void ){
   unsigned long rc = ERR_OK;
   
   rc = sm_create("SOnc",1,FIFO,&sem_once);
   
   return rc;    	
}
//------1---------2---------3---------4---------5---------6---------7---------8
unsigned long tk_pthread_sched_destruct( void ){
   unsigned long rc = ERR_OK;
   
   rc = sm_delete(sem_once);
   
   return rc;    	

}

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
\returns 0 or a error code

Works exactlly as \ref pthread_create except that it provides TinKer with a name of the thread as well

@note This is not portable. Use the \ref pthread_create instead

@see http://www.opengroup.org/onlinepubs/009695399/functions/pthread_create.html

@todo Chech if the stack concpets fit. Unscertain...
*/
int pthread_create_named_np (
   pthread_t               *thread,
   const pthread_attr_t    *attr,
   void *(*start_routine)  (void *),
   void *                  arg,
   char                    *threadName
){
   thid_t thid;
   struct tcb_t_ *tk_tcb;
   
   if (attr != NULL){           
      thid = tk_create_thread(
         threadName,
         (*attr)->priority,
         start_routine,
         arg,
         (*attr)->stacksize
      );
	  tk_tcb = _tk_specific_tcb(thid);
	  //NOTICE! Uncertain about this line.
      (*attr)->stackaddr = tk_tcb->stack_begin;
      
   }else{
      thid = tk_create_thread(
         threadName,
         TK_MAX_PRIO_LEVELS/2,
         start_routine,
         arg,
         TK_NORMAL_STACK_SIZE
      );
   };
   //Types are the same, but avoid warnings
   *thread=(pthread_t)_tk_specific_tcb(thid);
      
   return 0;
}

int pthread_join (
   pthread_t               thread,
   void**                  arg_return
){
   return tk_join(thread->Thid,arg_return);
}

int pthread_detach (
   pthread_t               thread
){
   return tk_detach(thread->Thid);
}

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Returns the id of the current thread

Do not compare thread id's (i.e. \ref pthread_t) directly to determine
if they are equal. On TinKer you can do that because it's a value based
type, but on other system pthread_t might be a reference. Use \ref
pthread_equal for this purpose to assure code is portable.

@warning Comparing without using \ref pthread_equal since it will not
render in a compilation error, either in situations where TinKer is the
kernel or in reference based \ref pthread_t kernel.

@see http://www.opengroup.org/onlinepubs/009695399/functions/pthread_self.html
@see http://developer.apple.com/documentation/Darwin/Reference/ManPages/man3/pthread_self.3.html#//apple_ref/doc/man/3/pthread_self
@see http://developer.apple.com/documentation/Darwin/Reference/ManPages/man3/pthread_equal.3.html#//apple_ref/doc/man/3/pthread_equal
*/
pthread_t pthread_self (void){
   return _tk_current_tcb();
}

/*!
@brief Compare thread id's with each other.

Compare thread id's with each other.

- API should do this 
- You shouldn't

@see pthread_self
*/
int pthread_equal(pthread_t t1, pthread_t t2){
   return (t1 == t2);
}

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
@see http://www.opengroup.org/onlinepubs/009695399/functions/pthread_once.html
*/
int pthread_once (
   pthread_once_t          *once_control,
   void (*init_routine) (void)
){   
   int            need2run = 0;
   unsigned long  rc = ERR_OK;
   
   if (once_control==NULL){
      return EINVAL;
   }else{
      //Proper way      
      //--- Detect ---
      rc = sm_p(sem_once,WAIT,/*FOREVER*/1000);
      assert(rc==ERR_OK);
      if (once_control->started < 0){
         need2run = 1;
         once_control->started++;
      }
      rc = sm_v(sem_once);
      assert(rc==ERR_OK);
      
      //--- Run if requred ---            
      if (need2run){
            
         init_routine();

         //--- Mark as finished ---            
         rc = sm_p(sem_once,WAIT,/*FOREVER*/1000);
         assert(rc==ERR_OK);

         once_control->done = 1;         
         rc = sm_v(sem_once);
         assert(rc==ERR_OK);
      }
   }
   
   return 0;
}
/*!
@todo: Very stubbed. Make this more complient!
*/
int pthread_cancel (pthread_t thread){
   assert ( tk_delete_thread((thid_t)thread) == TK_OK );
   return 0;
}

int pthread_yield    (void){
   tk_yield();
   return 0;
}

/*!
@todo: tinker tcb needs a policy field. policy not handled.
*/
int pthread_setschedparam (
   pthread_t thread,
	int policy,
   const struct sched_param *param
){
   int y = 0;
   y = tk_change_prio(thread->Thid, param->sched_priority);

   if (y)
      tk_yield();
   
   return 0;
}

int pthread_getschedparam (
   pthread_t thread,
	int *policy,
	struct sched_param *param)
{
   param->sched_priority = thread->Prio;
   return 0;
}



/** @defgroup PTHREAD_SCHED PTHREAD_SCHED: POSIX 1003.1c API - scheduling
@ingroup PTHREAD
@brief POSIX 1003.1c API - Scheduling and thread management 

<em>*Documentation and implementation in progress*</em>

@todo The "once lock" is brutal. It will potentially lock on all accesses, even by threads involved with anuther once_control. Either replace with a rw_lock (TBD) or an array of once locks.

<p><b>Go gack to</b> \ref COMPONENTS</p>
*/

  
/*!
 *  @defgroup CVSLOG_pthread_sched_c pthread_sched_c
 *  @ingroup CVSLOG 
 *  $Log: pthread_sched.c,v $
 *  Revision 1.12  2006-03-24 11:22:56  ambrmi09
 *  - pThreads RW locks implemented (rough aproach - no usage error detection)
 *  - restructuring of the pThread src-files
 *
 *  Revision 1.11  2006/03/19 22:57:54  ambrmi09
 *  First naive implementation of a pthread mutex
 *
 *  Revision 1.10  2006/03/19 12:44:36  ambrmi09
 *  Got rid of many compilation warnings. MSVC amd GCC actually gompiles
 *  without one single warning (yay!). Be aware that ther was a lot of
 *  comparisons between signed/unsigned in ITC. Fetts a bit shaky...
 *
 *  Revision 1.9  2006/03/17 12:20:03  ambrmi09
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
 *  Revision 1.8  2006/03/12 15:08:54  ambrmi09
 *  - Adjusted the source to accomodate the new file structure.
 *
 *  - All build environments uppdated and verified except BC5. For this one
 *  we stumbled across the header-file issue that I've been fearing. Seems
 *  we need to take care of that one after all.
 *
 *  @note The malloc bug still not solved.
 *
 *  Revision 1.7  2006/03/05 11:11:27  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.6  2006/02/28 18:16:55  ambrmi09
 *  - Mainly a ci for the new Workspace structure
 *  - Houwever, found and corrected a bug in mqueue.c (a NULL pointer
 *    assignement)
 *
 *  Revision 1.5  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.4  2006/02/21 22:10:32  ambrmi09
 *  - Added wrapper macro for pthread_create so that posix threads get named in
 *    TinKer (makes post-mortem easier). Very cool solution with a macro...
 *  - Improved post-mortem, the schedule gets dumpt also now
 *  - Wrapper macros for msleep and usleep (temporary)
 *  - Minor stubbing and wrapping of mq_unlink and pthread_cancel
 *  - Added a new test program (t est-posix.c ). This is verifyed to compile and
 *    run on both Linux and TinKer unmodified!
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
 













