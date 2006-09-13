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
@ingroup kernel_reimpl_ansi

@brief ANSI time functions reimplemented

The functions in this file are all reimplementations of ANSI functions
concerning POSIX time, that TinKer relies on but that are commonly
missing from many embedded toolchain suppliers.

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi

@see kernel_reimpl_ansi

@todo Function \ref time is quick and dirty. Certanly quick at least. Temporary solution just to pass compilation. <b>This really needs fixing.</b>

*/

#include <time.h>
#include "tk_tick.h"
#include "tk_hwclock.h"
#include <tk_hwsys.h> //< Try to remove this one
#include <errno.h>
#include <assert.h>


/*! 
An internal scaling factor that needs to be matched against the
timer resolution so that the POSIX requirements for CLOCKS_PER_SEC is
fullfilled. The current resolution is 1000 tics per second (1kHZ
interrupts). The POSIX CLOCKS_PER_SEC is preset to 1*10e6. How many
clock_ticks is there (or would there be) in a sys_mickey.
*/
#define TICK_PER_CLK 1000ul  //<! How much a tick is advanced on 
//each interrupt


#include "tk_tick.h"         //internal macros for tick handliing in this header
#if defined (HW_CLOCKED) 
#   define USE_HW_CLOCK      //!< Undef this to see the "error" in tk_msleep that happens each 17.2 minutes (see \Blog060227 for in-deapth discussion). Should be undefined if monitoring of ISR latency is turned off in the HW clock, since it will generate worse accuracy then reading only the sys_mikey_mickey alone.
#endif

/*! 
Optionally this can be defined instead, but leave /ref
USE_HW_CLOCK defined (same reasoning remains though, accuracy will
be totally spoiled). 
*/
#define NO_SYSTIMER_WRAP_MONITOR 

//return (clock_t)(-1);


/*!

@ingroup @ingroup kernel_internal_POSIX
        
@brief returns time since startup in 1 millionth of a second (uS)

This function is used by the kernal, but can also be used by the application.
We try to follow POSIX standard as close as possible with some minor exeptions:

The formal POSIX clock is somewhat different. There the time is relative
to since the calling <b>process</b> was started. On most small embedded
systems, including those using this kernel, there is only one process and
that is the whole binary it self. I.e. the meaning of a process is
the whole program and our clock funtion relates to time since startup
(reset).

@note Upper limit of for how long up-time can be determined is
dependant on the definition of clock_t. clock_t <b>should</b> be a
at least a 32-bit value. That means that this function will return the
same result approximately every 72 minutes.

@see clock_t

References:<br>
http://www.gnu.org/software/libc/manual/html_mono/libc.html#CPU%20Time
   
@return time since start in uS

@note Not all targets need this module. If supported by your compiler
use that module instead.

Implementation in principle the same as \ref getnanouptime

@todo check the "(TICK_PER_CLK * sys_mickey);  // This *HAS* to ne wrong" thingy
   
*/
clock_t clock(){   
   #if !defined (USE_HW_CLOCK)
   return (TICK_PER_CLK * sys_mickey);  // This *HAS* to ne wrong
   #else
   signed int        	pebbles,p2;  // Remainig value in HWclock register
   unsigned long        uS;       // Time passed since last update of tick expressed in uS.
   unsigned long        TuS;      // Temp of the above
   HWclock_stats_t      HWclock_stats;
   //int                  ecnt = 0;
   int                  zerocrossed = 0;
      

   pebbles = 0;   
   tk_getHWclock_Quality( CLK1, &HWclock_stats );
   
   TK_CLI();
   tk_getHWclock(         CLK1, &pebbles);   
   uS = sys_mickey; 
   TK_STI();
      

   uS *= 1000uL;

   if ((HWclock_stats.res > 16) && (HWclock_stats.freq_hz < 1000000)) {
      assert("tk: to high resolution HW clock. TinKer can't handle this ATM" == NULL);
   }
   
   if (pebbles <0){
      pebbles *= -1;
      uS++;
      zerocrossed=1;
   }   

   /*
   for ( ecnt =0; (unsigned long)pebbles >= HWclock_stats.perPebbles; ecnt++ ){
      if (!zerocrossed)
         assert("How the fuck...!"==NULL);
      pebbles -= HWclock_stats.perPebbles;
      uS++;      
   }
   */


   /*
   The following should be cought by the TISR and should never possible to happen.
   Enable if if you think you have a reason.
   */
   #ifndef NO_SYSTIMER_WRAP_MONITOR
   assert(HWclock_stats.perPebbles > (unsigned long)pebbles);
   #endif

   // Calculate the nuber of uS since last update of tick
   TuS = HWclock_stats.maxPebbles - pebbles;
   TuS = (TuS * (1000000L / (HWclock_stats.freq_hz/100)))/100;
      
   return (uS + TuS);

   #endif
}

time_t time (time_t *result){
   *result = (time_t)clock();
   return *result;
}


int gettimeofday (struct timeval *tp, struct timezone *tzp){
}


int settimeofday (const struct timeval *tp, const struct timezone *tzp){
}

/*!

http://lists.freebsd.org/pipermail/freebsd-threads/2005-June/003123.html

*/

int clock_gettime (
   clockid_t clock_id, 
   struct timespec *tp
){
    switch (clock_id) {
      case CLOCK_REALTIME:
         assert("CLOCK_REALTIME is not implemented yet" == 0);
         //nanotime(tp);
         break;
   
      case CLOCK_MONOTONIC:
         getnanouptime(tp);
         break;
   
      default:
         return EINVAL;
   }

   return ERR_OK;
}



/*!
@brief Converts time from <i>struct timespec</i> to formatted time
<
Converts time from the compact struct timespec representation for
the more user friendly struct fmttime. No information is lost in
the conversion, but remember that struct fmttime is not practically
usable for any time calculations. Only use struct fmttime to ease
interpretation back and forth to the user (or as debugging info).

@note Even though this function is part of the time source module,
it's not a POSIX standard function. Hence the _np suffix.

*/
void timespec2fmttime_np( 
   struct fmttime *totime,         //!< Converted time returned
   const struct timespec *fromtime //!< Original representation
){
   totime->days     =   fromtime->tv_sec / 86400;
   totime->hrs      =  (fromtime->tv_sec % 86400) / 3600;
   totime->mins     = ((fromtime->tv_sec % 86400) % 3600) / 60;
   totime->secs     = ((fromtime->tv_sec % 86400) % 3600) % 60;

   totime->nanos    = fromtime->tv_nsec;
}

/*!
It is often necessary to subtract two values of type struct timeval or
struct timespec. Here is the best way to do this. It works even on some
peculiar operating systems where the tv_sec member has an unsigned type.

Subtract the `struct timeval' values X and Y,
storing the result in RESULT.

@returns 1 if the difference is negative, otherwise 0.  
*/
     
int
   timeval_subtract (result, x, y)
         struct timeval *result, *x, *y;
   {
      /* Perform the carry for the later subtraction by updating y. */
      if (x->tv_usec < y->tv_usec) {
      int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
      y->tv_usec -= 1000000 * nsec;
      y->tv_sec += nsec;
      }
      if (x->tv_usec - y->tv_usec > 1000000) {
      int nsec = (x->tv_usec - y->tv_usec) / 1000000;
      y->tv_usec += 1000000 * nsec;
      y->tv_sec -= nsec;
      }
     
   /* Compute the time remaining to wait.
         tv_usec is certainly positive. */
      result->tv_sec = x->tv_sec - y->tv_sec;
      result->tv_usec = x->tv_usec - y->tv_usec;
     
   /* Return 1 if result is negative. */
      return x->tv_sec < y->tv_sec;
   }
   
   
  

/*!
 * @defgroup CVSLOG_time_c time_c
 * @ingroup CVSLOG
 *  $Log: time.c,v $
 *  Revision 1.18  2006-09-13 18:29:31  ambrmi09
 *  Commited needed in repocitory
 *
 *  Revision 1.17  2006/04/08 10:16:02  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.16.2.2  2006/04/07 12:10:07  ambrmi09
 *  Skeleton for handling syscalls using the ARM Angel interface in place
 *
 *  Basic terminal I/O for gnu_arm (LPC2129) - only output so far (input requires
 *  blocking).
 *
 *  Revision 1.16.2.1  2006/04/03 20:07:28  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.16  2006/03/12 15:08:55  ambrmi09
 *  - Adjusted the source to accomodate the new file structure.
 *
 *  - All build environments uppdated and verified except BC5. For this one
 *  we stumbled across the header-file issue that I've been fearing. Seems
 *  we need to take care of that one after all.
 *
 *  @note The malloc bug still not solved.
 *
 *  Revision 1.15  2006/03/11 15:11:32  ambrmi09
 *  - Trimmed the monitoring contitionals concernig missed sys_mickey
 *  updates (in time.c and GPT1.C). They are now off per default (remember to
 *  reenaple in real case apps).
 *
 *  - Preparing for moving the dave branch out of kernel scope, and move it
 *  as a part of the bsp instead.
 *
 *  Revision 1.14  2006/03/11 14:37:50  ambrmi09
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
 *  Revision 1.13  2006/03/05 11:11:27  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.12  2006/02/27 13:30:04  ambrmi09
 *  <b>Please read the in depth comments</b> about this check-in at \ref
 *  Blog051125
 *
 *  The test program (test.c) in this check-in is also particularly nasty
 *  since it gives really long latencies on each task switch (up to and
 *  above 500mS!). Test against this if you make any changes in either
 *  timing or dispatching. Even considering this hard case, the drift was
 *  very minor. The timing constants need re-trimming though.
 *
 *  Revision 1.11  2006/02/25 14:44:30  ambrmi09
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
 *  Revision 1.10  2006/02/23 15:33:33  ambrmi09
 *  Found a nasty "bug", that was not a read bug after all. At least not in the kernel as a feared. It turned out that I forgot some of the details about how timeouts were to be handled (especially in \ref ITC ). A timeout of value \b zero is equal of never to timeout (read more about it in define \ref FOREVER). However two important lesson learned: Even simple add operations get "funny" when adding large numbers (see line 303 in tk_ipc.c - in the \ref lock_stage function). Anyway. FOREVER should equal zero. (This issue makes me wonder sometimes how sane it really was to resurrect a project that has been dormant for nearly 10 years.) The CodeWright project ruler should be positioned on the actual line btw. This check-in will be accompanied  by a <tt>cvs tag</tt> for this reason, and for yet another nasty bug that seems to be a real dispatcher bug. The current source-set-up will show the bug within one mint (which is good since it makes it a little bit less of a search for the <I>"needle in the haystack</i>").
 *
 *  Revision 1.9  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.8  2006/02/19 22:00:38  ambrmi09
 *  Major brake-through!!! First working attempt with crude pThreads and
 *  POSIX RT queues works. (jihaa) :=D. Wow
 *
 *  Revision 1.7  2006/02/09 23:05:25  ambrmi09
 *  Doxygen related fixes
 *
 *  
 *******************************************************************/
   
     





