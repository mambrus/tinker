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
#ifndef TK_PTIMER_H
#define TK_PTIMER_H

#if defined(__GNUC__)
#include <tinker/config.h>
#if (!defined(TK_COMP_PTIMER) || (TK_COMP_PTIMER==0) ) && !defined(ECODES_ONLY)
#error "tk_ptime.h" belongs to [ptimer] that your configuration of TinKer did not include. Please reconfigure and rebuild TinKer.
#endif
#endif

#include <tk_itc.h>

enum PTIMER_ERROR_CODES {
	ERR_UNDEF_PTIMER = ERR_ITC_SENTINEL,
	ERR_TIME_NEG,
	ERR_TIMER_DELETED,
	ERR_NO_MORE_TIMERS,
	ERR_PTIMER_SENTINEL
};

#define ET_TIMEOUT 0
#define ET_TIMEOUT_P 1
#define ET_RELOAD_NEW 2
#define ET_CANCELLED 3

typedef enum {
	THWP_EVENT_ID,
	THWP_TIMER_ID,
	THWP_RTIME,
	THWP_LATCY,
	THWP_PROT_SZ
} e_timerHW_protIdx;

typedef struct ptimer_s {
	char name[4];
	unsigned int tid;
	struct ptimer_s *prev;
	unsigned int active;
	unsigned int numBlocked;
	time_t expTime;
	unsigned int neededCnts;
	unsigned int count;
	struct ptimer_s *next;
} ptimer_t;

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif
	unsigned long tk_ptime(void);
	unsigned long tk_ptime_destruct(void);
	unsigned long tk_ptimeevent_at(thid_t * tid, time_t * absTime);
	unsigned long tk_ptimeevent_in(thid_t * tid, time_t * relTime);
	unsigned long tk_ptimer_destroy(thid_t tid);
	unsigned long tk_ptimer_block(thid_t tid);
	unsigned long tk_ptimer_awake(thid_t tid);
	unsigned long tk_ptimer_sleep(thid_t tid, time_t * relTime);

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#endif
