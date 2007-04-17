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
Initialice board specific internals and perepherials
*/

#include <board/board.h>

#if ( TK_DCPU == __tk_860__ )
void __init_board(){

	//SC stuff (Serial Controller slvae processor) 
	// Clear the DP ram - start (DMM1 + 0x2000), 8k in size
	// The issue reset commant to CM
	// This operation is required by the CM
	{
		int i;
		unsigned char *addr_p = (__uint8_t *)(DMM1 + 0x2000);
		for (i=0;i<0x2000;i++){
			*addr_p = 0x00;
			addr_p++;
		}
		//CPM reset using the CPCR
		((cpcr_t*)&CPCR)->f.RST=1;
		while (((cpcr_t*)&CPCR)->f.FLG);

		// Set up the DP ram internal stucture using RCCR[ERAM], RMDS[ERAM4K]
		// There is no information about the details of RMDS in the manual, so
		// assume the worst. I.e. Set RCCR to minimum free DB/Data for the app and 
		// assume RDS[ERAM4k] is enabled. This leaves with the following free 
		// memory mapping (offset including):

		// 0x2800 - 0x2E00	DP_BD_0		Descriptors/Data
		// 0x3800 - 0x3A00	DP_BD_1		Descriptors/Data
		// 0x3C00 - 0x3E00	DP_PARA		Parameter area

		((rccr_t*)&RCCR)->f.ERAM=DP_ERAM;
				
	}

	{
	/*
	I/O and leds - this is actually part of the CPU but we want to keep the CPU init 
	generic and I/O config change from applicatio (i.e. board) to application
	*/	
	
		PAPAR=0x0;              //Make PA all I/O
	
	
	/*LED stuff*/
		bitset(PADIR,11);       //Make pin output (led1 green)
		bitset(PADIR,10);       //Make pin output (led1 red)
		bitset(PADIR,9);        //Make pin output (led2 green)
		bitset(PADIR,8);        //Make pin output (led2 red)
	
		bitset(PADAT,11);       //Turn off pin
		bitset(PADAT,10);       //Turn off pin
		bitset(PADAT,9);        //Turn off pin
		bitset(PADAT,8);        //Turn off pin
	
	/*MIB (serial) stuff*/
		bitset(PBPAR,25);       //Make pin PB25 SMTXD1 (MIB_TXD)
		bitclear(PBDIR,25);     // -"-
		bitclear(PBODR,25);     // Open Drain (not) - Drives output activly
	
		bitset(PBPAR,24);       //Make pin PB24 SMRXD1 (MIB_RXD)
		bitclear(PBDIR,24);     // -"-
		bitclear(PBODR,24);     // Open Drain (not) - Drives output activly
	
		bitclear(PBPAR,18);     //Make pin PB18 an I/O  (MIB_DET)
		bitclear(PBDIR,18);     //make it an inpuut
	}

/* Initialize perephrials according to application specifics */
	//console_init(9600, 8,'N',1);
	//console_init(115200, 8,'N',1);
	console_init(57600, 8,'N',1);
}

#else
#error Can't handle chosen CPU for this board
#endif


void __fini_board(){
}

void __exeptions_enable_board(){
}

void __exeptions_disable_board(){
}

