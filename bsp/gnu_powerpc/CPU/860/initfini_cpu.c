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
Initialice CPU specific internals
*/

#include <arch/powerpc/bits.h>
#include <asm/msr.h>
#include <asm/cm.h>
#include <asm/plprcr.h>
#include <asm/ictrl.h>
#include <asm/sccr.h>
#include <mmap_regs.h>
#include <isr.h>

//#define ISCT_SER_VAUE 0x00

//Debug variable for .sbss cleaning - remove later FIXME
#include <reent.h>
struct _reent *myPtr;

/*Consider putting this somewhere else FIXME */
typedef union{ 
	__uint32_t raw; 
	
	struct {
		__uint32_t ISB:16;
		__uint32_t PARTNUM:8;
		__uint32_t MASKNUM:8;
	
	}f;
}immr_t;


extern char _btext, _etext, _bdata, _edata, _bstart, _bend;
extern char _sizeof_t, _sizeof_d, _sizeof_b;
extern char heap_end;

void __init_cpu(){
	SIMASK=0x0; //Dissallow interrupts in case perepherials have not been reseted properly
	{
		plprcr_t *plprcr_p = (plprcr_t *)&PLPRCR;
		ictrl_t ictrl;
	/*
	Does not work with debugger.... Find out why => Done - see e-mail
		GET_SPR(_ICTRL,ictrl);
		ictrl.f.ISCT_SER=ISCT_SER_VAUE;
		//SET_SPR(_ICTRL,ictrl);
		GET_SPR(_ICTRL,ictrl);
		while (ictrl.f.ISCT_SER!=ISCT_SER_VAUE) {
			GET_SPR(_ICTRL,ictrl);
		}
	
		plprcr_p->f.MF=0x0FF;
	*/
		plprcr_t plprcr_test;
		plprcr_test.raw=0x00000000;
		plprcr_test.f.MF=0xFFF; 
	
		plprcr_test.raw=0x0000D000;

		//Set the output buffer strength of the CLKOUT pin.
		sccr_t *sccr_p = (sccr_t*)&SCCR;
		#define COM_ENABLED 	0x00 	//00 Clock output enabled full-strength buffer.
		#define COM_HALF 	0x01	//01 Clock output enabled half-strength output buffer.
						//10 Reserved.
		#define COM_DISABLED	0x03	//11 Clock output disabled.
		//sccr_p->f.COM=COM_DISABLED;
		//sccr_p->f.COM=0xc0000000;
		sccr_p->f.COM=COM_ENABLED;
	}


	// DER - debugg interface stuff 8does this work at all?)
	{
		#define DER_EXTIE 6
		__uint32_t der;
		GET_SPR(_DER,der);
		bitset(der,DER_EXTIE);	// Set -> Ext Int causes debug mode
		SET_SPR(_DER,der);
		GET_SPR(_DER,der);
	}
	//MSR - Mashine State Register
	{
		msr_t msr;
		GET_MSR(msr);
		int has_fp;
		int isr_highmem;
		int recoverable_ex;
		int machine_check;
		int litte_endian;
		int ext_excpt;
	
		has_fp = msr.f.FP;
		isr_highmem = msr.f.IP;
		recoverable_ex = msr.f.RI;
		machine_check = msr.f.ME;
		litte_endian = msr.f.LE;
		ext_excpt = msr.f.EE;
	
		#if defined (__BIG_ENDIAN__)
		if (litte_endian)
			return;                 //This is an endian error but we can't handle it yet
		#else
		if (!litte_endian)
			return;                 //This is an endian error but we can't handle it yet
		#endif
	
	
		if (isr_highmem){               //Always assume exceptions to be 0x0000-0x10000
			msr.f.IP = 0;
			SET_MSR(msr);
		}
	}	
	{
		//Initialize memory /*cleaning .sbss*/

		// FIXME It seems wrong with a stack size defined here...
		#define STACK_SIZE 0x0600	

		char *btext= 	&_btext;
		char *etext= 	&_etext; 
		char *bdata= 	&_bdata; 
		char *edata= 	&_edata; 
		char *bstart= 	&_bstart;
		char *bend= 	&_bend;
	
		char *sizeof_t=	&_sizeof_t;
		char *sizeof_d=	&_sizeof_d; 
		char *sizeof_b=	&_sizeof_b;

		//Some initializers depend on that variables start with value zero
		int *int_p;

/*NOTE Not sure about the dtata types below - samples usually show char */
		extern int __SDATA_START__;
		extern int __SBSS_START__;
		extern int __SBSS_END__;
		int *sbss_start = &__SBSS_START__;
		int *sbss_end = &__SBSS_END__;
		for (int_p=sbss_start; int_p <= sbss_end ; int_p++)
			*int_p=0;

		myPtr = _impure_ptr;


	}

	// IMMR Internal Memory Map Register 
	{
		#ifndef DMM1
		#error  Don't know where to map Internal Memory
		#endif
		immr_t immr;
		
		GET_SPR(_IMMR,immr);
		/*Assume adress has allreaby been set, either by HW, debugger or previous code*/
		/*This test might be omitted later*/
		if ((immr.f.ISB<<16) != DMM1)
			return;                 //This is an IMMR error but we can't handle it yet
	}
	/*Other internal self contained perepherials*/
	systimer_init();
	CM_init();

}

void __fini_cpu(){
}

void __exeptions_enable_cpu(){
	__uint32_t *_simask = &SIMASK;
	/* Enable external exeptions */
	{
		msr_t msr;
		SIPEND=0x0000;		//Clear pending IRQ and lvl
		GET_MSR(msr);
		if (msr.f.EE==0){
			msr.f.EE = 1;
			SET_MSR(msr);
		}
		msr.f.EE = 1;
		SET_MSR(msr);
	
		SET_SPR(_EIE,0xFF);	//External interrupt enable (command to the SIU, normally in prologe code)
	
		
//		SIMASK=0xFFFF0000;	//Allow all kinds of IRQ	
		bitset(SIMASK,LVL3);	//Set LVL3 interrupts
		bitset(SIMASK,IRQ3);	//Set IRQ3 interrupts (CAN)

		bitset(SIMASK,LVL5);	//Set LVL5 interrupts (CPM)		


		//SIEL=0x00000000;	//IRQ on level, no wakeup from low-pow
		SIEL=0xFFFF0000;	//IRQ on edge, do wakeup from low-pow
	}

}

void __exeptions_disable_cpu(){
}

