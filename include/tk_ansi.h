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

@brief ANSI functions reimplemented

This is the common headerfile for all ANSI re-implementations

Most of this file is obsolete now. It was thought to be a common wrapper
for all the ANSI wrapping needs (functions and types). It was soon discovered 
that this would be a mess beyond sanity. All the wrapping is now maintained 
in the each TinKer corresponding header-file.

The file is kept never the less for the time being.

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi

@see kernel_reimpl_ansi

*/

#ifndef TK_ANSI_H
#define TK_ANSI_H


#endif //TK_ANSI_H

//------1---------2---------3---------4---------5---------6---------7---------8


/** @defgroup kernel_reimpl_ansi ANSI functions reimplemented 
@ingroup PACKAGES 
@brief Files containing re-emplemented ANSI functions

The files in this package are involved one way or the other
with reimplemtation of various ANSI functions, defines and types. The
"stuff" we're talking about is what is normally part of stdlib and that
needs to be either \b implemented (because if you use too-chain for
small embedded targets, lot's of staff is simply missing) or \b
redefined (because for some reason or the other we have to use a TinKer
version of the particular function/type). I.e. the things you'll
find in this package is what what is normally defined in stdlib.h,
sys/types.h time.h e.t.a. AND that is needed (note, not every single
header file is wrapped - only those really needed).

@section Intro 
In case you do have a corresponding function in your tool-chain, you
don't want to have both. Because of that each function is conditionally
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


@attention <b>Warning:</b> notice we're playing around with void
pointers... All sorts of strange shit happens around void pointers, be
cartful!


<H3>References</H2>
@see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dnanchor/html/vc6anchor.asp
@see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vccore98/HTML/_PREDIR_Predefined_Macros.asp
*/



/*! 
 * @defgroup CVSLOG_tk_ansi_h tk_ansi_h
 * @ingroup CVSLOG
 *  $Log: tk_ansi.h,v $
 *  Revision 1.1  2006-11-27 22:29:23  ambrmi09
 *  Minor djustments completeing the move of some header files to public and due
 *  to some name clashed with user space naming conventions.
 *
 *  Revision 1.14  2006/04/08 10:16:02  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.13.2.1  2006/04/03 20:07:29  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.13  2006/03/27 13:40:15  ambrmi09
 *  As part of the preparation for the first release, code has been cleaned up a little
 *  and project has been checked that it will build on all it's intended targets.
 *
 *  Problems that remained had to do the ANSI wrapping.
 *
 *  Some modifications were neserary to make the BC5 build, but the result is cleaner
 *  and more consistent with the rest of the wrapping. As a consequence, stdlib.h was
 *  introduced.
 *
 *  Revision 1.12  2006/03/05 11:11:27  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.11  2006/03/04 14:28:44  ambrmi09
 *  Finally got the \ref clock() representation right. Now timing is
 *  behaving equaly between the targets X86_Linux, Cygqing, MSVC, BC5 and
 *  XC167.
 *
 *  Revision 1.10  2006/03/02 14:05:49  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.9  2006/02/28 18:16:55  ambrmi09
 *  - Mainly a ci for the new Workspace structure
 *  - Houwever, found and corrected a bug in mqueue.c (a NULL pointer
 *    assignement)
 *
 *  Revision 1.8  2006/02/28 13:18:20  ambrmi09
 *  Got MSVC target back-on track also.
 *
 *  Seems both MSVC and BCC have the same "error" regarding \ref CLK_TICK
 *  and/or \ref clock(). Could it be that it's TinKer's interpretation that
 *  is in fact wrong?
 *
 *  Revision 1.7  2006/02/28 11:50:08  ambrmi09
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
 *  Revision 1.6  2006/02/22 13:05:47  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.5  2006/02/16 15:11:00  ambrmi09
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
 *  Revision 1.4  2006/02/14 16:14:06  ambrmi09
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
