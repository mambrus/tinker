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

#ifndef CM_H
#define CM_H

//Stuff for the communications module

#include <sys/types.h>
#include <tinker/config.h>

#if defined(TK_DCPU)
	#if TK_DCPU==__tk_860__
		#include <CPU/860/mmap_regs.h>
	#else
		#Sorry, TinKer doesn't know how to use your CPU
	#endif
#else
	#error Must have CPU for board to work
#endif


#define DP_ERAM 3

#if (DP_ERAM==3)
	/*Our definitions of the DP memory map (assuming DP is configured for worst case)*/
	#define DP_BD_0	 (DMM1+0x2800)	/* 0x2800 - 0x2E00	DP_BD_0		Descriptors/Data */
	#define DP_BD_1	 (DMM1+0x3800)	/* 0x3800 - 0x3A00	DP_BD_1		Descriptors/Data */
	#define DP_PARA	 (DMM1+0x3C00)	/* 0x3C00 - 0x3E00	DP_PARA		Parameter area   */

	/*Same thing, but as offset counting from start of DP RAM*/
	#define DP_BD_0_OFFS	 (0x2800-0x2000)
	#define DP_BD_1_OFFS	 (0x3800-0x2000)
	#define DP_PARA_OFFS	 (0x3C00-0x2000)
#elif (DP_ERAM==0)

	//#ifdef NEVER
	#define DP_BD_0	 (DMM1+0x2000)	/* 0x2800 - 0x2E00	DP_BD_0		Descriptors/Data */
	#define DP_PARA	 (DMM1+0x3C00)	/* 0x3C00 - 0x3E00	DP_PARA		Parameter area   */

	/*Same thing, but as offset counting from start of DP RAM*/
	#define DP_BD_0_OFFS	 (0x2000-0x2000)
	#define DP_PARA_OFFS	 (0x3C00-0x2000)
#endif


/*
  0   RST    CP reset command. Set by the core and cleared by the CP. Executing this command clears RST
             and FLG within two general system clocks. The CPM reset routine takes approximately 60 clocks,
             but CPM initialization can start immediately after this command is issued. Use RST to reset the
             registers and parameters for all the channels (SCCs, SMCs, SPI, I2C, and PIP) as well as the
             CPM and RISC timer table. RST does not, however, affect the serial interface or parallel I/O
             registers.
             0 No reset issued.
             1 Reset issued.
 1–3  —      Reserved. Should be cleared.
 4–7  OPCODE Operation code for the core-issued CP commands. See Table 18-7.
8–11  CH_NUM Channel number. Deﬁnes the speciﬁc sub-block on which the command is to operate. Some
             sub-blocks share channel number encodings if their commands are mutually exclusive.
             0000 SCC1
             0001 I2C/IDMA1
             001x Reserved
             0100 SCC2
             0101 SPI/IDMA2/RISC timers
             011x Reserved
             1000 SCC3
             1001 SMC1
             101x Reserved
             1100 SCC4
             1101 SMC2/PIP
             111x Reserved
12–14        Reserved. Should be cleared.
  15  FLG    Command semaphore ﬂag. Set by the core and cleared by the CP.
             0 CP is ready for a new command.
             1 CP is currently processing a command—cleared when the command is done or after reset.


==================================================================================

                         SMC (UART/
                                                                        I2C
 Opcode        SCC                    SMC (GCI)          SPI                       IDMA      Timer
                         Transparent)
0000                                                                            —          —
        INIT RX AND TX INIT RX AND TX INIT RX AND INIT RX AND TX INIT RX AND TX
        PARAMS         PARAMS         TX PARAMS   PARAMS         PARAMS
0001                                  —                                         —          —
        INIT RX PARAMS INIT RX PARAMS             INIT RX PARAMS INIT RX PARAMS
0010                                  —                                         —          —
        INIT TX PARAMS INIT TX PARAMS             INIT TX PARAMS INIT TX PARAMS
0011                                  —           —              —              —          —
        ENTER HUNT     ENTER HUNT
        MODE           MODE
  0100                                      —            —               —           —         —
              STOP TX        STOP TX
  0101                         —            —            —               —                     —
         GRACEFUL STOP                                                           INIT IDMA
                TX
  0110                                      —            —               —           —         —
           RESTART TX     RESTART TX
  0111                                      —                                        —         —
           CLOSE RX BD    CLOSE RX BD               CLOSE RX BD    CLOSE RX BD
  1000                         —            —            —               —           —
            SET GROUP                                                                      SET TIMER
             ADDRESS
  1001          —              —                         —               —           —         —
                                           GCI
                                        TIMEOUT
  1010                         —                         —               —           —         —
            RESET BCS                  GCI ABORT
                                        REQUEST
  1011          —              —            —            —               —                     —
                                                                                STOP IDMA
  1100          —              —            —            —               —           —         —
  1101          —              —            —            —               —           —         —
  111X           U              U           U             U              U           U         U



               Initialize transmit and receive parameters. Initializes the CP’s temporary Tx and Rx parameters in
INIT TX AND RX
               the parameter RAM to the user-deﬁned reset values—often required when switching protocols.
PARAMS
               Initialize receive parameters. Initializes the CP’s temporary Rx parameters in the parameter RAM to
INIT RX PARAMS
               the user-deﬁned reset values—often required when switching protocols.
               Initialize transmit parameters. Initializes the CP’s temporary Tx parameters in the parameter RAM to
INIT TX PARAMS
               the user-deﬁned reset values—often required when switching protocols.
               Causes the receiver to stop and wait for a new frame—exact operation depends on the protocol
ENTER HUNT
               used.
MODE
               Stop transmission. Stops the transmitting channel as soon as the Tx FIFO has been emptied. It
STOP TX
               should only be used when transmission needs to be stopped as quickly as possible. Transmission
               continues when RESTART TX is issued.
               Graceful stop transmission. Stops the transmitting channel after the whole current frame has been
GRACEFUL STOP
               sent. Transmission continues when RESTART TX is issued and the ready bit is set in the next TxBD.
TX
               Restart transmission. After STOP TX or GRACEFUL STOP TX, RESTART TX starts the transmitter, which
RESTART TX
               begins polling the R bit of the current BD.
               Closes the current RxBD in mid-reception; reception continues using the next available BD. Use
CLOSE RX BD
               CLOSE RX BD to access the data buffer without waiting for the SCC to ﬁnish ﬁlling it.

            Initialize IDMA transfers. Initializes the IDMA internal CP state to the user-deﬁned reset value.
INIT IDMA
            Stop IDMA transfers. The CP terminates current IDMA transfers.
STOP IDMA
            Used to activate, deactivate, or reconﬁgure the 16 timers of the RISC timer table.
SET TIMER
            Sets a hash table bit for the Ethernet logical group address recognition function.
SET GROUP
ADDRESS
            GCI receiver sends an abort request.
GCI ABORT
REQUEST
            Performs the GCI timeout function.
GCI TIMEOUT
            Used in BISYNC mode to reset the block check sequence calculation.
RESET BCS
            Undeﬁned. Reserved for use by Motorola-supplied RAM microcode packages.
U

*/

/*
0x0	0000	INIT_RX_AND_TX_PARAMS
0x1	0001	INIT_RX_PARAMS
0x2	0010	INIT_TX_PARAMS
0x3	0011	ENTER_HUNT
0x4	0100	STOP_TX
0x5	0101	GRACEFUL_STOP_TX	INIT_IDMA
0x6	0110	RESTART_TX
0x7	0111	CLOSE_RX_BD
0x8	1000	SET_GROUP_ADDRESS	SET_TIMER
0x9	1001	GCI_TIMEOUT
0xA	1010	RESET_BCS		GCI_ABORT_REQUEST
0xB	1011	STOP_IDMA
*/

/*OP codes (i.e. commands)*/

#define INIT_RX_AND_TX_PARAMS	0x0
#define INIT_RX_PARAMS		0x1
#define INIT_TX_PARAMS		0x2
#define ENTER_HUNT		0x3
#define STOP_TX			0x4
#define GRACEFUL_STOP_TX	0x5
#define INIT_IDMA		0x5
#define RESTART_TX		0x6
#define CLOSE_RX_BD		0x7
#define SET_GROUP_ADDRESS	0x8
#define SET_TIMER		0x8
#define GCI_TIMEOUT		0x9
#define RESET_BCS		0xA
#define GCI_ABORT_REQUEST	0xA
#define STOP_IDMA		0xB

typedef union{
	__uint16_t raw;

	struct {
		__uint32_t RST:1;
		__uint32_t PADD_0:3;
		__uint32_t OPCODE:4;
		__uint32_t CH_NUM:4;
		__uint32_t PADD_1:3;
		__uint32_t FLG:1;
	}f;
}cpcr_t;

/* RCCR stuff... (RISC Controller Conﬁguration Register (RCCR))*/
/*
   0   TIME Timer enable. Controls whether the CP’s internal timer sends a tick to the CP based on the value
            programmed in the timer period (TIMEP).
            0 Stop RISC timer table scanning.
            1 Start RISC timer table scanning.
 1  —       Reserved. Should be cleared.
 2–7  TIMEP Timer period. Controls the period of the CP’s internal timer tick. The RISC timer table are scanned
            on each timer tick. The input to the timer tick generator is the system clock divided by 1,024. The
            formula is: timer tick period = (TIMEP + 1) × 1,024 system clocks. Thus, a value of 0 stored in this
            ﬁeld creates a timer tick every 1 × (1,024) = 1,024 system clocks; a value of 63 causes a tick every
            64 × (1,024) = 65,536 system clocks.
  8   DR1M IDMA request 1 mode. Controls the IDMA request 1 (DREQ1) sensitivity mode. See Section 19.3.7,
            “IDMA Interface Signals—DREQ and SDACK.”
            0 DREQ1 is edge-sensitive.
            1 DREQ1 is level-sensitive.
  9   DR0M IDMA request 0 mode. Controls the IDMA request 0 (DREQ0) sensitivity mode. See Section 19.3.7,
            “IDMA Interface Signals—DREQ and SDACK.”
            0 DREQ0 is edge-sensitive.
            1 DREQ0 is level-sensitive.
10–11 DRQP IDMA emulation request priority. Controls the priority of the external request signals that relate to the
            serial channels. See Section 18.3, “Communicating with the Peripherals.”
            00 IDMA requests have priority over the SCCs.
            01 IDMA requests have priority immediately following the SCCs (option 2).
            10 IDMA requests have the lowest priority (option 3).
            11 Reserved.
 12    EIE  External interrupt enable. Conﬁgure as instructed in the download process of a Motorola-supplied
            RAM microcode package.
            0 DREQ0 cannot interrupt the CP.
            1 DREQ0 will interrupt the CP.
 13    SCD  Scheduler conﬁguration. Conﬁgure as instructed in the download process of a Motorola-supplied
            RAM microcode package.
            0 Normal operation.
            1 Alternate conﬁguration of the scheduler.
14–15 ERAM Enable RAM microcode Conﬁgure as instructed in the download process of a Motorola-supplied
            microcode package. See Section 18.6.1, “System RAM and Microcode Packages.”
            00 Disable microcode program execution in the dual-port system RAM.
            01 Microcode executes from the ﬁrst 512 bytes and a 256-byte extension of dual-port system RAM.
            10 Microcode executes from the ﬁrst 1 Kbyte and a 256-byte extension of dual-port system RAM.
            11 Microcode executes from the ﬁrst 2 Kbytes and a 512-byte extension of dual-port system RAM.

*/

typedef union{
	__uint16_t raw;
	struct {
		__uint16_t TIME:1;
		__uint16_t PADD_0:1;
		__uint16_t TIMEP:6;
		__uint16_t DR1M:1;
		__uint16_t DR0M:1;
		__uint16_t DRQP:2;
		__uint16_t EIE:1;
		__uint16_t SCD:1;
		__uint16_t ERAM:2;
	}f;
}rccr_t;


/*Parameter memory mapping (predefined)*/
/*
0x3C00 1 0x1C00—0x1C7F SCC1
         0x1C80—0x1CAF I2C
         0x1CB0—0x1CBF Miscellaneous
         0x1CC0—0x1CFF IDMA1
0x3D00 2 0x1D00—0x1D7F SCC2
         0x1D80—0x1DAF SPI
         0x1DB0—0x1DBF RISC timer table
         0x1DC0—0x1DFF IDMA2
0x3E00 3 0x1E00—0x1E7F SCC3
         0x1E80—0x1EBF SMC1
         0x1EC0—0x1EFF Reserved
0x3F00 4 0x1F00—0x1F7F SCC4
         0x1F80—0x1FBF SMC2/PIP
         0x1FC0—0x1FFF Reserved
*/

#define DP_BASE (DMM1+0x2000)

#define DP_PARA_SCC1			(DP_BASE+0x1C00)
#define DP_PARA_I2C			(DP_BASE+0x1C80)
#define DP_PARA_Miscellaneous		(DP_BASE+0x1CB0)
#define DP_PARA_IDMA1			(DP_BASE+0x1CC0)
#define DP_PARA_SCC2			(DP_BASE+0x1D00)
#define DP_PARA_SPI			(DP_BASE+0x1D80)
#define DP_PARA_RISC_timer_table	(DP_BASE+0x1DB0)
#define DP_PARA_IDMA2			(DP_BASE+0x1DC0)
#define DP_PARA_SCC3			(DP_BASE+0x1E00)
#define DP_PARA_SMC1			(DP_BASE+0x1E80)
#define DP_PARA_Reserved_0		(DP_BASE+0x1EC0)
#define DP_PARA_SCC4			(DP_BASE+0x1F00)
#define DP_PARA_SMC2_PIP		(DP_BASE+0x1F80)
#define DP_PARA_Reserved_1		(DP_BASE+0x1FC0)


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


/*Generic BD destriptor. Don't use this - use a specific instead*/
typedef struct{
	__uint16_t BD_Status;
	__uint16_t BD_Length;
	__uint32_t BD_Pointer;
}_bd_t;




/*
                         Table 20-13. BRGCn Field Descriptions
 Bits Name                                              Description
0–13    —  Reserved, should be cleared.
  14   RST Reset BRG. Performs a software reset of the BRG identical to that of an external reset. A reset
           disables the BRG and drives BRGO high. This is externally visible only if BRGO is connected to the
           corresponding parallel I/O pin.
           0 Enable the BRG.
           1 Reset the BRG (software reset).
  15   EN  Enable BRG count. Used to dynamically stop the BRG from counting—useful for low-power modes.
           0 Stop all clocks to the BRG.
           1 Enable clocks to the BRG.
16–17 EXTC External clock source. Selects the BRG input clock.
           00 BRGCLK (internal clock generated by the clock synthesizer in the SIU).
           01 CLK2
           10 CLK6
           11 Reserved.
  18   ATB Autobaud. Selects autobaud operation for BRGn on the corresponding RXDn. ATB must remain zero
           until the SCC receives the three Rx clocks. Then the user must set ATB to obtain the correct baud
           rate. After the baud rate is obtained and locked, it is indicated by setting AB in the UART event
           register. See Section 20.4.2, “Autobaud Operation on the SCC UART.”
           0 Normal operation of the BRG.
           1 When RXD goes low, the BRG determines the length of the start bit and synchronizes the BRG
              to the actual baud rate.

19–30  CD   Clock divider. CD presets an internal 12-bit counter that is decremented at the DIV16 output rate.
            When the counter reaches zero, it is reloaded with CD. CD = 0xFFF produces the minimum clock
            rate for BGRO (divide by 4,096); CD = 0x000 produces the maximum rate (divide by 1). When
            dividing by an odd number, the counter ensures a 50% duty-cycle by asserting the terminal count
            once on clock low and next on clock high. The terminal count signals counter expiration and toggles
            the clock. See Section 20.4.3, “UART Baud Rate Examples.”
 31   DIV16  Divide-by-16. Selects a divide-by-1 or divide-by-16 prescaler before reaching the clock divider. See
            Section 20.4.3, “UART Baud Rate Examples.”
            0 Divide by 1.
            1 Divide by 16.
*/
typedef union{
	__uint32_t raw;
	struct {
		__uint32_t PADD_0		:14;
		__uint32_t RST			:1;
		__uint32_t EN			:1;
		__uint32_t EXTC			:2;
		__uint32_t ATB			:1;
		__uint32_t CD			:12;
		__uint32_t DIV16		:1;
	}f;
}brgc_t;

//=============================================================================
// CPIC registers follow (registers for controlling interrupts btw SIU and CM)
// Even though these registers are not strict CPM registers they relate
// only to CM which is the reason why they are defined in this header file.
//=============================================================================
/*
34.5 CPIC Registers
There are four CPIC registers:
   •  CPM interrupt conﬁguration register (CICR)—Deﬁnes CPM interrupt attributes.
   •  CPM interrupt pending register (CIPR)—Indicates which CPM interrupt sources
      require interrupt service.
   • CPM interrupt mask register (CIMR)—Can be used to mask CPM interrupt sources.
   4. CPM interrupt in-service register (CISR)—Allows nesting interrupt requests within
      the CPM interrupt level.
*/

//CICR --

/*
34.5.1 CPM Interrupt Conﬁguration Register (CICR)
The CPM interrupt conﬁguration register (CICR) deﬁnes CPM interrupt request levels, the
priority between the SCCs, and the highest priority interrupt.
          0   1     2    3     4   5    6    7      8     9   10    11   12   13 14   15
   Field                    —                        SCdP       SCcP      SCbP    SCaP
  Reset                               0000_0000_0000_0000
   R/W                                         R/W
   Addr                                       0x940
         16   17   18    19   20  21   22   23     24    25   26    27   28   29 30   31
   Field     IRL                 HP               IEN                  —             SPS
  Reset                               0000_0000_0000_0000
   R/W                                         R/W
   Addr                                       0x942
              Figure 34-3. CPM Interrupt Conﬁguration Register (CICR)
This register is affected by HRESET but is not affected by SRESET. CICR bits are
described in Table 34-3.


                                    Table 34-3. CICR Field Descriptions
   Bits   Name                                                    Description
  0–7       —       Reserved, should be cleared.
          SCdP 1 SCCd priority order. Deﬁnes which SCCs asserts its request in the SCCd priority position.
   8–9
                   00 SCC1 asserts its request in the SCCd position.
                   01 SCC2 asserts its request in the SCCd position.
                   10 SCC3 asserts its request in the SCCd position.
                   11 SCC4 asserts its request in the SCCd position.
          SCcP1 SCCc priority order. Deﬁnes which SCCs asserts its request in the SCCc priority position.
  10–11
                   00 SCC1 asserts its request in the SCCc position.
                   01 SCC2 asserts its request in the SCCc position.
                   10 SCC3 asserts its request in the SCCc position.
                   11 SCC4 asserts its request in the SCCc position.
          SCbP1 SCCb priority order. Deﬁnes which SCCs that asserts its request in the SCCb priority position.
  12–13
                   00 SCC1 asserts its request in the SCCb position.
                   01 SCC2 asserts its request in the SCCb position.
                   10 SCC3 asserts its request in the SCCb position.
                   11 SCC4 asserts its request in the SCCb position.
          SCaP1 SCCa priority order. Deﬁnes which SCCs that asserts its request in the SCCa priority position.
  14–15
                   00 SCC1 asserts its request in the SCCa position.
                   01 SCC2 asserts its request in the SCCa position.
                   10 SCC3 asserts its request in the SCCa position.
                   11 SCC4 asserts its request in the SCCa position.
  16–18    IRL     Interrupt request level. Contains the priority request level of the interrupt from the CPM that is sent
                   to the SIU. Level 0 indicates highest priority. IRL is initialized to zero during reset. In most systems,
                   value 0b100 is a good value to choose for IRL.
  19–23     HP     Highest priority. Speciﬁes the 5-bit interrupt number of the CPIC interrupt source that is advanced to
                   the highest priority in the table. These bits can be modiﬁed dynamically. (Programming HP =
                   0b11111 keeps PC15 the highest priority source for external interrupts to the core.)
    24     IEN     Interrupt enable. Master enable for CPM interrupts.
                   0 CPM interrupts are disabled
                   1 CPM interrupts are enabled
  25–30     —      Reserved
    31     SPS     Spread priority scheme. Selects the relative priority scheme; cannot be changed dynamically.
                   0 Grouped. The SCCs are grouped by priority at the top of the table.
                   1 Spread. The SCCs are spread by priority in the table.
1  Note: Do not program the same SCC to more than one priority position (a, b, c, or d). These bits can be changed
   dynamically. Also, the bit pattern 11 should be used for the combination that is not implemented.

*/

typedef union{
	__uint32_t raw;
	struct {
		__uint32_t PADD_0:8;
		__uint32_t SCdP:2;
		__uint32_t SCcP:2;
		__uint32_t SCbP:2;
		__uint32_t SCaP:2;
		__uint32_t IRL:3;
		__uint32_t HP:5;
		__uint32_t IEN:1;
		__uint32_t PADD_1:6;
		__uint32_t SPS:1;
	}f;
}cicr_t;



#define CPMIV_Parallel_I_O_PC15 	0x1F
#define CPMIV_SCC1 			0x1E
#define CPMIV_SCC2 			0x1D
#define CPMIV_SCC3 			0x1C
#define CPMIV_SCC4 			0x1B
#define CPMIV_Parallel_I_O_PC14 	0x1A
#define CPMIV_Timer_1 			0x19
#define CPMIV_Parallel_I_O_PC13 	0x18
#define CPMIV_Parallel_I_O_PC12 	0x17
#define CPMIV_SDMA_channel_bus_error 	0x16
#define CPMIV_IDMA1 			0x15
#define CPMIV_IDMA2 			0x14
//0x13       Reserved
#define CPMIV_Timer_2 			0x12
#define CPMIV_RISC_timer_table 		0x11
#define CPMIV_I2C 			0x10
#define CPMIV_Parallel_I_O_PC11		0x0F
#define CPMIV_Parallel_I_O_PC10		0x0E
//0x0D     Reserved
#define CPMIV_Timer_3			0x0c
#define CPMIV_Parallel_I_O_PC9 		0x0B
#define CPMIV_Parallel_I_O_PC8	 	0x0A
#define CPMIV_Parallel_I_O_PC7		0x09
//0x08     Reserved
#define CPMIV_Timer_4			0x07
#define CPMIV_Parallel_I_O_PC6		0x06
#define CPMIV_SPI			0x05
#define CPMIV_SMC1			0x04
#define CPMIV_SMC2_PIP			0x03
#define CPMIV_Parallel_I_O_PC5		0x02
#define CPMIV_Parallel_I_O_PC4		0x01
#define CPMIV_Error			0x00

typedef enum{
	cme_Error,
	cmid_Parallel_I_O_PC4,
	cmid_Parallel_I_O_PC5,
	cmid_SMC2_PIP,
	cmid_SMC1,
	cmid_SPI,

	cmid_Parallel_I_O_PC6,
	cmid_Timer_4,
	Reserved_0,

	cmid_Parallel_I_O_PC7,
	cmid_Parallel_I_O_PC8,
	cmid_Parallel_I_O_PC9,
	cmid_Timer_3,
	Reserved_1,

	cmid_Parallel_I_O_PC10,
	cmid_Parallel_I_O_PC11,

	cmid_I2C,
	cmid_RISC_timer_table,
	cmid_Timer_2,
	Reserved_2,
	cmid_IDMA2,
	cmid_IDMA1,
	cmid_SDMA_channel_bus_error,

	cmid_Parallel_I_O_PC12,
	cmid_Parallel_I_O_PC13,

	cmid_Timer_1,


	cmid_Parallel_I_O_PC14,
	cmid_SCC4,
	cmid_SCC3,
	cmid_SCC2,
	cmid_SCC1,

	cmid_Parallel_I_O_PC15
}cpmid_t;


/*
34.5.5 CPM Interrupt Vector Register (CIVR)
The CPM interrupt vector register (CIVR) is used to identify an interrupt source. The core
uses the IACK bit to acknowledge an interrupt. CIVR can be read at any time. This register
is affected by HRESET and SRESET.

0–4   VN  Vector number. Identiﬁes the interrupt source. These values are listed in Table 34-2.
5–14   —  Reserved. Writing to bits 5-15 has no effect because they are always read as zeros.
 15  IACK Interrupt acknowledge. When the core sets IACK, CIVR[VN] is updated with a 5-bit vector
          corresponding to the sub-block with the highest current priority. IACK is cleared after one clock cycle.
*/


typedef union{
	__uint16_t raw;
	struct {
		__uint16_t VN:5;
		__uint16_t PADD_0:10;
		__uint16_t IACK:1;
	}f;
}civr_t;

/*
        0     1    2     3     4    5      6     7      8      9    10    11  12      13   14  15
 Field PC15 SCC1 SCC2 SCC3 SCC4 PC14 TIMER1 PC13      PC12  SDMA IDMA1 IDMA2  —    TIMER2 RTT I2C
Reset                                      0000_0000_0000_0000
 R/W                                               R/W
 Addr                            0x944 (CIPR), 0x948 (CIMR), 0x94C (CISR)
        16   17   18    19    20   21     22    23     24     25    26    27  28      29   30  31
 Field PC11 PC10  —   TIMER3 PC9  PC8    PC7    —   TIMER4 PC6     SPI  SMC1 SMC2/   PC5  PC4  —
                                                                              PIP
Reset                                      0000_0000_0000_0000
 R/W                                               R/W
 Addr                            0x946 (CIPR), 0x94A (CIMR), 0x94E (CISR)
Figure 34-4. CPM Interrupt Pending/Mask/In-Service Registers (CIPR/CIMR/CISR)

*/


typedef union{
	__uint32_t raw;
	struct {
		__uint32_t PC15:1;
		__uint32_t SCC1:1;
		__uint32_t SCC2:1;
		__uint32_t SCC3:1;
		__uint32_t SCC4:1;
		__uint32_t PC14:1;
		__uint32_t TIMER1:1;
		__uint32_t PC13:1;

		__uint32_t PC12:1;
		__uint32_t SDMA:1;
		__uint32_t IDMA1:1;
		__uint32_t IDMA2:1;
		__uint32_t _rsv_0:1;
		__uint32_t TIMER2:1;
		__uint32_t RTT:1;
		__uint32_t I2C:1;

		__uint32_t PC11:1;
		__uint32_t PC10:1;
		__uint32_t _rsv_1:1;
		__uint32_t TIMER3:1;
		__uint32_t PC9:1;
		__uint32_t PC8:1;
		__uint32_t PC7:1;
		__uint32_t _rsv_2:1;

		__uint32_t TIMER4:1;
		__uint32_t PC6:1;
		__uint32_t SPI:1;
		__uint32_t SMC1:1;
		__uint32_t SMC2_PIP:1;
		__uint32_t PC5:1;
		__uint32_t PC4:1;
		__uint32_t _rsv_3:1;

	}f;
}ciid_t;

#include <CPU/860/isr.h>
void CM_init();
void CM_Handler( void );
int  CM_isr_install(int level, isr_handler isr);



#endif //CM_H

