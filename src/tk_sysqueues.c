/*******************************************************************
 *
 *  DESCRIPTION: tk_sysqueues.c 
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:    
 *
 *  Current $Revision: 1.1 $
 *******************************************************************/

/** include files **/

#include <tk_ipc.h>
#include <tk_sysqueues.h>

#include <stdlib.h>
#include <string.h>


/* default settings */

#define SYSQ_LENGT 10   //!< Lengt of each Q. Small number, but should be OK

/** external functions **/

/** external data **/

/** public data **/
unsigned long tk_sys_queues[TK_NUMBER_OF_SYSQ];

/** internal functions **/
unsigned long _tk_create_system_queues( ){
   int i;
   unsigned long rc = ERR_OK;
   char name[4]; // Queue name is deep-copied -> temp is ok to have on stack
   
   for (i=0; (i<TK_NUMBER_OF_SYSQ) && (rc==ERR_OK); i++){

      strncpy(name,"SQ",4);
      if (i>10){
         name[2] = 48 + i/10;
         name[3] = 48 + i%10;
      }
      rc = q_create_ny(name,SYSQ_LENGT,FIFO,&tk_sys_queues[i]);
   }
   return rc;
}

/** private data **/

   
/*******************************************************************  
 *
 *  $Log: tk_sysqueues.c,v $
 *  Revision 1.1  2005-12-01 13:05:25  ambrmi09
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
 *  
 *******************************************************************/

