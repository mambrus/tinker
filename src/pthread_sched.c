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
#include <tk_ipc.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>
#include <tk_ipc.h>
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
   unsigned int thid;
   struct tcb_t *tk_tcb;
   
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
         0,
         TK_NORMAL_STACK_SIZE
      );
   };
   //Types are the same, but avoid warnings
   *thread=(pthread_t)thid;
      
   return 0;
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
   return (pthread_t)tk_thread_id();
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
   struct tcb_t  *tcb;
   int            need2run = 0;
   unsigned long  rc = ERR_OK;
   
   if (once_control==NULL){
      //Global once control (TinKer native way).
	  //NOTICE! This branch is never tested.
   
      tcb = _tk_current_tcb();
   
      //TK_CLI();
         if (tcb->init_funct == NULL)
            need2run = 1;
      //TK_STI();
   
      if (need2run){
         tcb->init_funct = init_routine;
         init_routine();
      }
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

int pthread_cancel (pthread_t thread){
   assert ( tk_delete_thread((unsigned int)thread) == TK_OK );
}



/** @defgroup PTHREAD_SCHED PTHREAD_SCHED: POSIX 1003.1c API - scheduling
@ingroup PTHREAD
@brief POSIX 1003.1c API - Scheduling and thread management 

<em>*Documentation and implementation in progress*</em>

@todo The "once lock" is brutal. It will potentially lock on all accesses, even by threads involved with anuther once_control. Either replace with a rw_lock (TBD) or an array of once locks.

<p><b>Go gack to</b> \ref COMPONENTS</p>
*/

  
/*! 
 * @ingroup CVSLOG 
 *  $Log: pthread_sched.c,v $
 *  Revision 1.6  2006-02-28 18:16:55  ambrmi09
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
 













