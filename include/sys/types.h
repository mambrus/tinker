/*!
@file
@ingroup kernel_reimpl_ansi

@brief ANSI sys/types.h file <b>IMPORTANT:</b> See the note in the detailed section!

@note <b>Make sure NEVER to use this file in a build-chain where an
"official" sys/types.h exist. Libraries might be using a completely
different set of structs !</b>

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi.


@see kernel_reimpl_ansi

<HR>
*/

#ifndef SYS_TYPES_H
#define SYS_TYPES_H


/*!
This is an integer data type used to represent file modes. In the GNU
system, this is equivalent to unsigned int.
*/
typedef unsigned int mode_t;



//typedef struct pthread_t_ *pthread_t; //No way I'm going to return a pointer to anything for this!
typedef unsigned int pthread_t;
typedef struct pthread_attr_t_ *pthread_attr_t;
typedef struct pthread_once_t_ pthread_once_t;
typedef struct pthread_key_t_ *pthread_key_t;
typedef struct pthread_mutex_t_ *pthread_mutex_t;
typedef struct pthread_mutexattr_t_ *pthread_mutexattr_t;
typedef struct pthread_cond_t_ *pthread_cond_t;
typedef struct pthread_condattr_t_ *pthread_condattr_t;

#endif //SYS_TYPES_H


/*!
 * @defgroup CVSLOG_types_h types_h
 * @ingroup CVSLOG
 *  $Log: types.h,v $
 *  Revision 1.3  2006-02-22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  
 */

