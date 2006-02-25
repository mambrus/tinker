/*******************************************************************
 *
 *  DESCRIPTION: TinKer tick entry functions or macros.
 *
 *  AUTHOR: Michael Ambrus
 *                              
 *  HISTORY:    
 *
 *  Current $Revision: 1.18 $
 *
 *******************************************************************/
/*
Note, versin 1.7 - 1.15 are bogus. Just for testing the CVS notify
utility.


*/
  

#ifndef TK_TICK_H
#define TK_TICK_H

#if defined(__C166__)
   #define MEMTYPE idata
#else
   #define MEMTYPE
#endif

/*!
Use SPEEDUP larger than 1 to make kernel advance faster. 

@note <b>This is used for debugging kernel internals only</b>
*/
#define SPEEDUP 1ul
//#define SPEEDUP 1000ul


#if (SPEEDUP > 1)
   #define ADV( PERT ) (   \
      ( PERT * SPEEDUP )   \
   )
#elif (SPEEDUP == 1)
   #define ADV( PERT ) (   \
      ( PERT )             \
   )
#else
   #error Faulty SPEEDUP factor (use this for debugging dispatcher only)
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
      sys_mickey+=ADV(advance);              \
      sys_mackey++;                          \
   }else{                                    \
      sys_mickey+=ADV(advance);              \      
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


void getnanouptime (
   struct timespec *tp
);


  
  
/*!
 * @defgroup CVSLOG_tk_tick_h tk_tick_h
 * @ingroup CVSLOG
 *
 *  $Log: tk_tick.h,v $
 *  Revision 1.18  2006-02-25 14:44:30  ambrmi09
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
 *  Revision 1.17  2006/02/23 15:33:33  ambrmi09
 *  Found a nasty "bug", that was not a read bug after all. At least not in the kernel as a feared. It turned out that I forgot some of the details about how timeouts were to be handled (especially in \ref ITC ). A timeout of value \b zero is equal of never to timeout (read more about it in define \ref FOREVER). However two important lesson learned: Even simple add operations get "funny" when adding large numbers (see line 303 in tk_ipc.c - in the \ref lock_stage function). Anyway. FOREVER should equal zero. (This issue makes me wonder sometimes how sane it really was to resurrect a project that has been dormant for nearly 10 years.) The CodeWright project ruler should be positioned on the actual line btw. This check-in will be accompanied  by a <tt>cvs tag</tt> for this reason, and for yet another nasty bug that seems to be a real dispatcher bug. The current source-set-up will show the bug within one mint (which is good since it makes it a little bit less of a search for the <I>"needle in the haystack</i>").
 *
 *  Revision 1.16  2006/02/22 13:05:47  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.15  2006/02/22 10:20:53  ambrmi09
 *  Cleaning up after CVS testing
 *
 *  Revision 1.14  2006/02/22 09:56:26  ambrmi09
 *  Added some more informative description test
 *
 *  Revision 1.13  2006/02/22 09:50:56  ambrmi09
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

