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
#ifndef _IMPLEMENT_TK_H
#define _IMPLEMENT_TK_H

#include "tk_tuning.h"
#include "stypes.h"
typedef enum {
	READY = 0x00,
	_______T = 0x01,
	______S_ = 0x02,
	______ST = 0x03,
	_____Q__ = 0x04,
	_____Q_T = 0x05,
	_____QS_ = 0x06,
	_____QST = 0x07,
	____Z___ = 0x08,
	____Z__T = 0x09,
	____Z_S_ = 0x0A,
	____Z_ST = 0x0B,
	____ZQ__ = 0x0C,
	____ZQ_T = 0x0D,
	____ZQS_ = 0x0E,
	____ZQST = 0x0F
} PROCSTATE;

#define TERM _______T
#define SLEEP ______S_
#define QUEUE _____Q__
#define ZOMBI ____Z___
typedef enum { E_NONE, E_CHILDDEATH, E_TIMER, E_ITC, E_ITC2 } wakeE_t;
typedef enum {
	BON_SCHED = 0,
	BON_ITC,
	BON_PMUTEX,
	BON_SEMAPHORE,
	BON_PTIMER
} bon_sel_t;
typedef struct bon_t_ {
	bon_sel_t kind;
	union {
		struct tcb_t_ *tcb;
		struct itc_t_ *itc;
		struct pthread_mutex_t_ *mutex;
	} entity;
} bon_t;
typedef struct tcb_t_ {
	thid_t Thid, Gid;
	int noChilds;
	char name[TK_THREAD_NAME_LEN + 1];
	TK_BOOL valid;
	PROCSTATE state;
	bon_t bOnId;

	int _errno_;
	stack_t stack_begin;
	stack_t curr_sp;
	size_t stack_size;
	unsigned long stack_crc;
	clock_t wakeuptime;
	wakeE_t wakeupEvent;
	void *retval;

	unsigned int Prio, Idx;
} tk_tcb_t;

typedef struct stat_t {
	unsigned short procs_at_prio;
	unsigned short curr_idx;
} prio_stat_t;

struct tcb_t_ *_tk_current_tcb(void);
struct tcb_t_ *_tk_specific_tcb(thid_t id);
void _tk_main(void);
void tk_trap(int ec);
int _tk_try_detach_parent(thid_t, int);
thid_t _tk_next_runable_thread(void);
void _tk_context_switch_to_thread(thid_t, thid_t);

extern int root(void);

extern int __tk_IntFlagCntr;

#if (TK_HOWTO_CLOCK == TK_FNK_STUBBED)
clock_t clock_stubbed();
#define tk_clock() clock_stubbed()
#else
#define tk_clock() clock()
#endif

#if defined(TK_HOSTED)

#define tk_difftime difftime
#else
static time_t _tk_difftime(time_t t1, time_t t0)
{
	return t1 - t0;
}

#define tk_difftime _tk_difftime
#endif

#endif
