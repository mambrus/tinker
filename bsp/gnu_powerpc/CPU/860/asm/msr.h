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

#ifndef MSR_H
#define MSR_H

#include <sys/types.h>

#ifdef NEVER
0–12   —  Reserved
 13  POW  Power management enable
          0 Power management disabled (normal operation mode)
          1 Power management enabled (reduced power mode)
          Note: Power management functions are implementation-dependent; see Section 14.5, “Power
          Control (Low-Power Modes).”
 14    —  Reserved
 15   ILE Exception little-endian mode. When an exception occurs, this bit is copied into MSR[LE] to select the
          endian mode for the context established by the exception.
     EE 1
 16       External interrupt enable
          0 The processor delays recognition of external and decrementer interrupt conditions.
          1 The processor is enabled to take an external or decrementer interrupt.
     PR1
 17       Privilege level
          0 The processor can execute both user- and supervisor-level instructions.
          1 The processor can only execute user-level instructions.
     FP1
 18       Floating-point available.
          0 The processor prevents dispatch of ﬂoating-point instructions, including ﬂoating-point loads,
             stores, and moves.
          1 The processor can execute ﬂoating-point instructions. (This setting is invalid on the MPC860)
            ME1
    19              Machine check enable
                    0 Machine check exceptions are disabled.
                    1 Machine check exceptions are enabled.
    20       —      Reserved
            SE1
    21              Single-step trace enable (Optional)
                    0 The processor executes instructions normally.
                    1 A single-step trace exception is generated when the next instruction executes successfully.
                    Note: If the function is not implemented, SE is treated as reserved.
            BE1
    22              Branch trace enable (Optional)
                    0 The processor executes branch instructions normally.
                    1 The processor generates a branch trace exception after completing the execution of a branch
                        instruction, regardless of whether the branch was taken.
                    Note: If the function is not implemented, this bit is treated as reserved.
  23–24      —      Reserved
    25       IP     Exception preﬁx. The setting of IP speciﬁes whether an exception vector offset is prepended with Fs
                    or 0s. In the following description, nnnnn is the offset of the exception vector. See Table 6-1.
                    0 Exceptions are vectored to the physical address 0x000n_nnnn
                    1 Exceptions are vectored to the physical address 0xFFFn_nnnn
                    The reset value of IP is determined by the IIP bit (bit 2) in the hard reset conﬁguration word. See
                    Section 11.3.1.1, “Hard Reset Conﬁguration Word.” Subsequent soft resets cause IP to revert to the
                    value latched during hard reset conﬁguration.
            IR1
    26              Instruction address translation
                    0 Instruction address translation is disabled.
                    1 Instruction address translation is enabled.
                    For more information, see Chapter 8, “Memory Management Unit.”
            DR1
    27              Data address translation
                    0 Data address translation is disabled.
                    1 Data address translation is enabled.
                    For more information, see Chapter 8, “Memory Management Unit.”
  28–29      —      Reserved
            RI1
    30              Recoverable exception (for system reset and machine check exceptions).
                    0 Exception is not recoverable.
                    1 Exception is recoverable.
                    For more information, see Chapter 6, “Exceptions.”
            L E1
    31              Little-endian mode enable
                    0 The processor runs in big-endian mode.
                    1 The processor runs in little-endian mode.

1   These bits are loaded into SRR1 when an exception is taken. These bits are written back into the MSR when an rﬁ
   is executed.

#endif


/* Support for reading and writing the MSR  (Machine State Register)	     */
/* ------------------------------------------------------------------------- */
#define SET_MSR( INc ) 				\
	__asm__ __volatile__ ("mtmsr %[INa]"		\
	:						\
	: [INa] "r" (INc)				\
	: "memory" );

#define GET_MSR(  OUTc ) 				\
	__asm__ __volatile__ ("mfmsr %[OUTa]"		\
	: [OUTa] "=r" (OUTc)				\
	: 						\
	: "memory" );


#define GDB_KNOWS_OF_UNSIGNED

#if defined(GDB_KNOWS_OF_UNSIGNED)
	#define UINT32 __uint32_t
#else
	#define UINT32 __int32_t
#endif

typedef union{ 
	UINT32 raw; 
	
	struct {
		UINT32 RSRVD_1:13;
		UINT32 POW:1;
		UINT32 RSRVD_2:1;
		UINT32 ILE:1;
		UINT32 EE:1;
		UINT32 PR:1;
		UINT32 FP:1;
		UINT32 ME:1;
		UINT32 RSRVD_3:1;
		UINT32 SE:1;
		UINT32 BE:1;
		UINT32 RSRVD_4:2;
		UINT32 IP:1;
		UINT32 IR:1;
		UINT32 DR:1;
		UINT32 RSRVD_5:2;
		UINT32 RI:1;
		UINT32 LE:1;
	
	}f;
}msr_t;

#endif //MSR_H

