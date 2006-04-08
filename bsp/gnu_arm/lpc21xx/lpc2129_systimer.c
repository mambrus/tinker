/***************************************************************************
 *   Copyright (C) 2006 by Michael Ambrus                                  *
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
 
 /*!
 @brief Low level GNU_ARM based systimer for LPC2129
 
 Low level GNU_ARM based systimer for LPC2129
 
 */

#include "lpc2129_vic.h"
#include "lpc21xx.h"

#define PRESC_VAL 0x11223344;
#define MATCH_VAL 0x44332211;

/*
#define TIMER0_IR       0xE0004000  //<! Intrerrupt Register
#define TIMER0_TCR      0xE0004004  //<! Timer Control Register (0=cnt enable, 1=reset)
#define TIMER0_TC       0xE0004008  //<! Timer Counter
#define TIMER0_PR       0xE000400C  //<! Prescale Register
#define TIMER0_PC       0xE0004010  //<! Prescale Counter
#define TIMER0_MCR      0xE0004014  //<! Match Control register
#define TIMER0_MR0      0xE0004018  //<!   Match register   0
#define TIMER0_MR1      0xE000401C  //<!      "             1
#define TIMER0_MR2      0xE0004020  //<!      "             2
#define TIMER0_MR3      0xE0004024  //<!      "             3
#define TIMER0_CCR      0xE0004028  //<! Captur Control register
#define TIMER0_CR0      0xE000402C  //<!   Capture Register 0
#define TIMER0_CR1      0xE0004030  //<!                    1
#define TIMER0_CR2      0xE0004034  //<!                    2
#define TIMER0_CR3      0xE0004038  //<!                    3
#define TIMER0_EMR      0xE000403C  //<! 
*/                                  

//Convert a bit position into a mask with the n'th bit set
#define BIT(n) ( 1 << n )

//Bits for the IR
#define IR_MR0 0   //!< Interrupt Interrupt flag for match channel 0. 0
#define IR_MR1 1   //!< Interrupt Interrupt flag for match channel 1. 0
#define IR_MR2 2   //!< Interrupt Interrupt flag for match channel 2. 0
#define IR_MR3 3   //!< Interrupt Interrupt flag for match channel 3. 0
#define IR_CR0 4   //!< Interrupt Interrupt flag for capture channel 0 event. 0
#define IR_CR1 5   //!< Interrupt Interrupt flag for capture channel 1 event. 0
#define IR_CR2 6   //!< Interrupt Interrupt flag for capture channel 2 event. 0
#define IR_CR3 7   //!< Interrupt Interrupt flag for capture channel 3 event. 0

//Bits for Match Control Register (MCR)
#define MCR0_INT 0  //!< Interrupt on MR0
#define MCR0_RES 1  //!< Reset on MR0 When one, the TC will be reset if MR0 matches it. When zero this feature is disabled.
#define MCR0_STP 2  //!< Stop on MR0
#define MCR1_INT 3  //!< Interrupt on MR1
#define MCR1_RES 4  //!< Reset on MR1 When one, the TC will be reset if MR1 matches it. When zero this feature is disabled.
#define MCR1_STP 5  //!< Stop on MR1
#define MCR2_INT 6  //!< Interrupt on MR2
#define MCR2_RES 7  //!< Reset on MR2 When one, the TC will be reset if MR2 matches it. When zero this feature is disabled.
#define MCR2_STP 8  //!< Stop on MR2
#define MCR3_INT 9  //!< Interrupt on MR3
#define MCR3_RES 10 //!< Reset on MR3 When one, the TC will be reset if MR3 matches it. When zero this feature is disabled.
#define MCR3_STP 11 //!< Stop on MR3

//Bits for Capture Control Register (CCR)
#define CCR0_RISE 0  //!< Capture on CAPn.0 rising edge When one, a sequence of 0 then 1 on CAPn.0 will cause CR0 to be loaded with the contents of the TC. When zero, this feature is disabled.
#define CCR0_FALL 1  //!< Capture on CAPn.0 falling edge When one, a sequence of 1 then 0 on CAPn.0 will cause CR0 to be loaded with the contents of TC. When zero, this feature is disabled.
#define CCR0_INT  2  //!< Interrupt on CAPn.0 event When one, a CR0 load due to a CAPn.0 event will generate an interrupt. When zero, this feature is disabled.
#define CCR1_RISE 3  //!< Capture on CAPn.1 rising edge When one, a sequence of 0 then 1 on CAPn.1 will cause CR1 to be loaded with the contents of the TC. When zero, this feature is disabled.
#define CCR1_FALL 4  //!< Capture on CAPn.1 falling edge When one, a sequence of 1 then 0 on CAPn.1 will cause CR1 to be loaded with the contents of TC. When zero, this feature is disabled.
#define CCR1_INT  5  //!< Interrupt on CAPn.1 event When one, a CR1 load due to a CAPn.1 event will generate an interrupt. When zero, this feature is disabled.
#define CCR2_RISE 6  //!< Capture on CAPn.2 rising edge When one, a sequence of 0 then 1 on CAPn.2 will cause CR2 to be loaded with the contents of the TC. When zero, this feature is disabled.
#define CCR2_FALL 7  //!< Capture on CAPn.2 falling edge When one, a sequence of 1 then 0 on CAPn.2 will cause CR2 to be loaded with the contents of TC. When zero, this feature is disabled.
#define CCR2_INT  8  //!< Interrupt on CAPn.2 event When one, a CR2 load due to a CAPn.2 event will generate an interrupt. When zero, this feature is disabled.
#define CCR3_RISE 9  //!< Capture on CAPn.3 rising edge When one, a sequence of 0 then 1 on CAPn.3 will cause CR3 to be loaded with the contents of TC. When zero, this feature is disabled.
#define CCR3_FALL 10 //!< Capture on CAPn.3 falling edge When one, a sequence of 1 then 0 on CAPn.3 will cause CR3 to be loaded with the contents of TC. When zero, this feature is disabled.
#define CCR3_INT  11 //!< Interrupt on CAPn.3 event When one, a CR3 load due to a CAPn.3 event will generate an interrupt. When zero, this feature is disabled.

//EMR bits

/*! @brief External Match 0
This bit reflects the state of output MAT0.0/MAT1.0, whether or not this output is
connected to its pin. When a match occurs for MR0, this output of the timer can either
toggle, go low, go high, or do nothing. Bits EMR[4:5] control the functionality of this
output.*/
#define EMR0 0

/*! @brief External Match 1
This bit reflects the state of output MAT0.1/MAT1.1, whether or not this output is
connected to its pin. When a match occurs for MR1, this output of the timer can either
toggle, go low, go high, or do nothing. Bits EMR[6:7] control the functionality of this
output.*/
#define EMR1 1

/*! @brief External Match 2
This bit reflects the state of output MAT0.2/MAT1.2, whether or not this output is
connected to its pin. When a match occurs for MR2, this output of the timer can either
toggle, go low, go high, or do nothing. Bits EMR[8:9] control the functionality of this
output.*/
#define EMR2 2

/*! @brief External Match 3
This bit reflects the state of output MAT0.3/MAT1.3, whether or not this output is
connected to its pin. When a match occurs for MR3, this output of the timer can either
toggle, go low, go high, or do nothing. Bits EMR[10:11] control the functionality of this
output.*/
#define EMR3 3

//Actions for each EMR not defined (TBD)




void systimer_init(void *vc)
{
   TIMER0_TCR  = 1;

   
   vic_install_isr((vic_control*)vc); 
   vic_enable_int((vic_control*)vc);   
   
   TIMER0_PC   = PRESC_VAL;      

   TIMER0_IR   = BIT( IR_MR0 );  //Reset interrupt
   
   TIMER0_MCR  = BIT( MCR0_INT ) | BIT( MCR0_RES ); //Re-occuring interrupts on Match #0. All we have to do is reset the interrupt in the ISR
   TIMER0_MR0  = MATCH_VAL;

   
   TIMER0_TCR  = 0;
   
   
}

/*!
 * @defgroup CVSLOG_lpc2129_systimer_c lpc2129_systimer_c
 * @ingroup CVSLOG
 *  $Log: lpc2129_systimer.c,v $
 *  Revision 1.2  2006-04-08 10:15:56  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.1.2.1  2006/04/08 10:06:25  ambrmi09
 *  In preparation to join with main trunk - added forgotten files
 *
 *
 */
 
 
 
 
 

 