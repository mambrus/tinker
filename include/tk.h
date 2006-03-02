/*******************************************************************
 *
 *  DESCRIPTION: tk.h Headerfile for TinKerOs
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:    
 *
 *  Current $Revision: 1.31 $
 *
 *******************************************************************/
   
/*!
@file
@ingroup SCHED

@brief Tinker inner-most \e"guts"

This comonent is not selectable as the others normally are

For in-depth discussions about this component, see \ref
SCHED

@see COMPONENTS
*/

 
#ifndef TK_H
#define TK_H

/*- include files **/
#include <stddef.h>
#include <time.h>
#include <tk_hwtypes.h>   //should be OK now


enum SCHED_ERROR_CODES{
/*First are TiNker codes*/
/*COMPONENT SCHED*/
TK_OK            ,//!< @brief Operation returned without errors
TK_ERROR         ,//!< @brief General TinKer catch all error
TK_SENTINEL
};

#define ERR_OK TK_OK            //!< TinKer: No error

/*!
@name Kernel trimming constants

@note 
- TK_MAX_THREADS_AT_PRIO should be at least as big as TK_MAX_THREADS
- TK_MAX_THREADS is the big memory hog. It determines the pool size
- More or less TK_MAX_PRIO_LEVELS doesn't make that much difference in
  memory usage, but keep it low anyway. It's doubtful if it makes sense 
  to have a lot of priority levels anyway.

Modify these constants to get a kernel of desired size/speed ratio

@note </b>Do not change these if using pre-built kernel as lib</b>
*/
//@{
#define TK_MAX_THREADS           10    //!< Maximum number of threads kernel can handle
#define TK_MAX_PRIO_LEVELS       0x10  //!< Maximum number of priority levels. @todo fix this, Idle needs to bee last in last prio( needs one extra )
#define TK_MAX_THREADS_AT_PRIO   0x0A  //!< Maximum number of threads at a priority at any one time
#define TK_THREAD_NAME_LEN       0x17  //!< Number of characters (at most) in the TCB identifying the thread. @note If this is zero, which is valid, no human readable info about the thread is stored @note A size of 4 bytes is what used to be standard in pSos
//@}


//Some defines for the logic
#define YES                      1
#define NO                       0

#if      defined(__CYGWIN32__)  || defined(__CYGWIN__)  || \
         defined(__GNUC__)      || defined(__USE_GNU)   || \
         defined(_WIN32)        || defined(__BORLANDC__) || defined(__BCPLUSPLUS__) 

   #define TK_NORMAL_STACK_SIZE     0x1200 //!< @note Whats normal or reasonable differs between architectures. 

#else
   #define TK_NORMAL_STACK_SIZE     0x600 //!< @note Whats normal or reasonable differs between architectures.    
#endif


/*!
@name Build including following components

Modify these constants to include/exclude the following \ref COMPONENTS

Use either YES or NO to either include or omit.

@note </b>Do not change these if using pre-built kernel as lib</b>

You can set the options from command line also:
-DTK_COMP_ITC=1,-DTK_COMP_PTIMER=1,-DTK_COMP_KMEM=1,-DTK_COMP_PTHREAD=1,-DTK_COMP_POSIX_RT=1

MSVC "preprocessor directive" would be:
TK_COMP_ITC=1,TK_COMP_PTIMER=1,TK_COMP_KMEM=1,TK_COMP_PTHREAD=1,TK_COMP_POSIX_RT=1

@note Settings from command line will over-rule the ones in this file
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
@name Kernel termination codes (bit addressable

When TinKer itself dies (i.e. exits), it will do so using a exit code
telling about the reason of the exit.

@note Per \b kernel and \b not per \b thread codes
*/
//@{
#define TK_NOERR           0x0000   //!< Termination without errors

#define TK_ERR_1           0x0001
#define TK_ERR_ASSERT      0x0002   //!< Assertion failed
#define TK_ERR_3           0x0004
#define TK_ERR_4           0x0008

#define TK_ERR_STACK       0x0010   //!< Stack out of bounds check faliure
#define TK_ERR_STKINT      0x0020   //!< Stack integrity faliure detected
#define TK_ERR_7           0x0040
#define TK_ERR_8           0x0080

#define TK_ERR_9           0x0100
#define TK_ERR_10          0x0200
#define TK_ERR_11          0x0400
#define TK_ERR_12          0x0800

#define TK_ERR_13          0x1000
#define TK_ERR_14          0x2000
#define TK_ERR_15          0x4000
#define TK_ERR_16          0x8000
//@}

typedef enum{FALSE,TRUE}BOOL;
typedef void *start_func_ft(void *);
typedef start_func_ft   *start_func_f;

typedef void init_func_ft(void *);
typedef init_func_ft   *init_func_f;


/*- default settings **/

/*- external functions **/

/*- external data **/

/*- internal functions **/
void           _tk_assertfail(    /* emulates __assertfail */
   char *assertstr, 
   char *filestr, 
   int line
);

struct tcb_t  *_tk_current_tcb( void );
struct tcb_t  *_tk_specific_tcb( unsigned int id );
int           *_tk_errno();  
void           _tk_context_switch_to_thread(unsigned int RID,unsigned int SID);
void           _tk_main( void );


/*- public data **/

/*- private data **/
extern int Tk_IntFlagCntr;

/*- public functions **/


int            tk_delete_thread(unsigned int PID);

unsigned int   tk_create_thread(
   char          *name,
   unsigned int   prio,
   start_func_f   start_func,
   void          *inpar,
   size_t         stack_size
);
//Allocate at least 2k stack if you use printf in thread
//else 64bytes is probably sufficent.
void           tk_create_kernel( void );
void           tk_delete_kernel( void );
void           tk_yield( void );
void           tk_exit( int ec );
void           tk_msleep( unsigned int time_ms );
unsigned int   tk_thread_id( void );            

extern void    root( void ); /*! supplied by YOU - constitutes the root thread function*/

/*- private functions **/




#endif

  
/*! 
 * @defgroup CVSLOG_tk_h tk_h
 * @ingroup CVSLOG
 *
 *  $Log: tk.h,v $
 *  Revision 1.31  2006-03-02 14:05:48  ambrmi09
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









