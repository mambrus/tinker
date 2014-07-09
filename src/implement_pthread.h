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
#ifndef _IMPLEMENT_PTHREAD_H
#define _IMPLEMENT_PTHREAD_H
#include "tk_tuning.h"
#include "stypes.h"

#ifndef TK_MAX_THREADS
#error TK_MAX_THREADS not defined. Did you --enable-max_threads=<val> properly?
#endif
struct pthread_attr_t_ {
	unsigned long valid;
	tk_stack_t stackaddr;
	size_t stacksize;
	int detachstate;
	int priority;
};

struct pthread_key_t_ {
	int TBD_THIS_STRUCT;
};

typedef enum { BSINGLE = 0, BCAST } bcast_t;

int _mutex_lock_primitive(pthread_mutex_t * mutex);
int _mutex_unlock_primitive(pthread_mutex_t * mutex, bcast_t bcast);
#define MAKE_SOFTWARE_EXCEPTION(_severity,_facility,_exception) \
( (DWORD) ( ( (_severity) << 30 ) | \
    ( 1 << 29 ) | \
    ( 0 << 28 ) | \
    ( (_facility) << 16 ) | \
    ( (_exception) << 0 ) \
    ) )
#endif
