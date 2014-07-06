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

#endif				//TK_ANSI_H

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

