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
@ingroup SCHED

@brief Tinker inner-most \e"guts"

This component is not selectable as the others normally are

For in-depth discussions about this component, see \ref
SCHED

@note (Very old note, probably ridiculous now) Allocate at least 2k stack if
you use printf in thread else 64bytes is probably sufficient.

@see COMPONENTS
*/

#ifndef TK_H
#define TK_H
#define TINKER

#if defined(__GNUC__)
#include <tinker/config.h>
#include <sys/types.h>		//Force header-file wrap fix-up (incompatible pthread types in newlib issue)

#if defined(_POSIX_THREADS)
#error newlib/libc headers somehow managed set _POSIX_THREADS and probably a whole set of others incompatible pthread related things
#endif
#endif

#include <stddef.h>
#include <time.h>

enum SCHED_ERROR_CODES {
/*First are TiNker codes*/
/*COMPONENT SCHED*/
	TK_OK = 0,		//!< @brief Operation returned without errors
	TK_ERROR,		//!< @brief General TinKer catch all error
	TK_SENTINEL
};

#define ERR_OK TK_OK		//!< TinKer: No error

/*!
A simple macro to help rid of "parameters not used" warnings. Often
tread some tread arguments are not used. Warnings will clobber your
compiler output and make you less observant of truly dangerous warnings.
*/
#define TK_NO_WARN_ARG(x) ((void)x)

#ifdef __doxygen__
#error This section should never be enabled for a normal build
/*!
@name SCHED trimming constants

@note
Only information about these defines are here. The true defines are
in each targets corresponding tk_tuning.h.

- TK_MAX_THREADS_AT_PRIO should be at least as big as TK_MAX_THREADS
- TK_MAX_THREADS is the big memory hog. It determines the pool size
- More or less TK_MAX_PRIO_LEVELS doesn't make that much difference in
  memory usage, but keep it low anyway. It's doubtful if it makes sense
  to have a lot of priority levels anyway.

  Modify these constants to get a kernel of desired size/speed ratio

@warning </b>Do not change these if using pre-built kernel as lib</b>
*/
//@{
#define TK_MAX_THREADS           50	//!< Maximum number of threads kernel can handle
#define TK_MAX_PRIO_LEVELS       0x10	//!< Maximum number of priority levels. @todo fix this, Idle needs to bee last in last prio( needs one extra )
#define TK_MAX_THREADS_AT_PRIO   TK_MAX_THREADS	//!< Maximum number of threads at a priority at any one time
#define TK_THREAD_NAME_LEN       0x17	//!< Number of characters (at most) in the TCB identifying the thread. @note If this is zero, which is valid, no human readable info about the thread is stored @note A size of 4 bytes is what used to be standard in pSos
//@}
#endif

//Some defines for the logic
#define YES                      1
#define NO                       0

//Figure out how to do the following some other time...
//#if ( TK_MAX_THREADS * TK_NORMAL_STACK_SIZE > SIZEOF_STACKPOOL )
//# error YOU HAVE TOO LITTLE MEMORY FOR THE NUMBER OF THREADS YOU HAVE DEFINED
//#endif

/*!
@name Build including following components

Modify these constants to include/exclude the following \ref COMPONENTS

Use either YES or NO to either include or omit.

@note </b>Do not change these if using pre-built kernel as lib</b>

You can set the options from command line also:
-DTK_COMP_ITC=1,-DTK_COMP_PTIMER=1,-DTK_COMP_KMEM=1,-DTK_COMP_PTHREAD=1,-DTK_COMP_POSIX_RT=1

MSVC "preprocessor directive" would be:
TK_COMP_ITC=1,TK_COMP_PTIMER=1,TK_COMP_KMEM=1,TK_COMP_PTHREAD=1,TK_COMP_POSIX_RT=1

@note Settings can be over-ruled by compile-time defies or confugure options
*/
//@{
#ifndef  TK_COMP_ITC
#define  TK_COMP_ITC       YES	//!< @brief \ref ITC
#endif
#ifndef  TK_COMP_PTIMER
#define  TK_COMP_PTIMER    NO	//!< @brief \ref PTIMER
#endif
#ifndef  TK_COMP_KMEM
#define  TK_COMP_KMEM      NO	//!< @brief \ref KMEM
#endif
#ifndef  TK_COMP_PTHREAD
#define  TK_COMP_PTHREAD   NO	//!< @brief \ref PTHREAD
#endif
#ifndef  TK_COMP_POSIX_RT
#define  TK_COMP_POSIX_RT  NO	//!< @brief \ref POSIX_RT
#endif

//@}

//Kernel termination codes (bit adressable) -

/*!
@name Kernel termination Trap Codes (bit addressable)

When TinKer itself dies (i.e. exits), it will do so using a exit code
telling about the reason of the exit.

TinKer exit-codes (Trap Codes) are bit addressable. This means an exit-code
can contain multiple reasons for exit. To distinguish from normal errno and
TK_ERROR codes, most significan bit is set.

@note Per \b kernel and \b not per \b thread codes
*/
//@{
#define TC_ISA_ERR         0x8000	//!< Mark that this is a TinKer exit-code ( "TC" = Trap Code) error
#define TC_NOERR           (0x0000 | TC_ISA_ERR)	//!< Termination without errors

#define TC_MAX_THREADS     (0x0001 | TC_ISA_ERR)	//!< Total amount of threads would exceed limit
#define TC_ERR_ASSERT      (0x0002 | TC_ISA_ERR)	//!< Assertion failed
#define TC_MAX_PRIO_LEVELS (0x0004 | TC_ISA_ERR)	//!< Chosen priority too high
#define TC_ERR_4           (0x0008 | TC_ISA_ERR)

#define TC_ERR_STACK       (0x0010 | TC_ISA_ERR)	//!< Stack out of bounds check faliure
#define TC_ERR_STKINT      (0x0020 | TC_ISA_ERR)	//!< Stack integrity faliure detected
#define TC_THREADS_AT_PRIO (0x0040 | TC_ISA_ERR)	//!< To many threads at this prio
#define TC_AMOK            (0x0080 | TC_ISA_ERR)	//!< Kernel running amok detected

#define TC_NAME_LEN        (0x0100 | TC_ISA_ERR)	//!< Thread-name to long
#define TC_NOMEM           (0x0200 | TC_ISA_ERR)	//!< No memory left for allocation
#define TC_TCB_INVALID     (0x0400 | TC_ISA_ERR)	//!< Invalid TCB detected
#define TC_ERR_12          (0x0800 | TC_ISA_ERR)

#define TC_ERR_13          (0x1000 | TC_ISA_ERR)
#define TC_ERR_14          (0x2000 | TC_ISA_ERR)
#define TC_ERR_HW          (0x4000 | TC_ISA_ERR)	//!< HW driver detected a fatal error
#define TC_UNKNOWN         (0x8000 | TC_ISA_ERR)	//!< Unknown (or undefined) termination reason
//@}

typedef enum { TK_FALSE, TK_TRUE } TK_BOOL;
typedef void *start_func_ft(void *);
typedef start_func_ft *start_func_f;

typedef void init_func_ft(void *);
typedef init_func_ft *init_func_f;

typedef void isr_ft(void);
typedef isr_ft *isr_handler;

extern init_func_f boot_hook;
#if defined(HAVE_UINTPTR_T)
#include <stdint.h>
typedef uintptr_t tin_t;
#else
typedef int tin_t;		//!< A tinker        ID
#endif
typedef tin_t thid_t;		//!< A tinker thread ID
typedef tin_t titc_t;		//!< A tinker ITC    ID
typedef tin_t titi_t;		//!< A tinker pTimer ID

/*- default settings **/

/*- external functions **/

/*- external data **/

//*- public functions **/
//------1---------2---------3---------4---------5---------6---------7---------8

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

	int tk_delete_thread(thid_t PID);
	thid_t tk_create_thread(char *name,
				unsigned int prio,
				start_func_f start_func,
				void *inpar, size_t stack_size);
	int tk_join(thid_t, void **);
	int tk_detach(thid_t);
	void tk_create_kernel(void);
	void tk_delete_kernel(void);
	void tk_yield(void);
	void tk_yield_event(void);
	void tk_exit(int ec);
	void tk_msleep(unsigned int time_ms);
	thid_t tk_thread_id(void);
	int tk_change_prio(thid_t tid, int newPrio);
	int tk_isr_install(int level, isr_handler isr);
	void tk_isr_eoi(int level);
	void tk_preemplist(thid_t);

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#ifdef __cplusplus
extern "C" {
#endif

/*! Function supplied by \b YOU - constitutes the root thread function.
Aims "upwards" in the chain

@note: We can't impose of forse which compiler (C or C++) will be used by
the application. Therefor we must enforce the mangling of this function name
to follow C regardless of which compiler type is used to build TinKer itself.
*/

	extern int tk_root(void);

#ifdef __cplusplus
}
#endif
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

/*! Function supplied by \b YOU - initialize the BSP used by your sys-calls
Aims "downwards" in the chain
*/
	extern int tk_bsp_sysinit(void);

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
//------1---------2---------3---------4---------5---------6---------7---------8
// The following are rerouted by bacros, so they need to be public. Don't use
// in appl. code
//void           _tk_assertfail( char *assertstr, char *filestr, int line ); <-- moved to assert.h
//int           *_tk_errno(); <-- moved to errno.h
//------1---------2---------3---------4---------5---------6---------7---------8

///*- private functions **/
#if defined(TINKER)
#define main(x,y) tk_root()
#endif
#endif
