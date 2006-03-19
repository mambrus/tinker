/***************************************************************************
 *   Copyright (C) 2006 by Michale Ambrus                                  *
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
@ingroup ITC

@brief Native Inter-thread Communication

For in-depth discussions about this component, see \ref
ITC

@see ITC
@see COMPONENTS
*/
  

#ifndef tk_ipc_h
#define tk_ipc_h

/*- include files **/
#include <time.h>
#include <tk.h>

/*- local definitions **/
#define MAX_BLOCKED_ON_Q    TK_MAX_THREADS   /*!< Max num of proces blocked on queue or sem*/
#define MAX_NUM_Q       	50  /*!< Max num of semaphores OR queues (i.e. primitives) in total*/

/*!
@brief Never timeout blocking. I.e. block \e "for-ever"

We can use the value as a special case, because timeout of zero is not
sane in the context of clocking with timeout. That would mean never to
block implicitly, which makes you rises the question of why the call is
invoked in the first place. The benefits of 0 are many, among others, we
avoid nasty truncation errors that would be the result of a very big
value (making the timeout appear as if it had already happened when
actual time us added)

*/ 
#define FOREVER             0

/*- Error codes **/

/*!
COMPONENT ITC (originally pSos error codes). NOTE that the numbers are changed!!
*/
enum ITC_ERROR_CODES{


ERR_TIMEOUT=TK_SENTINEL  ,//!< @brief ITC has timedout
ERR_OBJDEL       ,//!< @brief ITC has been deleted
ERR_OBJID        ,//!< @brief ITC id incorrect ??
ERR_OBJTYPE      ,//!< @brief ITC type doesn'e mach object id
ERR_OBJFULL      ,//!< @brief Nodes objecttable is full
ERR_BUFSIZ       ,//!< @brief The allocated buffer is smaller than a message could be
ERR_NOQCB        ,//!< @brief Exceeds max numer of queues
ERR_FULL         ,//!< @brief The message buffer is full
ERR_NOSCB        ,//!< @brief Exceeds max numer of semaphores
ERR_NOSEM        ,//!< @brief Only if NOWAIT was selected
ERR_TATSDEL      ,//!< @brief There were threads waiting 
ERR_NOMEM        ,//!< @brief No more memory
ERR_BLOCKLIMIT   ,//!< @brief Can't block more procs on queue or semaphore
ERR_ITC_SENTINEL

};




/*!
Bit flags (or-able), used where ever there is a flag argument. 

@note not all flag combinations make sene for all function calls.
*/
typedef enum {
    WAIT            =0x00,      /*!< Default "value" (=0). OR-ing with this renders in no difference of the flags. Name only defined to make intent of usage clearer to read. The "true" bit that \e will affect the flags is \ref NOWAIT*/
    LIMIT           =0x00,      /*!< Default "value" (=0). OR-ing with this renders in no difference of the flags. Name only defined to make intent of usage clearer to read. The "true" bit that \e will affect the flags is \ref NOLIMIT*/
    FIFO            =0x00,      /*!< Default "value" (=0). OR-ing with this renders in no difference of the flags. Name only defined to make intent of usage clearer to read. The "true" bit that \e will affect the flags is \ref PRIOR*/
    LOCAL           =0x00,      /*!< Default "value" (=0). OR-ing with this renders in no difference of the flags. Name only defined to make intent of usage clearer to read. The "true" bit that \e will affect the flags is \ref GLOBAL*/
    GLOBAL          =0x01,      /*!< Adressable by other nodes. The opposite, i.e. \ref LOCAL has the value 0. @note TinKer can't handle this ATM*/
    PRIOR           =0x02,      /*!< Prioritized syncing. I.e. semaphore inherits priority by it's invoking thread. The opposite, \ref FIFO has value 0 and means "fifo"-order relesing of blocked pid:s*/
    NOLIMIT         =0x04,      /*!< Numer of messages or tokens are unlimitted. The opposite, i.e. \ref LIMIT has the value 0. @note TinKer cant handle this ATM*/  
    NOWAIT          =0x08       /*!< If no token or message avaiabe when calling to sm_p or
                                     XX_receive, function will return with error (i.e. equals 
                                     testing the semaphore)*/
}ipc_flags_t;


/*!
@brief Type of synch-primitive

The synch mechanism is common for all types of synchronizations. But
higher levels need to know what additional data is attached and how to
handle them. This will define what kind of primitive is in question.
*/
typedef enum{
   SEM,       //!< The primitive is a semaphore (i.e. a counting mutex)
   S_QUEUE,   //!< The primitive is a normal queue (fix-length data)
   V_QUEUE    //!< The primitive is a variable data-length queue (data is of variable length)
}t_ipctype;

/*!
@brief Content of fixed-length queues. 

Use fixed length queues to send scalars (i.e. integers, enums e.t.a.).

@note that the size is limited and should be equal to the largest scalar
your system will support. 
*/
#if 1       /*Strict pSos*/
   typedef unsigned long  q_t[4];
#else
   typedef char q_t[4];
#endif


/*!
@brief Block to to handle messages of vaiable length

Block to to handle messages of vaiable length
*/
typedef struct{
    void *mb;                   /*!< @brief The variable length message, 
                                     i.e. the actual data.*/
    unsigned long s;            /*!< @brief The size of the message */
}qv_t;

/*!
@brief Control block for the primitive synchronization mechanism

Control block for the primitive synchronization mechanism
*/
typedef struct {
    char            name[4];            /*!< @brief Queue name */
    int token;                          /*!< @brief Messages os tokens. 
                                             If = 0, no blocked, no messages   
                                             If > 0, token messages   
                                             If < 0, [token] blocked */
    unsigned long   flags;              /*!< @brief Attributes */
    unsigned long   sizeof_q;           /*!< @brief Size in indexes */
    unsigned long   maxSizeof_m;        /*!< @brief Max size of message (This is stupid ,pSos)*/ 
    t_ipctype       b_type;             /*!< @brief Determines what kind of primitive this block handles.*/ 
    struct tcb_t**  blocked_procs;      /*!< @brief List of blocked_procs[MAX_BLOCKED_ON_Q];*/
    unsigned long   in_idx;             /*!< @brief Input index for the blocked proc fifo */
    unsigned long   out_idx;            /*!< @brief Output index for the blocked proc fifo */
    unsigned long   min_idx;            /*!< @brief Input index for the blocked message fifo */
    unsigned long   mout_idx;           /*!< @brief Output index for the blocked message fifo */

    /*!
    Data of the sync primitive. @note Nothing needed for semaphore (i.e. NULL)
    */
    union {
       q_t *q;              /*!< @brief Data with fixed-lengt messages  */
       qv_t *qv;            /*!< @brief Data with fixed-lengt with messages of variable length */
    }m;                     
}t_ipc;
/* default settings */

/*- external functions **/

/*- external data **/

/*- internal functions **/

/*- public data **/

/*- private data **/

/*- public functions **/

/*- private functions **/

/*!
@name Creation and destruction of this component

Use these functions only at boot, and shut-down.

@see ITC
*/
//@{
unsigned long  tk_itc( void );
unsigned long  tk_itc_destruct( void );
//@}


/*
#if defined (PREEMPTABLE)

#define q_vcreate    q_vcreate_ny
#define q_vdelete    q_vdelete_ny
#define q_vreceive   q_vreceive_ny
#define q_vsend      q_vsend_ny
#define q_create     q_create_ny
#define q_delete     q_delete_ny
#define q_receive    q_receive_ny
#define q_send       q_send_ny
#define sm_create    sm_create_ny
#define sm_delete    sm_delete_ny
#define sm_p         sm_p_ny
#define sm_v         sm_v_ny
#define q_vcreate    q_vcreate_ny
#define q_vdelete    q_vdelete_ny
#define q_vreceive   q_vreceive_ny
#define q_vsend      q_vsend_ny
#define q_create     q_create_ny
#define q_delete     q_delete_ny
#define q_receive    q_receive_ny
#define q_send       q_send_ny
#define sm_create    sm_create_ny
#define sm_delete    sm_delete_ny
#define sm_p         sm_p_ny
#define sm_v         sm_v_ny

#endif
*/



/*!
@name Normal versions of the \ref ITC API

Normal version of the \ref ITC API

@see ITC
*/
//@{
unsigned long q_vcreate(
   char name[4],            /*!< <b> name                               </b> */
   unsigned long flags,     /*!< <b> attrib                             </b> */
   unsigned long count,     /*!< <b> queue size                         </b> */
   unsigned long mmlen,     /*!< <b> max message length                 </b> */
   unsigned long *qid       /*!< <b> id                                 </b> */
);

unsigned long q_vdelete(
    unsigned long qid       /*!< <b> id                                 </b> */
);

unsigned long q_vreceive(
   unsigned long qid,       /*!< <b> id                                 </b> */
   unsigned long flags,     /*!< <b> attrib                             </b> */
   unsigned long timeout,   /*!< <b> timeout in clock ticks !?vafan     </b> */
   void *msg_buf,           /*!< <b> message buffer                     </b> */
   unsigned long buf_len,   /*!< <b> The size of your allocated buffer  </b> */
   unsigned long *msg_len   /*!< <b> The size of the message            </b> */
);

unsigned long q_vsend(
   unsigned long qid,       /*!< <b> id                                 </b> */
   void *msg_buf,           /*!< <b> message buffer                     </b> */
   unsigned long msg_len    /*!< <b> The length of this message         </b> */
);

unsigned long q_create(
   char name[4],            /*!< <b> name                               </b> */
   unsigned long count,     /*!< <b> queue size                         </b> */
   unsigned long flags,     /*!< <b> attrib                             </b> */
   unsigned long *qid       /*!< <b> id                                 </b> */
);

unsigned long q_delete(
    unsigned long qid       /*!< <b> id                                 </b> */
);

unsigned long q_receive(
   unsigned long qid,       /*!< <b> id                                 </b> */
   unsigned long flags,     /*!< <b> attrib                             </b> */
   unsigned long timeout,   /*!< <b> timeout in clock ticks !?vafan     </b> */
   unsigned long msg_buf[4] /*!< <b> message buffer                     </b> */
);

unsigned long q_send(
   unsigned long qid,       /*!< <b> id                                 </b> */
   unsigned long msg_buf[4] /*!< <b> message buffer                     </b> */
);

unsigned long sm_create(
   char name[4],            /*!< <b> name                               </b> */
   unsigned long count,     /*!< <b> initial number of tokens           </b> */
   unsigned long flags,     /*!< <b> attrib                             </b> */
   unsigned long *qid       /*!< <b> id                                 </b> */
);

unsigned long sm_delete(
   unsigned long pid        /*!< <b> id                                 </b> */
);

unsigned long sm_p(         /*!< <b> sm_receive or sm_get               </b> */
   unsigned long qid,       /*!< <b> id                                 </b> */
   unsigned long flags,     /*!< <b> attrib                             </b> */
   unsigned long timeout    /*!< <b> timeout in clock ticks !?vafan     </b> */
);

unsigned long sm_v(         /*!< <b> sm_send or sm_put                  </b> */
   unsigned long qid        /*!< <b> id                                 </b> */
);
//@}



/*!
@name Non-yield versions of the \ref ITC API

Non-yield versions of the same API - to be used by ISRs or in PREEPTABLE
versions of kernel

@see ITC
*/
//@{
unsigned long q_vcreate_ny(
   char name[4],            /*!< <b> name                               </b> */
   unsigned long flags,     /*!< <b> attrib                             </b> */
   unsigned long count,     /*!< <b> queue size                         </b> */
   unsigned long mmlen,     /*!< <b> max message length                 </b> */
   unsigned long *qid       /*!< <b> id                                 </b> */
);

unsigned long q_vdelete_ny(
    unsigned long qid       /*!< <b> id                                 </b> */
);

unsigned long q_vreceive_ny(
   unsigned long qid,       /*!< <b> id                                 </b> */
   unsigned long flags,     /*!< <b> attrib                             </b> */
   unsigned long timeout,   /*!< <b> timeout in clock ticks !?vafan     </b> */
   void *msg_buf,           /*!< <b> message buffer                     </b> */
   unsigned long buf_len,   /*!< <b> The size of your allocated buffer  </b> */
   unsigned long *msg_len   /*!< <b> The size of the message            </b> */
);

unsigned long q_vsend_ny(
   unsigned long qid,       /*!< <b> id                                 </b> */
   void *msg_buf,           /*!< <b> message buffer                     </b> */
   unsigned long msg_len    /*!< <b> The length of this message         </b> */
);

unsigned long q_create_ny(
   char name[4],            /*!< <b> name                               </b> */
   unsigned long count,     /*!< <b> queue size                         </b> */
   unsigned long flags,     /*!< <b> attrib                             </b> */
   unsigned long *qid       /*!< <b> id                                 </b> */
);

unsigned long q_delete_ny(
    unsigned long qid       /*!< <b> id                                 </b> */
);

unsigned long q_receive_ny(
   unsigned long qid,       /*!< <b> id                                 </b> */
   unsigned long flags,     /*!< <b> attrib                             </b> */
   unsigned long timeout,   /*!< <b> timeout in clock ticks !?vafan     </b> */
   unsigned long msg_buf[4] /*!< <b> message buffer                     </b> */
);

unsigned long q_send_ny(
   unsigned long qid,       /*!< <b> id                                 </b> */
   unsigned long msg_buf[4] /*!< <b> message buffer                     </b> */
);

unsigned long sm_create_ny(
   char name[4],            /*!< <b> name                               </b> */
   unsigned long count,     /*!< <b> initial number of tokens           </b> */
   unsigned long flags,     /*!< <b> attrib                             </b> */
   unsigned long *qid       /*!< <b> id                                 </b> */
);

unsigned long sm_delete_ny(
   unsigned long pid        /*!< <b> id                                 </b> */
);

unsigned long sm_p_ny(         
   unsigned long qid,       /*!< <b> id                                 </b> */
   unsigned long flags,     /*!< <b> attrib                             </b> */
   unsigned long timeout    /*!< <b> timeout in clock ticks !?vafan     </b> */
);

unsigned long sm_v_ny(         
   unsigned long qid        /*!< <b> id                                 </b> */
);
//@}

#endif

  
/*!
 * @defgroup CVSLOG_tk_ipc_h tk_ipc_h
 * @ingroup CVSLOG
 *
 *  $Log: tk_itc.h,v $
 *  Revision 1.20  2006-03-19 12:44:36  ambrmi09
 *  Got rid of many compilation warnings. MSVC amd GCC actually gompiles
 *  without one single warning (yay!). Be aware that ther was a lot of
 *  comparisons between signed/unsigned in ITC. Fetts a bit shaky...
 *
 *  Revision 1.19  2006/03/17 12:20:03  ambrmi09
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
 *  Revision 1.17  2006/03/07 08:24:13  ambrmi09
 *  A very crude port for ARM is running (LPC2129) - @note THIS IS HIGHLY EXPERIMENTAL CODE
 *
 *  Revision 1.16  2006/03/05 11:11:25  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.15  2006/03/02 14:05:48  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.14  2006/02/23 15:33:33  ambrmi09
 *  Found a nasty "bug", that was not a read bug after all. At least not in the kernel as a feared. It turned out that I forgot some of the details about how timeouts were to be handled (especially in \ref ITC ). A timeout of value \b zero is equal of never to timeout (read more about it in define \ref FOREVER). However two important lesson learned: Even simple add operations get "funny" when adding large numbers (see line 303 in tk_ipc.c - in the \ref lock_stage function). Anyway. FOREVER should equal zero. (This issue makes me wonder sometimes how sane it really was to resurrect a project that has been dormant for nearly 10 years.) The CodeWright project ruler should be positioned on the actual line btw. This check-in will be accompanied  by a <tt>cvs tag</tt> for this reason, and for yet another nasty bug that seems to be a real dispatcher bug. The current source-set-up will show the bug within one mint (which is good since it makes it a little bit less of a search for the <I>"needle in the haystack</i>").
 *
 *  Revision 1.13  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.12  2006/02/20 19:17:14  ambrmi09
 *  - Made the errno variable thread specific (each thread has it's own)
 *  - Hid the details of using errno so that setting and reading it looks
 *    like using a normal variable
 *  - Extracted some stuff from tk.h that doesn't need to be public
 *  - Implemented perros and strerror including a storage with all the error
 *    strings (will go into NV ROM on a embedded system).
 *
 *  Revision 1.11  2006/02/19 22:00:38  ambrmi09
 *  Major brake-through!!! First working attempt with crude pThreads and
 *  POSIX RT queues works. (jihaa) :=D. Wow
 *
 *  Revision 1.10  2006/02/19 12:44:33  ambrmi09
 *  - Documented ITC
 *  - Started to build up the structure for the \ref PTHREAD component
 *
 *  Revision 1.9  2006/02/17 21:17:26  ambrmi09
 *  More doc structure - this time for the new PTHREAD's components
 *
 *  Revision 1.8  2006/02/16 15:11:00  ambrmi09
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
 *  Revision 1.7  2006/02/02 15:51:02  ambrmi09
 *  A lot of thought has been invested into the new PTIME component. Had to
 *  change things even in the systime parts (integrated in the SHEDUL
 *  component) to make it more generic. Think this will be really nice when
 *  it's ready, but has been a long road to get PTIME running (and I'm
 *  still not there).
 *
 *  Revision 1.6  2005/12/03 14:04:30  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *  Revision 1.5  2005/11/26 11:38:40  ambrmi09
 *  Cosmetic changes concerning CVS logs in source.
 *
 *  Revision 1.4  2005/11/25 17:55:29  ambrmi09
 *  Detection of a free-running kernel. Output post-mortem dump, then wait
 *  for real reset.
 *
 *  Revision 1.3  2005/11/25 14:35:16  ambrmi09
 *  A first naive aproach of ISR to thread syncronisation mechanism. It works
 *  but it is unstable.
 *
 *  Revision 1.2  2005/11/24 19:40:12  ambrmi09
 *  Lots of kernel API name changing. To support consistency and to prepare for
 *  the pthreads port.
 *
 *  Revision 1.1.1.1  2005/11/17 09:59:10  ambrmi09
 *  Created CVS jhome for TinKer from scratch. RCS history lost in database (kept in sourcecode)
 *
 *  Revision 1.4  1998/02/14 10:32:30  mickey
 *  Minor changes
 *  Handling of tokentest in semaphores added (NOWAIT)
 *
 *  Revision 1.3  1998/02/04 20:49:19  mickey
 *  FIrst atteptt to solve priority semaphores (has errors)
 *
 *  Revision 1.2  1998/02/01 20:03:28  mickey
 *  Jiihaa, first version of a working semaphore
 *
 *  Revision 1.1  1998/01/28 21:34:37  mickey
 *  Initial revision
 *
 *******************************************************************/
