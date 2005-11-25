/*******************************************************************
 *
 *  DESCRIPTION: tk.h Headerfile for TinKerOs
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:    
 *
 *  Current $Revision: 1.10 $
 *
 *  $Log: tk.h,v $
 *  Revision 1.10  2005-11-25 17:55:29  ambrmi09
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
 
#ifndef TK_H
#define TK_H

/** include files **/
#include <stddef.h>
#include <time.h>
#include <tk_hwsys.h>   //should be OK now

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
//   char*          stack_begin;         //!< First address of stack memory
//   char*          curr_sp;             //!< Current stackpointer of this thread
   char*          curr_sp;             //!< Current stackpointer of this thread
   size_t         stack_size;          //!< Size of stack
   clock_t        wakeuptime;          //!< When to wake up if sleeping
   wakeE_t        wakeupEvent;         //!< Helper variable mainly for IPC
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

tk_tcb_t      *_tk_current_tcb( void );/* Gives a way to enter my own proc (for TKOS only)*/
void           _tk_context_switch_to_thread(unsigned int RID,unsigned int SID);
void           _tk_main( void );


/** public data **/

/** private data **/

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













