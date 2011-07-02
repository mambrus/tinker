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
(excluding the build-chains that completly miss that file)
*/
#if !defined(__C166__)
#	include <tk_ansi_dirwrap.h>
#	include BUILDCHAIN(time.h)
#endif

#warning "difftime needs better implementation!!!!!!!!!!!!!"

   #define difftime(t1, t0) \
      (t1 - t0)

#ifndef TIME_H_TK
#define TIME_H_TK

#define clock tk_clock   //!< The undefine last is the real re-router
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
struct fmttime{
   long int days; /*!The remaining seconds goes into "days". This can in
                     theory become a very large value, hence a type as
                     large as the types converted from */
   int      hrs;  /*!Hours, this value is between 0-24 */
   int      mins; /*!Minutes, this value is between 0-60 */
   int      secs; /*!Seconds, this value is between 0-60 */
   long int nanos;/*!Nano-seconds, this value is between 0 - 999 999 999 */
};


void timespec2fmttime_np( struct fmttime *totime, const struct timespec *fromtime);

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
   #define time_t unsigned long
   #endif

   /*!
   The struct timeval structure represents an elapsed time. It is declared in sys/time.h

   Honestly speaking, I don't understand why this struct is needed since the struct timespec does the same job AND is of much higher presition. However, it's used in the POSIX standard API gettimeofday so I suppose we'll have to stick with it.

   Further reference: http://www.gnu.org/software/libc/manual/html_mono/libc.html#Elapsed%20Time

   @see timespec
   */
   struct timeval{
      long int tv_sec;  //!< This represents the number of whole seconds of elapsed time. 
      long int tv_usec; //!< This is the rest of the elapsed time (a fraction of a second), represented as the number of <b>microseconds</b>. It is always less than one million.  
   };

   /*!
   The struct timespec structure represents an elapsed time. It is declared in time.h

   this stuct is essential for the pthreads timing API that TinKer supports (pthread_mutex_timedlock, pthread_cond_timedwait to name a few). But also to the nanosleep function 

   A 32 bit signed storage for tv_sec will guarantee a time value that lasts for <b>4085 years!</b>.
   A 32 bit signed storage for tv_nsec is enough to store a fraction of a secons in nanoseconds. Actually it's more tha twice as big as nessesary since it can carry 2.147 seconds before overflowing.

   Further reference: http://www.gnu.org/software/libc/manual/html_mono/libc.html#Elapsed%20Time
   */
   struct timespec{
      long int tv_sec;  //!< This represents the number of whole seconds of elapsed time.
      long int tv_nsec; //!< This is the rest of the elapsed time (a fraction of a second), represented as the number of <b>nanoseconds</b>. It is always less than one billion
   };

   /*!
   This function is not supported on all targets properlly yet.

   @todo implement this
   */
   int nanosleep (const struct timespec *requested_time, struct timespec *remaining); 

   /*!
   Works like the POSIX spec says, excepth that timezone is alway ignored.

   @note This function will on some supported targes give the time in accurate uS resolution ecen if systetem tick is updated with a frequency lower than 1uS. This is achcheived by a cobination of reading the system timer and the HW clock dricing the system timer. 

   @todo investigate the timezone thingy and if there is an equivlent API wit nS resoution

   @todo implement this
   */
   int gettimeofday (struct timeval *tp, struct timezone *tzp);


   /*!
   Works like the POSIX spec says, excepth that timezone is alway ignored.

   @todo investigate the timezone thingy and if there is an equivlent API wit nS resoution

   @todo implement this 
   */

   int settimeofday (const struct timeval *tp, const struct timezone *tzp);

//   #if USE_TINKER_TIME_F
   /*!
   The time function returns the current calendar time as a value of type
   time_t. If the argument result is not a null pointer, the calendar time
   value is also stored in *result. If the current calendar time is not
   available, the value (time_t)(-1) is returned.

   @see http://www.gnu.org/software/libc/manual/html_mono/libc.html#Simple%20Calendar%20Time
   */
   time_t time (time_t *result);
//   #endif


   /*!
   This macro calculates the time between two times (or expressed
   differently: the relative time between to points in time).

   @note Always use difftime to calculate tme-differences in for best
   portability in your application. See \ref clock_t and \ref time_t for
   more information about TinKer's internal system time representation.

   The order of the operands have the same order as the sutraction operator. I.e.:

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
   #define TIMER_ABSTIME	1
   ?

   */
   typedef enum{
      CLOCK_REALTIME = 0,          /*!< Use HW clock to deliver time? Or actual time since epoch? */
      CLOCK_MONOTONIC = 1          /*!< Use sys clock (i.e updated \e "monotonic")*/
   } clockid_t;

   int nanosleep (const struct timespec *rqtp, struct timespec *rmtp);
   int clock_getres (clockid_t clock_id, struct timespec *res);
   int clock_gettime (clockid_t clock_id, struct timespec *tp);
   int clock_settime (clockid_t clock_id, const struct timespec *tp);

//------1---------2---------3---------4---------5---------6---------7---------8
#endif //!defined (__GNUC__)


#if !defined(__C166__)  
#undef clock
#undef time
#endif




#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif

#endif /*time_h_tk*/

  
/*!
 *  @defgroup CVSLOG_time_h time_h
 *  @ingroup CVSLOG
 *  $Log: time.h,v $
 *  Revision 1.30  2007-02-26 14:16:53  ambrmi09
 *  1) Drivers    - structure added
 *  2) Filesystem - in progress
 *
 *  Revision 1.29  2006/12/12 10:57:05  ambrmi09
 *  This adresses the second part of #1609064
 *
 *  Revision 1.28  2006/12/11 14:41:52  ambrmi09
 *  Solves #1609064 (part1)
 *
 *  Revision 1.27  2006/11/27 22:29:22  ambrmi09
 *  Minor djustments completeing the move of some header files to public and due
 *  to some name clashed with user space naming conventions.
 *
 *  Revision 1.26  2006/04/08 10:15:58  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.25.2.2  2006/04/03 20:07:23  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.25.2.1  2006/04/03 15:21:47  ambrmi09
 *  All targets updated with the new thread-starter (alternative 2).
 *
 *  This alternative has one weakness (explained elsewhere togeather
 *  with alternative 1), but so far it's the only one that will compile
 *  and function equally among all 4 (very different) compilers currently
 *  tested against: GCC, MSVC, BC5 and Keil.
 *
 *  If nothing else turns up, I'm willing to overcome the drawback (it's
 *  quite handleable) because it *truly* takes away a lot of pain with
 *  porting.
 *
 *  The ARM port (architecture level) is than's to this now fully operational
 *  without the r13 hack in the context switch. This includes thread
 *  cancellation and thread argument passing (which were not functioning in
 *  the old port).
 *
 *  If this revised code proves itself (i.e. no surprises turns up) then
 *  TinKer can be considered "almost ported" to any HW target that GCC is
 *  ported for :D (/cheers)
 *
 *  Revision 1.25  2006/03/27 13:40:15  ambrmi09
 *  As part of the preparation for the first release, code has been cleaned up a little
 *  and project has been checked that it will build on all it's intended targets.
 *
 *  Problems that remained had to do the ANSI wrapping.
 *
 *  Some modifications were neserary to make the BC5 build, but the result is cleaner
 *  and more consistent with the rest of the wrapping. As a consequence, stdlib.h was
 *  introduced.
 *
 *  Revision 1.24  2006/03/24 17:40:18  ambrmi09
 *  Cosmetic details
 *
 *  Revision 1.23  2006/03/05 11:11:24  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.22  2006/03/04 14:28:44  ambrmi09
 *  Finally got the \ref clock() representation right. Now timing is
 *  behaving equaly between the targets X86_Linux, Cygqing, MSVC, BC5 and
 *  XC167.
 *
 *  Revision 1.21  2006/03/02 15:33:08  ambrmi09
 *  Experimenting with build under Linux
 *
 *  Revision 1.20  2006/03/02 14:05:48  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.19  2006/02/28 18:59:23  ambrmi09
 *  Minor adjustments and verified that all (intended) build environment
 *  compile and produce runnable code. I.e.:
 *  - uVisionKernelProject
 *  - BC5
 *  - Core and POSIX projects in MSVC
 *
 *  Revision 1.18  2006/02/28 18:16:54  ambrmi09
 *  - Mainly a ci for the new Workspace structure
 *  - Houwever, found and corrected a bug in mqueue.c (a NULL pointer
 *    assignement)
 *
 *  Revision 1.17  2006/02/28 13:18:19  ambrmi09
 *  Got MSVC target back-on track also.
 *
 *  Seems both MSVC and BCC have the same "error" regarding \ref CLK_TICK
 *  and/or \ref clock(). Could it be that it's TinKer's interpretation that
 *  is in fact wrong?
 *
 *  Revision 1.16  2006/02/28 11:50:07  ambrmi09
 *  - Trimmed the time constants (ruffly). 4sek per 14hrs drift
 *  - Revived the Borland C (BC5) target. Projectfile also added (BC5.ide)
 *  - Started experimenting with a indlude filename macro, that has the
 *    the potential of solving my ANSI header/function dilemma (\ref
 *    BUILDCHAIN )
 *  - Some "fishyness" about BC5 handling of time. Either \ref clock or
 *    \ref CLK_TCK doesn't follow standard (the latter I know for a fact,
 *    since it's 1e3 instead of 1e6 - but thats not all). \ref tk_msleep is
 *    adjusted to try to see the error.
 *
 *  Revision 1.15  2006/02/27 13:30:03  ambrmi09
 *  <b>Please read the in depth comments</b> about this check-in at \ref
 *  Blog051125
 *
 *  The test program (test.c) in this check-in is also particularly nasty
 *  since it gives really long latencies on each task switch (up to and
 *  above 500mS!). Test against this if you make any changes in either
 *  timing or dispatching. Even considering this hard case, the drift was
 *  very minor. The timing constants need re-trimming though.
 *
 *  Revision 1.14  2006/02/25 14:44:30  ambrmi09
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
 *  Revision 1.13  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.12  2006/02/21 22:10:31  ambrmi09
 *  - Added wrapper macro for pthread_create so that posix threads get named in
 *    TinKer (makes post-mortem easier). Very cool solution with a macro...
 *  - Improved post-mortem, the schedule gets dumpt also now
 *  - Wrapper macros for msleep and usleep (temporary)
 *  - Minor stubbing and wrapping of mq_unlink and pthread_cancel
 *  - Added a new test program (t est-posix.c ). This is verifyed to compile and
 *    run on both Linux and TinKer unmodified!
 *
 *  Revision 1.11  2006/02/19 22:00:38  ambrmi09
 *  Major brake-through!!! First working attempt with crude pThreads and
 *  POSIX RT queues works. (jihaa) :=D. Wow
 *
 *  Revision 1.10  2006/02/16 15:10:59  ambrmi09
 *  Introduced a new component for better and safer useage of the heap.
 *  Package is called \red KMEM and the files are tk_mem.c and tk_mem.h (so
 *  far).
 *
 *  Started to take care of the long needed issue with error codes and
 *  better error handling. Introduced errno.h to begin with, whitch is part
 *  of the package \ref kernel_reimpl_ansi. Its not a good solution yet,
 *  since both kernel and ANSI codes are in the same file we have to invent
 *  a way to omit the ANSI defines when a tool-chain that has errno.h is
 *  used.
 *
 *  Revision 1.9  2006/02/09 23:05:24  ambrmi09
 *  Doxygen related fixes
 *
 *  
 *******************************************************************/





