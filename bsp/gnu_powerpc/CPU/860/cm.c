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
Communications module
*/


#include <asm/cm.h>
#include <mmap_regs.h>

#include <isr.h>

int CM_default_nn = 0;

void CM_default_handler(){
	CM_default_nn++;
};

isr_handler _cm_isr_table[32]={
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler,
	CM_default_handler
};



/*
Notes on how to handle CM interrupts:
1. Set the CIVR[IACK].
2. Read CIVR[VN] to determine the vector number for the interrupt handler.
3. Immediately read the SCC2 event register into a temporary location.
4. Decide which events in the SCCE2 must be handled and clear those bits as soon as
   possible. SCCE bits are cleared by writing ones.
5. Handle the events in the SCC2 Rx BD or Tx BD tables.
6. Clear CISR[SCC2].

*/

void CM_Handler( void ){
	civr_t *civr_p = (civr_t*)&CIVR;
	civr_p->f.IACK=1;
	civr_t civr_temp = *civr_p;

	int i;
	__uint32_t cisr_raw;

	cicr_t *cicr_p = (cicr_t*)&CICR;

	ciid_t *cipr_p = (ciid_t*)&CIPR;
	ciid_t *cimr_p = (ciid_t*)&CIMR;
	ciid_t *cisr_p = (ciid_t*)&CISR;

/*
	civr_t civr_temp;
	civr_temp.raw=0;
	civr_temp.f.VN = CPMIV_SMC1;
	civr_temp.f.IACK=1;
	CIVR = civr_temp.raw;
*/

	//Note that several CM IRQ bits might be set.
	/*
	for (i=0;i<32;i++){
		if (civr_temp.f.VN & m){
			_cm_isr_table[i]();
			cisr_p->raw |= m;    //Set the corresponding bit. I.e. notify that we're done
		}
		m = m << 1;
	}
	*/

	_cm_isr_table[civr_temp.f.VN]();
	cisr_p->raw |= (0x01 << civr_temp.f.VN);
	//cisr_p->raw &= (~(0x01 << civr_temp.f.VN));

}

/*!
Attach an interrupt handler routine to a certain CM event
This is second level IRQ handling defined by HW.
@note Mimics TinKer 1'st level IRQ
*/
int CM_isr_install(int level, isr_handler isr){
	_cm_isr_table[level]=isr;
	return 0;
};


void CM_init(){
	cicr_t *cicr_p = (cicr_t*)&CICR;

	cicr_p->f.IRL=5;
	cicr_p->f.IEN=1;

	civr_t *civr_p = (civr_t*)&CIVR;
	civr_p->f.IACK=1;

	tk_isr_install(lvl_Intrnl_5,CM_Handler);

}
