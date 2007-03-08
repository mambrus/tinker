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
@ingroup POSIX_RT

@brief RT queues - POSIX 1003.1b API

\code
This file was donated courtesy of the Zoi project (year 2000,
SIEMENS Elema). It was originally written as a wrapper for
pThreads-win32, to give Windows threads the ability of POSIX RT queues.
\endcode

For in-depth discussions about this component, see \ref
POSIX_RT

@see POSIX_RT
@see PTHREAD
*/
 
#ifndef _MQUEUE_H
#define _MQUEUE_H

#if defined(__GNUC__)
   #include <tinker/config.h>
   #if (!defined(TK_COMP_POSIX_RT) || TK_COMP_POSIX_RT==0 ) && !defined(ECODES_ONLY)
   #error "mqueue.h" belongs to a component that your build of TinKer didn't include. Please reconfigure and rebuild TinKer.
   #endif
#endif
/*****************************************************************************
 * include files
 *****************************************************************************/
#include <sys/types.h>
#include <sys/fcntl.h>
#include <stddef.h>

/*****************************************************************************
 * definitions
 *****************************************************************************/
 /*****************************************************************************
 * definitions
 *****************************************************************************/
typedef int mqd_t;

//#ifdef _MSVC_
#if defined(_WIN32) &&  defined(_MSC_VER)
#  define HAVE_MODE_T     //!< This might be obsolete. Used to overcome glitch in pThreads W32 semaphore.h
#  define EMSGSIZE     0x7F00
#   if !defined(TINKER)
       typedef int mode_t;     /*Belonging bits below*/
#   endif
#else

#endif

struct mq_attr{
   size_t mq_msgsize;
   size_t mq_maxmsg;
};
 
/*mq_close*/
/*EBADF */

/*mq_getattr*/
/*EBADF */

/*mq_open*/



/*
EXIST
ENAMETOOLONG 
ENOENT
ENOSPC
*/

/*mq_receive*/
/*
EAGAIN
EBADF
EMSGSIZE
*/

/* mq_send */
/*
EAGAIN
EBADF
EMSGSIZE
EINVAL
*/

/* mq_unlink */
/*ENOENT*/

/*****************************************************************************
 * public function declarations
 *****************************************************************************/

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

int mq_close(
   mqd_t                 mq
);

int mq_getattr(
   mqd_t                mq,
   struct mq_attr      *attrbuf
);

mqd_t mq_open(
   const char           *mq_name,
   int                   oflags,
   mode_t                mode,
   struct mq_attr       *mq_attr
);

size_t mq_receive(
   mqd_t                 mq,
   char                 *msg_buffer,
   size_t                buflen,
   unsigned int         *msgprio
);

int mq_setattr(
   mqd_t                 mqdes,
   const struct mq_attr *new_attrs,
   struct mq_attr       *old_attrs
);


int mq_send(
   mqd_t                 mq,
   const char           *msg,
   size_t                msglen,
   unsigned int          msgprio
);

int mq_unlink(
   const char           *mq_name
);

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif

#endif


/*
//Compact format so that TiNa can find the functions
int mq_close(mqd_t mq);
int mq_getattr(mqd_t mq, struct mq_attr *attrbuf);
mqd_t mq_open(const char *mq_name, int oflags,mode_t mode,struct mq_attr *mq_attr);
size_t mq_receive(mqd_t mq,char *msg_buffer,size_t buflen,unsigned int *msgprio);
int mq_setattr(mqd_t mqdes, const struct mq_attr *new_attrs,struct mq_attr *old_attrs);
int mq_send(mqd_t mq, const char *msg, size_t msglen, unsigned int msgprio);
int mq_unlink(const char *mq_name);
*/

/*!
 * @defgroup CVSLOG_mqueue_h mqueue_h
 * @ingroup CVSLOG
 *  $Log: mqueue.h,v $
 *  Revision 1.12  2007-03-08 23:28:35  ambrmi09
 *  Minor changes made in TinKer headers for TiNa code ceneration to work
 *
 *  Revision 1.11  2007-03-03 23:01:32  ambrmi09
 *  Added driver support for FIFO's
 *
 *  Revision 1.10  2006-12-12 10:57:05  ambrmi09
 *  This adresses the second part of #1609064
 *
 *  Revision 1.9  2006/11/05 19:06:03  ambrmi09
 *  Buildsystem adjusted to permit configuration of components.
 *  Now when component is enabled it will also be included in the build
 *  (instead of just sanity-tested in the source files).
 *
 *  Also a feature for application sanity is assed. When a header-file is
 *  included in the application, a check against the component it belongs
 *  to will be performed. That way user don't need to rely on run-time
 *  checks and can get feedback much earlier.
 *
 *  Revision 1.8  2006/04/08 10:15:57  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.7.2.1  2006/04/03 20:07:22  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.7  2006/03/17 12:20:02  ambrmi09
 *  Major uppdate (5 days hard work)
 *
 *  - Finally tied up all loose ends in the concept. Threads are now
 *  joinable
 *
 *  - Corrected one error: compacting scheduele while cancelling a
 *  threads
 *
 *  - Several new API, mainly concerned with cancelation (corrsp pThread
 *  also)
 *
 *  - Found a nasty bug while creating threads in threads for XC167. TOS is
 *  really a patchy solution ;( This one had to do with the compiler
 *  being fooled by the inline assembly and optimized something that was not
 *  optimizable (saving stack segment got wacked).
 *
 *  - Designed a concurrent qsort test-app. This is good for showing
 *  boss-worker model. Number of threads recoed on XC167 was 50 and on MSVS
 *  more than 150! Interesting to notice was that TinKer creation and
 *  cancelation for threads was much faster than Windows own (20-30 times
 *  faster).
 *
 *  - A MSVC workspace for pThreads-Win32. Good for testing apps
 *  transparency.
 *
 *  - Increased memory on XC167 (phyCore HW). now 32k for stacks and 16k for
 *  malloc. We still lack RAM that is not deployed (pHycore has
 *  128k + 256k physical RAM memory i think). Maximum for
 *  stack is 64k however (type of pointers determine this). If memory is
 *  increased further, we get a TRAP_B saying bad memory interface. Typical
 *  error for config memory issues in DaVe.
 *
 *  Revision 1.6  2006/03/05 11:11:24  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.5  2006/02/23 11:34:59  ambrmi09
 *  - Improved post mortem
 *   - Fixed bug in i2hex2.
 *   - Added uptime output
 *   - mark running among RDY threads in schedule dump
 *
 *  - \ref putchar now supports easy switching between serial0 and serial1
 *
 *  mqueue.h and mqueue.c should be untouched. But trying to identify if
 *  qsort is the reason for TinKer sometimes to hang (recent possible bug).
 *
 *  Revision 1.4  2006/02/22 13:05:45  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 * Revision 1.4  2006/02/22 13:05:45  ambrmi09
 * Major doxygen structure modification. No chancge in actual sourcecode.
 *
 * Revision 1.3  2006/02/21 22:10:31  ambrmi09
 * - Added wrapper macro for pthread_create so that posix threads get named in
 *   TinKer (makes post-mortem easier). Very cool solution with a macro...
 * - Improved post-mortem, the schedule gets dumpt also now
 * - Wrapper macros for msleep and usleep (temporary)
 * - Minor stubbing and wrapping of mq_unlink and pthread_cancel
 * - Added a new test program (t est-posix.c ). This is verifyed to compile and
 *   run on both Linux and TinKer unmodified!
 *
 * Revision 1.2  2006/02/20 15:22:00  ambrmi09
 * Documentation stuff. No code changes.
 *
 * Revision 1.1  2006/02/17 21:16:16  ambrmi09
 * Initial commit: code donated by the Zoi project (author: Michael Ambrus)
 *
 */

