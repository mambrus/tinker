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
#ifndef tk_ipc_h
#define tk_ipc_h

#if defined(__GNUC__)
#include <tinker/config.h>
#if (!defined(TK_COMP_ITC) || (TK_COMP_ITC==0) ) && !defined(ECODES_ONLY)
#error "<tk_itc.h> belongs to a component that your build of TinKer didn't include. Please reconfigure and rebuild TinKer."
#endif
#endif

#include <time.h>
#include <tk.h>

#ifdef __doxygen__
#error This section should never be enabled for a normal build
#define TK_MAX_BLOCKED_ON_Q TK_MAX_THREADS
#define TK_MAX_NUM_Q 50

#endif
#define TK_FOREVER 0

enum ITC_ERROR_CODES {

	ERR_TIMEOUT = TK_SENTINEL,
	ERR_OBJDEL,
	ERR_OBJID,
	ERR_OBJTYPE,
	ERR_OBJFULL,
	ERR_BUFSIZ,
	ERR_NOQCB,
	ERR_FULL,
	ERR_NOSCB,
	ERR_NOSEM,
	ERR_TATSDEL,
	ERR_NOMEM,
	ERR_BLOCKLIMIT,
	ERR_ITC_SENTINEL
};

typedef enum {
	WAIT = 0x00,
	LIMIT = 0x00,
	FIFO = 0x00,
	LOCAL = 0x00,
	GLOBAL = 0x01,
	PRIOR = 0x02,
	NOLIMIT = 0x04,
	NOWAIT = 0x08
} ipc_flags_t;
typedef enum {
	SEM,
	S_QUEUE,
	V_QUEUE
} itc_set_t;

#ifndef PSOS_COMP
#define PSOS_COMP
#endif

#ifdef PSOS_COMP
typedef unsigned long q_t[4];
#else
typedef char q_t[4];
#endif

typedef struct {
	void *mb;

	unsigned long s;
} qv_t;

typedef struct itc_t_ {
	char name[4];
	int token;

	unsigned long flags;
	unsigned long sizeof_q;
	unsigned long maxSizeof_m;
	itc_set_t b_type;
	struct tcb_t_ **blocked_procs;
	unsigned long in_idx;
	unsigned long out_idx;
	unsigned long min_idx;
	unsigned long mout_idx;

	union {
		q_t *q;
		qv_t *qv;
	} m;
} itc_t;

unsigned long tk_itc(void);
unsigned long tk_itc_destruct(void);

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif
	unsigned long q_vcreate(char name[4],
				unsigned long flags,
				unsigned long count,
				unsigned long mmlen, unsigned long *qid);

	unsigned long q_vdelete(unsigned long qid);

	unsigned long q_vreceive(unsigned long qid,
				 unsigned long flags,
				 unsigned long timeout,
				 void *msg_buf,
				 unsigned long buf_len, unsigned long *msg_len);

	unsigned long q_vsend(unsigned long qid,
			      void *msg_buf, unsigned long msg_len);

	unsigned long q_create(char name[4],
			       unsigned long count,
			       unsigned long flags, unsigned long *qid);

	unsigned long q_delete(unsigned long qid);

	unsigned long q_receive(unsigned long qid,
				unsigned long flags,
				unsigned long timeout, unsigned long msg_buf[4]
	    );

	unsigned long q_send(unsigned long qid, unsigned long msg_buf[4]
	    );

	unsigned long sm_create(char name[4],
				unsigned long count,
				unsigned long flags, unsigned long *qid);

	unsigned long sm_delete(unsigned long pid);

	unsigned long sm_p(unsigned long qid,
			   unsigned long flags, unsigned long timeout);

	unsigned long sm_v(unsigned long qid);
	unsigned long q_vcreate_ny(char name[4],
				   unsigned long flags,
				   unsigned long count,
				   unsigned long mmlen, unsigned long *qid);

	unsigned long q_vdelete_ny(unsigned long qid);

	unsigned long q_vreceive_ny(unsigned long qid,
				    unsigned long flags,
				    unsigned long timeout,
				    void *msg_buf,
				    unsigned long buf_len,
				    unsigned long *msg_len);

	unsigned long q_vsend_ny(unsigned long qid,
				 void *msg_buf, unsigned long msg_len);

	unsigned long q_create_ny(char name[4],
				  unsigned long count,
				  unsigned long flags, unsigned long *qid);

	unsigned long q_delete_ny(unsigned long qid);

	unsigned long q_receive_ny(unsigned long qid,
				   unsigned long flags,
				   unsigned long timeout,
				   unsigned long msg_buf[4]
	    );

	unsigned long q_send_ny(unsigned long qid, unsigned long msg_buf[4]
	    );

	unsigned long sm_create_ny(char name[4],
				   unsigned long count,
				   unsigned long flags, unsigned long *qid);

	unsigned long sm_delete_ny(unsigned long pid);

	unsigned long sm_p_ny(unsigned long qid,
			      unsigned long flags, unsigned long timeout);

	unsigned long sm_v_ny(unsigned long qid);

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#endif
