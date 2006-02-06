#include <time.h>

//! An internal scaling factor that needs to be matched against the timer 
//  resolution so that the POSIX requirements for CLOCKS_PER_SEC is fullfilled.
//  The current resolution is 1000 tics per second (1kHZ interrupts). The 
//  POSIX CLOCKS_PER_SEC is preset to 1*10e6. How many clock_ticks is there 
//  (or would there be) in a sys_mickey.

#define TICK_PER_CLK 1000  //<! How much a tick is advanced on
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


int gettimeofday (struct timeval *tp, struct timezone *tzp){
}


int settimeofday (const struct timeval *tp, const struct timezone *tzp){
}


/*!
@brief Converts time from <i>struct timespec</i> to formatted time

Converts time from the compact struct timespec representation for
the more user friendly struct fmttime. No information is lost in
the conversion, but remember that struct fmttime is not practically
usable for any time calculations. Only use struct fmttime to ease
interpretation back and forth to the user (or as debugging info).

*/
void timespec2fmttime( 
   struct fmttime *totime, //!< Converted time returned
   struct timespec fromtime//!< Original representation
){
   totime->days     =   fromtime.tv_sec / 86400;
   totime->hrs      =  (fromtime.tv_sec % 86400) / 3600;
   totime->mins     = ((fromtime.tv_sec % 86400) % 3600) / 60;
   totime->secs     = ((fromtime.tv_sec % 86400) % 3600) % 60;

   totime->nanos    = fromtime.tv_nsec;
}




