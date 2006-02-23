/*******************************************************************
 *
 *  DESCRIPTION: tk_ipc.h Inter Process Comunication TinKerOS.
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:    
 *
 *  Current $Revision: 1.14 $
 *
 *******************************************************************/
   
  
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
#define MAX_BLOCKED_ON_Q    20   /*!< Max num of proces blocked on queue or sem*/
#define MAX_NUM_Q           50   /*!< Max num of semaphores OR queues (i.e. primitives) in total*/

/*!<
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

/* #define ERR_OK              0x00 //!< No error #define ERR_TIMEOUT
0x01 //!< ITC has timedout
#define ERR_OBJDEL          0x05 //!< ITC has been deleted
#define ERR_OBJID           0x06 //!< ITC id incorrect ??
#define ERR_OBJTYPE         0x07 //!< ITC type doesn'e mach object id
#define ERR_OBJFULL         0x08 //!< Nodes objecttable is full
#define ERR_BUFSIZ          0x32 //!< The allocated buffer is smaller than a message could be
#define ERR_NOQCB           0x33 //!< Exceeds max numer of queues
#define ERR_FULL            0x35 //!< The message buffer is full
#define ERR_NOSCB           0x41 //!< Exceeds max numer of semaphores
#define ERR_NOSEM           0x42 //!< Only if NOWAIT was selected
#define ERR_TATSDEL         0x44 //!< There were threads waiting 

#define ERR_NOMEM           0x100//!<  No more memory
#define ERR_BLOCKLIMIT      0x101//!<  Can't block more procs on queue or semaphore
*/

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
typedef char q_t[4];

/*
@bried Block to to handle messages of vaiable length
*/
typedef struct{
    void *mb;                   /*!< @brief The variable length message, 
                                     i.e. the actual data.*/
    unsigned long s;            /*!< @brief The size of the message */
}qv_t;

/*
@bried Control block for the primitive synchronization mechanism
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
@name Normal versions of the \ITC API

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
@name Non-yield versions of the \ITC API

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
 *  Revision 1.14  2006-02-23 15:33:33  ambrmi09
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
