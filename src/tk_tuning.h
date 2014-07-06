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
/*
@ingroup kernel_internals

@brief Tuning the kernel for specific needs

To aid tuning the kernel for specific needs, TinKer has three levels of
tuning.

- The first level provides the developer and user with a high level of
flexibility of tuning. Tunings are done using the configure script and end
up in a specific file. This is however not possible for non-GNU targets,

- Therefore there is also a second level that for GNU targets works as a
fall-back in case any option has not been able to determine, but for
non-GNU targets provide the only way to configure TinKer.

- The last lever is a catch-all level. It sets sets safe but practically
unusable settings. This takes care of cases where porting has just started.

Tuning macros fall into two categories:

- Mapping of functions and function macros. TinKer doesn't rely on many
standard library API's. In fact only 3 needs to be known and only one
is an absolute requirement for a working kernel (clock). Mapping of
these is done by using either
	- TK_FNK_VOIDED - This function is voided out. No code will
	  be generated where this function is used. prink() is such
	  a function that can be voided in cases where no output is
	  desired or no possibility to get any text out is available.
	- TK_FNK_STUBBED - Code will use a simple stubbed function
	  that will permit the kernel to build, but most likely not
	  produce a fully functional kernel that behaves properly,
	  clock() is such a function that can be stubbed and is in that
	  case replaced by a simple loop counter.
	- TK_FNK_RENAMED the function is a stdlib call but will be
          recapped with a _tk_ prefix. This enables us in certain cases
	  to use a tool-chain with a full library, but where some of it's
	  functions are not desirable. clock() is such an example, where
	  a much better accuracy can be achieved using tinkers native
	  clock function (_tk_clock() ). Note that in all cases concerning
	  the 1003.1c and most cases concerning 1003.1b (ptreads & queues)
	  explicit re-mapping is not needed, TinKer will override those
	  functions in all cases internally.
	- TK_FNK_ORIGINAL - Is the opposite of the above. Use the function
	  "as is" in the code.

- Values that define certain resource usage as in the example below:
	- #define TK_MAX_THREADS          50
*/

#ifndef TK_TUNING_GLOB
#define TK_TUNING_GLOB
//------1---------2---------3---------4---------5---------6---------7---------8
// First some global stuf...
//------1---------2---------3---------4---------5---------6---------7---------8
#define TK_FNK_ORIGINAL 0	//!< Use & link against original name somewhere
#define TK_FNK_STUBBED  1	//!< Use the stubbed version of the function
#define TK_FNK_RENAMED  2	//!< Use a renamed version
#define TK_FNK_VOIDED   4	//!< Void function call out

#endif				//TK_TUNING_GLOB
//------1---------2---------3---------4---------5---------6---------7---------8
// 1'st level tuning.
// Highest abstraction (and most flexible) tuning
// NOTE This is only available for GNU targets. It and permit macros to be
// defined based on configure settings.
//
// This permits the kernel to be tuned without editing the source
// Non GNU targets must rely on the settings in their respective tk_tuning file
//------1---------2---------3---------4---------5---------6---------7---------8
#ifndef USED_TINKER_CONFIG_H
#define USED_TINKER_CONFIG_H
#if defined(__GNUC__)
#include <tinker/config.h>
#endif
#endif				// USED_TINKER_CONFIG_H

//------1---------2---------3---------4---------5---------6---------7---------8
// 2'nd level tuning. Get ARC specific default tuning values
//
// This section must not be left out by the normal header omission technique.
// It has to be re-red over and offer again for the macros to work and change
// according to where/how they're used
//------1---------2---------3---------4---------5---------6---------7---------8

#if defined(_WIN32) &&  defined(_MSC_VER)
#include <../bsp/X86_msvc/tk_tuning.h>

#elif defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
#include <../bsp/X86_Borland/tk_tuning.h>

#elif defined( __C166__ )
#include <../bsp/XC167_Keil/tk_tuning.h>

#elif defined(__GNUC__)

#ifndef ARCH
#error For GNU targets, ARCH has to be defined
#endif

   //Define the include wrapper temporally
#define INCLNAME( farch ) \
      <../bsp/gnu_ farch/tk_tuning.h>

   //Include the BSP specifig tk_tuning.h file
#include INCLNAME( ARCH )

   //Undefine the macro wrapper again so that we can re-use it
#undef INCLNAME

#else
#error "Can\'t determine the tool-chin you're using for the TINKER kernel"

#endif				//__GNUC__

//------1---------2---------3---------4---------5---------6---------7---------8
// 3'nd level tuning. Catch-all tuning fall-back NOTE Read the intro above...
//------1---------2---------3---------4---------5---------6---------7---------8

#ifndef TINKER_TUNING_CATCHALL
#define TINKER_TUNING_CATCHALL

	//App & system
#ifndef TK_MINIMUM_STACK_SIZE
#define TK_MINIMUM_STACK_SIZE 	0x200
#endif

#ifndef TK_NORMAL_STACK_SIZE
#define TK_NORMAL_STACK_SIZE  	0x200
#endif

	//SCHED
#ifndef TK_MAX_THREADS
#define TK_MAX_THREADS          50
#endif

#ifndef TK_MAX_PRIO_LEVELS
#define TK_MAX_PRIO_LEVELS      5
#endif

#ifndef TK_MAX_THREADS_AT_PRIO
#define TK_MAX_THREADS_AT_PRIO  10
#endif

#ifndef TK_THREAD_NAME_LEN
#define TK_THREAD_NAME_LEN      0x4
#endif

	//ITC
#ifndef TK_MAX_BLOCKED_ON_Q
#define TK_MAX_BLOCKED_ON_Q    	TK_MAX_THREADS
#endif

#ifndef TK_MAX_NUM_Q
#define TK_MAX_NUM_Q       	TK_MAX_THREADS
#endif

	//Functions and macros how-to's
#ifndef TK_HOWTO_MALLOC
#define TK_HOWTO_MALLOC 	TK_FNK_STUBBED
#endif

#ifndef TK_HOWTO_CLOCK
#define TK_HOWTO_CLOCK  	TK_FNK_STUBBED
#endif

#ifndef TK_HOWTO_PRINTK
#define TK_HOWTO_PRINTK 	TK_FNK_VOIDED
#endif

#ifndef TK_HOWTO_ASSERT
#define TK_HOWTO_ASSERT 	TK_FNK_RENAMED
#endif

#endif				// TINKER_TUNING_CATCHALL

#ifndef TK_TUNING_H_ROOT_HEADER
#define TK_TUNING_H_ROOT_HEADER
//FIXME I know other ports still have this somewhere else. Those should be moved here also
//the #if defined(__GNUC__) remains until then...
#if defined(__GNUC__)

	/*!
	   How printk is implemented on this target. I.e. ability to output on console.
	   - TK_FNK_VOIDED              Digests the call
	   - TK_FNK_ORIGINAL    Uses printf for output
	   - TK_FNK_RENAMED     Prints on stderr

	   @note macro should digest <b>all</b> arguments if TK_FNK_VOIDED is used. Therefore
	   doubble parenthesis have to be used. I.e. printk(("This is how..."))
	 */
#if (TK_HOWTO_PRINTK == TK_FNK_VOIDED)
#define printk(x) ((void)0)
#elif (TK_HOWTO_PRINTK == TK_FNK_ORIGINAL)
#define printk(x) printf x
#elif (TK_HOWTO_PRINTK == TK_FNK_RENAMED)
#define eprintf(...) fprintf (stderr, __VA_ARGS__)
#define printk(x) eprintf x
#else
#error Can not handle requested option for printk
#endif

#if (!(TK_SYSTEM == __SYS_HIXS__))
		/*! How to initializing the BSP is done on this target (Needed
		   for bare bone-metal targets). If the kernel is intended to run
		   under Linux or Cygwin we don't need/want these functions.
		   Otherwise, they belong implemented somewhere under bsp/<target> */
#define tk_bsp_sysinit _tk_bsp_sysinit
#if TK_HOSTED
#if TK_COMP_PTHREAD == __tk_no
#define tk_root root
#endif
#endif
#define BOOT_BSP_STUB
#endif

	/*
	   NOTE For Linux/Cygwin, the following might be required
	   #define printk(x) { printf x; fflush(stdout); }
	 */

#endif				//if defined(__GNUC__)
#endif				//TK_TUNING_H_ROOT_HEADER
//------1---------2---------3---------4---------5---------6---------7---------8
//
//------1---------2---------3---------4---------5---------6---------7---------8

