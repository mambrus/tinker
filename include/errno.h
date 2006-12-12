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
@ingroup kernel_reimpl_ansi

@brief ANSI errno.h file <b>IMPORTANT:</b> See the note in the detailed section!

@note <b>Make sure NEVER to use this file in a build-chain where an
"official" errno.h exist. Libraries might be using a completely
different set of codes, and you error handling will get totally
screwed!</b>

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi.

TinKer will by far not use most of these codes. But they're standardized
so we'll include them all.

@see kernel_reimpl_ansi


<HR>

<H2>Checking for Errors</H2>

Most library functions return a special value to indicate that they have
failed. The special value is typically -1, a null pointer, or a constant
such as EOF that is defined for that purpose. But this return value
tells you only that an error has occurred. To find out what kind of
error it was, you need to look at the error code stored in the variable
errno. This variable is declared in the header file errno.h.

volatile int errno  Variable 
The variable errno contains the system error number. You can change the
value of errno.

Since errno is declared volatile, it might be changed asynchronously by
a signal handler; see Defining Handlers. However, a properly written
signal handler saves and restores the value of errno, so you generally
do not need to worry about this possibility except when writing signal
handlers.

The initial value of errno at program startup is zero. Many library
functions are guaranteed to set it to certain nonzero values when they
encounter certain kinds of errors. These error conditions are listed for
each function. These functions do not change errno when they succeed;
thus, the value of errno after a successful call is not necessarily
zero, and you should not use errno to determine whether a call failed.
The proper way to do that is documented for each function. If the call
failed, you can examine errno.

Many library functions can set errno to a nonzero value as a result of
calling other library functions which might fail. You should assume that
any library function might alter errno when the function returns an
error.

Portability Note: ISO C specifies errno as a "modifiable lvalue" rather
than as a variable, permitting it to be implemented as a macro. For
example, its expansion might involve a function call, like *_errno ().
In fact, that is what it is on the GNU system itself. The GNU library,
on non-GNU systems, does whatever is right for the particular system.

There are a few library functions, like sqrt and atan, that return a
perfectly legitimate value in case of an error, but also set errno. For
these functions, if you want to check to see whether an error occurred,
the recommended method is to set errno to zero before calling the
function, and then check its value afterward.

All the error codes have symbolic names; they are macros defined in
errno.h. The names start with E and an upper-case letter or digit; you
should consider names of this form to be reserved names. See Reserved
Names.

The error code values are all positive integers and are all distinct,
with one exception: EWOULDBLOCK and EAGAIN are the same. Since the
values are distinct, you can use them as labels in a switch statement;
just don't use both EWOULDBLOCK and EAGAIN. Your program should not make
any other assumptions about the specific values of these symbolic
constants.

The value of errno doesn't necessarily have to correspond to any of
these macros, since some library functions might return other error
codes of their own for other situations. The only values that are
guaranteed to be meaningful for a particular library function are the
ones that this manual lists for that function.

On non-GNU systems, almost any system call can return EFAULT if it is
given an invalid pointer as an argument. Since this could only happen as
a result of a bug in your program, and since it will not happen on the
GNU system, we have saved space by not mentioning EFAULT in the
descriptions of individual functions.

In some Unix systems, many system calls can also return EFAULT if given
as an argument a pointer into the stack, and the kernel for some obscure
reason fails in its attempt to extend the stack. If this ever happens,
you should probably try using statically or dynamically allocated memory
instead of stack memory on that system.



<H2>Error Codes</H2>
The error code macros are defined in the header file errno.h. All of
them expand into integer constant values. Some of these error codes
can't occur on the GNU system, but they can occur using the GNU library
on other systems.

*/

#ifndef ERRNO_H_TK
#define ERRNO_H_TK


#include <tk_ansi.h>

#if !defined(__C166__)
#   include <tk_ansi_dirwrap.h>
#   include BUILDCHAIN(errno.h)
#endif


#if defined(errno)
   #undef errno          //!< We'll define our own errno \e "varialble"
#endif

//#include <tk.h>
#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS==0)
extern "C" {
#endif

int           *_tk_errno();

#if defined(__cplusplus) && (TK_CPLUSPLUS==0)
}
#endif

#if apa
#error Hehe
#endif


/*! 
@brief Access thread-internal errno variable

Map to an internal variable unique to the current threads. You use this
exactly as if it would be a variable, but it's not.
*/
#define errno (*_tk_errno())




#if !defined(__GNUC__) && !( defined(_WIN32) || defined(_MSC_VER))
//#if !defined(__GNUC__)
#include <tk_mem.h>  //Last in error chain 
/*!
To make life easier and not to have to trim every tiny errorcode, all
are put in this enumertion list.  ANSI error codes are normally defined
and we dont want to breake that convention. Therefore ANSO codes are
prefixed ANSI and then defined again each separatly.

@note The order of each code in the list is not critical, exept for the
forst one that has to be zero and has to mean "all OK".

The first part of the enumeration is a TinKer code specific area. It
should be fairlly small and even shrink over time the close we get to
following ANSI standard.

*/
enum ANSI_ERROR_CODES{
///*First are TiNker codes*/
///*COMPONENT SCHED*/
//TK_OK            ,//!< @brief Operation returned without errors
//TK_ERROR         ,//!< @brief General TinKer catch all error
//
//
///*COMPONENT ITC (originally pSos error codes). NOTE that the numbers are changed!!*/
//ERR_TIMEOUT      ,//!< @brief ITC has timedout
//ERR_OBJDEL       ,//!< @brief ITC has been deleted
//ERR_OBJID        ,//!< @brief ITC id incorrect ??
//ERR_OBJTYPE      ,//!< @brief ITC type doesn'e mach object id
//ERR_OBJFULL      ,//!< @brief Nodes objecttable is full
//ERR_BUFSIZ       ,//!< @brief The allocated buffer is smaller than a message could be
//ERR_NOQCB        ,//!< @brief Exceeds max numer of queues
//ERR_FULL         ,//!< @brief The message buffer is full
//ERR_NOSCB        ,//!< @brief Exceeds max numer of semaphores
//ERR_NOSEM        ,//!< @brief Only if NOWAIT was selected
//ERR_TATSDEL      ,//!< @brief There were threads waiting 
//ERR_NOMEM        ,//!< @brief No more memory
//ERR_BLOCKLIMIT   ,//!< @brief Can't block more procs on queue or semaphore
//
///*COMPONENT PTIMER*/                   
//
//ERR_UNDEF_PTIMER           ,//!< @brief No such or invalid ptimer 
//ERR_TIME_NEG               ,//!< @brief Trying to set event for time that's passed 
//ERR_TIMER_DELETED          ,//!< @brief The timer (i.e. the requested event) has been deleted 
//ERR_NO_MORE_TIMERS         ,//!< @brief You try to exeed the maximum number of pending timers
//
///*COMPONENT KMEM*/                   
//ERR_UNDEF_HEAPID           ,/*!< @brief No such or invalid \ref heap_t "heap" */
//

/*ANSI codes*/                                      
ANSI_ENUM_EPERM=ERR_KMEM_SENTINEL , //!< @brief \ref EPERM
ANSI_ENUM_ENOENT           , //!< @brief \ref ENOENT
ANSI_ENUM_ESRCH            , //!< @brief \ref ESRCH
ANSI_ENUM_EINTR            , //!< @brief \ref EINTR
ANSI_ENUM_EIO              , //!< @brief \ref EIO
ANSI_ENUM_ENXIO            , //!< @brief \ref ENXIO
ANSI_ENUM_E2BIG            , //!< @brief \ref E2BIG
ANSI_ENUM_ENOEXEC          , //!< @brief \ref ENOEXEC
ANSI_ENUM_EBADF            , //!< @brief \ref EBADF
ANSI_ENUM_ECHILD           , //!< @brief \ref ECHILD
ANSI_ENUM_EDEADLK          , //!< @brief \ref EDEADLK
ANSI_ENUM_ENOMEM           , //!< @brief \ref ENOMEM
ANSI_ENUM_EACCES           , //!< @brief \ref EACCES
ANSI_ENUM_EFAULT           , //!< @brief \ref EFAULT
ANSI_ENUM_ENOTBLK          , //!< @brief \ref ENOTBLK
ANSI_ENUM_EBUSY            , //!< @brief \ref EBUSY
ANSI_ENUM_EEXIST           , //!< @brief \ref EEXIST
ANSI_ENUM_EXDEV            , //!< @brief \ref EXDEV
ANSI_ENUM_ENODEV           , //!< @brief \ref ENODEV
ANSI_ENUM_ENOTDIR          , //!< @brief \ref ENOTDIR
ANSI_ENUM_EISDIR           , //!< @brief \ref EISDIR
ANSI_ENUM_EINVAL           , //!< @brief \ref EINVAL
ANSI_ENUM_EMFILE           , //!< @brief \ref EMFILE
ANSI_ENUM_ENFILE           , //!< @brief \ref ENFILE
ANSI_ENUM_ENOTTY           , //!< @brief \ref ENOTTY
ANSI_ENUM_ETXTBSY          , //!< @brief \ref ETXTBSY
ANSI_ENUM_EFBIG            , //!< @brief \ref EFBIG
ANSI_ENUM_ENOSPC           , //!< @brief \ref ENOSPC
ANSI_ENUM_ESPIPE           , //!< @brief \ref ESPIPE
ANSI_ENUM_EROFS            , //!< @brief \ref EROFS
ANSI_ENUM_EMLINK           , //!< @brief \ref EMLINK
ANSI_ENUM_EPIPE            , //!< @brief \ref EPIPE
ANSI_ENUM_EDOM             , //!< @brief \ref EDOM
ANSI_ENUM_ERANGE           , //!< @brief \ref ERANGE
ANSI_ENUM_EAGAIN           , //!< @brief \ref EAGAIN
ANSI_ENUM_EWOULDBLOCK      , //!< @brief \ref EWOULDBLOCK
ANSI_ENUM_EINPROGRESS      , //!< @brief \ref EINPROGRESS
ANSI_ENUM_EALREADY         , //!< @brief \ref EALREADY
ANSI_ENUM_ENOTSOCK         , //!< @brief \ref ENOTSOCK
ANSI_ENUM_EMSGSIZE         , //!< @brief \ref EMSGSIZE
ANSI_ENUM_EPROTOTYPE       , //!< @brief \ref EPROTOTYPE
ANSI_ENUM_ENOPROTOOPT      , //!< @brief \ref ENOPROTOOPT
ANSI_ENUM_EPROTONOSUPPORT  , //!< @brief \ref EPROTONOSUPPORT
ANSI_ENUM_ESOCKTNOSUPPORT  , //!< @brief \ref ESOCKTNOSUPPORT
ANSI_ENUM_EOPNOTSUPP       , //!< @brief \ref EOPNOTSUPP
ANSI_ENUM_EPFNOSUPPORT     , //!< @brief \ref EPFNOSUPPORT
ANSI_ENUM_EAFNOSUPPORT     , //!< @brief \ref EAFNOSUPPORT
ANSI_ENUM_EADDRINUSE       , //!< @brief \ref EADDRINUSE
ANSI_ENUM_EADDRNOTAVAIL    , //!< @brief \ref EADDRNOTAVAIL
ANSI_ENUM_ENETDOWN         , //!< @brief \ref ENETDOWN
ANSI_ENUM_ENETUNREACH      , //!< @brief \ref ENETUNREACH
ANSI_ENUM_ENETRESET        , //!< @brief \ref ENETRESET
ANSI_ENUM_ECONNABORTED     , //!< @brief \ref ECONNABORTED
ANSI_ENUM_ECONNRESET       , //!< @brief \ref ECONNRESET
ANSI_ENUM_ENOBUFS          , //!< @brief \ref ENOBUFS
ANSI_ENUM_EISCONN          , //!< @brief \ref EISCONN
ANSI_ENUM_ENOTCONN         , //!< @brief \ref ENOTCONN
ANSI_ENUM_EDESTADDRREQ     , //!< @brief \ref EDESTADDRREQ
ANSI_ENUM_ESHUTDOWN        , //!< @brief \ref ESHUTDOWN
ANSI_ENUM_ETOOMANYREFS     , //!< @brief \ref ETOOMANYREFS
ANSI_ENUM_ETIMEDOUT        , //!< @brief \ref ETIMEDOUT
ANSI_ENUM_ECONNREFUSED     , //!< @brief \ref ECONNREFUSED
ANSI_ENUM_ELOOP            , //!< @brief \ref ELOOP
ANSI_ENUM_ENAMETOOLONG     , //!< @brief \ref ENAMETOOLONG
ANSI_ENUM_EHOSTDOWN        , //!< @brief \ref EHOSTDOWN
ANSI_ENUM_EHOSTUNREACH     , //!< @brief \ref EHOSTUNREACH
ANSI_ENUM_ENOTEMPTY        , //!< @brief \ref ENOTEMPTY
ANSI_ENUM_EPROCLIM         , //!< @brief \ref EPROCLIM
ANSI_ENUM_EUSERS           , //!< @brief \ref EUSERS
ANSI_ENUM_EDQUOT           , //!< @brief \ref EDQUOT
ANSI_ENUM_ESTALE           , //!< @brief \ref ESTALE
ANSI_ENUM_EREMOTE          , //!< @brief \ref EREMOTE
ANSI_ENUM_EBADRPC          , //!< @brief \ref EBADRPC
ANSI_ENUM_ERPCMISMATCH     , //!< @brief \ref ERPCMISMATCH
ANSI_ENUM_EPROGUNAVAIL     , //!< @brief \ref EPROGUNAVAIL
ANSI_ENUM_EPROGMISMATCH    , //!< @brief \ref EPROGMISMATCH
ANSI_ENUM_EPROCUNAVAIL     , //!< @brief \ref EPROCUNAVAIL
ANSI_ENUM_ENOLCK           , //!< @brief \ref ENOLCK
ANSI_ENUM_EFTYPE           , //!< @brief \ref EFTYPE
ANSI_ENUM_EAUTH            , //!< @brief \ref EAUTH
ANSI_ENUM_ENEEDAUTH        , //!< @brief \ref ENEEDAUTH
ANSI_ENUM_ENOSYS           , //!< @brief \ref ENOSYS
ANSI_ENUM_ENOTSUP          , //!< @brief \ref ENOTSUP
ANSI_ENUM_EILSEQ           , //!< @brief \ref EILSEQ
ANSI_ENUM_EBACKGROUND      , //!< @brief \ref EBACKGROUND
ANSI_ENUM_EDIED            , //!< @brief \ref EDIED
ANSI_ENUM_ED               , //!< @brief \ref ED
ANSI_ENUM_EGREGIOUS        , //!< @brief \ref EGREGIOUS
ANSI_ENUM_EIEIO            , //!< @brief \ref EIEIO
ANSI_ENUM_EGRATUITOUS      , //!< @brief \ref EGRATUITOUS
ANSI_ENUM_EBADMSG          , //!< @brief \ref EBADMSG
ANSI_ENUM_EIDRM            , //!< @brief \ref EIDRM
ANSI_ENUM_EMULTIHOP        , //!< @brief \ref EMULTIHOP
ANSI_ENUM_ENODATA          , //!< @brief \ref ENODATA
ANSI_ENUM_ENOLINK          , //!< @brief \ref ENOLINK
ANSI_ENUM_ENOMSG           , //!< @brief \ref ENOMSG
ANSI_ENUM_ENOSR            , //!< @brief \ref ENOSR
ANSI_ENUM_ENOSTR           , //!< @brief \ref ENOSTR
ANSI_ENUM_EOVERFLOW        , //!< @brief \ref EOVERFLOW
ANSI_ENUM_EPROTO           , //!< @brief \ref EPROTO
ANSI_ENUM_ETIME            , //!< @brief \ref ETIME
ANSI_ENUM_ECANCELED        , //!< @brief \ref ECANCELED
ANSI_ENUM_ERESTART         , //!< @brief \ref ERESTART
ANSI_ENUM_ECHRNG           , //!< @brief \ref ECHRNG
ANSI_ENUM_EL2NSYNC         , //!< @brief \ref EL2NSYNC
ANSI_ENUM_EL3HLT           , //!< @brief \ref EL3HLT
ANSI_ENUM_EL3RST           , //!< @brief \ref EL3RST
ANSI_ENUM_ELNRNG           , //!< @brief \ref ELNRNG
ANSI_ENUM_EUNATCH          , //!< @brief \ref EUNATCH
ANSI_ENUM_ENOCSI           , //!< @brief \ref ENOCSI
ANSI_ENUM_EL2HLT           , //!< @brief \ref EL2HLT
ANSI_ENUM_EBADE            , //!< @brief \ref EBADE
ANSI_ENUM_EBADR            , //!< @brief \ref EBADR
ANSI_ENUM_EXFULL           , //!< @brief \ref EXFULL
ANSI_ENUM_ENOANO           , //!< @brief \ref ENOANO
ANSI_ENUM_EBADRQC          , //!< @brief \ref EBADRQC
ANSI_ENUM_EBADSLT          , //!< @brief \ref EBADSLT
ANSI_ENUM_EDEADLOCK        , //!< @brief \ref EDEADLOCK
ANSI_ENUM_EBFONT           , //!< @brief \ref EBFONT
ANSI_ENUM_ENONET           , //!< @brief \ref ENONET
ANSI_ENUM_ENOPKG           , //!< @brief \ref ENOPKG
ANSI_ENUM_EADV             , //!< @brief \ref EADV
ANSI_ENUM_ESRMNT           , //!< @brief \ref ESRMNT
ANSI_ENUM_ECOMM            , //!< @brief \ref ECOMM
ANSI_ENUM_EDOTDOT          , //!< @brief \ref EDOTDOT
ANSI_ENUM_ENOTUNIQ         , //!< @brief \ref ENOTUNIQ
ANSI_ENUM_EBADFD           , //!< @brief \ref EBADFD
ANSI_ENUM_EREMCHG          , //!< @brief \ref EREMCHG
ANSI_ENUM_ELIBACC          , //!< @brief \ref ELIBACC
ANSI_ENUM_ELIBBAD          , //!< @brief \ref ELIBBAD
ANSI_ENUM_ELIBSCN          , //!< @brief \ref ELIBSCN
ANSI_ENUM_ELIBMAX          , //!< @brief \ref ELIBMAX
ANSI_ENUM_ELIBEXEC         , //!< @brief \ref ELIBEXEC
ANSI_ENUM_ESTRPIPE         , //!< @brief \ref ESTRPIPE
ANSI_ENUM_EUCLEAN          , //!< @brief \ref EUCLEAN
ANSI_ENUM_ENOTNAM          , //!< @brief \ref ENOTNAM
ANSI_ENUM_ENAVAIL          , //!< @brief \ref ENAVAIL
ANSI_ENUM_EISNAM           , //!< @brief \ref EISNAM
ANSI_ENUM_EREMOTEIO        , //!< @brief \ref EREMOTEIO
ANSI_ENUM_ENOMEDIUM        , //!< @brief \ref ENOMEDIUM
ANSI_ENUM_EMEDIUMTYPE        //!< @brief \ref ANSI_ENUM_EMEDIUMTYPE
}; 

//#if defined(_WIN32) &&  defined(_MSC_VER)
/* MSVC defined allready
EPERM
ENOENT
ESRCH
EINTR
EIO
ENXIO
E2BIG
ENOEXEC
EBADF
ECHILD
EDEADLK
ENOMEM
EACCES
EFAULT
EBUSY
EEXIST
EXDEV
ENODEV
ENOTDIR
EISDIR
EINVAL
EMFILE
ENFILE
ENOTTY
EFBIG
ENOSPC
ESPIPE
EROFS
EMLINK
EPIPE
EDOM
ERANGE
EAGAIN
ENAMETOOLONG
ENOTEMPTY
ENOLCK
ENOSYS
EILSEQ
EDEADLOCK
EMSGSIZE
*/


#define  EPERM ANSI_ENUM_EPERM /*!<
Operation not permitted; only the owner of the file (or other resource) or processes with special privileges can perform the operation.  
*/

#define  ENOENT ANSI_ENUM_ENOENT /*!<
No such file or directory. This is a "file doesn't exist" error for ordinary files that are referenced in contexts where they are expected to already exist.  
*/

#define  ESRCH ANSI_ENUM_ESRCH /*!<
No process matches the specified process ID.  
*/

#define  EINTR ANSI_ENUM_EINTR /*!<
Interrupted function call; an asynchronous signal occurred and prevented completion of the call. When this happens, you should try the call again. 
You can choose to have functions resume after a signal that is handled, rather than failing with EINTR; see Interrupted Primitives. 
*/


#define  EIO ANSI_ENUM_EIO /*!<
Input/output error; usually used for physical read or write errors.  
*/

#define  ENXIO ANSI_ENUM_ENXIO /*!<
No such device or address. The system tried to use the device represented by a file you specified, and it couldn't find the device. This can mean that the device file was installed incorrectly, or that the physical device is missing or not correctly attached to the computer.  
*/

#define  E2BIG ANSI_ENUM_E2BIG /*!<
Argument list too long; used when the arguments passed to a new program being executed with one of the exec functions (see Executing a File) occupy too much memory space. This condition never arises in the GNU system.  
*/

#define  ENOEXEC ANSI_ENUM_ENOEXEC /*!<
Invalid executable file format. This condition is detected by the exec functions; see Executing a File.  
*/

#define  EBADF ANSI_ENUM_EBADF /*!<
Bad file descriptor; for example, I/O on a descriptor that has been closed or reading from a descriptor open only for writing (or vice versa).  
*/

#define  ECHILD ANSI_ENUM_ECHILD /*!<
There are no child processes. This error happens on operations that are supposed to manipulate child processes, when there aren't any processes to manipulate.  
*/

#define  EDEADLK ANSI_ENUM_EDEADLK /*!<
Deadlock avoided; allocating a system resource would have resulted in a deadlock situation. The system does not guarantee that it will notice all such situations. This error means you got lucky and the system noticed; it might just hang. See File Locks, for an example.  
*/

#define  ENOMEM ANSI_ENUM_ENOMEM /*!<
No memory available. The system cannot allocate more virtual memory because its capacity is full.  
*/

#define  EACCES ANSI_ENUM_EACCES /*!<
Permission denied; the file permissions do not allow the attempted operation.  
*/

#define  EFAULT ANSI_ENUM_EFAULT /*!<
Bad address; an invalid pointer was detected. In the GNU system, this error never happens; you get a signal instead.  
*/

#define  ENOTBLK ANSI_ENUM_ENOTBLK /*!<
A file that isn't a block special file was given in a situation that requires one. For example, trying to mount an ordinary file as a file system in Unix gives this error.  
*/

#define  EBUSY ANSI_ENUM_EBUSY /*!<
Resource busy; a system resource that can't be shared is already in use. For example, if you try to delete a file that is the root of a currently mounted filesystem, you get this error.  
*/

#define  EEXIST ANSI_ENUM_EEXIST /*!<
File exists; an existing file was specified in a context where it only makes sense to specify a new file.  
*/

#define  EXDEV ANSI_ENUM_EXDEV /*!<
An attempt to make an improper link across file systems was detected. This happens not only when you use link (see Hard Links) but also when you rename a file with rename (see Renaming Files).  
*/

#define  ENODEV ANSI_ENUM_ENODEV /*!<
The wrong type of device was given to a function that expects a particular sort of device.  
*/

#define  ENOTDIR ANSI_ENUM_ENOTDIR /*!<
A file that isn't a directory was specified when a directory is required.  
*/

#define  EISDIR ANSI_ENUM_EISDIR /*!<
File is a directory; you cannot open a directory for writing, or create or remove hard links to it.  
*/

#define  EINVAL ANSI_ENUM_EINVAL /*!<
Invalid argument. This is used to indicate various kinds of problems with passing the wrong argument to a library function.  
*/

#define  EMFILE ANSI_ENUM_EMFILE /*!<
The current process has too many files open and can't open any more. Duplicate descriptors do count toward this limit. 
In BSD and GNU, the number of open files is controlled by a resource limit that can usually be increased. If you get this error, you might want to increase the RLIMIT_NOFILE limit or make it unlimited; see Limits on Resources. 
*/


#define  ENFILE ANSI_ENUM_ENFILE /*!<
There are too many distinct file openings in the entire system. Note that any number of linked channels count as just one file opening; see Linked Channels. This error never occurs in the GNU system.  
*/

#define  ENOTTY ANSI_ENUM_ENOTTY /*!<
Inappropriate I/O control operation, such as trying to set terminal modes on an ordinary file.  
*/

#define  ETXTBSY ANSI_ENUM_ETXTBSY /*!<
An attempt to execute a file that is currently open for writing, or write to a file that is currently being executed. Often using a debugger to run a program is considered having it open for writing and will cause this error. (The name stands for "text file busy".) This is not an error in the GNU system; the text is copied as necessary.  
*/

#define  EFBIG ANSI_ENUM_EFBIG /*!<
File too big; the size of a file would be larger than allowed by the system.  
*/

#define  ENOSPC ANSI_ENUM_ENOSPC /*!<
No space left on device; write operation on a file failed because the disk is full.  
*/

#define  ESPIPE ANSI_ENUM_ESPIPE /*!<
Invalid seek operation (such as on a pipe).  
*/

#define  EROFS ANSI_ENUM_EROFS /*!<
An attempt was made to modify something on a read-only file system.  
*/

#define  EMLINK ANSI_ENUM_EMLINK /*!<
Too many links; the link count of a single file would become too large. rename can cause this error if the file being renamed already has as many links as it can take (see Renaming Files).  
*/

#define  EPIPE ANSI_ENUM_EPIPE /*!<
Broken pipe; there is no process reading from the other end of a pipe. Every library function that returns this error code also generates a SIGPIPE signal; this signal terminates the program if not handled or blocked. Thus, your program will never actually see EPIPE unless it has handled or blocked SIGPIPE.  
*/

#define  EDOM ANSI_ENUM_EDOM /*!<
Domain error; used by mathematical functions when an argument value does not fall into the domain over which the function is defined.  
*/

#define  ERANGE ANSI_ENUM_ERANGE /*!<
Range error; used by mathematical functions when the result value is not representable because of overflow or underflow.  
*/

#define  EAGAIN ANSI_ENUM_EAGAIN /*!<
Resource temporarily unavailable; the call might work if you try again later. The macro EWOULDBLOCK is another name for EAGAIN; they are always the same in the GNU C library. 
This error can happen in a few different situations: 

An operation that would block was attempted on an object that has non-blocking mode selected. Trying the same operation again will block until some external condition makes it possible to read, write, or connect (whatever the operation). You can use select to find out when the operation will be possible; see Waiting for I/O. 
Portability Note: In many older Unix systems, this condition was indicated by EWOULDBLOCK, which was a distinct error code different from EAGAIN. To make your program portable, you should check for both codes and treat them the same. 

A temporary resource shortage made an operation impossible. fork can return this error. It indicates that the shortage is expected to pass, so your program can try the call again later and it may succeed. It is probably a good idea to delay for a few seconds before trying it again, to allow time for other processes to release scarce resources. Such shortages are usually fairly serious and affect the whole system, so usually an interactive program should report the error to the user and return to its command loop. 
*/


#define  EWOULDBLOCK ANSI_ENUM_EWOULDBLOCK /*!<
In the GNU C library, this is another name for EAGAIN (above). The values are always the same, on every operating system. 
C libraries in many older Unix systems have EWOULDBLOCK as a separate error code. 
*/


#define  EINPROGRESS ANSI_ENUM_EINPROGRESS /*!<
An operation that cannot complete immediately was initiated on an object that has non-blocking mode selected. Some functions that must always block (such as connect; see Connecting) never return EAGAIN. Instead, they return EINPROGRESS to indicate that the operation has begun and will take some time. Attempts to manipulate the object before the call completes return EALREADY. You can use the select function to find out when the pending operation has completed; see Waiting for I/O.  
*/

#define  EALREADY ANSI_ENUM_EALREADY /*!<
An operation is already in progress on an object that has non-blocking mode selected.  
*/

#define  ENOTSOCK ANSI_ENUM_ENOTSOCK /*!<
A file that isn't a socket was specified when a socket is required.  
*/

#define  EMSGSIZE ANSI_ENUM_EMSGSIZE /*!<
The size of a message sent on a socket was larger than the supported maximum size.  
*/

#define  EPROTOTYPE ANSI_ENUM_EPROTOTYPE /*!<
The socket type does not support the requested communications protocol.  
*/

#define  ENOPROTOOPT ANSI_ENUM_ENOPROTOOPT /*!<
You specified a socket option that doesn't make sense for the particular protocol being used by the socket. See Socket Options.  
*/

#define  EPROTONOSUPPORT ANSI_ENUM_EPROTONOSUPPORT /*!<
The socket domain does not support the requested communications protocol (perhaps because the requested protocol is completely invalid). See Creating a Socket.  
*/

#define  ESOCKTNOSUPPORT ANSI_ENUM_ESOCKTNOSUPPORT /*!<
The socket type is not supported.  
*/

#define  EOPNOTSUPP ANSI_ENUM_EOPNOTSUPP /*!<
The operation you requested is not supported. Some socket functions don't make sense for all types of sockets, and others may not be implemented for all communications protocols. In the GNU system, this error can happen for many calls when the object does not support the particular operation; it is a generic indication that the server knows nothing to do for that call.  
*/

#define  EPFNOSUPPORT ANSI_ENUM_EPFNOSUPPORT /*!<
The socket communications protocol family you requested is not supported.  
*/

#define  EAFNOSUPPORT ANSI_ENUM_EAFNOSUPPORT /*!<
The address family specified for a socket is not supported; it is inconsistent with the protocol being used on the socket. See Sockets.  
*/

#define  EADDRINUSE ANSI_ENUM_EADDRINUSE /*!<
The requested socket address is already in use. See Socket Addresses.  
*/

#define  EADDRNOTAVAIL ANSI_ENUM_EADDRNOTAVAIL /*!<
The requested socket address is not available; for example, you tried to give a socket a name that doesn't match the local host name. See Socket Addresses.  
*/

#define  ENETDOWN ANSI_ENUM_ENETDOWN /*!<
A socket operation failed because the network was down.  
*/

#define  ENETUNREACH ANSI_ENUM_ENETUNREACH /*!<
A socket operation failed because the subnet containing the remote host was unreachable.  
*/

#define  ENETRESET ANSI_ENUM_ENETRESET /*!<
A network connection was reset because the remote host crashed.  
*/

#define  ECONNABORTED ANSI_ENUM_ECONNABORTED /*!<
A network connection was aborted locally.  
*/

#define  ECONNRESET ANSI_ENUM_ECONNRESET /*!<
A network connection was closed for reasons outside the control of the local host, such as by the remote machine rebooting or an unrecoverable protocol violation.  
*/

#define  ENOBUFS ANSI_ENUM_ENOBUFS /*!<
The kernel's buffers for I/O operations are all in use. In GNU, this error is always synonymous with ENOMEM; you may get one or the other from network operations.  
*/

#define  EISCONN ANSI_ENUM_EISCONN /*!<
You tried to connect a socket that is already connected. See Connecting.  
*/

#define  ENOTCONN ANSI_ENUM_ENOTCONN /*!<
The socket is not connected to anything. You get this error when you try to transmit data over a socket, without first specifying a destination for the data. For a connectionless socket (for datagram protocols, such as UDP), you get EDESTADDRREQ instead.  
*/

#define  EDESTADDRREQ ANSI_ENUM_EDESTADDRREQ /*!<
No default destination address was set for the socket. You get this error when you try to transmit data over a connectionless socket, without first specifying a destination for the data with connect.  
*/

#define  ESHUTDOWN ANSI_ENUM_ESHUTDOWN /*!<
The socket has already been shut down.  
*/

#define  ETOOMANYREFS ANSI_ENUM_ETOOMANYREFS /*!<
???  
*/

#define  ETIMEDOUT ANSI_ENUM_ETIMEDOUT /*!<
A socket operation with a specified timeout received no response during the timeout period.  
*/

#define  ECONNREFUSED ANSI_ENUM_ECONNREFUSED /*!<
A remote host refused to allow the network connection (typically because it is not running the requested service).  
*/

#define  ELOOP ANSI_ENUM_ELOOP /*!<
Too many levels of symbolic links were encountered in looking up a file name. This often indicates a cycle of symbolic links.  
*/

#define  ENAMETOOLONG ANSI_ENUM_ENAMETOOLONG /*!<
Filename too long (longer than PATH_MAX; see Limits for Files) or host name too long (in gethostname or sethostname; see Host Identification).  
*/

#define  EHOSTDOWN ANSI_ENUM_EHOSTDOWN /*!<
The remote host for a requested network connection is down.  
*/

#define  EHOSTUNREACH ANSI_ENUM_EHOSTUNREACH /*!<
The remote host for a requested network connection is not reachable.  
*/

#define  ENOTEMPTY ANSI_ENUM_ENOTEMPTY /*!<
Directory not empty, where an empty directory was expected. Typically, this error occurs when you are trying to delete a directory.  
*/

#define  EPROCLIM ANSI_ENUM_EPROCLIM /*!<
This means that the per-user limit on new process would be exceeded by an attempted fork. See Limits on Resources, for details on the RLIMIT_NPROC limit.  
*/

#define  EUSERS ANSI_ENUM_EUSERS /*!<
The file quota system is confused because there are too many users.  
*/

#define  EDQUOT ANSI_ENUM_EDQUOT /*!<
The user's disk quota was exceeded.  
*/

#define  ESTALE ANSI_ENUM_ESTALE /*!<
Stale NFS file handle. This indicates an internal confusion in the NFS system which is due to file system rearrangements on the server host. Repairing this condition usually requires unmounting and remounting the NFS file system on the local host.  
*/

#define  EREMOTE ANSI_ENUM_EREMOTE /*!<
An attempt was made to NFS-mount a remote file system with a file name that already specifies an NFS-mounted file. (This is an error on some operating systems, but we expect it to work properly on the GNU system, making this error code impossible.)  
*/

#define  EBADRPC ANSI_ENUM_EBADRPC /*!<
???  
*/

#define  ERPCMISMATCH ANSI_ENUM_ERPCMISMATCH /*!<
???  
*/

#define  EPROGUNAVAIL ANSI_ENUM_EPROGUNAVAIL /*!<
???  
*/

#define  EPROGMISMATCH ANSI_ENUM_EPROGMISMATCH /*!<
???  
*/

#define  EPROCUNAVAIL ANSI_ENUM_EPROCUNAVAIL /*!<
???  
*/

#define  ENOLCK ANSI_ENUM_ENOLCK /*!<
No locks available. This is used by the file locking facilities; see File Locks. This error is never generated by the GNU system, but it can result from an operation to an NFS server running another operating system.  
*/

#define  EFTYPE ANSI_ENUM_EFTYPE /*!<
Inappropriate file type or format. The file was the wrong type for the operation, or a data file had the wrong format. 
On some systems chmod returns this error if you try to set the sticky bit on a non-directory file; see Setting Permissions. 
*/


#define  EAUTH ANSI_ENUM_EAUTH /*!<
???  
*/

#define  ENEEDAUTH ANSI_ENUM_ENEEDAUTH /*!<
???  
*/

#define  ENOSYS ANSI_ENUM_ENOSYS /*!<
Function not implemented. This indicates that the function called is not implemented at all, either in the C library itself or in the operating system. When you get this error, you can be sure that this particular function will always fail with ENOSYS unless you install a new version of the C library or the operating system.  
*/

#define  ENOTSUP ANSI_ENUM_ENOTSUP /*!<
Not supported. A function returns this error when certain parameter values are valid, but the functionality they request is not available. This can mean that the function does not implement a particular command or option value or flag bit at all. For functions that operate on some object given in a parameter, such as a file descriptor or a port, it might instead mean that only that specific object (file descriptor, port, etc.) is unable to support the other parameters given; different file descriptors might support different ranges of parameter values. 
If the entire function is not available at all in the implementation, it returns ENOSYS instead. 
*/


#define  EILSEQ ANSI_ENUM_EILSEQ /*!<
While decoding a multibyte character the function came along an invalid or an incomplete sequence of bytes or the given wide character is invalid.  
*/

#define  EBACKGROUND ANSI_ENUM_EBACKGROUND /*!<
In the GNU system, servers supporting the term protocol return this error for certain operations when the caller is not in the foreground process group of the terminal. Users do not usually see this error because functions such as read and write translate it into a SIGTTIN or SIGTTOU signal. See Job Control, for information on process groups and these signals.  
*/

#define  EDIED ANSI_ENUM_EDIED /*!<
In the GNU system, opening a file returns this error when the file is translated by a program and the translator program dies while starting up, before it has connected to the file.  
*/

#define  ED ANSI_ENUM_ED /*!<
The experienced user will know what is wrong.  
*/

#define  EGREGIOUS ANSI_ENUM_EGREGIOUS /*!<
You did what?  
*/

#define  EIEIO ANSI_ENUM_EIEIO /*!<
Go home and have a glass of warm, dairy-fresh milk.  
*/

#define  EGRATUITOUS ANSI_ENUM_EGRATUITOUS /*!<
This error code has no purpose.  
*/

#define  EBADMSG ANSI_ENUM_EBADMSG /*!<
TBD
*/

#define  EIDRM ANSI_ENUM_EIDRM /*!<
TBD
*/


#define  EMULTIHOP ANSI_ENUM_EMULTIHOP /*!<
TBD
*/


#define  ENODATA ANSI_ENUM_ENODATA /*!<
TBD
*/


#define  ENOLINK ANSI_ENUM_ENOLINK /*!<
TBD
*/


#define  ENOMSG ANSI_ENUM_ENOMSG /*!<
TBD
*/


#define  ENOSR ANSI_ENUM_ENOSR /*!<
TBD
*/


#define  ENOSTR ANSI_ENUM_ENOSTR /*!<
TBD
*/



#define  EOVERFLOW ANSI_ENUM_EOVERFLOW /*!<
TBD
*/



#define  EPROTO ANSI_ENUM_EPROTO /*!<
TBD
*/



#define  ETIME ANSI_ENUM_ETIME /*!<
TBD
*/



#define  ECANCELED ANSI_ENUM_ECANCELED /*!<
Operation canceled; an asynchronous operation was canceled before it completed. See Asynchronous I/O. When you call aio_cancel, the normal result is for the operations affected to complete with this error; see Cancel AIO Operations.  


The following error codes are defined by the Linux/i386 kernel. They are not yet documented. 

*/

#define  ERESTART ANSI_ENUM_ERESTART /*!<
TBD
*/



#define  ECHRNG ANSI_ENUM_ECHRNG /*!<
TBD
*/



#define  EL2NSYNC ANSI_ENUM_EL2NSYNC /*!<
TBD
*/



#define  EL3HLT ANSI_ENUM_EL3HLT /*!<
TBD
*/



#define  EL3RST ANSI_ENUM_EL3RST /*!<
TBD
*/



#define  ELNRNG ANSI_ENUM_ELNRNG /*!<
TBD
*/



#define  EUNATCH ANSI_ENUM_EUNATCH /*!<
TBD
*/



#define  ENOCSI ANSI_ENUM_ENOCSI /*!<
TBD
*/



#define  EL2HLT ANSI_ENUM_EL2HLT /*!<
TBD
*/



#define  EBADE ANSI_ENUM_EBADE /*!<
TBD
*/



#define  EBADR ANSI_ENUM_EBADR /*!<
TBD
*/



#define  EXFULL ANSI_ENUM_EXFULL /*!<
TBD
*/



#define  ENOANO ANSI_ENUM_ENOANO /*!<
TBD
*/



#define  EBADRQC ANSI_ENUM_EBADRQC /*!<
TBD
*/



#define  EBADSLT ANSI_ENUM_EBADSLT /*!<
TBD
*/



#define  EDEADLOCK ANSI_ENUM_EDEADLOCK /*!<
TBD
*/



#define  EBFONT ANSI_ENUM_EBFONT /*!<
TBD
*/



#define  ENONET ANSI_ENUM_ENONET /*!<
TBD
*/



#define  ENOPKG ANSI_ENUM_ENOPKG /*!<
TBD
*/



#define  EADV ANSI_ENUM_EADV /*!<
TBD
*/



#define  ESRMNT ANSI_ENUM_ESRMNT /*!<
TBD
*/



#define  ECOMM ANSI_ENUM_ECOMM /*!<
TBD
*/



#define  EDOTDOT ANSI_ENUM_EDOTDOT /*!<
TBD
*/



#define  ENOTUNIQ ANSI_ENUM_ENOTUNIQ /*!<
TBD
*/



#define  EBADFD ANSI_ENUM_EBADFD /*!<
TBD
*/



#define  EREMCHG ANSI_ENUM_EREMCHG /*!<
TBD
*/



#define  ELIBACC ANSI_ENUM_ELIBACC /*!<
TBD
*/



#define  ELIBBAD ANSI_ENUM_ELIBBAD /*!<
TBD
*/



#define  ELIBSCN ANSI_ENUM_ELIBSCN /*!<
TBD
*/



#define  ELIBMAX ANSI_ENUM_ELIBMAX /*!<
TBD
*/



#define  ELIBEXEC ANSI_ENUM_ELIBEXEC /*!<
TBD
*/



#define  ESTRPIPE ANSI_ENUM_ESTRPIPE /*!<
TBD
*/



#define  EUCLEAN ANSI_ENUM_EUCLEAN /*!<
TBD
*/



#define  ENOTNAM ANSI_ENUM_ENOTNAM /*!<
TBD
*/



#define  ENAVAIL ANSI_ENUM_ENAVAIL /*!<
TBD
*/



#define  EISNAM ANSI_ENUM_EISNAM /*!<
TBD
*/



#define  EREMOTEIO ANSI_ENUM_EREMOTEIO /*!<
TBD
*/



#define  ENOMEDIUM ANSI_ENUM_ENOMEDIUM /*!<
TBD
*/



#define  EMEDIUMTYPE ANSI_ENUM_EMEDIUMTYPE /*!<
TBD
*/

#endif // !(__GNUC__)

#endif //ERRNO_H

/*!
 * @defgroup CVSLOG_errno_h errno_h
 * @ingroup CVSLOG
 *  $Log: errno.h,v $
 *  Revision 1.15  2006-12-12 10:57:04  ambrmi09
 *  This adresses the second part of #1609064
 *
 *  Revision 1.14  2006/12/11 14:41:51  ambrmi09
 *  Solves #1609064 (part1)
 *
 *  Revision 1.13  2006/11/27 22:29:22  ambrmi09
 *  Minor djustments completeing the move of some header files to public and due
 *  to some name clashed with user space naming conventions.
 *
 *  Revision 1.12  2006/04/08 10:15:57  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.11.2.1  2006/04/03 20:07:22  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.11  2006/03/19 12:44:35  ambrmi09
 *  Got rid of many compilation warnings. MSVC amd GCC actually gompiles
 *  without one single warning (yay!). Be aware that ther was a lot of
 *  comparisons between signed/unsigned in ITC. Fetts a bit shaky...
 *
 *  Revision 1.10  2006/03/05 11:11:24  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.9  2006/03/04 14:28:44  ambrmi09
 *  Finally got the \ref clock() representation right. Now timing is
 *  behaving equaly between the targets X86_Linux, Cygqing, MSVC, BC5 and
 *  XC167.
 *
 *  Revision 1.8  2006/03/02 14:05:48  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.7  2006/02/22 13:05:45  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  
 */


