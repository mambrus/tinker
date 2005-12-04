/*******************************************************************
 *
 *  DESCRIPTION: tk_ptime.c Pre-emptive timers for TinKer.
 *
 *  AUTHOR: Michael Ambrus
 *                              
 *  HISTORY:    
 *
 *  Current $Revision: 1.1 $
 *
 *******************************************************************/
  

/** include files **/
#include <tk.h>
#include <tk_sysqueues.h>
#include <tk_ptime.h>
#include <tk_hwsys.h>
#include <string.h>



/** local definitions **/
#define TK_MAX_PTIMERS TK_MAX_THREADS //!< Makes no sense to have more timer than threads

/* default settings */

/** external functions **/

/** external data **/

/** internal functions **/
unsigned int timersThread(void *inpar );

void insertInPendingList(ptimer_t *timer);
void deleteFromPendingList(ptimer_t *timer);

/** public data **/

/** private data **/

/** public functions **/

/** private functions **/

/** private data **/

ptimer_t timer_pool[TK_MAX_PTIMERS]; //!< Memory pool to avoid usage of malloc
unsigned int currentIdx;             //!< Index to next empty slot
ptimer_t *pendingTimers;             /*!< Sorted linked list to pending times. 
                                          The first one is also pending 
                                          awaitinng triggering from 
                                          timerEvent */
pgunstats_t pgunstats;               /*!< The quality statistics of the fireing 
                                          mechanism the lower level provides. 
                                          This value must be asked for and
                                          will not containg valid values 
                                          until then.*/
                                          
//Pubplic API
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Creates and initializes the ptime component
*/

void tk_create_ptime( void ){
   int i;
   pendingTimers = NULL;
   
   for (i=0;i<TK_MAX_PTIMERS; i++){
      strncpy(&timer_pool[i].name,"ZOMB",4);
      timer_pool[i].tid         = i;
      timer_pool[i].prev        = NULL;
      timer_pool[i].active      = NO;
      timer_pool[i].numBlocked  = 0;
      timer_pool[i].expTime     = 0;
      timer_pool[i].neededCnts  = 0;
      timer_pool[i].count       = 0;
      timer_pool[i].next        = NULL;
   }
}

/*!
Destroys the ptime component. Any pending threads are released.

@todo: Release threads

@todo: Define timeout codes. timeout_exp, timer_deleted e.t.a.

*/
void tk_delete_ptime( void ){
   int i;

   /*
   todo's here
   */
   
   for (i=0;i<TK_MAX_PTIMERS; i++){
      strncpy(&timer_pool[i].name,"ZOMB",4);
      timer_pool[i].tid         = i;
      timer_pool[i].prev        = NULL;
      timer_pool[i].active      = NO;
      timer_pool[i].numBlocked  = 0;
      timer_pool[i].expTime     = 0;
      timer_pool[i].neededCnts  = 0;
      timer_pool[i].count       = 0;
      timer_pool[i].next        = NULL;
   }
}

/*!
Creates (and starts) a timer that will expire <b>at</b> the absolute time 
you set.

*/
unsigned long tk_ptimeevent_at( 
   unsigned int     *tid,     /*!< out: The identity of the timer. Use this id for 
                                   any related operations (for example to 
                                   block on). */
   time_t           *absTime  /*!< in: A absolute time_t defining the event. 
                                   This can be of any lengt or resolution 
                                   supported by you system and the time_t 
                                   struture. @note This is also be the sorting 
                                   criteria/key for the list of pending timers 
                                   */
){                            /*!  Reurns ptime return code */
   TK_CLI();
   if (timer_pool[currentIdx].active){
      TK_STI();
      return (ERR_NO_MORE_TIMERS);
   }
   
  //Continue with stuff
  
  //Stuff finished - safe to reactivate event sources
  TK_STI();
}

/*!
Creates (and starts) a timer that will expire <b>in</b> the relative time counting from now.


*/
unsigned long tk_ptimeevent_in( 
   unsigned int     *tid,     /*!< out: The identity of the timer. Use this id for 
                                   any related operations (for example to 
                                   block on). */
   time_t           *absTime  /*!< in: A relative time_t defining the event. 
                                   This can be of any lengt or resolution 
                                   supported by you system and the time_t 
                                   struture. */
){                            /*!  Reurns ptime return code */
}

/*!
Deletes a pending timer including cancelling the event. In case any threads are
pending on the timer, those will be released but also given an reason different
from the "normal" case.

This must be a valid timer created by any of the set functions and in must 
not have expired yet. If any these pre-requisits are not met the function
will return an error.

@note To prevent logic missmatch in case a timer is fireing an even while 
removing it's data. The trigger will be attempted to be disarmed first, then if 
that is succesfull interrupts will be disabled during the rest of the operation 
completes.
*/

unsigned long tk_ptimer_remove( 
   unsigned int  tid          /*!< The identity of the timer.*/
){
  TK_CLI();
  if (!timer_pool[tid].active){
     TK_STI();
     return(ERR_UNDEF_PTIMER);
  }
  
  //Continue with stuff
  //Stuff finished - safe to reactivate event sources
  TK_STI();
}
//------1---------2---------3---------4---------5---------6---------7---------8


/*!
@ingroup kernel_glue

@brief Gives a way to enter specific timers

This gets the current threads internal timer structure (timer_t). Function is
"public" but is <b>not</b> ment to be used by application developers, rather as
a way for different layers of TinKer to interact.

@note <b>For TinKer internal use only</b>

*/
ptimer_t *_tk_ptimer( unsigned int tid ){
   return(&timer_pool[tid]);
}


/*!
@brief Internal thread that handles timers

On a very low leve (or HW level) the system can only provide <b>one</b>
real time-out event. And even worse: it's limited in either lengt or 
resolution (i.e. a compromize between the two) AND it can wake up 
only one pending thread. 

This manager thread on the other hand, makes it possibe to handle all the
mentioed drawbacks. A time-out event can be of very high resolution AND
destined to happen very far in the future, and when it expieres it can
wake up any number of threads pending on it. Also, you can have several
pending timers pending at the same ptimer event,

@note <b>For TinKer internal use only</b>

*/
unsigned int timersThread(void *inpar ){
}


  
/*! 
 * @addtogroup CVSLOG CVSLOG
 *  $Log: tk_ptime.c,v $
 *  Revision 1.1  2005-12-04 15:48:52  ambrmi09
 *  API for ne pre-emptable timers in place. Implementing this will be a
 *  hard but fun "nut" to crack. ptime has the potential of comming
 *  very close to the high-res timers that POSIX 1003.1c define and is a
 *  good pointer whether pthreads is a good next step or not for TinKer.
 *
 *  
 *******************************************************************/
