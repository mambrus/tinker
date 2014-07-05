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

/*
PLEASE NOTE

This code has not been thouroughly tested nor is complete.

*/

#ifndef CM_SCC_H
#define CM_SCC_H

//Stuff for the communications module (CM) - SCC specific

#include <sys/types.h>

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
/*
scc_rx_control_descriptor
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

scc_TX_control_descriptor
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

typedef union {
	__uint16_t raw;
	struct {
		__uint16_t E_Empty:1;
		__uint16_t Padd_0:1;
		__uint16_t W_Wrap:1;
		__uint16_t I_Interrupt:1;
		__uint16_t C_Control_character:1;
		__uint16_t A_Address:1;
		__uint16_t CM_Continuous_mode:1;
		__uint16_t ID_Buffer_closed_on_reception_of_idles:1;
		__uint16_t AM_Address_match:1;
		__uint16_t Padd_1:1;
		__uint16_t BR_Break_received:1;
		__uint16_t FR_Framing_error:1;
		__uint16_t PR_Parity_error:1;
		__uint16_t Padd_2:1;
		__uint16_t OV_Overrun:1;
		__uint16_t CD_Carrier_detect_lost:1;
	} f;
} bd_ctrl_scc_rx_t;

typedef union {
	__uint16_t raw;
	struct {
		__uint16_t R_Ready:1;
		__uint16_t Padd_0:1;
		__uint16_t W_Wrap:1;
		__uint16_t I_Interrupt:1;
		__uint16_t CR_Clear_to_send_report:1;
		__uint16_t A_Address:1;
		__uint16_t CM_Continuous_mode:1;
		__uint16_t P_Preamble:1;
		__uint16_t NS_No_stop_bit_sent:1;
		__uint16_t Padd_1:5;
		__uint16_t CT_CTS_lost:1;
	} f;
} bd_ctrl_scc_tx_t;

typedef struct {
	union {
		__uint16_t raw;
		bd_ctrl_scc_rx_t SCC_RX;	/*Note: same as for SMC */
		bd_ctrl_scc_tx_t SCC_TX;	/*Note: same as for SMC */
	} BD_Status;
	__uint16_t BD_Length;
	__uint32_t BD_Pointer;
} bd_scc_t;

/* Function code register (for RFCR and TFCR in scc_param_t)*/
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

typedef union {
	__uint8_t raw;
	struct {
		__uint8_t PADD_0:3;
		__uint8_t BO:2;
		__uint8_t AT:3;

	} f;
} scc_fcr_t;

/*Protocol specific areas*/
/*
SCC_UART_STUFF
===============================================================================
0x30    —    DWord Reserved
0x38 MAX_IDL Hword Maximum idle characters. When a character is received, the receiver begins
                   counting idle characters. If MAX_IDL idle characters are received before the next
                   data character, an idle timeout occurs and the buffer is closed, generating a
                   maskable interrupt request to the core to receive the data from the buffer. Thus,
                   MAX_IDL offers a way to demarcate frames. To disable the feature, clear
                   MAX_IDL. The bit length of an idle character is calculated as follows: 1 + data
                   length (5–9) + 1 (if parity is used) + number of stop bits (1–2). For 8 data bits, no
                   parity, and 1 stop bit, the character length is 10 bits.
0x3A   IDLC  Hword Temporary idle counter. Holds the current idle count for the idle timeout process.
                   IDLC is a down-counter and does not need to be initialized or accessed.
0x3C BRKCR   Hword Break count register (transmit). Determines the number of break characters the
                   transmitter sends. The transmitter sends a break character sequence when a
                   STOP TRANSMIT command is issued. For 8 data bits, no parity, 1 stop bit, and 1 start
                   bit, each break character consists of 10 zero bits.
                                                      16
0x3E  PAREC  Hword User-initialized,16-bit (modulo–2 ) counters incremented by the CP.
                   PAREC counts received parity errors.
0x40  FRMEC  Hword FRMEC counts received characters with framing errors.
             Hword NOSEC counts received characters with noise errors.
0x42 NOSEC
                   BRKEC counts break conditions on the signal. A break condition can last for
0x44  BRKEC  Hword hundreds of bit times, yet BRKEC is incremented only once during that period.
0x46  BRKLN  Hword Last received break length. Holds the length of the last received break character
                   sequence measured in character units. For example, if RXD (receive signal) is low
                   for 20 bit times and the deﬁned character length is 10 bits, BRKLN = 0x002,
                   indicating that the break sequence is at least 2 characters long. BRKLN is
                   accurate to within one character length.
0x48 UADDR1  Hword UART address character 1/2. In multidrop mode, the receiver provides automatic
                   address recognition for two addresses. In this case, program the lower order bytes
0x4A UADDR2  Hword of UADDR1 and UADDR2 with the two preferred addresses.
0x4C  RTEMP  Hword Temporary storage
0x4E TOSEQ      Hword Transmit out-of-sequence character. Inserts out-of-sequence characters, such as
                      XOFF and XON, into the transmit stream. The TOSEQ character is put in the Tx
                      FIFO without affecting a Tx buffer in progress. See Section 22.11, “Inserting
                      Control Characters into the Transmit Data Stream.”
0x50 CHARACTER1 Hword Control character 1–8. These characters deﬁne the Rx control characters on
                      which interrupts can be generated.
0x52 CHARACTER2 Hword
0x54 CHARACTER3 Hword
0x56 CHARACTER4 Hword
0x58 CHARACTER5 Hword
0x5A CHARACTER6 Hword
0x5C CHARACTER7 Hword
0x5E CHARACTER8 Hword
0x60 RCCM       Hword Receive control character mask. Used to mask comparison of CHARACTER1–8
                      so classes of control characters can be deﬁned. A one enables the comparison,
                      and a zero masks it.
0x62 RCCR       Hword Receive control character register. Used to hold the last rejected control character
                      (not written to the Rx buffer). Generates a maskable interrupt. If the core does not
                      process the interrupt and read RCCR before a new control character arrives, the
                      previous control character is overwritten.
0x64 RLBC       Hword Receive last break character. Used in synchronous UART when PSMR[RZS] = 1;
                      holds the last break character pattern. By counting zeros in RLBC, the core can
                      measure break length to a one-bit resolution. Read RLBC by counting the zeros
                      written from bit 0 to where the ﬁrst one was written. RLBC = 0b001xxxxxxxxxxxxx
                      indicates two zeros; 0b1xxxxxxxxxxxxxxx indicates no zeros.
                      Note that RLBC can be used in combination with BRKLN above to measure the
                      break length down to a bit resolution: (BRKLN + number of zeros in RLBC).
*/

typedef struct {
/*0x30 PADD_0*/ __uint32_t PADD_0;
/*0x34 PADD_1*/ __uint32_t PADD_1;
/*0x38 MAX_IDL*/ __uint16_t MAX_IDL;
/*0x3A  IDLC*/ __uint16_t IDLC;
/*0x3C BRKCR*/ __uint16_t BRKCR;
/*0x3E PAREC*/ __uint16_t PAREC;
/*0x40 FRMEC*/ __uint16_t FRMEC;
/*0x42 NOSEC*/ __uint16_t NOSEC;
/*0x44 BRKEC*/ __uint16_t BRKEC;
/*0x46 BRKLN*/ __uint16_t BRKLN;
/*0x48 UADDR1*/ __uint16_t UADDR1;
/*0x4A UADDR2*/ __uint16_t UADDR2;
/*0x4C RTEMP*/ __uint16_t RTEMP;
/*0x4E TOSEQ*/ __uint16_t TOSEQ;
/*0x50 CHARACTER1*/ __uint16_t CHARACTER1;
/*0x52 CHARACTER2*/ __uint16_t CHARACTER2;
/*0x54 CHARACTER3*/ __uint16_t CHARACTER3;
/*0x56 CHARACTER4*/ __uint16_t CHARACTER4;
/*0x58 CHARACTER5*/ __uint16_t CHARACTER5;
/*0x5A CHARACTER6*/ __uint16_t CHARACTER6;
/*0x5C CHARACTER7*/ __uint16_t CHARACTER7;
/*0x5E CHARACTER8*/ __uint16_t CHARACTER8;
/*0x60 RCCM*/ __uint16_t RCCM;
	/*0x62 RCCR */ __uint16_t _RCCR;
	/*Note: this is also the name of a SPR (hence the prefix) */
/*0x64 RLBC*/ __uint16_t RLBC;
} proto_scc_uart_t;

typedef struct {
/*0x00 RBASE*/ __uint16_t RBASE;
/*0x02 TBASE*/ __uint16_t TBASE;
	/*0x04 RFCR */ scc_fcr_t RFCR;
						/*__uint8_t*/
	/*0x05 TFCR */ scc_fcr_t TFCR;
						/*__uint8_t*/
/*0x06 MRBLR*/ __uint16_t MRBLR;
/*0x08 RSTATE*/ __uint32_t RSTATE;
/*0x0C   RIP*/ __uint32_t RIP;
/*0x10 RBPTR*/ __uint16_t RBPTR;
/*0x12 RCOUN_T*/ __uint16_t RCOUN_T;
/*0x14 RTEMP*/ __uint32_t RTEMP;
/*0x18 TSTATE*/ __uint32_t TSTATE;
/*0x1C   TIP*/ __uint32_t TIP;
/*0x20 TBPTR*/ __uint16_t TBPTR;
/*0x22 TCOUN*/ __uint16_t TCOUN;
/*0x24 TTEMP_T*/ __uint32_t TTEMP_T;
/*0x28  RCRC*/ __uint32_t RCRC;
/*0x2C  TCRC*/ __uint32_t TCRC;

/*0x30*/ union {
		proto_scc_uart_t SCC_UART;
	} PROTO;
} scc_param_t;

#endif				//CM_SCC_H
