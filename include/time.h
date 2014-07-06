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

The functions in this file are all reimplementation of ANSI functions
concerning POSIX time, that TinKer relies on but that are commonly
missing from many embedded tool-chain suppliers.

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi

@see kernel_reimpl_ansi

POSIX time API and definitions references:
http://www.opengroup.org/onlinepubs/007908799/xsh/time.h.html

GNU clib refs:

http://www.gnu.org/software/libc/manual/html_mono/libc.html#Simple%20Calendar%20Time
http://www.gnu.org/software/libc/manual/html_mono/libc.html#Elapsed%20Time
http://www.gnu.org/software/libc/manual/html_mono/libc.html#Sleeping

@note "the epoch" = 00:00:00 on January 1, 1970, Coordinated Universal Time.


*/

#include <tk_ansi.h>

/*
The Following will get the tool-chains version of time.h
(excluding the build-chains that completely miss that file)
*/

#if defined (__GNUC__)
#include <tinker/config.h>
#include <sys/times.h>
#endif
#if !defined(__C166__)
#	include <tk_ansi_dirwrap.h>
#	include BUILDCHAIN(time.h)
#endif

#ifndef TIME_H_TK
#define TIME_H_TK

#define clock tk_clock		//!< The undefined last is the real re-router
#define time  tk_time

//#include <limits.h> //<! Information about integers max and min values
//#include <stdint.h> //<! Fixed bitsize type definitions

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

/*!
@brief Struct holds time in formatted form for easier readability.

This is not a POSIX struct, hence it's not in time.h. However, I think there
should be one at least similar (but that I haven't discovered yet). Therefore
this struct is designed in a form that resembles the way POSIX would had done
it.

@note The types are signed for the following reasons:
- They are big enough to keep the values they should represent
- Signed integers are safer to use in arithmetic in-between calculus.
  Especially subtractions are easy to mess up when the values are big.
- Signed integers can be compared between each other. This is the same
  issue as with the above point regarding subtractions. Problem accrues
  when the values are big.
*/
	struct fmttime {
		long int days;	/*!The remaining seconds goes into "days". This can in
				   theory become a very large value, hence a type as
				   large as the types converted from */
		int hrs;	/*!Hours, this value is between 0-24 */
		int mins;	/*!Minutes, this value is between 0-60 */
		int secs;	/*!Seconds, this value is between 0-60 */
		long int nanos;	/*!Nano-seconds, this value is between 0 - 999 999 999 */
	};

//------1---------2---------3---------4---------5---------6---------7---------8

/*!
Wrapper macro until \ref PTIMER is ready
*/
#define sleep(t) ( tk_msleep( t * 1000 ) )

/*!
Wrapper macro until \ref PTIMER is ready
*/
#define usleep(t) ( tk_msleep( (unsigned long)t / 1000ul ) )

#if defined (__GNUC__) && defined (TK_BOARD)
//In this case we know and control our own time - still, make sure
//standard is respected.
#if defined(CLOCKS_PER_SEC)
#undef CLOCKS_PER_SEC
#endif
#if defined (CLK_TCK)
#undef CLK_TCK
#endif

#define CLOCKS_PER_SEC 1000000L
#define CLK_TCK CLOCKS_PER_SEC
#endif

#if !defined (__GNUC__)

#ifndef CLOCKS_PER_SEC
	/*!
	   According to POSIX standard this must be 1 milion regardless of true
	   resolution.

	 */
#define CLOCKS_PER_SEC 1000000L
#endif

#ifndef CLK_TCK
	//! Obsolete macro for historical reasons (because POSIX says so)
#define CLK_TCK CLOCKS_PER_SEC
#endif

#ifndef clock_t
	/*!
	   A clock entity is supposed to mean some sort of system ticks (i.e.
	   clocks), which doesnt nessesary have to be in a SI unit scale. However,
	   POSIX has determined to fix the scale of a clock unit to 1uS, hence our
	   clock entities are 1uS long.<br>

	   clock_t is at least a 32 bit value that contains "clocks". A clock
	   entity is precisely 1uS according to POSIX. <br>

	   This means that the largest value of a time this type can carry is at
	   least 4.2 million uS or 71 minuts (systems where long int is a 32 bit
	   value).
	   <br><br>

	   References:<br>
	   http://www.keil.com/support/man/docs/c166/c166_ap_datastorage.htm <br>

	   @see CLOCKS_PER_SEC

	 */
#define clock_t unsigned long
#endif

#ifndef time_t
	/*!
	   Represents time. This entity is time in some SI scale (i.e. precision).
	   Use time_h everywhere where you need to represent, store or handle real
	   world time values. Avoid using clock_t unless your desired scale is
	   "ticks" (which might not always mean 1uS units on a non 100% POSIX
	   compliant target).

	   TinKer has defined the precision of time_h to 1uS. The representation is
	   on most systems 32 bit (depending on how <i>long int</i> is
	   interpreted).

	 */
#define time_t long
#endif

	/*!
	   The struct timeval structure represents an elapsed time. It is declared
	   in sys/time.h

	   Honestly speaking, I don't understand why this struct is needed since the
	   struct timespec does the same job AND is of much higher precision.
	   However, it's used in the POSIX standard API gettimeofday so I suppose
	   we'll have to stick with it.

	   Further reference:
	   http://www.gnu.org/software/libc/manual/html_mono/libc.html#Elapsed%20Time

	   @see timespec
	 */
	struct timeval {
		long int tv_sec;	/*!< This represents the number of whole seconds of
					   elapsed time. */

		long int tv_usec;	/*!< This is the rest of the elapsed time (a fraction
					   of a second), represented as the number of
					   <b>microseconds</b>. It is always less than one
					   million. */
	};

	/*!
	   The struct timespec structure represents an elapsed time. It is declared in time.h

	   this stuct is essential for the pthreads timing API that TinKer supports
	   (pthread_mutex_timedlock, pthread_cond_timedwait to name a few). But also
	   to the nanosleep function

	   - A 32 bit signed storage for tv_sec will guarantee a time value that
	   lasts for <b>4085 years!</b>.
	   - A 32 bit signed storage for tv_nsec is enough to store a fraction of a
	   second in nanoseconds. Actually it's more tha twice as big as necessary
	   since it can carry 2.147 seconds before overflowing.

	   Further reference:
	   http://www.gnu.org/software/libc/manual/html_mono/libc.html#Elapsed%20Time
	 */
	struct timespec {
		long int tv_sec;	/*!< This represents the number of whole seconds of
					   elapsed time. */

		long int tv_nsec;	/*!< This is the rest of the elapsed time (a
					   fraction of a second), represented as the number
					   of <b>nanoseconds</b>. It is always less than one
					   billion */

	};

	/*!
	   This function is not supported on all targets properly yet.

	   @todo implement this
	 */
	int nanosleep(const struct timespec *requested_time,
		      struct timespec *remaining);

	/*!
	   Works like the POSIX spec says, except that timezone is alway ignored.

	   @note This function will on some supported target give the time in
	   accurate uS resolution even if system tick is updated with a frequency
	   lower than 1uS. This is achieved by a combination of reading the system
	   timer and the HW clock driving the system timer.

	   @todo investigate the timezone thingy and if there is an equivalent API
	   wit nS resolution

	   @todo implement this
	 */
	int gettimeofday(struct timeval *tp, struct timezone *tzp);

	/*!
	   Works like the POSIX spec says, except that timezone is alway ignored.

	   @todo investigate the timezone thingy and if there is an equivalent API
	   wit nS resolution

	   @todo implement this
	 */

	int settimeofday(const struct timeval *tp, const struct timezone *tzp);

//   #if USE_TINKER_TIME_F
	/*!
	   The time function returns the current calendar time as a value of type
	   time_t. If the argument result is not a null pointer, the calendar time
	   value is also stored in *result. If the current calendar time is not
	   available, the value (time_t)(-1) is returned.

	   @see http://www.gnu.org/software/libc/manual/html_mono/libc.html#Simple%20Calendar%20Time
	 */
	time_t time(time_t * result);
//   #endif

	/*!
	   This macro calculates the time between two times (or expressed
	   differently: the relative time between to points in time).

	   @note Always use difftime to calculate tme-differences in for best
	   portability in your application. See \ref clock_t and \ref time_t for
	   more information about TinKer's internal system time representation.

	   The order of the operands have the same order as the sutraction operator.
	   I.e.:

	   \code
	   C = difftime(A,B);
	   \endcode

	   ...means the same as:

	   \code
	   C = A-B;
	   \endcode

	   This macro handles wraparound, but <b>t0</b> is required to represent
	   the time that occur <b>first</b>. The time values can in TinKer be
	   represented in any type of same size as unsigned long, but for best
	   portability you should use time_t. ( A requirement however is that they
	   are represented by the same <em>"signed form"<em> of a type. I.e. either
	   both shold be signed or both should be unsigned

	   @warning Wrapping is handled properly only by scaler integer types with
	   representation size equal to that of <i>unsigned long<i> (i.e. 32 bit on
	   most systems).

	   time_t is always OK to use even for applications that might run on other
	   kernels. The difftime function (or macro) is a POSIX standard and will
	   in that case be implemented to calculate the time difference according
	   to whatever representation time_h has on that system (including complex
	   structure representations).

	   @see
	   http://www.gnu.org/software/libc/manual/html_mono/libc.html#Time%20Basics

	   Using \ref clock_t will work where ever
	   portability is not an issue (drivers, HAL e.t.a. I.e. anywhere target
	   specific and where you are in full control/awareness of how you handle
	   system time and it's representation.)

	   @note difftime in TinKer can only accuratly calculate time differences
	   according to the following:

	   - If you <b>know</b> which time comes firs, the resulting diff may lay
	   in the range 0 - \ref ULONG_MAX.

	   - If you don't know or, if you want to detect which time comes first,
	   then you need a result that is signed. Either user signed arguments or
	   cast the result to signed (explicit casting may not be needed on your
	   system, but it's best to do to avoid dependance on if your compiler
	   works on value or type preservation).

	   I.e. depending on which of the above cases apply, the relative time
	   difference may not exceed either ULONG_MAX or (ULONG_MAX/2 -1).

	 */

	/*
	   #define difftime(t1, t0) \
	   (t0 <= t1 ? t1 - t0 : t1 + (ULONG_MAX - t0) + 1 )
	 */
#ifndef difftime
#define difftime(t1, t0) \
      (t1 - t0)
#endif

//   #if USE_TINKER_CLOCK_F
	clock_t clock();
//   #endif

	/*
	   Confusion regarding this...

	   ...and what is
	   #define TIMER_ABSTIME     1
	   ?

	 */
	typedef enum {
		CLOCK_REALTIME = 0,	/*!< Use HW clock to deliver time? Or actual time since epoch? */
		CLOCK_MONOTONIC = 1	/*!< Use sys clock (i.e updated \e "monotonic") */
	} clockid_t;

	int nanosleep(const struct timespec *rqtp, struct timespec *rmtp);
	int clock_getres(clockid_t clock_id, struct timespec *res);
	int clock_gettime(clockid_t clock_id, struct timespec *tp);
	int clock_settime(clockid_t clock_id, const struct timespec *tp);

//------1---------2---------3---------4---------5---------6---------7---------8
#endif				//!defined (__GNUC__)

#if !defined(__C166__)
#undef clock
#undef time
#endif

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#endif				/*time_h_tk */
