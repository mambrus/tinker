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

#ifndef SJA1000_common_H
#define SJA1000_common_H

#include <sys/types.h>

/*!
@file

Registers common to all 3 modes (Reset, Basic, Pelican)

*/


//-----------------------------------------------------------------------------
/*!
Table 44 Bit interpretation of bus timing register 0 (BTR0); CAN address 6
      BIT 7           BIT 6         BIT 5            BIT 4          BIT 3           BIT 2          BIT 1           BIT 0
      SJW.1          SJW.0          BRP.5           BRP.4           BRP.3           BRP.2          BRP.1           BRP.0
6.5.1.1      Baud Rate Prescaler (BRP)
The period of the CAN system clock tscl is programmable and determines the individual bit timing. The CAN system clock
is calculated using the following equation:
tscl = 2 × tCLK × (32 × BRP.5 + 16 × BRP.4 + 8 × BRP.3 + 4 × BRP.2 + 2 × BRP.1 + BRP.0 + 1)
                                                         1
where tCLK = time period of the XTAL frequency = ------------
                                                            -
                                                      f XTAL
6.5.1.2      Synchronization Jump Width (SJW)
To compensate for phase shifts between clock oscillators of different bus controllers, any bus controller must
re-synchronize on any relevant signal edge of the current transmission. The synchronization jump width defines the
maximum number of clock cycles a bit period may be shortened or lengthened by one re-synchronization:
tSJW = tscl × (2 × SJW.1 + SJW.0 + 1)
*/
typedef struct{
	__uint8_t SJW:2; 	//!< Synchronization Jump Width (SJW)
	__uint8_t BRP:6; 	//!< Baud Rate Prescaler (BRP)
}sja1000_btr0_t;

/*!
6.5.2    BUS TIMING REGISTER 1 (BTR1)
The contents of bus timing register 1 defines the length of the bit period, the location of the sample point and the number
of samples to be taken at each sample point. This register can be accessed (read/write) if the reset mode is active.
In operating mode, this register is read only, if the PeliCAN mode is selected. In BasicCAN mode a ‘FFH’ is reflected.
Table 45 Bit interpretation of bus timing register 1 (BTR1); CAN address 7
     BIT 7          BIT 6            BIT 5           BIT 4          BIT 3           BIT 2          BIT 1           BIT 0
     SAM          TSEG2.2          TSEG2.1         TSEG2.0       TSEG1.3          TSEG1.2         TSEG1.1        TSEG1.0



 SAM                  1         triple; the bus is sampled three times; recommended for low/medium speed buses
                                (class A and B) where ﬁltering spikes on the bus line is beneﬁcial
                      0         single; the bus is sampled once; recommended for high speed buses (SAE class C)

6.5.2.2     Time Segment 1 (TSEG1) and Time Segment 2 (TSEG2)
TSEG1 and TSEG2 determine the number of clock cycles per bit period and the location of the sample point, where:
tSYNCSEG = 1 × tscl
tTSEG1 = tscl × (8 × TSEG1.3 + 4 × TSEG1.2 + 2 × TSEG1.1 + TSEG1.0 + 1)
tTSEG2 = tscl × (4 × TSEG2.2 + 2 × TSEG2.1 + TSEG2.0 + 1)

*/
typedef struct{
	__uint8_t SAM:1; 	//!< Sampling (SAM)
	__uint8_t TSEG2:3; 	//!< Time Segment 2 (TSEG2)
	__uint8_t TSEG1:4; 	//!< Time Segment 1 (TSEG1)
}sja1000_btr1_t;


/*!
6.5.3    OUTPUT CONTROL REGISTER (OCR)                         This register may be accessed (read/write) if the reset
                                                               mode is active. In operating mode, this register is read
The output control register allows the set-up of different
                                                               only, if the PeliCAN mode is selected. In BasicCAN mode
output driver configurations under software control.
                                                               a ‘FFH’ is reflected.
Table 46 Bit interpretation of the output control register (OCR); CAN address 8
     BIT 7          BIT 6           BIT 5           BIT 4         BIT 3           BIT 2         BIT 1           BIT 0
    OCTP1          OCTN1          OCPOL1           OCTP0         OCTN0           OCPOL0      OCMODE1        OCMODE0

Table 47 Interpretation of OCMODE bits
  OCMODE1        OCMODE0                               DESCRIPTION
       0               0      bi-phase output mode
       0               1      test output mode; note 1
       1               0      normal output mode
       1               1      clock output mode


*/
typedef struct{
	__uint8_t OCTP1:1; 	//!<
	__uint8_t OCTN1:1; 	//!<
	__uint8_t OCPOL1:1; 	//!<
	__uint8_t OCTP0:1; 	//!<
	__uint8_t OCTN0:1; 	//!<
	__uint8_t OCPOL0:1; 	//!<
	__uint8_t OCMODE:2; 	//!<
}sja1000_ocr_t;

/*!
6.5.4    CLOCK DIVIDER REGISTER (CDR)
The clock divider register controls the CLKOUT frequency
for the microcontroller and allows to deactivate the
CLKOUT pin. Additionally a dedicated receive interrupt
pulse on TX1, a receive comparator bypass and the
selection between BasicCAN mode and PeliCAN mode is
made here. The default state of the register after hardware
reset is divide-by-12 for Motorola mode (00000101) and
divide-by-2 for Intel mode (00000000).
On software reset (reset request/reset mode) this register
is not influenced.
The reserved bit (CDR.4) will always reflect a logic 0.
The application software should always write a logic 0 to
this bit in order to be compatible with future features, which
may be 1-active using this bit.

Table 49 Bit interpretation of the clock divider register (CDR); CAN address 31
    BIT 7           BIT 6           BIT 5           BIT 4         BIT 3          BIT 2  BIT 1 BIT 0
                                                    (0)(1)
  CAN mode          CBP           RXINTEN                        clock off        CD.2  CD.1  CD.0
Note
1. This bit cannot be written. During read-out of this register always a zero is given.



6.5.4.1      CD.2 to CD.0
The bits CD.2 to CD.0 are accessible without restrictions in reset mode as well as in operating mode. These bits are used
to define the frequency at the external CLKOUT pin. For an overview of selectable frequencies see Table 50.
Table 50 CLKOUT frequency selection; note 1
       CD.2               CD.1             CD.0                                CLKOUT FREQUENCY
         0                  0                0           f osc
                                                         --------
                                                            2
         0                  0                1           f osc
                                                         --------
                                                            4
         0                  1                0           f osc
                                                         --------
                                                            6
         0                  1                1           f osc
                                                         --------
                                                            8
         1                  0                0           f osc
                                                         --------
                                                          10
         1                  0                1           f osc
                                                         --------
                                                          12
         1                  1                0           f osc
                                                         --------
                                                          14
         1                  1                1          fosc
Note
1. fosc is the frequency of the external oscillator (XTAL).
6.5.4.2      Clock off                                            6.5.4.4     CBP
Setting this bit allows the external CLKOUT pin of the            Setting of CDR.6 allows to bypass the CAN input
SJA1000 to be disabled. A write access is possible only in        comparator and is only possible in reset mode. This is
reset mode. If this bit is set, CLKOUT is LOW during sleep        useful in the event that the SJA1000 is connected to an
mode, otherwise it is HIGH.                                       external transceiver circuit. The internal delay of the
                                                                  SJA1000 is reduced, which will result in a longer maximum
6.5.4.3      RXINTEN                                              possible bus length. If CBP is set, only RX0 is active. The
                                                                  unused RX1 input should be connected to a defined level
This bit allows the TX1 output to be used as a dedicated
                                                                  (e.g. VSS).
receive interrupt output. When a received message has
passed the acceptance filter successfully, a receive
                                                                  6.5.4.5     CAN mode
interrupt pulse with the length of one bit time is always
output at the TX1 pin (during the last bit of end of frame).      CDR.7 defines the CAN mode. If CDR.7 is at logic 0 the
The transmit output stage should operate in normal output         CAN controller operates in BasicCAN mode. If set to
mode. The polarity and output drive are programmable via          logic 1 the CAN controller operates in PeliCAN mode.
the output control register (see also Section 6.5.3). A write     Write access is only possible in reset mode.
access is only possible in reset mode.



*/
typedef struct{
	__uint8_t PELICAN:1; 	//!< CAN mode (1 is peican, 0 is basic)
	__uint8_t CBP:1; 	//!< Bypass the CAN input comparator
	__uint8_t RXINTEN:1; 	//!< TX1 output used as a dedicated receive interrupt output.
	__uint8_t zero:1; 	//!< Padding. Always read as zero
	__uint8_t CLK_OFF:1; 	//!< External CLKOUT pin of the SJA1000 is disabled.
	__uint8_t CD:3; 	//!< CLKOUT frequency
}sja1000_cdr_t;


#endif //SJA1000_common_H

