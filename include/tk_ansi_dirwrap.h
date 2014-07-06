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

#elif defined(__GNUC__)		/*|| defined(__CYGWIN32__)  || defined(__CYGWIN__) */
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

#define BUILDCHAIN_MUARCH( file ) \
   FNAME( TK_MULTI_ARCH_INCLUDES, file ) \


