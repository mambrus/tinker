/*+***************************************************************************
**
**
**
**
** Original Author: Mihael Ambrus	Creation Date: 070129
**
** Approved:	Department: MCC-VS
**
**                 Copyright (c) Maquet Critical Care, Sweden
**                        All rights reserved
**
***************************************************************************-*/
#include "console.h"

#include <board/board.h>
#include <tk.h>
#include "hwboard.h"
#include <CPU/860/isr.h>
#include <CPU/860/asm/si.h>
#include <string.h>
#include <assert.h>
#include <semaphore.h>

#define CHAN_SMC1 	0x09

static char RX_BD_BUFFER[SMC_BD_RX_BUFFLEN]; 
static char TX_BD_BUFFER[SMC_BD_TX_BUFFLEN];

#define SMC_CHAR_BUFFLEN	1024	//!< Lengt of intermediate buffert (between ISR and FS)

static char RX_CHAR_BUFFER[SMC_CHAR_BUFFLEN];
static int rx_inIdx=0;
static int rx_outIdx=0;


sem_t _con_rx_sem;
int _con_stdio_init = 0;


#ifdef NEVER
29.3.13 SMC UART Controller Programming Example
The following initialization sequence assumes 9,600 baud, 8 data bits, no parity, and 1 stop
bit in a 25-MHz system. BRG1 and SMC1 are used.
1. Conﬁgure the port B pins to enable SMTXD1 and SMRXD1. Set PBPAR[24, 25]
	then clear PBDIR[24, 25] and PBODR[24, 25].
2. Conﬁgure the BRG1. Write BRGC1 with 0x01_0144. The DIV16 bit is not used
	and the divider is 162 (decimal). The resulting BRG1 clock is 16× the preferred bit
	rate.
3. Connect BRG1 to SMC1 using the SI. Clear SIMODE[SMC1, SMC1CS].
4. Assuming one RxBD at the beginning of dual-port RAM followed by one TxBD,
	write RBASE with 0x0000 and TBASE with 0x0008.
5. Write 0x0091 to CPCR to execute the INIT RX AND TX PARAMETERS command.
6. Initialize the SDMA conﬁguration register (SDCR) to 0x0001.
7. Write RFCR and TFCR with 0x10 for normal operation.
8. Write MRBLR with the maximum number of bytes per receive buffer. Assume 16
	bytes, so MRBLR = 0x0010.
9. Write MAX_IDL with 0x0000 in the SMC UART-speciﬁc parameter RAM to
    disable the MAX_IDL functionality for this example.
10. Clear BRKLN and BRKEC in the SMC UART-speciﬁc parameter RAM.
11. Set BRKCR to 0x0001; if a STOP TRANSMIT COMMAND is issued, one break
    character is sent.
12. Initialize the RxBD. Assume the Rx buffer is at 0x0000_1000 in main memory.
    Write 0xB000 to Rx_BD_Status, 0x0000 to Rx_BD_Length (not required), and
    0x0000_1000 to Rx_BD_Pointer.
13. Assuming the Tx buffer is at 0x00002000 in main memory and contains ﬁve 8-bit
    characters, write 0xB000 to Tx_BD_Status, 0x0005 to Tx_BD_Length, and
    0x00002000 to Tx_BD_Pointer.
14. Write 0xFF to the SMCE register to clear any previous events.
15. Write 0x17 to the SMCM register to enable all possible SMC interrupts.
16. Write 0x00000010 to the CIMR so the SMC1 can generate a system interrupt.
    Initialize the CICR.
17. Write 0x4820 to SMCMR to conﬁgure normal operation (not loopback), 8-bit
    characters, no parity, 1 stop bit. The transmitter and receiver are not yet enabled.
18. Write 0x4823 to SMCMR to enable the SMC transmitter and receiver. This
    additional write ensures that the TEN and REN bits are enabled last.

After 5 bytes are sent, the TxBD is closed. The receive buffer closes after receiving 16
bytes. Subsequent data causes a busy (out-of-buffers) condition since only one RxBD is
ready.
#endif

int bsleep( int time ){
	int i,j,k,t=0x123;

	for (i=0; i<time; i++)
		for (j=0; j<6; j++)
			for (k=0; k<14; k++)
				t = (t * 776) % 8;
}

static char test_str[]="\n\rHello world! \n\rmpc860 is a @#&! micro-controller but Ambrus is a conqueror... \n\r";


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

static int nn_rx 	= 0;
static int nn_tx 	= 0;
static int nn_bsy 	= 0;
static int nn_brk 	= 0;
static int nn_brke 	= 0;

void _console_rx_handler();
void _console_tx_handler();
void _console_bsy_handler();
void _console_brk_handler();
void _console_brke_handler();

void console_Handler( void ){
	smce_smcm_t *smce_smcm_p = (smce_smcm_t*)&SMCE1;
	smcmr_t *smcmr_p = (smcmr_t*)&SMCMR1;
	smc_param_t *smc_param=(smc_param_t *)DP_PARA_SMC1;
	__uint8_t smce1 = SMCE1;

	if (smce1 & SMCE_RX) {
		nn_rx++;
		smce_smcm_p->f.RX=1;
		_console_rx_handler();
		//SMCE1 |= SMCE_RX;
	}
/*
	if (smce1 & SMCE_TX) {
		nn_tx++;
		smce_smcm_p->f.TX=1;
		_console_tx_handler();
		//SMCE1 |= SMCE_TX;
	}
*/
	if (smce1 & SMCE_BSY) {
		nn_bsy++;
		smce_smcm_p->f.BSY=1;
		_console_bsy_handler();
		//SMCE1 |= SMCE_BSY;
	}

	if (smce1 & SMCE_BRK) {
		nn_brk++;
		smce_smcm_p->f.BRK=1;
		_console_brk_handler();
		//SMCE1 |= SMCE_BRK;
	}
	if (smce1 & SMCE_BRKE) {
		nn_brke++;
		smce_smcm_p->f.BRKE=1;	
		_console_brke_handler();
		//SMCE1 |= SMCE_BRKE;
	}

}


/*
The function should be able to figure out BRG[CD] divisor settings by itself based on
the OSCCLK constant and PLPRCR[MF] & SCCR[DFBRG] setting


               VCOOUT freq
BRGCLK freq = ---------------------------------------
                                                    -
                    2 × DFBRG
              (2                                    )

*/
int console_init(int bpr, int nr_bits, int par, int nr_stop){
//Clean the buffers
	//bpr = 9600;

	{
		char *RxTemp=RX_BD_BUFFER;
		char *TxTemp=TX_BD_BUFFER;
		int i;
		for (i=0;i<SMC_BD_RX_BUFFLEN;i++){
			RX_BD_BUFFER[i]='R';
		}
		for (i=0;i<SMC_BD_TX_BUFFLEN;i++){
			TX_BD_BUFFER[i]='T';
		}
	}
	cpcr_t *cpcr_p = (cpcr_t*)&CPCR;
	smcmr_t *smcmr1_p = (smcmr_t*)&SMCMR1;
	brgc_t *brgc1_p = (brgc_t*)&BRGC1;
	sccr_t *sccr_p = (sccr_t*)&SCCR;
	plprcr_t *plprcr_p = (plprcr_t*)&PLPRCR;

	cicr_t *cicr_p = (cicr_t*)&CICR;
	civr_t *civr_p = (civr_t*)&CIVR;

	ciid_t *cipr_p = (ciid_t*)&CIPR;
	ciid_t *cimr_p = (ciid_t*)&CIMR;
	ciid_t *cisr_p = (ciid_t*)&CISR;

	simode_t *simode_p = (simode_t*)&SIMODE;

// Baud rate generator stuff (page 20.4.1)

// 2)
//Conﬁgure the BRG1. Write BRGC1 with 0x01_0144. The DIV16 bit is not used
//and the divider is 162 (decimal). The resulting BRG1 clock is 16× the preferred bit
//rate.

	{	
		brgc1_p->f.RST=1;
		__uint32_t oscclk = F_OSCCLK;
		__uint32_t brgclk = (oscclk * (plprcr_p->f.MF+1)) / (1 << sccr_p->f.DFBRG);
		__uint32_t cd = brgclk / bpr;

		cd = cd / 16;		//Not certain why needed. FIXME RCCR[TBS]? (Timebase frequency source is GCLK2 divided by 16.)
		if (cd > 0xFFF){
			/*Frequency to high -> We need to activate extra divisor*/
			brgc1_p->f.DIV16=1;
			cd=cd >> 4;
			
		}
		brgc1_p->f.CD = cd;
	
		brgc1_p->f.EN=1;
	}

// 3)
// SI mode (page 20.2.4.2)
//Connect BRG1 to SMC1 using the SI. Clear SIMODE[SMC1, SMC1CS]. 
/*
	bitclear(SIMODE,16);  //NMSI - I.e. dedicated pin, no multiplex 
	bitclear(SIMODE,17); //Clock source is BRG1 FIXME: this is 3 bits long 
*/
	simode_p->f.SMC1	=0x0; //NMSI - I.e. dedicated pin, no multiplex 
	simode_p->f.SMC1CS	=0x0; //Clock source is BRG1 (=0)
//Optional
	simode_p->f.SDMa 	= 0; //Diagnositc mode. =1 should be Automatic echo - doesn't work ;(
	simode_p->f.RFSDa 	= 0;
	simode_p->f.DSCa 	= 0;
	simode_p->f.CRTa 	= 0;
	simode_p->f.STZa 	= 0;
	simode_p->f.CEa 	= 0;
	simode_p->f.FEa 	= 0;
	simode_p->f.GMa 	= 0;
	simode_p->f.TFSDa 	= 0;
// 4)
/*
4. Assuming one RxBD at the beginning of dual-port RAM followed by one TxBD,
	write RBASE with 0x0000 and TBASE with 0x0008.
*/

//Note, We can't follow this asumtion since we'we configured DP for worst case
//Instead, first available memory is DP_BD_0_OFFS
	smc_param_t *smc_param=(smc_param_t *)DP_PARA_SMC1;
	smce_smcm_t *smce1=(smce_smcm_t *)&SMCE1;
	smce_smcm_t *smcm1=(smce_smcm_t *)&SMCM1;


	smc_param->RBASE=DP_BD_0_OFFS;		/*Note: offset counting from DP start*/
	smc_param->TBASE=DP_BD_0_OFFS + 8;	/*Note: offset counting from DP start*/
	bd_smc_t *RxBD=(bd_smc_t *)DP_BD_0;
	bd_smc_t *TxBD=(bd_smc_t *)(DP_BD_0+8);


//5. Write 0x0091 to CPCR to execute the INIT RX AND TX PARAMETERS command.
	CPCR = 0x0091;


// 6. Initialize the SDMA conﬁguration register (SDCR) to 0x0001.
	SDCR = 0x0001;

// 7. Write RFCR and TFCR with 0x10 for normal operation.
//Eeh? Some SDMA function code and byte ordering stuff...
// See 29.2.3.1 SMC Function Code Registers (RFCR/TFCR)

/*
	smc_param->RFCR.raw = 0x10;
	smc_param->TFCR.raw = 0x10;
*/
	smc_param->RFCR.f.BO = 0x2;		//True big endian
	smc_param->RFCR.f.AT = 0x0;		//Address type (SDMA fynction code)
	smc_param->TFCR.f.BO = 0x2;
	smc_param->TFCR.f.AT = 0x0;

//8. Write MRBLR with the maximum number of bytes per receive buffer. Assume 16
//	bytes, so MRBLR = 0x0010.
// Maximum receive buffer length. Deﬁnes the maximum number of bytes the CP writes
// to a receive buffer before it goes to the next buffer.


	//smc_param->MRBLR = 0x0010;
	smc_param->MRBLR = 0x0001;


//9. Write MAX_IDL with 0x0000 in the SMC UART-speciﬁc parameter RAM to
//    disable the MAX_IDL functionality for this example.
	
	smc_param->PROTO.SMC_UART.MAX_IDL=0x0000;
	//smc_param->PROTO.SMC_UART.MAX_IDL=0x0005;
	smc_param->PROTO.SMC_UART.R_MASK=0x0000;


//10. Clear BRKLN and BRKEC in the SMC UART-speciﬁc parameter RAM.
	smc_param->PROTO.SMC_UART.BRKLN=0;
	smc_param->PROTO.SMC_UART.BRKEC=0;

//11. Set BRKCR to 0x0001; if a STOP TRANSMIT COMMAND is issued, one break
//    character is sent.

	smc_param->PROTO.SMC_UART.BRKCR=0x0001;

//12. Initialize the RxBD. Assume the Rx buffer is at 0x0000_1000 in main memory.
//    Write 0xB000 to Rx_BD_Status, 0x0000 to Rx_BD_Length (not required), and
//    0x0000_1000 to Rx_BD_Pointer.

	RxBD->BD_Status.raw = 0xB000;
	RxBD->BD_Length = 0;
	RxBD->BD_Pointer = (__uint32_t)RX_BD_BUFFER;
	//RxBD->BD_Status.SMC_RX.f.CM_Continuous_mode = 1; //Avoid having to RxBD->BD_Status.SMC_RX.f.E_Empty = 1; in _console_rx_handler()
	//RxBD->BD_Pointer = 0x10000000; //Old stuff for debugging (obsolete)

//13. Assuming the Tx buffer is at 0x00002000 in main memory and contains ﬁve 8-bit
//    characters, write 0xB000 to Tx_BD_Status, 0x0005 to Tx_BD_Length, and
//    0x00002000 to Tx_BD_Pointer.

	TxBD->BD_Status.raw = 0xB000;
	//TxBD->BD_Length = SMC_BD_TX_BUFFLEN;
	TxBD->BD_Length = 0x0005;
	TxBD->BD_Pointer = (__uint32_t)TX_BD_BUFFER;
	//TxBD->BD_Status.SMC_TX.f.I_Interrupt = 0;  //Don't set Interrupt status on send. Makes it slow. BD busy?
	//TxBD->BD_Status.SMC_TX.f.CM_Continuous_mode = 1; //Not a good idea at all. Will never stop sending the BD

	//TxBD->BD_Pointer = 0x100000a0; //Old stuff for debugging (obsolete)


//Install ISR before any further bit-mipps
	CM_isr_install(cmid_SMC1,console_Handler);


//14. Write 0xFF to the SMCE register to clear any previous events.	
	SMCE1 = 0xFF;

//15. DONT! Write 0x17 to the SMCM register to enable all possible SMC interrupts.

//	SMCM1 = 0x17;

// All but TX instead (batter and faster)

	smcm1->f.RX=1;
	smcm1->f.TX=0;
	smcm1->f.BSY=1;
	smcm1->f.BRK=1;
	smcm1->f.BRKE=1;	


//16. Write 0x00000010 to the CIMR so the SMC1 can generate a system interrupt.
	CIMR = 0x00000010;

//16.b    Initialize the CICR.
/*
Now done as part of CM_Init():	
	cicr_p->f.IRL=5;
	cicr_p->f.IEN=1;

	civr_t civr_temp;
	civr_temp.raw=0;
	civr_temp.f.VN = CPMIV_SMC1;
	civr_temp.f.IACK=1;
	CIVR = civr_temp.raw;
*/
/*
NOTE 
34.5.1 CPM Interrupt Conﬁguration Register (CICR)
The CPM interrupt conﬁguration register (CICR) deﬁnes CPM interrupt request levels, the
priority between the SCCs, and the highest priority interrupt.

ehh??
*/

//17. Write 0x4820 to SMCMR to conﬁgure normal operation (not loopback), 8-bit
//    characters, no parity, 1 stop bit. The transmitter and receiver are not yet enabled.
	{
		int clen = 1;
		clen += nr_bits;
	
		smcmr1_p->uart.raw=0x0;
		
		smcmr1_p->uart.f.SM=2; /*UART mode*/
		smcmr1_p->uart.f.SL=nr_stop-1;
		clen += nr_stop;
		switch (par){		
			case 'E':
				smcmr1_p->uart.f.PEN=0x1; /*parity enabled*/
				smcmr1_p->uart.f.PM=0x1; /*parity even*/
				clen += 1;
			break;
			case 'O':
				smcmr1_p->uart.f.PEN=0x1; /*parity enabled*/
				smcmr1_p->uart.f.PM=0x0; /*parity odd*/
				clen += 1;
	
			break;
			case 'N':	//None
			case 'M':	//Mark - ignored, results in default (i.e. none)
			case 'S':	//Space - ignored, results in default (i.e. none)
			default:
				smcmr1_p->uart.f.PEN=0x0; /*parity not enabled*/
			break;
		}
		smcmr1_p->uart.f.CLEN=clen-1;
	}


//18. Write 0x4823 to SMCMR to enable the SMC transmitter and receiver. This
//    additional write ensures that the TEN and REN bits are enabled last.


	smcmr1_p->uart.f.TEN=0x1; 
	smcmr1_p->uart.f.REN=0x1; 


/*
29.2.4.2 SMC Transmitter Shortcut Sequence
This shorter sequence reinitializes transmit parameters to the state they had after reset.
   1. Clear SMCMR[TEN].
   2. Make any changes, then issue an INIT TX PARAMETERS command.
   3. Set SMCMR[TEN].


	
*/	

	{	
		cpcr_t cpcr;
		int i=0;
		for (i=0;i<1;i++){
			bsleep(10000);
			smcmr1_p->uart.f.TEN=0; 
			strcpy(TX_BD_BUFFER,test_str);
			TxBD->BD_Length = strlen(test_str);
			cpcr.raw=0;
			cpcr.f.OPCODE=INIT_TX_PARAMS;
			cpcr.f.CH_NUM=CHAN_SMC1;
			TxBD->BD_Status.SMC_TX.f.R_Ready=1;
			//smcmr1_p->uart.f.DM=1; /* Local loopback mode */
			//smcmr1_p->uart.f.DM=2; /* Echo mode */
			//TxBD->BD_Status.SCC_TX.f.CM_Continuous_mode=1;
			*((cpcr_t *)&CPCR)=cpcr;
			//CPCR = 0x0091;
			while (((cpcr_t*)&CPCR)->f.FLG);
			smcmr1_p->uart.f.TEN=1; 			
		}
	}
	return 0; // i.e. OK
}

/* Simple reads and writes - ingore fancy stuff like IRQ handling and thread support for this example */

int console_write(const char* buffer, int buff_len){
	bd_smc_t *TxBD=(bd_smc_t *)(DP_BD_0+8);
	cpcr_t *cpcr_p = (cpcr_t*)&CPCR;
	smcmr_t *smcmr1_p = (smcmr_t*)&SMCMR1;
	smce_smcm_t *smce1=(smce_smcm_t *)&SMCE1;
	smce_smcm_t *smcm1=(smce_smcm_t *)&SMCM1;
	cpcr_t cpcr;
	int wcount=0;

	cicr_t *cicr_p = (cicr_t*)&CICR;
	civr_t *civr_p = (civr_t*)&CIVR;

	ciid_t *cipr_p = (ciid_t*)&CIPR;
	ciid_t *cimr_p = (ciid_t*)&CIMR;
	ciid_t *cisr_p = (ciid_t*)&CISR;

	if (buff_len <= SMC_BD_TX_BUFFLEN){
		while (TxBD->BD_Status.SMC_TX.f.R_Ready) 	//Wait for pending write to finish
			usleep(1000);

		
		while (!smce1->f.TX && wcount<100000)		//Wait for FIFO to finish
			wcount++;				//Sometimes TX is never set (FIXME: BUG)

		TxBD->BD_Length = buff_len;
		memcpy(TX_BD_BUFFER,buffer,buff_len);

		while (((cpcr_t*)&CPCR)->f.FLG);		//Wait for any pending CP commands to finish
		cpcr.raw=0;
		cpcr.f.OPCODE=STOP_TX;				//Stop transmitting, but finish what's in FIFO
		cpcr.f.CH_NUM=CHAN_SMC1;
		*((cpcr_t *)&CPCR)=cpcr;			//Actuate the command

		//Do not do this... transmitter killed before bits in FIFO has reached the output it seems
		//Use the command below instead
		//smcmr1_p->uart.f.TEN=0; 	 		//inhibit further output
		
		
		while (((cpcr_t*)&CPCR)->f.FLG);		//Wait for any pending CP commands to finish
		cpcr.raw=0;
		cpcr.f.OPCODE=INIT_TX_PARAMS;
		cpcr.f.CH_NUM=CHAN_SMC1;
		TxBD->BD_Status.SMC_TX.f.R_Ready=1;

		*((cpcr_t *)&CPCR)=cpcr;			//Actuate the command


		//SMCE1 = 0xFF; // Write 0xFF to the SMCE register to clear any previous events.	
		//SMCM1 = 0x17; // Write 0x17 to the SMCM register to enable all possible SMC interrupts.

		SMCE1 = 0x02; //Set Tx bit - i.e. acknolage the interrupt before it happens (has no effect?)

		//Don't do this - Same reason as above
		//smcmr1_p->uart.f.TEN=1; 			//Enable the Tx output
	}else{
	}
	return buff_len;
}
/*!
Has a busy wait loop. Never ever use this from a normal program. Only to be used by the kernel internally.
*/
int __tk_console_write_emergency(const char* buffer, int buff_len){
	bd_smc_t *TxBD=(bd_smc_t *)(DP_BD_0+8);
	cpcr_t *cpcr_p = (cpcr_t*)&CPCR;
	smcmr_t *smcmr1_p = (smcmr_t*)&SMCMR1;
	smce_smcm_t *smce1=(smce_smcm_t *)&SMCE1;
	//smce_smcm_t *smcm1=(smce_smcm_t *)&SMCM1;
	cpcr_t cpcr;
	int wcount=0;

	if (buff_len <= SMC_BD_TX_BUFFLEN){
		while (TxBD->BD_Status.SMC_TX.f.R_Ready); 	//Wait for pending write to finish
		
		while (!smce1->f.TX && wcount<100000)		//Wait for FIFO to finish
			wcount++;				//Sometimes TX is never set (FIXME: BUG)

		TxBD->BD_Length = buff_len;
		memcpy(TX_BD_BUFFER,buffer,buff_len);

		while (((cpcr_t*)&CPCR)->f.FLG);		//Wait for any pending CP commands to finish
		cpcr.raw=0;
		cpcr.f.OPCODE=STOP_TX;				//Stop transmitting, but finish what's in FIFO
		cpcr.f.CH_NUM=CHAN_SMC1;
		*((cpcr_t *)&CPCR)=cpcr;			//Actuate the command

		//Do not do this... transmitter killed before bits in FIFO has reached the output it seems
		//Use the command below instead
		//smcmr1_p->uart.f.TEN=0; 	 		//inhibit further output
		
		
		while (((cpcr_t*)&CPCR)->f.FLG);		//Wait for any pending CP commands to finish
		cpcr.raw=0;
		cpcr.f.OPCODE=INIT_TX_PARAMS;
		cpcr.f.CH_NUM=CHAN_SMC1;
		TxBD->BD_Status.SMC_TX.f.R_Ready=1;

		*((cpcr_t *)&CPCR)=cpcr;			//Actuate the command


		SMCE1 = 0xFF; // Write 0xFF to the SMCE register to clear any previous events.	
		SMCM1 = 0x17; // Write 0x17 to the SMCM register to enable all possible SMC interrupts.

		//Don't do this - Same reason as above
		//smcmr1_p->uart.f.TEN=1; 			//Enable the Tx output
	}else{
	}
	return buff_len;
}

int console_read(char* buffer, int max_len){
	int ret_len;
	int total_len;
	assert(_con_stdio_init);
	sem_wait(&_con_rx_sem);

	if(rx_inIdx>rx_outIdx){
		total_len = rx_inIdx-rx_outIdx;
		if (total_len<=max_len){
			memcpy(buffer,&RX_CHAR_BUFFER[rx_outIdx],total_len);
			ret_len=total_len;
			rx_outIdx += ret_len;
			rx_outIdx %= SMC_CHAR_BUFFLEN;
			buffer[ret_len] = 0;				//This buffer is read to completion
			return ret_len;
		}else{
			memcpy(buffer,&RX_CHAR_BUFFER[rx_outIdx],max_len);
			ret_len=max_len;
			rx_outIdx += ret_len;
			rx_outIdx %= SMC_CHAR_BUFFLEN;
			return ret_len;
		}
		
	}

	return 0;
}

int console_stdio_init(){
	_con_stdio_init = 1;
	assure(sem_init(&_con_rx_sem, 0, 0) == 0);
}

//01010010
//11110010

//#define FAST_RX 1
#define RX_LED_DEBUG 1
/*
29.2.4.4 SMC Receiver Shortcut Sequence
This shorter sequence reinitializes receive parameters to their state after reset.
   1. Clear SMCMR[REN].
   2. Make any changes, then issue an INIT RX PARAMETERS command.
   3. Set SMCMR[REN].

*/
void _console_rx_handler(){
	bd_smc_t *RxBD=(bd_smc_t *)DP_BD_0;
	cpcr_t *cpcr_p = (cpcr_t*)&CPCR;	
	smcmr_t *smcmr1_p = (smcmr_t*)&SMCMR1;
	cpcr_t cpcr;
	int i,len,gotCR=0;
	static ledstat = 0;

	#ifndef FAST_RX
		smcmr1_p->uart.f.REN=0; 
	#endif

	len = RxBD->BD_Length;
	memcpy(&RX_CHAR_BUFFER[rx_inIdx],RX_BD_BUFFER,len);	
	RxBD->BD_Status.SMC_RX.f.E_Empty = 1;		//This @#! wasn't documented! ;( Same as 'RxBD->BD_Status.raw = 0xB000;'

	rx_inIdx+=len;
	rx_inIdx %= SMC_CHAR_BUFFLEN;

	#ifndef FAST_RX
		while (((cpcr_t*)&CPCR)->f.FLG);		//Wait for any pending CP commands to finish
	
		cpcr.raw=0;
		cpcr.f.OPCODE=INIT_RX_PARAMS;
		//cpcr.f.OPCODE=CLOSE_RX_BD;
		cpcr.f.CH_NUM=CHAN_SMC1;
		*((cpcr_t *)&CPCR)=cpcr;			//Actuate the command
	#endif

	
	for (i=0; i<len && !gotCR; i++){
		if (RX_BD_BUFFER[i] == '\r')
			gotCR = 1;
	}

	#ifndef FAST_RX
		smcmr1_p->uart.f.REN=1; 
	#endif
	//console_write(RX_BD_BUFFER,len); //For local echo. Bad idea - only use for debugging
	#ifdef RX_LED_DEBUG
		#include <board/ESC/led.h>
		if (!ledstat){
			ledstat=1;
			led_on(LED_1, LED_GREEN);
		}else{
			ledstat=0;
			led_off(LED_1, LED_GREEN);
		}
	#endif
	if (gotCR)
		sem_post(&_con_rx_sem);

};

void _console_tx_handler(){
	assure("Console BD handles TX (ehh?) - Config fekkup?" == 0);
};

void _console_bsy_handler(){
	assure("Console BD is busy - input to fast?" == 0);
};

void _console_brk_handler(){
	assure("Console BD detected brk - Config fekkup?" == 0);
};

void _console_brke_handler(){
	assure("Console BD detected brke - Config fekkup?" == 0);
};

