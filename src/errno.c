/***************************************************************************
 *   Copyright (C) 2006 by Michale Ambrus                                  *
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

@brief Contains storage for errno strings

Contains storage for errno strings

For in-depth discussions about this package, see \ref
kernel_reimpl_ansi

@see PACKAGES
*/

#include <pthread.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>

void perror (const char *message); //Should go int stdio.h
char * strerror (int errnum);      //Should go int string.h 

#define MAX_STR_LEN 64

static const char *error_srings[] = {
"TK_OK",
"TK_ERROR",
"ERR_TIMEOUT",
"ERR_OBJDEL",
"ERR_OBJID",
"ERR_OBJTYPE",
"ERR_OBJFULL",
"ERR_BUFSIZ",
"ERR_NOQCB",
"ERR_FULL",
"ERR_NOSCB",
"ERR_NOSEM",
"ERR_TATSDEL",
"ERR_NOMEM",
"ERR_BLOCKLIMIT",
"ERR_UNDEF_PTIMER",
"ERR_TIME_NEG",
"ERR_TIMER_DELETED",
"ERR_NO_MORE_TIMERS",
"ERR_UNDEF_HEAPID",
"EPERM",
"ENOENT",
"ESRCH",
"EINTR",
"EIO",
"ENXIO",
"E2BIG",
"ENOEXEC",
"EBADF",
"ECHILD",
"EDEADLK",
"ENOMEM",
"EACCES",
"EFAULT",
"ENOTBLK",
"EBUSY",
"EEXIST",
"EXDEV",
"ENODEV",
"ENOTDIR",
"EISDIR",
"EINVAL",
"EMFILE",
"ENFILE",
"ENOTTY",
"ETXTBSY",
"EFBIG",
"ENOSPC",
"ESPIPE",
"EROFS",
"EMLINK",
"EPIPE",
"EDOM",
"ERANGE",
"EAGAIN",
"EWOULDBLOCK",
"EINPROGRESS",
"EALREADY",
"ENOTSOCK",
"EMSGSIZE",
"EPROTOTYPE",
"ENOPROTOOPT",
"EPROTONOSUPPORT",
"ESOCKTNOSUPPORT",
"EOPNOTSUPP",
"EPFNOSUPPORT",
"EAFNOSUPPORT",
"EADDRINUSE",
"EADDRNOTAVAIL",
"ENETDOWN",
"ENETUNREACH",
"ENETRESET",
"ECONNABORTED",
"ECONNRESET",
"ENOBUFS",
"EISCONN",
"ENOTCONN",
"EDESTADDRREQ",
"ESHUTDOWN",
"ETOOMANYREFS",
"ETIMEDOUT",
"ECONNREFUSED",
"ELOOP",
"ENAMETOOLONG",
"EHOSTDOWN",
"EHOSTUNREACH",
"ENOTEMPTY",
"EPROCLIM",
"EUSERS",
"EDQUOT",
"ESTALE",
"EREMOTE",
"EBADRPC",
"ERPCMISMATCH",
"EPROGUNAVAIL",
"EPROGMISMATCH",
"EPROCUNAVAIL",
"ENOLCK",
"EFTYPE",
"EAUTH",
"ENEEDAUTH",
"ENOSYS",
"ENOTSUP",
"EILSEQ",
"EBACKGROUND",
"EDIED",
"ED",
"EGREGIOUS",
"EIEIO",
"EGRATUITOUS",
"EBADMSG",
"EIDRM",
"EMULTIHOP",
"ENODATA",
"ENOLINK",
"ENOMSG",
"ENOSR",
"ENOSTR",
"EOVERFLOW",
"EPROTO",
"ETIME",
"ECANCELED",
"ERESTART",
"ECHRNG",
"EL2NSYNC",
"EL3HLT",
"EL3RST",
"ELNRNG",
"EUNATCH",
"ENOCSI",
"EL2HLT",
"EBADE",
"EBADR",
"EXFULL",
"ENOANO",
"EBADRQC",
"EBADSLT",
"EDEADLOCK",
"EBFONT",
"ENONET",
"ENOPKG",
"EADV",
"ESRMNT",
"ECOMM",
"EDOTDOT",
"ENOTUNIQ",
"EBADFD",
"EREMCHG",
"ELIBACC",
"ELIBBAD",
"ELIBSCN",
"ELIBMAX",
"ELIBEXEC",
"ESTRPIPE",
"EUCLEAN",
"ENOTNAM",
"ENAVAIL",
"EISNAM",
"EREMOTEIO",
"ENOMEDIUM",
"EMEDIUMTYPE"
}; 

void perror (const char *message){
   char *ptr;
   unsigned long msgpti = (unsigned long)message;

   msgpti &= 0x0000FFFF;
   msgpti |= 0x00C00000;
   ptr = (char*)msgpti;

   printf(">>> TinKer Error: [%s] <<<\n",ptr);
}

char * strerror (int errnum){
   char *ptr;

   ptr = error_srings[errnum];
   return ptr;
}


  
/*! 
 *  @defgroup CVSLOG_errno_c errno_c
 *  @ingroup CVSLOG
 *  $Log: errno.c,v $
 *  Revision 1.4  2006-03-07 08:24:13  ambrmi09
 *  A very crude port for ARM is running (LPC2129) - @note THIS IS HIGHLY EXPERIMENTAL CODE
 *
 *  Revision 1.3  2006/03/05 11:11:27  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.2  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.1  2006/02/20 19:12:25  ambrmi09
 *  initial revision
 *
 *  
 *******************************************************************/
 













