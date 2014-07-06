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
#if !defined( SEMAPHORE_H )
#define SEMAPHORE_H

#if defined(__GNUC__)
#include <tinker/config.h>
#if (!defined(TK_COMP_POSIX_RT) || TK_COMP_POSIX_RT==0 ) && !defined(ECODES_ONLY)
#error "<semaphore.h> belongs to a component that your build of TinKer didn't include. Please reconfigure and rebuild TinKer."
#endif
#endif

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

	typedef unsigned long sem_t;

	int sem_init(sem_t * sem, int pshared, unsigned int value);
	int sem_destroy(sem_t * sem);
	int sem_trywait(sem_t * sem);
	int sem_wait(sem_t * sem);
	int sem_post(sem_t * sem);

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#endif
