/*!
@file
@ingroup PTHREAD_SYNC

@brief POSIX 1003.1c API - Thread synchronisation

The content of this source-file implement thread syncronisation. I.e. mutex, rw-lock e.t.a,

For in-depth discussions about this component, see \ref
PTHREAD_SYNC

@see PTHREAD_SCHED
@see PTHREAD

*/

#include <pthread.h>
#include <errno.h>

unsigned long tk_pthread_sync( void ){
   return ERR_OK;    	
}
//------1---------2---------3---------4---------5---------6---------7---------8
unsigned long tk_pthread_sync_destruct( void ){
   return ERR_OK;
}
//------1---------2---------3---------4---------5---------6---------7---------8

 



/** @defgroup PTHREAD_SYNC PTHREAD_SYNC: POSIX 1003.1c API - Thread synchronisation
@ingroup PTHREAD
@brief POSIX 1003.1c API - Thread synchronisation

Syncronisation between threads, i.e. 

- Mutexes and attribute handling thereof
- Event handling
- Timout event handling - special case of the above

<p><b>Go gack to</b> \ref COMPONENTS</p>

*/
  
/*! 
 * @addtogroup CVSLOG CVSLOG
 *  $Log: pthread_sync.c,v $
 *  Revision 1.2  2006-02-20 15:22:01  ambrmi09
 *  Documentation stuff. No code changes.
 *
 *  Revision 1.1  2006/02/19 22:12:07  ambrmi09
 *  CO of missed files
 *
 *  
 *******************************************************************/
 













