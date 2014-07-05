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
				   frequency <b>after</b> any prescaler
				   or chain of prescalers. This is
				   the actual <b>drift compensated</b>
				   frequency (not the theoretical
				   one found printed on the physical
				   oscillator circuit) */
	unsigned char res;	/*!<  Number of bits this HWClock_ handles
				 */
	HWtick_t perPebbles;	/*!<  Variable holds the vaule supposed to
				   reload the clock with in case
				   ofperiodical operations (i.e. CLK1
				   operation) */
	HWtick_t maxPebbles;	/*!<  Number of HW-ticks the HWclock_ can
				   handle without fraction. Any timeout
				   event requireing longer than
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
Set new time in HWclock. Pending timer will be prevented from fireing and current
time will be overwritten.

HWtick_t ticks - i.e. a non pointer variable

@note HWclockNR must be either CLK1 or CLK2. CLK1 is the system timer HW and CLK2 is
used for high-res timeout events (ptime)

@note This function must be provided by lower level BSP and you <b>must make sure this is ATOMIC</b>
*/
#define  tk_setHWclock(HWclockNR, ticks) tk_setHWclock_##HWclockNR(ticks)

/*!
Gets the time value in the HWclock while running (i.e. count down will not be
stopped). Pending timer will NOT be prevented from fireing while read is done.
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

/*!
 * @defgroup CVSLOG_tk_hwclock_h tk_hwclock_h
 * @ingroup CVSLOG
 *
 *  $Log: tk_hwclock.h,v $
 *  Revision 1.10  2006-04-08 10:16:03  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.9.2.1  2006/04/03 20:07:29  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.9  2006/03/12 15:08:56  ambrmi09
 *  - Adjusted the source to accomodate the new file structure.
 *
 *  - All build environments uppdated and verified except BC5. For this one
 *  we stumbled across the header-file issue that I've been fearing. Seems
 *  we need to take care of that one after all.
 *
 *  @note The malloc bug still not solved.
 *
 *  Revision 1.8  2006/03/05 11:11:28  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.7  2006/02/28 13:18:20  ambrmi09
 *  Got MSVC target back-on track also.
 *
 *  Seems both MSVC and BCC have the same "error" regarding \ref CLK_TICK
 *  and/or \ref clock(). Could it be that it's TinKer's interpretation that
 *  is in fact wrong?
 *
 *  Revision 1.6  2006/02/28 11:50:08  ambrmi09
 *  - Trimmed the time constants (ruffly). 4sek per 14hrs drift
 *  - Revived the Borland C (BC5) target. Projectfile also added (BC5.ide)
 *  - Started experimenting with a indlude filename macro, that has the
 *    the potential of solving my ANSI header/function dilemma (\ref
 *    BUILDCHAIN )
 *  - Some "fishyness" about BC5 handling of time. Either \ref clock or
 *    \ref CLK_TCK doesn't follow standard (the latter I know for a fact,
 *    since it's 1e3 instead of 1e6 - but thats not all). \ref tk_msleep is
 *    adjusted to try to see the error.
 *
 *  Revision 1.5  2006/02/22 13:05:47  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.4  2006/02/19 12:44:33  ambrmi09
 *  - Documented ITC
 *  - Started to build up the structure for the \ref PTHREAD component
 *
 *  Revision 1.3  2006/02/08 18:39:49  ambrmi09
 *  Improved precision by providing the actual frequency instead of the
 *  theoretical in tk_getHWclock_Quality_CLK1 .
 *
 *  Also found and corrected a serious truncation error in getuptime (in
 *  calculating calculating TnS).
 *
 *  The second error might explain some of the time errors that could not be
 *  explained before (for \f$ T_i=6mS \f$ there was a 300nS unexplainable
 *  drift error for each update of tick).
 *
 *  Revision 1.2  2006/02/02 17:44:40  ambrmi09
 *  Workaround for Keil include header bug (again)
 *
 *  Revision 1.1  2006/02/02 15:40:26  ambrmi09
 *  HW clock definitions added
 *
 *
 *******************************************************************/
