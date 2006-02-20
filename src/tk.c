/*******************************************************************
 *
 *  DESCRIPTION: tk.c Scheduler for TinKerOS.
 *
 *  AUTHOR: Michael Ambrus
 *                              
 *  HISTORY:    
 *
 *  Current $Revision: 1.31 $
 *
 *******************************************************************/
  
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

#include <tk.h>

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
   #include <tk_ipc.h>
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

/* default settings */

/*- external functions **/
//Temporary - is not portable
extern void _tk_reinit_stackaddr_xc167keil( stack_t *addr, size_t size );

/*- external data **/

/*- internal functions **/
unsigned int _tk_destructor( void *foo );
unsigned int _tk_idle( void *foo );

/*- public data **/

/*! 
Temporary solution for errno handling. Storage supplied for all (thread UNSAFE)

@see errno
*/
int _tk_daft_errno;  


/*- private data **/

int Tk_IntFlagCntr;

/*- public functions **/

/*- private functions **/


#define DEBUG
#ifdef DEBUG
#define static
#endif


tk_tcb_t proc_stat[TK_MAX_THREADS];
//This is not perfect. PiD will be reused when more than TK_MAX_THREADS has
//been used. I wold have wanted a Thid range that is much bigger and a lookup-
//table (gives greater uniquety - specially usefull i case of RPC) but that
//is costly (I think)
//coord_t lookUpTable[TK_MAX_THREADS];

typedef struct{
   unsigned short procs_at_prio;    //Used for optimizing sheduler.
   unsigned short curr_idx;
}prio_stat_t;

static unsigned int theSchedule[TK_MAX_PRIO_LEVELS][TK_MAX_THREADS_AT_PRIO];
static prio_stat_t scheduleIdxs[TK_MAX_PRIO_LEVELS];

static unsigned int active_thread;        //!< Deliberatlly left uninitialized to support post mortem analysis
static unsigned int thread_to_run   = 0;
static unsigned int procs_in_use    = 0;
static unsigned int proc_idx;             //!< Points at the last tk_tcb_t created
static unsigned int idle_Thid;            //!< Idle_Thid must be known, therefor public in this module (file)

unsigned int _tk_idle( void *foo ){       //!< idle loop (non public)
   while (TRUE){
      tk_yield();
   }
}

void tk_delete_kernel( void ){
      tk_delete_thread(idle_Thid);
}

unsigned int tk_thread_id( void ){
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

tk_tcb_t *_tk_specific_tcb( unsigned int id ){
   return(&proc_stat[id]);
}

/*!
@ingroup kernel

Creates the kernel. This is TinKers "init" funtion. All targets need to run
this once, but only once.
*/

#define TSTSZ 0x10
static const char testPatt[TSTSZ] = "TinKer is king!";
void tk_create_kernel( void ){
   int i,j;
   char *testArea;
   
   /*
   Detect if a kernel is running amok.    
   */
   testArea = malloc(TSTSZ);
   if (strncmp(testPatt,testArea,TSTSZ) == 0){   
      printf("Error: Kernel running amok detected\n");
      printf("Broken thread was ID=%d (name=\"%s\")\n",\
         active_thread,proc_stat[active_thread].name);
      memset (testArea, '\0', TSTSZ);     //Clear area then wait for reset
      tk_exit(2);
   }else{
      strncpy(testArea,testPatt,TSTSZ);
   }

   for (i=0; i<TK_MAX_THREADS; i++){
      proc_stat[i].state         = ZOMBIE;
      proc_stat[i].wakeuptime    = 0;
      proc_stat[i].isInit        = FALSE;
      //proc_stat[i].name        ="";
      proc_stat[i].Gid           = 0;
      proc_stat[i].Thid           = 0;
      proc_stat[i].noChilds      = 0;
      proc_stat[i].stack_size    = 0;
      STACK_PTR(proc_stat[i].stack_begin)   = 0uL;
      STACK_PTR(proc_stat[i].curr_sp)       = 0uL;
      proc_stat[i].stack_crc     = 0;
      proc_stat[i].prmtRetAddr   = NULL;
      proc_stat[i].wakeupEvent   = 0;
      proc_stat[i].start_funct   = NULL;
      proc_stat[i].init_funct    = NULL;

   }
   //The Root proc is already created but must be registred
   proc_stat[0].state = READY;
   proc_stat[0].isInit = TRUE;
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
   idle_Thid = tk_create_thread("idle",TK_MAX_PRIO_LEVELS - 1,_tk_idle,NULL,0x600/*MINIMUM_STACK_SIZE*/);
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

/*!
This function is entered as a result of a ret instruction from a thread. EAX
is passed as the return value. Not shure if it works on every processor 
*/
unsigned int _tk_destructor( void *foo ){
   unsigned int retval;
   GET_THREADS_RETVAL( retval );
   
   //This is critical, no more stack, will not work as is in a preemtive kernal
   #ifdef DEBUG
   printf("Dieing thread is returning %d\n\n",retval);
   #endif
   tk_delete_thread(active_thread);
   tk_yield();
   //should never tk_exit
   return(0);
}

/*!
@ingroup kernel

Delete a thread

@note be carefull about killing running threads. Any non-kernel resources
allocated will not be freed. Always prefere a controlled exit of a thread.
*/
int tk_delete_thread(
   unsigned int Thid              //!< Threads identity
){
   unsigned int Prio,Idx,i;
   
   if ( proc_stat[Thid].isInit == FALSE )  {
      //The process you are trying to delete does not exist
      return(TK_ERROR);    
   }
   Prio = proc_stat[Thid].Prio;
   Idx = proc_stat[Thid].Idx;
   procs_in_use--;
   proc_stat[Thid].state = ZOMBIE;
   //Tell the parent proc that one more child is gone if parent exists
   //else don't bother "" and then try to free "sleep until no workers"
   if (proc_stat[proc_stat[Thid].Gid].isInit) {
      proc_stat[proc_stat[Thid].Gid].noChilds--;
      if ( (proc_stat[proc_stat[Thid].Gid].noChilds == 0) &&
           ( proc_stat[proc_stat[Thid].Gid].state & _______T ) ) {
      }
   }
   //Take away the process fom theSchedule and compress gap at prio
   for(i=Idx; i < scheduleIdxs[Prio].procs_at_prio; i++){
      theSchedule[Prio][i] = theSchedule[Prio][i+1];
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
   //Make it final
   free( STACK_PTR(proc_stat[Thid].stack_begin) );
   proc_stat[Thid].isInit = FALSE;
   return(TK_OK);
}


/*
Storage variable - to be used in macros manipulating stacks. These can't be on 
the local stack since we are manipulateing that.
*/

static char *ct_oldTOS;    //!< will contain old top of stack adress
static char *ct_newSP;     //!< new stack-pointer. Is a return value storage from macro
static char *ct_temp1;     //!< fony temporary stackpointer used in the process of setting TOS
static unsigned long ct_temp2;  //!< Extra storage. For some targets used to manipulate segment part of stackpointers
static unsigned int ct_temp3;   //!< Extra storage. 
static stack_t ct_stack_struct; //!< Will be changed in macro. Never use outside of it (don't trust it)

/*!
@ingroup kernel

Creates a thread and puts it in runnable state (READY). Thread is not
run though, until next yeald (or interrupt in case of a preemtable
version). 

*/
unsigned int tk_create_thread(      
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
   unsigned int   slot_idx = scheduleIdxs[prio].procs_at_prio;
   start_func_f    *f_p;
   void          *v_p;
   size_t         real_stack_size;
   #ifdef DEBUG
   int            i;
   #endif

   //Error handling needs improvment (don't forget taking special care of
   //proc_idx)
   if (procs_in_use >= TK_MAX_THREADS){
      printf("tk: Error - Total amount of threads would exceed limit\n");
      tk_exit(1);
   }
   //Check if chosen prio is within limits
   if (prio >= TK_MAX_PRIO_LEVELS){
      printf("tk: Error - Chosen priority exceed bounds\n");
      tk_exit(1);
   }
   //Check if there will be enough room at that prio
   if (TK_MAX_THREADS_AT_PRIO <= ( scheduleIdxs[prio].procs_at_prio ) ){
      printf("tk: Error - To many threads at this prio\n");
      tk_exit(1);
   }
   //Find next empty slot - which is also the CREATED Thid
   do{
      proc_idx++;
      proc_idx %= TK_MAX_THREADS;
   }while (proc_stat[proc_idx].isInit == TRUE);
   //Test if the assigned name fits
   if (strlen(name)<TK_THREAD_NAME_LEN)
      strncpy(proc_stat[proc_idx].name,name,TK_THREAD_NAME_LEN);
   else{
      printf("tk: Error - Thread-name to long\n");
      tk_exit(1);
   }
   //Try to allocate memory for stack

   if (( STACK_PTR(proc_stat[proc_idx].stack_begin) = (char *) malloc(stack_size)) == NULL){
       printf("tk: Error - Can't create process (can't allocate memory for stack)\n");
       tk_exit(1);  // terminate program if out of memory
   }
   
   REINIT_STACKADDR( proc_stat[proc_idx].stack_begin, stack_size );
   
   proc_stat[proc_idx].isInit       = TRUE;
   proc_stat[proc_idx].state        = READY;
   proc_stat[proc_idx].Thid         = proc_idx;    //for future compability with lage Thid:s
   proc_stat[proc_idx].Gid          = active_thread; //Owned by..
   proc_stat[proc_idx].noChilds     = 0;
   proc_stat[proc_idx].stack_size   = stack_size;
   proc_stat[proc_idx].Prio         = prio;
   proc_stat[proc_idx].Idx          = slot_idx;
   proc_stat[proc_idx].stack_crc    = 0;
   proc_stat[proc_idx].prmtRetAddr  = NULL;
   proc_stat[proc_idx].wakeupEvent  = 0;
   proc_stat[proc_idx].start_funct  = f;
   proc_stat[proc_idx].init_funct   = NULL;


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


@todo Wraparound problem when actual time (int) added to relatine
timeout is greater than the size of an integer. This needs to be solved.
   
@return latency in 1/10 of the timeout resolution, i.e. in 1/10 of a mS.
   
   
*/

unsigned int tk_msleep( unsigned int time_ms ){
/* It's our kernal so we "know" that a clock equals 1uS */
   clock_t act_time_us; 
   clock_t wkp_time_us;
   clock_t latency_us;  //Temporary var only to aid debugging
   
   act_time_us    = clock();
   wkp_time_us    = act_time_us + (time_ms * 1000uL);
   //need a function t_diff that handles wraparound
   proc_stat[active_thread].wakeuptime = wkp_time_us;
   proc_stat[active_thread].state |= SLEEP;
   tk_yield();
   act_time_us    = clock();
/* returns diff in clocks, i.e. uS  */
   latency_us = difftime(proc_stat[active_thread].wakeuptime,act_time_us);
   return latency_us;
}

void _tk_wakeup_timedout_threads( void ){
   int i;
   clock_t act_time;

   act_time = clock();
   //Do not optimize this to "active_procs" until fragmentation of deleted procs
   //is solved.
   for(i=0;i<TK_MAX_THREADS;i++){
      if (proc_stat[i].state & SLEEP){
         if (proc_stat[i].isInit){           //dubble check
            if ( act_time >= proc_stat[i].wakeuptime ){
               proc_stat[i].state &= ~_____QST; /*Release ques also*/
               proc_stat[i].wakeupEvent = E_TIMER;
            }
         }
      }
   }
}

/*
@ingroup kernel_internal

Get the identity of the next thread to run. 
Scheduling policy is used to determine which one that would be.
*/
unsigned int _tk_next_runable_thread(){
   int idx,prio,cidx,midx,nbTry,loop,return_Thid,p_at_p;
   BOOL found;

   return_Thid  = idle_Thid; //In case no runnable proc is found...
   found       = FALSE;

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
   unsigned int RID,                //!< Thread ID to switch to
   unsigned int SID                 /*!< Thread ID to switch from. I.e. 
                                         current thread ID to put away in TCB*/
){
   TRY_CATCH_STACK_ERROR( proc_stat[SID].stack_begin, cswTEMP2 );
   INTEGRITY_CERTIFY_STACK( proc_stat[SID], cswTEMP2 ); //Certify the stack we're leaving from

   
   PUSH_CPU_GETCUR_STACK( cswTSP, cswTEMP );   

   STACK_PTR( proc_stat[SID].curr_sp ) = cswTSP;
   
   cswTSP = STACK_PTR( proc_stat[RID].curr_sp );
   active_thread=RID;

   CHANGE_STACK_POP_CPU( cswTSP, cswTEMP );   
   //TRY_CATCH_STACK_INTEGRITY_VIOLATION( proc_stat[active_thread], cswTEMP2 ); //Check integrity is OK before running 
}

/*!
Dispatch or yield to another thread that has more "right" to run.

I.e. <b>sealch</b> for another potential thread that is now in runable
state
and that has higher priority. In non-preemptive mode, create simulated
timeout-events on all threads currently sleeping, but who's timeout has
expired
(i.e. search for timeouts and change state to READY).

The concept of passively sleeping threads, actively woken up by the
kernel constantly searching for who's ready to run in a thread of its
own (the idle thread or lowest prio thread) concept is a key-concept in
non-preemptable mode.

@note This is the function to put here and there in you application if you run
a non-preemptable version of TinKer. Any execution will not be context
switched
until you either execute this function or run any other kernel function. <b>Any
other TinKer kernel function is also a context switching point.<b>. If
you don't
want that behaviour you have to use the non-yielded version of that
function
(i.e. with suffix -ny)

@note to use this function in preemptable mode </b> you must make sure that
there is no prefix-postfix stuff happening on the stack, neither before
the first PUSHALL nor after the last POPALL.
*/

void tk_yield( void ){
   PUSHALL();
   _tk_wakeup_timedout_threads();
   thread_to_run = _tk_next_runable_thread();
   _tk_context_switch_to_thread(thread_to_run,active_thread);   
   POPALL();
}

/*!

Dispatch or yield to another thread that has more "right" to run.

This is basically identical with tk_yield(), but it doesn't try to
search for threads to wake up that are sleeping on non-preempting
timers. This function is meant to be used for preemptable scheduling
and to be called from an event source (i.e. ISR).

@see void tk_yield( void )

*/

void tk_yield_event( void ){
   PUSHALL();
   thread_to_run = _tk_next_runable_thread();
   _tk_context_switch_to_thread(thread_to_run,active_thread);   
   POPALL();
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
      printf("tk: Program terminated normally");
   else
      printf("tk: Warning - Program terminated with errorcode [%d]",ec);
   while (1) {
      TRAP(ec);
   }
}

/*!
@ingroup kernel_glue

Works as the assert macro exept that you have to use the __file_ and __line_
explicitlly. Typically the assert macro calls this function on targets that do
not have assert implemented.

*/
void _tk_assertfail(
   char *assertstr, 
   char *filestr, 
   int line
) {
   printf("tk: Error - Assertion failed: %s,\nfile: %s,\nline: %d\n",assertstr,filestr,line);
   tk_exit( TK_ERR_ASSERT );
}


/*!
@ingroup kernel_glue

Called from your real main() or from your start-up code 

@note (for deeply embedded developers) You target have to fulfill two
things at least

- malloc has to work and return valid pointers - You need a working
printf to see run-time errors

*/
void _tk_main( void ){
   tk_create_kernel();
   #if defined(TK_COMP_KMEM) && TK_COMP_KMEM
      assert( tk_mem() == ERR_OK );
   #endif
   
   #if defined(TK_COMP_ITC) && TK_COMP_ITC
      assert( tk_itc() == ERR_OK );
      if (_tk_create_system_queues( ) != 0)
         tk_exit(1);
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


#if defined(_MSVC_) || defined(WIN32) 
int main(int argc, char **argv);
int main(int argc, char **argv){
   _tk_main();
   return 0;
}
#endif

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
 * @addtogroup CVSLOG CVSLOG
 *  $Log: tk.c,v $
 *  Revision 1.31  2006-02-20 15:22:01  ambrmi09
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
