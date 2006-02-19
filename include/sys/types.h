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

typedef struct pthread_t_ *pthread_t;
typedef struct pthread_attr_t_ *pthread_attr_t;
typedef struct pthread_once_t_ pthread_once_t;
typedef struct pthread_key_t_ *pthread_key_t;
typedef struct pthread_mutex_t_ *pthread_mutex_t;
typedef struct pthread_mutexattr_t_ *pthread_mutexattr_t;
typedef struct pthread_cond_t_ *pthread_cond_t;
typedef struct pthread_condattr_t_ *pthread_condattr_t;

