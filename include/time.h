/*! 
POSIX time API and definitions

References:

http://www.gnu.org/software/libc/manual/html_mono/libc.html#Simple%20Calendar%20Time
http://www.gnu.org/software/libc/manual/html_mono/libc.html#Elapsed%20Time
http://www.gnu.org/software/libc/manual/html_mono/libc.html#Sleeping

"the epoch" = 00:00:00 on January 1, 1970, Coordinated Universal Time. 

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

Honestly speaking, I don't understand why yhis struct is needed since the struct timespec does the same job AND is of much higher presition. However, it's used in the POSIX standard API gettimeofday so I suppose we'll have to stick with it.

@see timespec
*/
struct timeval{
   long int tv_sec;  //!< This represents the number of whole seconds of elapsed time. 
   long int tv_usec  //!< This is the rest of the elapsed time (a fraction of a second), represented as the number of <b>microseconds</b>. It is always less than one million.  
}

/*!
The struct timespec structure represents an elapsed time. It is declared in time.h

this stuct is essential for the pthreads timing API that TinKer supports (pthread_mutex_timedlock, pthread_cond_timedwait to name a few). But also to the nanosleep function 

A 32 bit signed storage for tv_sec will guarantee a time value that lasts for <b>4085 years!</b>.
A 32 bit signed storage for tv_nsec is enough to store a fraction of a secons in nanoseconds. Actually it's more tha twice as big as nessesary since it can carry 2.147 seconds before overflowing.

*/
struct timespec{
   long int tv_sec;  //!< This represents the number of whole seconds of elapsed time. 
   long int tv_nsec  //!< This is the rest of the elapsed time (a fraction of a second), represented as the number of <b>nanoseconds</b>. It is always less than one billion
}


/*!
This function is not supported on all targets properlly yet.

@tbd implement this
*/

int nanosleep (const struct timespec *requested_time, struct timespec *remaining); 

/*!
Works like the POSIX spec says, excepth that timezone is alway ignored.

@note This function will on some supported targes give the time in accurate uS resolution ecen if systetem tick is updated with a frequency lower than 1uS. This is achcheived by a cobination of reading the system timer and the HW clock dricing the system timer. 

@tbd investigate the timezone thingy and if there is an equivlent API wit nS resoution
*/
int gettimeofday (struct timeval *tp, struct timezone *tzp);


/*!
Works like the POSIX spec says, excepth that timezone is alway ignored.

@tbd investigate the timezone thingy and if there is an equivlent API wit nS resoution
*/
int settimeofday (const struct timeval *tp, const struct timezone *tzp);




/*! 
This macro calculates the time between two times (or expressed
differentlly: the relative time between to points in time).

It hadless wraparound, but <b>t1</b> is required to represent the
time that occures <b>first</b>. The time values can be
represented in any type, but preferably by time_t. A requirement is
however that they are represented by the same type. I.e. either both
signed or bot unsigend, either both floats or non floats e.t.a.

Wrapping is handled properly only by scalar types with representation
size equal to that of <i>unsigned long<i> (i.e. 32 bit on most systems).

time_t is always OK to use even for applications that might run on other
kernels. The difftime "function" is a POSIX standard and will in that
case be implemented to calculate the time difference according to
whatever representation time_h has on that system (including complex
struct representations). clock_t will work where portability is not an
issue (i.e. drivers, HAL e.t.a. that are target specific anyway and
written to suit both HW and kernel, and in the kernal itself).

@note wrapping is handled properly only by scalar types with
representation size equal to that of unsigned long



*/ 

#define difftime(t1, t2) \
   t1 <= t2 ? t2 - t1 : t2 + (ULONG_MAX - t1);



clock_t clock();

#endif /*time_h*/




