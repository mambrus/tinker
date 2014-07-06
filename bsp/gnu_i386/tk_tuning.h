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

//------1---------2---------3---------4---------5---------6---------7---------8
#ifndef TK_TUNING_H
#define TK_TUNING_H

#if defined(__GNUC__)
#include <tinker/config.h>
#endif

//App & system
#if      defined(__CYGWIN32__)  || defined(__CYGWIN__)
#ifndef TK_NORMAL_STACK_SIZE
#define TK_NORMAL_STACK_SIZE     0xD000	//!< @note Cygwin needs a humongus stack
#endif
#ifndef TK_MINIMUM_STACK_SIZE
#define TK_MINIMUM_STACK_SIZE    0x0D00	//!< TBD this @todo TBD this
#endif

#else
#ifndef TK_NORMAL_STACK_SIZE
#define TK_NORMAL_STACK_SIZE     0x1200	//!< @note Whats normal or reasonable differs between architectures.
#endif
#ifndef TK_MINIMUM_STACK_SIZE
#define TK_MINIMUM_STACK_SIZE    0x0600	//!< TBD this @todo TBD this
#endif

#endif

//SCHED
#ifndef TK_MAX_THREADS
#define TK_MAX_THREADS          0x100
#endif
#ifndef TK_MAX_PRIO_LEVELS
#define TK_MAX_PRIO_LEVELS      0x10
#endif
#ifndef TK_MAX_THREADS_AT_PRIO
#define TK_MAX_THREADS_AT_PRIO  TK_MAX_THREADS
#endif
#ifndef TK_THREAD_NAME_LEN
#define TK_THREAD_NAME_LEN      0x27
#endif

//ITC
#ifndef TK_MAX_BLOCKED_ON_Q
#define TK_MAX_BLOCKED_ON_Q    	TK_MAX_THREADS
#endif
#ifndef TK_MAX_NUM_Q
#define TK_MAX_NUM_Q       		0x100
#endif

#define TRAP( NUM )     \
   tk_trap( NUM )

#endif				//TK_TUNING_H

