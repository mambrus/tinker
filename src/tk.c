/*******************************************************************
 *
 *  DESCRIPTION: tk.c Scheduler for TinKerOS.
 *
 *  AUTHOR: Michael Ambrus
 *                              
 *  HISTORY:    
 *
 *  Current $Revision: 1.6 $
 *
 *  $Log: tk.c,v $
 *  Revision 1.6  2005-11-23 07:47:44  ambrmi09
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
 *  Handling av wakeup event. Needed for IPC to be able why a IPC object was
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

/** include files **/
#include <stdio.h>                                                      
#include <stdlib.h>
#include <string.h>

#include <tk.h>
#include "tk_hwsys.h"

/** local definitions **/

/* default settings */

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/

/** private functions **/


#define DEBUG
#ifdef DEBUG
#define static
#endif


proc_t proc_stat[max_procs];
//This is not perfect. PiD will be reused when more than max_procs has
//been used. I wold have wanted a Pid range that is much bigger and a lookup-
//table (gives greater uniquety - specially usefull i case of RPC) but that
//is costly (I think)
//coord_t lookUpTable[max_procs];

typedef struct{
   unsigned short procs_at_prio;    //Used for optimizing sheduler.
   unsigned short curr_idx;
}prio_stat_t;

static unsigned int theSchedule[max_prio_levels][max_procs_at_prio];
static prio_stat_t scheduleIdxs[max_prio_levels];

static unsigned int active_task = 0;
static unsigned int task_to_run = 0;
/*static*/ unsigned int procs_in_use = 0;
static unsigned int proc_idx;             //points at the last proc_t created
static unsigned int idle_Pid;             //idle_Pid must be known;

unsigned int __tk_idle( void *foo ){          //idle loop (non public)
   while (TRUE){
      schedul();
   }
}

void deleteKern( void ){
      deleteTask(idle_Pid);
}

unsigned int MyPid( void ){
   return(active_task);
}

proc_t *MyProc_p( void ){
   return(&proc_stat[active_task]);
}

void createKern( void ){
   int i,j;

   for (i=0; i<max_procs; i++){
      proc_stat[i].state         = ZOMBIE;
      proc_stat[i].wakeuptime    = 0;
      proc_stat[i].isInit        = FALSE;
      //proc_stat[i].name        ="";
      proc_stat[i].Gid           = 0;
      proc_stat[i].Pid           = 0;
      proc_stat[i].noChilds      = 0;
      proc_stat[i].stack_size    = 0;
      proc_stat[i].stack_begin   = NULL;
      proc_stat[i].curr_sp       = NULL;
      proc_stat[i].wakeupEvent   = 0;
   }
   //The Root proc is already created but must be registred
   proc_stat[0].state = READY;
   proc_stat[0].isInit = TRUE;
   strcpy(proc_stat[0].name,"root");
   procs_in_use = 1;
   proc_idx = 0;
   //Clear the tables (before creating idle - task)
   for (i=0; i<max_prio_levels; i++){
      //Clear the help table
      scheduleIdxs[i].procs_at_prio = 0;
      scheduleIdxs[i].curr_idx = 0;
      for (j=0; j<max_procs_at_prio; j++){
         //init the table to root everywhere
         theSchedule[i][j]=0;
      }
   }
   //Create a Idle task, whoes sole purpose is to burn up time
   //when nobody else is running
   idle_Pid = createTask("idle",max_prio_levels - 1,__tk_idle,NULL,64);
   //IdleProc must like root, i.e. bee owned by itself
   proc_stat[proc_stat[idle_Pid].Gid].noChilds--;
   //Awkward way to say that root has created one process less than it has
   proc_stat[idle_Pid].Gid = proc_stat[idle_Pid].Pid;
   //Idle ownde by it self
   //Init shedule tables
   //Put the root in the scedule
   scheduleIdxs[0].procs_at_prio = 1;
   theSchedule[0][0]=0;
   //Put idle task in shedule at lowest prio
   /*
   scheduleIdxs[max_prio_levels - 1].procs_at_prio = 1;
   theScedule[max_prio_levels - 1][0]=idle_Pid;
   */ /* NOT NEEDED, DONE ALREADY */
}

unsigned int destructor( void *foo ){
   //function enters as a result of a ret instruction. EAX is passed
   //as the return value. Not shure if it works on every processor
   unsigned int retval;


   GET_THREADS_RETVAL( retval );
   
   //This is critical, no more stack, will not work as is in a preemtive kernal
   #ifdef DEBUG
   printf("Dieing task is returning %d\n\n",retval);
   #endif
   deleteTask(active_task);
   schedul();
   //should never tk_exit
   return(0);
}

int deleteTask(unsigned int Pid){
   unsigned int Prio,Idx,i;
   
   if ( proc_stat[Pid].isInit == FALSE )  {
      //The process you are trying to delete does not exist
      return(TK_ERROR);    
   }
   Prio = proc_stat[Pid].Prio;
   Idx = proc_stat[Pid].Idx;
   procs_in_use--;
   proc_stat[Pid].state = ZOMBIE;
   //Tell the parent proc that one more child is gone if parent exists
   //else don't bother "" and then try to free "sleep until no workers"
   if (proc_stat[proc_stat[Pid].Gid].isInit) {
      proc_stat[proc_stat[Pid].Gid].noChilds--;
      if ( (proc_stat[proc_stat[Pid].Gid].noChilds == 0) &&
           ( proc_stat[proc_stat[Pid].Gid].state & _______T ) ) {
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
   free(proc_stat[Pid].stack_begin);
   proc_stat[Pid].isInit = FALSE;
   return(TK_OK);
}

//sfr  SPSEG                = 0xFF0C;       //Stack Pointer Segment Register

static char *ctTSP1, *ctTSP2; //!< fony temporary stackpointer used in the process of setting TOS
static unsigned long ctTEMP;  //!< Extra storage. For some targets used to manipulate segment part of stackpointers
static char *ctTTOS;          //!< Top of stack pointer (temporary), return value storage from macro



unsigned int createTask(
   char *name,
   unsigned int prio,
   funk_p f,
   void * inpar,
   size_t stack_size
){
   //where in theScheduler to put task id
   unsigned int slot_idx = scheduleIdxs[prio].procs_at_prio;
   funk_p *f_p;
   void *v_p;
   #ifdef DEBUG
   int i;
   #endif

   //Error handling needs improvment (don't forget taking special care of
   //proc_idx)
   if (procs_in_use >= max_procs){
      printf("tk: Error - Total amount of processer exceeds limit\n");
      tk_exit(1);
   }
   //Check if chosen prio is within limits
   if (prio >= max_prio_levels){
      printf("tk: Error - Chosen priority exceed bounds\n");
      tk_exit(1);
   }
   //Check if there will be enough room at that prio
   if (max_procs_at_prio <= ( scheduleIdxs[prio].procs_at_prio ) ){
      printf("tk: Error - To many processes at prio\n");
      tk_exit(1);
   }
   //Find next empty slot - which is also the CREATED Pid
   do{
      proc_idx++;
      proc_idx %= max_procs;
   }while (proc_stat[proc_idx].isInit == TRUE);
   //Test if the assigned name fits
   if (strlen(name)<proc_name_len)
      strcpy(proc_stat[proc_idx].name,name);
   else{
      printf("tk: Error - Process name to long\n");
      tk_exit(1);
   }
   //Try to allocate memory for stack

   if ((proc_stat[proc_idx].stack_begin = (char *) malloc(stack_size)) == NULL){
       printf("tk: Error - Can't create process (can't allocate memory for stack)\n");
       tk_exit(1);  // terminate program if out of memory
   }
   proc_stat[proc_idx].isInit       = TRUE;
   proc_stat[proc_idx].state        = READY;
   proc_stat[proc_idx].Pid          = proc_idx;    //for future compability with lage Pid:s
   proc_stat[proc_idx].Gid          = active_task; //Owned by..
   proc_stat[proc_idx].noChilds     = 0;
   proc_stat[proc_idx].Prio         = prio;
   proc_stat[proc_idx].Idx          = slot_idx;
   proc_stat[proc_idx].wakeupEvent  = 0;

   proc_stat[active_task].noChilds++;
   procs_in_use++;

   #ifdef DEBUG
   for (i=0;i<stack_size;i++)
      proc_stat[proc_idx].stack_begin[i]=0xff;
   #endif
   //Here's the secret.
   //preparing the stack
   //=============================
   //#0x4=inpar (fony pushed param)    //The stack looks like calling schedule
   //return adress to "destructor"     //from function destructor
   //#4=return adress (which is also calling adress of function task)
   //#4=EBP
   //#0x4=pushf
   //#0x20=pusha 

   v_p = (void *)&proc_stat[proc_idx].stack_begin[stack_size - 0x4];
   *(unsigned int*)v_p = (unsigned int)inpar;

   f_p = (funk_p *)&proc_stat[proc_idx].stack_begin[stack_size - 0x8];
   *f_p = destructor;

   f_p = (funk_p *)&proc_stat[proc_idx].stack_begin[stack_size - 0xC];
   *f_p = f;

   ctTSP1 = &proc_stat[proc_idx].stack_begin[stack_size - 0xC];

   //MARKALL();

   PREP_TOS( ctTSP2, ctTSP1, ctTTOS, ctTEMP );   
    //#pragma src
    //#pragma asm                                                                                                 
   //   MOV R1,R5                                                                                                
    //#pragma endasm  

   //Assign the stackpointer to top of stack
   //proc_stat[proc_idx].sp = &proc_stat[proc_idx].stack[stack_size - 0x34];
   proc_stat[proc_idx].curr_sp = ctTTOS;
   //Put process in scedule - assume tight tight schedule
   theSchedule[prio][slot_idx] = proc_idx;
   //Increase the amount of procs at same prio
   scheduleIdxs[prio].procs_at_prio++;
   return proc_idx ;
}

/**

@ingroup kernel   
        
@brief Sleeps for ms amount of time. 

Sleeps for ms amount of time. Passes control to other threads meanwhile.   

The longest time this function can sleep is dependant on the 
targets bitsize for a integer. On 16 bit targets that would be  65535 mS
or  aprox <b>max 65 S </b>.<br>

The timeout resolution is the same as for the in-argument, i.e. best
kernel supported timeout <b>resolution is 1mS</b>. Note however that the
precision is dependant of the sys_tick advancement resolution (i.e.
interrupt frequency). On a low performance target one sys_tick might be
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

unsigned int msleep( unsigned int time_ms ){
/* It's our kernal so we "know" that a clock equals 1uS */
   clock_t act_time_us; 
   clock_t wkp_time_us;
   clock_t latency_us;  //Temporary var only to aid debugging
   
   act_time_us    = clock();
   wkp_time_us    = act_time_us + (time_ms * 1000uL);
   //need a function t_diff that handles wraparound
   proc_stat[active_task].wakeuptime = wkp_time_us;
   proc_stat[active_task].state |= SLEEP;
   schedul();
   act_time_us    = clock();
/* returns diff in clocks, i.e. uS  */
   latency_us = difftime(proc_stat[active_task].wakeuptime,act_time_us);
   return latency_us;
}

void wakeUpSleepers( void ){
   int i;
   clock_t act_time;

   act_time = clock();
   //Do not optimize this to "active_procs" until fragmentation of deleted procs
   //is solved.
   for(i=0;i<max_procs;i++){
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

unsigned int next(){
   int idx,prio,cidx,midx,nbTry,loop,return_Pid,p_at_p;
   BOOL found;

   return_Pid  = idle_Pid; //In case no runnable proc is found...
   found       = FALSE;

   for(prio=0;prio<max_prio_levels && !found;prio++){ //prio from highets to lowest
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
               return_Pid = theSchedule[prio][idx];
               found = TRUE;
            }
            //Next proc at this prio that should try to run
            idx++;
            idx %= p_at_p; //saves some pointer-casts this way
         }
      }
   }
   return return_Pid;
}

static char *cswTSP;          //!< fony temporary stackpointer used in the process of setting TOS
static unsigned int cswTEMP;  //!< Extra storage. For some targets used to manipulate segment part of stackpointers


void runTask(unsigned int RID,unsigned int SID){
   PUSH_CPU_GETCUR_STACK( cswTSP, cswTEMP );   
   proc_stat[SID].curr_sp=cswTSP;
   
   cswTSP=proc_stat[RID].curr_sp;
   active_task=RID;
   /*************** OBS OBS - TESTPURPOSE ONLY *****************/
   STKUN = 0x7200;
   STKOV = 0x6500;
   /*************** OBS OBS - TESTPURPOSE ONLY *****************/
   //IEN = 0;
   CHANGE_STACK_POP_CPU( cswTSP, cswTEMP );   
   //IEN = 1;
}

void schedul( void ){
   wakeUpSleepers();
   task_to_run = next();
   runTask(task_to_run,active_task);
}

void tk_exit( int ec) {
   if (ec==0)
      printf("tk: Program terminated normally");
   else
      printf("tk: Warning - Program terminated with errorcode [%d]",ec);
   while (1) {
   }
}

void __tk_assertfail(
   char *assertstr, 
   char *filestr, 
   int line
) {
   printf("tk: Error - Assertion failed: %s,\nfile: %s,\nline: %d\n",assertstr,filestr,line);
   tk_exit(3);
}

void tk_main( void ){
   createKern();
   #ifdef IPC
   createIPC();
   #endif
    
   root();

   #ifdef IPC
   deleteIPC();
   #endif 
   deleteKern();
}


#if defined(_MSVC_) || defined(WIN32) 
int main(int argc, char **argv);
int main(int argc, char **argv){
   tk_main();
   return 0;
}
#endif

/*
void Test_scheduler( void ){
   while (TRUE){
      runTask(T1,ROOT);
      runTask(T2,ROOT);
      runTask(T3,ROOT);
   }
}
*/

