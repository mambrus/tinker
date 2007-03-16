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

#ifndef SJA1000_BASIC_H
#define SJA1000_BASIC_H

#include <sys/types.h>
#include "sja1000_common.h"

/*!
@file

BasicCAN address allocation
===========================
                              OPERATING MODE                                RESET MODE
   CAN
         SEGMENT
 ADDRESS                  READ                WRITE                 READ                  WRITE
0        control  control             control               control               control
1                 (FFH)               command               (FFH)                 command
                                      −                                           −
2                 status                                    status
                                      −                                           −
3                 interrupt                                 interrupt
                                      −
4                 (FFH)                                     acceptance code       acceptance code
                                      −
5                 (FFH)                                     acceptance mask       acceptance mask
                                      −
6                 (FFH)                                     bus timing 0          bus timing 0
                                      −
7                 (FFH)                                     bus timing 1          bus timing 1
                                      −
8                 (FFH)                                     output control        output control
9                 test                test; note 2          test                  test; note 2
                                                                                  −
10       transmit identiﬁer (10 to 3) identiﬁer (10 to 3)   (FFH)
                                                                                  −
         buffer
11                identiﬁer (2 to 0), identiﬁer (2 to 0),   (FFH)
                  RTR and DLC         RTR and DLC
                                                                                  −
12                data byte 1         data byte 1           (FFH)
                                                                                  −
13                data byte 2         data byte 2           (FFH)
                                                                                  −
14                data byte 3         data byte 3           (FFH)
                                                                                  −
15                data byte 4         data byte 4           (FFH)
                                                                                  −
16                data byte 5         data byte 5           (FFH)
                                                                                  −
17                data byte 6         data byte 6           (FFH)
                                                                                  −
18                data byte 7         data byte 7           (FFH)
                                                                                  −
19                data byte 8         data byte 8           (FFH)
20       receive  identiﬁer (10 to 3) identiﬁer (10 to 3)   identiﬁer (10 to 3)   identiﬁer (10 to 3)
         buffer
21                identiﬁer (2 to 0), identiﬁer (2 to 0),   identiﬁer (2 to 0),   identiﬁer (2 to 0),
                  RTR and DLC         RTR and DLC           RTR and DLC           RTR and DLC
22                data byte 1         data byte 1           data byte 1           data byte 1
23                data byte 2         data byte 2           data byte 2           data byte 2
24                data byte 3         data byte 3           data byte 3           data byte 3
25                data byte 4         data byte 4           data byte 4           data byte 4
26                data byte 5         data byte 5           data byte 5           data byte 5
27                data byte 6         data byte 6           data byte 6           data byte 6
28                data byte 7         data byte 7           data byte 7           data byte 7
29                data byte 8         data byte 8           data byte 8           data byte 8
                                      −                                           −
30                (FFH)                                     (FFH)
31                clock divider       clock divider; note 3 clock divider         clock divider

*/


//-----------------------------------------------------------------------------
/*!
Table 3 Bit interpretation of the control register (CR); CAN address 0
   BIT      SYMBOL                 NAME                  VALUE                           FUNCTION
           −             −                                 −
 CR.7                                                               reserved; note 1
           −             −                                 −
 CR.6                                                               reserved; note 2
           −             −                                 −
 CR.5                                                               reserved; note 3
 CR.4      OIE           Overrun Interrupt Enable          1        enabled; if the data overrun bit is set, the
                                                                    microcontroller receives an overrun interrupt
                                                                    signal (see also status register; Table 5)
                                                           0        disabled; the microcontroller receives no overrun
                                                                    interrupt signal from the SJA1000
 CR.3      EIE           Error Interrupt Enable            1        enabled; if the error or bus status change, the
                                                                    microcontroller receives an error interrupt signal
                                                                    (see also status register; Table 5)
                                                           0        disabled; the microcontroller receives no error
                                                                    interrupt signal from the SJA1000
 CR.2      TIE           Transmit Interrupt Enable         1        enabled; when a message has been successfully
                                                                    transmitted or the transmit buffer is accessible
                                                                    again, (e.g. after an abort transmission command)
                                                                    the SJA1000 transmits a transmit interrupt signal
                                                                    to the microcontroller
                                                           0        disabled; the microcontroller receives no transmit
                                                                    interrupt signal from the SJA1000

CR.1 RIE Receive Interrupt Enable 1 enabled; when a message has been received
                                    without errors, the SJA1000 transmits a receive
                                    interrupt signal to the microcontroller
                                  0 disabled; the microcontroller receives no transmit
                                    interrupt signal from the SJA1000
CR.0 RR  Reset Request; note 4    1 present; detection of a reset request results in
                                    aborting the current transmission/reception of a
                                    message and entering the reset mode
                                  0 absent; on the ‘1-to-0’ transition of the reset
                                    request bit, the SJA1000 returns to the operating
                                    mode

*/

typedef struct{ 
	__uint8_t padd:3;	//!< padding	
	__uint8_t OIE:1; 	//!< Overrun Interrupt Enable
	__uint8_t EIE:1; 	//!< Error Interrupt Enable
	__uint8_t TIE:1; 	//!< Transmit Interrupt Enable 
	__uint8_t RIE:1; 	//!< Receive Interrupt Enable
	__uint8_t RR:1; 	//!< Reset Request
}sja1000_basic_cr_t;

/*!
Table 4 Bit interpretation of the command register (CMR); CAN address 1
   BIT      SYMBOL                NAME               VALUE                           FUNCTION
           −             −                             −
 CMR.7                                                          reserved
           −             −                             −
 CMR.6                                                          reserved
           −             −                             −
 CMR.5                                                          reserved
 CMR.4     GTS           Go To Sleep; note 1           1        sleep; the SJA1000 enters sleep mode if no CAN
                                                                interrupt is pending and there is no bus activity
                                                       0        wake up; SJA1000 operates normal
 CMR.3     CDO           Clear Data Overrun;           1        clear; data overrun status bit is cleared
                         note 2                        0        no action
 CMR.2     RRB           Release Receive Buffer;       1        released; the receive buffer, representing the
                         note 3                                 message memory space in the RXFIFO is
                                                                released
                                                       0        no action
 CMR.1     AT            Abort Transmission;           1        present; if not already in progress, a pending
                         note 4                                 transmission request is cancelled
                                                       0        absent; no action
 CMR.0     TR            Transmission Request;         1        present; a message will be transmitted
                         note 5                        0        absent; no action
*/
typedef struct{ 
	__uint8_t padd:3;	//!< padding	
	__uint8_t GTS:1; 	//!< Go To Sleep
	__uint8_t CD0:1; 	//!< Clear Data Overrun
	__uint8_t RRB:1; 	//!< Release Receive Buffer
	__uint8_t AT:1; 	//!< Abort Transmission
	__uint8_t TR:1; 	//!< Transmission Request
}sja1000_basic_cmr_t;

/*!
Table 5 Bit interpretation of the status register (SR); CAN address 2
   BIT      SYMBOL                NAME                   VALUE                           FUNCTION
 SR.7      BS            Bus Status; note 1          1              bus-off; the SJA1000 is not involved in bus
                                                                    activities
                                                     0              bus-on; the SJA1000 is involved in bus activities
 SR.6      ES            Error Status; note 2        1              error; at least one of the error counters has
                                                                    reached or exceeded the CPU warning limit
                                                     0              ok; both error counters are below the warning limit
 SR.5      TS            Transmit Status; note 3     1              transmit; the SJA1000 is transmitting a message
                                                     0              idle; no transmit message is in progress
 SR.4      RS            Receive Status; note 3      1              receive; the SJA1000 is receiving a message
                                                     0              idle; no receive message is in progress
 SR.3      TCS           Transmission Complete       1              complete; the last requested transmission has
                         Status; note 4                             been successfully completed
                                                     0              incomplete; the previously requested transmission
                                                                    is not yet completed
 SR.2      TBS           Transmit Buffer Status;     1              released; the CPU may write a message into the
                         note 5                                     transmit buffer
                                                     0              locked; the CPU cannot access the transmit
                                                                    buffer; a message is waiting for transmission or is
                                                                    already in process
 SR.1      DOS           Data Overrun Status;        1              overrun; a message was lost because there was
                         note 6                                     not enough space for that message in the RXFIFO
                                                     0              absent; no data overrun has occurred since the
                                                                    last clear data overrun command was given
 SR.0      RBS           Receive Buffer Status;      1              full; one or more messages are available in the
                         note 7                                     RXFIFO
                                                     0              empty; no message is available

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
}sja1000_basic_sr_t;


/*!
Table 6 Bit interpretation of the interrupt register (IR); CAN address 3
    BIT     SYMBOL                 NAME                   VALUE                             FUNCTION
           −             −                            −
 IR.7                                                                reserved; note 1
           −             −                            −              reserved; note 1
 IR.6
           −             −                            −
 IR.5                                                                reserved; note 1
 IR.4      WUI           Wake-Up Interrupt;           1              set; this bit is set when the sleep mode is left
                         note 2                       0              reset; this bit is cleared by any read access of the
                                                                     microcontroller
 IR.3      DOI           Data Overrun Interrupt;      1              set; this bit is set on a ‘0-to-1’ transition of the data
                         note 3                                      overrun status bit, when the data overrun interrupt
                                                                     enable is set to logic 1 (enabled)
                                                      0              reset; this bit is cleared by any read access of the
                                                                     microcontroller
 IR.2      EI            Error Interrupt              1              set; this bit is set on a change of either the error
                                                                     status or bus status bits if the error interrupt
                                                                     enable is set to logic 1 (enabled)
                                                      0              reset; this bit is cleared by any read access of the
                                                                     microcontroller
 IR.1      TI            Transmit Interrupt           1              set; this bit is set whenever the transmit buffer
                                                                     status changes from logic 0 to logic 1 (released)
                                                                     and transmit interrupt enable is set to logic 1
                                                                     (enabled)
                                                      0              reset; this bit is cleared by any read access of the
                                                                     microcontroller
 IR.0      RI            Receive Interrupt; note 4 1                 set; this bit is set while the receive FIFO is not
                                                                     empty and the receive interrupt enable bit is set
                                                                     to logic 1 (enabled)
                                                      0              reset; this bit is cleared by any read access of the
                                                                     microcontroller

*/
typedef struct{ 
	__uint8_t padd:3; 	//!< Padding
	__uint8_t WUI:1; 	//!< Wake Up Interrupt
	__uint8_t DOI:1; 	//!< Data Overrun Interrupt
	__uint8_t EI:1; 	//!< Error Interrupt
	__uint8_t TI:1; 	//!< Transmitt Interrupt
	__uint8_t RI:1; 	//!< Receive Interrupt
}sja1000_basic_ir_t;

/*!
Table 7 Layout of transmit buffer
                                                                     BITS
    CAN
              FIELD             NAME
 ADDRESS                                    7        6        5    4       3     2     1     0
     10     descriptor identiﬁer byte 1  ID.10    ID.9     ID.8 ID.7    ID.6  ID.5  ID.4  ID.3
     11                 identiﬁer byte 2 ID.2     ID.1     ID.0 RTR     DLC.3 DLC.2 DLC.1 DLC.0
     12     data        TX data 1        transmit data byte 1
     13                 TX data 2        transmit data byte 2
     14                 TX data 3        transmit data byte 3
     15                 TX data 4        transmit data byte 4
     16                 TX data 5        transmit data byte 5
     17                 TX data 6        transmit data byte 6
     18                 TX data 7        transmit data byte 7
     19                 TX data 8        transmit data byte 8
*/

typedef struct{ 
	__uint16_t ID:11; 	//!< Identiﬁer
	__uint16_t RTR:1; 	//!< Remote Transmission Request
	__uint16_t DLC:4; 	//!< Data Length Code 
	__uint8_t data[8]; 	//!< transmit data [8]
}sja1000_basic_buffer_t;


/*!
sja1000_basic_t struct mapped to memory layout.
*/
typedef struct{
	sja1000_basic_cr_t	cr;	//!< control register (CR); CAN address 0
	sja1000_basic_cmr_t	cmr;	//!< command register (CMR); CAN address 1
	sja1000_basic_sr_t	sr;	//!< status register (SR); CAN address 2
	sja1000_basic_ir_t	ir;	//!< interrupt register (IR); CAN address 3
	__uint8_t		acr;	//!< Acceptance Code Register (ACR); CAN address 4
	__uint8_t		amr;	//!< Acceptance Mask Register (AMR); CAN address 5
	sja1000_btr0_t		btr0;	//!< BUS TIMING REGISTER 0 (BTR0); CAN address 6
	sja1000_btr1_t		btr1;	//!< BUS TIMING REGISTER 1 (BTR1); CAN address 7
	sja1000_ocr_t   	ocr;	//!< OUTPUT CONTROL REGISTER (OCR); CAN address 8
	__uint8_t		test;	//!< test; CAN address 9
	sja1000_basic_buffer_t	txb;	//!< transmit buffer; CAN address 10-19
	sja1000_basic_buffer_t	rxb;	//!< transmit buffer; CAN address 20-29
	__uint8_t		padd;	//!< padding; CAN address 30
 	sja1000_cdr_t		cdr;	//!< clock divider register (CDR); CAN address 31
}sja1000_basic_t;

#endif //SJA1000_BASIC_H

