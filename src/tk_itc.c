/*******************************************************************
 *
 *  DESCRIPTION: tk_ipc.c Inter Process Comunication TinKerOS.
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:    
 *
 *  Current $Revision: 1.8 $
 *******************************************************************/
/** include files **/
#include <stdio.h>			    	          	       	   	      	   
#include <stdlib.h>
#include <string.h>

#define NDEBUG
#include <tk.h>
#include <tk_hwsys.h>
#include <tk_ipc.h>

#ifdef IPC /* Should not test at sharp (libfile) version */
/** local definitions **/

#ifdef NDEBUG
	#define p_bQ(P,Q,R)   ((void)0)
#else
	#include <conio.h>
	#define p_bQ(P,Q,R) p_bQf(P,Q,R)
#endif
/* default settings */

/** external functions **/

/** external data **/

/** internal functions **/
static void removeBlocked(
	t_ipc *queue_p, 
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
	
/** public data **/

/** private data **/
static t_ipc *ipc_array[MAX_NUM_Q];	/* contains pointers to queue structs 			*/
static ipc_idx;						/* points to the most resently created IPC object*/

/** private functions **/
/*******************************************************************************
 * Local debugging tools
 ******************************************************************************/

int proveConcistency(unsigned int qid) {
/* 	Se if you can find exactly as many blocked processes between out and in
	as there are tokens. */
	unsigned int in = ipc_array[qid]->in_idx;
	unsigned int out = ipc_array[qid]->out_idx;
	unsigned int count = 0;
	int i;
	
	if (ipc_array[qid]->token > 0)
		return(TRUE);
		
	for (i=out; i != in; i++, i %= MAX_BLOCKED_ON_Q) {
		if (
			ipc_array[qid]->blocked_procs[i]->state & _____Q__ ||
			ipc_array[qid]->blocked_procs[i]->state == READY
		)
				count++;
	}
	
	if (count != abs(ipc_array[qid]->token))
		return(FALSE);
	return(TRUE);	
}
#ifndef NDEBUG			 

int no_duplicateBlock(unsigned int qid, unsigned int mark) {
	tk_tcb_t *checkProc,*refProc = ipc_array[qid]->blocked_procs[mark];
	unsigned int in = ipc_array[qid]->in_idx;
	unsigned int out = ipc_array[qid]->out_idx;
	unsigned int i;

	for (i=out;i!=in;i++,i %= MAX_BLOCKED_ON_Q) {
		if (i != mark)
			if (refProc == ipc_array[qid]->blocked_procs[i])
				return(FALSE);
	}
	return(TRUE);
}

void p_bQf(int x,int y,unsigned int qid) {
	
	unsigned int mark_idx,count = 0;	
	BOOL rc;
	
	gotoxy(x,y);
	printf("                                                               ");
	gotoxy(x,y);
	rc = TRUE;
   
	if(ipc_array[qid]->token == 0)
		return;
	for(mark_idx=ipc_array[qid]->out_idx;mark_idx!=ipc_array[qid]->in_idx;mark_idx++,mark_idx %= MAX_BLOCKED_ON_Q) {
		printf("%s ",ipc_array[qid]->blocked_procs[mark_idx]->name);
		if (!(no_duplicateBlock(qid,mark_idx)))
			rc = FALSE;
		count++;
	} 
	printf("T=%d D=%d",ipc_array[qid]->token, abs(ipc_array[qid]->out_idx - ipc_array[qid]->in_idx) );
	printf("\n");
	
	//assert(abs(ipc_array[qid]->out_idx - ipc_array[qid]->in_idx) == abs(ipc_array[qid]->token) );
	
	assert(count==abs(ipc_array[qid]->token));
	assert(rc);
		
}
#endif
/*******************************************************************************
 * Local helper functions
 ******************************************************************************/
 /******************************************************************************
 *  FUNCTION NAME: uintDiff
 *
 *  PARAMETERS:
 *		x1, the integer suppused to be the lowest
 *		x2, the integer supposed to be the highest
 *		max, wrapparound value
 *
 *  DESCRIPTION:
 *		Calculates the diff between 2 integers taking in account wrapparound
 *		effect. Tested and works fine.
 *		
 *  RETURNS:
 *		diff
 *****************************************************************************/
unsigned long uintDiff(
	unsigned long x1, 
	unsigned long x2, 
	unsigned long max ) 
{
	return( x1 <= x2 ? x2-x1 : max - x1 + x2);
}

/*
 ** removeBlocked
 *
 *  FILENAME: D:\users\Mickey\projs\TinKer\tkos\tk_ipc.c
 *
 *  PARAMETERS:
 *
 *  DESCRIPTION: Helperfunction. In case IPC blocked function is released by 
 *               it needs to bee removed so that the blocked list doesen't
 *               get full.
 *
 *  RETURNS:
 *
 */

static void removeBlocked(t_ipc *queue_p, unsigned int idx) {
	unsigned int cp_idx;
	unsigned int in_idx;
	
	in_idx = queue_p->in_idx;
	in_idx--;
	if ( in_idx > MAX_BLOCKED_ON_Q) {
		in_idx = MAX_BLOCKED_ON_Q - 1;
	}
	
	/* Special case (quicker). Take away from start instead */
	if (queue_p->out_idx == idx) {
		queue_p->blocked_procs[idx] = NULL;
		queue_p->out_idx++;
		queue_p->out_idx %=	MAX_BLOCKED_ON_Q;
		queue_p->token++;
		return;
	}
	if (in_idx == idx) {
		queue_p->blocked_procs[idx] = NULL;
		queue_p->in_idx--;
		if ( queue_p->in_idx > MAX_BLOCKED_ON_Q) {
			queue_p->in_idx = MAX_BLOCKED_ON_Q - 1;
		}
		queue_p->token++;
		return;
	}

	cp_idx = idx + 1;
	cp_idx %= MAX_BLOCKED_ON_Q;
	
	for (; idx != queue_p->in_idx ; ) {
		queue_p->blocked_procs[idx]	= queue_p->blocked_procs[cp_idx];	
		cp_idx++;
		cp_idx %= MAX_BLOCKED_ON_Q;
		idx++;
		idx %= MAX_BLOCKED_ON_Q;
	}
	queue_p->in_idx--;
	if ( queue_p->in_idx > MAX_BLOCKED_ON_Q) {
		queue_p->in_idx = MAX_BLOCKED_ON_Q - 1;
	}
	queue_p->token++;
}

static unsigned int findNextEmpySlot() {
	unsigned int seekcounter = 0;
	
	while ( (ipc_array[ipc_idx] != NULL) && ( seekcounter < MAX_NUM_Q) ) {
		ipc_idx++;
		ipc_idx %= MAX_NUM_Q;
		seekcounter ++;
	}
   return (seekcounter == MAX_NUM_Q) ? ERR_OBJFULL : ERR_OK;	
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
		/* Wow, gone tru */
		ipc_array[qid]->token --;
		return(ERR_OK);
	}else {
		/* Hrrm, someone else was here before me */
		/* Can I block? */
		if (abs(ipc_array[qid]->token) < (MAX_BLOCKED_ON_Q - 1) ) {
			ipc_array[qid]->token--;	
		}else
			return(ERR_BLOCKLIMIT);
			
		/* Put myself on the fifo list */
		ipc_array[qid]->blocked_procs[ipc_array[qid]->in_idx] = MySelf;
		ipc_array[qid]->in_idx++;
		ipc_array[qid]->in_idx %= MAX_BLOCKED_ON_Q;
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
				if (mark_idx > MAX_BLOCKED_ON_Q) {
					mark_idx = MAX_BLOCKED_ON_Q - 1;	
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
						ipc_array[qid]->blocked_procs[mark_idx]->wakeupEvent = E_IPC2;
						/*Remove it from blocked list */
						removeBlocked(ipc_array[qid],mark_idx);	
					}
					if (mark_idx == ipc_array[qid]->in_idx)
						break;
					mark_idx++; mark_idx %= MAX_BLOCKED_ON_Q;
				}
				while(ipc_array[qid]->token < 0 && mark_idx != ipc_array[qid]->in_idx);
				p_bQ(40,21,qid);
				return(ERR_TIMEOUT);
				
			}else if (MySelf->wakeupEvent == E_IPC2)
				return(ERR_TIMEOUT);
			return(ERR_OK);
		}
	}	
}

/******************************************************************************
 *  FUNCTION NAME: unlock_stage
 *
 *  PARAMETERS:
 *		IPC object identity.
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
				i++,i %= MAX_BLOCKED_ON_Q)
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
				/*printf("Hubba i örat\n");
				while(1);*/
				return(ERR_OK);
				
			}
							
			/*Find the highest waiting process*/ 
			for (
				i=ipc_array[qid]->out_idx;
				i!=ipc_array[qid]->in_idx;
				i++,i %= MAX_BLOCKED_ON_Q)
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
			ipc_array[qid]->blocked_procs[t_idx]->wakeupEvent = E_IPC;
			//ipc_array[qid]->token++;
			removeBlocked(ipc_array[qid],t_idx);	
			tk_yield(); 
			
		}else {
			do {
				Him = ipc_array[qid]->blocked_procs[ipc_array[qid]->out_idx];
				ipc_array[qid]->out_idx++,
				ipc_array[qid]->out_idx %= MAX_BLOCKED_ON_Q;
				ipc_array[qid]->token++;
			}while(!(Him->state & _____Q__) && ipc_array[qid]->token < 0);
			 /*Above can happen if made ready by timer */								   
			 /*but has not runned (had the oppotunity */								   
			 /*to remove from this list)*/								   
			 /*Should be obsolite now, but is not*/
			Him->state = ( (Him->state) & ~_____QST); /* In case of timeout active release that to */
			Him->wakeupEvent = E_IPC;
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
      if (abs(ipc_array[qid]->token) < (MAX_BLOCKED_ON_Q - 1) ) {
         ipc_array[qid]->token--;   
      }else
         return(ERR_BLOCKLIMIT);
         
      /* Put myself on the fifo list */
      ipc_array[qid]->blocked_procs[ipc_array[qid]->in_idx] = MySelf;
      ipc_array[qid]->in_idx++;
      ipc_array[qid]->in_idx %= MAX_BLOCKED_ON_Q;
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
            if (mark_idx > MAX_BLOCKED_ON_Q) {
               mark_idx = MAX_BLOCKED_ON_Q - 1; 
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
                  ipc_array[qid]->blocked_procs[mark_idx]->wakeupEvent = E_IPC2;
                  /*Remove it from blocked list */
                  removeBlocked(ipc_array[qid],mark_idx);  
               }
               if (mark_idx == ipc_array[qid]->in_idx)
                  break;
               mark_idx++; mark_idx %= MAX_BLOCKED_ON_Q;
            }
            while(ipc_array[qid]->token < 0 && mark_idx != ipc_array[qid]->in_idx);
            p_bQ(40,21,qid);
            return(ERR_TIMEOUT);
            
         }else if (MySelf->wakeupEvent == E_IPC2)
            return(ERR_TIMEOUT);
         return(ERR_OK);
      }
   }  
}

/******************************************************************************
 *  FUNCTION NAME: _unlock_stage_ny
 *
 *  PARAMETERS:
 *    IPC object identity.
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
            i++,i %= MAX_BLOCKED_ON_Q)
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
            /*printf("Hubba i örat\n");
            while(1);*/
            return(ERR_OK);
            
         }
                     
         /*Find the highest waiting process*/ 
         for (
            i=ipc_array[qid]->out_idx;
            i!=ipc_array[qid]->in_idx;
            i++,i %= MAX_BLOCKED_ON_Q)
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
         ipc_array[qid]->blocked_procs[t_idx]->wakeupEvent = E_IPC;
         //ipc_array[qid]->token++;
         removeBlocked(ipc_array[qid],t_idx);  
         //tk_yield(); 
         
      }else {
         do {
            Him = ipc_array[qid]->blocked_procs[ipc_array[qid]->out_idx];
            ipc_array[qid]->out_idx++,
            ipc_array[qid]->out_idx %= MAX_BLOCKED_ON_Q;
            ipc_array[qid]->token++;
         }while(!(Him->state & _____Q__) && ipc_array[qid]->token < 0);
          /*Above can happen if made ready by timer */                           
          /*but has not runned (had the oppotunity */                         
          /*to remove from this list)*/                           
          /*Should be obsolite now, but is not*/
         Him->state = ( (Him->state) & ~_____QST); /* In case of timeout active release that to */
         Him->wakeupEvent = E_IPC;
         //tk_yield(); /*In case the one you've just released has higher prio, run it*/   
      }
   }else               
      ipc_array[qid]->token++;   /* Increase number of tokens anyway */    
   return(ERR_OK);               
}

/*************************************************************************************************************
 * Public functions
 ************************************************************************************************************ */
void createIPC( void ){
    
    unsigned int i;
	
	ipc_idx = 0;
	
	for (i=0; i<MAX_NUM_Q; i++) {
		ipc_array[i] = NULL;
	}
    	
}

void deleteIPC( void ){
	unsigned long i,j; 
	
	/* If for any reason any queue is left unallocated, try to free its */
	/* allocated memory */
	for (i=0; i<MAX_NUM_Q; i++) {
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
}

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
	printf("%s \n",msg_buf);
	printf("%s \n",ipc_array[qid]->m.qv[ipc_array[ipc_array[qid]->mout_idx]->mout_idx].mb);*/
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
		if (!(ipc_array[qid]->token < ipc_array[qid]->sizeof_q))
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
	//printf("%s \n",ipc_array[qid]->m.qv[ipc_array[ipc_array[qid]->mout_idx]->mout_idx].mb);

	
	rc = unlock_stage(qid);
	if (!(proveConcistency(qid)))
		assert(0);
	if (rc !=  ERR_OK)
		return(rc);
	return(ERR_OK);
}

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

unsigned long q_delete(
	unsigned long qid       /* id */
) {
	unsigned long rc;
	
	free(ipc_array[qid]->m.q);
	rc = sm_delete(qid);
	return (rc);
}

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
		if (!(ipc_array[qid]->token < ipc_array[qid]->sizeof_q))
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
	}else if ((ipc_array[ipc_idx] = (t_ipc*)malloc(sizeof(t_ipc)) ) == NULL) {
		return(ERR_NOMEM);
	/* Allocate memory for pointertable of tk_tcb_t pointers */
	}else if ((ipc_array[ipc_idx]->blocked_procs = (tk_tcb_t**)malloc(MAX_BLOCKED_ON_Q * sizeof(tk_tcb_t*)) ) == NULL) {
			return(ERR_NOMEM);
	}
	
	/* Clean pointertable of blocked procs*/
	for (i=0; i<MAX_BLOCKED_ON_Q; i++) {
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

unsigned long sm_delete(
	unsigned long qid       /* id */
) {
	unsigned int i;																		  
	/* Check if IPC exists */
	/* Check if anyone is blocked (those are not freed but zombified)*/
	free(ipc_array[qid]->blocked_procs);
	free(ipc_array[qid]);
	ipc_array[qid] = NULL;
	return (ERR_OK);
}

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

unsigned long sm_v(			/* sm_send or sm_put */
   unsigned long qid		/* id */
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
   printf("%s \n",msg_buf);
   printf("%s \n",ipc_array[qid]->m.qv[ipc_array[ipc_array[qid]->mout_idx]->mout_idx].mb);*/
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
      if (!(ipc_array[qid]->token < ipc_array[qid]->sizeof_q))
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
   //printf("%s \n",ipc_array[qid]->m.qv[ipc_array[ipc_array[qid]->mout_idx]->mout_idx].mb);

   
   rc = _unlock_stage_ny(qid);
   if (!(proveConcistency(qid)))
      assert(0);
   if (rc !=  ERR_OK)
      return(rc);
   return(ERR_OK);
}

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

unsigned long q_delete_ny(
   unsigned long qid       /* id */
) {
   unsigned long rc;
   
   free(ipc_array[qid]->m.q);
   rc = sm_delete(qid);
   return (rc);
}

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
      if (!(ipc_array[qid]->token < ipc_array[qid]->sizeof_q))
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
   }else if ((ipc_array[ipc_idx] = (t_ipc*)malloc(sizeof(t_ipc)) ) == NULL) {
      return(ERR_NOMEM);
   /* Allocate memory for pointertable of tk_tcb_t pointers */
   }else if ((ipc_array[ipc_idx]->blocked_procs = (tk_tcb_t**)malloc(MAX_BLOCKED_ON_Q * sizeof(tk_tcb_t*)) ) == NULL) {
         return(ERR_NOMEM);
   }
   
   /* Clean pointertable of blocked procs*/
   for (i=0; i<MAX_BLOCKED_ON_Q; i++) {
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

unsigned long sm_delete_ny(
   unsigned long qid       /* id */
) {
   unsigned int i;                                                        
   /* Check if IPC exists */
   /* Check if anyone is blocked (those are not freed but zombified)*/
   free(ipc_array[qid]->blocked_procs);
   free(ipc_array[qid]);
   ipc_array[qid] = NULL;
   return (ERR_OK);
}

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

#endif




   
/*******************************************************************  
 *
 *  $Log: tk_itc.c,v $
 *  Revision 1.8  2005-12-01 13:05:25  ambrmi09
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

