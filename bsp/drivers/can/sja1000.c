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
/*!
@file This module consists of low level routines for the SJA1000 
CAN-controller.
*/
#include <sja1000.h>
#include <assert.h>

static int		_pmode;
static int		_xmode;
static sja1000_t	*_sja1000;

/*!
Initializes the circuit and puts it into operting mode

@note 32bit acceptance masking is used no matter if normal 
or extended frames are used.
*/
int sja1000_init(
	__uint32_t baddr,	//!< Base address of the circuit
	int pmode,		//!< 1=pelican mode, 0=basic mode
	int xmode,		//!< 1=extended frame (i.e. 29 bits header), 0=basic frame (i.e. 11 bits header)
	int bps,		//!< Speed of the bus, nominal speed is 500 Kbps
	__uint32_t ac, 		//!< Acceptance code @note MSB is the same either EFF or SFF
	__uint32_t am		//!< Acceptance mask @note MSB is the same either EFF or SFF
){
	
	_pmode=pmode;
	_xmode=xmode;
	_sja1000 = (sja1000_t*)baddr;

	if (pmode!=1)
		assert("This driver has only been implemented for pelican mode (so far...)" == 0);;

	_sja1000->pelican.mod.RM=1;			//Reset the circuit in case it's not allready

	_sja1000->pelican.cdr.PELICAN=pmode;		//Set in Pelican or basic mode

	_sja1000->pelican.mod.AFM=1;			//32-bit acceptance mask operation enabled.
	_sja1000->pelican.frame.acceptance.code = ac;
	_sja1000->pelican.frame.acceptance.mask = am;

	/*According to SJA1000 manual, mask out bits that should not be cared for*/
	if (xmode){
		_sja1000->pelican.frame.acceptance.mask = 
			_sja1000->pelican.frame.acceptance.mask | 0x00000003;
	}else{
		_sja1000->pelican.frame.acceptance.mask = 
			_sja1000->pelican.frame.acceptance.mask | 0x000F0000;

	}

	while (_sja1000->pelican.mod.RM) {
		int loop=0;
		_sja1000->pelican.mod.RM=0;		//Lift the reset state
		if (++loop>1000)			//Failed to activate SJA1000
			return 1;
	}
	return 0; // i.e. OK
}

/*!
Read incomming RX buffer including header

Returns the nuber of received bytes including the header
*/
int sja1000_read(const char* buffer, int buff_len){
	return buff_len;
}

/*!
Write data to RX buffer and header to frame info

Returns 0 if it was possible to write, else the whole package size is returned.

@note Indata is the whole pachage including header
*/
int sja1000_write(char* buffer, int max_len){
	int i,dlc;

	if (_xmode){
		assure(max_len>=4);
		dlc=max_len-4;
		_sja1000->pelican.frame.frame.frame_info.FF=1;
		_sja1000->pelican.frame.frame.frame_info.RTR=0;
		_sja1000->pelican.frame.frame.frame_info.DLC=dlc;
		_sja1000->pelican.frame.frame.format.EFF.id=((__uint32_t*)buffer)[0];
		for (i=0;i<dlc;i++)
			_sja1000->pelican.frame.frame.format.EFF.data[i] = buffer[i+4];
		
	}else{
		assure(max_len>=2);
		dlc=max_len-2;
		_sja1000->pelican.frame.frame.frame_info.FF=0;
		_sja1000->pelican.frame.frame.frame_info.RTR=0;
		_sja1000->pelican.frame.frame.frame_info.DLC=dlc;
		_sja1000->pelican.frame.frame.format.SFF.id=((__uint16_t*)buffer)[0];
		for (i=0;i<dlc;i++)
			_sja1000->pelican.frame.frame.format.EFF.data[i] = buffer[i+2];
	}
	return 0;
}