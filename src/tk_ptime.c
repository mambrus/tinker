/***************************************************************************
 *   Copyright (C) 2005,2006 by Michael Ambrus                             *
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
#include <tk.h>
#include <tk_itc.h>
#include <tk_sysqueues.h>
#include <tk_ptime.h>
#include <kernel/src/tk_hwclock.h>
#include <tk_hwsys.h>
#include <string.h>
#include <errno.h>

#define TK_MAX_PTIMERS TK_MAX_THREADS

#define HWClkID CLK1
void *timerdeamon(void *inpar);
ptimer_t *pendingTimers;

HWclock_stats_t HWclock_stats;
unsigned long tk_ptime(void)
{
	int i;
	HWtick_t hw_ticks;

	pendingTimers = NULL;
	tk_getHWclock_Quality(CLK2, &HWclock_stats);
	tk_disarmHWclock(CLK2);
	tk_getHWclock(CLK2, &hw_ticks);
	hw_ticks = HWclock_stats.maxPebbles;
	tk_setHWclock(CLK2, hw_ticks);
	tk_armHWclock(CLK2);

	tk_create_thread("TIME", 0, timerdeamon, 1, 0x600);
	return ERR_OK;
}

unsigned long tk_ptime_destruct(void)
{
	int i;
	return ERR_OK;
}

unsigned long tk_ptimeevent_at(unsigned int *tid, time_t * absTime)
{
}

unsigned long tk_ptimeevent_in(unsigned int *tid, time_t * absTime)
{
}

unsigned long tk_ptimer_destroy(unsigned int tid)
{
}

ptimer_t *_tk_ptimer(unsigned int tid)
{

	return tid;
}

void *timerdeamon(void *inpar)
{
	unsigned long msg_buf[4];

	printk(("Timer deamon started. Preemtive hi-res timer events now possible\n"));
	while (1) {
		q_receive(tk_sys_queues[Q_HW_TIMER_EVENT], WAIT, 0, msg_buf);
		printk(("Timer deamon: %d\n", msg_buf[THWP_EVENT_ID]));
		switch (msg_buf[THWP_EVENT_ID]) {
		case ET_TIMEOUT:
			break;
		case ET_TIMEOUT_P:
			break;
		case ET_RELOAD_NEW:
			break;
		case ET_CANCELLED:
			break;
		default:
			printk(("tk_ptime: Error - we really need to polish the error handling...\n"));
			tk_exit(TC_UNKNOWN);
		};

	}

}
