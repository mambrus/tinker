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

static unsigned long lock_stage_ny(
	unsigned long qid, 
	unsigned long timeout);
static unsigned long unlock_stage_ny(
	unsigned long qid);	

extern itc_t *__itc_array[TK_MAX_NUM_Q];	
extern int __itc_idx;						


/******************************************************************************
 *  FUNCTION NAME: _lock_stage_ny
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION:
 *
 *  RETURNS:
 *
 *****************************************************************************/
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
static unsigned long _lock_stage_ny(
	unsigned long qid, 
	unsigned long timeout) 
{
	tk_tcb_t *MySelf;       /* A pointer to my own process PCB */
	clock_t act_time;
	clock_t timeout_time;   /* When to timeout if timeoutable */
	unsigned int mark_idx;
	
	act_time = clock()/CLK_TCK*1000;
	MySelf = _tk_current_tcb();
	
	if ( __itc_array[qid]->token > 0 ) {  /*  */
		/* Wow, gone tru */
		__itc_array[qid]->token --;
		return(ERR_OK);
	}else {
		/* Hrrm, someone else was here before me */
		/* Can I block? */
		if (abs(__itc_array[qid]->token) < (TK_MAX_BLOCKED_ON_Q - 1) ) {
			__itc_array[qid]->token--;   
		}else
			return(ERR_BLOCKLIMIT);
		
	/* Put myself on the fifo list */
	__itc_array[qid]->blocked_procs[__itc_array[qid]->in_idx] = MySelf;
	__itc_array[qid]->in_idx++;
	__itc_array[qid]->in_idx %= TK_MAX_BLOCKED_ON_Q;
	/* Put myself om the "waiting" list */
	if (timeout == 0) {
		//assert(0);
		MySelf->state = (PROCSTATE)( (MySelf->state) | _____Q__);
		MySelf->wakeupEvent = E_NONE;
		//tk_yield();
		/* OK, I've been blocked and now I'm free*/     
		return(ERR_OK);
	} else {
		MySelf->state = (PROCSTATE)( (MySelf->state) | _____QS_);
		timeout_time = act_time + timeout;
		MySelf->wakeuptime = timeout_time;
		/*In case need to remove from list (timerelease)*/
		mark_idx = __itc_array[qid]->in_idx; /*One to far, correct later*/
		//tk_yield();
		
		/* who waked me?*/
		if (MySelf->wakeupEvent == E_TIMER) {
			/* Take process away from "que blocked" list*/
			mark_idx--;
			if (mark_idx > TK_MAX_BLOCKED_ON_Q) {
				mark_idx = TK_MAX_BLOCKED_ON_Q - 1; 
			}
		
			/*Scan block list for any more procs that has been released by timer*/
			
			if (__itc_array[qid]->token >= 0)
				return(ERR_TIMEOUT); /*DUNNO WHY*/
			
			_itc_removeBlocked(__itc_array[qid],mark_idx);
			
			if (__itc_array[qid]->token >= 0)
				return(ERR_TIMEOUT); /* Last one */ 
			
			mark_idx = __itc_array[qid]->out_idx;
			do {
				/*What happens if last one?? Good question!! solved above?*/
				if (
					__itc_array[qid]->blocked_procs[mark_idx]->state == READY &&
					__itc_array[qid]->blocked_procs[mark_idx]->wakeupEvent == E_TIMER
				) {
					/*Tag it, preventing succesiv removal*/
					__itc_array[qid]->blocked_procs[mark_idx]->wakeupEvent = E_ITC2;
					/*Remove it from blocked list */
					_itc_removeBlocked(__itc_array[qid],mark_idx);  
				}
				if (mark_idx == __itc_array[qid]->in_idx)
					break;
				mark_idx++; mark_idx %= TK_MAX_BLOCKED_ON_Q;
			}
			while(__itc_array[qid]->token < 0 && mark_idx != __itc_array[qid]->in_idx);
			return(ERR_TIMEOUT);
			
		}else if (MySelf->wakeupEvent == E_ITC2)
			return(ERR_TIMEOUT);
		return(ERR_OK);
		}
	}  
}

/******************************************************************************
 *  FUNCTION NAME: _unlock_stage_ny
 *
 *  PARAMETERS:
 *    ITC object identity.
 *
 *  DESCRIPTION:
 *    If processes is waiting, release the one chosen by certain criteria
 *    to READY.
 *
 * Note: 
 *    Freeing a different process means this function can block if the 
 *    process it releases has better priority.
 *
 *  RETURNS:
 *    ERR_OK 
 *
 *****************************************************************************/
//------1---------2---------3---------4---------5---------6---------7---------8 
/*!
Doc TBD

@todo documet this
*/
static unsigned long _unlock_stage_ny(
	unsigned long qid) 
{  
	tk_tcb_t *Him;       /* A pointer to the process to release  */
	unsigned int i;
	unsigned int t_idx,t_prio = TK_MAX_PRIO_LEVELS + 2;

	if (__itc_array[qid]->token < 0)  { /* Oops someone is perhaps waiting for me */
		if (__itc_array[qid]->flags & PRIOR) {
			
			/*Tidying away timerreleased processes*/ 
			for (
				i=__itc_array[qid]->out_idx;
				i!=__itc_array[qid]->in_idx; /*This is important, the last index can expire*/
				i++,i %= TK_MAX_BLOCKED_ON_Q)
			{
				if (!(__itc_array[qid]->blocked_procs[i]->state & _____Q__))  { 
					/*Oops, found one!*/
					//__itc_removeBlocked_ny(__itc_array[qid],i); 
				}
				if (__itc_array[qid]->in_idx == i)
				break;
			}
			//assert(__itc_array[qid]->token<0);
			if ((__itc_array[qid]->in_idx - __itc_array[qid]->out_idx) == 0) { /*__itc_array[qid]->token==0*/
				/*printk(("Hubba i �at\n"));
				while(1);*/
				return(ERR_OK);
			
			}
				
			/*Find the highest waiting process*/ 
			for (
				i=__itc_array[qid]->out_idx;
				i!=__itc_array[qid]->in_idx;
				i++,i %= TK_MAX_BLOCKED_ON_Q)
			{
				if (__itc_array[qid]->blocked_procs[i]->Prio < t_prio) {
					t_prio = __itc_array[qid]->blocked_procs[i]->Prio;
					t_idx = i;
				}
			}
			
			/*Now the highest waiting process should be found*/
			assert(t_prio != (TK_MAX_PRIO_LEVELS + 2)); /*Could not find anyone to release*/
			__itc_array[qid]->blocked_procs[t_idx]->state = (PROCSTATE)(__itc_array[qid]->blocked_procs[t_idx]->state & ~_____QST); 
			__itc_array[qid]->blocked_procs[t_idx]->wakeupEvent = E_ITC;
			//__itc_array[qid]->token++;
			_itc_removeBlocked(__itc_array[qid],t_idx);  
			//tk_yield(); 
			
		}else {
			
			do {
				Him = __itc_array[qid]->blocked_procs[__itc_array[qid]->out_idx];
				__itc_array[qid]->out_idx++,
				__itc_array[qid]->out_idx %= TK_MAX_BLOCKED_ON_Q;
				__itc_array[qid]->token++;
			}while(!(Him->state & _____Q__) && __itc_array[qid]->token < 0);
			
			/*Above can happen if made ready by timer */
			/*but has not runned (had the oppotunity */
			/*to remove from this list)*/
			/*Should be obsolite now, but is not*/
			Him->state = (PROCSTATE)( (Him->state) & ~_____QST); /* In case of timeout active release that to */
			Him->wakeupEvent = E_ITC;
			//+;tk_preemplist(Him);
			//tk_yield(); /*In case the one you've just released has higher prio, run it*/   
		}
	}else
		__itc_array[qid]->token++;   /* Increase number of tokens anyway */    
	return(ERR_OK);
}
/*******************************************************************************
  
  Non-yielded versions of the ITC API. These functions will not context switch.
  
  They will however test timeouts and set READY states and such.
   
 ******************************************************************************/
//------1---------2---------3---------4---------5---------6---------7---------8 
/*!
Doc TBD

@todo documet this
*/
unsigned long q_vcreate_ny(
	char name[4],
	unsigned long flags,
	unsigned long count,
	unsigned long mmlen,
	unsigned long *qid
){
   unsigned long rc;
   
	if ((rc =sm_create(name,0,flags,qid)) != ERR_OK)
		return(rc);
	/* Allocate memory for the message queue */
	if ((__itc_array[__itc_idx]->m.qv = (qv_t*)calloc(count,sizeof(qv_t)) ) == NULL) {
		sm_delete(*qid);
		return(ERR_NOMEM);
	}
	/* Complete the transformation to a complex message queue */
	__itc_array[__itc_idx]->sizeof_q     = count;
	/*__itc_array[__itc_idx]->sizeof_m      = 0ul;*/
	__itc_array[__itc_idx]->maxSizeof_m  = mmlen;
	__itc_array[__itc_idx]->b_type       = V_QUEUE;
	__itc_array[__itc_idx]->min_idx      = 0;
	__itc_array[__itc_idx]->mout_idx     = 0;
	return(ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_vdelete_ny(
	unsigned long qid       /* id */
) {
	unsigned long rc;
	unsigned int i;
	
	for ( i=0; i<__itc_array[qid]->sizeof_q ; i++) {
		free(__itc_array[qid]->m.qv[i].mb);         /*Delete evry messsage*/
	}
	free(__itc_array[qid]->m.qv);                  /*Delete message table*/
	rc = sm_delete(qid);
	return (rc);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_vreceive_ny(
	unsigned long qid,
	unsigned long flags,
	unsigned long timeout,
	void *msg_buf,
	unsigned long buf_len,  /* The size of your allocated buffer */
	unsigned long *msg_len
){
	unsigned long rc;
	
	/* Test if the id is valid */
	if (__itc_array[qid] == NULL )
		return(ERR_OBJDEL);
	if (__itc_array[qid]->b_type != V_QUEUE )
		return(ERR_OBJTYPE);
	if (__itc_array[qid]->token <= 0 && flags & NOWAIT )
		return(ERR_NOSEM);
	if (__itc_array[qid]->maxSizeof_m < buf_len )
		return(ERR_BUFSIZ);
		
	/* OK Semaphore is valid, back to buisiness */
	rc = _lock_stage_ny(qid, timeout);
	if (!(_itc_proveConcistency(qid)))
		assert(0);
	if (rc != ERR_OK)
		return(rc);
	
	/* If execution reached here it means there is a message in the queue */
	/*gotoxy(1,20);
	printk(("%s \n",msg_buf));
	printk(("%s \n",__itc_array[qid]->m.qv[__itc_array[__itc_array[qid]->mout_idx]->mout_idx].mb));*/
	memcpy(
		msg_buf, 
		__itc_array[qid]->m.qv[__itc_array[qid]->mout_idx].mb,
		__itc_array[qid]->m.qv[__itc_array[qid]->mout_idx].s + 1
	); 
	*msg_len = __itc_array[qid]->m.qv[__itc_array[qid]->mout_idx].s;
	/*The message is delivered, free the slot*/  
	free(__itc_array[qid]->m.qv[__itc_array[qid]->mout_idx].mb);
	
	__itc_array[qid]->mout_idx++;
	__itc_array[qid]->mout_idx %= __itc_array[qid]->sizeof_q;
	return(ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_vsend_ny(
	unsigned long qid,      
	void *msg_buf,
	unsigned long msg_len
){
	unsigned long rc;
	
	/* Test if the id is valid */
	if (__itc_array[qid] == NULL )
	return(ERR_OBJDEL);
	if (__itc_array[qid]->b_type != V_QUEUE )
	return(ERR_OBJTYPE);
	if (__itc_array[qid]->token > 0)
	if (!(__itc_array[qid]->token < (int)__itc_array[qid]->sizeof_q)) //Cast on the right side is important
		return(ERR_FULL);
	
	
	/* Allocate memory to store message */
	if ((__itc_array[__itc_idx]->m.qv[__itc_array[qid]->min_idx].mb = 
		malloc(msg_len + 1)) == NULL) 
	{
		return(ERR_NOMEM);
	}
	/* put the message in the message queue */
	memcpy(__itc_array[qid]->m.qv[__itc_array[qid]->min_idx].mb, msg_buf, msg_len + 1);
	__itc_array[qid]->m.qv[__itc_array[qid]->min_idx].s = msg_len;
	
	__itc_array[qid]->min_idx++;
	__itc_array[qid]->min_idx %= __itc_array[qid]->sizeof_q; 
	//gotoxy(1,19);
	printk(("%s \n",__itc_array[qid]->m.qv[__itc_array[__itc_array[qid]->mout_idx]->mout_idx].mb));
	
	
	rc = _unlock_stage_ny(qid);
	if (!(_itc_proveConcistency(qid)))
		assert(0);
	if (rc !=  ERR_OK)
		return(rc);
	return(ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_create_ny(
	char name[4],
	unsigned long count,
	unsigned long flags,
	unsigned long *qid
){
	unsigned long rc;
	
	if ((rc =sm_create(name,0,flags,qid)) != ERR_OK)
	return(rc);
	/* Allocate memory for the message queue */
	if ((__itc_array[__itc_idx]->m.q = (q_t*)calloc(count,sizeof(q_t)) ) == NULL) {
		sm_delete(*qid);
		return(ERR_NOMEM);
	}
	/* Complete the transformation to a simple message queue */
	__itc_array[__itc_idx]->sizeof_q = count;
	/*__itc_array[__itc_idx]->sizeof_m = 4;*/
	__itc_array[__itc_idx]->b_type = S_QUEUE;
	__itc_array[__itc_idx]->min_idx  = 0;
	__itc_array[__itc_idx]->mout_idx = 0;
	return(ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_delete_ny(
	unsigned long qid       /* id */
) {
	unsigned long rc;
	
	free(__itc_array[qid]->m.q);
	rc = sm_delete(qid);
	return (rc);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_receive_ny(
	unsigned long qid,      
	unsigned long flags,    
	unsigned long timeout,  
	unsigned long msg_buf[4]
){
	unsigned long rc;
	
	/* Test if the id is valid */
	if (__itc_array[qid] == NULL )
		return(ERR_OBJDEL);
	if (__itc_array[qid]->b_type != S_QUEUE )
		return(ERR_OBJTYPE);
	if (__itc_array[qid]->token <= 0 && flags & NOWAIT )
		return(ERR_NOSEM);
		
	/* OK Semaphore is valid, back to buisiness */
	rc = _lock_stage_ny(qid, timeout);
	if (!(_itc_proveConcistency(qid)))
		assert(0);
	if (rc != ERR_OK)
		return(rc);
	
	/* If execution reached here it means there is a message in the queue */
	memcpy(msg_buf, __itc_array[qid]->m.q[__itc_array[qid]->mout_idx], sizeof(q_t));
	__itc_array[qid]->mout_idx++;
	__itc_array[qid]->mout_idx %= __itc_array[qid]->sizeof_q;
	return(ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_send_ny(
	unsigned long qid,
	unsigned long msg_buf[4]
){
	unsigned long rc;
	TK_CLI();
	
	/* Test if the id is valid */
	if (__itc_array[qid] == NULL )
		{ return(ERR_OBJDEL); TK_STI(); }
	if (__itc_array[qid]->b_type != S_QUEUE )
		{ return(ERR_OBJTYPE); TK_STI(); }
	if (__itc_array[qid]->token > 0)
		if (!(__itc_array[qid]->token < (int)__itc_array[qid]->sizeof_q)) //Cast on the right side is important
			{ return(ERR_FULL); TK_STI(); }
	
	
	/* put the message in the message queue */
	memcpy(__itc_array[qid]->m.q[__itc_array[qid]->min_idx], msg_buf, sizeof(q_t));
	__itc_array[qid]->min_idx++;
	__itc_array[qid]->min_idx %= __itc_array[qid]->sizeof_q; 
	
	rc = _unlock_stage_ny(qid);
	if (!(_itc_proveConcistency(qid)))
		assert(0);
	if (rc !=  ERR_OK)
		{ return(rc); TK_STI(); }
	TK_STI();
	return(ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long sm_create_ny(
	char name[4],
	unsigned long count,
	unsigned long flags,
	unsigned long *qid
) {
	unsigned int rc,i;
	
	if ((rc = _itc_findNextEmpySlot()) != ERR_OK) {
		if (rc==ERR_OBJFULL) {
			return(rc); 
		}
	}else if ((__itc_array[__itc_idx] = (itc_t*)malloc(sizeof(itc_t)) ) == NULL) {
		return(ERR_NOMEM);
		/* Allocate memory for pointertable of tk_tcb_t pointers */
	}else if ((__itc_array[__itc_idx]->blocked_procs = (tk_tcb_t**)malloc(TK_MAX_BLOCKED_ON_Q * sizeof(tk_tcb_t*)) ) == NULL) {
		return(ERR_NOMEM);
	}
	
	/* Clean pointertable of blocked procs*/
	for (i=0; i<TK_MAX_BLOCKED_ON_Q; i++) {
		__itc_array[__itc_idx]->blocked_procs[i] = NULL;
	}
	strncpy(__itc_array[__itc_idx]->name,name,4);
	__itc_array[__itc_idx]->token =  count;
	__itc_array[__itc_idx]->flags = flags;
	__itc_array[__itc_idx]->sizeof_q = 0;
	/*__itc_array[__itc_idx]->sizeof_m = 0;*/
	__itc_array[__itc_idx]->b_type = SEM;
	__itc_array[__itc_idx]->in_idx  = 0;
	__itc_array[__itc_idx]->out_idx = 0;
	
	*qid = __itc_idx;
	return (ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long sm_delete_ny(
	unsigned long qid       /* id */
) {
	/* Check if ITC exists */
	/* Check if anyone is blocked (those are not freed but zombified)*/
	free(__itc_array[qid]->blocked_procs);
	free(__itc_array[qid]);
	__itc_array[qid] = NULL;
	return (ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long sm_p_ny( 			/* sm_receive or sm_get */
	unsigned long qid,		/* id */
	unsigned long flags,		/* attrib */
	unsigned long timeout		/* timeout in clock ticks !?vafan */
) {
	unsigned long rc;
	
	/* Test if the id is valid */
	if (__itc_array[qid] == NULL )
		return(ERR_OBJDEL);
	if (__itc_array[qid]->b_type != SEM )
		return(ERR_OBJTYPE);
	if (__itc_array[qid]->token <= 0 && flags & NOWAIT )
		return(ERR_NOSEM);
		
	/* OK Semaphore is valid, back to buisiness */
	rc = _lock_stage_ny(qid, timeout);
	return(rc);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long sm_v_ny(  /* sm_send or sm_put */
	unsigned long qid    /* id */
) {
	unsigned long rc;
	
	/* Test if the id is valid */ /*#$-NOTE:C001-$#*/
	if (__itc_array[qid] == NULL )
		return(ERR_OBJDEL);
	if (__itc_array[qid]->b_type != SEM )
		return(ERR_OBJTYPE);
	
	rc = _unlock_stage_ny(qid);
	return(rc);
	
	
}
//------1---------2---------3---------4---------5---------6---------7---------8
#endif //TK_COMP_ITC
