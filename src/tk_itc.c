/***************************************************************************
 *   Copyright (C) 1998        Michael Ambrus                              *
 *   Copyright (C) 2005,2006   Michael Ambrus                              *
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
#include <implement_itc.h>
#ifdef TK_COMP_ITC
static unsigned long lock_stage(unsigned long qid, unsigned long timeout);
static unsigned long unlock_stage(unsigned long qid);

extern itc_t *__itc_array[TK_MAX_NUM_Q];
extern int __itc_idx;

static unsigned long lock_stage(unsigned long qid, unsigned long timeout)
{
	tk_tcb_t *MySelf;
	clock_t act_time;
	clock_t timeout_time;
	unsigned int mark_idx;
	TK_CLI();

	act_time = clock() / CLK_TCK * 1000;
	MySelf = _tk_current_tcb();

	if (__itc_array[qid]->token > 0) {

		__itc_array[qid]->token--;
		STI_return(ERR_OK);
	} else {

		if (abs(__itc_array[qid]->token) < (TK_MAX_BLOCKED_ON_Q - 1)) {
			__itc_array[qid]->token--;
		} else
			STI_return(ERR_BLOCKLIMIT);

		__itc_array[qid]->blocked_procs[__itc_array[qid]->in_idx] =
		    MySelf;
		__itc_array[qid]->in_idx++;
		__itc_array[qid]->in_idx %= TK_MAX_BLOCKED_ON_Q;

		if (timeout == 0) {

			MySelf->state =
			    (PROCSTATE) ((MySelf->state) | _____Q__);
			MySelf->wakeupEvent = E_NONE;
			tk_yield_CLI();

			STI_return(ERR_OK);
		} else {
			MySelf->state =
			    (PROCSTATE) ((MySelf->state) | _____QS_);
			timeout_time = act_time + timeout;
			MySelf->wakeuptime = timeout_time;

			mark_idx = __itc_array[qid]->in_idx;
			tk_yield_CLI();

			if (MySelf->wakeupEvent == E_TIMER) {

				mark_idx--;
				if (mark_idx > TK_MAX_BLOCKED_ON_Q) {
					mark_idx = TK_MAX_BLOCKED_ON_Q - 1;
				}

				if (__itc_array[qid]->token >= 0)
					STI_return(ERR_TIMEOUT);

				_itc_removeBlocked(__itc_array[qid], mark_idx);

				if (__itc_array[qid]->token >= 0)
					STI_return(ERR_TIMEOUT);

				mark_idx = __itc_array[qid]->out_idx;
				do {

					if (__itc_array[qid]->blocked_procs
					    [mark_idx]->state == READY
					    &&
					    __itc_array[qid]->blocked_procs
					    [mark_idx]->wakeupEvent ==
					    E_TIMER) {

						__itc_array[qid]->blocked_procs
						    [mark_idx]->wakeupEvent =
						    E_ITC2;

						_itc_removeBlocked(__itc_array
								   [qid],
								   mark_idx);
					}
					if (mark_idx ==
					    __itc_array[qid]->in_idx)
						break;
					mark_idx++;
					mark_idx %= TK_MAX_BLOCKED_ON_Q;
				}
				while (__itc_array[qid]->token < 0
				       && mark_idx != __itc_array[qid]->in_idx);
				STI_return(ERR_TIMEOUT);

			} else if (MySelf->wakeupEvent == E_ITC2)
				STI_return(ERR_TIMEOUT);
			STI_return(ERR_OK);
		}
	}
	TK_STI();
}

static unsigned long unlock_stage(unsigned long qid)
{
	tk_tcb_t *Him;
	unsigned int i;
	unsigned int t_idx, t_prio = TK_MAX_PRIO_LEVELS + 2;
	TK_CLI();

	if (__itc_array[qid]->token < 0) {
		if (__itc_array[qid]->flags & PRIOR) {

			for (i = __itc_array[qid]->out_idx;
			     i != __itc_array[qid]->in_idx;
			     i++, i %= TK_MAX_BLOCKED_ON_Q) {
				if (!
				    (__itc_array[qid]->blocked_procs[i]->
				     state & _____Q__)) {

				}
				if (__itc_array[qid]->in_idx == i)
					break;
			}

			if ((__itc_array[qid]->in_idx -
			     __itc_array[qid]->out_idx) == 0) {

				STI_return(ERR_OK);

			}

			for (i = __itc_array[qid]->out_idx;
			     i != __itc_array[qid]->in_idx;
			     i++, i %= TK_MAX_BLOCKED_ON_Q) {
				if (__itc_array[qid]->blocked_procs[i]->Prio <
				    t_prio) {
					t_prio =
					    __itc_array[qid]->blocked_procs[i]->
					    Prio;
					t_idx = i;
				}
			}

			assert(t_prio != (TK_MAX_PRIO_LEVELS + 2));
			__itc_array[qid]->blocked_procs[t_idx]->state =
			    (PROCSTATE) (__itc_array[qid]->blocked_procs
					 [t_idx]->state & ~_____QST);
			__itc_array[qid]->blocked_procs[t_idx]->wakeupEvent =
			    E_ITC;

			_itc_removeBlocked(__itc_array[qid], t_idx);
			tk_yield_CLI();

		} else {
			do {
				Him =
				    __itc_array[qid]->blocked_procs[__itc_array
								    [qid]->out_idx];
				__itc_array[qid]->out_idx++,
				    __itc_array[qid]->out_idx %=
				    TK_MAX_BLOCKED_ON_Q;
				__itc_array[qid]->token++;
			} while (!(Him->state & _____Q__)
				 && __itc_array[qid]->token < 0);

			Him->state = (PROCSTATE) ((Him->state) & ~_____QST);
			Him->wakeupEvent = E_ITC;
			tk_yield_CLI();
		}
	} else
		__itc_array[qid]->token++;
	STI_return(ERR_OK);
}

unsigned long q_vcreate(char name[4],
			unsigned long flags,
			unsigned long count,
			unsigned long mmlen, unsigned long *qid)
{
	unsigned long rc;

	if ((rc = sm_create(name, 0, flags, qid)) != ERR_OK)
		return (rc);

	if ((__itc_array[__itc_idx]->m.qv =
	     (qv_t *) calloc(count, sizeof(qv_t))) == NULL) {
		sm_delete(*qid);
		return (ERR_NOMEM);
	}

	__itc_array[__itc_idx]->sizeof_q = count;

	__itc_array[__itc_idx]->maxSizeof_m = mmlen;
	__itc_array[__itc_idx]->b_type = V_QUEUE;
	__itc_array[__itc_idx]->min_idx = 0;
	__itc_array[__itc_idx]->mout_idx = 0;
	return (ERR_OK);
}

unsigned long q_vdelete(unsigned long qid)
{
	unsigned long rc;
	unsigned int i;

	for (i = 0; i < __itc_array[qid]->sizeof_q; i++) {
		free(__itc_array[qid]->m.qv[i].mb);
	}
	free(__itc_array[qid]->m.qv);
	rc = sm_delete(qid);
	return (rc);
}

unsigned long q_vreceive(unsigned long qid, unsigned long flags,
			 unsigned long timeout, void *msg_buf,
			 unsigned long buf_len, unsigned long *msg_len)
{
	unsigned long rc;

	if (__itc_array[qid] == NULL)
		return (ERR_OBJDEL);
	if (__itc_array[qid]->b_type != V_QUEUE)
		return (ERR_OBJTYPE);
	if (__itc_array[qid]->token <= 0 && flags & NOWAIT)
		return (ERR_NOSEM);
	if (__itc_array[qid]->maxSizeof_m < buf_len)
		return (ERR_BUFSIZ);

	rc = lock_stage(qid, timeout);
	if (!(_itc_proveConcistency(qid)))
		assert(0);
	if (rc != ERR_OK)
		return (rc);

	memcpy(msg_buf,
	       __itc_array[qid]->m.qv[__itc_array[qid]->mout_idx].mb,
	       __itc_array[qid]->m.qv[__itc_array[qid]->mout_idx].s + 1);
	*msg_len = __itc_array[qid]->m.qv[__itc_array[qid]->mout_idx].s;

	free(__itc_array[qid]->m.qv[__itc_array[qid]->mout_idx].mb);

	__itc_array[qid]->mout_idx++;
	__itc_array[qid]->mout_idx %= __itc_array[qid]->sizeof_q;
	return (ERR_OK);
}

unsigned long q_vsend(unsigned long qid, void *msg_buf, unsigned long msg_len)
{
	unsigned long rc;

	if (__itc_array[qid] == NULL)
		return (ERR_OBJDEL);
	if (__itc_array[qid]->b_type != V_QUEUE)
		return (ERR_OBJTYPE);
	if (__itc_array[qid]->token > 0)
		if (!
		    (__itc_array[qid]->token < (int)__itc_array[qid]->sizeof_q))
			return (ERR_FULL);

	if ((__itc_array[__itc_idx]->m.qv[__itc_array[qid]->min_idx].mb =
	     malloc(msg_len + 1)) == NULL) {
		return (ERR_NOMEM);
	}

	memcpy(__itc_array[qid]->m.qv[__itc_array[qid]->min_idx].mb, msg_buf,
	       msg_len + 1);
	__itc_array[qid]->m.qv[__itc_array[qid]->min_idx].s = msg_len;

	__itc_array[qid]->min_idx++;
	__itc_array[qid]->min_idx %= __itc_array[qid]->sizeof_q;

	printk(("%s \n",
		__itc_array[qid]->m.qv[__itc_array[__itc_array[qid]->mout_idx]->
				       mout_idx].mb));

	rc = unlock_stage(qid);
	if (!(_itc_proveConcistency(qid)))
		assert(0);
	if (rc != ERR_OK)
		return (rc);
	return (ERR_OK);
}

unsigned long q_create(char name[4],
		       unsigned long count,
		       unsigned long flags, unsigned long *qid)
{
	unsigned long rc;

	if ((rc = sm_create(name, 0, flags, qid)) != ERR_OK)
		return (rc);

	if ((__itc_array[__itc_idx]->m.q =
	     (q_t *) calloc(count, sizeof(q_t))) == NULL) {
		sm_delete(*qid);
		return (ERR_NOMEM);
	}

	__itc_array[__itc_idx]->sizeof_q = count;

	__itc_array[__itc_idx]->b_type = S_QUEUE;
	__itc_array[__itc_idx]->min_idx = 0;
	__itc_array[__itc_idx]->mout_idx = 0;
	return (ERR_OK);
}

unsigned long q_delete(unsigned long qid)
{
	unsigned long rc;

	free(__itc_array[qid]->m.q);
	rc = sm_delete(qid);
	return (rc);
}

unsigned long q_receive(unsigned long qid,
			unsigned long flags,
			unsigned long timeout, unsigned long msg_buf[4]
    )
{
	unsigned long rc;

	if (__itc_array[qid] == NULL)
		return (ERR_OBJDEL);
	if (__itc_array[qid]->b_type != S_QUEUE)
		return (ERR_OBJTYPE);
	if (__itc_array[qid]->token <= 0 && flags & NOWAIT)
		return (ERR_NOSEM);

	rc = lock_stage(qid, timeout);
	if (!(_itc_proveConcistency(qid)))
		assert(0);
	if (rc != ERR_OK)
		return (rc);

	memcpy(msg_buf, __itc_array[qid]->m.q[__itc_array[qid]->mout_idx],
	       sizeof(q_t));
	__itc_array[qid]->mout_idx++;
	__itc_array[qid]->mout_idx %= __itc_array[qid]->sizeof_q;
	return (ERR_OK);
}

unsigned long q_send(unsigned long qid, unsigned long msg_buf[4]
    )
{
	unsigned long rc;

	if (__itc_array[qid] == NULL)
		return (ERR_OBJDEL);
	if (__itc_array[qid]->b_type != S_QUEUE)
		return (ERR_OBJTYPE);
	if (__itc_array[qid]->token > 0)
		if (!
		    (__itc_array[qid]->token < (int)__itc_array[qid]->sizeof_q))
			return (ERR_FULL);

	memcpy(__itc_array[qid]->m.q[__itc_array[qid]->min_idx], msg_buf,
	       sizeof(q_t));
	__itc_array[qid]->min_idx++;
	__itc_array[qid]->min_idx %= __itc_array[qid]->sizeof_q;

	rc = unlock_stage(qid);
	if (!(_itc_proveConcistency(qid)))
		assert(0);
	if (rc != ERR_OK)
		return (rc);
	return (ERR_OK);
}

unsigned long sm_create(char name[4],
			unsigned long count,
			unsigned long flags, unsigned long *qid)
{
	unsigned int rc, i;

	if ((rc = _itc_findNextEmpySlot()) != ERR_OK) {
		if (rc == ERR_OBJFULL) {
			return (rc);
		}
	} else if ((__itc_array[__itc_idx] = (itc_t *) malloc(sizeof(itc_t))) ==
		   NULL) {
		return (ERR_NOMEM);

	} else
	    if ((__itc_array[__itc_idx]->blocked_procs =
		 (tk_tcb_t **) malloc(TK_MAX_BLOCKED_ON_Q *
				      sizeof(tk_tcb_t *))) == NULL) {
		return (ERR_NOMEM);
	}

	for (i = 0; i < TK_MAX_BLOCKED_ON_Q; i++) {
		__itc_array[__itc_idx]->blocked_procs[i] = NULL;
	}
	strncpy(__itc_array[__itc_idx]->name, name, 4);
	__itc_array[__itc_idx]->token = count;
	__itc_array[__itc_idx]->flags = flags;
	__itc_array[__itc_idx]->sizeof_q = 0;

	__itc_array[__itc_idx]->b_type = SEM;
	__itc_array[__itc_idx]->in_idx = 0;
	__itc_array[__itc_idx]->out_idx = 0;

	*qid = __itc_idx;
	return (ERR_OK);
}

unsigned long sm_delete(unsigned long qid)
{

	free(__itc_array[qid]->blocked_procs);
	free(__itc_array[qid]);
	__itc_array[qid] = NULL;
	return (ERR_OK);
}

unsigned long sm_p(unsigned long qid,
		   unsigned long flags, unsigned long timeout)
{
	unsigned long rc;

	if (__itc_array[qid] == NULL)
		return (ERR_OBJDEL);
	if (__itc_array[qid]->b_type != SEM)
		return (ERR_OBJTYPE);
	if (__itc_array[qid]->token <= 0 && flags & NOWAIT)
		return (ERR_NOSEM);

	rc = lock_stage(qid, timeout);
	return (rc);
}

unsigned long sm_v(unsigned long qid)
{
	unsigned long rc;

	if (__itc_array[qid] == NULL)
		return (ERR_OBJDEL);
	if (__itc_array[qid]->b_type != SEM)
		return (ERR_OBJTYPE);

	rc = unlock_stage(qid);
	return (rc);

}

#endif
