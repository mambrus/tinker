/*******************************************************************
 *
 *  DESCRIPTION: TinKer tick entry functions or macros.
 *
 *  AUTHOR: Michael Ambrus
 *                              
 *  HISTORY:    
 *
 *  Current $Revision: 1.13 $
 *
 *******************************************************************/
/*
Note, versin 1.7 - X are bogus. Just for testing the CVS notify
utility.

touching two files

*/
  

#ifndef TK_TICK_H
#define TK_TICK_H

#if defined(__C166__)
   #define MEMTYPE idata
#else
   #define MEMTYPE
#endif

/* 18 014 398 509 481 983 nS*/
/* 18 014 398 S*/
/* 208,5 days*/

//! This is the internal data of the timer system. Observe that accessing this 
//  variable is not thread/interrupt safe. This needs to be revised. 
//
//  @note Also very important - always include the memory type "language 
//  extention" when declaring external cariables like this.

#if defined(TICK_OWNER)
   unsigned long MEMTYPE sys_mackey = 0;             //!< Overflow counter for sys_mickey
   unsigned long MEMTYPE sys_mickey = 0;             //!< A timer tick. Zapp this: "Trimmed to be 1/10 of a mS"
#else
   extern unsigned long MEMTYPE sys_mickey;
   extern unsigned long MEMTYPE sys_mackey;
#endif   


/*!
@brief Advance system tick.

Call this from your timer ISR.

Usable for any interrupt frequency in multiples of 1mS. 

<b>Notice that operations are done on a running timer</b>

This implies some precautions since the "tick" are evaluated 
while operated on: 

- These operations must not be used anywhere but in the tick ISR
- They must complete before the next tick (don't choose too high frequencies)
- Other ISR must not keep running for longer than one "tick" time
- In preemptive configuration, the priority of the tick ISR must be higher than
  any of the events (for the same reason as mentioned in point above). This is 
  because if that event ISR preempts this operation, it will actually dispatch
  another thread to run, and the tick ISR will not be able to finish until that 
  thread goes off runable state. Threads preempting ISR's is usually not a 
  problem you follow the design rules (mentioned else where), but for the tick 
  ISR it is.

@note 
- Be cartful to match the advance value with the true interrupt frequency
- Use as slow frequencies as possible (large pebble values) to minimize drift
- But not to large. Don't exceed the maximum value the HW can candle
- <b>IMPORTANT:</b> Make sure the period time ( 1/frequency) is in even multiples 
  of 1 mS.

*/
#define _tk_tick_advance_mS( advance )       \
   if (sys_mickey+advance < sys_mickey){     \
      sys_mickey+=advance;                   \
      sys_mackey++;                          \
   }else{                                    \
      sys_mickey+=advance;                   \      
   }


/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>100kHz</b> interrupt frequency is suitable

@see _tk_tick_100uS()
@see _tk_tick_1mS()
*/
#define _tk_tick_10uS()               \
  sys_mickey+=10;                     \
                                      \
  if (!sys_mickey){                   \
    sys_mackey++;                     \
  }



/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>1kHz</b> interrupt frequency is suitable. This will give the 
highest possible relulution for tinker since 1 sys_mickey is equal to 100uS.

@see _tk_tick_1mS() 
@see _tk_tick_10uS()
@see _tk_tick_1mS()
@see _tk_tick_PC()
*/
#define _tk_tick_100us()              \
  sys_mickey++;                       \
                                      \
  if (!sys_mickey){                   \
    sys_mackey++;                     \
  }


/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>1KHz</b> interrupt frequency is suitable

@see _tk_tick_10uS()
@see _tk_tick_100uS()
@see _tk_tick_1mS()
@see _tk_tick_PC()
*/
#define _tk_tick_1mS()                \
  sys_mickey++;                       \
                                      \
  if (!sys_mickey){                   \
    sys_mackey++;                     \
  }


/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>100Hz</b> interrupt frequency is suitable (slow system)

@see _tk_tick_10uS()
@see _tk_tick_100uS()
@see _tk_tick_1mS()
@see _tk_tick_PC()
*/
#define _tk_tick_10mS()               \
  sys_mickey+=10;                     \
                                      \
  if (!sys_mickey){                   \
    sys_mackey++;                     \
  }


/*!
Update sytem tick.

Call this from your timer ISR.

Usable for systems where <b>18.2Hz</b> interrupt frequency is suitable (PC system)

@see _tk_tick_10uS()
@see _tk_tick_100uS()
@see _tk_tick_1mS()
@see _tk_tick_10mS()
*/
#define _tk_tick_PC()                 \
  sys_mickey+=10;                     \
                                      \
  if (!sys_mickey){                   \
    sys_mackey++;                     \
  }


#endif

  
/*!
 * @addtogroup CVSLOG CVSLOG
 *
 *  $Log: tk_tick.h,v $
 *  Revision 1.13  2006-02-22 09:50:56  ambrmi09
 *  Added some more informative description test
 *
 *  Revision 1.12  2006/02/22 09:47:43  ambrmi09
 *  Added some more informative description test
 *
 *  Revision 1.11  2006/02/22 09:42:19  ambrmi09
 *  Added some informative description test
 *
 *  Revision 1.10  2006/02/22 09:31:58  ambrmi09
 *  cvs test
 *
 *  Revision 1.9  2006/02/22 09:27:14  ambrmi09
 *  cvs test
 *
 *  Revision 1.8  2006/02/22 09:25:42  ambrmi09
 *  cvs test
 *
 *  Revision 1.7  2006/02/22 09:24:37  ambrmi09
 *  Minor format fixing.
 *
 *  Revision 1.6  2006/02/06 22:04:15  ambrmi09
 *  Trimming CLK1 - got 3 nice trims at 1,6 and 13 mS period times
 *
 *  Revision 1.5  2006/02/02 15:51:02  ambrmi09
 *  A lot of thought has been invested into the new PTIME component. Had to
 *  change things even in the systime parts (integrated in the SHEDUL
 *  component) to make it more generic. Think this will be really nice when
 *  it's ready, but has been a long road to get PTIME running (and I'm
 *  still not there).
 *
 *  Revision 1.4  2005/12/03 14:04:31  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *
 *******************************************************************/

