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

@brief Non yield version of the native Inter-thread Communication

For in-depth discussions about this component, see \ref
ITC

@see ITC
@see COMPONENTS
*/

#include <implement_itc.h>
#ifdef TK_COMP_ITC

/*!
contains pointers to queue structs
*/
itc_t *__itc_array[TK_MAX_NUM_Q];

/*!
points to the most resently created ITC object
*/
int __itc_idx;


/*******************************************************************************
 * Local debugging tools
 ******************************************************************************/

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
int _itc_proveConcistency(unsigned int qid) {
/* 	Se if you can find exactly as many blocked processes between out and in
	as there are tokens. */
	unsigned int in = __itc_array[qid]->in_idx;
	unsigned int out = __itc_array[qid]->out_idx;
	unsigned int count = 0;
	unsigned int i;

	if (__itc_array[qid]->token > 0)
		return(TK_TRUE);

	for (i=out; i != in; i++, i %= TK_MAX_BLOCKED_ON_Q) {
		if (
			__itc_array[qid]->blocked_procs[i]->state & _____Q__ ||
			__itc_array[qid]->blocked_procs[i]->state == READY
		)
	count++;
	}

	if (count != (unsigned int)( abs(__itc_array[qid]->token) ) )
		return(TK_FALSE);
	return(TK_TRUE);
}

/*******************************************************************************
 * Local helper functions
 ******************************************************************************/

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Calculates the diff between 2 integers taking in account wrapparound
effect. Tested and works fine.

\reurns diff
*/
unsigned long _itc_uintDiff(
	unsigned long x1,       //!< x1, the integer suppused to be the lowest
	unsigned long x2,       //!< x2, the integer supposed to be the highest
	unsigned long max       //!< max value (wrapparound value)
) {
	return( x1 <= x2 ? x2-x1 : max - x1 + x2);
}


//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Helperfunction. In case ITC blocked function is released by
it needs to bee removed so that the blocked list doesen't
get full.
*/
void _itc_removeBlocked(itc_t *queue_p, unsigned int idx) {
	unsigned int cp_idx;
	unsigned int in_idx;

	in_idx = queue_p->in_idx;
	in_idx--;
	if ( in_idx > TK_MAX_BLOCKED_ON_Q) {
		in_idx = TK_MAX_BLOCKED_ON_Q - 1;
	}

	/* Special case (quicker). Take away from start instead */
	if (queue_p->out_idx == idx) {
		queue_p->blocked_procs[idx] = NULL;
		queue_p->out_idx++;
		queue_p->out_idx %=	TK_MAX_BLOCKED_ON_Q;
		queue_p->token++;
		return;
	}
	if (in_idx == idx) {
		queue_p->blocked_procs[idx] = NULL;
		queue_p->in_idx--;
		if ( queue_p->in_idx > TK_MAX_BLOCKED_ON_Q) {
			queue_p->in_idx = TK_MAX_BLOCKED_ON_Q - 1;
		}
		queue_p->token++;
		return;
	}

	cp_idx = idx + 1;
	cp_idx %= TK_MAX_BLOCKED_ON_Q;

	for (; idx != queue_p->in_idx ; ) {
		queue_p->blocked_procs[idx]	= queue_p->blocked_procs[cp_idx];
		cp_idx++;
		cp_idx %= TK_MAX_BLOCKED_ON_Q;
		idx++;
		idx %= TK_MAX_BLOCKED_ON_Q;
	}
	queue_p->in_idx--;
	if ( queue_p->in_idx > TK_MAX_BLOCKED_ON_Q) {
		queue_p->in_idx = TK_MAX_BLOCKED_ON_Q - 1;
	}
	queue_p->token++;
}

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned int _itc_findNextEmpySlot() {
	unsigned int seekcounter = 0;

	while ( (__itc_array[__itc_idx] != NULL) && ( seekcounter < TK_MAX_NUM_Q) ) {
		__itc_idx++;
		__itc_idx %= TK_MAX_NUM_Q;
		seekcounter ++;
	}
   return (seekcounter == TK_MAX_NUM_Q) ? ERR_OBJFULL : ERR_OK;
}


/*************************************************************************************************************
 * Public functions
 ************************************************************************************************************ */
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long tk_itc( void ){
	unsigned int i;

	__itc_idx = 0;

	for (i=0; i<TK_MAX_NUM_Q; i++) {
		__itc_array[i] = NULL;
	}
   return ERR_OK;
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long tk_itc_destruct( void ){
	unsigned long i,j;

	/* If for any reason any queue is left unallocated, try to free its */
	/* allocated memory */
	for (i=0; i<TK_MAX_NUM_Q; i++) {
		if (__itc_array[i] != NULL) {
			if (__itc_array[i]->b_type ==	 S_QUEUE)	   /* free memory allocated for simple queue */
				free( __itc_array[i]->m.q );
			else if (__itc_array[i]->b_type == V_QUEUE) {/* free memory allocated for complex queue */
				for (j=0; j<__itc_array[i]->sizeof_q; j++) {
					free( __itc_array[i]->m.qv[j].mb );
				}
				free( __itc_array[i]->m.qv );
			}
			free( __itc_array[i] );
		}
	}
	return ERR_OK;
}
//------1---------2---------3---------4---------5---------6---------7---------8

/** @defgroup ITC ITC: Native Inter-thread Communication
@ingroup COMPONENTS
@brief Inter-thread Communication (native)

<p>
The ITC component is TinKers native synchronization component. It
consists of the normal primitives you would expect. I.e. semaphores and
queues (but no mutexes). The package doesn't supply a mutex concept at
the moment.
</p>
<p>
Strictly speaking, all synchronization in any kernel is based around
<b>one</b> base mechanism, from which all <em>other are derived</em>. In
TinKer this base mechanism is a semaphore (i.e. a <em>"counting"</em> mutex).
</p>
<p>
In future, We might even consider stripping this component to contain
<em>only</em> this base mechanism, since all other needs can be handled
by the \ref PTHREAD and \ref POSIX_RTQUEUES components.
</p>

@note Avoid using this API in your application, since you will create a
dependancy towards TinKer. This API is mainly intended for internal
purposes to be able to implement the \ref PTHREAD and \ref
POSIX_RTQUEUES components.

The public API in this package is mostly un-documented because it's
based on a well known API (pSos).

@see http://dr-linux.net/newbase/reference/psosCD/data/html/5070_002/psc_ch01.htm#583402

The only exception are functions with the _ny suffix, which stands for a
non-yield version of the corresponding function (i.e. the function in
base-name).

@note When you work with queues, note that no matter if you use variable
data-length versions or fixed length versions. Data is always
<b>copied</b>. Only way to screw up, is by using fixed data-length
queues and then pass references. <b>Don't do that!</b>

For POSIX 1003.1b equivalent functions, the following might serve as a good starting point:

@see http://www.opengroup.org/onlinepubs/009695399/functions/sem_open.html

@todo Fix-length queues need to avoid malloc. Unnecessary time-consuming
operation. Replace data-field with an array instead (same size as a
pointer anyway).

<p><b>Go gack to</b> \ref COMPONENTS</p>

*/

#endif //TK_COMP_ITC