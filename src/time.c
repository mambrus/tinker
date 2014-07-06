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
#include <time.h>
#include <errno.h>
#include <assert.h>

#if defined (__GNUC__)
#include <tinker/config.h>
#include <sys/times.h>
#include <sys/time.h>
#else

#include "tk_tick.h"
#include "tk_hwclock.h"
#include <tk_hwsys.h>
#define TICK_PER_CLK 1000ul

#include "tk_tick.h"
#if defined (HW_CLOCKED)
#define USE_HW_CLOCK
#endif

#define NO_SYSTIMER_WRAP_MONITOR

#endif
#ifndef HAVE_CLOCK
clock_t clock()
{
#if !defined (USE_HW_CLOCK)
	return (TICK_PER_CLK * sys_mickey);
#else
	signed int pebbles, p2;
	unsigned long uS;
	unsigned long TuS;
	HWclock_stats_t HWclock_stats;

	int zerocrossed = 0;

	pebbles = 0;
	tk_getHWclock_Quality(CLK1, &HWclock_stats);

	TK_CLI();
	tk_getHWclock(CLK1, &pebbles);
	uS = sys_mickey;
	TK_STI();

	uS *= 1000uL;

	if ((HWclock_stats.res > 16) && (HWclock_stats.freq_hz < 1000000)) {
		assert
		    ("tk: to high resolution HW clock. TinKer can't handle this ATM"
		     == NULL);
	}

	if (pebbles < 0) {
		pebbles *= -1;
		uS++;
		zerocrossed = 1;
	}
#ifndef NO_SYSTIMER_WRAP_MONITOR
	assert(HWclock_stats.perPebbles > (unsigned long)pebbles);
#endif

	TuS = HWclock_stats.maxPebbles - pebbles;
	TuS = (TuS * (1000000L / (HWclock_stats.freq_hz / 100))) / 100;

	return (uS + TuS);

#endif
}
#endif
#ifndef HAVE_TIME
time_t time(time_t * result)
{
	time_t tresult;
	tresult = (time_t) clock();

	if (result != NULL)
		*result = tresult;

	return tresult;
}
#endif
#ifndef HAVE_TIMES
clock_t times(struct tms * buffer)
{
	assert("Not implemented");

	return clock();
}
#endif
#ifndef HAVE_GETTIMEOFDAY
int gettimeofday(struct timeval *tp, struct timezone *tzp)
{
	assert("Not implemented");
	return 0;
}
#endif
#ifndef HAVE_SETTIMEOFDAY
int settimeofday(const struct timeval *tp, const struct timezone *tzp)
{
	assert("Not implemented");
	return 0;
}
#endif
#if !defined (__GNUC__)
int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
	switch (clock_id) {
	case CLOCK_REALTIME:
		assert("CLOCK_REALTIME is not implemented yet" == 0);

		break;

	case CLOCK_MONOTONIC:
		getnanouptime(tp);
		break;

	default:
		return EINVAL;
	}

	return ERR_OK;
}
#endif
void timespec2fmttime_np(struct fmttime *totime,
			 const struct timespec *fromtime)
{
	totime->days = fromtime->tv_sec / 86400;
	totime->hrs = (fromtime->tv_sec % 86400) / 3600;
	totime->mins = ((fromtime->tv_sec % 86400) % 3600) / 60;
	totime->secs = ((fromtime->tv_sec % 86400) % 3600) % 60;

	totime->nanos = fromtime->tv_nsec;
}

#if !defined (__GNUC__)
int timeval_subtract(result, x, y)
struct timeval *result, *x, *y;
{

	if (x->tv_usec < y->tv_usec) {
		int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		y->tv_usec -= 1000000 * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > 1000000) {
		int nsec = (x->tv_usec - y->tv_usec) / 1000000;
		y->tv_usec += 1000000 * nsec;
		y->tv_sec -= nsec;
	}

	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;

	return x->tv_sec < y->tv_sec;
}
#endif
