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
#error "<mqueue.h> belongs to a component that your build of TinKer didn't include. Please reconfigure and rebuild TinKer."
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
#define HAVE_MODE_T		//!< This might be obsolete. Used to overcome glitch in pThreads W32 semaphore.h
#define EMSGSIZE     0x7F00
#if !defined(TINKER)
typedef int mode_t;		/*Belonging bits below */
#endif
#else

#endif

struct mq_attr {
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

	int mq_close(mqd_t mq);

	int mq_getattr(mqd_t mq, struct mq_attr *attrbuf);

	mqd_t mq_open(const char *mq_name,
		      int oflags, mode_t mode, struct mq_attr *mq_attr);

	size_t mq_receive(mqd_t mq,
			  char *msg_buffer,
			  size_t buflen, unsigned int *msgprio);

	int mq_setattr(mqd_t mqdes,
		       const struct mq_attr *new_attrs,
		       struct mq_attr *old_attrs);

	int mq_send(mqd_t mq,
		    const char *msg, size_t msglen, unsigned int msgprio);

	int mq_unlink(const char *mq_name);

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
