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

#ifndef PLPRCR_H
#define PLPRCR_H

//Stuff for the  PLL, Low-Power, and Reset Control Register (PLPRCR)

#include <sys/types.h>

/*
                          Table 14-10. PLPRCR Field Descriptions
 Bits  Name                                                Description
 0–11 MF    Multiplication factor. Determines the factor by which the OSCCLK input is multiplied to produce
            VCOOUT. This ﬁeld controls the value of the divider in the SPLL feedback loop. Programmable
            between 1and 4096, where 0x000 corresponds to 1 and 0xFFF corresponds to 4096.
            The MF ﬁeld can be read and written at any time but do not change MF value unless core is fully
            serialized ICTRL[ISCT_SER]=000. Changing the MF ﬁeld causes the SPLL to lose its lock. All clocks
            are disabled until the SPLL reaches lock condition.
12–15 —     Reserved, should be cleared.
  16  SPLSS System PLL lock status sticky. Cleared by power-on reset. Not affected by hard reset. An out-of-lock
            indication sets the SPLSS bit and it remains set until the software clears it. At power-on reset, the
            state of the SPLSS bit is zero. Write a 1 to clear this bit (writing a zero has no effect).
            SPLSS is affected only by an unintentional loss of lock due to a hardware-related issue. A
            software-initiated loss of lock, such as changing PLPRCR[MF] or entering deep-sleep or
            power-down mode, does not affect SPLSS.
            0 SPLL remains locked.
            1 SPLL has gone out of lock at least once since the bit was cleared.
  17  TEXPS Timer expired status. Internal status bit set when the periodic timer expires, the real-time clock alarm
            sets, the timebase clock alarm sets, the decrementer interrupt occurs, or the system resets.
            This bit is cleared by writing a 1; writing a zero has no effect.
            When in power-down mode (LPM=11), the TEXPS bit also controls the TEXP external signal as
            shown below. See Section 14.5.7.1, “Software Initiation of Power-Down Mode, with Automatic
              Wake-up.”
            0 TEXP is negated.
            1 TEXP is asserted.
  18  —     Reserved, should be cleared.

 19   TMIST Timers interrupt status. Cleared at reset. Set when a real-time clock, periodic interrupt timer,
            timebase, or decrementer interrupt occurs. This bit is cleared by writing a 1; writing a zero has no
            effect. Entry into low-power mode is disabled when TMIST is set.
            0 No timer interrupt was detected.
            1 A timer interrupt was detected.
 20   —     Reserved, should be cleared.
 21   CSRC  Clock source. Speciﬁes whether DFNH or DFNL generates the general system clock. Cleared by
            hard reset.
            0 The general system clock is generated by the DFNH ﬁeld.
            1 The general system clock is generated by the DFNL ﬁeld.
22–23 LPM   Low-power modes. This bit, in conjunction with TEXPS and CSRC, speciﬁes the operating mode of
            the core. There are seven possible modes. In the normal modes, the user can write a non-zero value
            to this ﬁeld. In the other modes, only a reset or asynchronous interrupt can clear this ﬁeld.
            00     Normal high/normal low mode.
            01     Doze high/doze low mode.
            10     Sleep mode.
            11     Deep-sleep/power-down mode.
 24   CSR   Checkstop reset enable. Enables an automatic reset when the processor enters checkstop mode. If
            the processor enters debug mode at reset, then reset is not generated automatically; refer to
            Table 14-11. See Section 44.5.2.2, “Debug Enable Register (DER).”
 25   —     Reserved, should be cleared.
 26   FIOPD Force I/O pull down. Indicates when the address and data external pins are driven by an internal
            pull-down device in sleep and deep-sleep mode.
            0 No pull-down on the address and data bus.
            1 Address and data bus is driven low in sleep and deep-sleep mode.
27–31 —     Reserved, should be cleared.

=============================================================================================================

 Table 14-11. PLPRCR[CSR] and DER[CHSTPE] Bit Combinations
PLPRCR[CSR]   DER[CHSTPE] Checkstop Mode        Result
     0             0            No                —
     0             0           Yes                —
     0             1            No                —
     0             1           Yes        Enter debug mode
     1             0            No                —
     1             0           Yes         Automatic reset
     1             1            No                —
     1             1           Yes        Enter debug mode

*/

typedef union {
	__uint32_t raw;
	struct {
		__uint32_t MF:12;
		__uint32_t PAD_0:4;
		__uint32_t SPLSS:1;
		__uint32_t TEXPS:1;
		__uint32_t PAD_1:1;
		__uint32_t TMIST:1;
		__uint32_t PAD_2:1;
		__uint32_t CSRC:1;
		__uint32_t LPM:2;
		__uint32_t CSR:1;
		__uint32_t PAD_3:1;
		__uint32_t FIOPD:1;
		__uint32_t PAD_4:5;
	} f;
} plprcr_t;

#endif				//PLPRCR_H
