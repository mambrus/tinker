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

#ifndef TK_TICK_H
#define TK_TICK_H

#include <time.h>

#if defined (__GNUC__)
#include <sys/time.h>
#endif

/*!
One mickey is \b always 1 uS worth no matter actual resolution which gives a
frequency in ticks per second of 1000000 (i.e. the same as  CLOCKS_PER_SEC
on true POSIX targets).

*/
#define MICKEYS_PER_SEC 1000000uL

#if defined(__C166__)
#define MEMTYPE idata
#else
#define MEMTYPE
#endif

/*! If HWclock operations are supported or not. */
#define HW_CLOCKED

/*!
Use SPEEDUP larger than 1 to make kernel advance faster.

@note <b>This is used for debugging kernel internals only</b>
*/
#define SPEEDUP 1ul
//#define SPEEDUP 1000ul

#if (SPEEDUP > 1)
#define ADV( PERT ) (   \
      ( PERT * SPEEDUP )   \
   )
#elif (SPEEDUP == 1)
#define ADV( PERT ) (   \
      ( PERT )             \
   )
#else
#error Faulty SPEEDUP factor (use this for debugging dispatcher only)
#endif

/* 18 014 398 509 481 983 nS*/
/* 18 014 398 S*/
/* 208,5 days*/

//! This is the internal data of the timer system. Observe that accessing this
//  variable is not thread/interrupt safe. This needs to be revised.
//
//  @note Also very important - always include the memory type "language
//  extension" when declaring external variables like this.

#if defined(TICK_OWNER)
unsigned long MEMTYPE sys_mackey = 0;	//!< Overflow counter for sys_mickey
unsigned long MEMTYPE sys_mickey = 0;	//!< A timer tick. Zapp this: "Trimmed to be 1/10 of a mS"
#else
extern unsigned long MEMTYPE sys_mickey;
extern unsigned long MEMTYPE sys_mackey;
#endif

/*!
@brief Advance system tick.

Call this from your timer ISR.

Usable for any interrupt frequency in multiples of 1mS.

<b>Notice that operations are done on a running timer</b>

This implies some precautions since the "tick" are evaluated
while operated on:

- These operations must not be used anywhere but in the tick ISR
- They must complete before the next tick (don't choose too high frequencies)
- Other ISR must not keep running for longer than one "tick" time
- In preemptive configuration, the priority of the tick ISR must be higher than
  any of the events (for the same reason as mentioned in point above). This is
  because if that event ISR preempts this operation, it will actually dispatch
  another thread to run, and the tick ISR will not be able to finish until that
  thread goes off run-able state. Threads preempting ISRs are usually not a
  problem you follow the design rules (mentioned else where), but for the tick
  ISR it is.

@note
- Be cartful to match the advance value with the true interrupt frequency
- Use as slow frequencies as possible (large pebble values) to minimize drift
- But not to large. Don't exceed the maximum value the HW can candle
- <b>IMPORTANT:</b> Make sure the period time ( 1/frequency) is in even multiples
  of 1 mS.

*/

#define _tk_tick_advance_mS( advance )       \
   if (sys_mickey+advance < sys_mickey){     \
      sys_mickey+=ADV(advance);              \
      sys_mackey++;                          \
   }else{                                    \
      sys_mickey+=ADV(advance);              \
   }

void getnanouptime(struct timespec *tp);

#endif

