/*!
@file
@ingroup POSIX_RT

@brief RT semaphores - POSIX 1003.1b API

TBD

For in-depth discussions about this component, see \ref
POSIX_RT

@see POSIX_RT
@see PTHREAD
*/

#if !defined( SEMAPHORE_H )
#define SEMAPHORE_H

//#include <process.h>


#ifdef __cplusplus
extern "C"
{
#endif                          

typedef unsigned long sem_t;

int sem_init (sem_t * sem,
	      int pshared,
	      unsigned int value
	      );

int sem_destroy (sem_t * sem
		 );

int sem_trywait (sem_t * sem
		 );

int sem_wait (sem_t * sem
	      );

int sem_post (sem_t * sem
	      );

#ifdef __cplusplus
}                               
#endif                          

#endif                          

/*!
 * @defgroup CVSLOG_semaphore_h semaphore_h
 * @ingroup CVSLOG
 *  $Log: semaphore.h,v $
 *  Revision 1.4  2006-02-22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  
 */

