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

#if defined (__GNUC__)
#include <tinker/config.h>
#include <sys/times.h>
#endif

#if !defined(__C166__)
#include <tk_ansi_dirwrap.h>
#include BUILDCHAIN_INCLUDE(time.h)
#endif

#ifndef TIME_H_TK
#define TIME_H_TK

#define clock tk_clock
#define time tk_time

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif
	struct fmttime {
		long int days;

		int hrs;
		int mins;
		int secs;
		long int nanos;
	};

#define sleep(t) ( tk_msleep( t * 1000 ) )

#define usleep(t) ( tk_msleep( (unsigned long)t / 1000ul ) )

#if defined (__GNUC__) && defined (TK_BOARD)

#if defined(CLOCKS_PER_SEC)
#undef CLOCKS_PER_SEC
#endif
#if defined (CLK_TCK)
#undef CLK_TCK
#endif

#define CLOCKS_PER_SEC 1000000L
#define CLK_TCK CLOCKS_PER_SEC
#endif

#if !defined (__GNUC__)

#ifndef CLOCKS_PER_SEC

#define CLOCKS_PER_SEC 1000000L
#endif

#ifndef CLK_TCK

#define CLK_TCK CLOCKS_PER_SEC
#endif

#ifndef clock_t
#define clock_t unsigned long
#endif

#ifndef time_t
#define time_t long
#endif
	struct timeval {
		long int tv_sec;

		long int tv_usec;

	};
	struct timespec {
		long int tv_sec;

		long int tv_nsec;

	};

	int nanosleep(const struct timespec *requested_time,
		      struct timespec *remaining);
	int gettimeofday(struct timeval *tp, struct timezone *tzp);
	int settimeofday(const struct timeval *tp, const struct timezone *tzp);
	time_t time(time_t * result);
#ifndef difftime
#define difftime(t1,t0) \
      (t1 - t0)
#endif

	clock_t clock();
	typedef enum {
		CLOCK_REALTIME = 0,
		CLOCK_MONOTONIC = 1
	} clockid_t;

	int nanosleep(const struct timespec *rqtp, struct timespec *rmtp);
	int clock_getres(clockid_t clock_id, struct timespec *res);
	int clock_gettime(clockid_t clock_id, struct timespec *tp);
	int clock_settime(clockid_t clock_id, const struct timespec *tp);

#endif

#if !defined(__C166__)
#undef clock
#undef time
#endif

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#endif
