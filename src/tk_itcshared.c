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
#include <implement_itc.h>
#ifdef TK_COMP_ITC

itc_t *__itc_array[TK_MAX_NUM_Q];

int __itc_idx;
int _itc_proveConcistency(unsigned int qid)
{

	unsigned int in = __itc_array[qid]->in_idx;
	unsigned int out = __itc_array[qid]->out_idx;
	unsigned int count = 0;
	unsigned int i;

	if (__itc_array[qid]->token > 0)
		return (TK_TRUE);

	for (i = out; i != in; i++, i %= TK_MAX_BLOCKED_ON_Q) {
		if (__itc_array[qid]->blocked_procs[i]->state & _____Q__ ||
		    __itc_array[qid]->blocked_procs[i]->state == READY)
			count++;
	}

	if (count != (unsigned int)(abs(__itc_array[qid]->token)))
		return (TK_FALSE);
	return (TK_TRUE);
}

unsigned long _itc_uintDiff(unsigned long x1,
			    unsigned long x2, unsigned long max)
{
	return (x1 <= x2 ? x2 - x1 : max - x1 + x2);
}

void _itc_removeBlocked(itc_t * queue_p, unsigned int idx)
{
	unsigned int cp_idx;
	unsigned int in_idx;

	in_idx = queue_p->in_idx;
	in_idx--;
	if (in_idx > TK_MAX_BLOCKED_ON_Q) {
		in_idx = TK_MAX_BLOCKED_ON_Q - 1;
	}

	if (queue_p->out_idx == idx) {
		queue_p->blocked_procs[idx] = NULL;
		queue_p->out_idx++;
		queue_p->out_idx %= TK_MAX_BLOCKED_ON_Q;
		queue_p->token++;
		return;
	}
	if (in_idx == idx) {
		queue_p->blocked_procs[idx] = NULL;
		queue_p->in_idx--;
		if (queue_p->in_idx > TK_MAX_BLOCKED_ON_Q) {
			queue_p->in_idx = TK_MAX_BLOCKED_ON_Q - 1;
		}
		queue_p->token++;
		return;
	}

	cp_idx = idx + 1;
	cp_idx %= TK_MAX_BLOCKED_ON_Q;

	for (; idx != queue_p->in_idx;) {
		queue_p->blocked_procs[idx] = queue_p->blocked_procs[cp_idx];
		cp_idx++;
		cp_idx %= TK_MAX_BLOCKED_ON_Q;
		idx++;
		idx %= TK_MAX_BLOCKED_ON_Q;
	}
	queue_p->in_idx--;
	if (queue_p->in_idx > TK_MAX_BLOCKED_ON_Q) {
		queue_p->in_idx = TK_MAX_BLOCKED_ON_Q - 1;
	}
	queue_p->token++;
}

unsigned int _itc_findNextEmpySlot()
{
	unsigned int seekcounter = 0;

	while ((__itc_array[__itc_idx] != NULL) && (seekcounter < TK_MAX_NUM_Q)) {
		__itc_idx++;
		__itc_idx %= TK_MAX_NUM_Q;
		seekcounter++;
	}
	return (seekcounter == TK_MAX_NUM_Q) ? ERR_OBJFULL : ERR_OK;
}

unsigned long tk_itc(void)
{
	unsigned int i;

	__itc_idx = 0;

	for (i = 0; i < TK_MAX_NUM_Q; i++) {
		__itc_array[i] = NULL;
	}
	return ERR_OK;
}

unsigned long tk_itc_destruct(void)
{
	unsigned long i, j;

	for (i = 0; i < TK_MAX_NUM_Q; i++) {
		if (__itc_array[i] != NULL) {
			if (__itc_array[i]->b_type == S_QUEUE)
				free(__itc_array[i]->m.q);
			else if (__itc_array[i]->b_type == V_QUEUE) {
				for (j = 0; j < __itc_array[i]->sizeof_q; j++) {
					free(__itc_array[i]->m.qv[j].mb);
				}
				free(__itc_array[i]->m.qv);
			}
			free(__itc_array[i]);
		}
	}
	return ERR_OK;
}
#endif
