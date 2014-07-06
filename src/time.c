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

@note there exists two time.h. One time.h and one sys/time.h. The latter one is
used for hi-res calendar time-structs.
*/

#include <time.h>
#include <errno.h>
#include <assert.h>

#if defined (__GNUC__)
#include <tinker/config.h>
#include <sys/times.h>
#include <sys/time.h>
#else

#include "tk_tick.h"		//internal macros for tick handling in this header
#include "tk_hwclock.h"
#include <tk_hwsys.h>		//< Try to remove this one

   /*!
      An internal scaling factor that needs to be matched against the
      timer resolution so that the POSIX requirements for CLOCKS_PER_SEC is
      fullfilled. The current resolution is 1000 tics per second (1kHZ
      interrupts). The POSIX CLOCKS_PER_SEC is preset to 1*10e6. How many
      clock_ticks is there (or would there be) in a sys_mickey.
    */
#define TICK_PER_CLK 1000ul	//<! How much a tick is advanced on
   //each interrupt

#include "tk_tick.h"
#if defined (HW_CLOCKED)
#define USE_HW_CLOCK		//!< Undef this to see the "error" in tk_msleep that happens each 17.2 minutes (see \Blog060227 for in-depth discussion). Should be undefined if monitoring of ISR latency is turned off in the HW clock, since it will generate worse accuracy then reading only the sys_mikey_mickey alone.
#endif

   /*!
      Optionally this can be defined instead, but leave /ref
      USE_HW_CLOCK defined (same reasoning remains though, accuracy will
      be totally spoiled).
    */
#define NO_SYSTIMER_WRAP_MONITOR

   //return (clock_t)(-1);
#endif				//defined (__GNUC__)

//---------------------------------------------------------------------------------------
// The following section is used to figure out which function we can use to figure out
// usable TinKer time information for the dispatcher and various other needs (user needs).
// Notice that in case GNU build syste is used, a tinker.config.h exists which sets the
// HAVE_xxx macros
//---------------------------------------------------------------------------------------
/*!
@ingroup @ingroup kernel_internal_POSIX

@brief returns time since startup in 1 millionth of a second (uS)
CPU time
http://www.gnu.org/software/libc/manual/html_mono/libc.html#CPU%20Time

This function is used by the kernal, but can also be used by the application.
We try to follow POSIX standard as close as possible with some minor exceptions:

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

@todo check the "(TICK_PER_CLK * sys_mickey);  // This *HAS* to be wrong" thingy

*/

#ifndef HAVE_CLOCK
clock_t clock()
{
#if !defined (USE_HW_CLOCK)
	return (TICK_PER_CLK * sys_mickey);	// This *HAS* to ne wrong
#else
	signed int pebbles, p2;	// Remainig value in HWclock register
	unsigned long uS;	// Time passed since last update of tick expressed in uS.
	unsigned long TuS;	// Temp of the above
	HWclock_stats_t HWclock_stats;
	//int                  ecnt = 0;
	int zerocrossed = 0;

	pebbles = 0;
	tk_getHWclock_Quality(CLK1, &HWclock_stats);

	TK_CLI();
	tk_getHWclock(CLK1, &pebbles);
	uS = sys_mickey;
	TK_STI();

	uS *= 1000uL;

	if ((HWclock_stats.res > 16) && (HWclock_stats.freq_hz < 1000000)) {
		assert
		    ("tk: to high resolution HW clock. TinKer can't handle this ATM"
		     == NULL);
	}

	if (pebbles < 0) {
		pebbles *= -1;
		uS++;
		zerocrossed = 1;
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
	TuS = (TuS * (1000000L / (HWclock_stats.freq_hz / 100))) / 100;

	return (uS + TuS);

#endif
}
#endif

/*!

@ingroup @ingroup kernel_internal_POSIX

@brief Simple calendar time

(time(s) - 's' as in system)
Returns calendar time (i.e. absolute time) since epoch. To be able to do that we need
the time from RTC on startup, which in many cases are missing.

time() and clock() are indentical ecept for this startup offset (RTC).

I.e. this function emulates the case where RCT returns zero, which for relative time
cases is just as good.
*/
#ifndef HAVE_TIME
time_t time(time_t * result)
{
	time_t tresult;
	tresult = (time_t) clock();

	if (result != NULL)
		*result = tresult;

	return tresult;
}
#endif

/*!
@ingroup @ingroup kernel_internal_POSIX

@brief Processor (i.e. process or system) time inquiry
http://www.gnu.org/software/libc/manual/html_mono/libc.html#Processor%20Time

*/
#ifndef HAVE_TIMES
clock_t times(struct tms * buffer)
{
	assert("Not implemented");

	return clock();
}
#endif

/*!
@ingroup @ingroup kernel_internal_POSIX

@brief Get (calendare) time of day
http://www.gnu.org/software/libc/manual/html_mono/libc.html#High-Resolution%20Calendar

*/
#ifndef HAVE_GETTIMEOFDAY
int gettimeofday(struct timeval *tp, struct timezone *tzp)
{
	assert("Not implemented");
	return 0;
}
#endif

/*!
@ingroup @ingroup kernel_internal_POSIX

@brief Set (calendare) time of day
http://www.gnu.org/software/libc/manual/html_mono/libc.html#High-Resolution%20Calendar

*/
#ifndef HAVE_SETTIMEOFDAY
int settimeofday(const struct timeval *tp, const struct timezone *tzp)
{
	assert("Not implemented");
	return 0;
}
#endif

//---------------------------------------------------------------------------------------

/*!

http://lists.freebsd.org/pipermail/freebsd-threads/2005-June/003123.html

*/
#if !defined (__GNUC__)
int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
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
#endif				//!defined (__GNUC__)

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
void timespec2fmttime_np(struct fmttime *totime,	//!< Converted time returned
			 const struct timespec *fromtime	//!< Original representation
    )
{
	totime->days = fromtime->tv_sec / 86400;
	totime->hrs = (fromtime->tv_sec % 86400) / 3600;
	totime->mins = ((fromtime->tv_sec % 86400) % 3600) / 60;
	totime->secs = ((fromtime->tv_sec % 86400) % 3600) % 60;

	totime->nanos = fromtime->tv_nsec;
}

/*!
It is often necessary to subtract two values of type struct timeval or
struct timespec. Here is the best way to do this. It works even on some
peculiar operating systems where the tv_sec member has an unsigned type.

Subtract the `struct timeval' values X and Y,
storing the result in RESULT.

@returns 1 if the difference is negative, otherwise 0.
*/
#if !defined (__GNUC__)
int timeval_subtract(result, x, y)
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
#endif				//!defined (__GNUC__)
