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
Notes: See chapter 10.11 for details
*/


#include "systimer.h"
#include <asm/bits.h>
#include <mmap_regs.h>
#include <sys/types.h>
#include <isr.h>
#include <time.h>

/* Trimming constant - Adjust for 1mS __sys_mickey resolution*/

//#define PERT 0xFFFF
//#define PERT 0x1000
//#define PERT 950
//#define PERT 994
#define PERT 999
//#define PERT 12000

/* Select the clock source*/
#define CLK_SRC_OCM 	1
#define CLK_SRC_EXTCLK 	2
#define CLK_SRC 	CLK_SRC_EXTCLK

/* Internal stuff... */
#define RTDIV 7
#define RTSEL 8


/* The tick variables - gets updated on each tick */
__uint32_t __sys_mickey;
__uint32_t __sys_mackey;


#ifdef NEVER

FIXME: 
1) Connect to internal clck source			(done)
2) Figure out where to put the interrupt function	(done)
3) Why doesn't OCM as clk-source work?
#endif


/* Exsamples of handlers - They all do the same thing,
  only coding flavour differs */


//Nice C intensive version
void systimer_Handler_1( void )
{
	piscr_t  *piscr = (piscr_t*)&PISCR;
	pitcnt_t *pitc  = (pitcnt_t*)&PITC;
	
	piscr->f.PS=0x01;	// Reset pending PIT interrupt

	__sys_mickey++;
	(!__sys_mickey)?__sys_mackey++:__sys_mackey;
	
}

//Equivalent to the above but requires no extra variable;
void systimer_Handler_2( void )
{
	((piscr_t*)&PISCR)->f.PS=0x01;
	__sys_mickey++;
}

//Fastest version
void systimer_Handler_3( void )
{
	bitset(PISCR,PISCR_PS);
	__sys_mickey++;
}

void systimer_init(){
	piscr_t  *piscr = (piscr_t*)&PISCR;
	pitcnt_t *pitc  = (pitcnt_t*)&PITC;
	pitcnt_t *pitr  = (pitcnt_t*)&PITR;

	__sys_mickey = 0;
	__sys_mackey = 0;

	piscr->f.PTE=0x0;	//Stop counting (just in case)
	bitclear(PISCR,15);
#if (CLK_SRC == CLK_SRC_OCM)
	bitclear(SCCR,RTSEL);	// Select OCM as clock source
	bitclear(SCCR,RTDIV);	// Prescaler is 4
#else
	bitset(SCCR,RTSEL);	// Select EXTCLK as clock source 
	bitclear(SCCR,RTDIV);	// Prescaler is 4
#endif


	pitc->f.COUNT=PERT;
	//bitset(SCCR,RTDIV);	// Prescaler is 512 (enable this line for testing only)

	piscr->f.PIRQ=Intrnl_3;	// Set IRQ level
	// Place the handler in our vector table
	isr_table[IRQ_3_handler]=systimer_Handler_3; 

	piscr->f.PS=0x01;	// Reset pending PIT interrupt
	piscr->f.ZERO=0x0;	// Fill the padding with zeros

	piscr->f.PITF=0x01;	//The FRZ signal stops the PIT
	piscr->f.PIE=0x01;	//Enable PIE interrupts
	piscr->f.PTE=0x01;	//Start counting}

}

/*System integration interface follows*/

#if MICKEYS_PER_SEC > CLOCKS_PER_SEC
#define CRATIO  (MICKEYS_PER_SEC/CLOCKS_PER_SEC)
#else
#define MRATIO  (CLOCKS_PER_SEC/MICKEYS_PER_SEC)
#endif

clock_t ppc_clock           (){
	unsigned int TmickS_low  = __sys_mickey;           //Low  32 bit worth value (in board time-base)
	long long    TmickS_high = __sys_mackey;           //High ditto

	#ifdef CRATIO	
	long long Tcl = ((TmickS_high << 32) + TmickS_low ) / CRATIO;
	#else
	long long Tcl = ((TmickS_high << 32) + TmickS_low ) * MRATIO;
	#endif
	
	return (clock_t)Tcl; //Possibly trunc it... (best we can do anyway)	
}

