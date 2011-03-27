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
   #include <sys/types.h>	//Force header-file wrap fix-up (incompatible pthread types in newlib issue)

   #if defined(_POSIX_THREADS)
   #error newlib/libc headers somehow managed set _POSIX_THREADS and probably a whole set of others incompatible pthread related things
   #endif
#endif

#include <stddef.h>
#include <time.h>


enum SCHED_ERROR_CODES{
/*First are TiNker codes*/
/*COMPONENT SCHED*/
TK_OK=0          ,//!< @brief Operation returned without errors
TK_ERROR         ,//!< @brief General TinKer catch all error
TK_SENTINEL
};

#define ERR_OK TK_OK            //!< TinKer: No error

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
#define TK_MAX_THREADS           50    //!< Maximum number of threads kernel can handle
#define TK_MAX_PRIO_LEVELS       0x10  //!< Maximum number of priority levels. @todo fix this, Idle needs to bee last in last prio( needs one extra )
#define TK_MAX_THREADS_AT_PRIO   TK_MAX_THREADS  //!< Maximum number of threads at a priority at any one time
#define TK_THREAD_NAME_LEN       0x17  //!< Number of characters (at most) in the TCB identifying the thread. @note If this is zero, which is valid, no human readable info about the thread is stored @note A size of 4 bytes is what used to be standard in pSos
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
#define  TK_COMP_ITC       YES         //!< @brief \ref ITC
#endif
#ifndef  TK_COMP_PTIMER
#define  TK_COMP_PTIMER    NO          //!< @brief \ref PTIMER
#endif
#ifndef  TK_COMP_KMEM
#define  TK_COMP_KMEM      NO          //!< @brief \ref KMEM
#endif
#ifndef  TK_COMP_PTHREAD
#define  TK_COMP_PTHREAD   NO          //!< @brief \ref PTHREAD
#endif
#ifndef  TK_COMP_POSIX_RT
#define  TK_COMP_POSIX_RT  NO          //!< @brief \ref POSIX_RT
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
#define TC_ISA_ERR         0x8000                 //!< Mark that this is a TinKer exit-code ( "TC" = Trap Code) error
#define TC_NOERR           (0x0000 | TC_ISA_ERR)  //!< Termination without errors

#define TC_MAX_THREADS     (0x0001 | TC_ISA_ERR)  //!< Total amount of threads would exceed limit
#define TC_ERR_ASSERT      (0x0002 | TC_ISA_ERR)  //!< Assertion failed
#define TC_MAX_PRIO_LEVELS (0x0004 | TC_ISA_ERR)  //!< Chosen priority too high
#define TC_ERR_4           (0x0008 | TC_ISA_ERR)

#define TC_ERR_STACK       (0x0010 | TC_ISA_ERR)  //!< Stack out of bounds check faliure
#define TC_ERR_STKINT      (0x0020 | TC_ISA_ERR)  //!< Stack integrity faliure detected
#define TC_THREADS_AT_PRIO (0x0040 | TC_ISA_ERR)  //!< To many threads at this prio
#define TC_AMOK            (0x0080 | TC_ISA_ERR)  //!< Kernel running amok detected

#define TC_NAME_LEN        (0x0100 | TC_ISA_ERR)  //!< Thread-name to long
#define TC_NOMEM           (0x0200 | TC_ISA_ERR)  //!< No memory left for allocation
#define TC_TCB_INVALID     (0x0400 | TC_ISA_ERR)  //!< Invalid TCB detected
#define TC_ERR_12          (0x0800 | TC_ISA_ERR)

#define TC_ERR_13          (0x1000 | TC_ISA_ERR)
#define TC_ERR_14          (0x2000 | TC_ISA_ERR)
#define TC_ERR_HW          (0x4000 | TC_ISA_ERR)  //!< HW driver detected a fatal error
#define TC_UNKNOWN         (0x8000 | TC_ISA_ERR)  //!< Unknown (or undefined) termination reason
//@}

typedef enum{TK_FALSE,TK_TRUE}TK_BOOL;
typedef void *start_func_ft(void *);
typedef start_func_ft   *start_func_f;

typedef void init_func_ft(void *);
typedef init_func_ft   *init_func_f;

typedef void isr_ft(void);
typedef isr_ft   *isr_handler;


extern init_func_f boot_hook;

typedef int tin_t;      //!< A tinker        ID
typedef tin_t thid_t;   //!< A tinker thread ID
typedef tin_t titc_t;   //!< A tinker ITC    ID
typedef tin_t titi_t;   //!< A tinker pTimer ID


/*- default settings **/

/*- external functions **/

/*- external data **/

//*- public functions **/
//------1---------2---------3---------4---------5---------6---------7---------8

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif


int            tk_delete_thread(thid_t PID);
thid_t         tk_create_thread(
                  char          *name,
                  unsigned int   prio,
                  start_func_f   start_func,
                  void          *inpar,
                  size_t         stack_size
               );
int            tk_join(thid_t , void **);
int            tk_detach( thid_t );
void           tk_create_kernel( void );
void           tk_delete_kernel( void );
void           tk_yield( void );
void           tk_yield_event( void );
void           tk_exit( int ec );
void           tk_msleep( unsigned int time_ms );
thid_t         tk_thread_id( void );
int            tk_change_prio(thid_t tid, int newPrio);
int            tk_isr_install(int level, isr_handler isr);
void           tk_isr_eoi(int level);
void           tk_preemplist(thid_t);

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

extern int     tk_root( void ); 

#ifdef __cplusplus
}
#endif


#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

/*! Function supplied by \b YOU - initialize the BSP used by your sys-calls
Aims "downwards" in the chain
*/
extern int     tk_bsp_sysinit(void);

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif


//------1---------2---------3---------4---------5---------6---------7---------8
// The following are rerouted by bacros, so they need to be public. Don't use 
// in appl. code

//void           _tk_assertfail( char *assertstr, char *filestr, int line ); <-- moved to assert.h
//int           *_tk_errno(); <-- moved to errno.h
//------1---------2---------3---------4---------5---------6---------7---------8

/*- private functions **/

#if defined(TINKER)
   #define main(x,y) tk_root() 
#endif

#endif

  
/*! 
 * @defgroup CVSLOG_tk_h tk_h
 * @ingroup CVSLOG
 *
 *  $Log: tk.h,v $
 *  Revision 1.50  2007-03-23 20:27:23  ambrmi09
 *  1) Reorganization of ITC into several smaller files
 *  2) Component pthread now supports 3,5,9 and 16 priorities
 *
 *  Revision 1.49  2007-03-19 17:07:55  ambrmi09
 *  Interrupt structure/handling reworked - Motorola is wacked, clear IRQ by
 *  setting allready bit in SIPEND (!)
 *
 *  Revision 1.48  2007-03-04 19:07:25  ambrmi09
 *  1) Error handling refined - will handle error from different
 *     cathegories:
 *     - errno (perror)
 *     - TK errors
 *     - TK traps codes
 *     - exit handling can differ beween user exit codes and kernel
 *       trap codes.
 *  2) Extracted fluffy & un-critical code from tk.c (the error and exit
 *     stuff)
 *  3) Preparing to partition even further into tk_switch.c (saving this
 *     until next ci though).
 *
 *  Revision 1.47  2007-02-26 14:16:53  ambrmi09
 *  1) Drivers    - structure added
 *  2) Filesystem - in progress
 *
 *  Revision 1.46  2007-02-22 12:36:48  ambrmi09
 *  1) Structure adapted for modulerizing components under tinker/src
 *     in the same fashion as the modules under tinker/bsp
 *  2) New component added TK_FILESYS (resides under src/filesys)
 *
 *  Revision 1.45  2007-02-21 01:01:24  ambrmi09
 *  Was a very good and productive day! Kernel runs on new target, but stacks
 *  get busted. i think it's the setjmp, longjump think thats messing with us.
 *  It's not saving enough registers or the stack is creaping away fo some other
 *  reason.
 *
 *  The error seems quite easy though (relativly speaking i.e. ...)
 *
 *  Revision 1.44  2006/12/12 10:57:05  ambrmi09
 *  This adresses the second part of #1609064
 *
 *  Revision 1.43  2006/12/11 14:41:52  ambrmi09
 *  Solves #1609064 (part1)
 *
 *  Revision 1.42  2006/11/27 22:29:23  ambrmi09
 *  Minor djustments completeing the move of some header files to public and due
 *  to some name clashed with user space naming conventions.
 *
 *  Revision 1.41  2006/11/05 14:18:58  ambrmi09
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
 *  Revision 1.40  2006/04/08 10:15:58  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.39.2.6  2006/04/07 12:10:07  ambrmi09
 *  Skeleton for handling syscalls using the ARM Angel interface in place
 *
 *  Basic terminal I/O for gnu_arm (LPC2129) - only output so far (input requires
 *  blocking).
 *
 *  Revision 1.39.2.5  2006/04/06 09:01:56  ambrmi09
 *  Safety commit due to change of local sandbox FS type (had files checked out
 *  while changing the type - not to be recommended).
 *
 *  Revision 1.39.2.4  2006/04/03 20:07:23  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.39.2.3  2006/04/03 15:21:47  ambrmi09
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
 *  Revision 1.39.2.2  2006/03/31 17:42:56  ambrmi09
 *  Second idea for the new thread starter. This one plays nice with several
 *  more compilers beacuse of it's balances call-stack. It's not as
 *  beautiful as the former one IMO, but GNU is a pain in the but
 *  with it's call-stack optimizations (and decorations doesn't seem to work
 *  for Cygwin GCC ).
 *
 *  Revision 1.39.2.1  2006/03/30 10:52:20  ambrmi09
 *  First version of new threadstarter. It seems very promising. A *lot* of
 *  awfull pain concerning different targets has the potential to go away.
 *
 *  Revision 1.39  2006/03/24 11:22:55  ambrmi09
 *  - pThreads RW locks implemented (rough aproach - no usage error detection)
 *  - restructuring of the pThread src-files
 *
 *  Revision 1.38  2006/03/19 22:57:54  ambrmi09
 *  First naive implementation of a pthread mutex
 *
 *  Revision 1.37  2006/03/19 12:44:35  ambrmi09
 *  Got rid of many compilation warnings. MSVC amd GCC actually gompiles
 *  without one single warning (yay!). Be aware that ther was a lot of
 *  comparisons between signed/unsigned in ITC. Fetts a bit shaky...
 *
 *  Revision 1.36  2006/03/17 12:20:03  ambrmi09
 *  Major uppdate (5 days hard work)
 *
 *  - Finally tied up all loose ends in the concept. Threads are now
 *  joinable
 *
 *  - Corrected one error: compacting scheduele while cancelling a
 *  threads
 *
 *  - Several new API, mainly concerned with cancelation (corrsp pThread
 *  also)
 *
 *  - Found a nasty bug while creating threads in threads for XC167. TOS is
 *  really a patchy solution ;( This one had to do with the compiler
 *  being fooled by the inline assembly and optimized something that was not
 *  optimizable (saving stack segment got wacked).
 *
 *  - Designed a concurrent qsort test-app. This is good for showing
 *  boss-worker model. Number of threads recoed on XC167 was 50 and on MSVS
 *  more than 150! Interesting to notice was that TinKer creation and
 *  cancelation for threads was much faster than Windows own (20-30 times
 *  faster).
 *
 *  - A MSVC workspace for pThreads-Win32. Good for testing apps
 *  transparency.
 *
 *  - Increased memory on XC167 (phyCore HW). now 32k for stacks and 16k for
 *  malloc. We still lack RAM that is not deployed (pHycore has
 *  128k + 256k physical RAM memory i think). Maximum for
 *  stack is 64k however (type of pointers determine this). If memory is
 *  increased further, we get a TRAP_B saying bad memory interface. Typical
 *  error for config memory issues in DaVe.
 *
 *  Revision 1.35  2006/03/11 14:37:48  ambrmi09
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
 *  Revision 1.34  2006/03/07 08:24:13  ambrmi09
 *  A very crude port for ARM is running (LPC2129) - @note THIS IS HIGHLY EXPERIMENTAL CODE
 *
 *  Revision 1.33  2006/03/05 11:11:24  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.32  2006/03/04 14:28:44  ambrmi09
 *  Finally got the \ref clock() representation right. Now timing is
 *  behaving equaly between the targets X86_Linux, Cygqing, MSVC, BC5 and
 *  XC167.
 *
 *  Revision 1.31  2006/03/02 14:05:48  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.30  2006/02/28 18:16:54  ambrmi09
 *  - Mainly a ci for the new Workspace structure
 *  - Houwever, found and corrected a bug in mqueue.c (a NULL pointer
 *    assignement)
 *
 *  Revision 1.29  2006/02/28 13:18:19  ambrmi09
 *  Got MSVC target back-on track also.
 *
 *  Seems both MSVC and BCC have the same "error" regarding \ref CLK_TICK
 *  and/or \ref clock(). Could it be that it's TinKer's interpretation that
 *  is in fact wrong?
 *
 *  Revision 1.28  2006/02/27 13:30:03  ambrmi09
 *  <b>Please read the in depth comments</b> about this check-in at \ref
 *  Blog051125
 *
 *  The test program (test.c) in this check-in is also particularly nasty
 *  since it gives really long latencies on each task switch (up to and
 *  above 500mS!). Test against this if you make any changes in either
 *  timing or dispatching. Even considering this hard case, the drift was
 *  very minor. The timing constants need re-trimming though.
 *
 *  Revision 1.27  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.26  2006/02/21 22:10:32  ambrmi09
 *  - Added wrapper macro for pthread_create so that posix threads get named in
 *    TinKer (makes post-mortem easier). Very cool solution with a macro...
 *  - Improved post-mortem, the schedule gets dumpt also now
 *  - Wrapper macros for msleep and usleep (temporary)
 *  - Minor stubbing and wrapping of mq_unlink and pthread_cancel
 *  - Added a new test program (t est-posix.c ). This is verifyed to compile and
 *    run on both Linux and TinKer unmodified!
 *
 *  Revision 1.25  2006/02/20 19:17:14  ambrmi09
 *  - Made the errno variable thread specific (each thread has it's own)
 *  - Hid the details of using errno so that setting and reading it looks
 *    like using a normal variable
 *  - Extracted some stuff from tk.h that doesn't need to be public
 *  - Implemented perros and strerror including a storage with all the error
 *    strings (will go into NV ROM on a embedded system).
 *
 *  Revision 1.24  2006/02/20 15:22:00  ambrmi09
 *  Documentation stuff. No code changes.
 *
 *  Revision 1.23  2006/02/19 22:00:38  ambrmi09
 *  Major brake-through!!! First working attempt with crude pThreads and
 *  POSIX RT queues works. (jihaa) :=D. Wow
 *
 *  Revision 1.22  2006/02/19 12:44:32  ambrmi09
 *  - Documented ITC
 *  - Started to build up the structure for the \ref PTHREAD component
 *
 *  Revision 1.21  2006/02/16 15:11:00  ambrmi09
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
 *  Revision 1.20  2006/02/02 15:51:02  ambrmi09
 *  A lot of thought has been invested into the new PTIME component. Had to
 *  change things even in the systime parts (integrated in the SHEDUL
 *  component) to make it more generic. Think this will be really nice when
 *  it's ready, but has been a long road to get PTIME running (and I'm
 *  still not there).
 *
 *  Revision 1.19  2005/12/04 15:48:52  ambrmi09
 *  API for ne pre-emptable timers in place. Implementing this will be a
 *  hard but fun "nut" to crack. ptime has the potential of comming
 *  very close to the high-res timers that POSIX 1003.1c define and is a
 *  good pointer whether pthreads is a good next step or not for TinKer.
 *
 *  Revision 1.18  2005/12/03 14:04:30  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *  Revision 1.17  2005/12/02 07:52:24  ambrmi09
 *  Working snap of preemtive sceduling. Comments saved of an alternative way
 *  (which has many advantages but is harder to get running).
 *
 *  Revision 1.16  2005/12/01 13:05:24  ambrmi09
 *  This check-in if for preparing for peemtive mechanism (first try)
 *  Done since last check-in
 *
 *  - Got rid of a nasty include bug for the target dependant sys-files
 *  - Added yet anoter sys header file with type only, so that kernel doesn't
 *    have to include the whole lot (which will render in  another "bug" due
 *    to that Keil and Dave define the same thing. This might be solved
 *    differently later, but the separation doesnt hurt anyway.
 *  - Started a concept of system queues - which I hope will be the basis for
 *    the TinKer drivers concept (yet to be invented).
 *  - Made first crude attempts with preemtion.
 *
 *  Revision 1.15  2005/11/30 22:21:22  ambrmi09
 *  Mechanism for detecting stack integrity violation introduced. It needs more
 *  work. An interrupt will taint the current stack if it's using any kernel
 *  functions. This is not what we want, but the main idea is captured in this
 *  check-in.
 *
 *  Revision 1.14  2005/11/27 20:02:00  ambrmi09
 *  - Detection of UserStack out-of bounds detection added.
 *  - Added error generic handling mechanism (TRAP)
 *  - Several new macros (that also need to be ported btw)
 *  - Tested error mechanisms for stacks quite well. Those issues should be OK now
 *
 *  Revision 1.13  2005/11/26 16:13:19  ambrmi09
 *  Relevant for XC167 only.
 *
 *  Code for protecting system stacks added. Noted that if recursed functions
 *  used withing a thread and local variables are on user stack, that user stack
 *  will grow much more rapidly. This will lead to a kernel break that
 *  STKOV/STKUN can't catch. Suggestion:
 *  1. Let dispatcher set current user stack pointer on each context switch also
 *  2. Check agains max,min values against info in TOB, and try to
 *     detect out of boundary usage (high risk stack is broken allready though
 *     but hopefuly not for the current thread or we will not be able to output
 *     any diagnosics at all).
 *  3. If out of bouds detected, create a soft trap (for consistency with
 *     STKOF/STKUN error handling).
 *  4. Improve post mortem info. IP and CSP can't possibly be correct now. Also
 *     dump whole CPU and the whole current TCB. Try to look up adress in symbol
 *     table and print function name if possible.
 *  5. LVM and HVM (water marks) might be combined with pkt2.
 *
 *  Deliberate "bug" left in code to help my memory while creating debugging
 *  situation (0xA0).
 *
 *  Revision 1.12  2005/11/26 11:53:51  ambrmi09
 *  Made context switches of stack-pointers more consistent with other stack info.
 *
 *  Revision 1.11  2005/11/26 11:38:40  ambrmi09
 *  Cosmetic changes concerning CVS logs in source.
 *
 *  Revision 1.10  2005/11/25 17:55:29  ambrmi09
 *  Detection of a free-running kernel. Output post-mortem dump, then wait
 *  for real reset.
 *
 *  Revision 1.9  2005/11/24 19:40:12  ambrmi09
 *  Lots of kernel API name changing. To support consistency and to prepare for
 *  the pthreads port.
 *
 *  Revision 1.8  2005/11/23 20:46:43  ambrmi09
 *  Finally stacks seems OK. A bit worried about some "garbage" that turns up
 *  at each TOS at each tasks start
 *
 *  Revision 1.7  2005/11/23 11:31:05  ambrmi09
 *  New stack structure is in place. Now all we have to do should be to just
 *  attach DDP2:R0 to the user_stack part of it, and we "should" be done with
 *  the XC167 stack bug.
 *
 *  Revision 1.6  2005/11/23 07:47:44  ambrmi09
 *  Simple namechange
 *
 *  Revision 1.5  2005/11/22 23:33:53  ambrmi09
 *  New stack_t in place and tested for XC167. We seem to have some include confilict also between regs166.h and main.h (both system includes)
 *
 *  Revision 1.4  2005/11/22 20:07:56  ambrmi09
 *  Separated architecture specific code by introducing tk_hwsys_<TARGET>.h
 *  files.
 *  Prepared for new stack_t type (not introduced yet).
 *
 *  Revision 1.3  2005/11/18 13:18:27  ambrmi09
 *  Finally got the timing right (tested and verifyed). Amazing accurancy!
 *  accurancy from 60000mS to 1mS (executed 10000 ti,es) both show the same
 *  constant error (that they are equal can only be explained due to the
 *  meassuring method). Still need to work some on getting portability between
 *  fast and slow targets right so trimming of kernal will be easy.
 *
 *  Revision 1.2  2005/11/18 11:27:46  ambrmi09
 *  Starting documenting
 *
 *  Revision 1.1.1.1  2005/11/17 09:59:09  ambrmi09
 *  Created CVS jhome for TinKer from scratch. RCS history lost in database (kept in sourcecode)
 *
 *  Revision 1.5  1998/02/16 18:05:12  mickey
 *  Added macro assert.
 *   Added extra wakeup event
 *
 *  Revision 1.4  1998/02/14 10:27:45  mickey
 *  Handling of wakeup event
 *  void tk_exit( unsigned int ) replaces void exit( unsigned int)
 *
 *  Revision 1.3  1998/02/01 20:03:28  mickey
 *  Jiihaa, first version of a working semaphore
 *
 *  Revision 1.2  1998/01/28 21:32:26  mickey
 *  First working revision after tidying, with both Borland and Codewright IDE
 *
 *  Revision 1.1  1998/01/28 20:12:41  mickey
 *  Initial revision
 *
 *  
 *******************************************************************/









