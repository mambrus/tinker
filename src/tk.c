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
@ingroup SCHED

@brief TinKer kernel life"

@note This is a mandatory file as part of the \ref SCHED component

For in-depth discussions about this component, see \ref
SCHED

@see COMPONENTS
*/

/*- include files **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

#include <tk.h>
#include "context.h"
#include "tk_tuning.h"
#include "implement_tk.h"

#ifndef printk
#   error "Tinker needs a printk to be implemented for this target"
#endif


/*!
@name \COMPONENTS headerfiles

Included conditionally to avoid complication if work is in progress in
any of them.   errno.h

@see COMPONENTS
*/
//@{
#if defined(TK_COMP_KMEM) && (TK_COMP_KMEM==1)
   #include <tk_mem.h>
#endif

#if defined(TK_COMP_ITC) && (TK_COMP_ITC==1)
   #include <tk_itc.h>
   #include <tk_sysqueues.h>
#endif

#if defined(TK_COMP_PTIME) && (TK_COMP_PTIMER==1)
   #ifndef TK_COMP_PTHREAD
      #error Component PTIMER is dependant of PTHREAD
   #endif
   #ifndef TK_COMP_POSIX_RT
      #error Component PTIMER is dependant of POSIX_RT
   #endif
   #include <tk_ptimer.h>
#endif

#if defined(TK_COMP_PTHREAD) && (TK_COMP_PTHREAD==1)
   #ifndef TK_COMP_ITC
      #error Component POSIX_RT is dependant of ITC
   #endif
   #include <pthread.h>
#endif
#undef main

#if defined(TK_COMP_POSIX_RT) && (TK_COMP_POSIX_RT==1)
   #ifndef TK_COMP_PTHREAD
      #error Component POSIX_RT is dependant of PTHREAD
   #endif
   #ifndef TK_COMP_ITC
      #error Component POSIX_RT is dependant of ITC
   #endif
   #include <semaphore.h>
   #include <mqueue.h>
#endif

#if defined (TK_SYSTEM) && (TK_SYSTEM == __SYS_HIXS__)
   #include <tinker/hixs.h>
#endif

#if defined(TK_COMP_FILESYS) && (TK_COMP_FILESYS==1)
   #if (!(TK_SYSTEM == __SYS_HIXS__))
      #error Component TK_COMP_FILESYS is only available for HIXS adapted GNU targets
   #endif
   #include <filesys/filesys.h>
#endif

#ifndef MARKUP_STACKS_ON_START
   #if TK_HOSTED
      #define MARKUP_STACKS_ON_START __tk_no
   #else
      #define MARKUP_STACKS_ON_START __tk_yes
   #endif
#endif

//@}

/*- internal functions **/
void *_tk_destructor( void *retval );
void *_tk_idle( void *foo );
void  _tk_detach_children(thid_t thid);

void _tk_half_switch             (                             thid_t,thid_t);
void _tk_constructor             (start_func_f, void*, thid_t               );
void _tk_construct_thread_scope  (start_func_f, void*, thid_t               );


/*- public data **/

 /*- private data **/

int __tk_IntFlagCntr;  /*!< Increased +1 for each pending interrupt that's
                         not finished (ISR depth counter) */

/*- public functions **/

/*- private functions **/


/*!
@brief The main storage of all TCB

@note The index in this array \b is the thread ID.

This is not perfect. PiD will be reused when more than TK_MAX_THREADS has
been used. I would have wanted a Third range that is much bigger and a lookup-
table (gives greater uniqueness - specially useful for \ref ITC) but that
is costly (I think)...

@note that this is only the pool of TCB's, the schedule itself is \ref
__tk_schedule

coord_t lookUpTable[TK_MAX_THREADS];

@todo Make this static (non available directly by others. Fore using API
(additional needed to be implemented). Important for preempt-able mode that
components use access functions instead (locking issue).

*/
    struct tcb_t_ __tk_threadPool[TK_MAX_THREADS];


/*!
@brief The two dimensional dispatch-schedule

This two dimensional matrix constitute a schedule (imagine a real life
daily schedule if you like) that determines how exactly each thread is
to be handled by the dispatcher on each iteration.

Matrix layout
- First index (X) is the priority
- Second index (Y) is a list of treads at that priority (in various states,
  i.e. both "running", "blocked" and whatnot).


@todo Make this static (non available directly by others. Fore useing API
(additional needed to be implemented). Important for preemtable mode that
components use acces functions instead (locking issue).

*/
    thid_t __tk_schedule[TK_MAX_PRIO_LEVELS][TK_MAX_THREADS_AT_PRIO];

/*!
A index to the schedule. Contains information about how many threads are in
the schedule at each lvl and which index is currently executing (important
for load balancing and RR)

@todo Make this static (non available directly by others. Fore useing API
(additional needed to be implemented). Important for preemtable mode that
components use acces functions instead (locking issue).

*/
    struct stat_t __tk_roundrobin_prio_idxs[TK_MAX_PRIO_LEVELS];


    thid_t __tk_active_thread;      /*!< Deliberately left uninitialized to
                                      support post mortem analysis*/
    #define active_thread __tk_active_thread
    thid_t __tk_thread_to_run   = 0;
    thid_t __tk_procs_in_use    = 0;
    thid_t __tk_proc_idx;           /*!< Points at the last TCB created in
                                      the \ref __tk_threadPool pool*/
static  thid_t idle_Thid;           /*!< Idle_Thid must be known, therefor
                                      public in this module (file)*/


/*!
Assign a value to this function pointer in your app, and you can break into
the kernel before it boots.  Great tool for connecting other monitors
(syscall monitors e.t.a.). Note that this function pointer is initially and
intentionally zero, and non-static. Functionality relies on that .sdata gets
initialized properly. Note that the kernel will call this pointer as the
first thing it does. If the pointer is not or statically initialized by the
application, all sorts of bad things will happen.
*/

        init_func_f boot_hook;


/*!
Manages the boot_hook. Not all systems will support calls even to null
pointers (even though this is in most cases not hard to accomplish).
*/
void _b_hook(void *caller){
   if (boot_hook!=0)
      boot_hook(caller);
}

void *_tk_idle( void *foo ){       //!< idle loop (non public)
   TK_NO_WARN_ARG(foo);
   while (TK_TRUE){
      tk_yield();
   }
}

void tk_delete_kernel( void ){
      tk_delete_thread(idle_Thid);
}

thid_t tk_thread_id( void ){
   return(__tk_active_thread);
}

/*!
@ingroup kernel_glue

@brief Gives a way to enter my own threads TCB

This gets the current threads internal thread control block (TCB). Function
is "public" but is <b>not</b> meant to be used by application developers,
rather as a way for different layers of TinKer to interact.

@note <b>For TinKer internal use only</b>

*/
tk_tcb_t *_tk_current_tcb( void ){
   return(&__tk_threadPool[__tk_active_thread]);
}

/*!
@ingroup kernel_glue

Same as tk_current_tcb but gets info for a specific thread
*/

tk_tcb_t *_tk_specific_tcb( thid_t id ){
   return(&__tk_threadPool[id]);
}

/*!
@ingroup kernel_glue

This function will give you access to the current threads internal errno
variable.

@see errno
*/
int *_tk_errno(){
   return &(__tk_threadPool[__tk_active_thread]._errno_);
}

/*!
@ingroup kernel

Creates the kernel. This is TinKers "init" funtion. All targets need to run
this once, but only once.

@todo <b>Figure out why the malloc(1) needs to be there.</b> I susbect eiter
a byte-boundary probmem, a XC167 specific pointer prob (maybe in conjunction
with the stacp pointer types), or a error in malloc itself
*/

#define TSTSZ 0x10
static const char testPatt[TSTSZ] = "TinKer is king!";
void tk_create_kernel( void ){
   int i,j;
   char *testArea;

   TK_NO_WARN_ARG(testArea);

   /*
   Detect if a kernel is running amok.
   */

   /* PLEEEEEEEEEEASE REMEBER TO UNCOMMENT THIS LATER!
   testArea = malloc(TSTSZ);
   if (strncmp(testPatt,testArea,TSTSZ) == 0){
      printk(("Error: Kernel running amok detected\n"));
      printk(("Broken thread was ID=%d (name=\"%s\"))\n",\
         __tk_active_thread,__tk_threadPool[__tk_active_thread].name);
      memset (testArea, '\0', TSTSZ);     //Clear area then wait for reset
      tk_exit(TC_AMOK);
   }else{
      strncpy(testArea,testPatt,TSTSZ);
   }
   */

   for (i=0; i<TK_MAX_THREADS; i++){
      __tk_threadPool[i].state                        = ZOMBI;
      __tk_threadPool[i].bOnId.kind                   = BON_SCHED;
      __tk_threadPool[i].bOnId.entity.tcb             = NULL;
      __tk_threadPool[i].wakeuptime                   = 0;
      __tk_threadPool[i].valid                        = TK_FALSE;
      __tk_threadPool[i].name[TK_THREAD_NAME_LEN]     = 0;
      __tk_threadPool[i].Gid                          = 0;
      __tk_threadPool[i].Thid                         = 0;
      __tk_threadPool[i].noChilds                     = 0;
      __tk_threadPool[i]._errno_                      = 0;
      __tk_threadPool[i].stack_size                   = 0;
      STACK_PTR(__tk_threadPool[i].stack_begin)       = NULL;
      STACK_PTR(__tk_threadPool[i].curr_sp)           = NULL;
      __tk_threadPool[i].stack_crc                    = 0;
      __tk_threadPool[i].wakeupEvent                  = E_NONE;
      __tk_threadPool[i].retval                       = (void*)0;

/*
      __tk_threadPool[i].start_funct                  = NULL;
      __tk_threadPool[i].init_funct                   = NULL;
      __tk_threadPool[i].prmtRetAddr                  = NULL;
*/
   }
   //The Root thread is already created but must be registered
   __tk_threadPool[0].state = READY;
   __tk_threadPool[0].valid = TK_TRUE;
   strcpy(__tk_threadPool[0].name,"tk_root");
   __tk_procs_in_use = 1;
   __tk_proc_idx = 0;

   //Clear the tables (before creating idle - thread)
   for (i=0; i<TK_MAX_PRIO_LEVELS; i++){
      //Clear the help table
      __tk_roundrobin_prio_idxs[i].procs_at_prio = 0;
      __tk_roundrobin_prio_idxs[i].curr_idx = 0;
      for (j=0; j<TK_MAX_THREADS_AT_PRIO; j++){
         //init the table to root everywhere
         __tk_schedule[i][j]=0;
      }
   }

   //Create a Idle thread, whose sole purpose is to burn up time
   //when nobody else is running
   idle_Thid = tk_create_thread("idle",TK_MAX_PRIO_LEVELS - 1,_tk_idle,(void*)NULL,TK_MINIMUM_STACK_SIZE);

   //IdleProc must like root be owned by itself
   __tk_threadPool[__tk_threadPool[idle_Thid].Gid].noChilds--;

   //Awkward way to say that root has created one process less than it has
   __tk_threadPool[idle_Thid].Gid = __tk_threadPool[idle_Thid].Thid;

   //Idle owned by it self
   //Init schedule tables
   //Put the root in the schedule
   __tk_roundrobin_prio_idxs[0].procs_at_prio = 1;
   __tk_schedule[0][0]=0;

   //Put idle thread in schedule at lowest prio
   /*
   __tk_roundrobin_prio_idxs[TK_MAX_PRIO_LEVELS - 1].procs_at_prio = 1;
   theScedule[TK_MAX_PRIO_LEVELS - 1][0]=idle_Thid;
   */ /* NOT NEEDED, DONE ALREADY */
}

/*!

This function is entered as a result of a ret instruction from a thread.
On X86 EAX is passed as the return value.


Some systems don't like that we delete the stack while were using it.
We need either borrow another stack or let another thread do the actual
canceling.

The way TinKer does it is that it marks the thread as ZOMBI, detaches all
it's children and tries to detach it's parent (i.e. potentially also
awakening the parent). Then it goes to sleep of death and lets the rest of
the system figure out how and when to actually cancel it.

This seemingly complicated process of dieing has it's reasons:

- Wee need to keep threads TCB active for any joiners. In the TCB there
   are still important logic and also the joiners might need the threads
   return value.
- In OS hosted situations (kernel in kernel), another thread or process from
   the host OS could preempt us and claim the memory we just released as our
   stack. I.e. the thread to cancel it's own stack is a bit \e "bad" ;)
- In fully preemptable mode, TinKer itself could launch a thread that does
   the operation just mentioned above.

In either case, this is more complicated. But also much more robust.

*/

void *_tk_destructor( void *retval ){

   #ifdef DEBUG
   printk(("Dieing thread [id=%d] is returning! Return value is: 0x%lX\n\n",
      __tk_active_thread,retval));
   #endif

   __tk_threadPool[__tk_active_thread].state  = ZOMBI;
   __tk_threadPool[__tk_active_thread].retval = (void*)retval;
   _tk_detach_children(__tk_active_thread);

   /*Finalize the destruction*/
   //tk_delete_thread(__tk_active_thread);
   //tk_yield();

   while (TK_TRUE){
      tk_msleep(1000);
      #ifdef DEBUG
      printk(("Dead thread [id=%d] still waiting for Nirvana! \n",__tk_active_thread));
      #endif
   }
   //return(0);
}

/*!
@ingroup kernel

Delete a thread

@note be careful about killing running threads. Any non-kernel resources
allocated will not be freed. Always prepare a controlled exit of a thread.
*/
int tk_delete_thread(
   thid_t Thid              //!< Threads identity
){
   unsigned int Prio,Idx,i;
   int bury_it = TK_TRUE;

   if ( __tk_threadPool[Thid].valid == TK_FALSE )  {
      //The process you are trying to delete does not exist
      return(TK_ERROR);
   }


   if (!(__tk_threadPool[Thid].state & ZOMBI)){
       /* If not self terminated, but killed explicitly - we need to perform
        * some duties*/

      __tk_threadPool[Thid].state = ZOMBI;
      _tk_detach_children(Thid);
      bury_it = _tk_try_detach_parent(Thid, TK_FALSE);
   }

   if (bury_it){   //Can we bury the damned thing allready? or do we have to try another round?
      Prio = __tk_threadPool[Thid].Prio;
      Idx = __tk_threadPool[Thid].Idx;
      assert(__tk_schedule[Prio][Idx] == Thid);
      __tk_procs_in_use--;
      //Take away the process from __tk_schedule and compress gap at prio
      for(i=Idx; i < __tk_roundrobin_prio_idxs[Prio].procs_at_prio; i++){
         __tk_schedule[Prio][i] = __tk_schedule[Prio][i+1];
         __tk_threadPool[__tk_schedule[Prio][i]].Idx = i;
      }

      //#if DEBUG
      // not needed, could cause problems if procs last..
      //__tk_schedule[Prio][i] = 0;
      //#endif

      //Take away the process fom __tk_roundrobin_prio_idxs
      __tk_roundrobin_prio_idxs[Prio].procs_at_prio --;
      if (__tk_roundrobin_prio_idxs[Prio].curr_idx >=
         __tk_roundrobin_prio_idxs[Prio].procs_at_prio)
      {
         __tk_roundrobin_prio_idxs[Prio].curr_idx = 0;
      }
      //Make it final - remove it's primary resource: it's stack
      stalloc_free( STACK_PTR(__tk_threadPool[Thid].stack_begin) );

      __tk_threadPool[Thid].valid = TK_FALSE;
   }

   return(TK_OK);
}

int tk_detach(
   thid_t Thid              //!< Threads identity
){
   if ( __tk_threadPool[Thid].valid != TK_TRUE)
      return ESRCH;

   if ( __tk_threadPool[Thid].Gid != -1)
      return EINVAL;

   _tk_try_detach_parent(Thid, TK_TRUE); //forcefully detach
   return 0;
}

/*!
@ingroup kernel

Creates a thread and puts it in runnable state (READY). Thread is not
run though, until next yeald (or interrupt in case of a preemtable
version).

*/
thid_t tk_create_thread(
   char          *name,     /*!< Name of the thread. This name can later be
                                 used to identify or look up a tread by name.*/
   unsigned int   prio,     /*!< Priority. A low value means higher priority.
                                 The value must be between 0 and
                                 TK_MAX_PRIO_LEVELS. 0 is reserverd by
                                 convention for system deamons (and the root
                                 thread). <b>Avoid using priority level 0 for
                                 normal application threads. </b> */
   start_func_f   f,        //!< Start function, i.e. the treads entry point.
   void          *inpar,    //!< Any variable or value to start of with
   size_t         stack_size//!< Stack size
){
   //where in __tk_scheduler to put thread id
   thid_t   slot_idx = __tk_roundrobin_prio_idxs[prio].procs_at_prio;
   unsigned int   error = 0;
   thid_t valid_proc_idx;

   //Error handling needs improvement (don't forget taking special care of
   //__tk_proc_idx)
   if (__tk_procs_in_use >= TK_MAX_THREADS){
      printk(("tk: Error - Total amount of threads would exceed limit\n"));
      error |= TC_MAX_THREADS;
   }
   //Check if chosen prio is within limits
   if (prio >= TK_MAX_PRIO_LEVELS){
      printk(("tk: Error - Chosen priority too high\n"));
      error |= TC_MAX_PRIO_LEVELS;
   }
   //Check if there will be enough room at that prio
   if (TK_MAX_THREADS_AT_PRIO <= ( __tk_roundrobin_prio_idxs[prio].procs_at_prio ) ){
      printk(("tk: Error - To many threads at this prio\n"));
      error |= TC_THREADS_AT_PRIO;
   }
   if (error){
      tk_exit(error);
   }

   //Find next empty slot - which is also the CREATED Thid
   do{
      __tk_proc_idx++;
      __tk_proc_idx %= TK_MAX_THREADS;
   }while (__tk_threadPool[__tk_proc_idx].valid == TK_TRUE);

   //Test if the assigned name fits
   //In case highly optimized kernel, no names stored at all and no timly string copy
   #if TK_THREAD_NAME_LEN
      if (strlen(name)<TK_THREAD_NAME_LEN)
         strncpy(__tk_threadPool[__tk_proc_idx].name,name,TK_THREAD_NAME_LEN);
      else{
         printk(("tk: Error - Thread-name to long\n"));
         tk_exit(TC_NAME_LEN);
      }
   #endif
   __tk_threadPool[__tk_proc_idx].name[TK_THREAD_NAME_LEN] = 0; //Terminate string just in case it's needed. Note, there is one extra byte so indexing like this is OK.

   //Try to allocate memory for stack
   if (( STACK_PTR(__tk_threadPool[__tk_proc_idx].stack_begin) = (char *) stalloc(stack_size)) == NULL){
       printk(("tk: Error - Can't create process (can't allocate memory for stack)\n"));
       tk_exit(TC_NOMEM);  // terminate program if out of memory
   }

   REINIT_STACKADDR( __tk_threadPool[__tk_proc_idx].stack_begin, stack_size );

   __tk_threadPool[__tk_proc_idx].valid              = TK_TRUE;
   __tk_threadPool[__tk_proc_idx].state              = READY;
   __tk_threadPool[__tk_proc_idx].bOnId.kind         = BON_SCHED;
   __tk_threadPool[__tk_proc_idx].bOnId.entity.tcb   = NULL;
   __tk_threadPool[__tk_proc_idx].Thid               = __tk_proc_idx;    //for future compability with lage Thid:s
   __tk_threadPool[__tk_proc_idx].Gid                = __tk_active_thread; //Owned by..
   __tk_threadPool[__tk_proc_idx].noChilds           = 0;
   __tk_threadPool[__tk_proc_idx]._errno_            = 0;
   __tk_threadPool[__tk_proc_idx].stack_size         = stack_size;
   __tk_threadPool[__tk_proc_idx].Prio               = prio;
   __tk_threadPool[__tk_proc_idx].Idx                = slot_idx;
   __tk_threadPool[__tk_proc_idx].stack_crc          = 0;

   __tk_threadPool[__tk_proc_idx].wakeupEvent        = E_NONE;
   __tk_threadPool[__tk_proc_idx].retval             = inpar;  //This is convention and covers a special case
   /*
   __tk_threadPool[__tk_proc_idx].start_funct  = f;
   __tk_threadPool[__tk_proc_idx].init_funct   = NULL;
   __tk_threadPool[__tk_proc_idx].prmtRetAddr  = NULL;
   */

   __tk_threadPool[__tk_active_thread].noChilds++;
   __tk_procs_in_use++;

#if MARKUP_STACKS_ON_START
#  if defined (HAVE_MEMSET)
#  include <string.h>
      memset(STACK_PTR(__tk_threadPool[__tk_proc_idx].stack_begin),
            (unsigned char)__tk_proc_idx,stack_size-8);
#  else
      for (i=0;i<stack_size;i++)
         STACK_PTR(__tk_threadPool[__tk_proc_idx].stack_begin)[i] =
            (unsigned char)__tk_proc_idx;
#  endif
#endif

   /* The following will also yield to get thread in place, which if another
      creation/deletion occurs will interfere with kernel consistency */
   valid_proc_idx = __tk_proc_idx;
   _tk_construct_thread_scope( f, inpar, __tk_proc_idx );

   /* After yield, correct what has potentially become broken */
   slot_idx = __tk_roundrobin_prio_idxs[prio].procs_at_prio; /* Re-reed*/
   __tk_threadPool[__tk_proc_idx].Idx = slot_idx;            /* Re-write*/

   //Put process in schedule - assume tight tight schedule
   __tk_schedule[prio][slot_idx] = __tk_proc_idx;
   //Increase the amount of procs at same prio
   __tk_roundrobin_prio_idxs[prio].procs_at_prio++;

   //Make a integrity certification on the stack of this thread
   //INTEGRITY_CERTIFY_STACK(__tk_threadPool[valid_proc_idx], ct_temp3);
   return valid_proc_idx ;
}

int tk_join(thid_t PID, void ** value_ptr){
   if ( __tk_threadPool[PID].Gid != __tk_active_thread)    //Only threads own children can be joined in TinKer (restrictive)
      return EINVAL;

   if ( __tk_threadPool[PID].valid != TK_TRUE)
      return ESRCH;

   if (__tk_threadPool[PID].state & ZOMBI){ //Thread is finished allready. no need to block AND we can safelly read it's return value
      if (value_ptr != NULL) {
         *value_ptr = __tk_threadPool[PID].retval;
      }
      //Also release it for cancelling. This is done by breaking it's
      //parent-child relationship
      _tk_try_detach_parent(PID, TK_TRUE); //forcefully detach. This will also count down the child count in the parent

   }else{
      //prepare to block
      __tk_threadPool[__tk_active_thread].bOnId.kind       = BON_SCHED;
      __tk_threadPool[__tk_active_thread].bOnId.entity.tcb = &__tk_threadPool[PID];
      __tk_threadPool[__tk_active_thread].state = (PROCSTATE)(__tk_threadPool[__tk_active_thread].state | TERM);
      tk_yield();  //Will block
      assert(__tk_threadPool[__tk_active_thread].wakeupEvent == E_CHILDDEATH);
      if (value_ptr != NULL) {
         //We can't read the TCB of the thread we were just blocked on. It migt allready be cancelled
         //However, It knew about this case when it awoke us - and made a copy in *our* retval instead.
         *value_ptr = __tk_threadPool[__tk_active_thread].retval;
      }
   }

   return ERR_OK;
}

/*!

@ingroup kernel

@brief Sleeps for ms amount of time.

Sleeps for ms amount of time. Passes control to other threads meanwhile.

The longest time this function can sleep is dependant on the
targets bitsize for a integer. On 16 bit targets that would be  65535 mS
or  aprox <b>max 65 S </b>.<br>

The timeout resolution is the same as for the in-argument, i.e. best
kernel supported timeout <b>resolution is 1mS</b>. Note however that the
precision is dependant of the sys_mickey advancement resolution (i.e.
interrupt frequency). On a low performance target one sys_mickey might be
advanced less often than 1kHz (i.e. each mS). That means that the actual
precision is less than the resolution in this function. To help the
application to handle those cases, this function will return the latency
in 1/10 of the resolution.<br>

I.e. in case your timeout is later than you ideal, you can use this
valut in a  periodic thread to even out (compensate) for this case. In
this case each  thread will not run with a fixed frequency but
deliberatlly gitter a bit, so that the mean-value of the frequence is
the desired one.<br>

There isn't much any kernal can really do about this issue without beeing able
to control the HW in detail. That would however create a dependancy that is
beyont the scope of this project to handle. Note that choosing a better
precition is a delicate matter that is best suited for HW designers.
For now, alway remember to check the precition of you target.<br>

There are several sleep functions to choose between. Which one is best is
alwas a tradeof between resolution and maximum timeout.<br>


@note Note the latencies can occure even on targets
that have the same precision as resolution. In these cases the latency
is typically myck lower and should be in the samge of a few precition
periods (i.e. a very few multiples of the intersupt frequency) or
close to zero. This is due to that the code in the kernal itself takes
some time to reach the point where a dispatch is actuated.


@warning most of the above text is obsolete

@todo correct the text above

@note (060227). Detected problems using variables after the yield. Problem is believed to be C166 related due to the way it handles local variables (stack, register or other memory)

I noticed that the assumptions about the ANSI clock constants were
wrong. Either some targets don't follow standard or I've misunderstood
the text at GNU glibc. There's what different targets report:

- X86 Linux: CLK_TCK=[100], CLOCKS_PER_SEC=[1000000]
- Cywin: CLK_TCK=[1000], CLOCKS_PER_SEC=[1000]
- XC167: CLK_TCK=[1000000], CLOCKS_PER_SEC=[1000000]

I believe that the linux target is closest to following standard and
then I believe it means that says something about either the clock
update frequency OR the resolution (I don't know which yet).

It seems that the documentation at GNU regarding this is wrong. The only
thing that seems right is that to get the time in \b seconds, you take
the output from the \ref clock() function and divide that with \ref
CLOCKS_PER_SEC. Note that to get time in uS we're dividing 1e6 with
CLOCKS_PER_SEC separately.

@todo \ref ITC has some timeouts that need correcting according to this
also

*/

void tk_msleep( unsigned int time_ms ){
/* It's our kernal so we "know" that a clock equals 1uS */
   clock_t act_time_us;
   clock_t wkp_time_us;
   unsigned long in_us;
   //unsigned long clk_sek = CLK_TCK;
   //unsigned long clk_sek = CLOCKS_PER_SEC;

   in_us = time_ms * 1000uL;
   act_time_us    = tk_clock() * (1000000uL/CLOCKS_PER_SEC);
   wkp_time_us    = act_time_us + in_us;
   //wkp_time_us    = act_time_us + time_ms*1000uL;

   //need a function t_diff that handles wraparound (done 060225, note kept for ref.)
   __tk_threadPool[__tk_active_thread].wakeuptime = wkp_time_us;
   __tk_threadPool[__tk_active_thread].state = (PROCSTATE)(__tk_threadPool[__tk_active_thread].state | SLEEP);
   /*
   Hmm... don't do the following unless zombie is checked. Pointless info, so we'll skip it
   __tk_threadPool[__tk_active_thread].bOnId = __tk_active_thread;
   */
   tk_yield();
}

/*!
Changes the priority of a running thread.

\returns If state is actually changed (i.e. if yield is recommended)
*/
int tk_change_prio(thid_t tid, int newPrio){
   int y;
   int oldPrio;
   thid_t oldIdx,newIdx;
   int error = 0;
   int i;

   if ( __tk_threadPool[tid].valid != TK_TRUE){
      printk(("tk: Error - Invalid TCB detected\n"));
      error |= TC_TCB_INVALID;
   }

   //Check if chosen prio is within limits
   if (newPrio >= TK_MAX_PRIO_LEVELS){
      printk(("tk: Error - Chosen priority too high\n"));
      error |= TC_MAX_PRIO_LEVELS;
   }
   //Check if there will be enough room at that prio
   if (TK_MAX_THREADS_AT_PRIO <= ( __tk_roundrobin_prio_idxs[newPrio].procs_at_prio ) ){
      printk(("tk: Error - To many threads at this prio\n"));
      error |= TC_THREADS_AT_PRIO;
   }
   if (error){
      tk_exit(error);
   }

   oldPrio = __tk_threadPool[tid].Prio;
   oldIdx =  __tk_threadPool[tid].Idx;

   if (oldPrio == newPrio)          //No need to do anything
      return 0;

   if (__tk_threadPool[tid].state != READY)
      y = 0;
   else{
      if (newPrio > oldPrio)          //is yield recomended after completion?
         y = 1;
      else if (__tk_active_thread == tid)
         y = 1;
      else
         y = 0;
   }

   //Take away the process from __tk_schedule and compress gap at prio
   for(i=oldIdx; i < __tk_roundrobin_prio_idxs[oldPrio].procs_at_prio; i++){
      __tk_schedule[oldPrio][i] = __tk_schedule[oldPrio][i+1];
      __tk_threadPool[__tk_schedule[oldPrio][i]].Idx = i;
   }
   __tk_roundrobin_prio_idxs[oldPrio].procs_at_prio --;
   if (__tk_roundrobin_prio_idxs[oldPrio].curr_idx >= __tk_roundrobin_prio_idxs[oldPrio].procs_at_prio){ //Uncertain about this.. Same issue in kill_thread
      __tk_roundrobin_prio_idxs[oldPrio].curr_idx = 0;
   }

   //Put the thread in the schedule in it's new position
   newIdx = __tk_roundrobin_prio_idxs[newPrio].procs_at_prio;

   __tk_schedule[newPrio][newIdx] = tid;
   __tk_roundrobin_prio_idxs[newPrio].procs_at_prio++;

   //Finalize by correcting the TCB also
   __tk_threadPool[tid].Prio = newPrio;
   __tk_threadPool[tid].Idx  = newIdx;

   return y;

}

/*!
@brief Detach any children of the \e thid

This function is normally called as a step by a thread on it's way to
cancel.

When a thread that has children dies without joining it's children, it
implicitly means that he will never join them (obvious right? ;) ).
After executing this function, any children will be orfans.

When those childern exit, it means the following

- Exiting thread can imediatlly cancel (go to nirvans). It doesn't
have to wait for any parent to (maybe) join them. The opposite, when a
child dies before it's parent (assuming it's still atached), it has to
consider that the parent \b might deside to join later. The child can
not know this, so it has to stay in \ref ZOMBI state until there is no
question about intent any more.

- The TCB for each exiting thread can be reclaimed imediatlly. Meaning,
if you have a small system with only resources for a few threads, you
should always show intent by detaching threads after creating them, if
you never intend to join them (will save resources).

*/
void _tk_detach_children(
   thid_t thid
){
   thid_t j;

   if (  __tk_threadPool[thid].noChilds > 0 ){  //Dang! We must make our own children parentless ;(
      for(j=0;(__tk_threadPool[thid].noChilds>0) && (j<TK_MAX_THREADS);j++){
         if (__tk_threadPool[j].valid && __tk_threadPool[j].Gid == thid){
            /*Foud a poor sod*/
            __tk_threadPool[j].Gid = -1;
            __tk_threadPool[thid].noChilds--;
         }
      }
   }
}


/*!
@brief Detach the parent


Break the relation to the parent of the \e thid

This function is normally called as a step by a thread on it's way to
cancel. When this function is executed, it means that \e this is an
orfan. (Any children it has is however still attached to it. I.e.that
relation is unchanged.)

The only duty a exiting thread really has is to umblock it's parent \b once.
When this is done, the thread has no other obligations and can safelly cancel.

\returns The function will return succes (TK_TRUE) or faliure (TK_FALSE).

When we detach from a parent, we mean the following:

- That the parent can't join with (at least) us any longer.

- When \e thid exits, it can cancel emediatly and doesnt have to
consider any parent who \e might deside to join with us later.

The following rules apply to this function:

- Should it happen that a parent is allreadu joining us when this
function is called, we release it (no matter of the status of \e force
attribute).

- To detach a parent that might deside us later, we have to explicitly
say so by setting \e force to TK_TRUE. Otherwize, detachement will only be
a succes if a parent is joining on us.

- If \e thid is allready detached once, the function alwas returns TK_TRUE

*/

int _tk_try_detach_parent(
   thid_t thid,                  //!< The thread id whos parent we are concerned with
   int force                     //!< Force detachment
){
   int succeded_unblocking_parent = TK_FALSE;

   if (  __tk_threadPool[thid].Gid == -1 )
      return TK_TRUE;                     //allready orphan

   // (if con't) Yes, I have a parent, but is it waiting for my death or not?
   assert (__tk_threadPool[__tk_threadPool[thid].Gid].valid);  //sanity check
   //We must have faith now, that the Gid is actually the one that created us, and
   //not a reused identity (which should not be possible to happen - mybe invent a
   //snity check for this?)


   //If waiting for me (not my bro or sis) death, unblock the parent. (i.e. parent executed a tk_join)
   if (
      (__tk_threadPool[__tk_threadPool[thid].Gid].state & TERM) &&
      __tk_threadPool[__tk_threadPool[thid].Gid].bOnId.entity.tcb == &__tk_threadPool[thid] )
   {
      __tk_threadPool[__tk_threadPool[thid].Gid].state = (PROCSTATE)(__tk_threadPool[__tk_threadPool[thid].Gid].state & ~_______T); //Release the waiting parent
      __tk_threadPool[__tk_threadPool[thid].Gid].wakeupEvent = E_CHILDDEATH;
      //Special case, if both ___S_ and ____T (i.e. timeoutable join)
      if (__tk_threadPool[__tk_threadPool[thid].Gid].state == READY) { //What must be left is __S_
         __tk_threadPool[__tk_threadPool[thid].Gid].bOnId.entity.tcb = NULL;
         __tk_threadPool[__tk_threadPool[thid].Gid].bOnId.kind       = BON_SCHED;
      }else{
         __tk_threadPool[__tk_threadPool[thid].Gid].bOnId.entity.tcb = &__tk_threadPool[thid];
         __tk_threadPool[__tk_threadPool[thid].Gid].bOnId.kind       = BON_SCHED;
      }

      succeded_unblocking_parent = TK_TRUE;
      //We are likelly to get out of context before the parent can read our retval. Therefore make an
      //active copy to the parent's retval instead. The parent will know of this case and use that one
      //instead of trying to read a potentially invalid TCB.
      __tk_threadPool[__tk_threadPool[thid].Gid].retval = __tk_threadPool[thid].retval;

   } else
      succeded_unblocking_parent = TK_FALSE;

   if ( (succeded_unblocking_parent == TK_TRUE) || (force == TK_TRUE) ) {
      __tk_threadPool[__tk_threadPool[thid].Gid].noChilds--;
      assert(__tk_threadPool[__tk_threadPool[thid].Gid].noChilds >= 0);
      __tk_threadPool[thid].Gid = -1;
      return TK_TRUE;
   }

   return TK_FALSE;
}



/* @ingroup kernel_internal

Get the identity of the next thread to run.  Scheduling policy is used
to determine which one that would be.
*/

thid_t _tk_next_runable_thread(){
   int idx,prio,nbTry,loop, p_at_p;
   thid_t return_Thid;
   TK_BOOL found;
#ifndef NDEBUG
   thid_t last_thid = (thid_t)-1;
#endif

   return_Thid  = idle_Thid; //In case no runnable proc is found...
   found        = TK_FALSE;

   /* Iterate over prio, from highest (=0) to lowest */
   for(
      prio=0;
      prio<TK_MAX_PRIO_LEVELS && !found;
      prio++
   ){
      p_at_p = __tk_roundrobin_prio_idxs[prio].procs_at_prio;
      /* No procs at prio to run.. Prevent division by zero */
      if (p_at_p != 0) {
         //Manual RR from current to next ready
         idx =(__tk_roundrobin_prio_idxs[prio].curr_idx);
         //cidx = __tk_roundrobin_prio_idxs[prio].curr_idx;
         nbTry = __tk_roundrobin_prio_idxs[prio].procs_at_prio;
         for(
            loop = 0;
            loop < nbTry && !found;
               loop++
         ) {
            /* Asserts below search for bad logic in list. These are true
             * asserts */
#ifndef NDEBUG
            assert(__tk_threadPool[__tk_schedule[prio][idx]].valid);
            assert(__tk_threadPool[__tk_schedule[prio][idx]].Thid != last_thid);
            last_thid=__tk_threadPool[__tk_schedule[prio][idx]].Thid;
#endif
            if (__tk_threadPool[__tk_schedule[prio][idx]].state == READY){
               return_Thid = __tk_schedule[prio][idx];
               found = TK_TRUE;
            }
            //Next proc at this prio that should try to run
            idx++;
            idx %= p_at_p; //saves some pointer-casts this way
         }
      }
   }
   return return_Thid;
}


/*!
@ingroup kernel_internal

@brief Context switch to next tread.

Makes a context switch. Will prior to that also try to detect stack
out-of-bounds errors.

@todo Besides out of bounds check, a check for stack consistency would be
nice. Suggestion: Before each block, calculate CRC of the 0x10 bytes
from top and bottom of stack(s) and store in TCB. On each waking up, redo
the calculation and check against stored values. This should catch more
sophisticated out-of-bounds application bugs. (see note in about
TRY_CATCH_STACK_ERROR)

@see TRY_CATCH_STACK_ERROR


@warning <b>Has static variables (very much non-reentrant and thread un-safe)</b>

*/

void _tk_context_switch_to_thread(
   thid_t         RID,        //!< Thread ID to switch to
   thid_t         SID         /*!< Thread ID to switch from. I.e.
                                   current thread ID to put away in TCB*/
){
   static char *cswTSP;          //!< fony temporary stackpointer used in the process of setting TOS
   static unsigned int cswTEMP;  //!< Extra storage. For some targets used to manipulate segment part of stackpointers
   static unsigned int cswTEMP2; //!< Extra storage.

   TK_NO_WARN_ARG(cswTEMP);
   TK_NO_WARN_ARG(cswTEMP2);

   //TRY_CATCH_STACK_ERROR( __tk_threadPool[SID].stack_begin, cswTEMP2 );
   //INTEGRITY_CERTIFY_STACK( __tk_threadPool[SID], cswTEMP2 ); //Certify the stack we're leaving from

   PUSH_CPU_GETCUR_STACK( cswTSP, cswTEMP );

   #ifdef  JUMPER_BASED
   if (cswTEMP==0)
   #endif
   {
      STACK_PTR( __tk_threadPool[SID].curr_sp ) = cswTSP;

      cswTSP = STACK_PTR( __tk_threadPool[RID].curr_sp );

      __tk_active_thread=RID;
      CHANGE_STACK_POP_CPU( cswTSP, cswTEMP );
   }
   //TRY_CATCH_STACK_INTEGRITY_VIOLATION( __tk_threadPool[__tk_active_thread], cswTEMP2 ); //Check integrity is OK before running
}

void _tk_half_switch (
   thid_t         RID,        //!< Thread ID to switch to
   thid_t         SID         /*!< Thread ID to switch from. I.e.
                                   current thread ID to put away in TCB*/
){
   static char *cswTSP;          //!< fony temporary stackpointer used in the process of setting TOS
   static unsigned int cswTEMP;  //!< Extra storage. For some targets used to manipulate segment part of stackpointers
   static unsigned int cswTEMP2; //!< Extra storage.

   TK_NO_WARN_ARG(cswTEMP);
   TK_NO_WARN_ARG(cswTEMP2);


   //TRY_CATCH_STACK_ERROR( __tk_threadPool[SID].stack_begin, cswTEMP2 );
   //INTEGRITY_CERTIFY_STACK( __tk_threadPool[SID], cswTEMP2 ); //Certify the stack we're leaving from


   PUSH_CPU_GETCUR_STACK( cswTSP, cswTEMP );

   #ifdef  JUMPER_BASED
   if (cswTEMP==0)
   #endif
   {
      STACK_PTR( __tk_threadPool[SID].curr_sp ) = cswTSP;
      cswTSP = STACK_PTR( __tk_threadPool[SID].curr_sp );

      __tk_active_thread=RID;
      CHANGE_STACK_POP_CPU( cswTSP, cswTEMP );
   }
   //TRY_CATCH_STACK_INTEGRITY_VIOLATION( __tk_threadPool[__tk_active_thread], cswTEMP2 ); //Check integrity is OK before running
}


/*!
@brief Runs \b before threads entry function, but \b in the threads scope

*/

void _tk_constructor(
   start_func_f   f,        //!< Start function, i.e. the treads entry point.
   void          *inpar,    //!< Any variable or value to start of with
   thid_t         id        //!< The ID (needed to relate to the correct TCB)
){
   void *retval;

   //When we reach here, the "scope" is actally in place,
   //but we don't want to run the tread yet.
   //Instead we start by "context" switching back to the
   //thread that created us.

   //_tk_context_switch_to_thread(__tk_active_thread, id); //Note both arguments *seems* wrong (but they are correct).
   //When the above function returns - it will return to the caller *and not to us* (important)

   tk_yield();

   //Reaching the next line means we're been contended back here by the dispatcher.
   //The thread is now operating in its proper scope.
   retval = f(inpar);

   //The thread has given up life - call it's destructor to release it's resources (i.e. begin cancelation)
   _tk_destructor(retval);

}

/*!
@warning <b>Has static variables (very much non-reentrant and thread un-safe)</b>

*/
void _tk_construct_thread_scope(
   start_func_f   f,        //!< Start function, i.e. the treads entry point.
   void          *inpar,    //!< Any variable or value to start of with
   thid_t         id        //!< The ID (needed to relate to the correct TCB)
){
TK_CLI();

   //We're going to operate on a different stack, so we need
   //to store away the arguments somewhere guaranteed not to
   //be on any stack (needed so that we can reach their contents).
   static  start_func_f   t_f;
   static  void          *t_inpar;
   static  thid_t         t_id;

   //Some other code segmented temp variables needed by the macros
   static char *aSP;
   static unsigned int TEMP;
   //static stack_t SPS;

   TK_NO_WARN_ARG(TEMP);

   t_f       = f;
   t_inpar   = inpar;
   t_id      = id;

   //Set initial value of the SP in TCB (does not affect register SP)
   INIT_SP(
      __tk_threadPool[t_id].curr_sp,
      __tk_threadPool[t_id].stack_begin
   );

   //The following variable is used by the BIND_STACK macro.
   //Some targets use indirect addressing using the same registers we're
   //manipulating and cant access complex structures without them. Therefore
   //keep the following  access simple by using a copy stored in the code seg.
   //(Note: stack_t is a form of pointer that is used to refer to the physical stack)
   //SPS = __tk_threadPool[t_id].curr_sp;
  //TK_CLI();
   PUSHALL();              // <--       // Make sure CPU context is ok
  //TK_STI();
  //TK_CLI();
   _tk_half_switch(t_id, __tk_active_thread);
  //TK_STI();

   if (__tk_active_thread != t_id){
      POPALL();            // <--      // ...since we're coming back via
      return;                          // another function (might not be using
                                       // exactly the same regs, base-
                                       // pointers e.t.a.
   }

   //Do the actual stack change (CPU register level)
   aSP = STACK_PTR( __tk_threadPool[t_id].curr_sp );
   CHANGE_STACK( aSP, TEMP );

   //Some targets need a certain binding of the stack until we can use a single
   //value to access the whole stack (single value = the key to the whole = i.e.
   //the current SP register value).
   BIND_STACK( SPS, TEMP );
TK_STI();
   _tk_constructor(t_f, t_inpar, t_id);

}

#if defined(TK_COMP_FILESYS) && TK_COMP_FILESYS
//#include <unistd.h>
#include <fcntl.h>

int
     set_fflags(int desc, int flags)
     {
       int oldflags = fcntl (desc, F_GETFL, 0);
       /* If reading the flags failed, return error indication now. */
       if (oldflags == -1)
         return -1;

       oldflags |= O_NONBLOCK;

       /* Store modified flag word in the descriptor. */
       return fcntl (desc, F_SETFL, oldflags);
     }

#endif

#ifdef BOOT_BSP_STUB
/* Instance of dummy system initializer when there's no HIXS or other
 * syscall interface to be initialized */
int _tk_bsp_sysinit(void) {
   return 0;
}
#else
#  define _tk_bsp_sysinit tk_bsp_sysinit
#endif

#define _MSTR(x) \
    #x
#define MSTR(x) \
    _MSTR(x)
/*!
@ingroup kernel_glue

Called from your real main() or from your start-up code

@note (for deeply embedded developers) You target have to fulfill two
things at least:

- malloc has to work and return valid pointers
- You need a working printf to see run-time errors

*/
void _tk_main( void ){
   _b_hook(_tk_main);
   #if defined (TK_SYSTEM) && (TK_SYSTEM == __SYS_HIXS__)
      _syscall_mon(_tk_main);
      extern struct hixs_t hixs;
      hixs.exit = tk_exit;
   #endif
   _tk_bsp_sysinit();
   _b_hook(_tk_bsp_sysinit);

   printk(("BSP initialized\n"));

   #if defined(__GNUC__)
      printk(("TinKer running target : %s\n",MSTR(TK_ALIAS_HOST)));
      printk(("GCC version           : %s\n",MSTR(TK_GCC_VERSION)));
      printk(("Main architecture     : %s\n",MSTR(__TK_ARCH_up__)));
      printk(("CPU variant           : %s\n",MSTR(__TK_DCPU_up__)));
      printk(("Board                 : %s\n",MSTR(__TK_BOARD_up__)));
      printk(("System                : %s\n",MSTR(__TK_SYSTEM_up__)));
      printk(("Scheduling method     : %s\n",MSTR(TK_DISPATCH)));
   #endif

   _b_hook(tk_create_kernel);
   tk_create_kernel();
   printk(("TinKer kernel created\n"));

   printk(("ANSI timing constants:\n"));
   #if  defined( __C166__ )
       printk(("CLK_TCK=[%ld], CLOCKS_PER_SEC=[%ld]\n",CLK_TCK, CLOCKS_PER_SEC));
   #else
       printk(("CLK_TCK=[%d], CLOCKS_PER_SEC=[%d]\n",CLK_TCK, CLOCKS_PER_SEC));
   #endif
   #if defined(TK_COMP_KMEM) && TK_COMP_KMEM
      _b_hook(tk_mem);
      assert( tk_mem() == ERR_OK );
   #endif

   #if defined(TK_COMP_ITC) && TK_COMP_ITC
      _b_hook(tk_itc);
      assert( tk_itc() == ERR_OK );

      _b_hook(_tk_create_system_queues);
      assert(_tk_create_system_queues( ) == 0);  //FIXME this one needs re-work
      #if defined(TK_COMP_PTIMER) && TK_COMP_PTIMER
         _b_hook(tk_ptime);
         assert( tk_ptime() == ERR_OK );
      #endif
      #if defined(TK_COMP_PTHREAD) && TK_COMP_PTHREAD
         _b_hook(tk_pthread_sched);
         assert( tk_pthread_sched() == ERR_OK );
         _b_hook(tk_pthread_sync);
         assert( tk_pthread_sync()  == ERR_OK );
         #if defined(TK_COMP_POSIX_RT) && TK_COMP_POSIX_RT
            //TBD
         #endif
      #endif
   #endif

   #if defined(TK_COMP_FILESYS) && TK_COMP_FILESYS
      _b_hook(fs_init);
      printk(("Filesystem starting...\n"));
      assert(fs_init() == ERR_OK );
      //fdup(stdout);
      //fdup2(stderr,stdout);

      //set_fflags(stdout,O_NONBLOCK);
      //set_fflags(stderr,O_NONBLOCK);

      fprintf(stderr,"Filesystem initialized! (1)\n");
      printk(("Filesystem initialized! (2)\n"));
   #endif


   tk_root();

   #if defined(TK_COMP_FILESYS) && TK_COMP_FILESYS
      _b_hook(fs_fini);
      printk(("Filesystem shutting down...\n"));
      assert(fs_fini() == ERR_OK );
      fprintf(stderr,"Filesystem terminated!\n");
   #endif


   #if defined(TK_COMP_ITC) && TK_COMP_ITC
      #if defined(TK_COMP_PTHREAD) && TK_COMP_PTHREAD
         #if defined(TK_COMP_POSIX_RT) && TK_COMP_POSIX_RT
            //TBD
         #endif
         _b_hook(tk_pthread_sched_destruct);
         assert( tk_pthread_sched_destruct() == ERR_OK );
         _b_hook(tk_pthread_sync_destruct);
         assert( tk_pthread_sync_destruct()  == ERR_OK );
      #endif
      _b_hook(tk_itc_destruct);
      assert( tk_itc_destruct() == ERR_OK );
      #if defined(TK_COMP_PTIMER) && TK_COMP_PTIMER
         _b_hook(tk_ptime_destruct);
         assert( tk_ptime_destruct() == ERR_OK );
      #endif
   #endif

   #if defined(TK_COMP_KMEM) && TK_COMP_KMEM
      _b_hook(tk_mem_destruct);
      assert( tk_mem_destruct() == ERR_OK );
   #endif
   _b_hook(tk_delete_kernel);
   tk_delete_kernel();
   printk(("Kernel deleted\n"));
}


#if (defined(_WIN32)          &&  defined(_MSC_VER))             \
 ||  defined(__BORLANDC__)    || defined(__BCPLUSPLUS__)         \
 ||  defined(__CYGWIN32__)    || defined(__CYGWIN__) || defined(__GNUC__)

/*
//Declaring a helper variable as (any any type of) function pointer
//makes gdb be able to look up it's name (practical when debugging).
void (*my_lastcall)(void);

void My_syscall_mon( void *syscall ) {
   my_lastcall = syscall;
}

int My_write(int file, char *ptr, int len){
    int todo;

    for (todo = 0; todo < len; todo++) {
       //writechar(*ptr++);
    }
    return len;
}

//We "know" the following functions exists. They are really function pointers
//so we need to let the compiler know they are assignable
extern int  (*hixs_write)(int file, char *ptr, int len);
extern void (*hixs_syscall_mon)(void *syscall);
*/


int main(int argc, char **argv);
int main(int argc, char **argv){
   /*
   hixs_syscall_mon   = My_syscall_mon;

   //Now test your new syscall monitor
   hixs_syscall_mon(main); //Short-cut call
   _syscall_mon(main);     //Hooked call
   //If you return from here, monitoring *should* work

   //Test overloading the _write sys-call (by overloading it's hooked fuction)
   hixs_write=My_write;

   //The following call should now generate calls to the monitor
   //(i.e. My_syscall_mon since we overloaded it) and to My_write
   printf("Hello world");
   */

   //assert("This is an intentional faliure..." == NULL);
   //BOOT_HOOK;
   _b_hook(main);
    _tk_main();
   _b_hook(NULL);
   TRAP(0);
   return 0;
}
#endif


/** @defgroup SCHED SCHED: TinKer scheduler
@ingroup COMPONENTS

@brief TinKer scheduler - the \e"guts" & internals of the TinKer kernel

*-* Documentation of this component is work in progress *-*

<p><b>Go gack to</b> \ref COMPONENTS</p>
*/




/*!
 * @defgroup CVSLOG_tk_c tk_c
 * @ingroup CVSLOG
 *  $Log: tk.c,v $
 *  Revision 1.82  2007-04-04 08:12:45  ambrmi09
 *  Improvement to lower latency in collaborative mode. Might need
 *  concideration because it alters the priority rule-set.
 *
 *  Revision 1.81  2007-03-23 20:42:11  ambrmi09
 *  Major renaming of shared (to be) variables that will be shared among several
 *  files. This is due to the need to partition tk.c
 *
 *  Revision 1.80  2007-03-19 17:07:55  ambrmi09
 *  Interrupt structure/handling reworked - Motorola is wacked, clear IRQ by
 *  setting allready bit in SIPEND (!)
 *
 *  Revision 1.79  2007-03-04 19:07:25  ambrmi09
 *  1) Error handling refined - will handle error from different
 *     cathegories:
 *     - errno (perror)
 *     - TK errors
 *     - TK traps codes
 *     - exit handling can differ beween user exit codes and kernel
 *       trap codes.
 *  2) Extracted fluffy & un-critical code from tk.c (the error and exit
 *     stuff)
 *  3) Preparing to partition even further into tk_switch.c (saving this
 *     until next ci though).
 *
 *  Revision 1.78  2007-03-03 23:01:32  ambrmi09
 *  Added driver support for FIFO's
 *
 *  Revision 1.77  2007-03-02 10:15:13  ambrmi09
 *  Broaden the reagin interrups are dissaböed when creating threads. This
 *  whole theing needs to be over-looked.
 *
 *  Revision 1.76  2007-02-27 23:28:25  ambrmi09
 *  - Generic driver sceletons provided - good as starting points for custom drivers
 *  - A sample LED driver fully functional (PowerPC 860/ESC only)
 *
 *  Revision 1.75  2007-02-26 14:16:53  ambrmi09
 *  1) Drivers    - structure added
 *  2) Filesystem - in progress
 *
 *  Revision 1.74  2007-02-24 12:17:14  ambrmi09
 *  1) Sync PowerPC sources
 *  2) Add structure for i386-hixs-elf target. (Hopefully our new test-bed)
 *     for developping the filesys component.)
 *
 *  Revision 1.73  2007-02-22 23:22:31  ambrmi09
 *  FS structure added.
 *
 *  Revision 1.72  2007-02-22 12:36:48  ambrmi09
 *  1) Structure adapted for modulerizing components under tinker/src
 *     in the same fashion as the modules under tinker/bsp
 *  2) New component added TK_FILESYS (resides under src/filesys)
 *
 *  Revision 1.71  2007-02-21 21:18:41  ambrmi09
 *  Nasty bug fixed. Turns out setjump/jongjum is more sensitive for interrupt
 *  interference than the old techique. Will be reluctant to change back though
 *  since I really like this design. Saving context in the beginnig of each
 *  pool might be a better solution. For now the issue is solved with interrupt
 *  enable/disable protection (which should go there sooner or later anyway, I
 *  would just feel better if the mentioned fix would be implemented also as a
 *  double precaution).
 *
 *  Revision 1.70  2007-02-21 01:01:24  ambrmi09
 *  Was a very good and productive day! Kernel runs on new target, but stacks
 *  get busted. i think it's the setjmp, longjump think thats messing with us.
 *  It's not saving enough registers or the stack is creaping away fo some other
 *  reason.
 *
 *  The error seems quite easy though (relativly speaking i.e. ...)
 *
 *  Revision 1.69  2006/12/11 14:41:52  ambrmi09
 *  Solves #1609064 (part1)
 *
 *  Revision 1.68  2006/12/01 10:58:51  ambrmi09
 *  Solves #1605911 #1605893
 *
 *  Revision 1.67  2006/11/27 22:29:24  ambrmi09
 *  Minor djustments completeing the move of some header files to public and due
 *  to some name clashed with user space naming conventions.
 *
 *  Revision 1.66  2006/11/05 19:10:06  ambrmi09
 *  minor
 *
 *  Revision 1.65  2006/11/05 19:06:04  ambrmi09
 *  Buildsystem adjusted to permit configuration of components.
 *  Now when component is enabled it will also be included in the build
 *  (instead of just sanity-tested in the source files).
 *
 *  Also a feature for application sanity is assed. When a header-file is
 *  included in the application, a check against the component it belongs
 *  to will be performed. That way user don't need to rely on run-time
 *  checks and can get feedback much earlier.
 *
 *  Revision 1.64  2006/11/05 14:18:59  ambrmi09
 *  Build system and source modified to make better use of config.h
 *
 *  This file now contains information about how the kernel is configured
 *  and can be used by both application and kernel build (old solution only
 *  let kernel-buils know of these details).
 *
 *  This applies to both tk_tuning, component configuration among others.
 *  Use './configure --help' to see a full list. Note that  if a certain
 *  feature is not configured, the old tk_tuning will fill in the gaps.
 *  This is especially usefull when not using GNU build- and configure-
 *  tool-chain. Hopefully, we'll be able to get rid of tk_tuning.h in the
 *  future.
 *
 *  Revision 1.63  2006/10/13 21:47:11  ambrmi09
 *  2 new CPU ports added: PowerPC and Blackfin. Also testing a new a new concept
 *  for thread creation that potentionally has big transparency advantages (needs
 *  to be proven in preemtive cases).
 *
 *  * PowerPC tested against simulator
 *  * Blackfin only compile and link (gcc and Newlib needes adaptation/porting -
 *    GDB and sinulator still missing)
 *
 *  Revision 1.62  2006/09/28 17:42:44  ambrmi09
 *  HIXS system integration for RM done. ARM now has two different system call API supported. This is mostly interesting from a transparency point of view, but also a good exersisze for bfin and ppc, which are not ported yet (since I'm not planning on implementing any other system integrations than HIXS from now on).
 *
 *  Revision 1.61  2006/09/27 13:46:26  ambrmi09
 *  * New build system launched
 *  * Preparation for more systems (HIXS) and targets done
 *
 *  Revision 1.60  2006/09/14 10:09:08  ambrmi09
 *  Tuneup for Linux GDB console to output when supposed to. Varidiac function
 *  that fails when called from other thread than root added as example
 *  (invalidated).
 *
 *  Revision 1.59  2006/09/13 18:29:31  ambrmi09
 *  Commited needed in repocitory
 *
 *  Revision 1.58  2006/04/08 10:16:02  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.57.2.7  2006/04/07 12:10:07  ambrmi09
 *  Skeleton for handling syscalls using the ARM Angel interface in place
 *
 *  Basic terminal I/O for gnu_arm (LPC2129) - only output so far (input requires
 *  blocking).
 *
 *  Revision 1.57.2.6  2006/04/06 09:01:56  ambrmi09
 *  Safety commit due to change of local sandbox FS type (had files checked out
 *  while changing the type - not to be recommended).
 *
 *  Revision 1.57.2.5  2006/04/03 20:07:28  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.57.2.4  2006/04/03 15:21:47  ambrmi09
 *  All targets updated with the new thread-starter (alternative 2).
 *
 *  This alternative has one weakness (explained elsewhere togeather
 *  with alternative 1), but so far it's the only one that will compile
 *  and function equally among all 4 (very different) compilers currently
 *  tested against: GCC, MSVC, BC5 and Keil.
 *
 *  If nothing else turns up, I'm willing to overcome the drawback (it's
 *  quite handleable) because it *truly* takes away a lot of pain with
 *  porting.
 *
 *  The ARM port (architecture level) is than's to this now fully operational
 *  without the r13 hack in the context switch. This includes thread
 *  cancellation and thread argument passing (which were not functioning in
 *  the old port).
 *
 *  If this revised code proves itself (i.e. no surprises turns up) then
 *  TinKer can be considered "almost ported" to any HW target that GCC is
 *  ported for :D (/cheers)
 *
 *  Revision 1.57.2.3  2006/03/31 17:42:56  ambrmi09
 *  Second idea for the new thread starter. This one plays nice with several
 *  more compilers beacuse of it's balances call-stack. It's not as
 *  beautiful as the former one IMO, but GNU is a pain in the but
 *  with it's call-stack optimizations (and decorations doesn't seem to work
 *  for Cygwin GCC ).
 *
 *  Revision 1.57.2.2  2006/03/30 15:39:28  ambrmi09
 *  Tres amigos (the magic function) needs to have equally balanced stack-usages.
 *
 *  This is among others, the same number and types of variables.
 *
 *  Be aware that optimization might play us a trick here in future.
 *
 *  Revision 1.57.2.1  2006/03/30 10:52:20  ambrmi09
 *  First version of new threadstarter. It seems very promising. A *lot* of
 *  awfull pain concerning different targets has the potential to go away.
 *
 *  Revision 1.57  2006/03/24 11:22:56  ambrmi09
 *  - pThreads RW locks implemented (rough aproach - no usage error detection)
 *  - restructuring of the pThread src-files
 *
 *  Revision 1.56  2006/03/19 22:57:55  ambrmi09
 *  First naive implementation of a pthread mutex
 *
 *  Revision 1.55  2006/03/19 12:44:36  ambrmi09
 *  Got rid of many compilation warnings. MSVC amd GCC actually gompiles
 *  without one single warning (yay!). Be aware that ther was a lot of
 *  comparisons between signed/unsigned in ITC. Fetts a bit shaky...
 *
 *  Revision 1.54  2006/03/17 12:20:04  ambrmi09
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
 *  Revision 1.53  2006/03/14 10:23:19  ambrmi09
 *  Dedicated package "stack malloc" for XC167 added (see \ref
 *  STALLOC). This nice little package helped me find a new bug that not yet
 *  had shown itself. For some reason, some threads (actually only the \ref
 *  idle thread, but I suspect the problem is general) wrote outside their
 *  top stack boundary. I still don't know why, but it can only be one of
 *  two reasons:
 *
 *  - Alignment problem (a patch fixing that is already implemented)
 *  - The compiler makes some assumptions about its local data (i.e. data on
 *  stack, but in C166 lingo this is called user stack).
 *
 *  Keep eyes open for the second one since it's very hard to verify and/or
 *  prove. \ref INTEGRITY_CERTIFY_STACK should be put back in business (was
 *  temporary removed due to implementation problems and performance
 *  issues).
 *
 *  Revision 1.52  2006/03/12 15:08:56  ambrmi09
 *  - Adjusted the source to accomodate the new file structure.
 *
 *  - All build environments uppdated and verified except BC5. For this one
 *  we stumbled across the header-file issue that I've been fearing. Seems
 *  we need to take care of that one after all.
 *
 *  @note The malloc bug still not solved.
 *
 *  Revision 1.51  2006/03/11 14:37:50  ambrmi09
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
 *  Revision 1.47  2006/03/04 19:32:35  ambrmi09
 *  Modified sources to allow build GNU targets transparently.
 *
 *  Revision 1.46  2006/03/04 14:28:44  ambrmi09
 *  Finally got the \ref clock() representation right. Now timing is
 *  behaving equaly between the targets X86_Linux, Cygqing, MSVC, BC5 and
 *  XC167.
 *
 *  Revision 1.45  2006/03/04 12:50:30  ambrmi09
 *  Struggling with ANSi time constants
 *
 *  Revision 1.44  2006/03/04 11:30:00  ambrmi09
 *  Runds under Cygwin now.
 *  @note. Threads seem to need a very big stack under Cygwin (reason unknown),
 *
 *  Revision 1.43  2006/03/03 16:57:32  ambrmi09
 *  Fust successfull run under Linux. I recon the ABI is correct, but needs verifying. If Im right, this should run on Cygwin as well
 *
 *  Revision 1.42  2006/03/02 17:28:07  ambrmi09
 *  Eclipse forces me to make a stupid patch in the top Makefile.
 *
 *  Revision 1.41  2006/03/02 15:33:08  ambrmi09
 *  Experimenting with build under Linux
 *
 *  Revision 1.40  2006/03/02 14:05:49  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.39  2006/02/28 18:59:23  ambrmi09
 *  Minor adjustments and verified that all (intended) build environment
 *  compile and produce runnable code. I.e.:
 *  - uVisionKernelProject
 *  - BC5
 *  - Core and POSIX projects in MSVC
 *
 *  Revision 1.38  2006/02/28 13:18:20  ambrmi09
 *  Got MSVC target back-on track also.
 *
 *  Seems both MSVC and BCC have the same "error" regarding \ref CLK_TICK
 *  and/or \ref clock(). Could it be that it's TinKer's interpretation that
 *  is in fact wrong?
 *
 *  Revision 1.37  2006/02/28 11:50:07  ambrmi09
 *  - Trimmed the time constants (ruffly). 4sek per 14hrs drift
 *  - Revived the Borland C (BC5) target. Projectfile also added (BC5.ide)
 *  - Started experimenting with a indlude filename macro, that has the
 *    the potential of solving my ANSI header/function dilemma (\ref
 *    BUILDCHAIN )
 *  - Some "fishyness" about BC5 handling of time. Either \ref clock or
 *    \ref CLK_TCK doesn't follow standard (the latter I know for a fact,
 *    since it's 1e3 instead of 1e6 - but thats not all). \ref tk_msleep is
 *    adjusted to try to see the error.
 *
 *  Revision 1.36  2006/02/27 13:30:04  ambrmi09
 *  <b>Please read the in depth comments</b> about this check-in at \ref
 *  Blog051125
 *
 *  The test program (test.c) in this check-in is also particularly nasty
 *  since it gives really long latencies on each task switch (up to and
 *  above 500mS!). Test against this if you make any changes in either
 *  timing or dispatching. Even considering this hard case, the drift was
 *  very minor. The timing constants need re-trimming though.
 *
 *  Revision 1.35  2006/02/25 14:44:30  ambrmi09
 *  Found the nasty \ref BUG_000_001. Solution is robust but potentially degrades
 *  tinkers timing presition.
 *
 *  Found another bug caused by wraparound, that occures once every 71.6 minuts
 *  but selcom shows itself.
 *
 *  @note  that systimer (sys_mickey) wraps once every 49.7 days but
 *  kernel native time-keeping wraps 1000 times more often (71.6
 *  minutes). This is due to that current precision on sys_time is in
 *  mS, but kernel precision is in uS as a preparation to that the
 *  \ref clock will be replaced by a higher precision time function
 *  (\uptime or something similar).
 *
 *  prepared for better presision clock (true uS presision).
 *
 *  Revision 1.34  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.33  2006/02/21 22:10:32  ambrmi09
 *  - Added wrapper macro for pthread_create so that posix threads get named in
 *    TinKer (makes post-mortem easier). Very cool solution with a macro...
 *  - Improved post-mortem, the schedule gets dumpt also now
 *  - Wrapper macros for msleep and usleep (temporary)
 *  - Minor stubbing and wrapping of mq_unlink and pthread_cancel
 *  - Added a new test program (t est-posix.c ). This is verifyed to compile and
 *    run on both Linux and TinKer unmodified!
 *
 *  Revision 1.32  2006/02/20 19:17:14  ambrmi09
 *  - Made the errno variable thread specific (each thread has it's own)
 *  - Hid the details of using errno so that setting and reading it looks
 *    like using a normal variable
 *  - Extracted some stuff from tk.h that doesn't need to be public
 *  - Implemented perros and strerror including a storage with all the error
 *    strings (will go into NV ROM on a embedded system).
 *
 *  Revision 1.31  2006/02/20 15:22:01  ambrmi09
 *  Documentation stuff. No code changes.
 *
 *  Revision 1.30  2006/02/19 22:00:38  ambrmi09
 *  Major brake-through!!! First working attempt with crude pThreads and
 *  POSIX RT queues works. (jihaa) :=D. Wow
 *
 *  Revision 1.29  2006/02/19 12:44:33  ambrmi09
 *  - Documented ITC
 *  - Started to build up the structure for the \ref PTHREAD component
 *
 *  Revision 1.28  2006/02/16 15:11:00  ambrmi09
 *  Introduced a new component for better and safer useage of the heap.
 *  Package is called \red KMEM and the files are tk_mem.c and tk_mem.h (so
 *  far).
 *
 *  Started to take care of the long needed issue with error codes and
 *  better error handling. Introduced errno.h to begin with, whitch is part
 *  of the package \ref kernel_reimpl_ansi. Its not a good solution yet,
 *  since both kernel and ANSI codes are in the same file we have to invent
 *  a way to omit the ANSI defines when a tool-chain that has errno.h is
 *  used.
 *
 *  Revision 1.27  2006/02/14 16:14:06  ambrmi09
 *  Bsearch implemented, a lot of doc regarding \ref qsort \ref bsearch
 *  \ref _tk_qsort and \ref _tk_bsearch is added.
 *
 *  Also the qsort/bsearch example from GNU libc ref manual is copy &
 *  pasted into the root thread.
 *
 *  Revision 1.26  2006/02/02 15:51:02  ambrmi09
 *  A lot of thought has been invested into the new PTIME component. Had to
 *  change things even in the systime parts (integrated in the SHEDUL
 *  component) to make it more generic. Think this will be really nice when
 *  it's ready, but has been a long road to get PTIME running (and I'm
 *  still not there).
 *
 *  Revision 1.25  2005/12/04 15:48:52  ambrmi09
 *  API for ne pre-emptable timers in place. Implementing this will be a
 *  hard but fun "nut" to crack. ptime has the potential of comming
 *  very close to the high-res timers that POSIX 1003.1c define and is a
 *  good pointer whether pthreads is a good next step or not for TinKer.
 *
 *  Revision 1.24  2005/12/03 14:04:30  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *  Revision 1.23  2005/12/02 14:40:51  ambrmi09
 *  First version of what is going to be the prefered way of preemptive
 *  coding. TinKer needed very little modifications to be able to do this.
 *  Only one function is added tk_event_yield(), which is to be uused from any ISR.
 *
 *  New test app is also provided for testing and validating preemptiveness:
 *  test-preepmt.c
 *
 *  Revision 1.22  2005/12/02 09:44:10  ambrmi09
 *  A better version on the "aternative method" of preemtive context swithing.
 *  This one actually works (if you skip the last part where R7 is popped back).
 *  Once again, checked in to seve as a reminder if the new method I have in mind
 *  turns hout to have drawbacks.
 *
 *  This method has one major drawback (even if I get it to work), and that is that
 *  validation of CPU context after switching back is very hars and close to
 *  impossible. The principle of the code checked in now should be clear, but
 *  at least one register is NOT restored properlly - and that is PSW. That could
 *  prolly be handled the same way as R7, byt the more complicated this mechanism
 *  gets, the harder it is to validate - it takes hours just to cover one iteration
 *  (and you need "a lot" of iterations to be even fairlly confident).
 *
 *  Revision 1.21  2005/12/02 07:52:24  ambrmi09
 *  Working snap of preemtive sceduling. Comments saved of an alternative way
 *  (which has many advantages but is harder to get running).
 *
 *  Revision 1.20  2005/12/01 13:05:25  ambrmi09
 *  This check-in if for preparing for peemtive mechanism (first try)
 *  Done since last check-in
 *
 *  - Got rid of a nasty include bug for the target dependant sys-files
 *  - Added yet anoter sys header file with type only, so that kernel doesn't
 *    have to include the whole lot (which will render in  another "bug" due
 *    to that Keil and Dave define the same thing. This might be solved
 *    differently later, but the separation doesnt hurt anyway.
 *  - Started a concept of system queues - which I hope will be the basis for
 *    the TinKer drivers concept (yet to be invented).
 *  - Made first crude attempts with preemtion.
 *
 *  Revision 1.19  2005/11/30 22:21:22  ambrmi09
 *  Mechanism for detecting stack integrity violation introduced. It needs more
 *  work. An interrupt will taint the current stack if it's using any kernel
 *  functions. This is not what we want, but the main idea is captured in this
 *  check-in.
 *
 *  Revision 1.18  2005/11/27 20:02:00  ambrmi09
 *  - Detection of UserStack out-of bounds detection added.
 *  - Added error generic handling mechanism (TRAP)
 *  - Several new macros (that also need to be ported btw)
 *  - Tested error mechanisms for stacks quite well. Those issues should be OK now
 *
 *  Revision 1.17  2005/11/27 15:07:25  ambrmi09
 *  Found a serious bug about how user stacks lacalization. This is an intermediate
 *  but working check-in. Need fixing for handling word boudary overfolw.
 *
 *  Also Traps are made a little bit more robust in case DPP0:R0 are trashed.
 *  DPP0 is deducted in such case, but could need some extra info about good value
 *  for R0.
 *
 *  Revision 1.16  2005/11/26 16:13:19  ambrmi09
 *  Relevant for XC167 only.
 *
 *  Code for protecting system stacks added. Noted that if recursed functions
 *  used withing a thread and local variables are on user stack, that user stack
 *  will grow much more rapidly. This will lead to a kernel break that
 *  STKOV/STKUN can't catch. Suggestion:
 *  1. Let dispatcher set current user stack pointer on each context switch also
 *  2. Check agains max,min values against info in TOB, and try to
 *     detect out of boundary usage (high risk stack is broken allready though
 *     but hopefuly not for the current thread or we will not be able to output
 *     any diagnosics at all).
 *  3. If out of bouds detected, create a soft trap (for consistency with
 *     STKOF/STKUN error handling).
 *  4. Improve post mortem info. IP and CSP can't possibly be correct now. Also
 *     dump whole CPU and the whole current TCB. Try to look up adress in symbol
 *     table and print function name if possible.
 *  5. LVM and HVM (water marks) might be combined with pkt2.
 *
 *  Deliberate "bug" left in code to help my memory while creating debugging
 *  situation (0xA0).
 *
 *  Revision 1.15  2005/11/26 11:53:51  ambrmi09
 *  Made context switches of stack-pointers more consistent with other stack info.
 *
 *  Revision 1.14  2005/11/26 11:38:40  ambrmi09
 *  Cosmetic changes concerning CVS logs in source.
 *
 *  Revision 1.13  2005/11/25 21:20:03  ambrmi09
 *  Post mortem will now output the correct thread ID that broke.
 *
 *  A quick-n dirty solution for reading stdin is in place. Real files need to
 *  be implemented soon ;)
 *
 *  Revision 1.12  2005/11/25 18:06:40  ambrmi09
 *  Unstability bug found. Didnt think about that lot of time is spent in the
 *  idle loop and that interrupts will quite likelly put RET adresses there.
 *  By giving thread a fair stack, unstability was removed. Make a mental note
 *  about this, since it will happen again (with faster ISR's).
 *
 *  Post mortem was not succesfull in printing out the correct thread. It reported
 *  (root) thread, but I'm pretty syre problem was (idle) thread.
 *
 *  Revision 1.11  2005/11/25 17:55:29  ambrmi09
 *  Detection of a free-running kernel. Output post-mortem dump, then wait
 *  for real reset.
 *
 *  Revision 1.10  2005/11/25 14:35:16  ambrmi09
 *  A first naive aproach of ISR to thread syncronisation mechanism. It works
 *  but it is unstable.
 *
 *  Revision 1.9  2005/11/24 19:40:12  ambrmi09
 *  Lots of kernel API name changing. To support consistency and to prepare for
 *  the pthreads port.
 *
 *  Revision 1.8  2005/11/23 20:46:43  ambrmi09
 *  Finally stacks seems OK. A bit worried about some "garbage" that turns up
 *  at each TOS at each tasks start
 *
 *  Revision 1.7  2005/11/23 11:31:06  ambrmi09
 *  New stack structure is in place. Now all we have to do should be to just
 *  attach DDP2:R0 to the user_stack part of it, and we "should" be done with
 *  the XC167 stack bug.
 *
 *  Revision 1.6  2005/11/23 07:47:44  ambrmi09
 *  Simple namechange
 *
 *  Revision 1.5  2005/11/22 23:33:54  ambrmi09
 *  New stack_t in place and tested for XC167. We seem to have some include confilict also between regs166.h and main.h (both system includes)
 *
 *  Revision 1.4  2005/11/22 20:07:56  ambrmi09
 *  Separated architecture specific code by introducing tk_hwsys_<TARGET>.h
 *  files.
 *  Prepared for new stack_t type (not introduced yet).
 *
 *  Revision 1.3  2005/11/18 13:18:27  ambrmi09
 *  Finally got the timing right (tested and verifyed). Amazing accurancy!
 *  accurancy from 60000mS to 1mS (executed 10000 ti,es) both show the same
 *  constant error (that they are equal can only be explained due to the
 *  meassuring method). Still need to work some on getting portability between
 *  fast and slow targets right so trimming of kernal will be easy.
 *
 *  Revision 1.2  2005/11/18 11:23:32  ambrmi09
 *  Starting to document and to clean up kernel internals.
 *
 *  Revision 1.1.1.1  2005/11/17 09:59:09  ambrmi09
 *  Created CVS jhome for TinKer from scratch. RCS history lost in database (kept in sourcecode)
 *
 *  Revision 1.6  1998/02/16 18:03:41  mickey
 *  Added tk_assertFail
 *
 *  Revision 1.5  1998/02/14 10:35:42  mickey
 *  tk_exit added
 *  Handling av wakeup event. Needed for ITC to be able why a ITC object was
 *  released.
 *
 *  Revision 1.4  1998/02/01 20:03:29  mickey
 *  Jiihaa, first version of a working semaphore
 *
 *  Revision 1.2  1998/01/28 21:32:27  mickey
 *  First working revision after tidying, with both Borland and Codewright IDE
 *
 *  Revision 1.1  1998/01/28 20:12:08  mickey
 *  Initial revision
 *
 *
 *******************************************************************/
