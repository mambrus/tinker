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


#include "isr.h"
#include <asm/bits.h>
#include <asm/msr.h>
#include <mmap_regs.h>
#include <sys/types.h>
#include <systimer.h>



#ifdef NEVER
<Place comments here>
#endif

__int8_t idx;
__int8_t n_isr=0;

void default_handler(){
	idx++;
	idx--;
};

void alignement_issue(){
	idx++;
	idx--;
};

void possible_starvation(){
	idx++;
	idx--;
};

__uint32_t exc_id;
/*Just loop until watch-dog gets us...*/
void exception_invalid(){
	GET_GPR(0,exc_id);
	exc_id = exc_id & 0x0000000F;

	while(1);
}


handler_function isr_table[16]={
	default_handler,
	default_handler,
	default_handler,
	default_handler,
	default_handler,
	default_handler,
	default_handler,
	default_handler,
	default_handler,
	default_handler,
	default_handler,
	default_handler,
	default_handler,
	default_handler,
	default_handler,
	default_handler
};
//#include <pushpop.h>	/*Do not push/pop here. let the exception code do it instead*/
#define DONT_FECKUP_ON_PURPOSE 1
void isr_external(){	
	//PUSHALL;
	__uint32_t stack_ptr;
	__uint32_t cr;
	idx=*(__int8_t*)(&SIVEC);
	idx = (idx/4)-1;
	
	GET_GPR(1,stack_ptr);

	n_isr++;
	if (n_isr>1)
		possible_starvation();

	if (stack_ptr & 0x00000003)
		alignement_issue();
	else
		isr_table[idx]();
#ifndef DONT_FECKUP_ON_PURPOSE
//feck up un purpose to check that context is fully saved and resored
	SET_SPR(_CTR,0xFFFF);
	SET_SPR(_XER,0xFFFF);
	GET_CR(cr);
	SET_CR(0xFFFFFFFF);
#endif
	SET_SPR(_EIE,0xFF);	//permit nested interrupts
	n_isr--;
	//POPALL;
}


