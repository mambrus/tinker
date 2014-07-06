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
#define TICK_OWNER
#include "tk_tick.h"
#include <tk.h>
#include <stdio.h>
#include <time.h>
#include <tk_hwsys.h>

#if defined(HW_CLOCKED)
#include "tk_hwclock.h"
#endif
#define FACTOR 0x418937
#define FFRACT 0x128
void getnanouptime(struct timespec *tp)
{
	unsigned long MSmS;
	unsigned long LSS, LSmS;
	unsigned long MSfrac;
	unsigned long MSrest;
	unsigned long pebbles;
	unsigned long nS;
	unsigned long TnS;

#if defined(HW_CLOCKED)
	HWclock_stats_t HWclock_stats;
#endif
	pebbles = 0;

#if defined(HW_CLOCKED)
	tk_getHWclock_Quality(CLK1, &HWclock_stats);
	tk_disarmHWclock(CLK1);
	tk_getHWclock(CLK1, &pebbles);
#endif

	MSmS = sys_mackey;
	LSmS = sys_mickey;

#if defined(HW_CLOCKED)
	tk_armHWclock(CLK1);

	if ((HWclock_stats.res > 16) && (HWclock_stats.freq_hz < 1000000)) {
		printk(("tk: to high resolution HW clock. TinKer can't handle this ATM \n"));
		tk_exit(TC_ERR_HW);
	}
#endif

	TnS = HWclock_stats.maxPebbles - pebbles;
	TnS = (TnS * (1000000000L / (HWclock_stats.freq_hz / 100))) / 100;

	MSfrac = (MSmS % 1000L) * FACTOR + ((MSmS % 1000L) * FFRACT) / 1000L;
	MSrest = ((MSmS % 1000L) * FFRACT) % 1000L;
	LSS = (LSmS / 1000L) + MSfrac;

	nS = ((LSmS % 1000L) + MSrest) * 1000000L;

	nS = nS + TnS;
	LSS = LSS + nS / 1000000000L;
	nS = nS % 1000000000L;
	tp->tv_sec = LSS;
	tp->tv_nsec = nS;

}
