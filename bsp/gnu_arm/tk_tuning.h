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
#ifndef TK_MINIMUM_STACK_SIZE
#define TK_MINIMUM_STACK_SIZE 	0x0300
#endif
#ifndef TK_NORMAL_STACK_SIZE
#define TK_NORMAL_STACK_SIZE  	0x0300
#endif

//SCHED
#ifndef TK_MAX_THREADS
#define TK_MAX_THREADS          10
#endif
#ifndef TK_MAX_PRIO_LEVELS
#define TK_MAX_PRIO_LEVELS      16
#endif
#ifndef TK_MAX_THREADS_AT_PRIO
#define TK_MAX_THREADS_AT_PRIO  3
#endif
#ifndef TK_THREAD_NAME_LEN
#define TK_THREAD_NAME_LEN      0x17
#endif

//ITC
#ifndef TK_MAX_BLOCKED_ON_Q
#define TK_MAX_BLOCKED_ON_Q    	TK_MAX_THREADS
#endif
#ifndef TK_MAX_NUM_Q
#define TK_MAX_NUM_Q       		10
#endif

/*
#define TK_HOWTO_MALLOC 		TK_FNK_ORIGINAL
#define TK_HOWTO_CLOCK  		TK_FNK_STUBBED
#define TK_HOWTO_PRINTK 		TK_FNK_VOIDED
#define TK_HOWTO_ASSERT 		TK_FNK_RENAMED
*/

#define TK_HOWTO_MALLOC 		TK_FNK_ORIGINAL
//#define TK_HOWTO_CLOCK                TK_FNK_STUBBED
#define TK_HOWTO_CLOCK  		TK_FNK_ORIGINAL
#define TK_HOWTO_PRINTK 		TK_FNK_RENAMED
#define TK_HOWTO_ASSERT 		TK_FNK_RENAMED

#define TRAP( NUM )     \
   tk_trap( NUM )

#endif				//TK_TUNING_H

