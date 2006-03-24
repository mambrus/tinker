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
@ingroup PTHREAD_SYNC

@brief This implements the attribute API for pThread mutexes


For in-depth discussions about this component, see \ref
PTHREAD_SYNC

@see PTHREAD_SCHED
@see PTHREAD

*/

#include <pthread.h>
#include <errno.h>
#include <assert.h>

#include "implement_pthread.h"
#include "implement_tk.h"


//------1---------2---------3---------4---------5---------6---------7---------8

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_mutexattr_init.html

@todo Stubbed. Impl TBD
*/
int pthread_mutexattr_init (pthread_mutexattr_t *__attr){
   assert("Not implemented yet" == 0); 
   return 0;
}

/*!
http://www.opengroup.org/onlinepubs/009695399/functions/pthread_mutexattr_destroy.html

@todo Stubbed. Impl TBD
*/
int pthread_mutexattr_destroy (pthread_mutexattr_t *__attr){
   assert("Not implemented yet" == 0); 
   return 0;
}
/*
int pthread_mutexattr_getpshared (__const pthread_mutexattr_t *
int pthread_mutexattr_setpshared (pthread_mutexattr_t *__attr,
int pthread_mutexattr_settype (pthread_mutexattr_t *__attr, int __kind)
int pthread_mutexattr_gettype (__const pthread_mutexattr_t *__restrict
*/

//------1---------2---------3---------4---------5---------6---------7---------8

  
/*!
 *  @defgroup CVSLOG_pthread_attr_mutex_c pthread_attr_mutex_c
 *  @ingroup CVSLOG
 *  $Log: pthread_attr_mutex.c,v $
 *  Revision 1.2  2006-03-24 17:40:19  ambrmi09
 *  Cosmetic details
 *
 *  Revision 1.1  2006/03/24 11:22:56  ambrmi09
 *  - pThreads RW locks implemented (rough aproach - no usage error detection)
 *  - restructuring of the pThread src-files
 *
 *  
 *******************************************************************/
 













