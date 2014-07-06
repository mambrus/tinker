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

    High resolution clock function and internal
    system clock storage owner.
  */

#define TICK_OWNER
#include "tk_tick.h"
#include <tk.h>
#include <stdio.h>
#include <time.h>
#include <tk_hwsys.h>

#if defined(HW_CLOCKED)
#include "tk_hwclock.h"
#endif

/*
We need a constant that is 0x100000000 / 1000 (note the different bases).
Not even preprocessor can calculate this on many compilers so we did that
"off compile-time"
This equals to the real value 4294967,296. Defines below are it's integer
and frac part
*/

#define FACTOR    0x418937
#define FFRACT    0x128

/*!
@brief returns time since startup of the target expressed in seconds and
fractions in nS. Precision limited only by HW (if supported).

Similar implementation is found in BSD:
http://nixdoc.net/man-pages/FreeBSD/man9/getnanouptime.9.html

Internal running time is in mS and is kept in two 32-bit variables. The
LS of these (called "tick") contains number of uS since startup, and the
MS contains the number of times the LS has wrapped around.

Together they form a 64 bit variable keeping the number of mS since
startup. This means the combined value will keep accurate time for
584.942.417,4 years (!). It's relative accuracy though, absolute accuracy
would have had to take into account drift, but it's good enough for timeouts
and all sorts of time calculations between two events in time for a very
long run-time.

What about precision then? Well only because the LS value contains time
in mS, it doesn't mean that the precision is in mS! On some systems the
value is updated each 10mS, or 100mS (or much worse - on a PC's this is
done 18.2 times/S = 54 mS. I.e. about 50 times less "accurate").

However, the time resulting from this function can be much higher even
than 1uS  (if HW supports it) by reading the remaining time in HW
timer register until next update of "ticks". In practice, the precision
is determined by the HW and is of date of this writing in practice 100
or 200 nS.

The time is presented in the ANSI struct timespec to better fit the
pthread component. This is however an expression in (signed)
32-bit value seconds and (signed) 32-bit valued nS fractions. This
boils down to mean that the precision and resolution is the same as the
internal time representation, but the maximum time is about 10 million times
shorter (i.e.  \f$ \frac{2^32}{2} S = 68.1 \f$ years) - which should still be good in
most cases though.

@note Be careful when you calculate differences between values of struct
timespec. It's easy to limit the maximum time in between to, both 1/2 and
1/4 of it's maximum of 68 years.

@note It is said that <em>"A beloved child has many names"</em>. We call
the internal timer value a "tick". Other names that occurs in "kernel
lingo" are "mickeys", "mackeys", "jiffies" and "pebbles". They all mean
more or less the same thing: time since a certain time (often startup),
fractioned in some sort of unit (often interrupt period time). In
TinKer we use most of these words, but with the following beaning:<p>
- tick    - is the internal time. This is represented in mS

- mickey  - mickeys are the LS part of a tick (32 bit value)

- mackey  - mackeys are the MS part of a tick (32 bit value)

- pebble  - is the register value in the HW clock. One pebble's
            meaning in actual time depends on the frequency driving
            the HWclock.

@note The way TnS is calculated is very critical. We either loose
resolution (keeping denominator high) or add error (denominator
is pre-devided, but truncation occurs). We might need to consider
inventing 64-bit operations.

@todo see the second note in getuptime
*/

void getnanouptime(struct timespec *tp	//!< The returned uptime in nS precision
    )
{
	unsigned long /*MSS, */ MSmS;	// 32-bit <b>H</b>igh part of seconds, milliseconds
	unsigned long LSS, LSmS;	// 32-bit <b>L</b>ow part of seconds, milliseconds
	unsigned long MSfrac;	// whole seconds left from MSmS part
	unsigned long MSrest;	// The rest after converting to seconds (to propagate down to nS)
	unsigned long pebbles;	// Remaining value in HWclock register
	unsigned long nS;	// Time passed since last update of tick expressed in nS. Lets hope we don't need a 64 bit value for this.
	unsigned long TnS;	// Temp of the above

#if defined(HW_CLOCKED)
	HWclock_stats_t HWclock_stats;
#endif

// First part is to gather the bits and pieces holding the time

// Read HW clock if supported follows:
// Note that the following operation will not cause drift. Nether HWclock
// is prevented from counting nor is systimer prevented from counting ticks,
// only is systimer possibly delayed a little in the unlikely event that this
// read comes very close to a timeout - that might cause a small drift depending on the HW configuration
// (automatic reload or reload by ISR). For now we accept this small drift.

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

// Now we should have all we need, and we can start converting.

	// Calculate the number of nS since last update of tick
	TnS = HWclock_stats.maxPebbles - pebbles;
	TnS = (TnS * (1000000000L / (HWclock_stats.freq_hz / 100))) / 100;

	//convert tick to <i>struct timespec</i>
	MSfrac = (MSmS % 1000L) * FACTOR + ((MSmS % 1000L) * FFRACT) / 1000L;
	MSrest = ((MSmS % 1000L) * FFRACT) % 1000L;
	LSS = (LSmS / 1000L) + MSfrac;

	//MSS = MSmS / 1000L;  //<-not used

	nS = ((LSmS % 1000L) + MSrest) * 1000000L;
	//            ^^^                              ^^^
	//    fraction of millisec from LS       fraction of millisec from MS        in nanosecs

	//Add the fraction originally expressed in pebbles and compensate seconds if needed
	nS = nS + TnS;
	LSS = LSS + nS / 1000000000L;	// All the "rests" added together might result in more than one second.
	nS = nS % 1000000000L;	// take away the amount that got into seconds

	//MSS is not compensated - bug will not be seen unless running system for 60 years, and only if nS
	//frac causes overflow that ripples though both nS and S. Compensating for this will cause
	//extra code to run on every invocation and that will most certainly never do anything (waste
	//of time). Besides MSS is never used outside this function (double waste). This note kept for
	//future reference.

//Finally we should have the information requested. Copy to caller
	tp->tv_sec = LSS;
	tp->tv_nsec = nS;

}
