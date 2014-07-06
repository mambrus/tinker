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

/*- include files **/
//#include <time.h>

/*- public definitions **/

/*!
A 32 bit value (at least) representing the time in HW ticks in the HW clock.

@note the lengt in physical time that each tick represents is different between
different targets depending on HW timers resolution and frequency.
*/
#define HWtick_t unsigned long

/*!
Stats (i.e. quality) of the Timer "HWclock_" that the HW layer of this
service is supporting/providing. Note that the upper part (i.e. this
file) is generic and is supposed to be transparent over many different
targets.
*/
typedef struct {
	unsigned long freq_hz;	/*!<  The frequency driving the HWclock
				   expressed in Hertz. I.e.
				   frequency <b>after</b> any pre-scaler
				   or chain of pre-scalers. This is
				   the actual <b>drift compensated</b>
				   frequency (not the theoretical
				   one found printed on the physical
				   oscillator circuit) */
	unsigned char res;	/*!<  Number of bits this HWClock_ handles
				 */
	HWtick_t perPebbles;	/*!<  Variable holds the value supposed to
				   reload the clock with in case
				   of periodical operations (i.e. CLK1
				   operation) */
	HWtick_t maxPebbles;	/*!<  Number of HW-ticks the HWclock_ can
				   handle without fraction. Any timeout
				   event requiring longer than
				   this time needs to have it's time
				   chopped up in multiples of this
				   amount of time. This value is
				   interesting for CLK2 operations
				   @note Make sure type holding this
				   value is at least 32 bits long. */
} HWclock_stats_t;

/*!
Get the quality of the HWclock. Will assume this never changes after system
startup. (lower level must assure this to be true).

HWclock_stats_t *HWclock_stats

@note HWclockNR must be either CLK1 or CLK2. CLK1 is the system timer HW and CLK2 is
used for high-res timeout events (ptime)

@note This function must be provided by lower level BSP
*/
#define tk_getHWclock_Quality(HWclockNR, HWclock_stats) tk_getHWclock_Quality_##HWclockNR(HWclock_stats)

/*!
Set new time in HWclock. Pending timer will be prevented from firing and current
time will be overwritten.

HWtick_t ticks - i.e. a non pointer variable

@note HWclockNR must be either CLK1 or CLK2. CLK1 is the system timer HW and CLK2 is
used for high-res timeout events (ptime)

@note This function must be provided by lower level BSP and you <b>must make sure this is ATOMIC</b>
*/
#define  tk_setHWclock(HWclockNR, ticks) tk_setHWclock_##HWclockNR(ticks)

/*!
Gets the time value in the HWclock while running (i.e. count down will not be
stopped). Pending timer will NOT be prevented from firing while read is done.
To make an atomic read, first use disarmHWclock.

HWtick_t *ticks - i.e. a <b>pointer return</b> variable

@note HWclockNR must be either CLK1 or CLK2. CLK1 is the system timer HW and CLK2 is
used for high-res timeout events (ptime)

@note This function must be provided by lower level BSP and you <b>must make sure this is ATOMIC</b>
*/
#define tk_getHWclock(HWclockNR, ticks) tk_getHWclock_##HWclockNR(ticks)

/*!
Arms the HW clock so that it can fire (i.e. enables generating an interrupt).

@note HWclockNR must be either CLK1 or CLK2. CLK1 is the system timer HW and CLK2 is
used for high-res timeout events (ptime)

@note This function must be provided by lower level BSP
*/
#define tk_armHWclock(HWclockNR) tk_armHWclock_##HWclockNR()

/*!
Disarms the HW clock so that it can NOT fire (i.e. disables generating an interrupt).

@note HWclockNR must be either CLK1 or CLK2. CLK1 is the system timer HW and CLK2 is
used for high-res timeout events (ptime)

@note This function must be provided by lower level BSP
*/
#define tk_disarmHWclock(HWclockNR) tk_disarmHWclock_##HWclockNR()

/*------------------------ obsolete follows? -----------------------------------*/

/*!
Arms the HWclock_ to trigger at a certain "time" from now. Time is expressed in
"ticks" and are normalized to have correspond to the actual time
required (this is handled in this component based on the HWclock_ quality provided).

@note This function must be provided by lower level BSP
*/
//extern void armHWclock           (HWtick_t ticks);

/*!
Dissarms any pending fireing of the HWclock_. Function will return if it was
succesfull or not. Unsucessfull will most likelly mean that the fireing
allready happened.

@note This function must be provided by lower level BSP
*/
//extern int disarmHWclock_       ();

/*- external data **/

/*- internal functions **/

/*- public data **/

/*- private data **/

/*- public functions **/

/*- private functions **/

/*- include lower layer **/

#if defined(_WIN32) &&  defined(_MSC_VER)
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

