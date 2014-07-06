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
@ingroup ITC

@brief Native Inter-thread Communication

For in-depth discussions about this component, see \ref
ITC

@see ITC
@see COMPONENTS
*/

#ifndef tk_ipc_h
#define tk_ipc_h

#if defined(__GNUC__)
#include <tinker/config.h>
#if (!defined(TK_COMP_ITC) || (TK_COMP_ITC==0) ) && !defined(ECODES_ONLY)
#error "<tk_itc.h> belongs to a component that your build of TinKer didn't include. Please reconfigure and rebuild TinKer."
#endif
#endif

/*- include files **/
#include <time.h>
#include <tk.h>

/*- local definitions **/

#ifdef __doxygen__
#error This section should never be enabled for a normal build
/*!
@name ITC trimming constants

@note
Only information about these defines are here. The true defines are
in each targets corresponding tk_tuning.h.

@warning </b>Do not change these if using pre-built kernel as lib</b>
*/
//@{
#define TK_MAX_BLOCKED_ON_Q    TK_MAX_THREADS	/*!< Max num of proces blocked on queue or sem */
#define TK_MAX_NUM_Q       	50	/*!< Max num of semaphores OR queues (i.e. primitives) in total */
//@}
#endif

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
#define TK_FOREVER             0

/*- Error codes **/

/*!
COMPONENT ITC (originally pSos error codes). NOTE that the numbers are changed!!
*/
enum ITC_ERROR_CODES {

	ERR_TIMEOUT = TK_SENTINEL,	//!< @brief ITC has timedout
	ERR_OBJDEL,		//!< @brief ITC has been deleted
	ERR_OBJID,		//!< @brief ITC id incorrect ??
	ERR_OBJTYPE,		//!< @brief ITC type doesn'e mach object id
	ERR_OBJFULL,		//!< @brief Nodes objecttable is full
	ERR_BUFSIZ,		//!< @brief The allocated buffer is smaller than a message could be
	ERR_NOQCB,		//!< @brief Exceeds max numer of queues
	ERR_FULL,		//!< @brief The message buffer is full
	ERR_NOSCB,		//!< @brief Exceeds max numer of semaphores
	ERR_NOSEM,		//!< @brief Only if NOWAIT was selected
	ERR_TATSDEL,		//!< @brief There were threads waiting
	ERR_NOMEM,		//!< @brief No more memory
	ERR_BLOCKLIMIT,		//!< @brief Can't block more procs on queue or semaphore
	ERR_ITC_SENTINEL
};

/*!
Bit flags (or-able), used where ever there is a flag argument.

@note not all flag combinations make sene for all function calls.
*/
typedef enum {
	WAIT = 0x00,		/*!< Default "value" (=0). OR-ing with this renders in no difference of the flags. Name only defined to make intent of usage clearer to read. The "true" bit that \e will affect the flags is \ref NOWAIT */
	LIMIT = 0x00,		/*!< Default "value" (=0). OR-ing with this renders in no difference of the flags. Name only defined to make intent of usage clearer to read. The "true" bit that \e will affect the flags is \ref NOLIMIT */
	FIFO = 0x00,		/*!< Default "value" (=0). OR-ing with this renders in no difference of the flags. Name only defined to make intent of usage clearer to read. The "true" bit that \e will affect the flags is \ref PRIOR */
	LOCAL = 0x00,		/*!< Default "value" (=0). OR-ing with this renders in no difference of the flags. Name only defined to make intent of usage clearer to read. The "true" bit that \e will affect the flags is \ref GLOBAL */
	GLOBAL = 0x01,		/*!< Adressable by other nodes. The opposite, i.e. \ref LOCAL has the value 0. @note TinKer can't handle this ATM */
	PRIOR = 0x02,		/*!< Prioritized syncing. I.e. semaphore inherits priority by it's invoking thread. The opposite, \ref FIFO has value 0 and means "fifo"-order relesing of blocked pid:s */
	NOLIMIT = 0x04,		/*!< Numer of messages or tokens are unlimitted. The opposite, i.e. \ref LIMIT has the value 0. @note TinKer cant handle this ATM */
	NOWAIT = 0x08		/*!< If no token or message avaiabe when calling to sm_p or
				   XX_receive, function will return with error (i.e. equals
				   testing the semaphore) */
} ipc_flags_t;

/*!
@brief Type of synch-primitive

The synch mechanism is common for all types of synchronizations. But
higher levels need to know what additional data is attached and how to
handle them. This will define what kind of primitive is in question.
*/
typedef enum {
	SEM,			//!< The primitive is a semaphore (i.e. a counting mutex)
	S_QUEUE,		//!< The primitive is a normal queue (fix-length data)
	V_QUEUE			//!< The primitive is a variable data-length queue (data is of variable length)
} itc_set_t;

/*!
@brief Content of fixed-length queues.

Use fixed length queues to send scalars (i.e. integers, enums e.t.a.).

@note that the size is limited and should be equal to the largest scalar
your system will support.
*/
#if 1				/*Strict pSos */
typedef unsigned long q_t[4];
#else
typedef char q_t[4];
#endif

/*!
@brief Block to to handle messages of vaiable length

Block to to handle messages of vaiable length
*/
typedef struct {
	void *mb;		/*!< @brief The variable length message,
				   i.e. the actual data. */
	unsigned long s;	/*!< @brief The size of the message */
} qv_t;

/*!
@brief Control block for the primitive synchronization mechanism

Control block for the primitive synchronization mechanism
*/
typedef struct itc_t_ {
	char name[4];		/*!< @brief Queue name */
	int token;		/*!< @brief Messages os tokens.
				   If = 0, no blocked, no messages
				   If > 0, token messages
				   If < 0, [token] blocked */
	unsigned long flags;	/*!< @brief Attributes */
	unsigned long sizeof_q;	/*!< @brief Size in indexes */
	unsigned long maxSizeof_m;	/*!< @brief Max size of message (This is stupid ,pSos) */
	itc_set_t b_type;	/*!< @brief Determines what kind of primitive this block handles. */
	struct tcb_t_ **blocked_procs;	/*!< @brief List of blocked_procs[TK_MAX_BLOCKED_ON_Q]; */
	unsigned long in_idx;	/*!< @brief Input index for the blocked proc fifo */
	unsigned long out_idx;	/*!< @brief Output index for the blocked proc fifo */
	unsigned long min_idx;	/*!< @brief Input index for the blocked message fifo */
	unsigned long mout_idx;	/*!< @brief Output index for the blocked message fifo */

	/*!
	   Data of the sync primitive. @note Nothing needed for semaphore (i.e. NULL)
	 */
	union {
		q_t *q;		/*!< @brief Data with fixed-lengt messages  */
		qv_t *qv;	/*!< @brief Data with fixed-lengt with messages of variable length */
	} m;
} itc_t;
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
unsigned long tk_itc(void);
unsigned long tk_itc_destruct(void);
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

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

/*!
@name Normal versions of the \ref ITC API

Normal version of the \ref ITC API

@see ITC
*/
//@{
	unsigned long q_vcreate(char name[4],	/*!< <b> name                               </b> */
				unsigned long flags,	/*!< <b> attrib                             </b> */
				unsigned long count,	/*!< <b> queue size                         </b> */
				unsigned long mmlen,	/*!< <b> max message length                 </b> */
				unsigned long *qid	/*!< <b> id                                 </b> */
	    );

	unsigned long q_vdelete(unsigned long qid	/*!< <b> id                                 </b> */
	    );

	unsigned long q_vreceive(unsigned long qid,	/*!< <b> id                                 </b> */
				 unsigned long flags,	/*!< <b> attrib                             </b> */
				 unsigned long timeout,	/*!< <b> timeout in clock ticks !?vafan     </b> */
				 void *msg_buf,	/*!< <b> message buffer                     </b> */
				 unsigned long buf_len,	/*!< <b> The size of your allocated buffer  </b> */
				 unsigned long *msg_len	/*!< <b> The size of the message            </b> */
	    );

	unsigned long q_vsend(unsigned long qid,	/*!< <b> id                                 </b> */
			      void *msg_buf,	/*!< <b> message buffer                     </b> */
			      unsigned long msg_len	/*!< <b> The length of this message         </b> */
	    );

	unsigned long q_create(char name[4],	/*!< <b> name                               </b> */
			       unsigned long count,	/*!< <b> queue size                         </b> */
			       unsigned long flags,	/*!< <b> attrib                             </b> */
			       unsigned long *qid	/*!< <b> id                                 </b> */
	    );

	unsigned long q_delete(unsigned long qid	/*!< <b> id                                 </b> */
	    );

	unsigned long q_receive(unsigned long qid,	/*!< <b> id                                 </b> */
				unsigned long flags,	/*!< <b> attrib                             </b> */
				unsigned long timeout,	/*!< <b> timeout in clock ticks !?vafan     </b> */
				unsigned long msg_buf[4]	/*!< <b> message buffer                     </b> */
	    );

	unsigned long q_send(unsigned long qid,	/*!< <b> id                                 </b> */
			     unsigned long msg_buf[4]	/*!< <b> message buffer                     </b> */
	    );

	unsigned long sm_create(char name[4],	/*!< <b> name                               </b> */
				unsigned long count,	/*!< <b> initial number of tokens           </b> */
				unsigned long flags,	/*!< <b> attrib                             </b> */
				unsigned long *qid	/*!< <b> id                                 </b> */
	    );

	unsigned long sm_delete(unsigned long pid	/*!< <b> id                                 </b> */
	    );

	unsigned long sm_p(	/*!< <b> sm_receive or sm_get               </b> */
				  unsigned long qid,	/*!< <b> id                                 </b> */
				  unsigned long flags,	/*!< <b> attrib                             </b> */
				  unsigned long timeout	/*!< <b> timeout in clock ticks !?vafan     </b> */
	    );

	unsigned long sm_v(	/*!< <b> sm_send or sm_put                  </b> */
				  unsigned long qid	/*!< <b> id                                 </b> */
	    );
//@}

/*!
@name Non-yield versions of the \ref ITC API

Non-yield versions of the same API - to be used by ISRs or in PREEPTABLE
versions of kernel

@see ITC
*/
//@{
	unsigned long q_vcreate_ny(char name[4],	/*!< <b> name                               </b> */
				   unsigned long flags,	/*!< <b> attrib                             </b> */
				   unsigned long count,	/*!< <b> queue size                         </b> */
				   unsigned long mmlen,	/*!< <b> max message length                 </b> */
				   unsigned long *qid	/*!< <b> id                                 </b> */
	    );

	unsigned long q_vdelete_ny(unsigned long qid	/*!< <b> id                                 </b> */
	    );

	unsigned long q_vreceive_ny(unsigned long qid,	/*!< <b> id                                 </b> */
				    unsigned long flags,	/*!< <b> attrib                             </b> */
				    unsigned long timeout,	/*!< <b> timeout in clock ticks !?vafan     </b> */
				    void *msg_buf,	/*!< <b> message buffer                     </b> */
				    unsigned long buf_len,	/*!< <b> The size of your allocated buffer  </b> */
				    unsigned long *msg_len	/*!< <b> The size of the message            </b> */
	    );

	unsigned long q_vsend_ny(unsigned long qid,	/*!< <b> id                                 </b> */
				 void *msg_buf,	/*!< <b> message buffer                     </b> */
				 unsigned long msg_len	/*!< <b> The length of this message         </b> */
	    );

	unsigned long q_create_ny(char name[4],	/*!< <b> name                               </b> */
				  unsigned long count,	/*!< <b> queue size                         </b> */
				  unsigned long flags,	/*!< <b> attrib                             </b> */
				  unsigned long *qid	/*!< <b> id                                 </b> */
	    );

	unsigned long q_delete_ny(unsigned long qid	/*!< <b> id                                 </b> */
	    );

	unsigned long q_receive_ny(unsigned long qid,	/*!< <b> id                                 </b> */
				   unsigned long flags,	/*!< <b> attrib                             </b> */
				   unsigned long timeout,	/*!< <b> timeout in clock ticks !?vafan     </b> */
				   unsigned long msg_buf[4]	/*!< <b> message buffer                     </b> */
	    );

	unsigned long q_send_ny(unsigned long qid,	/*!< <b> id                                 </b> */
				unsigned long msg_buf[4]	/*!< <b> message buffer                     </b> */
	    );

	unsigned long sm_create_ny(char name[4],	/*!< <b> name                               </b> */
				   unsigned long count,	/*!< <b> initial number of tokens           </b> */
				   unsigned long flags,	/*!< <b> attrib                             </b> */
				   unsigned long *qid	/*!< <b> id                                 </b> */
	    );

	unsigned long sm_delete_ny(unsigned long pid	/*!< <b> id                                 </b> */
	    );

	unsigned long sm_p_ny(unsigned long qid,	/*!< <b> id                                 </b> */
			      unsigned long flags,	/*!< <b> attrib                             </b> */
			      unsigned long timeout	/*!< <b> timeout in clock ticks !?vafan     </b> */
	    );

	unsigned long sm_v_ny(unsigned long qid	/*!< <b> id                                 </b> */
	    );
//@}

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#endif
