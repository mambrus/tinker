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
 
 High resolution clock function and internal 
 system clock storage owner.
 */
   

#define TICK_OWNER
#include "tk_tick.h"
#include <tk.h>
#include <stdio.h>
#include <time.h>
#include <tk_hwsys.h> //Should really be moved into tk_hwclock.h, but we cant since Keil prepocessor is buggd. 


#if defined(HW_CLOCKED)
   #include "tk_hwclock.h"
#endif

/*
We need a constant that is 0x100000000 / 1000 (note the different bases).
Not even preprocessor can calculate this on many compilors so we did that
"off compile-time"
This equals to the real value 4294967,296. Defines below are it's integer and fract part
*/

#define FACTOR    0x418937
#define FFRACT    0x128

/*!
@brief returns time since startup of the target expressed in seconds and
fractions in nS. Precision limited only by HW (if supported).

Similar implementation is found in BSD: 
http://nixdoc.net/man-pages/FreeBSD/man9/getnanouptime.9.html

Internal running time is in mS and is kept in two 32-bit variables. The
LS of these (called "tick") contains number of uS since startup, and the
MS contains the number of times the LS has wrapped around.

Together they form a 64 bit variable keeping the number of mS since
startup. This means the combined value will keep accurate time for
584.942.417,4 years (!). It's relative accuracy though, absolute accurancy 
would have had to take into account drift, but it's good enough for timeouts 
and all sorsts of time calculations between two events in time for a very 
long run-time.

What about precision then? Well only because the LS value contains time
in mS, it doesn't mean that the precision is in mS! On some systems the
value is updated each 10mS, or 100mS (or much worse - on a PC's this is
done 18.2 times/S = 54 mS. I.e. about 50 times less "accurate").

However, the time resulting from this function can be much higher even
than 1uS  (if HW supports it) by reading the remaining time in HW
timer register until next update of "ticks". In practice, the precision
is determined by the HW and is of date of this writing in practice 100
or 200 nS.


The time is presented in the ANSI struct timespec to better fit the
pthreads component. This is however an expression in (signed)
32-bit value seconds and (signed) 32-bit valued nS fractions. This
boils down to mean that the precision and resolution is the same as the
internal time representation, but the maximum time is about 10 miljon times
shorter (i.e.  \f$ \frac{2^32}{2} S = 68.1 \f$ years) - which should still be good in
most cases though.

@note Be carful when you calculate differences between values of struct
timespec. It's easy to limit the maximum time in between to, both 1/2 and
1/4 of it's maximum of 68 years.

@note It is said that <em>"A beloved child has many names"</em>. We call
the internal timer value a "tick". Other names that occure in "kernal
lingo" are "mickeys", "mackeys", "jiffies" and "pebbles". They all mean
more or less the same thing: time since a certain time (often startup),
fractioned in some sort of unit (often interrupt period time). In
TinKer we use most of these words, but with the following beaning:<p>
- tick    - is the internal time. This is represented in mS

- mickey  - mickeys are the LS part of a tick (32 bit value)

- mackey  - mackeys are the MS part of a tick (32 bit value)

- pebble  - is the register value in the HW clock. One pebbe's 
            meaning in actual time depends on the preqiency driving 
            the HWclock.

@note The way TnS is calculated is very critical. We either loose 
resolution (keeping determinator high) or add error (determinator 
is pre-devided, but truncation occures). We might need to consider 
inventing 64-bit operations.

@todo see the second note in getuptime
*/

void getnanouptime (
   struct timespec *tp            //!< The returned uptime in nS presision
){
   unsigned long    /*MSS,*/MSmS; // 32-bit <b>H</b>igh part of seconds, millisecons
   unsigned long        LSS,LSmS; // 32-bit <b>L</b>ow part of seconds, millisecons
   unsigned long        MSfrac;   // whole seconds left from MSmS part
   unsigned long        MSrest;   // The rest after converting to seconds (to propagate down to nS)
   unsigned long        pebbles;  // Remainig value in HWclock register
   unsigned long        nS;       // Time passed since last update of tick expressed in nS. Lets hope we dont need a 64 bit value for this.
   unsigned long        TnS;      // Temp of the above
   
   #if defined(HW_CLOCKED)
   HWclock_stats_t      HWclock_stats;
   #endif

// First part is to gather the bits and pieces holding the time


// Read HW clock if supported follows:
// Note that the following operation will not cause drift. Nither HWclock 
// is prevented from counting nor is systimer prevented from counting ticks, 
// only is systimer possibly delayed a little in the unlikely event that this 
// read comes very close to a timeout - that might cause a small drift depending on the HW configuration 
// (automatic reload or reload by ISR). For now we accept this small drift.
      
   pebbles = 0;
   
   #if defined(HW_CLOCKED)
   tk_getHWclock_Quality( CLK1, &HWclock_stats );
   tk_disarmHWclock(      CLK1 );
   tk_getHWclock(         CLK1, &pebbles);
   #endif
   
   MSmS = sys_mackey; 
   LSmS = sys_mickey; 
      
   #if defined(HW_CLOCKED)
   tk_armHWclock(         CLK1 );

   if ((HWclock_stats.res > 16) && (HWclock_stats.freq_hz < 1000000)) {
      printf("tk: to high resolution HW clock. TinKer can't handle this ATM \n");
      tk_exit(1);
   }
   #endif


// Now we should have all we need, and we can start converting.

   // Calculate the nuber of nS since last update of tick
   TnS = HWclock_stats.maxPebbles - pebbles;
   TnS = (TnS * (1000000000L / (HWclock_stats.freq_hz/100)))/100;
   
   //convert tick to <i>struct timespec</i>
   MSfrac = (MSmS % 1000L)*FACTOR +((MSmS % 1000L)*FFRACT)/1000L;
   MSrest = ((MSmS % 1000L)*FFRACT)%1000L;
   LSS    = (LSmS / 1000L) + MSfrac;
   
   //MSS = MSmS / 1000L;  //<-not used
   
   nS  = ( ( LSmS % 1000L )      +               MSrest                    )    * 1000000L;
   //            ^^^                              ^^^
   //    fraction of milli sec from LS       fraction of millisec from MS        in namosecs
   
   //Add the fraction originally expressed in pebbles and compensate seconds if needed
   nS  = nS + TnS;
   LSS = LSS + nS/1000000000L;  // All the "rests" added togeather might result in more than one second. 
   nS  = nS%1000000000L;        // take away the amount that got into seconds

   //MSS is not compensated - bug will not be seen unless running system for 60 years, and only if nS 
   //frac causes overflow that ripples though both nS and S. Compensating for this will cause 
   //extra code to run on every invocation and that will most certanlly never do anything (waste 
   //of time). Besides MSS is never used outside this function (double waste). This note kept for
   //future reference.
   
//Finally we should have the information requested. Copy to caller
   tp->tv_sec  = LSS;
   tp->tv_nsec = nS;

}

  
/*!
 * @defgroup CVSLOG_tk_tick_c tk_tick_c 
 * @ingroup CVSLOG
 *
 *  $Log: tk_tick.c,v $
 *  Revision 1.14  2006-03-05 11:11:28  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.13  2006/02/27 13:30:04  ambrmi09
 *  <b>Please read the in depth comments</b> about this check-in at \ref
 *  Blog051125
 *
 *  The test program (test.c) in this check-in is also particularly nasty
 *  since it gives really long latencies on each task switch (up to and
 *  above 500mS!). Test against this if you make any changes in either
 *  timing or dispatching. Even considering this hard case, the drift was
 *  very minor. The timing constants need re-trimming though.
 *
 *  Revision 1.12  2006/02/25 14:44:30  ambrmi09
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
 *  Revision 1.11  2006/02/22 13:05:47  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.10  2006/02/22 10:20:53  ambrmi09
 *  Cleaning up after CVS testing
 *
 *  Revision 1.9  2006/02/22 09:56:26  ambrmi09
 *  Added some more informative description test
 *
 *  Revision 1.8  2006/02/22 09:50:56  ambrmi09
 *  Added some more informative description test
 *
 *  Revision 1.7  2006/02/08 18:39:49  ambrmi09
 *  Improved precision by providing the actual frequency instead of the
 *  theoretical in tk_getHWclock_Quality_CLK1 .
 *
 *  Also found and corrected a serious truncation error in getuptime (in
 *  calculating calculating TnS).
 *
 *  The second error might explain some of the time errors that could not be
 *  explained before (for \f$ T_i=6mS \f$ there was a 300nS unexplainable
 *  drift error for each update of tick).
 *
 *  Revision 1.6  2006/02/08 13:57:58  ambrmi09
 *  CLK1 mechanism refined. Compensation tried to be made undependant of timer
 *  ISR frequency. The important thing is not the indepencandy itself, but to
 *  understand where the errors come in.
 *
 *  Revision 1.5  2006/02/03 19:04:19  ambrmi09
 *  Hopefully got the calculus right for converting from internal time to
 *  "struct timespec" in function getuptime.
 *
 *  Revision 1.4  2006/02/02 17:44:40  ambrmi09
 *  Workaround for Keil include header bug (again)
 *
 *  Revision 1.3  2006/02/02 16:25:02  ambrmi09
 *  Minor syntax errors fixed
 *
 *  Revision 1.2  2006/02/02 15:45:07  ambrmi09
 *  Low level API for internal high-res clock operations added (note: no <b>direct</b> HW dependency)
 *
 *
 *******************************************************************/





