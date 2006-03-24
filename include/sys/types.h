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


/*!
@name 

These defines makes sure we're using our own types no matter what
tool-chain you're using building TinKer and (any app using TinKer).

In case of GNU tool-chain (but possibly some others too), there is a
high probability that these types are already defined. Since we're in
fact implementing pThreads, we need to have out own types. Anyone
needing the GCC pthread types are not likely to use TinKer anyway. Note
that both in app and kernel internals, the types seem to be called the
normal pthread names (very nifty!).

@see COMPONENTS
*/
//@{
//#define pthread_t_            tk_pthread_t_
#define pthread_attr_t_       tk_pthread_attr_t_
#define pthread_once_t_       tk_pthread_once_t_
#define pthread_key_t_        tk_pthread_key_t_
#define pthread_mutex_t_      tk_pthread_mutex_t_
#define pthread_mutexattr_t_  tk_pthread_mutexattr_t_
#define pthread_cond_t_       tk_pthread_cond_t_
#define pthread_condattr_t_   tk_pthread_condattr_t_

#define pthread_t             tk_pthread_t
#define pthread_attr_t        tk_pthread_attr_t
#define pthread_once_t        tk_pthread_once_t
#define pthread_key_t         tk_pthread_key_t
#define pthread_mutex_t       tk_pthread_mutex_t
#define pthread_mutexattr_t   tk_pthread_mutexattr_t
#define pthread_cond_t        tk_pthread_cond_t
#define pthread_condattr_t    tk_pthread_condattr_t
#define pthread_rwlock_t      tk_pthread_rwlock_t
#define pthread_rwlockattr_t  tk_pthread_rwlockattr_t
//@}



#if !defined (__GNUC__)
   /*!
   This is an integer data type used to represent file modes. In the GNU
   system, this is equivalent to unsigned int.
   */
   typedef unsigned int mode_t;
#endif   //defined (_GNU_)


   //typedef struct pthread_t_ *pthread_t; //No way I'm going to return a pointer to anything for this!
   //typedef unsigned int pthread_t;
   typedef struct tcb_t_               *pthread_t; 
   typedef struct pthread_attr_t_      *pthread_attr_t;
   typedef struct pthread_once_t_       pthread_once_t;
   typedef struct pthread_key_t_       *pthread_key_t;
   typedef struct pthread_mutex_t_      pthread_mutex_t;
   typedef struct pthread_mutexattr_t_  pthread_mutexattr_t;
   typedef struct pthread_cond_t_       pthread_cond_t;
   typedef struct pthread_condattr_t_   pthread_condattr_t;
   typedef struct pthread_rwlock_t_     pthread_rwlock_t;
   typedef struct pthread_rwlockattr_t_ pthread_rwlockattr_t;

#endif //SYS_TYPES_H


/*!
 * @defgroup CVSLOG_types_h types_h
 * @ingroup CVSLOG
 *  $Log: types.h,v $
 *  Revision 1.8  2006-03-24 11:22:55  ambrmi09
 *  - pThreads RW locks implemented (rough aproach - no usage error detection)
 *  - restructuring of the pThread src-files
 *
 *  Revision 1.7  2006/03/19 22:57:54  ambrmi09
 *  First naive implementation of a pthread mutex
 *
 *  Revision 1.6  2006/03/17 14:18:42  ambrmi09
 *  Finalized pThreads and RT gueues for GNU build-chain
 *
 *  Revision 1.5  2006/03/05 11:11:26  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.4  2006/03/02 14:05:49  ambrmi09
 *  Posting to GNU toolchain started
 *
 *  Revision 1.3  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  
 */

