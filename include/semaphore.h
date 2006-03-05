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
@ingroup POSIX_RT

@brief RT semaphores - POSIX 1003.1b API

TBD

For in-depth discussions about this component, see \ref
POSIX_RT

@see POSIX_RT
@see PTHREAD
*/

#if !defined( SEMAPHORE_H )
#define SEMAPHORE_H

//#include <process.h>


#ifdef __cplusplus
extern "C"
{
#endif                          

typedef unsigned long sem_t;

int sem_init (sem_t * sem,
	      int pshared,
	      unsigned int value
	      );

int sem_destroy (sem_t * sem
		 );

int sem_trywait (sem_t * sem
		 );

int sem_wait (sem_t * sem
	      );

int sem_post (sem_t * sem
	      );

#ifdef __cplusplus
}                               
#endif                          

#endif                          

/*!
 * @defgroup CVSLOG_semaphore_h semaphore_h
 * @ingroup CVSLOG
 *  $Log: semaphore.h,v $
 *  Revision 1.5  2006-03-05 11:11:24  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.4  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  
 */

