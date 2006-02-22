/*******************************************************************
 *
 *  DESCRIPTION: tk_sysqueues.c 
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:    
 *
 *  Current $Revision: 1.6 $
 *******************************************************************/
  

/*- include files **/

#include <tk_ipc.h>
#include <tk_sysqueues.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>


/* default settings */

#define SYSQ_LENGT 10   /*!< Lengt of each Q (i.e. the number of 
elements in each queue) Small number, but should be OK since these 
Q's are ment to be used by threads that run on highest prio. */

/*- external functions **/

/*- external data **/

/*- public data **/
unsigned long tk_sys_queues[TK_NUMBER_OF_SYSQ];

/*- internal functions **/
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

/*- private data **/

   
/*!
 * @defgroup CVSLOG_tk_sysqueues_c tk_sysqueues_c
 * @ingroup CVSLOG
 *
 *  $Log: tk_sysqueues.c,v $
 *  Revision 1.6  2006-02-22 13:05:47  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.5  2006/02/19 12:44:33  ambrmi09
 *  - Documented ITC
 *  - Started to build up the structure for the \ref PTHREAD component
 *
 *  Revision 1.4  2006/02/16 15:11:01  ambrmi09
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
 *  Revision 1.3  2006/02/02 15:51:02  ambrmi09
 *  A lot of thought has been invested into the new PTIME component. Had to
 *  change things even in the systime parts (integrated in the SHEDUL
 *  component) to make it more generic. Think this will be really nice when
 *  it's ready, but has been a long road to get PTIME running (and I'm
 *  still not there).
 *
 *  Revision 1.2  2005/12/03 14:04:31  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *  Revision 1.1  2005/12/01 13:05:25  ambrmi09
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

