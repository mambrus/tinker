/***************************************************************************
 *   Copyright (C) 2005,2006 by Michael Ambrus                             *
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

#ifndef TK_HWSYS_H
#define TK_HWSYS_H

#if defined(DOXYGEN_ONLY)

//------1---------2---------3---------4---------5---------6---------7---------8

/*
@ingroup kernel_internals

@brief Sets a minimum safety area from stack bottom.

This value is used when kernel is crashing due to stack-starvation
Value should be set as low as possible, but just enough to allow a few extra
invocations of printk to output some diagnostic text about the reason of the
crash.
*/

#define SAFEZONE  0

/*
@ingroup kernel_internals

@brief Defines the least amount of stack you can use

Never go below this. In fact, don't even come close. Remember ISR might put return
addresses at your threads stack without your explicit control.

@note The idle thread uses this-stack size. Remember that this thread will be the
most likely to be in use when interrupts happen. You don't want to make the size
too small.

*/
#define TK_MINIMUM_STACK_SIZE 0

/*
@ingroup kernel_internals

@brief Defines a normal stack size

Thread with this stack size should be able to handle printf

@note What's normal or reasonable differs between architectures.

*/
#define TK_NORMAL_STACK_SIZE 0

/**
@ingroup kernel_internals

@brief Prevents <b>all</b> interrupts on the target

Will increase a counter after the actual CLI.
The counter aids nesting (nesting however be avoided),

*/
#define TK_CLI()

/**
@ingroup kernel_internals

@brief Restores <b>all</b> interrupts on the target

Actual STI will not be invoked until counter set by TK_CLI is zero.
This aids nesting (should however be avoided),

*/
#define TK_STI()

/**
@ingroup kernel_internals

@brief Call for error handler

Dieing kernel will call this macro as part of it's error-handling. This
is typically implemented as a software interrupt on targets that support
that. On others, this is just a call to a function.

*/
#define TRAP( NUM )

/**
@ingroup kernel_internals

@brief Pushes all CPU on current stack

Pushes all CPU on stack in a way that corresponds to what happens when an
interrupt. Pushes will (and should be) on the current stack in scope.

@note The rule to follow is not needed in a non-preemptive version of
of the kernel, but a very good rule to follow just in case you decide to add
preemptive ability later.

@todo Verify for XC167

*/
#define PUSHALL()

/**
@ingroup kernel_internals

@brief Pops all CPU from current stack

Pops all CPU from current stack the same way an interrupt does when it's ISR is
finished.

@note You have to change the SP to the new stack before using this macro

@todo Verify for XC167

*/
#define POPALL()

/**
@ingroup kernel_internals

@brief Prepares Top Of Stack
@obsolete

Use this macro to set up a until stack content on a <B>newly</B> created
threads stack so that the stack follows two requirements:<br><br>

1. The return address shall be the start address of the threads function

2. The stack shall follow the same rules as PUSHALL and POPALL, i.e it should
follow the rule that the top of the stack should be the same as when an
interrupt has occurred.

@note Some extra storage variables are needed to manipulate the SP. <b> These
might not be alocatd on the same stack they manipulate.</B> Best rule to follow
is to have these variables in the code segment (i.e. globally declared).

*/
#define PREP_TOS(                                                             \
   _oldTOS, /*< in:  The newly created stacks start address */                 \
   _newSP,  /*!< out: The resulting SP. I.e. (char*)(TSP1+lengt-tos_data)*/   \
   _temp1,  /*!< Temporary stack-pointer for internal manipulations*/          \
   _temp2,  /*!< Extra variable for internal stuff (not needed by all targets)*/\
   _stack_struct /*!< Some targets need to manipulate the TCB stack struct*/  \
)

/**
@ingroup kernel_internals

@brief Pushes all CPU on current stack and return SP.

Pushes all CPU on current stack and returns the stack pointer after that push

Typically used in the first stage of a context switch. The resulting SP is
saved in the threads TCB to be used when it's time to revive it.

*/

#define PUSH_CPU_GETCUR_STACK(                                                \
   TSP1,    /*!< The resulting SP                                              */\
   TEMP     /*!< Extra variable for internal stuff (not needed by all targets)*/\
)

/**
@ingroup kernel_internals

@brief Change to new SP and pop all CPU

Change SP to the one supplied and push all CPU from it.

Typically used in the second stage of a context switch. The intended SP is retrieved
from the corresponding threads TCB.

*/
#define CHANGE_STACK_POP_CPU(                                                 \
   TSP1,                                                                      \
   TEMP                                                                       \
)

/**
@ingroup kernel_internals

@brief Dunno what this is ;/

@todo If this macro is really needed, document it also!
*/

#define GET_THREADS_RETVAL( THRETVAL )

/**
@ingroup kernel_internals

@brief Refer to a stack address as a pointer.

Refer to a stack address as a pointer. Only the stacks actual address
will be handled (assuming the whole of a stack can be referred to with
one address).

@note In case you system has complex stacks you might need to
post-adjust this.

@note In a XC167/Keil combo target, a thread has two stacks. One
system-stack (for return addresses and parameter passing) and one
user-stack (local function data). Only the real stack, i.e. the system
stack, is treated with this macro. Since we assume (i.e. we have
defined), that there is always one and only one address for a stack there
must be a relation between system- and user-stacks, and any user-stack
address can be deducted from the system-stack address. However, note that
to set everything up correct in the structure one has to run a
post-operative stack normalization.

@note This will work on the targets supported up until TinKer's state up
until now (051123), but might need to be reworked or replaced with a
function later on. In future implementations a stack_t might not carry
an linear address at all, but a id. In that case a simple macro will not
do at all.
*/

#define STACK_PTR( ADDR )

/*
@brief Initialize or re-initialize a stack_t address

On some targets where the stack address in complex, this macro makes the
pos operative operation mentioned for STACK_PTR. On targets where
stack_t is a simple char*, this macro does nothing.

@note Always remember to run this macro after a assignment of a new
stack address if you want the stack to have up-to-date structural
information.

@note In a XC167/Keil combo target, this is especially important. The
macro recalculates a user stack, based on the system-stack and some
additional information about where to find it (hard-coded and internal).

*/
#define REINIT_STACKADDR( ADDR, size )

/*
@brief Gets the real stack size.

Gets the real stack size to be used to get top of stack for return addresses.

@note <b>NOT TO BE USED FOR CALC ADRESSES FOR DATA</b> (some obscure
processors make a difference between those stacks.)

*/
#define REAL_STACK_SIZE( ADDR )

/*
@brief Inspects the stack for overflow errors.

Inspects the stack and tries to find stack overflows. On some targets this can
be implemented in hardware. On others who have more than one stack per
thread, one stack might be HW guarded and the other not. Implementation
of this macro varies a lot between targets.

@note this macro will only make a simple LWM check using its own
stack-pointer. In a situation where a thread has made deep recursion
but nested all the way back, the stack-pointer could be OK, but it could
still have trashed neighbouring threads stacks.

*/

#define TRY_CATCH_STACK_ERROR( STACK_T, TEMP )

/*
@ingroup kernel_internals

@brief Calculate a stack integrity value.

Calculate a stack integrity value on the stack pointed out by arg 1

@note Due to that this operation is rather costly, certain targets or
cases might want to replace this with a NOP.

*/
#define INTEGRITY_CERTIFY_STACK( TCB_T, TEMP )

/*
@ingroup kernel_internals

@brief Calculate a stack integrity value.

Calculate a stack integrity value on the stack pointed out by arg 1

@note Due to that this operation is rather costly, certain targets or
cases might want to replace this with a NOP.

*/
#define TRY_CATCH_STACK_INTEGRITY_VIOLATION( TCB_T, TEMP )

#endif				//DOXYGEN_ONLY

//------1---------2---------3---------4---------5---------6---------7---------8
#if defined(_WIN32) &&  defined(_MSC_VER)
#include <../bsp/X86_msvc/tk_hwsys_msvcX86.h>

#elif defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
#include <../bsp/X86_Borland/tk_hwsys_borlandX86.h>

#elif defined( __C166__ )
#include <../bsp/XC167_Keil/tk_hwsys_keilC166.h>

#elif defined(__GNUC__)

#ifndef ARCH
#error For GNU targets, ARCH has to be defined
#endif

#define INCLNAME( farch ) \
      <../src/arch/farch/context-gnu_ farch.h>

#define INCLABI( farch, abi ) \
      <../src/arch/farch/context-gnu_ farch-abi.h>

#ifdef ABI
#include INCLABI( ARCH, ABI )
#else
#include INCLNAME( ARCH )
#endif

#undef str
#undef INCLNAME
#undef INCLABI

#else
#error "Can\'t determine the target for the TINKER kernel"

#endif

//------1---------2---------3---------4---------5---------6---------7---------8

#endif				//TK_HWSYS_H

/*!
 * @defgroup CVSLOG_tk_hwsys_h tk_hwsys_h
 * @ingroup CVSLOG
 *  $Log: context.h,v $
 *  Revision 1.1  2006-11-05 14:18:59  ambrmi09
 *  Build system and source modified to make better use of config.h
 *
 *  This file now contains information about how the kernel is configured
 *  and can be used by both application and kernel build (old solution only
 *  let kernel-buils know of these details).
 *
 *  This applies to both tk_tuning, component configuration among others.
 *  Use './configure --help' to see a full list. Note that  if a certain
 *  feature is not configured, the old tk_tuning will fill in the gaps.
 *  This is especially usefull when not using GNU build- and configure-
 *  tool-chain. Hopefully, we'll be able to get rid of tk_tuning.h in the
 *  future.
 *
 *  Revision 1.20  2006/04/08 10:15:58  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.19.2.2  2006/04/03 20:07:23  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.19.2.1  2006/03/31 17:42:56  ambrmi09
 *  Second idea for the new thread starter. This one plays nice with several
 *  more compilers beacuse of it's balances call-stack. It's not as
 *  beautiful as the former one IMO, but GNU is a pain in the but
 *  with it's call-stack optimizations (and decorations doesn't seem to work
 *  for Cygwin GCC ).
 *
 *  Revision 1.19  2006/03/19 12:44:36  ambrmi09
 *  Got rid of many compilation warnings. MSVC amd GCC actually gompiles
 *  without one single warning (yay!). Be aware that ther was a lot of
 *  comparisons between signed/unsigned in ITC. Fetts a bit shaky...
 *
 *  Revision 1.18  2006/03/11 14:37:48  ambrmi09
 *  - Replaced printf with printk in in-depth parts of the kernel. This is
 *  to make porting easier since printk can then be mapped to whatever
 *  counsole output ability there is (including none if there isn't any).
 *
 *  - Conditionals for: 1) time ISR latency and 2) clock systimer faliure
 *  introduced. This is because debugging involves stopping the program but
 *  not the clock HW, which will trigger the "trap" as soon as resuming the
 *  program after a BP stop (or step). I.e. inhibit those part's when
 *  debugging (which is probably most of the time). Remeber to re-enable for
 *  "release" version of any application.
 *
 *  - Working on getting rid of all the compilation warnings.
 *
 *  - Detected a new serious bug. If an extra malloc is not executed
 *  *before* the first thread is created that requires a steck  (i.e. the
 *  idle tread sice root allready has a stack), that thread will fail with
 *  an illegal OP-code trap. This has been traced to be due to a faulty
 *  malloc and/or possibly a memory alignement problem. The first block
 *  allocated, will be about 6 positions to high up in the memory map, which
 *  means that sthe total block will not really fit. If that block is the
 *  stack of a thread, those positions will be either the context or the
 *  return adress of that thread (which is bad). I'm concerned about not
 *  detecting this error before, which leads me to believe that this
 *  actually is an alignement issue in malloc and it's anly pure chance
 *  wheather the bug will manifest or not. This is a problem related
 *  to the Keil_XC167 target only.
 *
 *  Revision 1.17  2006/03/05 11:11:25  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.16  2006/03/04 19:32:34  ambrmi09
 *  Modified sources to allow build GNU targets transparently.
 *
 *  Revision 1.15  2006/03/02 15:33:08  ambrmi09
 *  Experimenting with build under Linux
 *
 *  Revision 1.14  2006/03/02 14:05:48  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.13  2006/02/28 13:18:19  ambrmi09
 *  Got MSVC target back-on track also.
 *
 *  Seems both MSVC and BCC have the same "error" regarding \ref CLK_TICK
 *  and/or \ref clock(). Could it be that it's TinKer's interpretation that
 *  is in fact wrong?
 *
 *  Revision 1.12  2006/02/28 11:50:07  ambrmi09
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
 *  Revision 1.11  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.10  2005/12/03 14:04:30  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *
 *******************************************************************/
