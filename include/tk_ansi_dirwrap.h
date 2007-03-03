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

Special file for getting tool-chain head files.

@note <b>This file is supposed to be included "several" times.</b> Hence 
these is no "#ifndef" protection.

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi

@see kernel_reimpl_ansi

*/

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO 0
#endif

/*
Make sure pthread stypes, time-constants and other stuff from other from any 
of these OS'es that we don't care about. This will affect certain types and 
macros both for the kernel, but also for any application using the kernel 
header structure

#if defined(__rtems__)
#undef __rtems__
#endif

#if defined(__svr4__)
#undef __svr4__
#endif

#if defined(__CYGWIN__)
#undef __CYGWIN__
#endif
*/

/*
Below will do the same thing but it's safer (undefining the others is too crude 
since they might be used for other datatypes like dev_t). 

What the below setting does is preventing newlib headers to include
the sys/features file, which is __rtems__ only specific and which sets
_POSIX_THREADS among other pthread retated things.
*/
#ifndef _SYS_FEATURES_H
#define _SYS_FEATURES_H
#endif

//------1---------2---------3---------4---------5---------6---------7---------8
#if defined(_WIN32) &&  defined(_MSC_VER)
   #define CHAINPATH C:\Program Files\Microsoft Visual Studio\VC98\Include

#elif defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
   #define CHAINPATH C:/BC5/INCLUDE

#elif defined( __C166__ )
// #define CHAINPATH C:\Program Files\Keil\C166\INC
   #define CHAINPATH C:\Keil\C166\INC

#elif defined(__GNUC__) /*|| defined(__CYGWIN32__)  || defined(__CYGWIN__)*/
   #ifndef CHAINPATH
      #define CHAINPATH /usr/include
   #endif

//Getting rid of other build-ins that will interfear
// NOTE Inspect tool-chain builtins by invoking:
// echo | *-*-*-cpp -dM | less
// Look for macros that don't have '__' prefix and suffix

//The following workarounds are necessary for our BUILDCHAIN macro to work
   #ifdef bfin
      #warning Workaround enabled for GCC builtin macro bug: bfin
      #undef bfin
      #ifndef __bfin__
         #define __bfin__
      #endif
   #endif
   #ifdef BFIN
      #warning Workaround enabled for GCC builtin macro bug: BFIN
      #undef BFIN
      #ifndef __BFIN__
         #define __BFIN__
      #endif
   #endif
   #ifdef i386
//      #warning Workaround enabled for GCC builtin macro bug: i386
      #undef i386
      #ifndef __i386__
         #define __i386__
      #endif
   #endif

#else
   #error "Can\'t determine the target for the TINKER kernel"
#endif
//------1---------2---------3---------4---------5---------6---------7---------8

#define DEFSTR( x ) \
   #x

#define FNAME( path, file ) \
   DEFSTR( path/file )
   
#define BUILDCHAIN( file ) \
   FNAME( CHAINPATH, file ) \


/*! 
 * @defgroup CVSLOG_tk_ansi_dirwrap_h tk_ansi_dirwrap_h
 * @ingroup CVSLOG
 *
 *  $Log: tk_ansi_dirwrap.h,v $
 *  Revision 1.6  2007-03-03 23:01:32  ambrmi09
 *  Added driver support for FIFO's
 *
 *  Revision 1.5  2007-02-26 14:16:53  ambrmi09
 *  1) Drivers    - structure added
 *  2) Filesystem - in progress
 *
 *  Revision 1.4  2007-02-24 14:27:16  ambrmi09
 *  * i386-hixs-elf compiles and links properly
 *
 *  Revision 1.3  2007-02-24 12:17:14  ambrmi09
 *  1) Sync PowerPC sources
 *  2) Add structure for i386-hixs-elf target. (Hopefully our new test-bed)
 *     for developping the filesys component.)
 *
 *  Revision 1.2  2007-02-19 17:34:37  ambrmi09
 *  Tinker will now compile for target powerpc-hixs-rtems
 *
 *  Revision 1.1  2006/11/21 17:19:39  ambrmi09
 *  Moved tk_ansi_dirwrap to public directory (applications without TinKer sources needs also wrapping)
 *
 *  Revision 1.8  2006/10/09 17:08:06  ambrmi09
 *  * Sceleton for bfin and powerpc now compiles.
 *  * Refingnemen in auto-tools to build correct BSP depending on toolchain and settings
 *  * BFIN,bfin macro work-around
 *
 *  Revision 1.7  2006/04/08 10:16:02  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.6.2.2  2006/04/03 20:07:29  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.6.2.1  2006/04/03 15:21:48  ambrmi09
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
 *  Revision 1.6  2006/03/27 13:40:15  ambrmi09
 *  As part of the preparation for the first release, code has been cleaned up a little
 *  and project has been checked that it will build on all it's intended targets.
 *
 *  Problems that remained had to do the ANSI wrapping.
 *
 *  Some modifications were neserary to make the BC5 build, but the result is cleaner
 *  and more consistent with the rest of the wrapping. As a consequence, stdlib.h was
 *  introduced.
 *
 *  Revision 1.5  2006/03/05 11:11:27  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.4  2006/03/04 19:32:35  ambrmi09
 *  Modified sources to allow build GNU targets transparently.
 *
 *  Revision 1.3  2006/03/04 14:28:44  ambrmi09
 *  Finally got the \ref clock() representation right. Now timing is
 *  behaving equaly between the targets X86_Linux, Cygqing, MSVC, BC5 and
 *  XC167.
 *
 *  Revision 1.2  2006/03/02 14:05:50  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.1  2006/02/28 18:40:11  ambrmi09
 *  initial ci
 *
 *
 */
