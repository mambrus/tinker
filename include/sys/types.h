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
#if defined (__GNUC__)
#include <tk_ansi.h>
#include <tinker/config.h>
#include <tk_ansi_dirwrap.h>

#define __DEV_T void*

#include BUILDCHAIN_SYS_INCLUDE(types.h)

#endif

#ifndef SYS_TYPES_H_TK
#define SYS_TYPES_H_TK
#define pthread_attr_t_ tk_pthread_attr_t_
#define pthread_once_t_ tk_pthread_once_t_
#define pthread_key_t_ tk_pthread_key_t_
#define pthread_mutex_t_ tk_pthread_mutex_t_
#define pthread_mutexattr_t_ tk_pthread_mutexattr_t_
#define pthread_cond_t_ tk_pthread_cond_t_
#define pthread_condattr_t_ tk_pthread_condattr_t_

#define pthread_t tk_pthread_t
#define pthread_attr_t tk_pthread_attr_t
#define pthread_once_t tk_pthread_once_t
#define pthread_key_t tk_pthread_key_t
#define pthread_mutex_t tk_pthread_mutex_t
#define pthread_mutexattr_t tk_pthread_mutexattr_t
#define pthread_cond_t tk_pthread_cond_t
#define pthread_condattr_t tk_pthread_condattr_t
#define pthread_rwlock_t tk_pthread_rwlock_t
#define pthread_rwlockattr_t tk_pthread_rwlockattr_t

#if !defined (__GNUC__)

typedef unsigned int mode_t;
#endif

typedef struct tcb_t_ *pthread_t;
typedef struct pthread_attr_t_ *pthread_attr_t;
typedef struct pthread_once_t_ pthread_once_t;
typedef struct pthread_key_t_ *pthread_key_t;
typedef struct pthread_mutex_t_ pthread_mutex_t;
typedef struct pthread_mutexattr_t_ pthread_mutexattr_t;
typedef struct pthread_cond_t_ pthread_cond_t;
typedef struct pthread_condattr_t_ pthread_condattr_t;
typedef struct pthread_rwlock_t_ pthread_rwlock_t;
typedef struct pthread_rwlockattr_t_ pthread_rwlockattr_t;

#endif
