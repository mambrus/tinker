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

#ifndef CM_SMC_H
#define CM_SMC_H

//Stuff for the SMC part of the communications module (CM)

#include <sys/types.h>

/*
smc_rx_control_descriptor
===============================================================================
0  E Empty.
     0 The buffer is full or reception was aborted due to an error. The core can read or write to any ﬁelds
        of this BD. The CPM does not reuse this BD while E = 0.
     1 The buffer is not full. The CPM controls this BD and buffer. The core should not modify this BD.
1 —  Reserved, should be cleared.
2 W  Wrap (last buffer descriptor in the BD table).
     0 Not the last descriptor in the table.
     1 Last descriptor in the table. After this buffer is used, the CPM receives incoming data using the
        BD pointed to by RBASE. The number of BDs in this table is programmable and determined only
        by the W bit.
3  I Interrupt.
     0 No interrupt is generated after this buffer is ﬁlled.
     1 The CP sets SCCE[RX] when this buffer is completely ﬁlled by the CPM, indicating the need for
        the core to process the buffer. Setting SCCE[RX] causes an interrupt if not masked.
4  C Control character.
     0 This buffer does not contain a control character.
     1 The last byte in this buffer matches a user-deﬁned control character.
5  A Address.
     0 The buffer contains only data.
     1 For manual multidrop mode, A indicates the ﬁrst byte of this buffer is an address byte. Software
        should perform address comparison. In automatic multidrop mode, A indicates the buffer contains
        a message received immediately after an address matched UADDR1 or UADDR2. The address
        itself is not written to the buffer but is indicated by the AM bit.
6 CM Continuous mode.
     0 Normal operation. The CPM clears E after this BD is closed.
     1 The CPM does not clear E after this BD is closed, allowing the buffer to be overwritten when the
        CPM accesses this BD again. E is cleared if an error occurs during reception, regardless of CM.
7 ID Buffer closed on reception of idles. The buffer is closed because a programmable number of
     consecutive idle sequences (MAX_IDL) was received.
8 AM Address match. Signiﬁcant only if the address bit is set and automatic multidrop mode is selected in
     PSMR[UM]. After an address match, AM identiﬁes which user-deﬁned address character was
     matched.
     0 The address matched the value in UADDR2.
     1 The address matched the value in UADDR1.
9 —  Reserved, should be cleared.
10 BR Break received. Set when a break sequence is received as data is being received into this buffer.
11 FR Framing error. Set when a character with a framing error (a character without a stop bit) is received
      and located in the last byte of this buffer. A new Rx buffer is used to receive subsequent data.
12 PR Parity error. Set when a character with a parity error is received and located in the last byte of this
      buffer. A new Rx buffer is used to receive subsequent data.
13 —  Reserved, should be cleared.
14 OV Overrun. Set when a receiver overrun occurs during reception.
15 CD Carrier detect lost. Set when the carrier detect signal is negated during reception.
*/

typedef union{ 
	__uint16_t raw; 
	struct {
		__uint16_t E_Empty					:1;
		__uint16_t Padd_0					:1;
		__uint16_t W_Wrap					:1;
		__uint16_t I_Interrupt					:1;
		__uint16_t C_Control_character				:1;
		__uint16_t A_Address					:1;
		__uint16_t CM_Continuous_mode				:1;
		__uint16_t ID_Buffer_closed_on_reception_of_idles	:1;
		__uint16_t AM_Address_match				:1;
		__uint16_t Padd_1					:1;
		__uint16_t BR_Break_received				:1;
		__uint16_t FR_Framing_error				:1;
		__uint16_t PR_Parity_error				:1;
		__uint16_t Padd_2					:1;
		__uint16_t OV_Overrun					:1;
		__uint16_t CD_Carrier_detect_lost			:1;
	}f;
}bd_ctrl_smc_rx_t;

/*
smc_TX_control_descriptor
===============================================================================
0 R Ready.
    0 The buffer is not ready. This BD and buffer can be modiﬁed. The CPM automatically clears R after
       the buffer is sent or an error occurs.
    1 The user-prepared buffer is waiting to begin transmission or is being transmitted. Do not modify
       the BD once R is set.
1 — Reserved, should be cleared.
2 W Wrap (last buffer descriptor in TxBD table).
    0 Not the last BD in the table.
    1 Last BD in the table. After this buffer is used, the CPM sends data using the BD pointed to by
       TBASE. The number of TxBDs in this table is determined only by the W bit.
3 I Interrupt.
    0 No interrupt is generated after this buffer is processed.
    1 SCCE[TX] is set after this buffer is processed by the CPM, which can cause an interrupt.
 4   CR Clear-to-send report.
        0 The next buffer is sent with no delay (assuming it is ready), but if a CTS lost condition occurs,
          TxBD[CT] may not be set in the correct TxBD or may not be set at all. Asynchronous ﬂow control,
          however, continues to function normally.
        1 Normal CTS lost error reporting and three bits of idle are sent between consecutive buffers.
 5    A Address. Valid only in multidrop mode—automatic or manual.
        0 This buffer contains only data.
        1 This buffer contains address characters. All data in this buffer is sent as address characters.
 6   CM Continuous mode.
        0 Normal operation. The CPM clears R after this BD is closed.
        1 The CPM does not clear R after this BD is closed, allowing the buffer to be resent next time the
          CPM accesses this BD. However, R is cleared by transmission errors, regardless of CM.
 7    P Preamble.
        0 No preamble sequence is sent.
        1 Before sending data, the controller sends an idle character consisting of all ones. If the data length
          of this BD is zero, only a preamble is sent.
 8   NS No stop bit or shaved stop bit sent.
        0 Normal operation. Stop bits are sent with all characters in this buffer.
        1 If PSMR[SYN] = 1, data in this buffer is sent without stop bits. If SYN = 0, the stop bit is shaved,
          depending on the DSR setting; see Section 22.14, “Fractional Stop Bits (Transmitter).”
9–14 —  Reserved, should be cleared.
 15  CT CTS lost. The CPM writes this status bit after sending the associated buffer.
        0 CTS remained asserted during transmission.
        1 CTS negated during transmission.
*/


typedef union{ 
	__uint16_t raw;
	struct {
		__uint16_t R_Ready					:1;
		__uint16_t Padd_0					:1;
		__uint16_t W_Wrap					:1;
		__uint16_t I_Interrupt					:1;
		__uint16_t CR_Clear_to_send_report			:1;
		__uint16_t A_Address					:1;
		__uint16_t CM_Continuous_mode				:1;
		__uint16_t P_Preamble					:1;
		__uint16_t NS_No_stop_bit_sent				:1;
		__uint16_t Padd_1					:5;
		__uint16_t CT_CTS_lost					:1;
	}f;
}bd_ctrl_smc_tx_t;

/* Buffer Descriptor (BD) stuff */
/* 
Note: that the status and control field (BD_Status) is differend depending on what type of
protocol and directin the descriptor is beeing used for.

Note: control and status is the same for SCC and SMC, Direction differs however...
*/

/*
                     0x00       Status and control
                     0x02       Data length
                     0x04       High-order of buffer pointer
                     0x06       Low-order of buffer pointer
*/

typedef struct{ 
	union {
		__uint16_t raw;
		bd_ctrl_smc_rx_t SMC_RX;	/*Note: same as for SCC*/
		bd_ctrl_smc_tx_t SMC_TX;	/*Note: same as for SCC*/
	}BD_Status;
	__uint16_t BD_Length;
	__uint32_t BD_Pointer;
}bd_smc_t;

/* Function code register (for RFCR and TFCR in smc_param_t)*/
/*
0–2   —     Reserved, should be cleared.
3–4   BO    Byte ordering. Program BO to select the required byte ordering for the buffer. If BO is changed
            on-the-ﬂy, it takes effect at the beginning of the next frame (Ethernet, HDLC, and transparent) or at
            the beginning of the next BD. See Appendix A, “Byte Ordering.”
            00 Reserved
            01 Modiﬁed little-endian.
            1x Big-endian or true little-endian.
5–7 AT[1–3] Address type. Contains the function code value used during the SDMA channel memory access.
            Note AT[0] is driven high to identify this SDMA channel access as a DMA type.
*/


/*Note: This happens to be the same as for SCC, but we differ between them anyway because 
not all fcr are the same (CM has more then SCC and SMC) and we want to follow some 
convention*/
typedef union{ 
	__uint8_t raw; 
	struct {
		__uint8_t PADD_0:3;
		__uint8_t BO:2;
		__uint8_t AT:3;

	}f;
}smc_fcr_t;

/*
         Table 29-2. SMC UART and Transparent Parameter RAM Memory Map
Offset 1  Name   Width                                           Description


 0x00    RBASE  Hword  RxBDs and TxBDs base address. (BD table pointer) Deﬁne starting points in the dual-port
                       RAM of the set of BDs for the SMC send and receive functions. They allow ﬂexible
 0x02    TBASE  Hword  partitioning of the BDs. By selecting RBASE and TBASE entries for all SMCs and by
                       setting W in the last BD in each list, BDs are allocated for the send and receive side of
                       every SMC. Initialize these entries before enabling the corresponding channel.
                       Conﬁguring BD tables of two enabled SMCs to overlap causes erratic operation. RBASE
                       and TBASE should be a multiple of eight.
 0x04    RFCR   Byte   Rx/Tx function code. See Section 29.2.3.1, “SMC Function Code Registers
                       (RFCR/TFCR).”
 0x05    TFCR   Byte
 0x06    MRBLR  Hword  Maximum receive buffer length. The most bytes the MPC860 writes to a Rx buffer before
                       moving to the next buffer. It can write fewer bytes than MRBLR if a condition like an error
                       or end-of-frame occurs, but it cannot exceed MRBLR. Rx buffers should not be smaller
                       than MRBLR. SMC Tx buffers are unaffected by MRBLR.
                       Tx buffers can be individually given varying lengths through the data length ﬁeld. MRBLR
                       can be changed while an SMC is operating only if it is done in a single bus cycle with one
                       16-bit move (not two 8-bit bus cycles back-to-back). This occurs when the CP shifts
                       control to the next RxBD, so the change does not take effect immediately. To guarantee
                       the exact RxBD on which the change occurs, change MRBLR only while the SMC
                       receiver is disabled. MRBLR should be greater than zero and should be even if character
                       length exceeds 8 bits.
 0x08    RSTATE Word   Rx internal state. Can be used only by the CP.
                       Rx internal data pointer. 2 Updated by the SDMA channels to show the next address in
 0x0C    —      Word
                       the buffer to be accessed.
 0x10    RBPTR  Hword  RxBD pointer. Points to the next BD for each SMC channel that the receiver transfers data
                       to when it is in idle state, or to the current BD during frame processing. After a reset or
                       when the end of the BD table is reached, the CP initializes RBPTR to the value in RBASE.
                       Most applications never need to write RBPTR, but it can be written when the receiver is
                       disabled or when no receive buffer is in use.
                       Rx internal byte count. 2 A down-count value initialized with the MRBLR value and
 0x12    —      Hword
                       decremented with every byte the SDMA channels write.
                       Rx temp. 2 Can be used only by the CP.
 0x14    —      Word
 0x18    TSTATE Word   Tx internal state. Can be used only by the CP.
                       Tx internal data pointer. 2 Updated by the SDMA channels to show the next address in
 0x1C    —      Word
                       the buffer to be accessed.
 0x20    TBPTR  Hword  TxBD pointer. Points to the next BD for each SMC channel the transmitter transfers data
                       from when it is in idle state or to the current BD during frame transmission. After reset or
                       when the end of the table is reached, the CP initializes TBPTR to the TBASE value. Most
                       applications never need to write TBPTR, but it can be written when the transmitter is
                       disabled or when no transmit buffer is in use. For instance, after a STOP TRANSMIT or
                       GRACEFUL STOP TRANSMIT command is issued and the frame completes its transmission.

 0x22    —      Hword
                       Tx internal byte count. 2 A down-count value initialized with the TxBD data length and

                       decremented with every byte the SDMA channels read.

 0x24    —      Word
                       Tx temp. 2 Can be used only by the CP.


0x28 — Hword First half-word of protocol-speciﬁc area.
0x32 — Hword Last half-word of protocol-speciﬁc area.



SMC_UART_STUFF
===============================================================================


0x28 MAX_IDL Hword Maximum idle characters. When a character is received on the line, the SMC starts
                   counting idle characters received. If MAX_IDL idle characters arrive before the next
                   character, an idle time-out occurs and the buffer closes, which sends an interrupt
                   request to the core to receive data from the buffer. An idle character is deﬁned as a full
                   character length of logic high. MAX_IDL can be used to demarcate frames in UART
                   mode. Clearing MAX_IDL disables this function so idle never causes the buffer to close,
                   regardless of how many idle characters are received. The length of an idle character is
                   calculated as follows: 1 + data length (5 to 14) + 1 (if parity bit is used) + number of stop
                   bits (1 or 2). For example, for 8 data bits, no parity, and 1 stop bit, character length is 10
                   bits.
0x2A IDLC    Hword Temporary idle counter. Down-counter in which the CP stores the current idle counter
                   value in the MAX_IDL time-out process.
0x2C BRKLN   Hword Last received break length. Holds the length of the last received break character
                   sequence measured in character units. For example, if the receive signal is low for 20
                   bit times and the deﬁned character length is 10 bits, BRKLN = 0x002, indicating that the
                   break sequence is at least 2 characters long. BRKLN is accurate to within one character
                   length.
0x2E BRKEC   Hword Receive break condition counter. Counts break conditions on the line. A break condition
                   may last for hundreds of bit times, yet BRKEC increments only once during that period.
0x30 BRKCR   Hword Break count register (transmit). Determines the number of break characters the UART
                   controller sends when the SMC sends a break character sequence after a STOP
                   TRANSMIT command. For 8 data bits, no parity, 1 stop bit, and 1 start bit, each break
                   character is 10 zeros.
0x32 R_MASK  Hword Temporary bit mask.

*/

typedef struct{ 

/*0x28 MAX_IDL*/ 	__uint16_t MAX_IDL;
/*0x2A IDLC*/    	__uint16_t IDLC;
/*0x2C BRKLN*/   	__uint16_t BRKLN;
/*0x2E BRKEC*/   	__uint16_t BRKEC;
/*0x30 BRKCR*/   	__uint16_t BRKCR;
/*0x32 R_MASK*/  	__uint16_t R_MASK;

}proto_smc_uart_t;




typedef struct{ 
	__uint16_t	RBASE;			/*0x00*/
	__uint16_t	TBASE;			/*0x02*/

	smc_fcr_t	RFCR;			/*0x04*/
	smc_fcr_t	TFCR;			/*0x05*/

	__uint16_t	MRBLR;			/*0x06*/

	__uint32_t	RSTATE;			/*0x08*/
	__uint32_t	UNKNOW_0;		/*0x0C*/

	__uint16_t	RBPTR;			/*0x10*/
	__uint16_t	UNKNOW_1;		/*0x12*/

	__uint32_t	UNKNOW_2;		/*0x14*/
	__uint32_t	TSTATE;			/*0x18*/
	__uint32_t	UNKNOW_3;		/*0x1C*/

	__uint16_t	TBPTR;			/*0x20*/
	__uint16_t	UNKNOW_4;		/*0x22*/

	__uint32_t	UNKNOW_5;		/*0x24*/
	struct {				/*0X28*/
		proto_smc_uart_t	SMC_UART;
	}PROTO;
	
}smc_param_t;


/*
                   Table 29-10. SMCE/SMCM Field Descriptions
Bits Name                                               Description
 0     —  Reserved, should be cleared.
 1   BRKE Break end. Set no sooner than after one idle bit is received after the break sequence.
 2     —  Reserved, should be cleared.
 3   BRK  Break character received. Set when a break character is received. If a very long break sequence
          occurs, this interrupt occurs only once after the ﬁrst all-zeros character is received.
 4     —  Reserved, should be cleared.
 5    BSY Busy condition. Set when a character is received and discarded due to a lack of buffers. Set no
          sooner than the middle of the last stop bit of the ﬁrst receive character for which there is no available
          buffer. Reception resumes when an empty buffer is provided.
 6     TX Tx buffer. Set when the transmit data of the last character in the buffer is written to the transmit FIFO.
          Wait two character times to ensure that data is completely sent over the transmit pin.
 7    RX  Rx buffer. Set when a buffer is received and its associated RxBD is closed. Set no sooner than the
          middle of the last stop bit of the last character that is written to the receive buffer.

*/


#define SMCE_BRKE			(1<<6)

#define SMCE_BRK			(1<<4)

#define SMCE_BSY			(1<<2)
#define SMCE_TX				(1<<1)
#define SMCE_RX				(1<<0)
typedef union{ 
	__uint8_t raw; 
	
	struct {
		__uint8_t PAD_0		:1;
		__uint8_t BRKE		:1;
		__uint8_t PAD_1		:1;
		__uint8_t BRK		:1;
		__uint8_t PAD_2		:1;
		__uint8_t BSY		:1;
		__uint8_t TX		:1;
		__uint8_t RX		:1;
	}f;
}smce_smcm_t;

/*
               Table 29-1. SMCMR Field Descriptions (continued)
Bits Name                                                 Description
0 —       Reserved, should be cleared
1–4  CLEN Character length (UART). Number of bits in the character minus one. The total is the sum of 1 (start
          bit always present) + number of data bits (5–14) + number of parity bits (0 or 1) + number of stop
          bits (1 or 2). For example, for 8 data bits, no parity, and 1 stop bit, the total number of bits in the
          character is 1 + 8 + 0 + 1 = 10. So, CLEN should be programmed to 9.
          Characters range from 5–14 bits. If the data bit length is less than 8, the msbs of each byte in
          memory are not used on transmit and are written with zeros on receive. If the length is more than 8,
          the msbs of each 16-bit word are not used on transmit and are written with zeros on receive.
          The character must not exceed 16 bits. For a 14-bit data length, set SL to one stop bit and disable
          parity. For a 13-bit data length with parity enabled, set SL to one stop bit. Writing values 0 to 3 to
          CLEN causes erratic behavior.
          Character length (transparent). The values 3–15 specify 4–16 bits per character. If a character is less
          than 8 bits, the msbs of the byte in buffer memory are not used on transmit and are written with zeros
          on receive. If character length is more than 8 bits but less than 16, the msbs of the half-word in buffer
          memory are not used on transmit and are written with zeros on receive.
          Note: Using values 0–2 causes erratic behavior. Larger character lengths increase an SMC
          channel’s potential performance and lowers the performance impact of other channels. For instance,
          using 16- rather than 8-bit characters is encouraged if 16-bit characters are acceptable in the end
          application.
          Character length (GCI). Number of bits in the C/I and monitor channels of the SCIT channels 0 or 1.
          Values 0–15 correspond to 1–16 bits. CLEN should be 13 for SCIT channel 0 or GCI (8 data bits,
          plus A and E bits, plus 4 C/I bits = 14 bits). It should be 15 for the SCIT channel 1 (8 data, bits, plus
          A and E bits, plus 6 C/I bits = 16 bits).
 5     SL Stop length. (UART)
          0 One stop bit.
          1 Two stop bits.
       —  Reserved, should be cleared (transparent)
      ME  Monitor enable. (GCI)
          0 The SMC does not support the monitor channel.
          1 The SMC supports the monitor channel.
 6    PEN Parity enable. (UART)
          0 No parity.
          1 Parity is enabled for the transmitter and receiver as determined by the PM bit.
       BS Byte sequence (transparent). For a character length greater than 8 bits, BS controls the byte
          transmission sequence if REVD is set. Clear BS to maintain compatibility with MC68360 QUICC.
          0 Normal mode. Should be selected if the character length is not larger than 8 bits.
          1 Transmit lower address byte ﬁrst.
       —  Reserved, should be cleared. (GCI)
 7    PM  Parity mode. (UART)
          0 Odd parity.
          1 Even parity.
     REVD Reverse data. (transparent)
          0 Normal mode.
          1 Reverse the character bit order. The msb is sent ﬁrst.
       C# SCIT channel number. (GCI)
          0 SCIT channel 0
          1 SCIT channel 1. Required for Siemens ARCOFI and SGS S/T chips.
8–9    —  Reserved, should be cleared
10–11 SM  SMC mode.
          00 GCI or SCIT support.
          01 Reserved.
          10 UART (must be selected for SMC UART operation).
          11 Totally transparent operation.
12–13 DM  Diagnostic mode.
          00 Normal operation.
          01 Local loopback mode.
          10 Echo mode.
          11 Reserved.
 14   TEN SMC transmit enable.
          0 SMC transmitter disabled.
          1 SMC transmitter enabled.
 15   REN SMC receive enable.
          0 SMC receiver disabled.
          1 SMC receiver enabled.
*/


/*Generic for - bugged, don't use this (FIXME)*/
typedef union{ 
	__uint16_t raw; 
	
	struct {
		__uint16_t PAD_0			:1;
		__uint16_t CLEN				:4;
		union {
			struct{
				__uint16_t SL		:1;
				__uint16_t PEN		:1;
				__uint16_t PM		:1;
			}uart;
			struct{
				__uint16_t PAD_0_1	:1;
				__uint16_t BS		:1;
				__uint16_t REVD		:1;
			}transp;
			struct{
				
				__uint16_t ME		:1;
				__uint16_t PAD_0_2	:1;
				__uint16_t C_NR		:1;
			}gci;
		}mode;
		__uint16_t PAD_1			:2;
		__uint16_t SM				:2;
		__uint16_t DM				:2;
		__uint16_t TEN				:1;
		__uint16_t REN				:1;
			
	}f;
}_smcmr_t;

typedef union{ 
	__uint16_t raw; 
	
	struct {
		__uint16_t PAD_0			:1;
		__uint16_t CLEN				:4;
				__uint16_t SL		:1;
				__uint16_t PEN		:1;
				__uint16_t PM		:1;
		__uint16_t PAD_1			:2;
		__uint16_t SM				:2;
		__uint16_t DM				:2;
		__uint16_t TEN				:1;
		__uint16_t REN				:1;
			
	}f;
}_smcmr_uart_t;

typedef union{ 
	__uint16_t raw; 
	
	struct {
		__uint16_t PAD_0			:1;
		__uint16_t CLEN				:4;
				__uint16_t PAD_0_1	:1;
				__uint16_t BS		:1;
				__uint16_t REVD		:1;
		__uint16_t PAD_1			:2;
		__uint16_t SM				:2;
		__uint16_t DM				:2;
		__uint16_t TEN				:1;
		__uint16_t REN				:1;
			
	}f;
}_smcmr_transp_t;

typedef union{ 
	__uint16_t raw; 
	
	struct {
		__uint16_t PAD_0			:1;
		__uint16_t CLEN				:4;
				__uint16_t ME		:1;
				__uint16_t PAD_0_2	:1;
				__uint16_t C_NR		:1;
		__uint16_t PAD_1			:2;
		__uint16_t SM				:2;
		__uint16_t DM				:2;
		__uint16_t TEN				:1;
		__uint16_t REN				:1;
			
	}f;
}_smcmr_gci_t;

typedef union{ 
	_smcmr_uart_t 		uart;
	_smcmr_transp_t 	transp;	
	_smcmr_gci_t  		gci;
}smcmr_t;




#endif //CM_SMC_H

