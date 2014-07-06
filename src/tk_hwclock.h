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
#ifndef TK_HWCLOCK_H
#define TK_HWCLOCK_H
#define HWtick_t unsigned long

typedef struct {
	unsigned long freq_hz;

	unsigned char res;

	HWtick_t perPebbles;

	HWtick_t maxPebbles;
} HWclock_stats_t;

#define tk_getHWclock_Quality(HWclockNR,HWclock_stats) tk_getHWclock_Quality_ ##HWclockNR(HWclock_stats)

#define tk_setHWclock(HWclockNR,ticks) tk_setHWclock_ ##HWclockNR(ticks)

#define tk_getHWclock(HWclockNR,ticks) tk_getHWclock_ ##HWclockNR(ticks)

#define tk_armHWclock(HWclockNR) tk_armHWclock_ ##HWclockNR()

#define tk_disarmHWclock(HWclockNR) tk_disarmHWclock_ ##HWclockNR()

#if defined(_WIN32) && defined(_MSC_VER)
#error "HW timers not implemented for this target yet"
#elif defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
#error "HW timers not implemented for this target yet"
#elif defined( __C166__ )
#include <../bsp/XC167_Keil/dave/GPT1.H>
#include <../bsp/XC167_Keil/dave/GPT2.H>
#elif defined(__GNUC__)
#error "Implementation (only simulated \"scheduler in process\" possible) for a GNU system not done yet"
#else
#error "Can\'t determine the target for the TINKER kernel"
#endif

#endif
