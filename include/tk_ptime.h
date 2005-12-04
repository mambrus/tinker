/*******************************************************************
 *
 *  DESCRIPTION: tk_ptime.h  Pre-emptive timers for TinKer.
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:    
 *
 *  Current $Revision: 1.1 $
 *
 *******************************************************************/
   

 
#ifndef TK_PTIMER_H
#define TK_PTIMER_H

/** include files **/
#include <time.h>

/** local definitions **/

/** Error codes **/
#define ERR_OK             0x00 /* No error */
#define ERR_UNDEF_PTIMER   0x02 /* No such or invalid ptimer */
#define ERR_TIME_NEG       0x03 /* */
#define ERR_TIMER_DELETED  0x04 /* */
#define ERR_NO_MORE_TIMERS 0x06 /* */


/*!
TinKers ptimer_t structure. This structure contains all
information the kernel needs to know about pre-emptable timers (ptimers).
*/
typedef struct ptimer_s{
   char           	name[4];    //!< Name of the timer
   unsigned int   	tid;        //!< Identity of this timer
   struct ptimer_s *prev;       //!< Previous in list
   unsigned int   	active;     //!< If this timer is active or not @note: <b>internal use only </b>
   unsigned int   	numBlocked; //!< Number of blocked threads on this timer
   time_t         	expTime;    //!< Expire time
   unsigned int   	neededCnts; //!< Number of iterations needed to complete
   unsigned int   	count;      //!< Completed this number of iterations
   struct ptimer_s *next;       //!< Next in list
}ptimer_t;

/*!
Stats of the Timer "gun" that the lower layer is supporting/providing.
*/

typedef struct{
   clock_t        ticksPerNs; /*!< Number of ticks per each nano-second. Will 
                                   be at least a few until HW is running with 
                                   GHz timers */
   clock_t        tickFrac;   /*!< Fraction of the above. Needed for 
                                   and accurancy/rounding errors. */
   unsigned int   maxTicks;   /*!< Number of ticks the gun can handle. Any 
                                   timeout event requireing longer than this 
                                   time needs to have it's time chopped up in 
                                   fractions of this amount of time.*/
                                   
}
pgunstats_t;


/** default settings **/

/** external functions **/


/*!
Get the quality of the gun. Will assume this never changes after system 
startup. (lower level must assure this to be true).

@note This function must be provided by lower level BSP
*/
extern void askGunQuality   (pgunstats_t *gunstats);

/*!
Arms the gun to trigger at a certain "time" from now. Time is expressed in 
"ticks" and are normalized to have correspond to the actual time 
required (this is handled in this component based on the gun quality provided).

@note This function must be provided by lower level BSP
*/
extern void armGun          (clock_t ticks);

/*!
Dissarms any pending fireing of the gun. Function will return if it was 
succesfull or not. Unsucessfull will most likelly mean that the fireing 
allready happened.

@note This function must be provided by lower level BSP
*/
extern int disarmGun       ();


/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/


void           tk_create_ptime( void );
void           tk_delete_ptime( void );

unsigned long  tk_ptimeevent_at( unsigned int *tid, time_t *absTime );
unsigned long  tk_ptimeevent_in( unsigned int *tid, time_t *relTime );
unsigned long  tk_ptimer_remove( unsigned int  tid );
unsigned long  tk_ptimer_remove( unsigned int  tid );


/** private functions **/




#endif

  
/*! 
 * @addtogroup CVSLOG CVSLOG
 *
 *  $Log: tk_ptime.h,v $
 *  Revision 1.1  2005-12-04 15:48:52  ambrmi09
 *  API for ne pre-emptable timers in place. Implementing this will be a
 *  hard but fun "nut" to crack. ptime has the potential of comming
 *  very close to the high-res timers that POSIX 1003.1c define and is a
 *  good pointer whether pthreads is a good next step or not for TinKer.
 *
 *  
 *******************************************************************/









