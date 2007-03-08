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

@brief RT semaphores - POSIX 1003.1b API

TBD

For in-depth discussions about this component, see \ref
POSIX_RT

@see POSIX_RT
@see PTHREAD
*/

#if !defined( SEMAPHORE_H )
#define SEMAPHORE_H

#if defined(__GNUC__)
   #include <tinker/config.h>
   #if (!defined(TK_COMP_POSIX_RT) || TK_COMP_POSIX_RT==0 ) && !defined(ECODES_ONLY)
   #error "semaphore.h" belongs to a component that your build of TinKer didn't include. Please reconfigure and rebuild TinKer.
   #endif
#endif


#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif




typedef unsigned long sem_t;

int sem_init (sem_t * sem, int pshared, unsigned int value );
int sem_destroy (sem_t * sem );
int sem_trywait (sem_t * sem );
int sem_wait (sem_t * sem );
int sem_post (sem_t * sem);

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif

#endif

/*!
 * @defgroup CVSLOG_semaphore_h semaphore_h
 * @ingroup CVSLOG
 *  $Log: semaphore.h,v $
 *  Revision 1.9  2007-03-08 23:28:35  ambrmi09
 *  Minor changes made in TinKer headers for TiNa code ceneration to work
 *
 *  Revision 1.8  2006-12-12 10:57:05  ambrmi09
 *  This adresses the second part of #1609064
 *
 *  Revision 1.7  2006/11/05 19:06:03  ambrmi09
 *  Buildsystem adjusted to permit configuration of components.
 *  Now when component is enabled it will also be included in the build
 *  (instead of just sanity-tested in the source files).
 *
 *  Also a feature for application sanity is assed. When a header-file is
 *  included in the application, a check against the component it belongs
 *  to will be performed. That way user don't need to rely on run-time
 *  checks and can get feedback much earlier.
 *
 *  Revision 1.6  2006/04/08 10:15:58  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.5.2.1  2006/04/03 20:07:23  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.5  2006/03/05 11:11:24  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.4  2006/02/22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  
 */

