/*!
@file
@ingroup kernel_reimpl_ansi

@brief ANSI functions reimplemented

This is the common headerfile for all ANSI re-implementations

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi

@see kernel_reimpl_ansi

*/

#ifndef TK_ANSI_H
#define TK_ANSI_H

//#include <stdio.h>
#include <stdlib.h>

#ifndef COMPARISON_FN_T 
/*!
A comparison funtion definition that ANSI search & sort funtions use.
It's return value is supposed to have the same meaning as for strcmp

@return \f$ = \cases{ 0, & if L = R \cr <0 , & if L < R \cr >0, & if L > R } \f$


@return  0 if eq, 
@return <0 if L<R 
@return >0 if L>R 

GNU reference: @see http://www.gnu.org/software/libc/manual/html_mono/libc.html#Comparison%20Functions
*/
typedef int comparison_fn_t (  
   const void *L,  //!< <em>"Leftmost"</em> element to compare with
   const void *R   //!< <em>"Rightmost"</em> element to compare with
);

#endif


/*!
@name CONDITIONALS

These macro's define whether a certain re-implemented ANSI function
should be part of the TinKer binary or not.

*/

//@{

#define TK_NEEDS_QSORT
#define TK_NEEDS_BSEARCH
#define TK_NEEDS_PRINTF

//@}

#define APP_NEEDS_INTERNALS /*!< Define this conditional if you want access to the
                                 internal functions this module has */





//------1---------2---------3---------4---------5---------6---------7---------8

#if defined(__cplusplus)

extern "C"
{
#endif


   #if defined(TK_NEEDS_QSORT)
      #define qsort tk_qsort
      #if defined(_MSVC_) || defined(_WIN32)
         void __cdecl qsort ( void *, size_t, size_t, comparison_fn_t );         
      #else
         void         qsort ( void *, size_t, size_t, comparison_fn_t );         
      #endif
   #endif


   #if defined(TK_NEEDS_BSEARCH)
      #define bsearch tk_bsearch
      #if defined(_MSVC_) || defined(_WIN32)
         void * __cdecl bsearch ( const void *, const void *, size_t, size_t, comparison_fn_t );
      #else
         void *         bsearch ( const void *, const void *, size_t, size_t, comparison_fn_t );
      #endif
   
   #endif

   #if defined(APP_NEEDS_INTERNALS)

      #if defined(_MSVC_) || defined(_WIN32)
            void __cdecl _tk_quicksort ( void *, int, int, int, comparison_fn_t );
            int  __cdecl _tk_bsearch   ( void *, void *, int, int, int, comparison_fn_t );

      #else
            void         _tk_quicksort ( void *, int, int, int, comparison_fn_t );
            int          _tk_bsearch   ( void *, void *, int, int, int, comparison_fn_t );      
      #endif
   #endif


#if defined(__cplusplus)
}
#endif


//------1---------2---------3---------4---------5---------6---------7---------8


/** @defgroup kernel_reimpl_ansi ANSI functions reimplemented 
@ingroup CODEMOD 
@brief Files containing re-emplemented ANSI functions

The files in this Doxygen "module" are involved one way or the other
with reimplemtation of various ANSI functions, defines e.t.a. The
"stuff" we're talking about is what is normally part of stdlib (i.e.
what stdlib.h defines).

@section Intro 
In case you do have a corresponding function, you don't
want to have both. Because of that each function is conditionally
compiled and you have to explicitly set each definition. This way when
you port for a new target (assuming no conditionals have been set by you
yet), you will know if a function is supported or not since the linker
will produce link errors for functions not found.

   
@section missing_functions Simply missing functions
Typically a tool-chain comes with only partial of the stdlib. Here
comes an example of functions <b>NOT</b> included in such a typical
tool-chain (Keil). 

@see http://www.keil.com/support/man/docs/c166/c166_xa_librarydif.htm

@note If a function in the list below is colored blue, that means that
the specific function is both implemented AND included in this certain
build of TinKer. Use this as a fast way to determine which version
of a certain function you're useing (in case of uncertainty).
   
The following ANSI Standard library routines are not included in the C166 library:

abort
asctime
atexit
\ref                       bsearch
clearerr
\ref                       clock
ctime
difftime
div
exit
fclose
feof
ferror
fflush
fgetc
fgetpos
fgets  
fmod
fopen
fprintf
fputc
fputs
fread
freopen
frexp
fscanf
fseek
fsetpos
ftell
fwrite
getc
getenv
gmtime
ldexp  
ldiv
localeconv
localtime
mblen
mbstowcs
mbtowc
mktime
perror
putc
\ref                       qsort
raise
remove
rename
rewind
setbuf
setlocale
setvbuf  
signal
strcoll
strerror
strftime
strtok
strxfrm
system
time
tmpfile
tmpnam
ungetc
vfprintf
wcstombs
wctomb  

The following ANSI Standard library routines is probably part of you
tool-chain, but <em>you most likely</em> need replace with TinKer
equivalents:

\ref                       printf
\ref                       putchar

@section other_reasons Other reasons
In some cases there exist other reasons for why you would want to use
TinKers re-implemented version of a certain ANSI function:

- Re-entrance, your tool suppliers version is not re-entrant. Since
threads can preempt another thread currently in any function (including
stdlib) and make another call to the same function, the result of the
first one can get corrupted (if the function wasn't designed considering
re-entrance). This is however in most embedded tool-chain not an issue,
but you should really check.

- Thread safety, your tool suppliers version is not thread safe. Part of
the function must never be preempted (or interrupted), but you supplier
never considered that case. File operations are such a case (including
terminal I/O such as the common printf).

- Memory issues, your tool suppliers version is hogging memory or 
allocating from regions that you can't allow (or don't support) e.t.a.

- Optimizations, you might need a different behaviour (speed, temporal
determinism e.t.a.)

- And many other reasons that I cant remember right now...

*/

#endif

/*! 
 * @addtogroup CVSLOG CVSLOG
 *  $Log: tk_ansi.h,v $
 *  Revision 1.4  2006-02-14 16:14:06  ambrmi09
 *  Bsearch implemented, a lot of doc regarding \ref qsort \ref bsearch
 *  \ref _tk_qsort and \ref _tk_bsearch is added.
 *
 *  Also the qsort/bsearch example from GNU libc ref manual is copy &
 *  pasted into the root thread.
 *
 *  Revision 1.3  2006/02/13 14:31:24  ambrmi09
 *  Crude first version of ANSI qsort implemented.
 *
 *  Revision 1.2  2006/02/09 23:05:25  ambrmi09
 *  Doxygen related fixes
 *
 *  Revision 1.1  2006/02/09 22:40:39  ambrmi09
 *  Initial creation of ANSI re-implementation module
 *
 *  
 *******************************************************************/
