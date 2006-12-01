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
@ingroup POSIX_RT

@brief RT queues - POSIX 1003.1b API

\code
This file was donated courtesy of the Zoi project (year 2000, 
SIEMENS Elema). It was originally written as a wrapper for
pThreads-win32, to give Windows threads the ability of POSIX RT queues.
\endcode


For in-depth discussions about this component, see \ref
POSIX_RT

@see POSIX_RT
@see PTHREAD

@todo Consider replacing qsort with insert sort (or at least a non-reqursive version)
*/


/*****************************************************************************
 * include files
 *****************************************************************************/
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>

//#ifdef _MSVC_
#if defined(_WIN32) &&  defined(_MSC_VER)
   #include <errno.h>
   #include <search.h>
   #define  PATH_MAX    255
#elif defined(__CYGWIN32__) || defined(__CYGWIN__)
   #include <sys/errno.h>
#else   //Embedded TinKer config
   #include <errno.h>
   #include <tk_ansi.h>         //Normally qsort is part of stdlib. But on embedded tool-chains this is often missing.   
   #define  PATH_MAX    24   
#endif

#define _MQ_NO_WARN_VAR(x) ((void)x)  //!< Used silence warnings about unused variables



/*****************************************************************************
 * local definitions
 *****************************************************************************/
#define NUMBER_OF_QUEUES   100
#define NUMBER_OF_MESSAGES 100
#define NUMBER_OF_FILES    100
#define CREATE_ONLY        (NUMBER_OF_FILES + 1)

/* Calculates number of messages in queue */
#define NUMB_MESS(Q) ((Q->mBox.mIdxIn>=Q->mBox.mIdxOut)? \
   Q->mBox.mIdxIn - Q->mBox.mIdxOut: \
   Q->mq_attr.mq_maxmsg - Q->mBox.mIdxOut + Q->mBox.mIdxIn \
)

typedef struct{
   time_t               time;
   unsigned int         prio;
   unsigned int         inOrder;
}OrderT;

typedef struct{
   char                *buffer; /* Buffer where message is stored */
   int                  msgSz;  /* Length of message when sent */
   OrderT               order;  /* Based on timestamp and mess prio */   
}MessT;

typedef struct {
   size_t               mIdxIn; /*The index for writing */
   size_t               mIdxOut;/*The index for reading */  
   MessT               *messArray;   
   unsigned int         lastInOrder; /*together with timestamp = unique */
}MBox;

typedef struct{
   int                   taken;
   char                  mq_name[PATH_MAX];
   struct mq_attr        mq_attr;
   MBox                  mBox;
   sem_t                 sem;
   /*Todo: mode (access mode) */
}QueueD;

typedef struct {     /*File descriptor */
   int                   taken;
   pthread_t             tId;    /* Belongs to task */
   mode_t                oflags; /* Open for mode   */
   int                   qId;    /* Attachec queue  */

}FileD;

/*****************************************************************************
 * private data
 *****************************************************************************/
static QueueD  queuePool[ NUMBER_OF_QUEUES ];
static int     qPIdx = 0; /* Todo: Make this roundtrip */

static FileD   filePool[ NUMBER_OF_FILES ];
static int     fPIdx = 0; /* Todo: Make this roundtrip */

static sem_t   poolAccessSem;
static pthread_once_t mq_once= PTHREAD_ONCE_INIT;
	      
/*****************************************************************************
 * private function declarations
 *****************************************************************************/
static void initialize( void );
static void sortByPrio( QueueD *Q );
/*Comp. func for qsort */
static int compMess( const void *elem1, const void *elem2 ); 

/*****************************************************************************
 * public function implementations
 *****************************************************************************/

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
@see http://www.opengroup.org/onlinepubs/009695399/functions/mq_open.html
*/
/*!
@see http://www.opengroup.org/onlinepubs/009695399/functions/mq_close.html
*/
int mq_close(
   mqd_t                 mq
){
   pthread_once(&mq_once, initialize);
   _MQ_NO_WARN_VAR(mq);
   assert(sem_wait(&poolAccessSem) == 0);
   
   assert(sem_post(&poolAccessSem) == 0);

   return 0;
}


//------1---------2---------3---------4---------5---------6---------7---------8
/*!
@see http://www.opengroup.org/onlinepubs/009695399/functions/mq_getattr.html
*/
int mq_getattr(
   mqd_t                mq,
   struct mq_attr      *attrbuf
){
   pthread_once(&mq_once, initialize);
   _MQ_NO_WARN_VAR(mq);
   _MQ_NO_WARN_VAR(attrbuf);
   assert(sem_wait(&poolAccessSem) == 0);
   /*Find next empy slot in qPool*/
   
   assert(sem_post(&poolAccessSem) == 0);

   return 0;
}

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
@see http://www.opengroup.org/onlinepubs/009695399/functions/mq_open.html
*/
mqd_t mq_open(
   const char           *mq_name,
   int                   oflags,
   mode_t                mode,   /* Used for permissions. Is ignored ATM */
   struct mq_attr       *mq_attr
){
   int                   i,j;
   size_t                k;
   int                   qId;   /* queue ID in message pool */
   int                   dId;   /* descriptor */
   
   pthread_once(&mq_once, initialize);
   _MQ_NO_WARN_VAR(mode);
   
   assert(sem_wait(&poolAccessSem) == 0);
   
   /*First test some harmless common things */
   if (strlen(mq_name)>PATH_MAX ) { /* Name too long */
      errno = ENAMETOOLONG;   
      assert(sem_post(&poolAccessSem) == 0);
      return(-1);   
   }

   if (mq_name[0] != '/' ) { /* Invalid name of queue */
      errno = EINVAL;   
      assert(sem_post(&poolAccessSem) == 0);
      return(-1);   
   }
   
   if (oflags & O_CREAT) {     /*Wishes to create new queue */
      int reuse = 0;

      /* Seek for new empty slot in queue pool */
      for (i=qPIdx,j=0; j<NUMBER_OF_QUEUES && queuePool[i].taken != 0; j++) {
         i++;
         i%=NUMBER_OF_QUEUES;
      }
   
      if (j==NUMBER_OF_QUEUES) { /* No empty slot found */
         errno = EMFILE;   
         assert(sem_post(&poolAccessSem) == 0);
         return(-1);   
      }
      qId = j; /* slot identifier */
      queuePool[i].taken = 1;

      /* Scan the pool to se if duplicate name exists */
      for (i=0; i<NUMBER_OF_QUEUES; i++) {
         if (queuePool[i].taken){
            if (strncmp( queuePool[i].mq_name, mq_name, PATH_MAX) == 0){
               /* Duplicate name exists */
               if (oflags & O_CREAT) {
                  /*Exclusive ?*/
                  assert(sem_post(&poolAccessSem) == 0);
                  errno =  EEXIST;
                  return(-1);
               } else {
                  /* Attach to existing queue, don't create*/
                  /* Silently reuse... */
                  qId = i;
                  reuse = 1;
               }
            }
         }
      }      

      /* So far so good. Create the queue */
      if (!reuse) {
         queuePool[qId].taken = 1;
         strncpy( queuePool[qId].mq_name, mq_name, PATH_MAX  );
   
         /* Create default attributes (TBD) */
         /*
         Todo: ... queuePool[qId].mode->xx = xx 

         Mean-while, assure it exists!
         */
         if (mq_attr == NULL){
            errno = EINVAL;
            return(-1);
         }
         
         /*Todo: doesn't work.. why?*/
         /*memcpy( &queuePool[qId].mq_attr, mq_attr, sizeof(struct mq_attr)); strange...*/
         queuePool[qId].mq_attr.mq_maxmsg = mq_attr->mq_maxmsg;
         queuePool[qId].mq_attr.mq_msgsize = mq_attr->mq_msgsize;

         /* Create the message array */
         queuePool[qId].mBox.messArray = (MessT*)malloc((mq_attr->mq_maxmsg) * sizeof(MessT));
         /* Create every message buffer and put it in array */
         if (queuePool[qId].mBox.messArray == NULL) {
            /*No more memory left on heap */
            assert(sem_post(&poolAccessSem) == 0);
            errno =  ENFILE;
            return(-1);
         }
         
         for (k=0; k<mq_attr->mq_maxmsg; k++){
            queuePool[qId].mBox.messArray[k].buffer = (char*)malloc(mq_attr->mq_msgsize * sizeof(char));
            if (queuePool[qId].mBox.messArray[k].buffer == NULL){
               
               /*No more memory left on heap */
               /*Tidy up..*/
               for (j=k; j>=0; j--){
                  free(queuePool[qId].mBox.messArray[j].buffer);
               }
               free(queuePool[qId].mBox.messArray);

               assert(sem_post(&poolAccessSem) == 0);
               errno =  ENFILE;
               return(-1);
            }
                        
            queuePool[qId].mBox.messArray[k].msgSz = 0; 
            queuePool[qId].mBox.messArray[k].order.inOrder = 0; 
            queuePool[qId].mBox.messArray[k].order.prio    = 0; /*Tada...!*/
            queuePool[qId].mBox.messArray[k].order.time    = 0; 
            queuePool[qId].mBox.lastInOrder = 0;   
         }
         
         queuePool[qId].mBox.mIdxIn = 0; /*No pending messages */
         queuePool[qId].mBox.mIdxOut = 0;
         
         /* initialize the queues semaphore */
         if (!(oflags & O_NONBLOCK))
            assert(sem_init (&queuePool[qId].sem, 0, 0) == 0);
      }
   }
   
   /* Attach queue handle to file handle */
   if (oflags & (O_RDONLY | O_WRONLY | O_RDWR) ) { /* Want to use queue */
      if (!(oflags & O_CREAT)) { /*Hasn't been created this time. Find it!*/
         for (i=0; i<NUMBER_OF_QUEUES; i++) {
            if (queuePool[i].taken)
               if (strncmp( queuePool[i].mq_name, mq_name, PATH_MAX) == 0)
                  break;
         }
         if ( i == NUMBER_OF_QUEUES ) {/* Not found ... */
            assert(sem_post(&poolAccessSem) == 0);
            errno =  ENOENT;
            return(-1);
         }
         qId = i;
      }/* else: The qId is valid allready */

      
      /* Attach the pool handle to the file descriptor */
      /* Seek for new empty slot in file pool */
      for (i=fPIdx,j=0; j<NUMBER_OF_FILES && filePool[i].taken != 0; j++) {
         i++;
         i%=NUMBER_OF_FILES;
      }
   
      if (j==NUMBER_OF_FILES) { /* No empty slot found */
         errno = EMFILE;   
         assert(sem_post(&poolAccessSem) == 0);
         return(-1);   
      }
      dId = j; /* slot identifier */
      filePool[i].taken = 1;
      filePool[i].qId = qId;
      filePool[i].oflags = oflags;
      filePool[i].tId = pthread_self();

   }else{
      dId =  CREATE_ONLY;  /* Not valid file handle, Created only */
   }

   assert(sem_post( &poolAccessSem) == 0);
   return(dId);  
}

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
@see http://www.opengroup.org/onlinepubs/009695399/functions/mq_receive.html
*/
size_t mq_receive(
   mqd_t                 mq,
   char                 *msg_buffer,
   size_t                buflen,
   unsigned int         *msgprio
){
   QueueD   *Q;
   size_t   msgSize;

   pthread_once(&mq_once, initialize);
   assert(sem_wait(&poolAccessSem) == 0);
   
   /*Is valid filehandle?*/
 
   if (!(
      ((mq>=0) && (mq<NUMBER_OF_FILES)) &&
      ( filePool[mq].taken == 1 ) &&
      ( filePool[mq].tId == pthread_self())
   )){
      assert(sem_post(&poolAccessSem) == 0);
      errno =  EBADF;
      return(-1);
   }

   if (!( filePool[mq].oflags & (O_RDONLY | O_RDWR))){
      assert(sem_post(&poolAccessSem) == 0);
      errno =  EBADF;
      return(-1);
   }


   Q = &queuePool[filePool[mq].qId];

   if (Q->taken != 1){
      assert(sem_post(&poolAccessSem) == 0);
      errno =  EBADF;
      return(-1);
   }
   
   /* Check if message fits */
   if (!(buflen >= Q->mq_attr.mq_msgsize)){
      assert(sem_post(&poolAccessSem) == 0);
      errno = EMSGSIZE;
      return(-1);
   }
   
   if ( NUMB_MESS(Q) <= 0 ) {
      if (filePool[mq].oflags & O_NONBLOCK){
         assert(sem_post(&poolAccessSem) == 0);
         errno =  EAGAIN; 
         return(-1);
      }
   }

   /* OK so far */
   /* Release poolaccess */
   assert(sem_post(&poolAccessSem) == 0);
   /* Will block if nescessary */
   assert(sem_wait(&Q->sem) == 0);
      
   memcpy( 
      msg_buffer, 
      Q->mBox.messArray[Q->mBox.mIdxOut].buffer, 
      Q->mBox.messArray[Q->mBox.mIdxOut].msgSz
   );   
   if (msgprio)  //Special case if atribute is NULL. Check what standard says about that
      *msgprio = Q->mBox.messArray[Q->mBox.mIdxIn].order.prio;

   msgSize = Q->mBox.messArray[Q->mBox.mIdxOut].msgSz; 

   Q->mBox.mIdxOut++;
   Q->mBox.mIdxOut %= Q->mq_attr.mq_maxmsg;

   return(msgSize);
}

//------1---------2---------3---------4---------5---------6---------7---------8
/*!
@see http://www.opengroup.org/onlinepubs/009695399/functions/mq_setattr.html
*/
int mq_setattr(
   mqd_t                 mqdes,
   const struct mq_attr *new_attrs,
   struct mq_attr       *old_attrs
){
   pthread_once(&mq_once, initialize);
   _MQ_NO_WARN_VAR(mqdes);
   _MQ_NO_WARN_VAR(new_attrs);
   _MQ_NO_WARN_VAR(old_attrs);
   assert(sem_wait(&poolAccessSem) == 0);
   
   assert(sem_post(&poolAccessSem) == 0);

   return 0;
}


//------1---------2---------3---------4---------5---------6---------7---------8
/*!
@see http://www.opengroup.org/onlinepubs/009695399/functions/mq_send.html
*/
int mq_send(
   mqd_t             mq,
   const char       *msg,
   size_t            msglen,
   unsigned int      msgprio
){
   QueueD   *Q;
   time_t   ttime;

   pthread_once(&mq_once, initialize);
   assert(sem_wait(&poolAccessSem) == 0);
   
   /*Is valid filehandle?*/
 
   if (!(
      ((mq>=0) && (mq<NUMBER_OF_FILES)) &&
      ( filePool[mq].taken == 1 ) &&
      ( filePool[mq].tId == pthread_self())
   )){
      assert(sem_post(&poolAccessSem) == 0);
      errno =  EBADF;
      return(-1);
   }

   if (!( filePool[mq].oflags & (O_WRONLY | O_RDWR))){
      assert(sem_post(&poolAccessSem) == 0);
      errno =  EBADF;
      return(-1);
   }


   Q = &queuePool[filePool[mq].qId];

   if (Q->taken != 1){
      assert(sem_post(&poolAccessSem) == 0);
      errno =  EBADF;
      return(-1);
   }
   
   /* Check if message fits */
   if (!(Q->mq_attr.mq_msgsize >= msglen)){
      assert(sem_post(&poolAccessSem) == 0);
      errno =  EMSGSIZE;
      return(-1);
   }
   
   if (!(NUMB_MESS(Q) < Q->mq_attr.mq_maxmsg-1)){ /* queue is full*/
                                                  /* Save 1, else sort fucks up*/
      /* Todo: never bocks (yet) */
      if (filePool[mq].oflags & O_NONBLOCK){    /* to block or not to block*/
         assert(sem_post(&poolAccessSem) == 0);
         errno =  EAGAIN; /* Todo: block on send full */
         return(-1);
      }else{
         assert(sem_post(&poolAccessSem) == 0);
         errno =  EAGAIN; /* Todo: block on send full */
         return(-1);
      }
   }
      
   /*OK so far*/
   
   memcpy( Q->mBox.messArray[Q->mBox.mIdxIn].buffer, msg, msglen);

   Q->mBox.messArray[Q->mBox.mIdxIn].msgSz = msglen;
   
   Q->mBox.messArray[Q->mBox.mIdxIn].order.prio = msgprio;
   Q->mBox.messArray[Q->mBox.mIdxIn].order.inOrder =  
      Q->mBox.lastInOrder++;
   Q->mBox.messArray[Q->mBox.mIdxIn].order.time = time(&ttime);

   Q->mBox.mIdxIn++;
   Q->mBox.mIdxIn %= Q->mq_attr.mq_maxmsg;

   sortByPrio( Q );

   assert(sem_post(&Q->sem) == 0);

   assert(sem_post(&poolAccessSem) == 0);

   return(0);
}

/*****************************************************************************
 * private function implementations
 *****************************************************************************/

/*****************************************************************************
 * FUNCTION NAME:
 *  
 *
 * DESCRIPTION:
 *
 *  
 * NOTES:
 *  
 *****************************************************************************/
static void initialize( void ) {
   int i;

   /* Make atomic */
   assert(sem_init (&poolAccessSem, 0, 1) == 0);
   /* Make atomic */

   assert(sem_wait(&poolAccessSem) == 0);
   for (i=0; i<NUMBER_OF_QUEUES; i++) {
	   queuePool[i].taken = 0;
   }
   for (i=0; i<NUMBER_OF_FILES; i++) {
	   filePool[i].taken = 0;
   }
   assert(sem_post(&poolAccessSem) == 0);
}

/*****************************************************************************
 * FUNCTION NAME: compMess
 *  
 *
 * DESCRIPTION: Compare function for quick sort. Sorts by prio, timestamp,
 *              incom order.
 *
 *  
 * NOTES:
 *  
 *****************************************************************************/
static int compMess(
   const void *elem1, 
   const void *elem2 
){
   MessT            *e1 = (MessT*)elem1;
   MessT            *e2 = (MessT*)elem2;
   
   /* Per prio */
   if (e1->order.prio > e2->order.prio)
      return(-1);
   if (e1->order.prio < e2->order.prio)
      return(1);
   /* Per timestamp (resolution is 1 sek) */
   if (e1->order.time < e2->order.time)
      return(-1);
   if (e1->order.time > e2->order.time)
      return(1);
   /* Per order of sending */
   if (e1->order.inOrder < e2->order.inOrder)
      return(-1);
   if (e1->order.inOrder > e2->order.inOrder)
      return(1);
   return(0); /*Should never happen*/
}

//------1---------2---------3---------4---------5---------6---------7---------8
static void sortByPrio( QueueD *Q ){
   int szMess = NUMB_MESS(Q);
   int i,j;
   
   
   if (Q->mBox.mIdxOut <= Q->mBox.mIdxIn){    
      qsort( 
         &Q->mBox.messArray[Q->mBox.mIdxOut], 
         szMess, 
         sizeof(MessT), 
         compMess 
      );   
   }else{
      MessT *tempT;
      
      tempT = (MessT*)malloc( szMess * sizeof(MessT));
      /* Copy the content from queue into temp storage */
      i = Q->mBox.mIdxOut;
      /*i --;
      i %= Q->mq_attr.mq_maxmsg;*/
      for (j=0; j<szMess; j++){
         tempT[j] = Q->mBox.messArray[i]; 
         i++;
         i %= Q->mq_attr.mq_maxmsg;
      }
      
      qsort( 
         tempT, 
         szMess, 
         sizeof(MessT), 
         compMess 
      );

      /* Copy back */
      i = Q->mBox.mIdxOut;
      /*i --;
      i %= Q->mq_attr.mq_maxmsg;*/
      for (j=0; j<szMess; j++){
         Q->mBox.messArray[i] = tempT[j];
         i++;
         i %= Q->mq_attr.mq_maxmsg;
      }

      free(tempT);
   }

}

/*!
Just a stub to get test-code through compiler. Should be easy enoygh to implement though
*/
int mq_unlink(
   const char *mq_name
){
   _MQ_NO_WARN_VAR(mq_name);
   return 0;
}


//      /*This is a tricky bastard.. 2 parts*/
//      k = Q->mq_attr.mq_maxmsg - Q->mBox.mIdxOut;
//      j = szMess - k;
//      l = Q->mBox.mIdxOut - Q->mBox.mIdxOut;
//      for (i=0; i<j; i++){
//         Q->mBox.messArray[k-i] = Q->mBox.messArray[k-i+j];
//      }

//      for (i=0; i<k; i++){
//         Q->mBox.messArray[i] = Q->mBox.messArray[i+l];
//      }
//      /* Reindexate */
//      Q->mBox.mIdxOut = 0;
//      Q->mBox.mIdxIn = szMess;
//      /* Finally sort */
//      qsort( 
//         Q->mBox.messArray, 
//         szMess, 
//         sizeof(MessT), 
//         compMess 
//      );


/** @defgroup POSIX_RT POSIX_RT: POSIX 1003.1b API
@ingroup PTHREAD
@brief RT queues and semaphores - POSIX 1003.1b API

<em>*Documentation and implementation in progress*</em>

Good references about the API:
@see http://www.opengroup.org/onlinepubs/009695399/idx/im.html (look for functions starting with mq_)
@see http://www.opengroup.org/onlinepubs/009695399/idx/is.html (look for functions starting with sem_)

<p><b>Go gack to</b> \ref COMPONENTS</p>

*/


/*!
 * @defgroup CVSLOG_mqueue_c mqueue_c
 * @ingroup CVSLOG
 *  $Log: mqueue.c,v $
 *  Revision 1.19  2006-12-01 10:58:51  ambrmi09
 *  Solves #1605911 #1605893
 *
 *  Revision 1.18  2006/11/27 22:29:23  ambrmi09
 *  Minor djustments completeing the move of some header files to public and due
 *  to some name clashed with user space naming conventions.
 *
 *  Revision 1.17  2006/04/08 10:16:00  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.16.2.1  2006/04/03 20:07:26  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.16  2006/03/27 13:40:15  ambrmi09
 *  As part of the preparation for the first release, code has been cleaned up a little
 *  and project has been checked that it will build on all it's intended targets.
 *
 *  Problems that remained had to do the ANSI wrapping.
 *
 *  Some modifications were neserary to make the BC5 build, but the result is cleaner
 *  and more consistent with the rest of the wrapping. As a consequence, stdlib.h was
 *  introduced.
 *
 *  Revision 1.15  2006/03/24 18:23:43  ambrmi09
 *  Another turn of cosmetics
 *
 *  Revision 1.14  2006/03/17 14:18:42  ambrmi09
 *  Finalized pThreads and RT gueues for GNU build-chain
 *
 *  Revision 1.13  2006/03/17 12:20:03  ambrmi09
 *  Major uppdate (5 days hard work)
 *
 *  - Finally tied up all loose ends in the concept. Threads are now
 *  joinable
 *
 *  - Corrected one error: compacting scheduele while cancelling a
 *  threads
 *
 *  - Several new API, mainly concerned with cancelation (corrsp pThread
 *  also)
 *
 *  - Found a nasty bug while creating threads in threads for XC167. TOS is
 *  really a patchy solution ;( This one had to do with the compiler
 *  being fooled by the inline assembly and optimized something that was not
 *  optimizable (saving stack segment got wacked).
 *
 *  - Designed a concurrent qsort test-app. This is good for showing
 *  boss-worker model. Number of threads recoed on XC167 was 50 and on MSVS
 *  more than 150! Interesting to notice was that TinKer creation and
 *  cancelation for threads was much faster than Windows own (20-30 times
 *  faster).
 *
 *  - A MSVC workspace for pThreads-Win32. Good for testing apps
 *  transparency.
 *
 *  - Increased memory on XC167 (phyCore HW). now 32k for stacks and 16k for
 *  malloc. We still lack RAM that is not deployed (pHycore has
 *  128k + 256k physical RAM memory i think). Maximum for
 *  stack is 64k however (type of pointers determine this). If memory is
 *  increased further, we get a TRAP_B saying bad memory interface. Typical
 *  error for config memory issues in DaVe.
 *
 *  Revision 1.12  2006/03/12 15:08:54  ambrmi09
 *  - Adjusted the source to accomodate the new file structure.
 *
 *  - All build environments uppdated and verified except BC5. For this one
 *  we stumbled across the header-file issue that I've been fearing. Seems
 *  we need to take care of that one after all.
 *
 *  @note The malloc bug still not solved.
 *
 *  Revision 1.11  2006/03/05 11:11:27  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.10  2006/03/02 14:05:49  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.9  2006/02/28 18:16:55  ambrmi09
 *  - Mainly a ci for the new Workspace structure
 *  - Houwever, found and corrected a bug in mqueue.c (a NULL pointer
 *    assignement)
 *
 *  Revision 1.8  2006/02/23 11:34:59  ambrmi09
 *  - Improved post mortem
 *   - Fixed bug in i2hex2.
 *   - Added uptime output
 *   - mark running among RDY threads in schedule dump
 *
 *  - \ref putchar now supports easy switching between serial0 and serial1
 *
 *  mqueue.h and mqueue.c should be untouched. But trying to identify if
 *  qsort is the reason for TinKer sometimes to hang (recent possible bug).
 *
 *  Revision 1.7  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 * Revision 1.6  2006/02/21 22:10:32  ambrmi09
 * - Added wrapper macro for pthread_create so that posix threads get named in
 *   TinKer (makes post-mortem easier). Very cool solution with a macro...
 * - Improved post-mortem, the schedule gets dumpt also now
 * - Wrapper macros for msleep and usleep (temporary)
 * - Minor stubbing and wrapping of mq_unlink and pthread_cancel
 * - Added a new test program (t est-posix.c ). This is verifyed to compile and
 *   run on both Linux and TinKer unmodified!
 *
 * Revision 1.5  2006/02/20 19:17:14  ambrmi09
 * - Made the errno variable thread specific (each thread has it's own)
 * - Hid the details of using errno so that setting and reading it looks
 *   like using a normal variable
 * - Extracted some stuff from tk.h that doesn't need to be public
 * - Implemented perros and strerror including a storage with all the error
 *   strings (will go into NV ROM on a embedded system).
 *
 * Revision 1.4  2006/02/20 15:22:00  ambrmi09
 * Documentation stuff. No code changes.
 *
 * Revision 1.3  2006/02/19 22:00:38  ambrmi09
 * Major brake-through!!! First working attempt with crude pThreads and
 * POSIX RT queues works. (jihaa) :=D. Wow
 *
 * Revision 1.2  2006/02/19 12:44:33  ambrmi09
 * - Documented ITC
 * - Started to build up the structure for the \ref PTHREAD component
 *
 * Revision 1.1  2006/02/17 21:14:54  ambrmi09
 * Initial commit: code donated by the Zoi project (author: Michael Ambrus)
 *
 */
