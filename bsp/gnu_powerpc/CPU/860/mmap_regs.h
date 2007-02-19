// Generated from the file [reg860_ESC.def] at [Tue Feb  6 21:32:44 CET 2007]
// Script def2mmap.sh by Michael Ambrus, <michael.ambrus@gmail.com> (c) 2007

#ifndef MMAP_REGS_H
#define MMAP_REGS_H
#include <sys/types.h>

#define DMM1    0xFA200000


// Memory mapped registers follows...

#define SIUMCR (*(volatile __uint32_t *)(DMM1+0x000))
#define SYPCR (*(volatile __uint32_t *)(DMM1+0x004))
#define SWSR (*(volatile __uint16_t *)(DMM1+0x00e))
#define SIPEND (*(volatile __uint32_t *)(DMM1+0x010))
#define SIMASK (*(volatile __uint32_t *)(DMM1+0x014))
#define SIEL (*(volatile __uint32_t *)(DMM1+0x018))
#define SIVEC (*(volatile __uint32_t *)(DMM1+0x01c))
#define TESR (*(volatile __uint32_t *)(DMM1+0x020))
#define SDCR (*(volatile __uint32_t *)(DMM1+0x030))
#define PBR0 (*(volatile __uint32_t *)(DMM1+0x080))
#define POR0 (*(volatile __uint32_t *)(DMM1+0x084))
#define PBR1 (*(volatile __uint32_t *)(DMM1+0x088))
#define POR1 (*(volatile __uint32_t *)(DMM1+0x08c))
#define PBR2 (*(volatile __uint32_t *)(DMM1+0x090))
#define POR2 (*(volatile __uint32_t *)(DMM1+0x094))
#define PBR3 (*(volatile __uint32_t *)(DMM1+0x098))
#define POR3 (*(volatile __uint32_t *)(DMM1+0x09c))
#define PBR4 (*(volatile __uint32_t *)(DMM1+0x0a0))
#define POR4 (*(volatile __uint32_t *)(DMM1+0x0a4))
#define PBR5 (*(volatile __uint32_t *)(DMM1+0x0a8))
#define POR5 (*(volatile __uint32_t *)(DMM1+0x0ac))
#define PBR6 (*(volatile __uint32_t *)(DMM1+0x0b0))
#define POR6 (*(volatile __uint32_t *)(DMM1+0x0b4))
#define PBR7 (*(volatile __uint32_t *)(DMM1+0x0b8))
#define POR7 (*(volatile __uint32_t *)(DMM1+0x0bc))
#define PGCRA (*(volatile __uint32_t *)(DMM1+0x0e0))
#define PGCRB (*(volatile __uint32_t *)(DMM1+0x0e4))
#define PSCR (*(volatile __uint32_t *)(DMM1+0x0e8))
#define PIPR (*(volatile __uint32_t *)(DMM1+0x0f0))
#define PER (*(volatile __uint32_t *)(DMM1+0x0f8))
#define BR0 (*(volatile __uint32_t *)(DMM1+0x100))
#define OR0 (*(volatile __uint32_t *)(DMM1+0x104))
#define BR1 (*(volatile __uint32_t *)(DMM1+0x108))
#define OR1 (*(volatile __uint32_t *)(DMM1+0x10c))
#define BR2 (*(volatile __uint32_t *)(DMM1+0x110))
#define OR2 (*(volatile __uint32_t *)(DMM1+0x114))
#define BR3 (*(volatile __uint32_t *)(DMM1+0x118))
#define OR3 (*(volatile __uint32_t *)(DMM1+0x11c))
#define BR4 (*(volatile __uint32_t *)(DMM1+0x120))
#define OR4 (*(volatile __uint32_t *)(DMM1+0x124))
#define BR5 (*(volatile __uint32_t *)(DMM1+0x128))
#define OR5 (*(volatile __uint32_t *)(DMM1+0x12c))
#define BR6 (*(volatile __uint32_t *)(DMM1+0x130))
#define OR6 (*(volatile __uint32_t *)(DMM1+0x134))
#define BR7 (*(volatile __uint32_t *)(DMM1+0x138))
#define OR7 (*(volatile __uint32_t *)(DMM1+0x13c))
#define MAR (*(volatile __uint32_t *)(DMM1+0x164))
#define MCR (*(volatile __uint32_t *)(DMM1+0x168))
#define MAMR (*(volatile __uint32_t *)(DMM1+0x170))
#define MBMR (*(volatile __uint32_t *)(DMM1+0x174))
#define MSTAT (*(volatile __uint16_t *)(DMM1+0x178))
#define MPTPR (*(volatile __uint16_t *)(DMM1+0x17a))
#define MDR (*(volatile __uint32_t *)(DMM1+0x17c))
#define TBSCR (*(volatile __uint16_t *)(DMM1+0x200))
#define TBREFF0 (*(volatile __uint32_t *)(DMM1+0x204))
#define TBREFF1 (*(volatile __uint32_t *)(DMM1+0x208))
#define RTCSC (*(volatile __uint16_t *)(DMM1+0x220))
#define RTC (*(volatile __uint32_t *)(DMM1+0x224))
#define RTSEC (*(volatile __uint32_t *)(DMM1+0x228))
#define RTCAL (*(volatile __uint32_t *)(DMM1+0x22c))
#define PISCR (*(volatile __uint16_t *)(DMM1+0x240))
#define PITC (*(volatile __uint32_t *)(DMM1+0x244))
#define PITR (*(volatile __uint32_t *)(DMM1+0x248))
#define SCCR (*(volatile __uint32_t *)(DMM1+0x280))
#define PLPRCR (*(volatile __uint32_t *)(DMM1+0x284))
#define RSR (*(volatile __uint32_t *)(DMM1+0x288))
#define TBSCRK (*(volatile __uint32_t *)(DMM1+0x300))
#define TBREFF0K (*(volatile __uint32_t *)(DMM1+0x304))
#define TBREFF1K (*(volatile __uint32_t *)(DMM1+0x308))
#define TBK (*(volatile __uint32_t *)(DMM1+0x30c))
#define RTCSCK (*(volatile __uint32_t *)(DMM1+0x320))
#define RTCK (*(volatile __uint32_t *)(DMM1+0x324))
#define RTSECK (*(volatile __uint32_t *)(DMM1+0x328))
#define RTCALK (*(volatile __uint32_t *)(DMM1+0x32c))
#define PISCRK (*(volatile __uint32_t *)(DMM1+0x340))
#define PITCK (*(volatile __uint32_t *)(DMM1+0x344))
#define SCCRK (*(volatile __uint32_t *)(DMM1+0x380))
#define PLPRCRK (*(volatile __uint32_t *)(DMM1+0x384))
#define RSRK (*(volatile __uint32_t *)(DMM1+0x388))
#define I2MOD (*(volatile __uint8_t *)(DMM1+0x860))
#define I2ADD (*(volatile __uint8_t *)(DMM1+0x864))
#define I2BRG (*(volatile __uint8_t *)(DMM1+0x868))
#define I2COM (*(volatile __uint8_t *)(DMM1+0x86c))
#define I2CER (*(volatile __uint8_t *)(DMM1+0x870))
#define I2CMR (*(volatile __uint8_t *)(DMM1+0x874))
#define SDAR (*(volatile __uint32_t *)(DMM1+0x904))
#define SDSR (*(volatile __uint8_t *)(DMM1+0x908))
#define SDMR (*(volatile __uint8_t *)(DMM1+0x90c))
#define IDSR1 (*(volatile __uint8_t *)(DMM1+0x910))
#define IDMR1 (*(volatile __uint8_t *)(DMM1+0x914))
#define IDSR2 (*(volatile __uint8_t *)(DMM1+0x918))
#define IDMR2 (*(volatile __uint8_t *)(DMM1+0x91c))
#define CIVR (*(volatile __uint16_t *)(DMM1+0x930))
#define CICR (*(volatile __uint32_t *)(DMM1+0x940))
#define CIPR (*(volatile __uint32_t *)(DMM1+0x944))
#define CIMR (*(volatile __uint32_t *)(DMM1+0x948))
#define CISR (*(volatile __uint32_t *)(DMM1+0x94c))
#define PADIR (*(volatile __uint16_t *)(DMM1+0x950))
#define PAPAR (*(volatile __uint16_t *)(DMM1+0x952))
#define PAODR (*(volatile __uint16_t *)(DMM1+0x954))
#define PADAT (*(volatile __uint16_t *)(DMM1+0x956))
#define PCDIR (*(volatile __uint16_t *)(DMM1+0x960))
#define PCPAR (*(volatile __uint16_t *)(DMM1+0x962))
#define PCSO (*(volatile __uint16_t *)(DMM1+0x964))
#define PCDAT (*(volatile __uint16_t *)(DMM1+0x966))
#define PCINT (*(volatile __uint16_t *)(DMM1+0x968))
#define PDDIR (*(volatile __uint16_t *)(DMM1+0x970))
#define PDPAR (*(volatile __uint16_t *)(DMM1+0x972))
#define PDDAT (*(volatile __uint16_t *)(DMM1+0x976))
#define TGCR (*(volatile __uint16_t *)(DMM1+0x980))
#define TMR1 (*(volatile __uint16_t *)(DMM1+0x990))
#define TMR2 (*(volatile __uint16_t *)(DMM1+0x992))
#define TRR1 (*(volatile __uint16_t *)(DMM1+0x994))
#define TRR2 (*(volatile __uint16_t *)(DMM1+0x996))
#define TCR1 (*(volatile __uint16_t *)(DMM1+0x998))
#define TCR2 (*(volatile __uint16_t *)(DMM1+0x99a))
#define TCN1 (*(volatile __uint16_t *)(DMM1+0x99c))
#define TCN2 (*(volatile __uint16_t *)(DMM1+0x99e))
#define TMR3 (*(volatile __uint16_t *)(DMM1+0x9a0))
#define TMR4 (*(volatile __uint16_t *)(DMM1+0x9a2))
#define TRR3 (*(volatile __uint16_t *)(DMM1+0x9a4))
#define TRR4 (*(volatile __uint16_t *)(DMM1+0x9a6))
#define TCR3 (*(volatile __uint16_t *)(DMM1+0x9a8))
#define TCR4 (*(volatile __uint16_t *)(DMM1+0x9aa))
#define TCN3 (*(volatile __uint16_t *)(DMM1+0x9ac))
#define TCN4 (*(volatile __uint16_t *)(DMM1+0x9ae))
#define TER1 (*(volatile __uint16_t *)(DMM1+0x9b0))
#define TER2 (*(volatile __uint16_t *)(DMM1+0x9b2))
#define TER3 (*(volatile __uint16_t *)(DMM1+0x9b4))
#define TER4 (*(volatile __uint16_t *)(DMM1+0x9b6))
#define CPCR (*(volatile __uint16_t *)(DMM1+0x9c0))
#define RCCR (*(volatile __uint16_t *)(DMM1+0x9c4))
#define RES (*(volatile __uint8_t *)(DMM1+0x9c6))
#define RMDS (*(volatile __uint8_t *)(DMM1+0x9c7))
#define RMDR (*(volatile __uint32_t *)(DMM1+0x9c8))
#define RCTR1 (*(volatile __uint16_t *)(DMM1+0x9cc))
#define RCTR2 (*(volatile __uint16_t *)(DMM1+0x9ce))
#define RCTR3 (*(volatile __uint16_t *)(DMM1+0x9d0))
#define RCTR4 (*(volatile __uint16_t *)(DMM1+0x9d2))
#define RTER (*(volatile __uint16_t *)(DMM1+0x9d6))
#define RTMR (*(volatile __uint16_t *)(DMM1+0x9da))
#define BRGC1 (*(volatile __uint32_t *)(DMM1+0x9f0))
#define BRGC2 (*(volatile __uint32_t *)(DMM1+0x9f4))
#define BRGC3 (*(volatile __uint32_t *)(DMM1+0x9f8))
#define BRGC4 (*(volatile __uint32_t *)(DMM1+0x9fc))
#define GSMR_L1 (*(volatile __uint32_t *)(DMM1+0xa00))
#define GSMR_H1 (*(volatile __uint32_t *)(DMM1+0xa04))
#define PSMR1 (*(volatile __uint16_t *)(DMM1+0xa08))
#define TODR1 (*(volatile __uint16_t *)(DMM1+0xa0c))
#define DSR1 (*(volatile __uint16_t *)(DMM1+0xa0e))
#define SCCE1 (*(volatile __uint16_t *)(DMM1+0xa10))
#define SCCM1 (*(volatile __uint16_t *)(DMM1+0xa14))
#define SCCS1 (*(volatile __uint8_t *)(DMM1+0xa17))
#define GSMR_L2 (*(volatile __uint32_t *)(DMM1+0xa20))
#define GSMR_H2 (*(volatile __uint32_t *)(DMM1+0xa24))
#define PSMR2 (*(volatile __uint16_t *)(DMM1+0xa28))
#define TODR2 (*(volatile __uint16_t *)(DMM1+0xa2c))
#define DSR2 (*(volatile __uint16_t *)(DMM1+0xa2e))
#define SCCE2 (*(volatile __uint16_t *)(DMM1+0xa30))
#define SCCM2 (*(volatile __uint16_t *)(DMM1+0xa34))
#define SCCS2 (*(volatile __uint8_t *)(DMM1+0xa37))
#define GSMR_L3 (*(volatile __uint32_t *)(DMM1+0xa40))
#define GSMR_H3 (*(volatile __uint32_t *)(DMM1+0xa44))
#define PSMR3 (*(volatile __uint16_t *)(DMM1+0xa48))
#define TODR3 (*(volatile __uint16_t *)(DMM1+0xa4c))
#define DSR3 (*(volatile __uint16_t *)(DMM1+0xa4e))
#define SCCE3 (*(volatile __uint16_t *)(DMM1+0xa50))
#define SCCM3 (*(volatile __uint16_t *)(DMM1+0xa54))
#define SCCS3 (*(volatile __uint8_t *)(DMM1+0xa57))
#define GSMR_L4 (*(volatile __uint32_t *)(DMM1+0xa60))
#define GSMR_H4 (*(volatile __uint32_t *)(DMM1+0xa64))
#define PSMR4 (*(volatile __uint16_t *)(DMM1+0xa68))
#define TODR4 (*(volatile __uint16_t *)(DMM1+0xa6c))
#define DSR4 (*(volatile __uint16_t *)(DMM1+0xa6e))
#define SCCE4 (*(volatile __uint16_t *)(DMM1+0xa70))
#define SCCM4 (*(volatile __uint16_t *)(DMM1+0xa74))
#define SCCS4 (*(volatile __uint8_t *)(DMM1+0xa77))
#define SMCMR1 (*(volatile __uint16_t *)(DMM1+0xa82))
#define SMCE1 (*(volatile __uint8_t *)(DMM1+0xa86))
#define SMCM1 (*(volatile __uint8_t *)(DMM1+0xa8a))
#define SMCMR2 (*(volatile __uint16_t *)(DMM1+0xa92))
#define SMCE2 (*(volatile __uint8_t *)(DMM1+0xa96))
#define SMCM2 (*(volatile __uint8_t *)(DMM1+0xa9a))
#define SPMODE (*(volatile __uint16_t *)(DMM1+0xaa0))
#define SPIE (*(volatile __uint8_t *)(DMM1+0xaa6))
#define SPIM (*(volatile __uint8_t *)(DMM1+0xaaa))
#define SPCOM (*(volatile __uint8_t *)(DMM1+0xaad))
#define PIPC (*(volatile __uint16_t *)(DMM1+0xab2))
#define PTPR (*(volatile __uint16_t *)(DMM1+0xab6))
#define PBDIR (*(volatile __uint32_t *)(DMM1+0xab8))
#define PBPAR (*(volatile __uint32_t *)(DMM1+0xabc))
#define PBODR (*(volatile __uint16_t *)(DMM1+0xac2))
#define PBDAT (*(volatile __uint32_t *)(DMM1+0xac4))
#define SIMODE (*(volatile __uint32_t *)(DMM1+0xae0))
#define SIGMR (*(volatile __uint8_t *)(DMM1+0xae4))
#define SISTR (*(volatile __uint8_t *)(DMM1+0xae6))
#define SICMR (*(volatile __uint8_t *)(DMM1+0xae7))
#define SICR (*(volatile __uint32_t *)(DMM1+0xaec))
#define SIRP (*(volatile __uint32_t *)(DMM1+0xaf0))

// Special Purpose Register ID's (SPR) follows...
// (to be used with SET_SPR and GET_SPR macros)

#define _PC 26
#define _XER 1
#define _LR 8
#define _CTR 9
#define _DSISR 18
#define _DAR 19
#define _DEC 22
#define _SRR0 26
#define _SRR1 27
#define _CMPA 144
#define _CMPB 145
#define _CMPC 146
#define _CMPD 147
#define _ICR 148
#define _DER 149
#define _COUNTA 150
#define _COUNTB 151
#define _CMPE 152
#define _CMPF 153
#define _CMPG 154
#define _CMPH 155
#define _LCTRL1 156
#define _LCTRL2 157
#define _ICTRL 158
#define _BAR 159
#define _TBL 268
#define _TBU 269
#define _SPRG0 272
#define _SPRG1 273
#define _SPRG2 274
#define _SPRG3 275
#define _PVR 287
#define _IMMR 638
#define _IC_CST 560
#define _IC_ADR 561
#define _IC_DAT 562
#define _DC_CST 568
#define _DC_ADR 569
#define _DC_DAT 570
#define _MI_CTR 784
#define _MI_AP 786
#define _MI_EPN 787
#define _MI_TWC 789
#define _MI_RPN 790
#define _MD_CTR 792
#define _M_CASID 793
#define _MD_AP 794
#define _MD_EPN 795
#define _M_TWB 796
#define _MD_TWC 797
#define _MD_RPN 798
#define _M_TW 799
#define _MI_DBCAM 816
#define _MI_DBRAM0 817
#define _MI_DBRAM1 818
#define _MD_DBCAM 824
#define _MD_DBRAM0 825
#define _MD_DBRAM1 826
#define _EIE 80
#define _EID 81
#define _NRI 82
#define _DPIR 631
#define _MI_CAM 816
#define _MI_RAM0 817
#define _MI_RAM1 818
#define _MD_CAM 824
#define _MD_RAM0 825
#define _MD_RAM1 826
#define _DPDR 630

#endif //MMAP_REGS_H
