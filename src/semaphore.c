/*!
@file
@ingroup POSIX_RT

@brief RT semaphores - POSIX 1003.1b API

*-* Doc in progress *-*

<p>
A very hastily done wrapper done in 1hr. Probably needs a lot of more work.
</p>


For in-depth discussions about this component, see \ref
POSIX_RT

@see POSIX_RT
@see PTHREAD
*/

#include <semaphore.h>
#include <errno.h>
#include <assert.h>
#include <tk_ipc.h>


/*!
@brief Defines a semaphore. Equivalent to TinKer \ref sm_create

@see http://www.opengroup.org/onlinepubs/009695399/functions/sem_init.html
*/
int sem_init (
   sem_t          *sem,
   int            pshared,
   unsigned int   value
){
   unsigned int rc;
   
   if (pshared != 0)
      return EINVAL;
   
   rc = sm_create("PS", value, FIFO, sem);    
   if (rc==ERR_OK)
      return 0;
   else
      return EINVAL;


}

/*!
@brief Undefines a semaphore. Equivalent to TinKer \ref sm_delete

@see http://www.opengroup.org/onlinepubs/009695399/functions/sem_destroy.html
*/
int sem_destroy (sem_t * sem){
}

/*!
@brief TBD

@see http://www.opengroup.org/onlinepubs/009695399/functions/sem_trywait.html
*/
int sem_trywait (sem_t * sem){
   assert("Not implemented" == NULL);
}

/*!
@brief Locks the semaphore. Equivalent to TinKer \ref sm_p

@see http://www.opengroup.org/onlinepubs/009695399/functions/sem_wait.html
*/
int sem_wait (sem_t * sem){
   unsigned int rc;
   
   rc = sm_p(*sem,WAIT,FOREVER);
   
   if (rc==ERR_OK)
      return 0;
   else
      return EINVAL;
}

/*!
@brief Unlocks the semaphore. Equivalent to TinKer \ref sm_v

@see http://www.opengroup.org/onlinepubs/009695399/functions/sem_post.html
*/
int sem_post (sem_t * sem){
      unsigned int rc;
   
   rc = sm_v(*sem);
   
   if (rc==ERR_OK)
      return 0;
   else
      return EINVAL;

}
