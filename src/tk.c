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
@ingroup SCHED

@brief Tinker inner-most \e"guts"

This comonent is not selectable as the others normally are

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
#include <tk_hwsys.h>
#include <../src/implement_tk.h>

#ifndef printk
#   error Tinker needs a prink to be implemented for this target
#endif

//#if !(CLK_TCK == CLOCKS_PER_SEC)
//#error CLK_TCK == CLOCKS_PER_SEC. Target not following POSIX standard
//#endif

/*!
@name \COMPONENTS headerfiles

Included conditionally to avoid complication if work is in progress in
any of them.   errno.h

@see COMPONENTS
*/
//@{
#if defined(TK_COMP_KMEM) && TK_COMP_KMEM
   #include <tk_mem.h>
#endif

#if defined(TK_COMP_ITC) && TK_COMP_ITC
   #include <tk_itc.h>
#endif

#if defined(TK_COMP_PTIME) && TK_COMP_PTIMER
   #ifndef TK_COMP_PTHREAD
      #error Component PTIMER is dependant of PTHREAD
   #endif
   #ifndef TK_COMP_POSIX_RT
      #error Component PTIMER is dependant of POSIX_RT
   #endif
   #include <tk_ptimer.h>
#endif

#if defined(TK_COMP_PTHREAD) && TK_COMP_PTHREAD
   #ifndef TK_COMP_ITC
      #error Component POSIX_RT is dependant of ITC
   #endif
   #include <pthread.h>
   #undef main
#endif

#if defined(TK_COMP_POSIX_RT) && TK_COMP_POSIX_RT
   #ifndef TK_COMP_PTHREAD
      #error Component POSIX_RT is dependant of PTHREAD
   #endif
   #ifndef TK_COMP_ITC
      #error Component POSIX_RT is dependant of ITC
   #endif
   #include <semaphore.h>
   #include <mqueue.h>
#endif

//@}

//The sysqueues should maybe be a component?
#include <tk_sysqueues.h>

#include "tk_hwsys.h"




/*- local definitions **/
/*
#if defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
   #define tk_clock()  (clock() * 10)
#else
   #define tk_clock()  clock()
#endif
*/
#define tk_clock()  clock()

/* default settings */

/*- external functions **/
//Temporary - is not portable
extern void _tk_reinit_stackaddr_xc167keil( stack_t *addr, size_t size );

/*- external data **/

/*- internal functions **/
void *_tk_destructor( void *foo );
void *_tk_idle( void *foo );
void  _tk_detach_children(thid_t thid);
int   _tk_try_detach_parent( thid_t, int);

/*- public data **/

 /*- private data **/

int Tk_IntFlagCntr;

/*- public functions **/

/*- private functions **/


#define DEBUG
#ifdef DEBUG
#define static
#endif

/*!
@brief The main storage of all TCB

@note The index in this array \b is the thread ID.

This is not perfect. PiD will be reused when more than TK_MAX_THREADS has
been used. I would have wanted a Thid range that is much bigger and a lookup-
table (gives greater uniqueness - specially useful for \ref ITC) but that
is costly (I think)...

@note that this is only the pool of TCB's, the schedule itself is \ref theSchedule 

coord_t lookUpTable[TK_MAX_THREADS];

*/
struct tcb_t proc_stat[TK_MAX_THREADS];  


/*!
@brief The two dimensional dispatch-schedule

This two dimensional matrix constitute a schedule (imagine a real life
daily schedule if you like) that determines how exactly each thread is
to be handled by the dispatcher on each iteration.

Matrix layout
- First index (X) is the priority
- Second index (Y) is a list of treads at that priority (in various states, i.e. both "running", "blocked" and whatnot). 

*/
static thid_t theSchedule[TK_MAX_PRIO_LEVELS][TK_MAX_THREADS_AT_PRIO];
static struct stat_t scheduleIdxs[TK_MAX_PRIO_LEVELS];

static thid_t active_thread;        //!< Deliberatlly left uninitialized to support post mortem analysis
static thid_t thread_to_run   = 0;
static thid_t procs_in_use    = 0;
static thid_t proc_idx;             //!< Points at the last TCB created in the \ref proc_stat pool
static thid_t idle_Thid;            //!< Idle_Thid must be known, therefor public in this module (file)

void *_tk_idle( void *foo ){       //!< idle loop (non public)
   TK_NO_WARN_ARG(foo);
   while (TRUE){
      tk_yield();
   }
}

void tk_delete_kernel( void ){
      tk_delete_thread(idle_Thid);
}

thid_t tk_thread_id( void ){
   return(active_thread);
}

/*!
@ingroup kernel_glue

@brief Gives a way to enter my own threads TCB

This gets the current threads internal thread control block (TCB). Function is
"public" but is <b>not</b> ment to be used by application developers, rather as
a way for different layers of TinKer to interact.

@note <b>For TinKer internal use only</b>

*/
tk_tcb_t *_tk_current_tcb( void ){
   return(&proc_stat[active_thread]);
}

/*!
@ingroup kernel_glue

Same as tk_current_tcb but gets info for a specific thread
*/

tk_tcb_t *_tk_specific_tcb( thid_t id ){
   return(&proc_stat[id]);
}

/*! 
@ingroup kernel_glue

This function will give you access to the current threads internal errno 
variable.

@see errno
*/
int *_tk_errno(){
   return &(proc_stat[active_thread]._errno_);
}

/*!
@ingroup kernel

Creates the kernel. This is TinKers "init" funtion. All targets need to run
this once, but only once.

@todo <b>Figure out why the malloc(1) needs to be there.</b> I susbect eiter a byte-boundary probmem, a XC167 specific pointer prob (maybe in conjunction with the stacp pointer types), or a error in malloc itself
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
      printk("Error: Kernel running amok detected\n");
      printk("Broken thread was ID=%d (name=\"%s\")\n",\
         active_thread,proc_stat[active_thread].name);
      memset (testArea, '\0', TSTSZ);     //Clear area then wait for reset
      tk_exit(TC_AMOK);
   }else{
      strncpy(testArea,testPatt,TSTSZ);
   }
   */

   for (i=0; i<TK_MAX_THREADS; i++){
      proc_stat[i].state                        = ZOMBI;
      proc_stat[i].bOnId                        = -1;
      proc_stat[i].wakeuptime                   = 0;
      proc_stat[i].valid                       = FALSE;
      proc_stat[i].name[TK_THREAD_NAME_LEN]     = 0;
      proc_stat[i].Gid                          = 0;
      proc_stat[i].Thid                         = 0;
      proc_stat[i].noChilds                     = 0;
      proc_stat[i]._errno_                      = 0;
      proc_stat[i].stack_size                   = 0;
      STACK_PTR(proc_stat[i].stack_begin)       = NULL;      
      STACK_PTR(proc_stat[i].curr_sp)           = NULL;      
      proc_stat[i].stack_crc                    = 0;      
      proc_stat[i].wakeupEvent                  = 0;
      proc_stat[i].retval                       = (void*)0;
      
/*
      proc_stat[i].start_funct                  = NULL;
      proc_stat[i].init_funct                   = NULL;
      proc_stat[i].prmtRetAddr                  = NULL;
*/
   }
   //The Root proc is already created but must be registred
   proc_stat[0].state = READY;
   proc_stat[0].valid = TRUE;
   strcpy(proc_stat[0].name,"root");
   procs_in_use = 1;
   proc_idx = 0;
   //Clear the tables (before creating idle - thread)
   for (i=0; i<TK_MAX_PRIO_LEVELS; i++){
      //Clear the help table
      scheduleIdxs[i].procs_at_prio = 0;
      scheduleIdxs[i].curr_idx = 0;
      for (j=0; j<TK_MAX_THREADS_AT_PRIO; j++){
         //init the table to root everywhere
         theSchedule[i][j]=0;
      }
   }
   //Create a Idle thread, whoes sole purpose is to burn up time
   //when nobody else is running
   idle_Thid = tk_create_thread("idle",TK_MAX_PRIO_LEVELS - 1,_tk_idle,(void*)NULL,0x0600/*MINIMUM_STACK_SIZE*/);
   //IdleProc must like root, i.e. bee owned by itself
   proc_stat[proc_stat[idle_Thid].Gid].noChilds--;
   //Awkward way to say that root has created one process less than it has
   proc_stat[idle_Thid].Gid = proc_stat[idle_Thid].Thid;
   //Idle ownde by it self
   //Init shedule tables
   //Put the root in the scedule
   scheduleIdxs[0].procs_at_prio = 1;
   theSchedule[0][0]=0;
   //Put idle thread in shedule at lowest prio
   /*
   scheduleIdxs[TK_MAX_PRIO_LEVELS - 1].procs_at_prio = 1;
   theScedule[TK_MAX_PRIO_LEVELS - 1][0]=idle_Thid;
   */ /* NOT NEEDED, DONE ALREADY */
}
unsigned long des_retval;   
unsigned long des_temp;

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
        

@todo We need to figure out another way to do prepare TOS. Getting all this 
right for each target is a real pain in the a**.

*/

#if defined(_WIN32) &&  defined(_MSC_VER)
__declspec(naked) void   * _tk_destructor( void *foo ){
#else
void *_tk_destructor( void *foo ){
#endif
   GET_THREADS_RETVAL( des_retval, des_temp );
   
   TK_NO_WARN_ARG(foo); 
   
   
   #ifdef DEBUG
   //printk("Dieing thread [id=%d] is returning! Return value is: 0x%lX\n\n",active_thread,des_retval);  
   #endif
   
   
   proc_stat[active_thread].state  = ZOMBI;
   proc_stat[active_thread].retval = (void*)des_retval;
   _tk_detach_children(active_thread); 
   
   /*Finalize the destruction*/
   //tk_delete_thread(active_thread);
   //tk_yield();

   while (TRUE){
      tk_msleep(1000);
      #ifdef DEBUG
      printk("Dead thread [id=%d] still waiting for Nirvana! \n",active_thread);
      #endif
   }
   //return(0);
}

/*!
@ingroup kernel

Delete a thread

@note be carefull about killing running threads. Any non-kernel resources
allocated will not be freed. Always prefere a controlled exit of a thread.
*/
int tk_delete_thread(
   thid_t Thid              //!< Threads identity
){
   unsigned int Prio,Idx,i;
   int bury_it = TRUE;
   
   if ( proc_stat[Thid].valid == FALSE )  {
      //The process you are trying to delete does not exist
      return(TK_ERROR);    
   }

   
   if (!(proc_stat[Thid].state & ZOMBI)){ //If not self terminated, but killed explicitlly - we need to perform some duties
      proc_stat[Thid].state = ZOMBI;
      _tk_detach_children(Thid); 
      bury_it = _tk_try_detach_parent(Thid, FALSE); 
   }
   
   if (bury_it){   //Can we buy the damned thing allready? or do we have to try another round?
      Prio = proc_stat[Thid].Prio;
      Idx = proc_stat[Thid].Idx;
      assert(theSchedule[Prio][Idx] == Thid);
      procs_in_use--;
      //Take away the process from theSchedule and compress gap at prio
      for(i=Idx; i < scheduleIdxs[Prio].procs_at_prio; i++){
         theSchedule[Prio][i] = theSchedule[Prio][i+1];
         proc_stat[theSchedule[Prio][i]].Idx = i;
      }
      //#if DEBUG
      // not needed, could cause problems if procs last..
      //theSchedule[Prio][i] = 0;
      //#endif
      //Take away the process fom scheduleIdxs
      scheduleIdxs[Prio].procs_at_prio --;
      if (scheduleIdxs[Prio].curr_idx >= scheduleIdxs[Prio].procs_at_prio){
         scheduleIdxs[Prio].curr_idx = 0;
      }
      //Make it final - remove it's primary resource: it's stack
      stalloc_free( STACK_PTR(proc_stat[Thid].stack_begin) );
      proc_stat[Thid].valid = FALSE;
   }
   
   return(TK_OK);
}

int tk_detach(
   thid_t Thid              //!< Threads identity
){
   if ( proc_stat[Thid].valid != TRUE)
      return ESRCH;
      
   if ( proc_stat[Thid].Gid != -1)
      return EINVAL;
      
   _tk_try_detach_parent(Thid, TRUE); //forcefully detach
   return 0;
}


/*
Storage variable - to be used in macros manipulating stacks. These can't be on 
the local stack since we are manipulateing that.
*/

static char *ct_oldTOS;    //!< will contain old top of stack adress
static char *ct_newSP;     //!< new stack-pointer. Is a return value storage from macro
//static char *ct_temp1;     //!< fony temporary stackpointer used in the process of setting TOS
static unsigned long ct_temp1;  //!< fony temporary stackpointer used in the process of setting TOS
static unsigned long ct_temp2;  //!< Extra storage. For some targets used to manipulate segment part of stackpointers
static unsigned int ct_temp3;   //!< Extra storage. 
static stack_t ct_stack_struct; //!< Will be changed in macro. Never use outside of it (don't trust it)

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
   //where in theScheduler to put thread id
   thid_t   slot_idx = scheduleIdxs[prio].procs_at_prio;
   start_func_f  *f_p;
   void          *v_p;
   size_t         real_stack_size;
   unsigned int   error = 0;
   #ifdef DEBUG
   unsigned int   i;
   #endif

   //Error handling needs improvment (don't forget taking special care of
   //proc_idx)
   if (procs_in_use >= TK_MAX_THREADS){
      printk("tk: Error - Total amount of threads would exceed limit\n");
      error |= TC_MAX_THREADS;   
   }
   //Check if chosen prio is within limits
   if (prio >= TK_MAX_PRIO_LEVELS){
      printk("tk: Error - Chosen priority too high\n");
      error |= TC_MAX_PRIO_LEVELS;   
   }
   //Check if there will be enough room at that prio
   if (TK_MAX_THREADS_AT_PRIO <= ( scheduleIdxs[prio].procs_at_prio ) ){
      printk("tk: Error - To many threads at this prio\n");
      error |= TC_THREADS_AT_PRIO;   
   }
   if (error){
      tk_exit(error);
   }
   
   //Find next empty slot - which is also the CREATED Thid
   do{
      proc_idx++;
      proc_idx %= TK_MAX_THREADS;
   }while (proc_stat[proc_idx].valid == TRUE);

   //Test if the assigned name fits   
   //In case highly optimized kernel, no names stored at all and no timly string copy
   #if TK_THREAD_NAME_LEN
      if (strlen(name)<TK_THREAD_NAME_LEN)
         strncpy(proc_stat[proc_idx].name,name,TK_THREAD_NAME_LEN);
      else{
         printk("tk: Error - Thread-name to long\n");
         tk_exit(TC_NAME_LEN);
      }
   #endif
   proc_stat[proc_idx].name[TK_THREAD_NAME_LEN] = 0; //Terminate string just in case it's needed. Note, there is one extra byte so indexing like this is OK.
   
   //Try to allocate memory for stack
   if (( STACK_PTR(proc_stat[proc_idx].stack_begin) = (char *) stalloc(stack_size)) == NULL){
       printk("tk: Error - Can't create process (can't allocate memory for stack)\n");
       tk_exit(TC_NOMEM);  // terminate program if out of memory
   }

   REINIT_STACKADDR( proc_stat[proc_idx].stack_begin, stack_size );
   
   proc_stat[proc_idx].valid       = TRUE;
   proc_stat[proc_idx].state        = READY;
   proc_stat[proc_idx].bOnId        = -1;
   proc_stat[proc_idx].Thid         = proc_idx;    //for future compability with lage Thid:s
   proc_stat[proc_idx].Gid          = active_thread; //Owned by..   
   proc_stat[proc_idx].noChilds     = 0;
   proc_stat[proc_idx]._errno_      = 0;
   proc_stat[proc_idx].stack_size   = stack_size;
   proc_stat[proc_idx].Prio         = prio;
   proc_stat[proc_idx].Idx          = slot_idx;
   proc_stat[proc_idx].stack_crc    = 0;

   proc_stat[proc_idx].wakeupEvent  = 0;
   proc_stat[proc_idx].retval       = inpar;  //This is convention and covers a special case
   /*
   proc_stat[proc_idx].start_funct  = f;
   proc_stat[proc_idx].init_funct   = NULL;
   proc_stat[proc_idx].prmtRetAddr  = NULL;
   */

   proc_stat[active_thread].noChilds++;
   procs_in_use++;

   #ifdef DEBUG
   for (i=0;i<stack_size;i++)
      STACK_PTR(proc_stat[proc_idx].stack_begin)[i] = (unsigned char)proc_idx;
   #endif
   //Here's the secret.
   //preparing the stack  
   //=============================
   //#0x4=inpar (fony pushed param)    //The stack looks like calling schedule
   //return adress to "_tk_destructor"     //from function _tk_destructor
   //#4=return adress (which is also calling adress of function thread)
   //#4=EBP
   //#0x4=pushf
   //#0x20=pusha 
   
   real_stack_size = REAL_STACK_SIZE(proc_stat[proc_idx].stack_begin);

   v_p = (void *)&STACK_PTR(proc_stat[proc_idx].stack_begin)[real_stack_size - 0x4];
   *(unsigned int*)v_p = (unsigned int)inpar;

   f_p = (start_func_f *)&STACK_PTR(proc_stat[proc_idx].stack_begin)[real_stack_size - 0x8];
   *f_p = _tk_destructor;

   f_p = (start_func_f *)&STACK_PTR(proc_stat[proc_idx].stack_begin)[real_stack_size - 0xC];
   *f_p = f;

   ct_oldTOS = &STACK_PTR(proc_stat[proc_idx].stack_begin)[real_stack_size - 0xC];
   ct_stack_struct = proc_stat[proc_idx].stack_begin;

   //MARKALL();
   
   PREP_TOS( ct_oldTOS, ct_newSP, ct_temp1, ct_temp2, ct_stack_struct );
    //#pragma src
    //#pragma asm                                                                                                 
   //   MOV R1,R5                                                                                                
    //#pragma endasm  

   //Assign the stackpointer to top of stack
   //proc_stat[proc_idx].sp = &proc_stat[proc_idx].stack[stack_size - 0x34];
   STACK_PTR(proc_stat[proc_idx].curr_sp) = ct_newSP;
   //Put process in scedule - assume tight tight schedule
   theSchedule[prio][slot_idx] = proc_idx;
   //Increase the amount of procs at same prio
   scheduleIdxs[prio].procs_at_prio++;
   
   //Make a integrity certification on the stack of this thread
   //INTEGRITY_CERTIFY_STACK(proc_stat[proc_idx], ct_temp3);
   return proc_idx ;
}

int tk_join(thid_t PID, void ** value_ptr){
   if ( proc_stat[PID].Gid != active_thread)    //Only threads own children can be joined in TinKer (restrictive)
      return EINVAL;                            
      
   if ( proc_stat[PID].valid != TRUE)
      return ESRCH;
      
   if (proc_stat[PID].state & ZOMBI){ //Thread is finished allready. no need to block AND we can safelly read it's return value
      if (value_ptr != NULL) {
         *value_ptr = proc_stat[PID].retval;
      }
      //Also release it for cancelling. This is done by breaking it's 
      //parent-child relationship
      _tk_try_detach_parent(PID, TRUE); //forcefully detach. This will also count down the child count in the parent

   }else{
      //prepare to block   
      proc_stat[active_thread].bOnId = PID;
      proc_stat[active_thread].state |= TERM;
      tk_yield();  //Will block
      assert(proc_stat[active_thread].wakeupEvent == E_CHILDDEATH);
      if (value_ptr != NULL) {
         //We can't read the TCB of the thread we were just blocked on. It migt allready be cancelled
         //However, It knew about this case when it awoke us - and made a copy in *our* retval instead. 
         *value_ptr = proc_stat[active_thread].retval;
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
   unsigned long clk_sek = CLK_TCK; 
   //unsigned long clk_sek = CLOCKS_PER_SEC;
   
   in_us = time_ms * 1000uL;   
   act_time_us    = tk_clock() * (1000000uL/CLOCKS_PER_SEC);
   wkp_time_us    = act_time_us + in_us;
   //wkp_time_us    = act_time_us + time_ms*1000uL;

   //need a function t_diff that handles wraparound (done 060225, note kept for ref.)
   proc_stat[active_thread].wakeuptime = wkp_time_us;
   proc_stat[active_thread].state |= SLEEP;
   /*
   Hmm... don't do the following unless zombie is checked. Pointless info, so we'll skip it
   proc_stat[active_thread].bOnId = active_thread;
   */
   tk_yield();
}


/*!
@brief create a "virtual" timeout event (whenever possible).

Iterates though the whole schedule, trying to find sleeping threads
who's timeout time has expired and flag them as \ref READY. 

@see PROCSTATE

In non-preemptive mode there is no way to wake up a sleeping thread any
other way. Therefore \b detection thereof must be done.

This is done mainly by the idle thread (it has nothing else to do
anyway) and in all the context-swappable points (i.e. in principle in
most of the public functions belonging to \ref SCHED ).

There those points are is easiest to find out by tracing the \ref
tk_yield function.

This concept is a heritage from the non-preemptive concept, but waking
up sleeping threads this way is still done. This will also be kept in in
the \ref idle thread even when \ref PTIMER package is finished.

When \ref PTIMER package is finished, the \ref tk_yield might change
behaviour, but the \ref idle will still not (to keep compatibility
towards the old \ref ITC). Note that timeout handling for \ref PTHREAD
is planned to be handled by \ref PTIMER.

Timing using this concept was (and still is) mainly intended for simple
blocking timeouts in \ref ITC and was never intended for high-precision
time keeping (even though that works quite well also).

 */
void _tk_wakeup_timedout_threads( void ){
   thid_t i;
   clock_t act_time_us;

   //act_time = tk_clock();
   act_time_us    = tk_clock() * (1000000uL/CLOCKS_PER_SEC);
   //Note: The following is not just a sanity check, the list might be 
   //fragmented.Explains also why the the whole table (pool) has to be 
   //travesesd, and not just [0..procs_in_use]
   for(i=0;i<TK_MAX_THREADS;i++){
      if (proc_stat[i].valid){           

         if (proc_stat[i].state & SLEEP){  //This one is sleeping. Time to wake him up?
            //if ( act_time >= proc_stat[i].wakeuptime ){
            //if ( (signed long)(act_time - proc_stat[i].wakeuptime) >= 0 ){
            if ( (signed long)(difftime(act_time_us,proc_stat[i].wakeuptime) ) >= 0 ){
               proc_stat[i].state &= ~_____QS_; /*Release ques also (but not the TERM bit)*/
               proc_stat[i].wakeupEvent = E_TIMER;
            }
         }
      
         //This thread wants to die (zombied). But make sure someone else is doing it 
         //(otherwize we would remove our own stack, which is both ugly and dangerous)
         if ((proc_stat[i].state & ZOMBI) && (i != active_thread)){ 
         
            //We shuld not really need to do the following line. Should 
            //have been done in any canceling mechanism of the thread.
            //Note and commented call saved for future reference.
            
            //_tk_detach_children(i); 
            
            
            if ( _tk_try_detach_parent(i, FALSE) ){
               //Finally, it should now be safe to send the zombied thread to Nirvana 
               tk_delete_thread(i);
            } 
            //else: The parent is either blocking on a brother or sister OR 
            //neither, but might consider blocking later. I.e. we have to stay 
            //zombied until it desides (i.e. either joins us, dies or someone detaches us)                      
         }
      }
   }
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
   
   if (  proc_stat[thid].noChilds > 0 ){  //Dang! We must make our own children parentless ;(
      for(j=0;(proc_stat[thid].noChilds>0) && (j<TK_MAX_THREADS);j++){
         if (proc_stat[j].valid && proc_stat[j].Gid == thid){
            /*Foud a poor sod*/
            proc_stat[j].Gid = -1;
            proc_stat[thid].noChilds--;
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

\returns The function will return succes (TRUE) or faliure (FALSE).

When we detach from a parent, we mean the following:

- That the parent can't join with (at least) us any longer.

- When \e thid exits, it can cancel emediatly and doesnt have to
consider any parent who \e might deside to join with us later.

The following rules apply to this function:

- Should it happen that a parent is allreadu joining us when this
function is called, we release it (no matter of the status of \e force
attribute).

- To detach a parent that might deside us later, we have to explicitly
say so by setting \e force to TRUE. Otherwize, detachement will only be
a succes if a parent is joining on us.

- If \e thid is allready detached once, the function alwas returns TRUE

*/   

int _tk_try_detach_parent(
   thid_t thid,                  //!< The thread id whos parent we are concerned with
   int force                     //!< Force detachment
){
   int succeded_unblocking_parent = FALSE;      
   
   if (  proc_stat[thid].Gid == -1 )
      return TRUE;                     //allready orphan
      
   // (if con't) Yes, I have a parent, but is it waiting for my death or not?         
   assert (proc_stat[proc_stat[thid].Gid].valid);  //sanity check
   //We must have faith now, that the Gid is actually the one that created us, and 
   //not a reused identity (which should not be possible to happen - mybe invent a 
   //snity check for this?)
   
                  
   //If waiting for me (not my bro or sis) death, unblock the parent. (i.e. parent executed a tk_join)
   if ((proc_stat[proc_stat[thid].Gid].state & TERM) && proc_stat[proc_stat[thid].Gid].bOnId == thid ){
      proc_stat[proc_stat[thid].Gid].state &= ~_______T; //Release the waiting parent
      proc_stat[proc_stat[thid].Gid].wakeupEvent = E_CHILDDEATH;
      succeded_unblocking_parent = TRUE;
      //We are likelly to get out of context before the parent can read our retval. Therefore make an
      //active copy to the parent's retval instead. The parent will know of this case and use that one 
      //instead of trying to read a potentially invalid TCB.
      proc_stat[proc_stat[thid].Gid].retval = proc_stat[thid].retval;
      
   } else
      succeded_unblocking_parent = FALSE;
      
   if ( (succeded_unblocking_parent == TRUE) || (force == TRUE) ) {
      proc_stat[proc_stat[thid].Gid].noChilds--;
      assert(proc_stat[proc_stat[thid].Gid].noChilds >= 0);
      proc_stat[thid].Gid = -1;
      return TRUE;
   } 
   
   return FALSE;               
}



/* @ingroup kernel_internal

Get the identity of the next thread to run.  Scheduling policy is used
to determine which one that would be. 
*/ 

thid_t _tk_next_runable_thread(){ 
   int idx,prio,cidx,/*midx,*/nbTry,loop,return_Thid,p_at_p; BOOL found;

   return_Thid  = idle_Thid; //In case no runnable proc is found...
   found        = FALSE;

   for(prio=0;prio<TK_MAX_PRIO_LEVELS && !found;prio++){ //prio from highets to lowest
      p_at_p = scheduleIdxs[prio].procs_at_prio;
      if (p_at_p != 0){ //No procs at prio to run.. prevent division by zero
         //Manual RR from current to next ready
         idx =(scheduleIdxs[prio].curr_idx);
         cidx = scheduleIdxs[prio].curr_idx;
         nbTry = scheduleIdxs[prio].procs_at_prio;                             
         for(  loop = 0;
               loop < nbTry && !found;
               loop++){
            if (proc_stat[theSchedule[prio][idx]].state == READY){
               return_Thid = theSchedule[prio][idx];
               found = TRUE;
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

*/
static char *cswTSP;          //!< fony temporary stackpointer used in the process of setting TOS
static unsigned int cswTEMP;  //!< Extra storage. For some targets used to manipulate segment part of stackpointers
static unsigned int cswTEMP2; //!< Extra storage.

void _tk_context_switch_to_thread(
   thid_t RID,                //!< Thread ID to switch to
   thid_t SID                 /*!< Thread ID to switch from. I.e. 
                                         current thread ID to put away in TCB*/
){
   //TRY_CATCH_STACK_ERROR( proc_stat[SID].stack_begin, cswTEMP2 );
   //INTEGRITY_CERTIFY_STACK( proc_stat[SID], cswTEMP2 ); //Certify the stack we're leaving from

   
   PUSH_CPU_GETCUR_STACK( cswTSP, cswTEMP );

   STACK_PTR( proc_stat[SID].curr_sp ) = cswTSP;
   
   cswTSP = STACK_PTR( proc_stat[RID].curr_sp );
   active_thread=RID;

   CHANGE_STACK_POP_CPU( cswTSP, cswTEMP );
   //TRY_CATCH_STACK_INTEGRITY_VIOLATION( proc_stat[active_thread], cswTEMP2 ); //Check integrity is OK before running 
}

/*!
Dispatch or yield to another thread that has more "right" to run.

I.e. <b>search</b> for another potential thread that is now in \e "runable"
state and that has higher priority. 

Also: In non-preemptive mode (and as a secondary duty), it creates simulated
timeout-events on all threads currently sleeping, but who's timeout has
expired (i.e. search for expired timeouts and change state to READY if needed).

@note In TinKer lingo we use the words
- \e "sleeping" - for tread blocked on timeout
- \e "blocked" - for thread blocked for any other reason

The concept of passively sleeping threads but actively awaken by the
kernel (in idle thread or any dispatch point) is a key-concept in
non-preemptable mode. In this mode the kernel is constantly searching
for who's ready to run. This is normaly done in a specially
dedicated - the \ref _tk_idle "idle thread" (i.e. the lowest priority
thread).

@note This is the function to put here and there in you application if
you run a non-preemptable version of TinKer. Any execution will not
be context switched until you either execute this function or run any
other kernel function. <b>Any other TinKer kernel function is also a
context switching point.</b>. If you don't want that behaviour you have
to use the non-yielded version of that function (i.e. with suffix -ny)
<p>

@note to use this function in <b>preemptable mode</b> you must make sure
that there is no prefix-postfix stuff happening on the stack, neither
before the first PUSHALL nor after the last POPALL.

*/
void tk_yield( void ){
   TK_CLI();   
   PUSHALL();   
   TK_STI();
   
   _tk_wakeup_timedout_threads();
   
   TK_CLI();
   
   //Do not premit interrupts between the following two. Proc statuses 
   //(i.e. thread statuses) frozen in time.
   thread_to_run = _tk_next_runable_thread();
   _tk_context_switch_to_thread(thread_to_run,active_thread);   
   
   POPALL();
   TK_STI();
}

/*!
Dispatch or \e yield to another thread that has more "right" to run.

This is basically identical with tk_yield(), but it doesn't try to
search for threads to wake up that are sleeping on non-preempting
timers. This function is meant to be used for preemptable scheduling
and to be called from an event source (i.e. ISR).

@see void tk_yield( void )

*/
void tk_yield_event( void ){
   TK_CLI();
   PUSHALL();
   thread_to_run = _tk_next_runable_thread();
   _tk_context_switch_to_thread(thread_to_run,active_thread);   
   POPALL();
   TK_STI();
}

/*!

Last thing that is called when a thread gives up live freely or when an
error of some sort happened (either kernel internal or user program
specific).

In case of en error, this function also acts as a critical error-handler
entr point (critical = execution is deemed to stop).

*/
void tk_exit( int ec ) {
   if (ec==0)
      printk("tk: Program terminated normally");
   else
      printk("tk: Warning - Program terminated with errorcode [%d]",ec);
   while (1) {
      TRAP(ec);
   }
}

/*!
@ingroup kernel_glue

Works as the assert macro exept that you have to use the __file_ and __line_
explicitlly. Typically the assert macro will be defined to call this function 
on targets that do not have assert implemented by TinKer.

*/
void _tk_assertfail(
   char *assertstr, 
   char *filestr, 
   int line
) {
   printk("tk: Error - Assertion failed: %s,\nfile: %s,\nline: %d\n",assertstr,filestr,line);
   tk_exit( TC_ERR_ASSERT );
}
/*
   testcall(regval);
0x00001c74 <_tk_main+16>:  ldr   r3, [r11, #-20]
0x00001c78 <_tk_main+20>:  mov   r0, r3
0x00001c7c <_tk_main+24>:  bl    0x1c44 <testcall>
   GET_SP(regval);
*/  
void *testcall(void *inpar ){
   return inpar;
}
/*
void *testcall(void *inpar ){
0x00001c44 <testcall>:    mov   r12, sp
0x00001c48 <testcall+4>:  stmdb sp!, {r11, r12, lr, pc}
0x00001c4c <testcall+8>:  sub   r11, r12, #4 ; 0x4
0x00001c50 <testcall+12>: sub   sp, sp, #4   ; 0x4
0x00001c54 <testcall+16>: str   r0, [r11, #-16]
   return inpar;
0x00001c58 <testcall+20>: ldr   r3, [r11, #-16]
}
0x00001c5c <testcall+24>: mov   r0, r3
0x00001c60 <testcall+28>: ldmia sp, {r3, r11, sp, pc}
*/

/*!
@ingroup kernel_glue

Called from your real main() or from your start-up code 

@note (for deeply embedded developers) You target have to fulfill two
things at least:

- malloc has to work and return valid pointers 
- You need a working printf to see run-time errors

*/
void _tk_main( void ){
/*Learning about ARM 
   unsigned int      regval; 
   char *cptr;

   testcall(regval);
   GET_SP(regval);
   GET_LR(regval);
   GET_PC(regval);

   
  Learning about ARM */

   tk_create_kernel();
   printk("ANSI timing constants:\n");
   #if  defined( __C166__ )
       printk("CLK_TCK=[%ld], CLOCKS_PER_SEC=[%ld]\n",CLK_TCK, CLOCKS_PER_SEC);
   #else
       printk("CLK_TCK=[%d], CLOCKS_PER_SEC=[%d]\n",CLK_TCK, CLOCKS_PER_SEC);
   #endif
   #if defined(TK_COMP_KMEM) && TK_COMP_KMEM
      assert( tk_mem() == ERR_OK );
   #endif
   
   #if defined(TK_COMP_ITC) && TK_COMP_ITC
      assert( tk_itc() == ERR_OK );
      assert(_tk_create_system_queues( ) == 0);
      #if defined(TK_COMP_PTIMER) && TK_COMP_PTIMER
         assert( tk_ptime() == ERR_OK );
      #endif
      #if defined(TK_COMP_PTHREAD) && TK_COMP_PTHREAD
         assert( tk_pthread_sched() == ERR_OK );
         assert( tk_pthread_sync()  == ERR_OK );
         #if defined(TK_COMP_POSIX_RT) && TK_COMP_POSIX_RT
            //TBD
         #endif
      #endif
   #endif
    
   root();

   #if defined(TK_COMP_ITC) && TK_COMP_ITC
      #if defined(TK_COMP_PTHREAD) && TK_COMP_PTHREAD
         #if defined(TK_COMP_POSIX_RT) && TK_COMP_POSIX_RT
            //TBD
         #endif
         assert( tk_pthread_sched_destruct() == ERR_OK );
         assert( tk_pthread_sync_destruct()  == ERR_OK );
      #endif
      assert( tk_itc_destruct() == ERR_OK );
      #if defined(TK_COMP_PTIMER) && TK_COMP_PTIMER
         assert( tk_ptime_destruct() == ERR_OK );
      #endif
   #endif
   
   #if defined(TK_COMP_KMEM) && TK_COMP_KMEM
      assert( tk_mem_destruct() == ERR_OK );
   #endif 
   tk_delete_kernel();
}


#if (defined(_WIN32)          &&  defined(_MSC_VER))             \
 ||  defined(__BORLANDC__)    || defined(__BCPLUSPLUS__)         \
 ||  defined(__CYGWIN32__)    || defined(__CYGWIN__) || defined(__GNUC__)
 
int main(int argc, char **argv);
int main(int argc, char **argv){ 
   _tk_main();
   TRAP(0);
}
#endif

//#define TK_STUBS
#ifdef TK_STUBS
long int stub_mickey = 0;
/*!
 When involved with porting, the \ref clock function is most often on implemented, wrong or just stubbed. 
  
 Since \ref clock is a esential part of the \ref SCHED internals, you need to have a stub that will do something  meantingful, so at least the dispaching will work.

 */
clock_t sclock(){   
   stub_mickey++;
    //stub_mickey=0;
   return (clock_t)stub_mickey;
}
#endif //TK_STUBS

/*
void Test_scheduler( void ){
   while (TRUE){
      _tk_context_switch_to_thread(T1,ROOT);
      _tk_context_switch_to_thread(T2,ROOT);
      _tk_context_switch_to_thread(T3,ROOT);
   }
}
*/

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
 *  Revision 1.55  2006-03-19 12:44:36  ambrmi09
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
