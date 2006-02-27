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

#ifndef time_h
#define time_h

#include <limits.h> //<! Information about integers max and min values
//#include <stdint.h> //<! Fixed bitsize type definitions

/*! 
According to POSIX standard this must be 1 milion regardless of true
resolution.

*/
#define CLOCKS_PER_SEC 1000000L

//! Obsolete macro for historical reasons (because POSIX says so)
#define CLK_TCK CLOCKS_PER_SEC

/*! 
Wrapper macro until \ref PTIMER is ready
*/
#define sleep(t) ( tk_msleep( t * 1000 ) )

/*! 
Wrapper macro until \ref PTIMER is ready
*/
#define usleep(t) ( tk_msleep( t / 1000 ) )


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

/*!
The time function returns the current calendar time as a value of type
time_t. If the argument result is not a null pointer, the calendar time
value is also stored in *result. If the current calendar time is not
available, the value (time_t)(-1) is returned.

@see http://www.gnu.org/software/libc/manual/html_mono/libc.html#Simple%20Calendar%20Time
*/
time_t time (time_t *result);





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
#define difftime(t1, t0) \
   (t1 - t0)

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

//------1---------2---------3---------4---------5---------6---------7---------8
void timespec2fmttime_np( struct fmttime *totime, const struct timespec *fromtime);
clock_t clock();

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

#endif /*time_h*/

  
/*! 
 *  @defgroup CVSLOG_time_h time_h
 *  @ingroup CVSLOG
 *  $Log: time.h,v $
 *  Revision 1.15  2006-02-27 13:30:03  ambrmi09
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





