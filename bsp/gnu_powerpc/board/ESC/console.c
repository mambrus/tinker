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
#include "hwboard.h"

#include <string.h>

#define RX_BUFFLEN	160
#define TX_BUFFLEN	160
#define CHAN_SMC1 	0x09

static char RX_BUFFER[RX_BUFFLEN];
static char TX_BUFFER[TX_BUFFLEN];

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
The function should be able to figure out BRG[CD] divisor settings by itself based on
the OSCCLK constant and PLPRCR[MF] & SCCR[DFBRG] setting


               VCOOUT freq
BRGCLK freq = ---------------------------------------
                                                    -
                    2 × DFBRG
              (2                                    )

*/
void console_init(int bpr, int nr_bits, int par, int nr_stop){
//Clean the buffers
	{
		char *RxTemp=RX_BUFFER;
		char *TxTemp=TX_BUFFER;
		int i;
		for (i=0;i<RX_BUFFLEN;i++){
			RX_BUFFER[i]='R';
		}
		for (i=0;i<TX_BUFFLEN;i++){
			TX_BUFFER[i]='T';
		}
	}
	cpcr_t *cpcr_p = (cpcr_t*)&CPCR;
	smcmr_t *smcmr1_p = (smcmr_t*)&SMCMR1;
	brgc_t *brgc1_p = (brgc_t*)&BRGC1;
	sccr_t *sccr_p = (sccr_t*)&SCCR;
	plprcr_t *plprcr_p = (plprcr_t*)&PLPRCR;

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
	bitclear(SIMODE,16);  //NMSI - I.e. dedicated pin, no multiplex 
	bitclear(SIMODE,17); //Clock source is BRG1 FIXME: this is 3 bits long 
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

	smc_param->RFCR.raw = 0x10;
	smc_param->TFCR.raw = 0x10;


//8. Write MRBLR with the maximum number of bytes per receive buffer. Assume 16
//	bytes, so MRBLR = 0x0010.

	smc_param->MRBLR = 0x0010;


//9. Write MAX_IDL with 0x0000 in the SMC UART-speciﬁc parameter RAM to
//    disable the MAX_IDL functionality for this example.
	
	smc_param->PROTO.SMC_UART.MAX_IDL=0x0000;


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
	RxBD->BD_Pointer = (__uint32_t)RX_BUFFER;
	//RxBD->BD_Pointer = 0x10000000;

//13. Assuming the Tx buffer is at 0x00002000 in main memory and contains ﬁve 8-bit
//    characters, write 0xB000 to Tx_BD_Status, 0x0005 to Tx_BD_Length, and
//    0x00002000 to Tx_BD_Pointer.

	TxBD->BD_Status.raw = 0xB000;
	//TxBD->BD_Length = TX_BUFFLEN;
	TxBD->BD_Length = 0x0005;
	TxBD->BD_Pointer = (__uint32_t)TX_BUFFER;
	//TxBD->BD_Pointer = 0x100000a0;

//14. Write 0xFF to the SMCE register to clear any previous events.	
	SMCE1 = 0xFF;

//15. Write 0x17 to the SMCM register to enable all possible SMC interrupts.

	SMCM1 = 0x17;

//16. Write 0x00000010 to the CIMR so the SMC1 can generate a system interrupt.
//    Initialize the CICR.
	CIMR = 0x00000010;

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
			strcpy(TX_BUFFER,test_str);
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
}

/* Simple reads and writes - ingore fancy stuff like IRQ handling and thread support for this example */

void console_write(const char* buffer, int buff_len){
	bd_smc_t *TxBD=(bd_smc_t *)(DP_BD_0+8);
	cpcr_t *cpcr_p = (cpcr_t*)&CPCR;
	smcmr_t *smcmr1_p = (smcmr_t*)&SMCMR1;
	smce_smcm_t *smce1=(smce_smcm_t *)&SMCE1;
	//smce_smcm_t *smcm1=(smce_smcm_t *)&SMCM1;
	cpcr_t cpcr;
	int wcount=0;

	if (buff_len <= TX_BUFFLEN){
		while (TxBD->BD_Status.SMC_TX.f.R_Ready); 	//Wait for pending write to finish

		
		while (!smce1->f.TX && wcount<100000)		//Wait for FIFO to finish
			wcount++;				//Sometimes TX is never set (FIXME: BUG)

		TxBD->BD_Length = buff_len;
		memcpy(TX_BUFFER,buffer,buff_len);

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
}

void console_read(char* buffer, int max_len){
	bd_smc_t *RxBD=(bd_smc_t *)DP_BD_0;
	cpcr_t *cpcr_p = (cpcr_t*)&CPCR;
	smcmr_t *smcmr1_p = (smcmr_t*)&SMCMR1;
	cpcr_t cpcr;

}


