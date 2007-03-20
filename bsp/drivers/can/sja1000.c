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

Notes for connecting to physical bus (note taken for Kvases LapCAN which 
follows ISO 11898-2):

@verbatim
The DRVcan -251, -1053, and -1054 types have the following pin
configuration:
D-SUB pin       Colour code       Function
number
1               N/A               Not connected.
2               Green             CAN_L (low level @ dominant)
3               Brown             GND
4               Orange            Reserved, do not connect.
5               Black             Shield
6               N/A               Not connected.
7               Red               CAN_H (high level @ dominant)
8               N/A               Not connected.
9               Yellow            Not connected.
Pins 2, 3, 4, and 7 are protected by 250 mA fuses.
@endverbatim
*/
#include <sja1000.h>
#include <string.h>
#include <assert.h>
#include <can.h>
#include <tk.h>
#include <tk_tuning.h>
#include <semaphore.h>

#define f_xtal 48000000

//NOTE Below: to be removed
//#define SEF_TEST_MODE 1

//Local variables
static int			_pmode;			//!< Mode of operation (Peliacn =1, Normal=0) 
static int			_IRQ;			//!< IRQ number for the driver
static int			_xmode;			//!< Extended or basic frames
static sja1000_t		*_sja1000;		//!< Address of SJA1000 (chip mapped on struct)
static sja1000_pelican_raw_t	*_sja1000_raw;		//!< Address of SJA1000 (chip mapped on raw struct)
static sja1000_pelican_frame_t	*_pelican_txbuff;	//!< Address of internal Tx buffer (mapped on message struct)

//Variables shared among the files for this driver
sja1000_pelican_frame_t _can_RxBuff[RX_BUFFSZ];	/*!< Drivers recieve buffer. Neede to ease RT requirements
							     and to permit blocking reads. */

int _can_rxidx_in=0;				//!< Points to the next free Rx message storage in ring buffer
int _can_rxidx_out=0;				//!< Points to current message to read from ring-buffer
int _can_nrx=0;				//!< Number of messages waiting in ringbuffer

//Variables defied by other files belonging to this driver
sem_t _can_rx_sem;


/*! Interrupt counters */
static struct {
	__uint32_t BEI; //!< Bus Error Interrupt Enable
	__uint32_t ALI; //!< Arbitration Lost Interrupt
	__uint32_t EPI; //!< Error Passive Interrupt
	__uint32_t WUI; //!< Wake-Up Interrupt
	__uint32_t DOI; //!< Data Overrun Interrupt
	__uint32_t EI;  //!< Error Warning Interrupt
	__uint32_t TI;  //!< Transmit Interrupt
	__uint32_t RI;  //!< Receive Interrupt
}icntr;


/*!
Initializes the circuit and puts it into operting mode

@note 32bit acceptance masking is used no matter if normal 
or extended frames are used.
*/
int sja1000_init(
	__uint32_t baddr,	//!< Base address of the circuit
	int IRQn,		//!< Interrupt number for the main CAN handler
	int pmode,		//!< 1=pelican mode, 0=basic mode
	int xmode,		//!< 1=extended frame (i.e. 29 bits header), 0=basic frame (i.e. 11 bits header)
	int bps,		//!< Speed of the bus, nominal speed is 500 Kbps
	__uint32_t ac, 		//!< Acceptance code @note MSB is the same either EFF or SFF
	__uint32_t am		//!< Acceptance mask @note MSB is the same either EFF or SFF
){
	
	_pmode=pmode;
	_xmode=xmode;
	_sja1000 		= (sja1000_t*)baddr;
	_sja1000_raw 		= (sja1000_pelican_raw_t*)baddr;
	_pelican_txbuff 	= (sja1000_pelican_frame_t*)(baddr+96);

	if (pmode!=1)
		assert("This driver has only been implemented for pelican mode (so far...)" == 0);;

	assure(sem_init(&_can_rx_sem, 0, 0) == 0);

	_sja1000->pelican.mod.RM=1;			//Reset the circuit in case it's not allready

	_sja1000->pelican.cdr.PELICAN=pmode;		//Set in Pelican or basic mode
	_sja1000->pelican.cdr.CBP=1;			//Bypass imput comparator (we have external teancieber)
	_sja1000->pelican.cdr.RXINTEN=0;		//Don't use TX1 as interrupt pin
	//_sja1000->pelican.cdr.CD=3;			//CLKOUT freq is same as f_osc
	_sja1000->pelican.cdr.CD=0;			//CLKOUT freq is f_osc/2
	_sja1000->pelican.cdr.CLK_OFF=0;		//Don't disable external CLKOUT

	_sja1000->pelican.mod.AFM=1;			//32-bit acceptance mask operation enabled.
	_sja1000->pelican.fracc.acceptance.code = ac;
	_sja1000->pelican.fracc.acceptance.mask = am;

	//t_scl = 2 * 1/f_xtal * (BRP + 1) = 1/bps
	//     <=>
	//BRP   = f_xtal/(bps * 2) - 1
	assure( (f_xtal/(bps * 2) - 1) < 0x40);
	//_sja1000->pelican.btr0.BRP = f_xtal/(bps * 2) - 1;
	//_sja1000->pelican.btr0.SJW = 1; //Only guessing TODO: investigate this

	_sja1000->pelican.btr0.BRP = 1;
	_sja1000->pelican.btr0.SJW = 2; //Only guessing TODO: investigate this


	_sja1000->pelican.btr1.SAM=0;	// No triple sampling
	_sja1000->pelican.btr1.TSEG2=2;	// 
	_sja1000->pelican.btr1.TSEG1=5;	// 
	_sja1000->pelican.btr1.TSEG1=3;	// 

	_sja1000->pelican.ocr.OCMODE	=2;	// Normal output mode
	//Drivers for TX0 and TX1 - both in push-pull
	_sja1000->pelican.ocr.OCTP0	=1;
	_sja1000->pelican.ocr.OCTN0	=1;
	_sja1000->pelican.ocr.OCPOL0	=0;

	_sja1000->pelican.ocr.OCTP1	=1;
	_sja1000->pelican.ocr.OCTN1	=1;
	_sja1000->pelican.ocr.OCPOL1	=1;



	#ifdef SEF_TEST_MODE
	_sja1000->pelican.mod.STM=1;			//Self Test Mode - will not require acks on the bus
	#endif

	/*According to SJA1000 manual, mask out bits that should not be cared for*/
	if (xmode){
		_sja1000->pelican.fracc.acceptance.mask = 
			_sja1000->pelican.fracc.acceptance.mask | 0x00000003;
	}else{
		_sja1000->pelican.fracc.acceptance.mask = 
			_sja1000->pelican.fracc.acceptance.mask | 0x000F0000;
	}

	//_sja1000_raw->cdr_raw=0xC0;;		//Kock info - enter PeliCan and external transceiver mode
	//_sja1000_raw->btr0_raw=0x81;;		//Kock info - Bitrate 500khz, Jump width 2
	//_sja1000_raw->btr1_raw=0x23;;		//Kock info - TSEG ???
	_sja1000_raw->ocr_raw=0xDB;		//Kock info

/*Set-up the interrupts*/
	_IRQ=IRQn;
	memset(&icntr,0,sizeof(icntr));
	tk_isr_install(IRQn,sja1000_Handler);
	//tk_isr_install(IRQn,sja1000_Handler);
	//isr_table[IRQ_3_handler]=sja1000_Handler;

	_sja1000->pelican.ier.BEIE=1;	//Bus Error Interrupt Enable
	_sja1000->pelican.ier.ALIE=1;	//Arbitration Lost Interrupt Enable
	_sja1000->pelican.ier.EPIE=1;	//Error Passive Interrupt Enable
	_sja1000->pelican.ier.WUIE=1;	//Wake-Up Interrupt Enable
	_sja1000->pelican.ier.DOIE=1;	//Data Overrun Interrupt Enable
	_sja1000->pelican.ier.EIE=1;	//Error Warning Interrupt Enable
	_sja1000->pelican.ier.TIE=1;	//Transmit Interrupt Enable
	_sja1000->pelican.ier.RIE=1;	//Receive Interrupt Enable


	while (_sja1000->pelican.mod.RM) {
		int loop=0;
		_sja1000->pelican.mod.RM=0;		//Lift the reset state
		if (++loop>1000)			//Failed to activate SJA1000
			return 1;
	}


	_sja1000->pelican.cmr.bits.AT=1;		//Abort any transmission pending
	_sja1000->pelican.cmr.bits.CDO=1;		//Clear Data Overrun
	_sja1000->pelican.cmr.bits.RRB=1;		//Release receive buffer

	return 0; // i.e. OK
}

/*!
Read incomming RX buffer including header

Returns the number of received bytes including the header
*/
int sja1000_read(const char* buffer, int buff_len){
	int nRx;
	can_t *frmtbuff = buffer;
	int dlc,psz;

	_sja1000->pelican.ier.RIE = 0;
	nRx = _can_nrx;
	_sja1000->pelican.ier.RIE = 1;	
	assert(nRx<RX_BUFFSZ);

	
	sem_wait(&_can_rx_sem);
	
	dlc = _can_RxBuff[_can_rxidx_out].frinfo.DLC;
	psz = dlc + sizeof(__uint32_t);

	assert(buff_len>=psz);

	if (_can_RxBuff[_can_rxidx_out].frinfo.FF) {
		frmtbuff->id = _can_RxBuff[_can_rxidx_out].format.EFF.id;
		memcpy(
			frmtbuff->data,
			_can_RxBuff[_can_rxidx_out].format.EFF.data,
			dlc
		);
	}else{
		frmtbuff->id = _can_RxBuff[_can_rxidx_out].format.SFF.id;
		memcpy(
			frmtbuff->data,
			_can_RxBuff[_can_rxidx_out].format.SFF.data,
			dlc
		);

	};

	_can_rxidx_out++;
	_can_rxidx_out = _can_rxidx_out % RX_BUFFSZ;

	_sja1000->pelican.ier.RIE = 0;
	_can_nrx--;
	_sja1000->pelican.ier.RIE = 1;

	return psz;
}

/*!
Write data to RX buffer and header to frame info

Returns 0 if it was possible to write, else the whole package size is returned.

@note Indata is the whole package including header
*/
int sja1000_write(char* buffer, int max_len){ 
	int i,dlc;
	//sja1000_pelican_frinfo_t frinfo;
	sja1000_pelican_frame_t	frame;

	while (!_sja1000->pelican.sr.TBS);			//Wait for transfer buffer to become free. NOTE This is a busy wait and might need improvement

	if (_xmode){
		assure(max_len>=4);
		dlc=max_len-4;
		
		frame.frinfo.FF=1;
		frame.frinfo.RTR=0;
		frame.frinfo.DLC=dlc;
		frame.frinfo.padd=0;
		frame.format.EFF.id=((__uint32_t*)buffer)[0];
		frame.format.EFF.padd2=0;
		for (i=0;i<dlc;i++)
			frame.format.EFF.data[i] = buffer[i+4];
		
	}else{
		assure(max_len>=2);
		dlc=max_len-2;

		frame.frinfo.RTR=0;
		frame.frinfo.DLC=dlc;
		frame.frinfo.padd=0;

		frame.frinfo.FF=0;

		frame.format.SFF.id=((__uint16_t*)buffer)[0];
		frame.format.SFF.padd2=0;
		for (i=0;i<dlc;i++)
			frame.format.EFF.data[i] = buffer[i+2];
	}

	memcpy(&_sja1000->pelican.fracc.frame,&frame,sizeof(frame));

	_sja1000->pelican.cmr.bits.TR=1;			//Transmission request
	//_sja1000->pelican.cmr.cmd= BIT_SRR /*| BIT_TR*/;	//Self transmission request
/*
	__uint8_t mycmd = BIT_SRR;
	sja1000_pelican_cmr_t hippshapps;
	hippshapps.cmd= mycmd;
*/

	return 0;
}


/* ISR - Interrupt handlers */ 
/*! Bus Error Interrupt*/
void isr_BEI(){
	icntr.BEI++;
};

/*! Arbitration Lost Interrupt */
void isr_ALI(){
	icntr.ALI++;
};

/*! Error Passive Interrupt */
void isr_EPI(){
	icntr.EPI++;
};

/*! Wake-Up Interrupt */
void isr_WUI(){
	icntr.WUI++;
};

/*! Data Overrun Interrupt */
void isr_DOI(){
	_sja1000->pelican.cmr.bits.CDO=1;
	icntr.DOI++;
};

/*! Error Warning Interrupt */
void isr_EI(){
	icntr.EI++;
};

/*! Transmit Interrupt */
void isr_TI(){
	icntr.TI++;
	//_sja1000->pelican.ier.TIE=0;	
};

/*! Receive Interrupt */
void isr_RI(){
	//CMR.2 RRB Release Receive Buffer

	icntr.RI++;
	_can_nrx++;

	memcpy(
		&_can_RxBuff[_can_rxidx_in],
		&_sja1000->pelican.fracc.frame,
		sizeof(sja1000_pelican_frame_t)
	);
	_sja1000->pelican.cmr.bits.RRB=1;
	_can_rxidx_in++;
	_can_rxidx_in = _can_rxidx_in % RX_BUFFSZ;

	sem_post(&_can_rx_sem);
};

/*"Main SJA1000 interrupt handler*/
void sja1000_Handler( void )
{
	sja1000_pelican_ir_t ir = _sja1000->pelican.ir;

	if (ir.BEI)
		isr_BEI();
	if (ir.ALI)
		isr_ALI();
	if (ir.EPI)
		isr_EPI();
	if (ir.WUI)
		isr_WUI();
	if (ir.DOI)
		isr_DOI();
	if (ir.EI)
		isr_EI();
	if (ir.TI)
		isr_TI();
	if (ir.RI)
		isr_RI();
	tk_isr_eoi(_IRQ);	//Tell system handler is done
	//_sja1000->pelican.mod.RM=1;			//Reset the circuit in case it's not allready
}


