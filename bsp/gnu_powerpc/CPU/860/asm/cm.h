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
	#if TK_DCPU==860
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


#ifdef NEVER
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

#endif

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
#ifdef NEVER
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

#endif

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




#ifdef NEVER
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
#endif
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
#endif //CM_H

