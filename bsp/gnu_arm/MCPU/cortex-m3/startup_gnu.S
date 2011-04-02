/***************************************************************************
 *   Copyright (C) 2011 by Michael Ambrus                                  *
 *   ambrmi09@gmail.com                                                    *
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
# *** Startup Code (executed after Reset) ***
	.code 16

# Starupt Code must be linked first at Address at which it expects to run.

	.text
	.thumb

/*Probably does nothing*/
	.align  0
	.global _startup
	.code 16
	.func   _startup
_startup:
  	bl		clock_setup
	bl		bss_clear
	bl		statics_init

	.thumb_func
	.type   board_main, %function
/*Call secondary boot*/
	ldr		r7, =board_main
	blx		r7
/*Should never return, but if we do, lets go somewhere safe*/
	bl		loop_forever

	.size   _startup, . - _startup
	.endfunc

clock_setup:
/*Empty placeholder for now (will be call to board-code)*/
	mov 	pc,lr

bss_clear:
/*Clear bss memory*/
	mov 	pc,lr

statics_init:
/*Copy from ROM to RAM, initializating statics*/
	mov 	pc,lr

loop_forever:
/*Stay here until WD grabs us*/
	bl		loop_forever


	.end
















# Initialise Interrupt System
#  ...


# Setup Stack for each mode

#				LDR	 R0, =Top_Stack

##  Enter Undefined Instruction Mode and set its Stack Pointer
#				MSR	 CPSR_c, #Mode_UND|I_Bit|F_Bit
#				MOV	 SP, R0
#				SUB	 R0, R0, #UND_Stack_Size

##  Enter Abort Mode and set its Stack Pointer
#				MSR	 CPSR_c, #Mode_ABT|I_Bit|F_Bit
#				MOV	 SP, R0
#				SUB	 R0, R0, #ABT_Stack_Size

##  Enter FIQ Mode and set its Stack Pointer
#				MSR	 CPSR_c, #Mode_FIQ|I_Bit|F_Bit
#				MOV	 SP, R0
#				SUB	 R0, R0, #FIQ_Stack_Size

##  Enter IRQ Mode and set its Stack Pointer
#				MSR	 CPSR_c, #Mode_IRQ|I_Bit|F_Bit
#				MOV	 SP, R0
#				SUB	 R0, R0, #IRQ_Stack_Size

##  Enter Supervisor Mode and set its Stack Pointer
#				MSR	 CPSR_c, #Mode_SVC|I_Bit|F_Bit
#				MOV	 SP, R0
#				SUB	 R0, R0, #SVC_Stack_Size

##  Enter User Mode and set its Stack Pointer
#				MSR	 CPSR_c, #Mode_USR
#				MOV	 SP, R0

##  Setup a default Stack Limit (when compiled with "-mapcs-stack-check")
#				SUB	 SL, SP, #USR_Stack_Size


## Relocate .data section (Copy from ROM to RAM)
#				LDR	 R1, =_etext
#				LDR	 R2, =_data
#				LDR	 R3, =_edata
#LoopRel:		CMP	 R2, R3
#				LDRLO   R0, [R1], #4
#				STRLO   R0, [R2], #4
#				BLO	 LoopRel


## Clear .bss section (Zero init)
#				MOV	 R0, #0
#				LDR	 R1, =__bss_start__
#				LDR	 R2, =__bss_end__
#LoopZI:		 CMP	 R1, R2
#				STRLO   R0, [R1], #4
#				BLO	 LoopZI


# Enter the C code
#				B	   _start

#				ADR	 LR, __main_exit
#				LDR	 R0, =main
#				BX	  R0
#__main_exit:	B	   __main_exit
