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

@todo CLK_TCK handling needs a look-over. Make it same as \ref tk_msleep

@see ITC
@see COMPONENTS
*/
  

/*- include files **/
#include <tk.h>

#include <stdio.h>			    	          	       	   	      	   
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

#define NOTDEBUG_WITH_CURSES  //!< Visual "debugging" for queue operations if disabled 

#if (TK_HOWTO_CLOCK == TK_FNK_STUBBED)
#define clock clock_stubbed
#endif

#include "implement_tk.h"
#include "context.h"
#include <tk_itc.h>

/* You should not test at sharp (libfile) version*/
#ifdef TK_COMP_ITC 
/*- local definitions **/

#ifdef NOTDEBUG_WITH_CURSES
	#define p_bQ(P,Q,R)   ((void)0)
#else
	#include <conio.h>
	#define p_bQ(P,Q,R) p_bQf(P,Q,R)
#endif
/* default settings */

/*- external functions **/

/*- external data **/

/*- internal functions **/
static void removeBlocked(
	itc_t *queue_p, 
	unsigned int idx);
static unsigned int findNextEmpySlot( 
	void );
static unsigned long lock_stage(
	unsigned long qid, 
	unsigned long timeout);
static unsigned long unlock_stage(
	unsigned long qid);	
static unsigned long uintDiff(
	unsigned long x1, 
	unsigned long x2, 
	unsigned long max);	
	
/*- public data **/

/*- private data **/
/*! 
contains pointers to queue structs 			
*/
static itc_t *ipc_array[TK_MAX_NUM_Q];	

/*! 
points to the most resently created ITC object
*/
static ipc_idx;						

/*- private functions **/
/*******************************************************************************
 * Local debugging tools
 ******************************************************************************/

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
int proveConcistency(unsigned int qid) {
/* 	Se if you can find exactly as many blocked processes between out and in
	as there are tokens. */
	unsigned int in = ipc_array[qid]->in_idx;
	unsigned int out = ipc_array[qid]->out_idx;
	unsigned int count = 0;
	unsigned int i;
	
	if (ipc_array[qid]->token > 0)
		return(TRUE);
		
	for (i=out; i != in; i++, i %= TK_MAX_BLOCKED_ON_Q) {
		if (
			ipc_array[qid]->blocked_procs[i]->state & _____Q__ ||
			ipc_array[qid]->blocked_procs[i]->state == READY
		)
      count++;
	}
	
	if (count != (unsigned int)( abs(ipc_array[qid]->token) ) )
		return(FALSE);
	return(TRUE);	
}

#ifndef NOTDEBUG_WITH_CURSES
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
int no_duplicateBlock(unsigned int qid, unsigned int mark) {
	tk_tcb_t *checkProc,*refProc = ipc_array[qid]->blocked_procs[mark];
	unsigned int in = ipc_array[qid]->in_idx;
	unsigned int out = ipc_array[qid]->out_idx;
	unsigned int i;

	for (i=out;i!=in;i++,i %= TK_MAX_BLOCKED_ON_Q) {
		if (i != mark)
			if (refProc == ipc_array[qid]->blocked_procs[i])
				return(FALSE);
	}
	return(TRUE);
}

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
void p_bQf(int x,int y,unsigned int qid) {
	
	unsigned int mark_idx,count = 0;	
	BOOL rc;
	
	gotoxy(x,y);
	printk(("                                                               "));
	gotoxy(x,y);
	rc = TRUE;
   
	if(ipc_array[qid]->token == 0)
		return;
	for(mark_idx=ipc_array[qid]->out_idx;mark_idx!=ipc_array[qid]->in_idx;mark_idx++,mark_idx %= TK_MAX_BLOCKED_ON_Q) {
		printk(("%s ",ipc_array[qid]->blocked_procs[mark_idx]->name));
		if (!(no_duplicateBlock(qid,mark_idx)))
			rc = FALSE;
		count++;
	} 
	printk(("T=%d D=%d",ipc_array[qid]->token, abs(ipc_array[qid]->out_idx - ipc_array[qid]->in_idx)) );
	printk(("\n"));
	
	//assert(abs(ipc_array[qid]->out_idx - ipc_array[qid]->in_idx) == abs(ipc_array[qid]->token) );
	
	assert(count==abs(ipc_array[qid]->token));
	assert(rc);
		
}
#endif //NOTDEBUG_WITH_CURSES
/*******************************************************************************
 * Local helper functions
 ******************************************************************************/

//------1---------2---------3---------4---------5---------6---------7---------8 
/*!
Calculates the diff between 2 integers taking in account wrapparound
effect. Tested and works fine.

\reurns diff
*/
unsigned long uintDiff(
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
static void removeBlocked(itc_t *queue_p, unsigned int idx) {
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
static unsigned int findNextEmpySlot() {
	unsigned int seekcounter = 0;
	
	while ( (ipc_array[ipc_idx] != NULL) && ( seekcounter < TK_MAX_NUM_Q) ) {
		ipc_idx++;
		ipc_idx %= TK_MAX_NUM_Q;
		seekcounter ++;
	}
   return (seekcounter == TK_MAX_NUM_Q) ? ERR_OBJFULL : ERR_OK;	
}

/******************************************************************************
 *  FUNCTION NAME: lock_stage
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
static unsigned long lock_stage(
	unsigned long qid, 
	unsigned long timeout) 
{
	tk_tcb_t *MySelf;			/* A pointer to my own process PCB */
	clock_t act_time;
	clock_t timeout_time;	/* When to timeout if timeoutable */
	unsigned int mark_idx;
	
	act_time = clock()/CLK_TCK*1000;
	MySelf = _tk_current_tcb();
	
	if ( ipc_array[qid]->token > 0 ) {	/*  */
		/* Wow, gone thru */
		ipc_array[qid]->token --;
		return(ERR_OK);
	}else {
		/* Hrrm, someone else was here before me */
		/* Can I block? */
		if (abs(ipc_array[qid]->token) < (TK_MAX_BLOCKED_ON_Q - 1) ) {
			ipc_array[qid]->token--;	
		}else
			return(ERR_BLOCKLIMIT);
			
		/* Put myself on the fifo list */
		ipc_array[qid]->blocked_procs[ipc_array[qid]->in_idx] = MySelf;
		ipc_array[qid]->in_idx++;
		ipc_array[qid]->in_idx %= TK_MAX_BLOCKED_ON_Q;
		/* Put myself om the "waiting" list */
		if (timeout == 0) {
			//assert(0);
			MySelf->state = ( (MySelf->state) | _____Q__);
			MySelf->wakeupEvent = 0;
			tk_yield();
			/* OK, I've been blocked and now I'm free*/		
			return(ERR_OK);
		}else {
			p_bQ(40,21,qid);
			MySelf->state = ( (MySelf->state) | _____QS_);
			timeout_time = act_time + timeout;
			MySelf->wakeuptime = timeout_time;
			/*In case need to remove from list (timerelease)*/
			mark_idx = ipc_array[qid]->in_idx; /*One to far, correct later*/
			tk_yield();
			
			/* who waked me?*/
			if (MySelf->wakeupEvent == E_TIMER) {
				/* Take process away from "que blocked" list*/
				mark_idx--;
				if (mark_idx > TK_MAX_BLOCKED_ON_Q) {
					mark_idx = TK_MAX_BLOCKED_ON_Q - 1;	
				}
				
				/*Scan block list for any more procs that has been released by timer*/
				
				if (ipc_array[qid]->token >= 0)
					return(ERR_TIMEOUT); /*DUNNO WHY*/
				
				removeBlocked(ipc_array[qid],mark_idx);
				
				if (ipc_array[qid]->token >= 0)
					return(ERR_TIMEOUT); /* Last one */	
					
				mark_idx = ipc_array[qid]->out_idx;
				do {
					/*What happens if last one?? Good question!! solved above?*/
					if (
						ipc_array[qid]->blocked_procs[mark_idx]->state == READY &&
						ipc_array[qid]->blocked_procs[mark_idx]->wakeupEvent == E_TIMER
					) {
						/*Tag it, preventing succesiv removal*/
						ipc_array[qid]->blocked_procs[mark_idx]->wakeupEvent = E_ITC2;
						/*Remove it from blocked list */
						removeBlocked(ipc_array[qid],mark_idx);	
					}
					if (mark_idx == ipc_array[qid]->in_idx)
						break;
					mark_idx++; mark_idx %= TK_MAX_BLOCKED_ON_Q;
				}
				while(ipc_array[qid]->token < 0 && mark_idx != ipc_array[qid]->in_idx);
				p_bQ(40,21,qid);
				return(ERR_TIMEOUT);
				
			}else if (MySelf->wakeupEvent == E_ITC2)
				return(ERR_TIMEOUT);
			return(ERR_OK);
		}
	}	
}

/******************************************************************************
 *  FUNCTION NAME: unlock_stage
 *
 *  PARAMETERS:
 *		ITC object identity.
 *
 *  DESCRIPTION:
 *		If processes is waiting, release the one chosen by certain criteria
 *		to READY.
 *
 *	Note: 
 *		Freeing a different process means this function can block if the 
 *		process it releases has better priority.
 *
 *  RETURNS:
 *		ERR_OK 
 *
 *****************************************************************************/
//------1---------2---------3---------4---------5---------6---------7---------8 
/*!
Doc TBD

@todo documet this
*/
static unsigned long unlock_stage(
	unsigned long qid) 
{	
	tk_tcb_t *Him;			/* A pointer to the process to release  */
	unsigned int i;
	unsigned int t_idx,t_prio = TK_MAX_PRIO_LEVELS + 2;
	
	if (ipc_array[qid]->token < 0)  { /* Oops someone is perhaps waiting for me */
		if (ipc_array[qid]->flags & PRIOR) {
			
			/*Tidying away timerreleased processes*/ 
			for (
				i=ipc_array[qid]->out_idx;
				i!=ipc_array[qid]->in_idx; /*This is important, the last index can expire*/
				i++,i %= TK_MAX_BLOCKED_ON_Q)
			{
				if (!(ipc_array[qid]->blocked_procs[i]->state & _____Q__))  { 
					/*Oops, found one!*/
					
					p_bQ(40,21,qid);

					//removeBlocked(ipc_array[qid],i);	
				}
				if (ipc_array[qid]->in_idx == i)
				  	break;
			}
			//assert(ipc_array[qid]->token<0);
			if ((ipc_array[qid]->in_idx - ipc_array[qid]->out_idx) == 0) { /*ipc_array[qid]->token==0*/
				/*printk(("Hubba i �at\n"));
				while(1);*/
				return(ERR_OK);
				
			}
							
			/*Find the highest waiting process*/ 
			for (
				i=ipc_array[qid]->out_idx;
				i!=ipc_array[qid]->in_idx;
				i++,i %= TK_MAX_BLOCKED_ON_Q)
			{
				if (ipc_array[qid]->blocked_procs[i]->Prio < t_prio) {
					t_prio = ipc_array[qid]->blocked_procs[i]->Prio;
					t_idx = i;
				}
			}
			
			/*Now the highest waiting process should be found*/
			p_bQ(40,21,qid);
			assert(t_prio != (TK_MAX_PRIO_LEVELS + 2)); /*Could not find anyone to release*/
			ipc_array[qid]->blocked_procs[t_idx]->state &= ~_____QST; 
			ipc_array[qid]->blocked_procs[t_idx]->wakeupEvent = E_ITC;
			//ipc_array[qid]->token++;
			removeBlocked(ipc_array[qid],t_idx);	
			tk_yield(); 
			
		}else {
			do {
				Him = ipc_array[qid]->blocked_procs[ipc_array[qid]->out_idx];
				ipc_array[qid]->out_idx++,
				ipc_array[qid]->out_idx %= TK_MAX_BLOCKED_ON_Q;
				ipc_array[qid]->token++;
			}while(!(Him->state & _____Q__) && ipc_array[qid]->token < 0);
			 /*Above can happen if made ready by timer */								   
			 /*but has not runned (had the oppotunity */								   
			 /*to remove from this list)*/								   
			 /*Should be obsolite now, but is not*/
			Him->state = ( (Him->state) & ~_____QST); /* In case of timeout active release that to */
			Him->wakeupEvent = E_ITC;
			tk_yield(); /*In case the one you've just released has higher prio, run it*/	
		}
	}else 				  
		ipc_array[qid]->token++;	/* Increase number of tokens anyway */	   
	return(ERR_OK);		   		
}

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
   
   if ( ipc_array[qid]->token > 0 ) {  /*  */
      /* Wow, gone tru */
      ipc_array[qid]->token --;
      return(ERR_OK);
   }else {
      /* Hrrm, someone else was here before me */
      /* Can I block? */
      if (abs(ipc_array[qid]->token) < (TK_MAX_BLOCKED_ON_Q - 1) ) {
         ipc_array[qid]->token--;   
      }else
         return(ERR_BLOCKLIMIT);
         
      /* Put myself on the fifo list */
      ipc_array[qid]->blocked_procs[ipc_array[qid]->in_idx] = MySelf;
      ipc_array[qid]->in_idx++;
      ipc_array[qid]->in_idx %= TK_MAX_BLOCKED_ON_Q;
      /* Put myself om the "waiting" list */
      if (timeout == 0) {
         //assert(0);
         MySelf->state = ( (MySelf->state) | _____Q__);
         MySelf->wakeupEvent = 0;
         //tk_yield();
         /* OK, I've been blocked and now I'm free*/     
         return(ERR_OK);
      }else {
         p_bQ(40,21,qid);
         MySelf->state = ( (MySelf->state) | _____QS_);
         timeout_time = act_time + timeout;
         MySelf->wakeuptime = timeout_time;
         /*In case need to remove from list (timerelease)*/
         mark_idx = ipc_array[qid]->in_idx; /*One to far, correct later*/
         //tk_yield();
         
         /* who waked me?*/
         if (MySelf->wakeupEvent == E_TIMER) {
            /* Take process away from "que blocked" list*/
            mark_idx--;
            if (mark_idx > TK_MAX_BLOCKED_ON_Q) {
               mark_idx = TK_MAX_BLOCKED_ON_Q - 1; 
            }
            
            /*Scan block list for any more procs that has been released by timer*/
            
            if (ipc_array[qid]->token >= 0)
               return(ERR_TIMEOUT); /*DUNNO WHY*/
            
            removeBlocked(ipc_array[qid],mark_idx);
            
            if (ipc_array[qid]->token >= 0)
               return(ERR_TIMEOUT); /* Last one */ 
               
            mark_idx = ipc_array[qid]->out_idx;
            do {
               /*What happens if last one?? Good question!! solved above?*/
               if (
                  ipc_array[qid]->blocked_procs[mark_idx]->state == READY &&
                  ipc_array[qid]->blocked_procs[mark_idx]->wakeupEvent == E_TIMER
               ) {
                  /*Tag it, preventing succesiv removal*/
                  ipc_array[qid]->blocked_procs[mark_idx]->wakeupEvent = E_ITC2;
                  /*Remove it from blocked list */
                  removeBlocked(ipc_array[qid],mark_idx);  
               }
               if (mark_idx == ipc_array[qid]->in_idx)
                  break;
               mark_idx++; mark_idx %= TK_MAX_BLOCKED_ON_Q;
            }
            while(ipc_array[qid]->token < 0 && mark_idx != ipc_array[qid]->in_idx);
            p_bQ(40,21,qid);
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
   
   if (ipc_array[qid]->token < 0)  { /* Oops someone is perhaps waiting for me */
      if (ipc_array[qid]->flags & PRIOR) {
         
         /*Tidying away timerreleased processes*/ 
         for (
            i=ipc_array[qid]->out_idx;
            i!=ipc_array[qid]->in_idx; /*This is important, the last index can expire*/
            i++,i %= TK_MAX_BLOCKED_ON_Q)
         {
            if (!(ipc_array[qid]->blocked_procs[i]->state & _____Q__))  { 
               /*Oops, found one!*/
               
               p_bQ(40,21,qid);

               //_removeBlocked_ny(ipc_array[qid],i); 
            }
            if (ipc_array[qid]->in_idx == i)
               break;
         }
         //assert(ipc_array[qid]->token<0);
         if ((ipc_array[qid]->in_idx - ipc_array[qid]->out_idx) == 0) { /*ipc_array[qid]->token==0*/
            /*printk(("Hubba i �at\n"));
            while(1);*/
            return(ERR_OK);
            
         }
                     
         /*Find the highest waiting process*/ 
         for (
            i=ipc_array[qid]->out_idx;
            i!=ipc_array[qid]->in_idx;
            i++,i %= TK_MAX_BLOCKED_ON_Q)
         {
            if (ipc_array[qid]->blocked_procs[i]->Prio < t_prio) {
               t_prio = ipc_array[qid]->blocked_procs[i]->Prio;
               t_idx = i;
            }
         }
         
         /*Now the highest waiting process should be found*/
         p_bQ(40,21,qid);
         assert(t_prio != (TK_MAX_PRIO_LEVELS + 2)); /*Could not find anyone to release*/
         ipc_array[qid]->blocked_procs[t_idx]->state &= ~_____QST; 
         ipc_array[qid]->blocked_procs[t_idx]->wakeupEvent = E_ITC;
         //ipc_array[qid]->token++;
         removeBlocked(ipc_array[qid],t_idx);  
         //tk_yield(); 
         
      }else {
         do {
            Him = ipc_array[qid]->blocked_procs[ipc_array[qid]->out_idx];
            ipc_array[qid]->out_idx++,
            ipc_array[qid]->out_idx %= TK_MAX_BLOCKED_ON_Q;
            ipc_array[qid]->token++;
         }while(!(Him->state & _____Q__) && ipc_array[qid]->token < 0);
          /*Above can happen if made ready by timer */                           
          /*but has not runned (had the oppotunity */                         
          /*to remove from this list)*/                           
          /*Should be obsolite now, but is not*/
         Him->state = ( (Him->state) & ~_____QST); /* In case of timeout active release that to */
         Him->wakeupEvent = E_ITC;
         //tk_yield(); /*In case the one you've just released has higher prio, run it*/   
      }
   }else               
      ipc_array[qid]->token++;   /* Increase number of tokens anyway */    
   return(ERR_OK);               
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
	
	ipc_idx = 0;
	
	for (i=0; i<TK_MAX_NUM_Q; i++) {
		ipc_array[i] = NULL;
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
		if (ipc_array[i] != NULL) {
			if (ipc_array[i]->b_type ==	 S_QUEUE)	   /* free memory allocated for simple queue */
				free( ipc_array[i]->m.q );				   
			else if (ipc_array[i]->b_type == V_QUEUE) {/* free memory allocated for complex queue */
				for (j=0; j<ipc_array[i]->sizeof_q; j++) {
					free( ipc_array[i]->m.qv[j].mb );					
				}
				free( ipc_array[i]->m.qv );
			}
			free( ipc_array[i] );
		}
	} 
   return ERR_OK;
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_vcreate(
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
	if ((ipc_array[ipc_idx]->m.qv = (qv_t*)calloc(count,sizeof(qv_t)) ) == NULL) {
		sm_delete(*qid);
		return(ERR_NOMEM);
	}
	/* Complete the transformation to a complex message queue */
	ipc_array[ipc_idx]->sizeof_q 		= count;
	/*ipc_array[ipc_idx]->sizeof_m 		= 0ul;*/
	ipc_array[ipc_idx]->maxSizeof_m 	= mmlen;
	ipc_array[ipc_idx]->b_type			= V_QUEUE;
	ipc_array[ipc_idx]->min_idx  		= 0;
	ipc_array[ipc_idx]->mout_idx 		= 0;
	return(ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_vdelete(
	unsigned long qid       /* id */
) {
	unsigned long rc;
	unsigned int i;
	
	for ( i=0; i<ipc_array[qid]->sizeof_q ; i++) {
		free(ipc_array[qid]->m.qv[i].mb);			/*Delete evry messsage*/
	}
	free(ipc_array[qid]->m.qv);						/*Delete message table*/
	rc = sm_delete(qid);
	return (rc);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_vreceive(
	unsigned long qid,      
	unsigned long flags,    
	unsigned long timeout,  
	void *msg_buf,
	unsigned long buf_len,	/* The size of your allocated buffer */
    unsigned long *msg_len
){
	unsigned long rc;
	
	/* Test if the id is valid */
	if (ipc_array[qid] == NULL )
		return(ERR_OBJDEL);
	if (ipc_array[qid]->b_type != V_QUEUE )
		return(ERR_OBJTYPE);
	if (ipc_array[qid]->token <= 0 && flags & NOWAIT )
		return(ERR_NOSEM);
	if (ipc_array[qid]->maxSizeof_m < buf_len )
		return(ERR_BUFSIZ);
			
	/* OK Semaphore is valid, back to buisiness */
	rc = lock_stage(qid, timeout);
	if (!(proveConcistency(qid)))
		assert(0);
	if (rc != ERR_OK)
		return(rc);

	/* If execution reached here it means there is a message in the queue */
	/*gotoxy(1,20);
	printk(("%s \n",msg_buf));
	printk(("%s \n",ipc_array[qid]->m.qv[ipc_array[ipc_array[qid]->mout_idx]->mout_idx].mb));*/
	memcpy(
		msg_buf, 
		ipc_array[qid]->m.qv[ipc_array[qid]->mout_idx].mb,
		ipc_array[qid]->m.qv[ipc_array[qid]->mout_idx].s + 1
	);	
	*msg_len = ipc_array[qid]->m.qv[ipc_array[qid]->mout_idx].s;
	/*The message is delivered, free the slot*/	
	free(ipc_array[qid]->m.qv[ipc_array[qid]->mout_idx].mb);
	
	ipc_array[qid]->mout_idx++;
	ipc_array[qid]->mout_idx %= ipc_array[qid]->sizeof_q;
	return(ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_vsend(
	unsigned long qid,      
	void *msg_buf,
	unsigned long msg_len
){
	unsigned long rc;
	
	/* Test if the id is valid */
	if (ipc_array[qid] == NULL )
		return(ERR_OBJDEL);
	if (ipc_array[qid]->b_type != V_QUEUE )
		return(ERR_OBJTYPE);
	if (ipc_array[qid]->token > 0)
		if (!(ipc_array[qid]->token < (int)ipc_array[qid]->sizeof_q))  //Cast on the right side is important
			return(ERR_FULL);
		
	
	/* Allocate memory to store message */
	if ((ipc_array[ipc_idx]->m.qv[ipc_array[qid]->min_idx].mb = 
		malloc(msg_len + 1)) == NULL) 
	{
		return(ERR_NOMEM);
	}
	/* put the message in the message queue */
	memcpy(ipc_array[qid]->m.qv[ipc_array[qid]->min_idx].mb, msg_buf, msg_len + 1);
	ipc_array[qid]->m.qv[ipc_array[qid]->min_idx].s = msg_len;
	
	ipc_array[qid]->min_idx++;
	ipc_array[qid]->min_idx %= ipc_array[qid]->sizeof_q; 
	//gotoxy(1,19);
	printk(("%s \n",ipc_array[qid]->m.qv[ipc_array[ipc_array[qid]->mout_idx]->mout_idx].mb));

	
	rc = unlock_stage(qid);
	if (!(proveConcistency(qid)))
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
unsigned long q_create(
   char name[4],         
   unsigned long count,  
   unsigned long flags,  
   unsigned long *qid    
){
	unsigned long rc;
	
   if ((rc =sm_create(name,0,flags,qid)) != ERR_OK)
		return(rc);
	/* Allocate memory for the message queue */
	if ((ipc_array[ipc_idx]->m.q = (q_t*)calloc(count,sizeof(q_t)) ) == NULL) {
		sm_delete(*qid);
		return(ERR_NOMEM);
	}
	/* Complete the transformation to a simple message queue */
	ipc_array[ipc_idx]->sizeof_q = count;
	/*ipc_array[ipc_idx]->sizeof_m = 4;*/
	ipc_array[ipc_idx]->b_type = S_QUEUE;
	ipc_array[ipc_idx]->min_idx  = 0;
	ipc_array[ipc_idx]->mout_idx = 0;
	return(ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_delete(
	unsigned long qid       /* id */
) {
	unsigned long rc;
	
	free(ipc_array[qid]->m.q);
	rc = sm_delete(qid);
	return (rc);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_receive(
	unsigned long qid,      
	unsigned long flags,    
	unsigned long timeout,  
	unsigned long msg_buf[4]
){
	unsigned long rc;
	
	/* Test if the id is valid */
	if (ipc_array[qid] == NULL )
		return(ERR_OBJDEL);
	if (ipc_array[qid]->b_type != S_QUEUE )
		return(ERR_OBJTYPE);
	if (ipc_array[qid]->token <= 0 && flags & NOWAIT )
		return(ERR_NOSEM);
			
	/* OK Semaphore is valid, back to buisiness */
	rc = lock_stage(qid, timeout);
	if (!(proveConcistency(qid)))
		assert(0);
	if (rc != ERR_OK)
		return(rc);

	/* If execution reached here it means there is a message in the queue */
	memcpy(msg_buf, ipc_array[qid]->m.q[ipc_array[qid]->mout_idx], sizeof(q_t));
	ipc_array[qid]->mout_idx++;
	ipc_array[qid]->mout_idx %= ipc_array[qid]->sizeof_q;
	return(ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long q_send(
	unsigned long qid,      
	unsigned long msg_buf[4]
){
	unsigned long rc;
	
	/* Test if the id is valid */
	if (ipc_array[qid] == NULL )
		return(ERR_OBJDEL);
	if (ipc_array[qid]->b_type != S_QUEUE )
		return(ERR_OBJTYPE);
	if (ipc_array[qid]->token > 0)
		if (!(ipc_array[qid]->token < (int)ipc_array[qid]->sizeof_q)) //Cast on the right side is important
			return(ERR_FULL);
		
	
	/* put the message in the message queue */
	memcpy(ipc_array[qid]->m.q[ipc_array[qid]->min_idx], msg_buf, sizeof(q_t));
	ipc_array[qid]->min_idx++;
	ipc_array[qid]->min_idx %= ipc_array[qid]->sizeof_q; 
	
	rc = unlock_stage(qid);
	if (!(proveConcistency(qid)))
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
unsigned long sm_create(
	char name[4],           
	unsigned long count,    
	unsigned long flags,    
	unsigned long *qid      
) {
	unsigned int rc,i;

	if ((rc = findNextEmpySlot()) != ERR_OK) {
		if (rc==ERR_OBJFULL) {
		   return(rc);	
		}						
	}else if ((ipc_array[ipc_idx] = (itc_t*)malloc(sizeof(itc_t)) ) == NULL) {
		return(ERR_NOMEM);
	/* Allocate memory for pointertable of tk_tcb_t pointers */
	}else if ((ipc_array[ipc_idx]->blocked_procs = (tk_tcb_t**)malloc(TK_MAX_BLOCKED_ON_Q * sizeof(tk_tcb_t*)) ) == NULL) {
			return(ERR_NOMEM);
	}
	
	/* Clean pointertable of blocked procs*/
	for (i=0; i<TK_MAX_BLOCKED_ON_Q; i++) {
		ipc_array[ipc_idx]->blocked_procs[i] = NULL;
	}
	strncpy(ipc_array[ipc_idx]->name,name,4);
	ipc_array[ipc_idx]->token =  count;
	ipc_array[ipc_idx]->flags = flags;
	ipc_array[ipc_idx]->sizeof_q = 0;
	/*ipc_array[ipc_idx]->sizeof_m = 0;*/
	ipc_array[ipc_idx]->b_type = SEM;
	ipc_array[ipc_idx]->in_idx  = 0;
	ipc_array[ipc_idx]->out_idx = 0;
	
	*qid = ipc_idx;
	return (ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long sm_delete(
	unsigned long qid       /* id */
) {
	/* Check if ITC exists */
	/* Check if anyone is blocked (those are not freed but zombified)*/
	free(ipc_array[qid]->blocked_procs);
	free(ipc_array[qid]);
	ipc_array[qid] = NULL;
	return (ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
@brief Acquires a semaphore token (take the semaphore)

Corresponds to POSIX 1003.1b sm_receive or sm_get              
*/
unsigned long sm_p(				/* sm_receive or sm_get */
	unsigned long qid,      	/* id */
	unsigned long flags,    	/* attrib */
	unsigned long timeout   	/* timeout in clock ticks !?vafan */
) {
	unsigned long rc;
	
	/* Test if the id is valid */
	if (ipc_array[qid] == NULL )
		return(ERR_OBJDEL);
	if (ipc_array[qid]->b_type != SEM )
		return(ERR_OBJTYPE);
	if (ipc_array[qid]->token <= 0 && flags & NOWAIT )
		return(ERR_NOSEM);
			
	/* OK Semaphore is valid, back to buisiness */
	rc = lock_stage(qid, timeout);
	return(rc);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
@brief Releases a semaphore token

Corresponds to POSIX 1003.1b sm_send or sm_put                  
*/
unsigned long sm_v(			   /* sm_send or sm_put */
   unsigned long qid		      /* id */
) {
	unsigned long rc;
		
	/* Test if the id is valid */ /*#$-NOTE:C001-$#*/
	if (ipc_array[qid] == NULL )
		return(ERR_OBJDEL);
	if (ipc_array[qid]->b_type != SEM )
		return(ERR_OBJTYPE);
		
	rc = unlock_stage(qid);
	return(rc);
	  
	
}


/*******************************************************************************
  
  Un-yielded versions of the same API. These functions will not context switch.
  
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
   if ((ipc_array[ipc_idx]->m.qv = (qv_t*)calloc(count,sizeof(qv_t)) ) == NULL) {
      sm_delete(*qid);
      return(ERR_NOMEM);
   }
   /* Complete the transformation to a complex message queue */
   ipc_array[ipc_idx]->sizeof_q     = count;
   /*ipc_array[ipc_idx]->sizeof_m      = 0ul;*/
   ipc_array[ipc_idx]->maxSizeof_m  = mmlen;
   ipc_array[ipc_idx]->b_type       = V_QUEUE;
   ipc_array[ipc_idx]->min_idx      = 0;
   ipc_array[ipc_idx]->mout_idx     = 0;
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
   
   for ( i=0; i<ipc_array[qid]->sizeof_q ; i++) {
      free(ipc_array[qid]->m.qv[i].mb);         /*Delete evry messsage*/
   }
   free(ipc_array[qid]->m.qv);                  /*Delete message table*/
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
   if (ipc_array[qid] == NULL )
      return(ERR_OBJDEL);
   if (ipc_array[qid]->b_type != V_QUEUE )
      return(ERR_OBJTYPE);
   if (ipc_array[qid]->token <= 0 && flags & NOWAIT )
      return(ERR_NOSEM);
   if (ipc_array[qid]->maxSizeof_m < buf_len )
      return(ERR_BUFSIZ);
         
   /* OK Semaphore is valid, back to buisiness */
   rc = _lock_stage_ny(qid, timeout);
   if (!(proveConcistency(qid)))
      assert(0);
   if (rc != ERR_OK)
      return(rc);

   /* If execution reached here it means there is a message in the queue */
   /*gotoxy(1,20);
   printk(("%s \n",msg_buf));
   printk(("%s \n",ipc_array[qid]->m.qv[ipc_array[ipc_array[qid]->mout_idx]->mout_idx].mb));*/
   memcpy(
      msg_buf, 
      ipc_array[qid]->m.qv[ipc_array[qid]->mout_idx].mb,
      ipc_array[qid]->m.qv[ipc_array[qid]->mout_idx].s + 1
   ); 
   *msg_len = ipc_array[qid]->m.qv[ipc_array[qid]->mout_idx].s;
   /*The message is delivered, free the slot*/  
   free(ipc_array[qid]->m.qv[ipc_array[qid]->mout_idx].mb);
   
   ipc_array[qid]->mout_idx++;
   ipc_array[qid]->mout_idx %= ipc_array[qid]->sizeof_q;
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
   if (ipc_array[qid] == NULL )
      return(ERR_OBJDEL);
   if (ipc_array[qid]->b_type != V_QUEUE )
      return(ERR_OBJTYPE);
   if (ipc_array[qid]->token > 0)
      if (!(ipc_array[qid]->token < (int)ipc_array[qid]->sizeof_q)) //Cast on the right side is important
         return(ERR_FULL);
      
   
   /* Allocate memory to store message */
   if ((ipc_array[ipc_idx]->m.qv[ipc_array[qid]->min_idx].mb = 
      malloc(msg_len + 1)) == NULL) 
   {
      return(ERR_NOMEM);
   }
   /* put the message in the message queue */
   memcpy(ipc_array[qid]->m.qv[ipc_array[qid]->min_idx].mb, msg_buf, msg_len + 1);
   ipc_array[qid]->m.qv[ipc_array[qid]->min_idx].s = msg_len;
   
   ipc_array[qid]->min_idx++;
   ipc_array[qid]->min_idx %= ipc_array[qid]->sizeof_q; 
   //gotoxy(1,19);
   printk(("%s \n",ipc_array[qid]->m.qv[ipc_array[ipc_array[qid]->mout_idx]->mout_idx].mb));

   
   rc = _unlock_stage_ny(qid);
   if (!(proveConcistency(qid)))
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
   if ((ipc_array[ipc_idx]->m.q = (q_t*)calloc(count,sizeof(q_t)) ) == NULL) {
      sm_delete(*qid);
      return(ERR_NOMEM);
   }
   /* Complete the transformation to a simple message queue */
   ipc_array[ipc_idx]->sizeof_q = count;
   /*ipc_array[ipc_idx]->sizeof_m = 4;*/
   ipc_array[ipc_idx]->b_type = S_QUEUE;
   ipc_array[ipc_idx]->min_idx  = 0;
   ipc_array[ipc_idx]->mout_idx = 0;
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
   
   free(ipc_array[qid]->m.q);
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
   if (ipc_array[qid] == NULL )
      return(ERR_OBJDEL);
   if (ipc_array[qid]->b_type != S_QUEUE )
      return(ERR_OBJTYPE);
   if (ipc_array[qid]->token <= 0 && flags & NOWAIT )
      return(ERR_NOSEM);
         
   /* OK Semaphore is valid, back to buisiness */
   rc = _lock_stage_ny(qid, timeout);
   if (!(proveConcistency(qid)))
      assert(0);
   if (rc != ERR_OK)
      return(rc);

   /* If execution reached here it means there is a message in the queue */
   memcpy(msg_buf, ipc_array[qid]->m.q[ipc_array[qid]->mout_idx], sizeof(q_t));
   ipc_array[qid]->mout_idx++;
   ipc_array[qid]->mout_idx %= ipc_array[qid]->sizeof_q;
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
   if (ipc_array[qid] == NULL )
      { return(ERR_OBJDEL); TK_STI(); }
   if (ipc_array[qid]->b_type != S_QUEUE )
      { return(ERR_OBJTYPE); TK_STI(); }
   if (ipc_array[qid]->token > 0)
      if (!(ipc_array[qid]->token < (int)ipc_array[qid]->sizeof_q)) //Cast on the right side is important
         { return(ERR_FULL); TK_STI(); }
      
   
   /* put the message in the message queue */
   memcpy(ipc_array[qid]->m.q[ipc_array[qid]->min_idx], msg_buf, sizeof(q_t));
   ipc_array[qid]->min_idx++;
   ipc_array[qid]->min_idx %= ipc_array[qid]->sizeof_q; 
   
   rc = _unlock_stage_ny(qid);
   if (!(proveConcistency(qid)))
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

   if ((rc = findNextEmpySlot()) != ERR_OK) {
      if (rc==ERR_OBJFULL) {
         return(rc); 
      }                 
   }else if ((ipc_array[ipc_idx] = (itc_t*)malloc(sizeof(itc_t)) ) == NULL) {
      return(ERR_NOMEM);
   /* Allocate memory for pointertable of tk_tcb_t pointers */
   }else if ((ipc_array[ipc_idx]->blocked_procs = (tk_tcb_t**)malloc(TK_MAX_BLOCKED_ON_Q * sizeof(tk_tcb_t*)) ) == NULL) {
         return(ERR_NOMEM);
   }
   
   /* Clean pointertable of blocked procs*/
   for (i=0; i<TK_MAX_BLOCKED_ON_Q; i++) {
      ipc_array[ipc_idx]->blocked_procs[i] = NULL;
   }
   strncpy(ipc_array[ipc_idx]->name,name,4);
   ipc_array[ipc_idx]->token =  count;
   ipc_array[ipc_idx]->flags = flags;
   ipc_array[ipc_idx]->sizeof_q = 0;
   /*ipc_array[ipc_idx]->sizeof_m = 0;*/
   ipc_array[ipc_idx]->b_type = SEM;
   ipc_array[ipc_idx]->in_idx  = 0;
   ipc_array[ipc_idx]->out_idx = 0;
   
   *qid = ipc_idx;
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
   free(ipc_array[qid]->blocked_procs);
   free(ipc_array[qid]);
   ipc_array[qid] = NULL;
   return (ERR_OK);
}
//------1---------2---------3---------4---------5---------6---------7---------8
/*!
Doc TBD

@todo documet this
*/
unsigned long sm_p_ny(        /* sm_receive or sm_get */
   unsigned long qid,         /* id */
   unsigned long flags,       /* attrib */
   unsigned long timeout      /* timeout in clock ticks !?vafan */
) {
   unsigned long rc;
   
   /* Test if the id is valid */
   if (ipc_array[qid] == NULL )
      return(ERR_OBJDEL);
   if (ipc_array[qid]->b_type != SEM )
      return(ERR_OBJTYPE);
   if (ipc_array[qid]->token <= 0 && flags & NOWAIT )
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
   if (ipc_array[qid] == NULL )
      return(ERR_OBJDEL);
   if (ipc_array[qid]->b_type != SEM )
      return(ERR_OBJTYPE);
      
   rc = _unlock_stage_ny(qid);
   return(rc);
     
   
}
//------1---------2---------3---------4---------5---------6---------7---------8
#endif

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



   
/*!
 * @defgroup CVSLOG_tk_ipc_c tk_ipc_c
 * @ingroup CVSLOG
 *
 *  $Log: tk_itc.c,v $
 *  Revision 1.28  2006-11-05 14:19:00  ambrmi09
 *  Build system and source modified to make better use of config.h
 *
 *  This file now contains information about how the kernel is configured
 *  and can be used by both application and kernel build (old solution only
 *  let kernel-buils know of these details).
 *
 *  This applies to both tk_tuning, component configuration among others.
 *  Use './configure --help' to see a full list. Note that  if a certain
 *  feature is not configured, the old tk_tuning will fill in the gaps.
 *  This is especially usefull when not using GNU build- and configure-
 *  tool-chain. Hopefully, we'll be able to get rid of tk_tuning.h in the
 *  future.
 *
 *  Revision 1.27  2006/04/08 10:16:03  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.26.2.2  2006/04/03 20:07:29  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.26.2.1  2006/04/03 15:21:48  ambrmi09
 *  All targets updated with the new thread-starter (alternative 2).
 *
 *  This alternative has one weakness (explained elsewhere togeather
 *  with alternative 1), but so far it's the only one that will compile
 *  and function equally among all 4 (very different) compilers currently
 *  tested against: GCC, MSVC, BC5 and Keil.
 *
 *  If nothing else turns up, I'm willing to overcome the drawback (it's
 *  quite handleable) because it *truly* takes away a lot of pain with
 *  porting.
 *
 *  The ARM port (architecture level) is than's to this now fully operational
 *  without the r13 hack in the context switch. This includes thread
 *  cancellation and thread argument passing (which were not functioning in
 *  the old port).
 *
 *  If this revised code proves itself (i.e. no surprises turns up) then
 *  TinKer can be considered "almost ported" to any HW target that GCC is
 *  ported for :D (/cheers)
 *
 *  Revision 1.26  2006/03/19 22:57:55  ambrmi09
 *  First naive implementation of a pthread mutex
 *
 *  Revision 1.25  2006/03/19 12:44:37  ambrmi09
 *  Got rid of many compilation warnings. MSVC amd GCC actually gompiles
 *  without one single warning (yay!). Be aware that ther was a lot of
 *  comparisons between signed/unsigned in ITC. Fetts a bit shaky...
 *
 *  Revision 1.24  2006/03/12 15:08:56  ambrmi09
 *  - Adjusted the source to accomodate the new file structure.
 *
 *  - All build environments uppdated and verified except BC5. For this one
 *  we stumbled across the header-file issue that I've been fearing. Seems
 *  we need to take care of that one after all.
 *
 *  @note The malloc bug still not solved.
 *
 *  Revision 1.23  2006/03/11 14:37:50  ambrmi09
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
 *  Revision 1.22  2006/03/05 11:11:28  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.21  2006/03/05 10:39:02  ambrmi09
 *  Code will now compile for arm7. Note compile only, not run.
 *
 *  Revision 1.20  2006/02/28 18:59:23  ambrmi09
 *  Minor adjustments and verified that all (intended) build environment
 *  compile and produce runnable code. I.e.:
 *  - uVisionKernelProject
 *  - BC5
 *  - Core and POSIX projects in MSVC
 *
 *  Revision 1.19  2006/02/28 11:50:08  ambrmi09
 *  - Trimmed the time constants (ruffly). 4sek per 14hrs drift
 *  - Revived the Borland C (BC5) target. Projectfile also added (BC5.ide)
 *  - Started experimenting with a indlude filename macro, that has the
 *    the potential of solving my ANSI header/function dilemma (\ref
 *    BUILDCHAIN )
 *  - Some "fishyness" about BC5 handling of time. Either \ref clock or
 *    \ref CLK_TCK doesn't follow standard (the latter I know for a fact,
 *    since it's 1e3 instead of 1e6 - but thats not all). \ref tk_msleep is
 *    adjusted to try to see the error.
 *
 *  Revision 1.18  2006/02/23 15:33:33  ambrmi09
 *  Found a nasty "bug", that was not a read bug after all. At least not in the kernel as a feared. It turned out that I forgot some of the details about how timeouts were to be handled (especially in \ref ITC ). A timeout of value \b zero is equal of never to timeout (read more about it in define \ref FOREVER). However two important lesson learned: Even simple add operations get "funny" when adding large numbers (see line 303 in tk_ipc.c - in the \ref lock_stage function). Anyway. FOREVER should equal zero. (This issue makes me wonder sometimes how sane it really was to resurrect a project that has been dormant for nearly 10 years.) The CodeWright project ruler should be positioned on the actual line btw. This check-in will be accompanied  by a <tt>cvs tag</tt> for this reason, and for yet another nasty bug that seems to be a real dispatcher bug. The current source-set-up will show the bug within one mint (which is good since it makes it a little bit less of a search for the <I>"needle in the haystack</i>").
 *
 *  Revision 1.17  2006/02/22 13:05:47  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.16  2006/02/20 19:17:15  ambrmi09
 *  - Made the errno variable thread specific (each thread has it's own)
 *  - Hid the details of using errno so that setting and reading it looks
 *    like using a normal variable
 *  - Extracted some stuff from tk.h that doesn't need to be public
 *  - Implemented perros and strerror including a storage with all the error
 *    strings (will go into NV ROM on a embedded system).
 *
 *  Revision 1.15  2006/02/20 15:22:01  ambrmi09
 *  Documentation stuff. No code changes.
 *
 *  Revision 1.14  2006/02/19 22:00:39  ambrmi09
 *  Major brake-through!!! First working attempt with crude pThreads and
 *  POSIX RT queues works. (jihaa) :=D. Wow
 *
 *  Revision 1.13  2006/02/19 12:44:33  ambrmi09
 *  - Documented ITC
 *  - Started to build up the structure for the \ref PTHREAD component
 *
 *  Revision 1.12  2006/02/17 21:17:27  ambrmi09
 *  More doc structure - this time for the new PTHREAD's components
 *
 *  Revision 1.11  2006/02/16 15:11:00  ambrmi09
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
 *  Revision 1.10  2006/02/02 15:51:02  ambrmi09
 *  A lot of thought has been invested into the new PTIME component. Had to
 *  change things even in the systime parts (integrated in the SHEDUL
 *  component) to make it more generic. Think this will be really nice when
 *  it's ready, but has been a long road to get PTIME running (and I'm
 *  still not there).
 *
 *  Revision 1.9  2005/12/03 14:04:30  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *  Revision 1.8  2005/12/01 13:05:25  ambrmi09
 *  This check-in if for preparing for peemtive mechanism (first try)
 *  Done since last check-in
 *
 *  - Got rid of a nasty include bug for the target dependant sys-files
 *  - Added yet anoter sys header file with type only, so that kernel doesn't
 *    have to include the whole lot (which will render in  another "bug" due
 *    to that Keil and Dave define the same thing. This might be solved
 *    differently later, but the separation doesnt hurt anyway.
 *  - Started a concept of system queues - which I hope will be the basis for
 *    the TinKer drivers concept (yet to be invented).
 *  - Made first crude attempts with preemtion.
 *
 *  Revision 1.7  2005/11/30 22:21:22  ambrmi09
 *  Mechanism for detecting stack integrity violation introduced. It needs more
 *  work. An interrupt will taint the current stack if it's using any kernel
 *  functions. This is not what we want, but the main idea is captured in this
 *  check-in.
 *
 *  Revision 1.6  2005/11/26 11:38:40  ambrmi09
 *  Cosmetic changes concerning CVS logs in source.
 *
 *  Revision 1.5  2005/11/25 14:35:16  ambrmi09
 *  A first naive aproach of ISR to thread syncronisation mechanism. It works
 *  but it is unstable.
 *
 *  Revision 1.4  2005/11/24 19:40:12  ambrmi09
 *  Lots of kernel API name changing. To support consistency and to prepare for
 *  the pthreads port.
 *
 *  Revision 1.3  2005/11/23 20:46:43  ambrmi09
 *  Finally stacks seems OK. A bit worried about some "garbage" that turns up
 *  at each TOS at each tasks start
 *
 *  Revision 1.2  2005/11/23 11:27:19  ambrmi09
 *  Unix line-endings corrected. Iny one LF per line should now be in the file on CVS server
 *
 *  Revision 1.1.1.1  2005/11/17 09:59:09  ambrmi09
 *  Created CVS jhome for TinKer from scratch. RCS history lost in database (kept in sourcecode)
 *
 *  Revision 1.10  1998/02/16 18:02:28  mickey
 *  Corrected bugs in previous version. This seams to behave stable.
 *  Still a bit unsecure about all the solutions. Don't understand then all
 *  aspecially  R 291. Assert technology found some cases that I missed .
 *
 *  Revision 1.9  1998/02/14 19:32:54  mickey
 *  Found cases when previous version bailed out ( >1 timerreleases)
 *  This version works better, but I'n ot satisfied. Solution will cause trouble
 *  in priority semaphores
 *
 *  Revision 1.8  1998/02/14 10:37:42  mickey
 *  Timeoutable semaphores. 1st implementation.
 *  This was a hard nut to crack.
 *
 *  Revision 1.7  1998/02/10 20:58:35  mickey
 *  Working priority semaphore. Poor solution. Static sm will not work with
 *  more than one semaphores. Gives slow performance.
 *
 *  Revision 1.6  1998/02/04 20:49:18  mickey
 *  FIrst atteptt to solve priority semaphores (has errors)					 
 *
 *  Revision 1.5  1998/02/01 20:03:27  mickey
 *  Jiihaa, first version of a working semaphore
 *
 *  Revision 1.4  1998/01/28 20:36:59  mickey
 *  First revision
 *
 *  Revision 1.3  1998/01/28 20:33:27  mickey
 *  <>
 *
 *  Revision 1.2  1998/01/28 20:32:18  mickey
 *  First Revision
 *  
 *******************************************************************/

