/*******************************************************************
 *
 *  DESCRIPTION: tk_ipc.h Inter Process Comunication TinKerOS.
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:    
 *
 *  Current $Revision: 1.2 $
 *
 *  $Log: tk_itc.h,v $
 *  Revision 1.2  2005-11-24 19:40:12  ambrmi09
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
 
#ifndef tk_ipc_h
#define tk_ipc_h

/** include files **/
#include <time.h>
#include <tk.h>

/** local definitions **/
#define MAX_BLOCKED_ON_Q    20  /*Max num of proces blocked on queue or sem*/
#define MAX_NUM_Q           50  /*Max num of semaphores OR queues*/

/** Error codes **/
#define ERR_OK              0x00 /* No error */
#define ERR_TIMEOUT         0x01 /* IPC has timedout */
#define ERR_OBJDEL          0x05 /* IPC has been deleted */
#define ERR_OBJID           0x06 /* IPC id incorrect ?? */
#define ERR_OBJTYPE         0x07 /* IPC type doesn'e mach object id */
#define ERR_OBJFULL         0x08 /* Nodes objecttable is full */
#define ERR_BUFSIZ          0x32 /* The allocated buffer is smaller than a message could be */
#define ERR_NOQCB           0x33 /* Exceeds max numer of queues */
#define ERR_FULL            0x35 /* The message buffer is full */
#define ERR_NOSCB           0x41 /* Exceeds max numer of semaphores */
#define ERR_NOSEM           0x42 /* Only if NOWAIT was selected */
#define ERR_TATSDEL         0x44 /* There were tasks waiting */

#define ERR_NOMEM           0x100/* No more memory */
#define ERR_BLOCKLIMIT      0x101/* Can't block more procs on queue or semaphore */

typedef enum {
    WAIT            =0x00,
    LIMIT           =0x00,
    FIFO            =0x00,
    GLOBAL          =0x01,      /* 0 = LOCAL adressable*/
    PRIOR           =0x02,      /* 0 = FIFO  order relesing of blocked pid:s*/
    NOLIMIT         =0x04,      /* 0 = numer of messages or tokens ar unlimitted*/  
    NOWAIT          =0x08       /* If no token or message avaiabe calls to sm_p or*/ 
                                /* XX_receive will return with error (equals */
                                /* testing the semaphore)*/
}ipc_flags_t;



typedef enum{SEM, S_QUEUE, V_QUEUE}t_ipctype;
typedef char q_t[4];
typedef struct{
    void *mb;                   /* The variable length message */
    unsigned long s;            /* The size of the message */
}qv_t;
typedef struct {
    char            name[4];            /* Queue name */
    int token;                          /* Messages os tokens. 
                                        /* If = 0, no blocked, no messages */
                                        /* If > 0, token messages */
                                        /* If < 0, [token] blocked */
    unsigned long   flags;              /* Attributes */
    unsigned long   sizeof_q;           /* Size in indexes */
    /*unsigned long     sizeof_m;*/         /* Size of every message (qv) */
    unsigned long   maxSizeof_m;        /* Max size of message (This is stupid ,pSos)*/ 
    t_ipctype       b_type;
    tk_tcb_t**        blocked_procs;      /* blocked_procs[MAX_BLOCKED_ON_Q];*/
    unsigned long   in_idx;             /* Input index for the blocked proc fifo */
    unsigned long   out_idx;            /* Output index for the blocked proc fifo */
    unsigned long   min_idx;            /* Input index for the blocked message fifo */
    unsigned long   mout_idx;           /* Output index for the blocked message fifo */
    union {
       q_t *q;              /* Table with fixed messages  */
       qv_t *qv;            /* Table with messages of variable length */
    }m;                     /* Nothing needed for semaphore           */
}t_ipc;
/* default settings */

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/

/** private functions **/
void createIPC( void );
void deleteIPC( void );

unsigned long q_vcreate(
   char name[4],            /*!< name                                       */
   unsigned long flags,     /*!< attrib                                     */
   unsigned long count,     /*!< queue size                                 */
   unsigned long mmlen,     /*!< max message length                         */
   unsigned long *qid       /*!< id                                         */
);

unsigned long q_vdelete(
    unsigned long qid       /*!< id                                         */
);

unsigned long q_vreceive(
   unsigned long qid,       /*!< id                                         */
   unsigned long flags,     /*!< attrib                                     */
   unsigned long timeout,   /*!< timeout in clock ticks !?vafan             */
   void *msg_buf,           /*!< message buffer                             */
   unsigned long buf_len,   /*!< The size of your allocated buffer          */
   unsigned long *msg_len   /*!< The size of the message                    */
);

unsigned long q_vsend(
   unsigned long qid,       /*!< id                                         */
   void *msg_buf,           /*!< message buffer                             */
   unsigned long msg_len    /*!< The length of this message                 */
);

unsigned long q_create(
   char name[4],            /*!< name                                       */
   unsigned long count,     /*!< queue size                                 */
   unsigned long flags,     /*!< attrib                                     */
   unsigned long *qid       /*!< id                                         */
);

unsigned long q_delete(
    unsigned long qid       /*!< id                                         */
);

unsigned long q_receive(
   unsigned long qid,       /*!< id                                         */
   unsigned long flags,     /*!< attrib                                     */
   unsigned long timeout,   /*!< timeout in clock ticks !?vafan             */
   unsigned long msg_buf[4] /*!< message buffer                             */
);

unsigned long q_send(
   unsigned long qid,       /*!< id                                         */
   unsigned long msg_buf[4] /*!< message buffer                             */
);

unsigned long sm_create(
   char name[4],            /*!< name                                       */
   unsigned long count,     /*!< initial number of tokens                   */
   unsigned long flags,     /*!< attrib                                     */
   unsigned long *qid       /*!< id                                         */
);

unsigned long sm_delete(
   unsigned long pid        /*!< id                                         */
);

unsigned long sm_p(         /*!< sm_receive or sm_get                       */
   unsigned long qid,       /*!< id                                         */
   unsigned long flags,     /*!< attrib                                     */
   unsigned long timeout    /*!< timeout in clock ticks !?vafan             */
);

unsigned long sm_v(         /*!< sm_send or sm_put                          */
   unsigned long qid        /*!< id                                         */
);
#endif
