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

