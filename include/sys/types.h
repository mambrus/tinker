/*!
@file
@ingroup kernel_reimpl_ansi

@brief ANSI sys/types.h file <b>IMPORTANT:</b> See the note in the detailed section!

@note <b>Make sure NEVER to use this</b> file in a build-chain where an
"official" sys/types.h exist. Libraries might be using a completely
different set of structs ! Except for GNU tool-chain, where #if logic takes 
care of the issue.

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi.


@see kernel_reimpl_ansi

<HR>
*/

#if defined (__GNUC__)
   #include <../src/tk_ansi.h>
   #include <../src/tk_ansi_dirwrap.h>
   #include BUILDCHAIN(sys/types.h)
#endif

#ifndef SYS_TYPES_H_TK
#define SYS_TYPES_H_TK


#if !defined (__GNUC__)
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

#endif   //defined (_GNU_)

#endif //SYS_TYPES_H


/*!
 * @defgroup CVSLOG_types_h types_h
 * @ingroup CVSLOG
 *  $Log: types.h,v $
 *  Revision 1.4  2006-03-02 14:05:49  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.3  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  
 */

