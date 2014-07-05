/***************************************************************************
 *   Copyright (C) 2007 by Michael Ambrus                                  *
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

#ifndef SCCR_H
#define SCCR_H

//Stuff for the

#include <sys/types.h>


/*

                          Table 14-9. SCCR Field Descriptions
 Bits  Name                                             Description
  0   —     Reserved, should be cleared.
 1–2  COM   Clock output module. This ﬁeld controls the output buffer strength of the CLKOUT pin. When both
            bits are set, the CLKOUT pin is held in the high state. These bits can be dynamically changed
            without generating spikes on the CLKOUT pin. If the CLKOUT pin is not connected to external
            circuits, clock output should be disabled to minimize noise and power dissipation. The COM ﬁeld
            is cleared by hard reset.
            00 Clock output enabled full-strength buffer.
            01 Clock output enabled half-strength output buffer.
            10 Reserved.
            11 Clock output disabled.
 3–5  —     Reserved, should be cleared.
  6   TBS   Timebase source. Determines the clock source that drives the timebase and decrementer.
            0 Timebase frequency source is the OSCCLK divided by 4 or 16.
            1 Timebase frequency source is GCLK2 divided by 16.
  7   RTDIV Real-time clock divide. Determines if the clock, the crystal oscillator or main clock oscillator, to the
            real-time clock and periodic interrupt timer is divided by 4 or 512. At power-on reset this bit is
            cleared if the MODCK1 and MODCK2 signals are low.
            0 The clock is divided by 4.
            1 The clock is divided by 512.
  8   RTSEL Real-time clock select. Selects the crystal oscillator or main clock oscillator as the input source to
            PITRTCLK. At power-on reset, it reﬂects the value of MODCK1.
            0 OSCM (crystal oscillator) is selected.
            1 EXTCLK is selected.
  9   CRQEN CPM request enable. Cleared by power-on or hard reset. In low-power modes, speciﬁes if the
            general system clock returns to high frequency while the CP is active.
            0 The system remains in low frequency even if the communication processor module is active.
            1 The system switches to high frequency when the communication processor module is active.
  10  PRQEN Power management request enable. In low-power modes, speciﬁes whether the general system
            clock returns to a high frequency when a pending interrupt from the interrupt controller or
            MSR[POW] is clear (normal mode). Cleared by power-on or hard reset.
            0 The system remains in low frequency even if there is a pending interrupt from the interrupt
               controller or MSR[POW] = 0 (normal mode).
            1 The system switches to high frequency when there is a pending interrupt from the Interrupt
               controller or MSR[POW] = 0.
11–12 —     Reserved, should be cleared.
13–14 EBDF  External bus division factor. This ﬁeld deﬁnes the frequency division factor between GCLKx and
            GCLKx_50. CLKOUT is similar to GCLK2_50. The GCLKx_50 is used by the bus interface and
            memory controller to interface with an external system. This ﬁeld is initialized during hard reset
            using the hard reset conﬁguration word in Section 11.3.1.1, “Hard Reset Conﬁguration Word.”
            00 CLKOUT is GCLK2 divided by 1.
            01 CLKOUT is GCLK2 divided by 2.
            1x Reserved.
15–16 —     Reserved, should be cleared.

17–18 DFSYNC Division factor for the SYNCCLK. This ﬁeld sets the VCOOUT frequency division factor for the
             SYNCCLK signal. Changing the value of this ﬁeld does not result in a loss-of-lock condition. This
             ﬁeld is cleared by a power-on or hard reset.
             00 Divide by 1 (normal operation).
             01 Divide by 4.
             10 Divide by 16.
             11 Divide by 64.
19–20 DFBRG  Division factor of the BRGCLK. This ﬁeld sets the VCOOUT frequency division factor for the
             BRGCLK signal. Changing the value of this ﬁeld does not result in a loss-of-lock condition. This
             ﬁeld is cleared by a power-on or hard reset.
             00 Divide by 1 (normal operation).
             01 Divide by 4.
             10 Divide by 16.
             11 Divide by 64.
21–23 DFNL   Division factor low frequency. Sets the VCOOUT frequency division factor for general system
             clocks to be used in low-power mode. In low-power mode, the MPC860 automatically switches to
             the DFNL frequency. To select the DFNL frequency, load this ﬁeld with the divide value and set the
             CSRC bit. A loss-of-lock condition will not occur when changing the value of this ﬁeld. This ﬁeld is
             cleared by a power-on or hard reset.
             000 Divide by 2.
             001 Divide by 4.
             010 Divide by 8.
             011 Divide by 16.
             100 Divide by 32.
             101 Divide by 64.
             110 Reserved.
             111 Divide by 256.
24–26 DFNH   Division factor high frequency. Sets the VCOOUT frequency division factor for general system
             clocks to be used in normal mode. In normal mode, the MPC860 automatically switches to the
             DFNH frequency. To select the DFNH frequency, load this ﬁeld with the divide value and clear
             CSRC. A loss-of-lock condition does not occur when this ﬁeld is changed. This ﬁeld is cleared by
             a power-on or hard reset.
             000 Divide by 1.
             001 Divide by 2.
             010 Divide by 4.
             011 Divide by 8.
             100 Divide by 16.
             101 Divide by 32.
             110 Divide by 64.
             111 Reserved.
27–31 —      Reserved, should be cleared.

*/


typedef union{
	__uint32_t raw;
	struct {
		__uint32_t PADD_0		:1;
		__uint32_t COM			:2;
		__uint32_t PADD_1		:3;

		__uint32_t TBS			:1;
		__uint32_t RTDIV		:1;
		__uint32_t RTSEL		:1;
		__uint32_t CRQEN		:1;
		__uint32_t PRQEN		:1;

		__uint32_t PADD_3		:2;
		__uint32_t EBDF			:2;
		__uint32_t PADD_4		:2;
		__uint32_t DFSYNC		:2;
		__uint32_t DFBRG		:2;

		__uint32_t DFNL			:3;
		__uint32_t DFNH			:3;

		__uint32_t PADD_5		:5;
	}f;
}sccr_t;





#endif //CM_SCC_H

