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
   errno = rc;
   
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
   assert("sem_destroy - Not implemented" == NULL);
}

/*!
@brief TBD

@see http://www.opengroup.org/onlinepubs/009695399/functions/sem_trywait.html
*/
int sem_trywait (sem_t * sem){
   assert("sem_trywait - Not implemented" == NULL);
}

/*!
@brief Locks the semaphore. Equivalent to TinKer \ref sm_p

@see http://www.opengroup.org/onlinepubs/009695399/functions/sem_wait.html
*/
int sem_wait (sem_t * sem){
   unsigned int rc;
   
   rc = sm_p(*sem,WAIT,FOREVER);
   errno = rc;
   
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
   errno = rc;
   
   if (rc==ERR_OK)
      return 0;
   else
      return EINVAL;

}

/*!
 * @defgroup CVSLOG_semaphore_c semaphore_c
 * @ingroup CVSLOG
 *  $Log: semaphore.c,v $
 *  Revision 1.7  2006-02-23 15:33:33  ambrmi09
 *  Found a nasty "bug", that was not a read bug after all. At least not in the kernel as a feared. It turned out that I forgot some of the details about how timeouts were to be handled (especially in \ref ITC ). A timeout of value \b zero is equal of never to timeout (read more about it in define \ref FOREVER). However two important lesson learned: Even simple add operations get "funny" when adding large numbers (see line 303 in tk_ipc.c - in the \ref lock_stage function). Anyway. FOREVER should equal zero. (This issue makes me wonder sometimes how sane it really was to resurrect a project that has been dormant for nearly 10 years.) The CodeWright project ruler should be positioned on the actual line btw. This check-in will be accompanied  by a <tt>cvs tag</tt> for this reason, and for yet another nasty bug that seems to be a real dispatcher bug. The current source-set-up will show the bug within one mint (which is good since it makes it a little bit less of a search for the <I>"needle in the haystack</i>").
 *
 *  Revision 1.6  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  
 */
