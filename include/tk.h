/*******************************************************************
 *
 *  DESCRIPTION: tk.h Headerfile for TinKerOs
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:    
 *
 *  Current $Revision: 1.18 $
 *
 *******************************************************************/
   

 
#ifndef TK_H
#define TK_H

/** include files **/
#include <stddef.h>
#include <time.h>
#include <tk_hwtypes.h>   //should be OK now

/** local definitions **/
#define TK_MAX_THREADS          10
#define TK_MAX_PRIO_LEVELS      0x10  //fix this, Idle needs to bee last in last prio( needs one extra )
#define TK_MAX_THREADS_AT_PRIO  0x8
#define TK_THREAD_NAME_LEN      0x08

//Some defines for the logic
#define TK_OK              0
#define TK_ERROR           1
#define YES                1
#define NO                 0

//Include components 
#define  IPC               YES

//Kernel termination codes (bit adressable) - Note, per kernel and not per thread codes

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

//assert macro
#ifdef NDEBUG
#  define assert(p)   ((void)0)
#else
#  
#  define assert(p) ((p) ? (void)0 : (void) _tk_assertfail( \
                    #p, __FILE__, __LINE__ ) )              
#endif


typedef enum{FALSE,TRUE}BOOL;
typedef unsigned int funk(void *);
typedef funk *funk_p;

// The following 2 defines the process status and on what it is blocked.
// __T = TERM  = Process is waiting for one or more children to terminate
// _S_ = SLEEP = Process is blocked on timer (sleeping)
// Q__ = QUEUE = Process is blocked on queue or semafor
typedef enum {
   READY=0x0,     _______T=0x1,  ______S_=0x2,  ______ST=0x3,
   _____Q__=0x4,  _____Q_T=0x5,  _____QS_=0x6,  _____QST=0x7,
   ZOMBIE=0X80
}PROCSTATE;

typedef enum{TERM=1,SLEEP=2,QUEUE=4}STATEBITS;
typedef enum{E_CHILDDEATH, E_TIMER, E_IPC, E_IPC2}wakeE_t;

/*!
Thread control block (TCB). This structure contains all
information the kernel needs to know about a thread.

@note the type for various stack pointers. This type is supplied by BSP
adaptions to cover certain architectures special aspects of a "stack".
In a 32bit "normal" CPU this is often a char*, but for some obscure MPU:s
like the C166 family, this is a much more complex structure.
*/

typedef struct    tk_tcb_t_s{
   unsigned int   Pid,Gid;             //!< Process ID and Parent ID (Gid)
   unsigned int   noChilds;            //!< Numb of procs this has created
   char           name[TK_THREAD_NAME_LEN]; //!< Name of the process
   BOOL           isInit;              //!< Memory for stack is allocated
   PROCSTATE      state;               //!< State of the process
   stack_t        stack_begin;         //!< First address of stack memory
   stack_t        curr_sp;             //!< Current stackpointer of this thread
   size_t         stack_size;          //!< Size of stack
   unsigned long  stack_crc;           //!< Control value of integrity check
   clock_t        wakeuptime;          //!< When to wake up if sleeping
   wakeE_t        wakeupEvent;         //!< Helper variable mainly for IPC
   void          *prmtRetAddr;         //!< Preempted return adress - used in preempted mode.
   unsigned int   Prio,Idx;            //!< Helpers, prevent need of lookup
}tk_tcb_t;

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/
void           _tk_assertfail(    /* emulates __assertfail */
   char *assertstr, 
   char *filestr, 
   int line
);

tk_tcb_t      *_tk_current_tcb( void );
tk_tcb_t      *_tk_specific_tcb( unsigned int id );
void           _tk_context_switch_to_thread(unsigned int RID,unsigned int SID);
void           _tk_main( void );


/** public data **/

/** private data **/
extern int Tk_IntFlagCntr;

/** public functions **/


int            tk_delete_thread(unsigned int PID);

unsigned int   tk_create_thread(
   char          *name,
   unsigned int   prio,
   funk_p         start_func,
   void          *inpar,
   size_t         stack_size
);
//Allocate at least 2k stack if you use printf in thread
//else 64bytes is probably sufficent.
void           tk_create_kernel( void );
void           tk_delete_kernel( void );
void           tk_yield( void );
void           tk_exit( int ec );
unsigned int   tk_msleep( unsigned int time_ms );
unsigned int   tk_thread_id( void );            

extern void    root( void ); /*! supplied by YOU - constitutes the root thread function*/

/** private functions **/




#endif

  
/*! 
 * @addtogroup CVSLOG CVSLOG
 *
 *  $Log: tk.h,v $
 *  Revision 1.18  2005-12-03 14:04:30  ambrmi09
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









