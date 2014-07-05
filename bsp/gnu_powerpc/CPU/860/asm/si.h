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

#ifndef SI_H
#define SI_H

//Stuff for the "Serial Interface" and "Serial Communications Controllers"

#include <sys/types.h>

/*
20.2.4.2 SI Mode Register (SIMODE)
The SI mode register (SIMODE), shown in Figure 20-13., deﬁnes the SI operation modes
for the TDM channels and SMCs.

         0  1         3   4   5  6   7    8    9   10   11  12  13 14   15
 Field SMC2    SMC2CS     SDMb  RFSDb  DSCb  CRTb STZb CEb FEb GMb  TFSDb
Reset                                    0
 R/W                                    R/W
 Addr                                  0xAE0
        16  17        19 20  21 22  23   24   25   26   27  28  29 30   31
 Field SMC1    SMC1CS     SDMa  RFSDa  DSCa  CRTa STZa CEa FEa GMa  TFSDa
Reset                                    0
 R/W                                    R/W
 Addr                                  0xAE2

 Bits  Name                                              Description
 0, 16 SMCx SMCx connection
            0 NMSI mode. The clock source is determined by SMCxCS and the data comes from a dedicated
               pin (SMTXD1 and SMRXD1 for SMC1 or SMTXD2 and SMRXD2 for SMC2) in NMSI mode.
            1 SMCx is connected to the multiplexed SI (TDM channel).
 1–3, SMCxC SMCx clock source (NMSI mode). SMCx can take its Tx and Rx clocks from a baud rate generator
17–19    S  or one of four pins from the bank of clocks. However, Tx and Rx clocks must be common when
            connected to the NMSI.
            000 BRG1
            001 BRG2
            010 BRG3
            011 BRG4
            100 CLK1 for SMC1; CLK5 for SMC2
            101 CLK2 for SMC1; CLK6 for SMC2
            110 CLK3 for SMC1; CLK7 for SMC2
            111 CLK4 for SMC1; CLK8 for SMC2
 4–5,  SDMx SI diagnostic mode for TDMa/b. In modes 01,10, and 11, Rx and Tx clocks should be common.
20–21       00 Normal operation.
            01 Automatic echo. The TDM transmitter automatically resends its Rx data bit-by-bit. The Rx section
                operates normally, but the Tx section can only resend Rx data. L1GRx is ignored.
            10 Internal loopback. TDM transmitter output is connected internally to the TDM receiver
                input—L1TXDx is connected to L1RXDx. The receiver and transmitter operate normally, but data
                on L1RXDx is ignored. Data appears on L1TXDx, L1RQx is asserted normally, and L1GRx is
                ignored.
            11 Loopback control. TDM transmitter output is connected internally to the TDM receiver
                input—L1TXDx is connected to L1RXDx. Transmitter output L1TXDx and L1RQx are inactive.
                Provides loopback testing of the entire TDM without affecting the external serial lines.

 6–7, RFSDx Receive frame sync delay for TDMa/b. Indicates the delay between the Rx sync and the ﬁrst bit of
22–23       the Rx frame. Even if CRTx is set, RFSDx does not control the Tx frame delay.
            00 No bit delay. The ﬁrst bit of the frame is received on the same clock as sync. Use for GCI.
            01 1-bit delay. Use for IDL.
            10 2-bit delay.
            11 3-bit delay.
            See the examples in Figure 20-14 and Figure 20-15.
 8, 24 DSCx Double speed clock for TDMa/b—for TDM interfaces, such as GCI, that deﬁne the input clock to be
            twice as fast as the data rate.
            0 Channel clock (L1RCLKx and/or L1TCLKx) is equal to the data clock. Use for IDL and most other
               TDM formats.
            1 Channel clock rate is twice the data rate. Use for GCI.
 9, 25 CRTx Common receive and transmit pins for TDMa/b.
            0 Separate pins. The receive section of the TDM uses L1RCLKx and L1RSYNCx for framing; the
               transmit section uses L1TCLKx and L1TSYNCx for framing.
            1 Common pins. Both the transmit and receive section use L1RCLKx as the clock pin of the channel
               and L1RSYNCx as the sync pin. Use for IDL and GCI. Useful when the transmit and receive
               section of a given TDM share clock and sync signals. L1TCLKx and L1TSYNCx can be used for
               their alternate functions.
10, 26 STZx Set L1TXDx to zero for TDMa/b.
            0 Normal operation.
            1 L1TXDx is cleared until serial clocks are available60.”
11, 27 CEx  Clock edge for TDMa/b.
            When DSCx = 0:
            0 Data is sent on the rising clock edge and received on the falling edge (use for IDL).
            1 Data is sent on the falling edge of the clock and received on the rising edge.
            When DSCx = 1:
            0 Data is sent on the rising clock edge and received on the rising edge.
            1 Data is sent on the falling edge of the clock and received on the falling edge (use for GCI).
12, 28  FEx Frame sync edge for TDMa/b. Indicates when L1RSYNCx and L1TSYNCx pulses are sampled with
            the falling/rising edge of the channel clock.
            0 Falling edge. Use for IDL and GCI.
            1 Rising edge.

13, 29  GM   Grant mode for TDMa/b.
             0 GCI/SCIT mode. The GCI/SCIT D channel grant mechanism for transmission is supported
                internally. The grant is signalled by one bit of the Rx frame and is marked by setting SIRAM[CSEL]
                to 111 to assert an internal strobe. See Section 20.2.6.2.2, “SCIT Mode
             1 IDL mode. A grant mechanism is supported if the corresponding SICR[GRn] are set. The grant is
                a sample of the L1GR signal while L1TSYNCx is asserted. This grant mechanism implies the IDL
                access controls transmission on the D channel. See Section 20.2.5.2, “Programming the IDL
                Interface.”Note that for the MPC860 Rev. B and later, if GMa = 1, then the RTS4 signal on ports
                B and C functions as L1RQa. (The RTS4 function is still available on port D.)
             Note: For the MPC860 Rev. B and later, if GMb = 1, then the RTS3 signal on ports B and C functions
             as L1RQb. (The RTS3 function is still available on port D.)
14–15, TFSDx Transmit frame sync delay for TDMa/b. Determines the delay between the Tx sync and the ﬁrst bit of
30–31        the Tx frame. If CRTx is set, the Rx sync is used as the common sync, and the TFSDx bits refer to
             this common sync.
             00 No bit delay. The ﬁrst bit of the frame is sent on the same clock as the sync.
             01 1-bit delay.
             10 2-bit delay.
             11 3-bit delay.

*/

typedef union {
	__uint32_t raw;
	struct {
		__uint32_t SMC2:1;
		__uint32_t SMC2CS:3;
		__uint32_t SDMb:2;
		__uint32_t RFSDb:2;
		__uint32_t DSCb:1;
		__uint32_t CRTb:1;
		__uint32_t STZb:1;
		__uint32_t CEb:1;
		__uint32_t FEb:1;
		__uint32_t GMb:1;
		__uint32_t TFSDb:2;

		__uint32_t SMC1:1;
		__uint32_t SMC1CS:3;
		__uint32_t SDMa:2;
		__uint32_t RFSDa:2;
		__uint32_t DSCa:1;
		__uint32_t CRTa:1;
		__uint32_t STZa:1;
		__uint32_t CEa:1;
		__uint32_t FEa:1;
		__uint32_t GMa:1;
		__uint32_t TFSDa:2;
	} f;
} simode_t;
#endif				//SI_H
