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

#ifndef SJA1000_RESET_H
#define SJA1000_RESET_H
#include <sys/types.h>
#include "sja1000_common.h"

/*!
@file

Table 11 Reset mode conﬁguration; notes 1 and 2
                                                                                         VALUE
                                                                                            SETTING MOD.0
   REGISTER          BIT           SYMBOL                 NAME                 RESET BY      BY SOFTWARE
                                                                              HARDWARE         OR DUE TO
                                                                                                 BUS-OFF
                               −
 Mode           MOD.7 to 5                      reserved                   0 (reserved)     0 (reserved)
                MOD.4          SM               Sleep Mode                 0 (wake-up)      0 (wake-up)
                MOD.3          AFM              Acceptance Filter Mode     0 (dual)         X
                MOD.2          STM              Self Test Mode             0 (normal)       X
                MOD.1          LOM              Listen Only Mode           0 (normal)       X
                MOD.0          RM               Reset Mode                 1 (present)      1 (present)
                               −
 Command        CMR.7 to 5                      reserved                   0 (reserved)     0 (reserved)
                CMR.4          SRR              Self Reception Request     0 (absent)       0 (absent)
                CMR.3          CDO              Clear Data Overrun         0 (no action)    0 (no action)
                CMR.2          RRB              Release Receive Buffer     0 (no action)    0 (no action)
                CMR.1          AT               Abort Transmission         0 (absent)       0 (absent)
                CMR.0          TR               Transmission Request       0 (absent)       0 (absent)
 Status         SR.7           BS               Bus Status                 0 (bus-on)       X
                SR.6           ES               Error Status               0 (ok)           X
                SR.5           TS               Transmit Status            1 (wait idle)    1 (wait idle)
                SR.4           RS               Receive Status             1 (wait idle)    1 (wait idle)
                SR.3           TCS              Transmission Complete      1 (complete)     X
                                                Status
                SR.2           TBS              Transmit Buffer Status     1 (released)     1 (released)
                SR.1           DOS              Data Overrun Status        0 (absent)       0 (absent)
                SR.0           RBS              Receive Buffer Status      0 (empty)        0 (empty)
 Interrupt      IR.7           BEI              Bus Error Interrupt        0 (reset)        0 (reset)
                IR.6           ALI              Arbitration Lost Interrupt 0 (reset)        0 (reset)
                IR.5           EPI              Error Passive Interrupt    0 (reset)        0 (reset)
                IR.4           WUI              Wake-Up Interrupt          0 (reset)        0 (reset)
                IR.3           DOI              Data Overrun Interrupt     0 (reset)        0 (reset)
                IR.2           EI               Error Warning Interrupt    0 (reset)        X; note 3
                IR.1           TI               Transmit Interrupt         0 (reset)        0 (reset)
                IR.0           RI               Receive Interrupt          0 (reset)        0 (reset)

Interrupt    IER.7  BEIE    Bus Error Interrupt        X X
enable                      Enable
             IER.6  ALIE    Arbitration Lost Interrupt X X
                            Enable
             IER.5  EPIE    Error Passive Interrupt    X X
                            Enable
             IER.4  WUIE    Wake-Up Interrupt          X X
                            Enable
             IER.3  DOIE    Data Overrun Interrupt     X X
                            Enable
             IER.2  EIE     Error Warning Interrupt    X X
                            Enable
             IER.1  TIE     Transmit Interrupt         X X
                            Enable
             IER.0  RIE     Receive Interrupt Enable X   X
Bus timing 0 BTR0.7 SJW.1   Synchronization Jump       X X
                            Width 1
             BTR0.6 SJW.0   Synchronization Jump       X X
                            Width 0
             BTR0.5 BRP.5   Baud Rate Prescaler 5      X X
             BTR0.4 BRP.4   Baud Rate Prescaler 4      X X
             BTR0.3 BRP.3   Baud Rate Prescaler 3      X X
             BTR0.2 BRP.2   Baud Rate Prescaler 2      X X
             BTR0.1 BRP.1   Baud Rate Prescaler 1      X X
             BTR0.0 BRP.0   Baud Rate Prescaler 0      X X
Bus timing 1 BTR1.7 SAM     Sampling                   X X
             BTR1.6 TSEG2.2 Time Segment 2.2           X X
             BTR1.5 TSEG2.1 Time Segment 2.1           X X
             BTR1.4 TSEG2.0 Time Segment 2.0           X X
             BTR1.3 TSEG1.3 Time Segment 1.3           X X
             BTR1.2 TSEG1.2 Time Segment 1.2           X X
             BTR1.1 TSEG1.1 Time Segment 1.1           X X
             BTR1.0 TSEG1.0 Time Segment 1.0           X X

Output control   OCR.7 OCTP1        Output Control            X               X
                                    Transistor P1
                 OCR.6 OCTN1        Output Control            X               X
                                    Transistor N1
                 OCR.5 OCPOL1       Output Control Polarity 1 X               X
                 OCR.4 OCTP0        Output Control            X               X
                                    Transistor P0
                 OCR.3 OCTN0        Output Control            X               X
                                    Transistor N0
                 OCR.2 OCPOL0       Output Control Polarity 0 X               X
                 OCR.1 OCMODE1      Output Control Mode 1     X               X
                 OCR.0 OCMODE0      Output Control Mode 0     X               X
                 −
Arbitration lost       ALC          Arbitration Lost Capture  0               X
capture
                 −
Error code             ECC          Error Code Capture        0               X
capture
                 −
Error warning          EWLR         Error Warning Limit       96              X
limit                               Register
                 −     RXERR        Receive Error Counter     0 (reset)       X; note 4
RX error
counter
                 −     TXERR        Transmit Error Counter    0 (reset)       X; note 4
TX error
counter
                 −
TX buffer              TXB          Transmit Buffer           X               X
                 −
RX buffer              RXB          Receive Buffer            X; note 5       X; note 5
                 −
ACR 0 to 3             ACR0 to ACR3 Acceptance Code           X               X
                                    Registers
                 −
AMR 0 to 3             AMR0 to AMR3 Acceptance Mask           X               X
                                    Registers
                 −
RX message             RMC          RX Message Counter        0               0
counter
                 −
RX buffer start        RBSA         RX Buffer Start Address   00000000        X
address
                 −
Clock divider          CDR          Clock Divider Register    00000000 Intel; X
                                                              00000101
                                                              Motorola
*/


/*!
Table 12 Bit interpretation of the mode register (MOD); CAN address ‘0’
   BIT       SYMBOL                 NAME               VALUE                           FUNCTION
            −             −                              −
 MOD.7                                                            reserved
            −             −                              −
 MOD.6                                                            reserved
            −             −                              −        reserved
 MOD.5
 MOD.4      SM            Sleep Mode; note 1             1        sleep; the CAN controller enters sleep mode if no
                                                                  CAN interrupt is pending and if there is no bus
                                                                  activity
                                                         0        wake-up; the CAN controller wakes up if sleeping
 MOD.3      AFM           Acceptance Filter Mode;        1        single; the single acceptance ﬁlter option is
                          note 2                                  enabled (one ﬁlter with the length of 32 bit is
                                                                  active)
                                                         0        dual; the dual acceptance ﬁlter option is enabled
                                                                  (two ﬁlters, each with the length of 16 bit are
                                                                  active)
 MOD.2      STM           Self Test Mode; note 2         1        self test; in this mode a full node test is possible
                                                                  without any other active node on the bus using the
                                                                  self reception request command; the
                                                                  CAN controller will perform a successful
                                                                  transmission, even if there is no acknowledge
                                                                  received
                                                         0        normal; an acknowledge is required for successful
                                                                  transmission
*/


typedef struct{ 
	__uint8_t padd:3; 	//!< Padding
	__uint8_t SM:1; 	//!< Sleep mode
	__uint8_t AFM:1; 	//!< Acceptance Filter Mode
	__uint8_t STM:1; 	//!< Self Test Mode
	__uint8_t LOM:1; 	//!< Listen Only Mode
	__uint8_t RM:1; 	//!< Reset Mode
}sja1000_pelican_mod_t;


/*!
6.4.4     COMMAND REGISTER (CMR)
A command bit initiates an action within the transfer layer of the CAN controller. This register is write only, all bits will
return a logic 0 when being read. Between two commands at least one internal clock cycle is needed in order to proceed.
The internal clock is half of the external oscillator frequency.
Table 13 Bit interpretation of the command register (CMR); CAN address 1
    BIT       SYMBOL                NAME                   VALUE                          FUNCTION
              −                                               −      −
 CMR.7                     reserved
              −                                               −      −
                           reserved
 CMR.6
              −                                               −      −
 CMR.5                     reserved
 CMR.4        SRR          Self Reception Request;            1      present; a message shall be transmitted and
                           notes 1 and 2                             received simultaneously
                                                                     − (absent)
                                                              0
 CMR.3        CDO          Clear Data Overrun;                1      clear; the data overrun status bit is cleared
                           note 3                                    − (no action)
                                                              0
 CMR.2        RRB          Release Receive Buffer;            1      released; the receive buffer, representing the
                           note 4                                    message memory space in the RXFIFO is
                                                                     released
                                                                     − (no action)
                                                              0
 CMR.1        AT           Abort Transmission;                1      present; if not already in progress, a pending
                           notes 5 and 2                             transmission request is cancelled
                                                                     − (absent)
                                                              0
 CMR.0        TR           Transmission Request;              1      present; a message shall be transmitted
                           notes 6 and 2                             − (absent)
                                                              0

*/
typedef struct{ 
	__uint8_t padd:3; 	//!< Padding
	__uint8_t SRR:1; 	//!< Self Reception Request
	__uint8_t CD0:1; 	//!< Clear Data Overrun
	__uint8_t RRB:1; 	//!< Release Receive Buffer
	__uint8_t AT:1; 	//!< Abort Transmission
	__uint8_t TR:1; 	//!< Transmission Request
}sja1000_pelican_cmr_t;

/*!
         STATUS REGISTER (SR)
6.4.5
The content of the status register reflects the status of the CAN controller. The status register appears to the CPU as a
read only memory.
Table 14 Bit interpretation of the status register (SR); CAN address 2
    BIT      SYMBOL                NAME                    VALUE                           FUNCTION
 SR.7       BS            Bus Status; note 1                  1       bus-off; the CAN controller is not involved in bus
                                                                      activities
                                                              0       bus-on; the CAN controller is involved in bus
                                                                      activities
 SR.6       ES            Error Status; note 2                1       error; at least one of the error counters has
                                                                      reached or exceeded the CPU warning limit
                                                                      deﬁned by the Error Warning Limit Register
                                                                      (EWLR)
                                                              0       ok; both error counters are below the warning limit
 SR.5       TS            Transmit Status; note 3             1       transmit; the CAN controller is transmitting a
                                                                      message
                                                              0       idle
 SR.4       RS            Receive Status; note 3              1       receive; the CAN controller is receiving a
                                                                      message
                                                              0       idle
 SR.3       TCS           Transmission Complete               1       complete; last requested transmission has been
                          Status; note 4                              successfully completed
                                                              0       incomplete; previously requested transmission is
                                                                      not yet completed
 SR.2       TBS           Transmit Buffer Status;             1       released; the CPU may write a message into the
                          note 5                                      transmit buffer
                                                              0       locked; the CPU cannot access the transmit
                                                                      buffer; a message is either waiting for
                                                                      transmission or is in the process of being
                                                                      transmitted

SR.1 DOS Data Overrun Status;   1 overrun; a message was lost because there was
         note 6                   not enough space for that message in the RXFIFO
                                0 absent; no data overrun has occurred since the
                                  last clear data overrun command was given
SR.0 RBS Receive Buffer Status; 1 full; one or more complete messages are available
         note 7                   in the RXFIFO
                                0 empty; no message is available


*/
typedef struct{ 
	__uint8_t BS:1; 	//!< Bus Status
	__uint8_t ES:1; 	//!< Error Status
	__uint8_t TS:1; 	//!< Transmit Status
	__uint8_t RS:1; 	//!< Receive Status
	__uint8_t TCS:1; 	//!< Transmission Complete Status
	__uint8_t TBS:1; 	//!< Transmit Buffer Status
	__uint8_t DOS:1; 	//!< Data Overrun Status
	__uint8_t RBS:1; 	//!< Receive Buffer Status
}sja1000_pelican_sr_t;

/*!
IR.7 BEI Bus Error Interrupt        1 set; this bit is set when the CAN controller detects
                                      an error on the CAN-bus and the BEIE bit is set
                                      within the interrupt enable register
                                    0 reset
IR.6 ALI Arbitration Lost Interrupt 1 set; this bit is set when the CAN controller lost the
                                      arbitration and becomes a receiver and the ALIE
                                      bit is set within the interrupt enable register
                                    0 reset
IR.5 EPI Error Passive Interrupt    1 set; this bit is set whenever the CAN controller has
                                      reached the error passive status (at least one
                                      error counter exceeds the protocol-deﬁned level of
                                      127) or if the CAN controller is in the error passive
                                      status and enters the error active status again and
                                      the EPIE bit is set within the interrupt enable
                                      register
                                    0 reset
IR.4 WUI Wake-Up Interrupt;         1 set; this bit is set when the CAN controller is
         note 1                       sleeping and bus activity is detected and the
                                      WUIE bit is set within the interrupt enable register
                                    0 reset
IR.3 DOI Data Overrun Interrupt     1 set; this bit is set on a ‘0-to-1’ transition of the data
                                      overrun status bit and the DOIE bit is set within
                                      the interrupt enable register
                                    0 reset
IR.2 EI  Error Warning Interrupt    1 set; this bit is set on every change (set and clear)
                                      of either the error status or bus status bits and the
                                      EIE bit is set within the interrupt enable register
                                    0 reset
IR.1 TI  Transmit Interrupt         1 set; this bit is set whenever the transmit buffer
                                      status changes from ‘0-to-1’ (released) and the
                                      TIE bit is set within the interrupt enable register
                                    0 reset
IR.0 RI  Receive Interrupt; note 2  1 set; this bit is set while the receive FIFO is not
                                      empty and the RIE bit is set within the interrupt
                                      enable register
                                    0 reset; no more message is available within the
                                      RXFIFO

*/
typedef struct{ 
	__uint8_t BEI:1; 	//!< Bus Error Interrupt
	__uint8_t ALI:1; 	//!< Arbitration Lost Interrupt
	__uint8_t EPI:1; 	//!< Error Passive Interrupt
	__uint8_t WUI:1; 	//!< Wake-Up Interrupt
	__uint8_t DOI:1; 	//!< Data Overrun Interrupt
	__uint8_t EI:1; 	//!< Error Warning Interrupt
	__uint8_t TI:1; 	//!< Transmit Interrupt
	__uint8_t RI:1; 	//!< Receive Interrupt
}sja1000_pelican_ir_t;

/*!
Table 16 Bit interpretation of the interrupt enable register (IER); CAN address 4
    BIT      SYMBOL                 NAME                  VALUE                            FUNCTION
 IER.7      BEIE          Bus Error Interrupt                1        enabled; if an bus error has been detected, the
                          Enable                                      CAN controller requests the respective interrupt
                                                             0        disabled
 IER.6      ALIE          Arbitration Lost Interrupt         1        enabled; if the CAN controller has lost arbitration,
                          Enable                                      the respective interrupt is requested
                                                             0        disabled
 IER.5      EPIE          Error Passive Interrupt            1        enabled; if the error status of the CAN controller
                          Enable                                      changes from error active to error passive or vice
                                                                      versa, the respective interrupt is requested
                                                             0        disabled
 IER.4      WUIE          Wake-Up Interrupt                  1        enabled; if the sleeping CAN controller wakes up,
                          Enable                                      the respective interrupt is requested
                                                             0        disabled
 IER.3      DOIE          Data Overrun Interrupt             1        enabled; if the data overrun status bit is set (see
                          Enable                                      status register; Table 14), the CAN controller
                                                                      requests the respective interrupt
                                                             0        disabled
                                                             1        enabled; if the error or bus status change (see
 IER.2      EIE           Error Warning Interrupt
                          Enable                                      status register; Table 14), the CAN controller
                                                                      requests the respective interrupt
                                                             0        disabled
 IER.1      TIE           Transmit Interrupt Enable          1        enabled; when a message has been successfully
                                                                      transmitted or the transmit buffer is accessible
                                                                      again (e.g. after an abort transmission command),
                                                                      the CAN controller requests the respective
                                                                      interrupt
                                                             0        disabled
 IER.0      RIE           Receive Interrupt                  1        enabled; when the receive buffer status is ‘full’ the
                          Enable; note 1                              CAN controller requests the respective interrupt
                                                             0        disabled

*/
typedef struct{ 
	__uint8_t BEIE:1; 	//!< Bus Error Interrupt Enable
	__uint8_t ALIE:1; 	//!< Arbitration Lost Interrupt Enable
	__uint8_t EPIE:1; 	//!< Error Passive Interrupt Enable
	__uint8_t WUIE:1; 	//!< Wake-Up Interrupt Enable
	__uint8_t DOIE:1; 	//!< Data Overrun Interrupt Enable
	__uint8_t EIE:1; 	//!< Error Warning Interrupt Enable
	__uint8_t TIE:1; 	//!< Transmit Interrupt Enable
	__uint8_t RIE:1; 	//!< Receive Interrupt Enable
}sja1000_pelican_ier_t;

/*!
Table 17 Bit interpretation of the arbitration lost capture register (ALC); CAN address 11
    BIT        SYMBOL               NAME                           VALUE                   FUNCTION
              −
 ALC.7 to                  reserved                   For value and function see Table 18
 ALC.5
 ALC.4        BITNO4       bit number 4
 ALC.3        BITNO3       bit number 3
 ALC.2        BITNO2       bit number 2
 ALC.1        BITNO1       bit number 1
 ALC.0        BITNO0       bit number 0

*/
typedef struct{ 
	__uint8_t padd:3; 	//!< Padding
	__uint8_t BITNO:5; 	//!< Bit number indicating where arbitration is lost. @note gap in value 11-12 (SRTR-IDE bits)
}sja1000_pelican_alc_t;


/*!
Table 19 Bit interpretation of the error code capture register (ECC); CAN address 12
   BIT       SYMBOL                 NAME                VALUE                            FUNCTION
                                                            −         −
 ECC.7(1)   ERRC1         Error Code 1
                                                            −         −
 ECC.6(1)   ERRC0         Error Code 0
 ECC.5      DIR           Direction                         1         RX; error occurred during reception
                                                            0         TX; error occurred during transmission
                                                            −         −
 ECC.4(2)   SEG4          Segment 4
                                                            −         −
 ECC.3(2)   SEG3          Segment 3
                                                            −         −
 ECC.2(2)   SEG2          Segment 2
                                                            −         −
 ECC.1(2)   SEG1          Segment 1
                                                            −         −
 ECC.0(2)   SEG0          Segment 0


Table 20 Bit interpretation of bits ECC.7 and ECC.6
                                  BIT ECC.6
       BIT ECC.7                                                       FUNCTION
           0                          0            bit error
           0                          1            form error
           1                          0            stuff error
           1                          1            other type of error


Table 21 Bit interpretation of bits ECC.4 to ECC.0; note 1
 BIT ECC.4 BIT ECC.3 BIT ECC.2 BIT ECC.1 BIT ECC.0                              FUNCTION

02     00010 	ID.28 to ID.21
03     00011 	start of frame


04     00100 	bit SRTR
05     00101 	bit IDE
06     00110 	ID.20 to ID.18
07     00111 	ID.17 to ID.13

08     01000 	CRC sequence
09     01001 	reserved bit 0
0A     01010 	data ﬁeld
0B     01011 	data length code

0C     01100 	bit RTR
0D     01101 	reserved bit 1
0E     01110 	ID.4 to ID.0
0F     01111 	ID.12 to ID.5

11     10001 	active error ﬂag
12     10010 	intermission
13     10011 	tolerate dominant bits

16     10110 	passive error ﬂag
17     10111 	error delimiter

18     11000 	CRC delimiter
19     11001 	acknowledge slot
1A     11010 	end of frame
1B     11011 	acknowledge delimiter
1C     11100 	overload ﬂag

*/
typedef struct{ 
	__uint8_t ERRC:2; 	//!< 
	__uint8_t DIR:1; 	//!< 
	__uint8_t SEG:5; 	//!< 
}sja1000_pelican_ecc_t;

/*!
Table 25 TX frame information (SFF); CAN address 16
    BIT 7         BIT 6         BIT 5        BIT 4   BIT 3    BIT 2    BIT 1    BIT 0
    FF(1)        RTR(2)          X(3)         X(3)  DLC.3(4) DLC.2(4) DLC.1(4) DLC.0(4)

*/

typedef	struct{
		__uint8_t FF:1; 	//!< 1=EFF; extended frame format
		__uint8_t RTR:1; 	//!< Remote Transmission Request
		__uint8_t padd:2; 	//!< Padding
		__uint8_t DLC:4; 	//!<  Data length code
	}sja1000_pelican_frame_info_t;

typedef	struct{
		__uint16_t	id:11;
		__uint16_t	padd2:5;
		__uint8_t	data[8];
	}sja1000_pelican_frame_SFF_t;

typedef	struct{

		__uint32_t	id:29;
		__uint32_t	padd2:3;
		__uint8_t	data[8];
	}sja1000_pelican_frame_EFF_t;

/*
struct sja1000_pelican_frame_st{
	sja1000_pelican_frame_info_t frame_info;
	union{
		sja1000_pelican_frame_SFF_t SFF;
		sja1000_pelican_frame_EFF_t EFF;
	}format;
}__attribute__((__packed__));

typedef struct sja1000_pelican_frame_st sja1000_pelican_frame_t;
*/

typedef struct {
	sja1000_pelican_frame_info_t frame_info;
	union{
		sja1000_pelican_frame_SFF_t SFF;
		sja1000_pelican_frame_EFF_t EFF;
	}format;
}__attribute__((__packed__)) sja1000_pelican_frame_t;


/*!
sja1000_pelican_t struct mapped to memory layout.
*/
typedef struct{
	sja1000_pelican_mod_t	mod;	//!< MODE REGISTER (MOD)
	sja1000_pelican_cmr_t	cmr;	//!< command register (CMR); CAN address 1
	sja1000_pelican_sr_t	sr;	//!< status register (SR); CAN address 2
	sja1000_pelican_ir_t	ir;	//!< interrupt register (IR); CAN address 3
	sja1000_pelican_ier_t	ier;	//!< INTERRUPT ENABLE REGISTER (IER); CAN address 4
	__uint8_t		_rsrvd0;//!< Reserved; CAN address 5
	sja1000_btr0_t		btr0;	//!< BUS TIMING REGISTER 0 (BTR0); CAN address 6
	sja1000_btr1_t		btr1;	//!< BUS TIMING REGISTER 1 (BTR1); CAN address 7
	sja1000_ocr_t   	ocr;	//!< OUTPUT CONTROL REGISTER (OCR); CAN address 8
	__uint8_t		test;	//!< test; CAN address 9
	__uint8_t		_rsrvd1;//!< Reserved; CAN address 10
	sja1000_pelican_alc_t	alc;	//!< ARBITRATION LOST CAPTURE REGISTER (ALC); CAN address 11
	sja1000_pelican_ecc_t	ecc;	//!< Error Code Capture (ECC); CAN address 12
	__uint8_t		ewl;	//!< Error Warning Limit; CAN address 13
	__uint8_t		rxerr;	//!< RX Error Counter; CAN address 14
	__uint8_t		txerr;	//!< TX Error Counter; CAN address 15
	union {
		sja1000_pelican_frame_t	frame;	//!< Rx & Tx frame (depending on if reading or writing)
		struct{
			__uint32_t	code;
			__uint32_t	mask;
		}__attribute__((__packed__)) acceptance;
	}frame;				//!< TX/RX frame or acceptance; CAN address 16-28
	__uint8_t		rmc;	//!< RX MESSAGE COUNTER (RMC); CAN address 29
	__uint8_t		rbsa;	//!< RX BUFFER START ADDRESS REGISTER (RBSA); CAN address 30
 	sja1000_cdr_t		cdr	//!< clock divider register (CDR); CAN address 31
}sja1000_pelican_t;


#endif //MSR_H

