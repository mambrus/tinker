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
#include <kernel/src/tk_tick.h>
#include <errno.h>
#include <assert.h>


//! An internal scaling factor that needs to be matched against the timer 
//  resolution so that the POSIX requirements for CLOCKS_PER_SEC is fullfilled.
//  The current resolution is 1000 tics per second (1kHZ interrupts). The 
//  POSIX CLOCKS_PER_SEC is preset to 1*10e6. How many clock_ticks is there 
//  (or would there be) in a sys_mickey.

#define TICK_PER_CLK 1000ul  //<! How much a tick is advanced on 
//each interrupt


#include "tk_tick.h"  //internal macros for tick handliing

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
   
   
*/

clock_t clock(){
   return TICK_PER_CLK * sys_mickey;
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
It is often necessary to subtract two values of type struct timeval or struct timespec. Here is the best way to do this. It works even on some peculiar operating systems where the tv_sec member has an unsigned type.

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
 * @ingroup CVSLOG CVSLOG
 *  $Log: time.c,v $
 *  Revision 1.11  2006-02-25 14:44:30  ambrmi09
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
   
     





