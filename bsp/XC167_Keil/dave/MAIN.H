//****************************************************************************
// @Module        Project Settings
// @Filename      MAIN.H
// @Project       Dave.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC167CI-16F40
//
// @Compiler      Keil
//
// @Codegenerator 2.4
//
// @Description   This file contains all function prototypes and macros for 
//                the MAIN module.
//
//----------------------------------------------------------------------------
// @Date          2006-03-11 15:46:47
//
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,1)

// USER CODE END



/*
******************************************************************************
@Note

Notes of EBC Module:
External RAM def added by Michael Ambrus to fit PhyCoreXC167. 

PhyCore module has 128k external RAN starting at adress 0x200000

Bus arbitration enabled

******************************************************************************
*/


#ifndef _MAIN_H_
#define _MAIN_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************
#define KEIL

// USER CODE BEGIN (MAIN_Header,4)

// USER CODE END


//****************************************************************************
// @Declaration of SFRs
//****************************************************************************


// The source and destination pointers SRCPx and DSTPx
#define SRCP0                  (*((uword volatile *) 0xEC40))
#define DSTP0                  (*((uword volatile *) 0xEC42))
#define SRCP1                  (*((uword volatile *) 0xEC44))
#define DSTP1                  (*((uword volatile *) 0xEC46))
#define SRCP2                  (*((uword volatile *) 0xEC48))
#define DSTP2                  (*((uword volatile *) 0xEC4A))
#define SRCP3                  (*((uword volatile *) 0xEC4C))
#define DSTP3                  (*((uword volatile *) 0xEC4E))
#define SRCP4                  (*((uword volatile *) 0xEC50))
#define DSTP4                  (*((uword volatile *) 0xEC52))
#define SRCP5                  (*((uword volatile *) 0xEC54))
#define DSTP5                  (*((uword volatile *) 0xEC56))
#define SRCP6                  (*((uword volatile *) 0xEC58))
#define DSTP6                  (*((uword volatile *) 0xEC5A))
#define SRCP7                  (*((uword volatile *) 0xEC5C))
#define DSTP7                  (*((uword volatile *) 0xEC5E))

// ADC End of Conversion Interrupt Control Register
#define ADC_CIC                (*((uword volatile *) 0xFF98))
#define ADC_CIC_GPX                      ((T_Reg16 *) 0xFF98)->bit8
#define ADC_CIC_IE                       ((T_Reg16 *) 0xFF98)->bit6
#define ADC_CIC_IR                       ((T_Reg16 *) 0xFF98)->bit7

// A/D Converter Control Register
#define ADC_CON                (*((uword volatile *) 0xFFA0))
#define ADC_CON_ADBSY                    ((T_Reg16 *) 0xFFA0)->bit8
#define ADC_CON_ADCIN                    ((T_Reg16 *) 0xFFA0)->bit10
#define ADC_CON_ADCRQ                    ((T_Reg16 *) 0xFFA0)->bit11
#define ADC_CON_ADST                     ((T_Reg16 *) 0xFFA0)->bit7
#define ADC_CON_ADWR                     ((T_Reg16 *) 0xFFA0)->bit9

// A/D Converter Control Register 1
#define ADC_CON1               (*((uword volatile *) 0xFFA6))
#define ADC_CON1_CAL                      ((T_Reg16 *) 0xFFA6)->bit13
#define ADC_CON1_ICST                     ((T_Reg16 *) 0xFFA6)->bit15
#define ADC_CON1_RES                      ((T_Reg16 *) 0xFFA6)->bit12
#define ADC_CON1_SAMPLE                   ((T_Reg16 *) 0xFFA6)->bit14

// A/D Converter Control Register 0
#define ADC_CTR0               (*((uword volatile *) 0xFFBE))
#define ADC_CTR0_0                        ((T_Reg16 *) 0xFFBE)->bit4
#define ADC_CTR0_ADBSY                    ((T_Reg16 *) 0xFFBE)->bit8
#define ADC_CTR0_ADCIN                    ((T_Reg16 *) 0xFFBE)->bit10
#define ADC_CTR0_ADCRQ                    ((T_Reg16 *) 0xFFBE)->bit11
#define ADC_CTR0_ADST                     ((T_Reg16 *) 0xFFBE)->bit7
#define ADC_CTR0_ADWR                     ((T_Reg16 *) 0xFFBE)->bit9
#define ADC_CTR0_MD                       ((T_Reg16 *) 0xFFBE)->bit15
#define ADC_CTR0_SMPL                     ((T_Reg16 *) 0xFFBE)->bit14

// A/D Converter Control Register 2
#define ADC_CTR2               (*((uword volatile *) 0xF09C))

// A/D Converter Injection Control Register 2
#define ADC_CTR2IN             (*((uword volatile *) 0xF09E))

// A/D Converter Result Register
#define ADC_DAT                (*((uword volatile *) 0xFEA0))

// A/D Converter Result Register 2
#define ADC_DAT2               (*((uword volatile *) 0xF0A0))

// ADC Overrun Error Control Register
#define ADC_EIC                (*((uword volatile *) 0xFF9A))
#define ADC_EIC_GPX                      ((T_Reg16 *) 0xFF9A)->bit8
#define ADC_EIC_IE                       ((T_Reg16 *) 0xFF9A)->bit6
#define ADC_EIC_IR                       ((T_Reg16 *) 0xFF9A)->bit7

// CS1 Address Range and Size Selection Register
#define ADDRSEL1               (*((uword volatile *) 0xEE1E))

// CS2 Address Range and Size Selection Register
#define ADDRSEL2               (*((uword volatile *) 0xEE26))

// CS3 Address Range and Size Selection Register
#define ADDRSEL3               (*((uword volatile *) 0xEE2E))

// CS4 Address Range and Size Selection Register
#define ADDRSEL4               (*((uword volatile *) 0xEE36))

// CS5 Address Range and Size Selection Register
#define ADDRSEL5               (*((uword volatile *) 0xEE3E))

// CS6 Address Range and Size Selection Register
#define ADDRSEL6               (*((uword volatile *) 0xEE46))

// CS7 Address Range and Size Selection Register
#define ADDRSEL7               (*((uword volatile *) 0xEE4E))

// Alternate I/O Source 0 Port P1H
#define ALTSEL0P1H             (*((uword volatile *) 0xF120))
#define ALTSEL0P1H_P0                       ((T_Reg16 *) 0xF120)->bit0
#define ALTSEL0P1H_P1                       ((T_Reg16 *) 0xF120)->bit1
#define ALTSEL0P1H_P2                       ((T_Reg16 *) 0xF120)->bit2
#define ALTSEL0P1H_P3                       ((T_Reg16 *) 0xF120)->bit3
#define ALTSEL0P1H_P4                       ((T_Reg16 *) 0xF120)->bit4
#define ALTSEL0P1H_P5                       ((T_Reg16 *) 0xF120)->bit5
#define ALTSEL0P1H_P6                       ((T_Reg16 *) 0xF120)->bit6
#define ALTSEL0P1H_P7                       ((T_Reg16 *) 0xF120)->bit7

// P1L Alternate Select Register 0
#define ALTSEL0P1L             (*((uword volatile *) 0xF130))
#define ALTSEL0P1L_P0                       ((T_Reg16 *) 0xF130)->bit0
#define ALTSEL0P1L_P1                       ((T_Reg16 *) 0xF130)->bit1
#define ALTSEL0P1L_P2                       ((T_Reg16 *) 0xF130)->bit2
#define ALTSEL0P1L_P3                       ((T_Reg16 *) 0xF130)->bit3
#define ALTSEL0P1L_P4                       ((T_Reg16 *) 0xF130)->bit4
#define ALTSEL0P1L_P5                       ((T_Reg16 *) 0xF130)->bit5
#define ALTSEL0P1L_P6                       ((T_Reg16 *) 0xF130)->bit6
#define ALTSEL0P1L_P7                       ((T_Reg16 *) 0xF130)->bit7

// Alternate I/O Source 0 Port P2
#define ALTSEL0P2              (*((uword volatile *) 0xF122))
#define ALTSEL0P2_P10                      ((T_Reg16 *) 0xF122)->bit10
#define ALTSEL0P2_P11                      ((T_Reg16 *) 0xF122)->bit11
#define ALTSEL0P2_P12                      ((T_Reg16 *) 0xF122)->bit12
#define ALTSEL0P2_P13                      ((T_Reg16 *) 0xF122)->bit13
#define ALTSEL0P2_P14                      ((T_Reg16 *) 0xF122)->bit14
#define ALTSEL0P2_P15                      ((T_Reg16 *) 0xF122)->bit15
#define ALTSEL0P2_P8                       ((T_Reg16 *) 0xF122)->bit8
#define ALTSEL0P2_P9                       ((T_Reg16 *) 0xF122)->bit9

// Alternate I/O Source Port 3 Selection
#define ALTSEL0P3              (*((uword volatile *) 0xF126))
#define ALTSEL0P3_P0                       ((T_Reg16 *) 0xF126)->bit0
#define ALTSEL0P3_P1                       ((T_Reg16 *) 0xF126)->bit1
#define ALTSEL0P3_P10                      ((T_Reg16 *) 0xF126)->bit10
#define ALTSEL0P3_P11                      ((T_Reg16 *) 0xF126)->bit11
#define ALTSEL0P3_P13                      ((T_Reg16 *) 0xF126)->bit13
#define ALTSEL0P3_P3                       ((T_Reg16 *) 0xF126)->bit3
#define ALTSEL0P3_P8                       ((T_Reg16 *) 0xF126)->bit8
#define ALTSEL0P3_P9                       ((T_Reg16 *) 0xF126)->bit9

// Alternate I/O Source 0 Port P4
#define ALTSEL0P4              (*((uword volatile *) 0xF12A))
#define ALTSEL0P4_P6                       ((T_Reg16 *) 0xF12A)->bit6
#define ALTSEL0P4_P7                       ((T_Reg16 *) 0xF12A)->bit7

// Alternate I/O Source 0 Port P6
#define ALTSEL0P6              (*((uword volatile *) 0xF12C))
#define ALTSEL0P6_P0                       ((T_Reg16 *) 0xF12C)->bit0
#define ALTSEL0P6_P1                       ((T_Reg16 *) 0xF12C)->bit1
#define ALTSEL0P6_P2                       ((T_Reg16 *) 0xF12C)->bit2
#define ALTSEL0P6_P3                       ((T_Reg16 *) 0xF12C)->bit3
#define ALTSEL0P6_P4                       ((T_Reg16 *) 0xF12C)->bit4
#define ALTSEL0P6_P5                       ((T_Reg16 *) 0xF12C)->bit5
#define ALTSEL0P6_P6                       ((T_Reg16 *) 0xF12C)->bit6
#define ALTSEL0P6_P7                       ((T_Reg16 *) 0xF12C)->bit7

// Alternate I/O Source 0 Port P7
#define ALTSEL0P7              (*((uword volatile *) 0xF13C))
#define ALTSEL0P7_P4                       ((T_Reg16 *) 0xF13C)->bit4
#define ALTSEL0P7_P5                       ((T_Reg16 *) 0xF13C)->bit5
#define ALTSEL0P7_P6                       ((T_Reg16 *) 0xF13C)->bit6
#define ALTSEL0P7_P7                       ((T_Reg16 *) 0xF13C)->bit7

// Alternate I/O Source 0 Port P9
#define ALTSEL0P9              (*((uword volatile *) 0xF138))
#define ALTSEL0P9_P0                       ((T_Reg16 *) 0xF138)->bit0
#define ALTSEL0P9_P1                       ((T_Reg16 *) 0xF138)->bit1
#define ALTSEL0P9_P2                       ((T_Reg16 *) 0xF138)->bit2
#define ALTSEL0P9_P3                       ((T_Reg16 *) 0xF138)->bit3
#define ALTSEL0P9_P4                       ((T_Reg16 *) 0xF138)->bit4
#define ALTSEL0P9_P5                       ((T_Reg16 *) 0xF138)->bit5

// Alternate I/O Source 1 Port P3
#define ALTSEL1P3              (*((uword volatile *) 0xF128))
#define ALTSEL1P3_P1                       ((T_Reg16 *) 0xF128)->bit1

// Alternate I/O Source 1 Port P4
#define ALTSEL1P4              (*((uword volatile *) 0xF136))
#define ALTSEL1P4_P7                       ((T_Reg16 *) 0xF136)->bit7

// Alternate I/O Source 1 Port P7
#define ALTSEL1P7              (*((uword volatile *) 0xF13E))
#define ALTSEL1P7_P4                       ((T_Reg16 *) 0xF13E)->bit4
#define ALTSEL1P7_P5                       ((T_Reg16 *) 0xF13E)->bit5
#define ALTSEL1P7_P6                       ((T_Reg16 *) 0xF13E)->bit6
#define ALTSEL1P7_P7                       ((T_Reg16 *) 0xF13E)->bit7

// Alternate I/O Source 1 Port P9
#define ALTSEL1P9              (*((uword volatile *) 0xF13A))
#define ALTSEL1P9_P0                       ((T_Reg16 *) 0xF13A)->bit0
#define ALTSEL1P9_P1                       ((T_Reg16 *) 0xF13A)->bit1
#define ALTSEL1P9_P2                       ((T_Reg16 *) 0xF13A)->bit2
#define ALTSEL1P9_P3                       ((T_Reg16 *) 0xF13A)->bit3
#define ALTSEL1P9_P4                       ((T_Reg16 *) 0xF13A)->bit4
#define ALTSEL1P9_P5                       ((T_Reg16 *) 0xF13A)->bit5

// ASC0 Autobaud Control Register
#define ASC0_ABCON             (*((uword volatile *) 0xF1B8))
#define ASC0_ABCON_ABDETEN                  ((T_Reg16 *) 0xF1B8)->bit3
#define ASC0_ABCON_ABEN                     ((T_Reg16 *) 0xF1B8)->bit0
#define ASC0_ABCON_ABSTEN                   ((T_Reg16 *) 0xF1B8)->bit2
#define ASC0_ABCON_AUREN                    ((T_Reg16 *) 0xF1B8)->bit1
#define ASC0_ABCON_FCDETEN                  ((T_Reg16 *) 0xF1B8)->bit4
#define ASC0_ABCON_RXINV                    ((T_Reg16 *) 0xF1B8)->bit11
#define ASC0_ABCON_TXINV                    ((T_Reg16 *) 0xF1B8)->bit10

// ASC0 Autobaud Interrupt Control Register
#define ASC0_ABIC              (*((uword volatile *) 0xF15C))
#define ASC0_ABIC_GPX                      ((T_Reg16 *) 0xF15C)->bit8
#define ASC0_ABIC_IE                       ((T_Reg16 *) 0xF15C)->bit6
#define ASC0_ABIC_IR                       ((T_Reg16 *) 0xF15C)->bit7

// ASC0 Autobaud Status Register
#define ASC0_ABSTAT            (*((uword volatile *) 0xF0B8))
#define ASC0_ABSTAT_DETWAIT                  ((T_Reg16 *) 0xF0B8)->bit4
#define ASC0_ABSTAT_FCCDET                   ((T_Reg16 *) 0xF0B8)->bit1
#define ASC0_ABSTAT_FCSDET                   ((T_Reg16 *) 0xF0B8)->bit0
#define ASC0_ABSTAT_SCCDET                   ((T_Reg16 *) 0xF0B8)->bit3
#define ASC0_ABSTAT_SCSDET                   ((T_Reg16 *) 0xF0B8)->bit2

// Serial Channel 0 Baud Rate Generator Reload Register
#define ASC0_BG                (*((uword volatile *) 0xFEB4))

// Serial Channel 0 Control Register
#define ASC0_CON               (*((uword volatile *) 0xFFB0))
#define ASC0_CON_BRS                      ((T_Reg16 *) 0xFFB0)->bit13
#define ASC0_CON_FDE                      ((T_Reg16 *) 0xFFB0)->bit11
#define ASC0_CON_FE                       ((T_Reg16 *) 0xFFB0)->bit9
#define ASC0_CON_FEN                      ((T_Reg16 *) 0xFFB0)->bit6
#define ASC0_CON_LB                       ((T_Reg16 *) 0xFFB0)->bit14
#define ASC0_CON_ODD                      ((T_Reg16 *) 0xFFB0)->bit12
#define ASC0_CON_OE                       ((T_Reg16 *) 0xFFB0)->bit10
#define ASC0_CON_OEN                      ((T_Reg16 *) 0xFFB0)->bit7
#define ASC0_CON_PE                       ((T_Reg16 *) 0xFFB0)->bit8
#define ASC0_CON_PEN_RXDI                 ((T_Reg16 *) 0xFFB0)->bit5
#define ASC0_CON_R                        ((T_Reg16 *) 0xFFB0)->bit15
#define ASC0_CON_REN                      ((T_Reg16 *) 0xFFB0)->bit4
#define ASC0_CON_STP                      ((T_Reg16 *) 0xFFB0)->bit3

// ASC0 Error Interrupt Control Register
#define ASC0_EIC               (*((uword volatile *) 0xFF70))
#define ASC0_EIC_GPX                      ((T_Reg16 *) 0xFF70)->bit8
#define ASC0_EIC_IE                       ((T_Reg16 *) 0xFF70)->bit6
#define ASC0_EIC_IR                       ((T_Reg16 *) 0xFF70)->bit7

// Fractional Divider Register
#define ASC0_FDV               (*((uword volatile *) 0xFEB6))

// FIFO Status Register
#define ASC0_FSTAT             (*((uword volatile *) 0xF0BA))

// ASC0 IrDA Pulse Mode and Width Reg.
#define ASC0_PMW               (*((uword volatile *) 0xFEAA))

// Serial Channel 0 Receiver Buffer Register (RO)
#define ASC0_RBUF              (*((uword volatile *) 0xFEB2))

// ASC0 Receive Interrupt Control Register
#define ASC0_RIC               (*((uword volatile *) 0xFF6E))
#define ASC0_RIC_GPX                      ((T_Reg16 *) 0xFF6E)->bit8
#define ASC0_RIC_IE                       ((T_Reg16 *) 0xFF6E)->bit6
#define ASC0_RIC_IR                       ((T_Reg16 *) 0xFF6E)->bit7

// Receive FIFO Control Register
#define ASC0_RXFCON            (*((uword volatile *) 0xF0C6))
#define ASC0_RXFCON_RXFEN                    ((T_Reg16 *) 0xF0C6)->bit0
#define ASC0_RXFCON_RXFFLU                   ((T_Reg16 *) 0xF0C6)->bit1
#define ASC0_RXFCON_RXTMEN                   ((T_Reg16 *) 0xF0C6)->bit2

// ASC0 Transmit Buffer Interrupt Control Register
#define ASC0_TBIC              (*((uword volatile *) 0xF19C))
#define ASC0_TBIC_GPX                      ((T_Reg16 *) 0xF19C)->bit8
#define ASC0_TBIC_IE                       ((T_Reg16 *) 0xF19C)->bit6
#define ASC0_TBIC_IR                       ((T_Reg16 *) 0xF19C)->bit7

// Serial Channel 0 Transmitter Buffer Register (WO)
#define ASC0_TBUF              (*((uword volatile *) 0xFEB0))

// ASC0 Transmit Interrupt Control Register
#define ASC0_TIC               (*((uword volatile *) 0xFF6C))
#define ASC0_TIC_GPX                      ((T_Reg16 *) 0xFF6C)->bit8
#define ASC0_TIC_IE                       ((T_Reg16 *) 0xFF6C)->bit6
#define ASC0_TIC_IR                       ((T_Reg16 *) 0xFF6C)->bit7

// Transmit FIFO Control Register
#define ASC0_TXFCON            (*((uword volatile *) 0xF0C4))
#define ASC0_TXFCON_TXFEN                    ((T_Reg16 *) 0xF0C4)->bit0
#define ASC0_TXFCON_TXFFLU                   ((T_Reg16 *) 0xF0C4)->bit1
#define ASC0_TXFCON_TXTMEN                   ((T_Reg16 *) 0xF0C4)->bit2

// ASC1 Autobaud Control Register
#define ASC1_ABCON             (*((uword volatile *) 0xF1BC))
#define ASC1_ABCON_ABDETEN                  ((T_Reg16 *) 0xF1BC)->bit3
#define ASC1_ABCON_ABEN                     ((T_Reg16 *) 0xF1BC)->bit0
#define ASC1_ABCON_ABSTEN                   ((T_Reg16 *) 0xF1BC)->bit2
#define ASC1_ABCON_AUREN                    ((T_Reg16 *) 0xF1BC)->bit1
#define ASC1_ABCON_FCDETEN                  ((T_Reg16 *) 0xF1BC)->bit4
#define ASC1_ABCON_RXINV                    ((T_Reg16 *) 0xF1BC)->bit11
#define ASC1_ABCON_TXINV                    ((T_Reg16 *) 0xF1BC)->bit10

// ASC1 Autobaud Interrupt Control Register
#define ASC1_ABIC              (*((uword volatile *) 0xF1BA))
#define ASC1_ABIC_GPX                      ((T_Reg16 *) 0xF1BA)->bit8
#define ASC1_ABIC_IE                       ((T_Reg16 *) 0xF1BA)->bit6
#define ASC1_ABIC_IR                       ((T_Reg16 *) 0xF1BA)->bit7

// ASC1 Autobaud Status Register
#define ASC1_ABSTAT            (*((uword volatile *) 0xF0BC))
#define ASC1_ABSTAT_DETWAIT                  ((T_Reg16 *) 0xF0BC)->bit4
#define ASC1_ABSTAT_FCCDET                   ((T_Reg16 *) 0xF0BC)->bit1
#define ASC1_ABSTAT_FCSDET                   ((T_Reg16 *) 0xF0BC)->bit0
#define ASC1_ABSTAT_SCCDET                   ((T_Reg16 *) 0xF0BC)->bit3
#define ASC1_ABSTAT_SCSDET                   ((T_Reg16 *) 0xF0BC)->bit2

// Serial Channel 1 Baud Rate Generator Reload Register
#define ASC1_BG                (*((uword volatile *) 0xFEBC))

// Serial Channel 1 Control Register
#define ASC1_CON               (*((uword volatile *) 0xFFB8))
#define ASC1_CON_BRS                      ((T_Reg16 *) 0xFFB8)->bit13
#define ASC1_CON_FDE                      ((T_Reg16 *) 0xFFB8)->bit11
#define ASC1_CON_FE                       ((T_Reg16 *) 0xFFB8)->bit9
#define ASC1_CON_FEN                      ((T_Reg16 *) 0xFFB8)->bit6
#define ASC1_CON_LB                       ((T_Reg16 *) 0xFFB8)->bit14
#define ASC1_CON_ODD                      ((T_Reg16 *) 0xFFB8)->bit12
#define ASC1_CON_OE                       ((T_Reg16 *) 0xFFB8)->bit10
#define ASC1_CON_OEN                      ((T_Reg16 *) 0xFFB8)->bit7
#define ASC1_CON_PE                       ((T_Reg16 *) 0xFFB8)->bit8
#define ASC1_CON_PEN_RXDI                 ((T_Reg16 *) 0xFFB8)->bit5
#define ASC1_CON_R                        ((T_Reg16 *) 0xFFB8)->bit15
#define ASC1_CON_REN                      ((T_Reg16 *) 0xFFB8)->bit4
#define ASC1_CON_STP                      ((T_Reg16 *) 0xFFB8)->bit3

// ASC1 Error Interrupt Control Register
#define ASC1_EIC               (*((uword volatile *) 0xF192))
#define ASC1_EIC_GPX                      ((T_Reg16 *) 0xF192)->bit8
#define ASC1_EIC_IE                       ((T_Reg16 *) 0xF192)->bit6
#define ASC1_EIC_IR                       ((T_Reg16 *) 0xF192)->bit7

// Fractional Divider Register
#define ASC1_FDV               (*((uword volatile *) 0xFEBE))

// FIFO Status Register
#define ASC1_FSTAT             (*((uword volatile *) 0xF0BE))

// ASC1 IrDA Pulse Mode and Width Reg.
#define ASC1_PMW               (*((uword volatile *) 0xFEAC))

// Serial Channel 1 Receiver Buffer Register (RO)
#define ASC1_RBUF              (*((uword volatile *) 0xFEBA))

// ASC1 Receive Interrupt Control Register
#define ASC1_RIC               (*((uword volatile *) 0xF18A))
#define ASC1_RIC_GPX                      ((T_Reg16 *) 0xF18A)->bit8
#define ASC1_RIC_IE                       ((T_Reg16 *) 0xF18A)->bit6
#define ASC1_RIC_IR                       ((T_Reg16 *) 0xF18A)->bit7

// Receive FIFO Control Register
#define ASC1_RXFCON            (*((uword volatile *) 0xF0A6))
#define ASC1_RXFCON_RXFEN                    ((T_Reg16 *) 0xF0A6)->bit0
#define ASC1_RXFCON_RXFFLU                   ((T_Reg16 *) 0xF0A6)->bit1
#define ASC1_RXFCON_RXTMEN                   ((T_Reg16 *) 0xF0A6)->bit2

// ASC1 Transmit Buffer Interrupt Control Register
#define ASC1_TBIC              (*((uword volatile *) 0xF150))
#define ASC1_TBIC_GPX                      ((T_Reg16 *) 0xF150)->bit8
#define ASC1_TBIC_IE                       ((T_Reg16 *) 0xF150)->bit6
#define ASC1_TBIC_IR                       ((T_Reg16 *) 0xF150)->bit7

// Serial Channel 1 Transmitter Buffer Register (WO)
#define ASC1_TBUF              (*((uword volatile *) 0xFEB8))

// ASC1 Transmit Interrupt Control Register
#define ASC1_TIC               (*((uword volatile *) 0xF182))
#define ASC1_TIC_GPX                      ((T_Reg16 *) 0xF182)->bit8
#define ASC1_TIC_IE                       ((T_Reg16 *) 0xF182)->bit6
#define ASC1_TIC_IR                       ((T_Reg16 *) 0xF182)->bit7

// Transmit FIFO Control Register
#define ASC1_TXFCON            (*((uword volatile *) 0xF0A4))
#define ASC1_TXFCON_TXFEN                    ((T_Reg16 *) 0xF0A4)->bit0
#define ASC1_TXFCON_TXFFLU                   ((T_Reg16 *) 0xF0A4)->bit1
#define ASC1_TXFCON_TXTMEN                   ((T_Reg16 *) 0xF0A4)->bit2

// Register Bank Selection Register 0
#define BNKSEL0                (*((uword volatile *) 0xEC20))

// Register Bank Selection Register 1
#define BNKSEL1                (*((uword volatile *) 0xEC22))

// Register Bank Selection Register 2
#define BNKSEL2                (*((uword volatile *) 0xEC24))

// Register Bank Selection Register 3
#define BNKSEL3                (*((uword volatile *) 0xEC26))

// CAN Mode 0 Interrupt Control register
#define CAN_0IC                (*((uword volatile *) 0xF196))
#define CAN_0IC_GPX                      ((T_Reg16 *) 0xF196)->bit8
#define CAN_0IC_IE                       ((T_Reg16 *) 0xF196)->bit6
#define CAN_0IC_IR                       ((T_Reg16 *) 0xF196)->bit7

// CAN Mode 1 Interrupt Control register
#define CAN_1IC                (*((uword volatile *) 0xF142))
#define CAN_1IC_GPX                      ((T_Reg16 *) 0xF142)->bit8
#define CAN_1IC_IE                       ((T_Reg16 *) 0xF142)->bit6
#define CAN_1IC_IR                       ((T_Reg16 *) 0xF142)->bit7

// CAN Mode 2 Interrupt Control register
#define CAN_2IC                (*((uword volatile *) 0xF144))
#define CAN_2IC_GPX                      ((T_Reg16 *) 0xF144)->bit8
#define CAN_2IC_IE                       ((T_Reg16 *) 0xF144)->bit6
#define CAN_2IC_IR                       ((T_Reg16 *) 0xF144)->bit7

// CAN Mode 3 Interrupt Control register
#define CAN_3IC                (*((uword volatile *) 0xF146))
#define CAN_3IC_GPX                      ((T_Reg16 *) 0xF146)->bit8
#define CAN_3IC_IE                       ((T_Reg16 *) 0xF146)->bit6
#define CAN_3IC_IR                       ((T_Reg16 *) 0xF146)->bit7

// CAN Mode 4 Interrupt Control register
#define CAN_4IC                (*((uword volatile *) 0xF148))
#define CAN_4IC_GPX                      ((T_Reg16 *) 0xF148)->bit8
#define CAN_4IC_IE                       ((T_Reg16 *) 0xF148)->bit6
#define CAN_4IC_IR                       ((T_Reg16 *) 0xF148)->bit7

// CAN Mode 5 Interrupt Control register
#define CAN_5IC                (*((uword volatile *) 0xF14A))
#define CAN_5IC_GPX                      ((T_Reg16 *) 0xF14A)->bit8
#define CAN_5IC_IE                       ((T_Reg16 *) 0xF14A)->bit6
#define CAN_5IC_IR                       ((T_Reg16 *) 0xF14A)->bit7

// CAN Mode 6 Interrupt Control register
#define CAN_6IC                (*((uword volatile *) 0xF14C))
#define CAN_6IC_GPX                      ((T_Reg16 *) 0xF14C)->bit8
#define CAN_6IC_IE                       ((T_Reg16 *) 0xF14C)->bit6
#define CAN_6IC_IR                       ((T_Reg16 *) 0xF14C)->bit7

// CAN Mode 7 Interrupt Control register
#define CAN_7IC                (*((uword volatile *) 0xF14E))
#define CAN_7IC_GPX                      ((T_Reg16 *) 0xF14E)->bit8
#define CAN_7IC_IE                       ((T_Reg16 *) 0xF14E)->bit6
#define CAN_7IC_IR                       ((T_Reg16 *) 0xF14E)->bit7

// Node A Bit Timing Register High
#define CAN_ABTRH              (*((uword volatile far *) 0x20020E))
#define CAN_ABTRH_LBM                      0x0001

// Node A Bit Timing Register Low
#define CAN_ABTRL              (*((uword volatile far *) 0x20020C))
#define CAN_ABTRL_DIV8X                    0x8000

// Node A Control Register
#define CAN_ACR                (*((uword volatile far *) 0x200200))
#define CAN_ACR_CALM                     0x0080
#define CAN_ACR_CCE                      0x0040
#define CAN_ACR_EIE                      0x0008
#define CAN_ACR_INIT                     0x0001
#define CAN_ACR_LECIE                    0x0010
#define CAN_ACR_SIE                      0x0004

// Node A Error Counter Register High
#define CAN_AECNTH             (*((uword volatile far *) 0x200222))
#define CAN_AECNTH_LEINC                    0x0200
#define CAN_AECNTH_LETD                     0x0100

// Node A Error Counter Register Low
#define CAN_AECNTL             (*((uword volatile far *) 0x200220))

// Node A Frame Counter Register High
#define CAN_AFCRH              (*((uword volatile far *) 0x200216))
#define CAN_AFCRH_CFCIE                    0x0040
#define CAN_AFCRH_CFCOV                    0x0080

// Node A Frame Counter Register Low
#define CAN_AFCRL              (*((uword volatile far *) 0x200214))

// Node A Global Interrupt Node Pointer Register
#define CAN_AGINP              (*((uword volatile far *) 0x200210))

// Node A INTID Mask Register 4 Low
#define CAN_AIMR4              (*((uword volatile far *) 0x20021C))
#define CAN_AIMR4_IMC32                    0x0001
#define CAN_AIMR4_IMC33                    0x0002
#define CAN_AIMR4_IMC34                    0x0004

// Node A INTID Mask Register 0 High
#define CAN_AIMRH0             (*((uword volatile far *) 0x20021A))
#define CAN_AIMRH0_IMC16                    0x0001
#define CAN_AIMRH0_IMC17                    0x0002
#define CAN_AIMRH0_IMC18                    0x0004
#define CAN_AIMRH0_IMC19                    0x0008
#define CAN_AIMRH0_IMC20                    0x0010
#define CAN_AIMRH0_IMC21                    0x0020
#define CAN_AIMRH0_IMC22                    0x0040
#define CAN_AIMRH0_IMC23                    0x0080
#define CAN_AIMRH0_IMC24                    0x0100
#define CAN_AIMRH0_IMC25                    0x0200
#define CAN_AIMRH0_IMC26                    0x0400
#define CAN_AIMRH0_IMC27                    0x0800
#define CAN_AIMRH0_IMC28                    0x1000
#define CAN_AIMRH0_IMC29                    0x2000
#define CAN_AIMRH0_IMC30                    0x4000
#define CAN_AIMRH0_IMC31                    0x8000

// Node A INTID Mask Register 0 Low
#define CAN_AIMRL0             (*((uword volatile far *) 0x200218))
#define CAN_AIMRL0_IMC0                     0x0001
#define CAN_AIMRL0_IMC1                     0x0002
#define CAN_AIMRL0_IMC10                    0x0400
#define CAN_AIMRL0_IMC11                    0x0800
#define CAN_AIMRL0_IMC12                    0x1000
#define CAN_AIMRL0_IMC13                    0x2000
#define CAN_AIMRL0_IMC14                    0x4000
#define CAN_AIMRL0_IMC15                    0x8000
#define CAN_AIMRL0_IMC2                     0x0004
#define CAN_AIMRL0_IMC3                     0x0008
#define CAN_AIMRL0_IMC4                     0x0010
#define CAN_AIMRL0_IMC5                     0x0020
#define CAN_AIMRL0_IMC6                     0x0040
#define CAN_AIMRL0_IMC7                     0x0080
#define CAN_AIMRL0_IMC8                     0x0100
#define CAN_AIMRL0_IMC9                     0x0200

// Node A Interrupt Pending Register
#define CAN_AIR                (*((uword volatile far *) 0x200208))

// Node A Status Register
#define CAN_ASR                (*((uword volatile far *) 0x200204))
#define CAN_ASR_BOFF                     0x0080
#define CAN_ASR_EWRN                     0x0040
#define CAN_ASR_RXOK                     0x0010
#define CAN_ASR_TXOK                     0x0008

// Node B Bit Timing Register High
#define CAN_BBTRH              (*((uword volatile far *) 0x20024E))
#define CAN_BBTRH_LBM                      0x0001

// Node B Bit Timing Register Low
#define CAN_BBTRL              (*((uword volatile far *) 0x20024C))
#define CAN_BBTRL_DIV8X                    0x8000

// Node B Control Register
#define CAN_BCR                (*((uword volatile far *) 0x200240))
#define CAN_BCR_CALM                     0x0080
#define CAN_BCR_CCE                      0x0040
#define CAN_BCR_EIE                      0x0008
#define CAN_BCR_INIT                     0x0001
#define CAN_BCR_LECIE                    0x0010
#define CAN_BCR_SIE                      0x0004

// Node B Error Counter Register High
#define CAN_BECNTH             (*((uword volatile far *) 0x200262))
#define CAN_BECNTH_LEINC                    0x0200
#define CAN_BECNTH_LETD                     0x0100

// Node B Error Counter Register Low
#define CAN_BECNTL             (*((uword volatile far *) 0x200260))

// Node B Frame Counter Register High
#define CAN_BFCRH              (*((uword volatile far *) 0x200256))
#define CAN_BFCRH_CFCIE                    0x0040
#define CAN_BFCRH_CFCOV                    0x0080

// Node B Frame Counter Register Low
#define CAN_BFCRL              (*((uword volatile far *) 0x200254))

// Node B Global Interrupt Node Pointer Register
#define CAN_BGINP              (*((uword volatile far *) 0x200250))

// Node B INTID Mask Register 4 Low
#define CAN_BIMR4              (*((uword volatile far *) 0x20025C))
#define CAN_BIMR4_IMC32                    0x0001
#define CAN_BIMR4_IMC33                    0x0002
#define CAN_BIMR4_IMC34                    0x0004

// Node B INTID Mask Register 0 High
#define CAN_BIMRH0             (*((uword volatile far *) 0x20025A))
#define CAN_BIMRH0_IMC16                    0x0001
#define CAN_BIMRH0_IMC17                    0x0002
#define CAN_BIMRH0_IMC18                    0x0004
#define CAN_BIMRH0_IMC19                    0x0008
#define CAN_BIMRH0_IMC20                    0x0010
#define CAN_BIMRH0_IMC21                    0x0020
#define CAN_BIMRH0_IMC22                    0x0040
#define CAN_BIMRH0_IMC23                    0x0080
#define CAN_BIMRH0_IMC24                    0x0100
#define CAN_BIMRH0_IMC25                    0x0200
#define CAN_BIMRH0_IMC26                    0x0400
#define CAN_BIMRH0_IMC27                    0x0800
#define CAN_BIMRH0_IMC28                    0x1000
#define CAN_BIMRH0_IMC29                    0x2000
#define CAN_BIMRH0_IMC30                    0x4000
#define CAN_BIMRH0_IMC31                    0x8000

// Node B INTID Mask Register 0 Low
#define CAN_BIMRL0             (*((uword volatile far *) 0x200258))
#define CAN_BIMRL0_IMC0                     0x0001
#define CAN_BIMRL0_IMC1                     0x0002
#define CAN_BIMRL0_IMC10                    0x0400
#define CAN_BIMRL0_IMC11                    0x0800
#define CAN_BIMRL0_IMC12                    0x1000
#define CAN_BIMRL0_IMC13                    0x2000
#define CAN_BIMRL0_IMC14                    0x4000
#define CAN_BIMRL0_IMC15                    0x8000
#define CAN_BIMRL0_IMC2                     0x0004
#define CAN_BIMRL0_IMC3                     0x0008
#define CAN_BIMRL0_IMC4                     0x0010
#define CAN_BIMRL0_IMC5                     0x0020
#define CAN_BIMRL0_IMC6                     0x0040
#define CAN_BIMRL0_IMC7                     0x0080
#define CAN_BIMRL0_IMC8                     0x0100
#define CAN_BIMRL0_IMC9                     0x0200

// Node B Interrupt Pending Register
#define CAN_BIR                (*((uword volatile far *) 0x200248))

// Node B Status Register
#define CAN_BSR                (*((uword volatile far *) 0x200244))
#define CAN_BSR_BOFF                     0x0080
#define CAN_BSR_EWRN                     0x0040
#define CAN_BSR_RXOK                     0x0010
#define CAN_BSR_TXOK                     0x0008

// Message Object 0 Arbitration Mask Register High
#define CAN_MSGAMRH0           (*((uword volatile far *) 0x20030E))

// Message Object 1 Arbitration Mask Register High
#define CAN_MSGAMRH1           (*((uword volatile far *) 0x20032E))

// Message Object 10 Arbitration Mask Register High
#define CAN_MSGAMRH10          (*((uword volatile far *) 0x20044E))

// Message Object 11 Arbitration Mask Register High
#define CAN_MSGAMRH11          (*((uword volatile far *) 0x20046E))

// Message Object 12 Arbitration Mask Register High
#define CAN_MSGAMRH12          (*((uword volatile far *) 0x20048E))

// Message Object 13 Arbitration Mask Register High
#define CAN_MSGAMRH13          (*((uword volatile far *) 0x2004AE))

// Message Object 14 Arbitration Mask Register High
#define CAN_MSGAMRH14          (*((uword volatile far *) 0x2004CE))

// Message Object 15 Arbitration Mask Register High
#define CAN_MSGAMRH15          (*((uword volatile far *) 0x2004EE))

// Message Object 16 Arbitration Mask Register High
#define CAN_MSGAMRH16          (*((uword volatile far *) 0x20050E))

// Message Object 17 Arbitration Mask Register High
#define CAN_MSGAMRH17          (*((uword volatile far *) 0x20052E))

// Message Object 18 Arbitration Mask Register High
#define CAN_MSGAMRH18          (*((uword volatile far *) 0x20054E))

// Message Object 19 Arbitration Mask Register High
#define CAN_MSGAMRH19          (*((uword volatile far *) 0x20056E))

// Message Object 2 Arbitration Mask Register High
#define CAN_MSGAMRH2           (*((uword volatile far *) 0x20034E))

// Message Object 20 Arbitration Mask Register High
#define CAN_MSGAMRH20          (*((uword volatile far *) 0x20058E))

// Message Object 21 Arbitration Mask Register High
#define CAN_MSGAMRH21          (*((uword volatile far *) 0x2005AE))

// Message Object 22 Arbitration Mask Register High
#define CAN_MSGAMRH22          (*((uword volatile far *) 0x2005CE))

// Message Object 23 Arbitration Mask Register High
#define CAN_MSGAMRH23          (*((uword volatile far *) 0x2005EE))

// Message Object 24 Arbitration Mask Register High
#define CAN_MSGAMRH24          (*((uword volatile far *) 0x20060E))

// Message Object 25 Arbitration Mask Register High
#define CAN_MSGAMRH25          (*((uword volatile far *) 0x20062E))

// Message Object 26 Arbitration Mask Register High
#define CAN_MSGAMRH26          (*((uword volatile far *) 0x20064E))

// Message Object 27 Arbitration Mask Register High
#define CAN_MSGAMRH27          (*((uword volatile far *) 0x20066E))

// Message Object 28 Arbitration Mask Register High
#define CAN_MSGAMRH28          (*((uword volatile far *) 0x20068E))

// Message Object 29 Arbitration Mask Register High
#define CAN_MSGAMRH29          (*((uword volatile far *) 0x2006AE))

// Message Object 3 Arbitration Mask Register High
#define CAN_MSGAMRH3           (*((uword volatile far *) 0x20036E))

// Message Object 30 Arbitration Mask Register High
#define CAN_MSGAMRH30          (*((uword volatile far *) 0x2006CE))

// Message Object 31 Arbitration Mask Register High
#define CAN_MSGAMRH31          (*((uword volatile far *) 0x2006EE))

// Message Object 4 Arbitration Mask Register High
#define CAN_MSGAMRH4           (*((uword volatile far *) 0x20038E))

// Message Object 5 Arbitration Mask Register High
#define CAN_MSGAMRH5           (*((uword volatile far *) 0x2003AE))

// Message Object 6 Arbitration Mask Register High
#define CAN_MSGAMRH6           (*((uword volatile far *) 0x2003CE))

// Message Object 7 Arbitration Mask Register High
#define CAN_MSGAMRH7           (*((uword volatile far *) 0x2003EE))

// Message Object 8 Arbitration Mask Register High
#define CAN_MSGAMRH8           (*((uword volatile far *) 0x20040E))

// Message Object 9 Arbitration Mask Register High
#define CAN_MSGAMRH9           (*((uword volatile far *) 0x20042E))

// Message Object 0 Arbitration Mask Register Low
#define CAN_MSGAMRL0           (*((uword volatile far *) 0x20030C))

// Message Object 1 Arbitration Mask Register Low
#define CAN_MSGAMRL1           (*((uword volatile far *) 0x20032C))

// Message Object 10 Arbitration Mask Register Low
#define CAN_MSGAMRL10          (*((uword volatile far *) 0x20044C))

// Message Object 11 Arbitration Mask Register Low
#define CAN_MSGAMRL11          (*((uword volatile far *) 0x20046C))

// Message Object 12 Arbitration Mask Register Low
#define CAN_MSGAMRL12          (*((uword volatile far *) 0x20048C))

// Message Object 13 Arbitration Mask Register Low
#define CAN_MSGAMRL13          (*((uword volatile far *) 0x2004AC))

// Message Object 14 Arbitration Mask Register Low
#define CAN_MSGAMRL14          (*((uword volatile far *) 0x2004CC))

// Message Object 15 Arbitration Mask Register Low
#define CAN_MSGAMRL15          (*((uword volatile far *) 0x2004EC))

// Message Object 16 Arbitration Mask Register Low
#define CAN_MSGAMRL16          (*((uword volatile far *) 0x20050C))

// Message Object 17 Arbitration Mask Register Low
#define CAN_MSGAMRL17          (*((uword volatile far *) 0x20052C))

// Message Object 18 Arbitration Mask Register Low
#define CAN_MSGAMRL18          (*((uword volatile far *) 0x20054C))

// Message Object 19 Arbitration Mask Register Low
#define CAN_MSGAMRL19          (*((uword volatile far *) 0x20056C))

// Message Object 2 Arbitration Mask Register Low
#define CAN_MSGAMRL2           (*((uword volatile far *) 0x20034C))

// Message Object 20 Arbitration Mask Register Low
#define CAN_MSGAMRL20          (*((uword volatile far *) 0x20058C))

// Message Object 21 Arbitration Mask Register Low
#define CAN_MSGAMRL21          (*((uword volatile far *) 0x2005AC))

// Message Object 22 Arbitration Mask Register Low
#define CAN_MSGAMRL22          (*((uword volatile far *) 0x2005CC))

// Message Object 23 Arbitration Mask Register Low
#define CAN_MSGAMRL23          (*((uword volatile far *) 0x2005EC))

// Message Object 24 Arbitration Mask Register Low
#define CAN_MSGAMRL24          (*((uword volatile far *) 0x20060C))

// Message Object 25 Arbitration Mask Register Low
#define CAN_MSGAMRL25          (*((uword volatile far *) 0x20062C))

// Message Object 26 Arbitration Mask Register Low
#define CAN_MSGAMRL26          (*((uword volatile far *) 0x20064C))

// Message Object 27 Arbitration Mask Register Low
#define CAN_MSGAMRL27          (*((uword volatile far *) 0x20066C))

// Message Object 28 Arbitration Mask Register Low
#define CAN_MSGAMRL28          (*((uword volatile far *) 0x20068C))

// Message Object 29 Arbitration Mask Register Low
#define CAN_MSGAMRL29          (*((uword volatile far *) 0x2006AC))

// Message Object 3 Arbitration Mask Register Low
#define CAN_MSGAMRL3           (*((uword volatile far *) 0x20036C))

// Message Object 30 Arbitration Mask Register Low
#define CAN_MSGAMRL30          (*((uword volatile far *) 0x2006CC))

// Message Object 31 Arbitration Mask Register Low
#define CAN_MSGAMRL31          (*((uword volatile far *) 0x2006EC))

// Message Object 4 Arbitration Mask Register Low
#define CAN_MSGAMRL4           (*((uword volatile far *) 0x20038C))

// Message Object 5 Arbitration Mask Register Low
#define CAN_MSGAMRL5           (*((uword volatile far *) 0x2003AC))

// Message Object 6 Arbitration Mask Register Low
#define CAN_MSGAMRL6           (*((uword volatile far *) 0x2003CC))

// Message Object 7 Arbitration Mask Register Low
#define CAN_MSGAMRL7           (*((uword volatile far *) 0x2003EC))

// Message Object 8 Arbitration Mask Register Low
#define CAN_MSGAMRL8           (*((uword volatile far *) 0x20040C))

// Message Object 9 Arbitration Mask Register Low
#define CAN_MSGAMRL9           (*((uword volatile far *) 0x20042C))

// Message Object 0 Arbitration Register High
#define CAN_MSGARH0            (*((uword volatile far *) 0x20030A))

// Message Object 1 Arbitration Register High
#define CAN_MSGARH1            (*((uword volatile far *) 0x20032A))

// Message Object 10 Arbitration Register High
#define CAN_MSGARH10           (*((uword volatile far *) 0x20044A))

// Message Object 11 Arbitration Register High
#define CAN_MSGARH11           (*((uword volatile far *) 0x20046A))

// Message Object 12 Arbitration Register High
#define CAN_MSGARH12           (*((uword volatile far *) 0x20048A))

// Message Object 13 Arbitration Register High
#define CAN_MSGARH13           (*((uword volatile far *) 0x2004AA))

// Message Object 14 Arbitration Register High
#define CAN_MSGARH14           (*((uword volatile far *) 0x2004CA))

// Message Object 15 Arbitration Register High
#define CAN_MSGARH15           (*((uword volatile far *) 0x2004EA))

// Message Object 16 Arbitration Register High
#define CAN_MSGARH16           (*((uword volatile far *) 0x20050A))

// Message Object 17 Arbitration Register High
#define CAN_MSGARH17           (*((uword volatile far *) 0x20052A))

// Message Object 18 Arbitration Register High
#define CAN_MSGARH18           (*((uword volatile far *) 0x20054A))

// Message Object 19 Arbitration Register High
#define CAN_MSGARH19           (*((uword volatile far *) 0x20056A))

// Message Object 2 Arbitration Register High
#define CAN_MSGARH2            (*((uword volatile far *) 0x20034A))

// Message Object 20 Arbitration Register High
#define CAN_MSGARH20           (*((uword volatile far *) 0x20058A))

// Message Object 21 Arbitration Register High
#define CAN_MSGARH21           (*((uword volatile far *) 0x2005AA))

// Message Object 22 Arbitration Register High
#define CAN_MSGARH22           (*((uword volatile far *) 0x2005CA))

// Message Object 23 Arbitration Register High
#define CAN_MSGARH23           (*((uword volatile far *) 0x2005EA))

// Message Object 24 Arbitration Register High
#define CAN_MSGARH24           (*((uword volatile far *) 0x20060A))

// Message Object 25 Arbitration Register High
#define CAN_MSGARH25           (*((uword volatile far *) 0x20062A))

// Message Object 26 Arbitration Register High
#define CAN_MSGARH26           (*((uword volatile far *) 0x20064A))

// Message Object 27 Arbitration Register High
#define CAN_MSGARH27           (*((uword volatile far *) 0x20066A))

// Message Object 28 Arbitration Register High
#define CAN_MSGARH28           (*((uword volatile far *) 0x20068A))

// Message Object 29 Arbitration Register High
#define CAN_MSGARH29           (*((uword volatile far *) 0x2006AA))

// Message Object 3 Arbitration Register High
#define CAN_MSGARH3            (*((uword volatile far *) 0x20036A))

// Message Object 30 Arbitration Register High
#define CAN_MSGARH30           (*((uword volatile far *) 0x2006CA))

// Message Object 31 Arbitration Register High
#define CAN_MSGARH31           (*((uword volatile far *) 0x2006EA))

// Message Object 4 Arbitration Register High
#define CAN_MSGARH4            (*((uword volatile far *) 0x20038A))

// Message Object 5 Arbitration Register High
#define CAN_MSGARH5            (*((uword volatile far *) 0x2003AA))

// Message Object 6 Arbitration Register High
#define CAN_MSGARH6            (*((uword volatile far *) 0x2003CA))

// Message Object 7 Arbitration Register High
#define CAN_MSGARH7            (*((uword volatile far *) 0x2003EA))

// Message Object 8 Arbitration Register High
#define CAN_MSGARH8            (*((uword volatile far *) 0x20040A))

// Message Object 9 Arbitration Register High
#define CAN_MSGARH9            (*((uword volatile far *) 0x20042A))

// Message Object 0 Arbitration Register Low
#define CAN_MSGARL0            (*((uword volatile far *) 0x200308))

// Message Object 1 Arbitration Register Low
#define CAN_MSGARL1            (*((uword volatile far *) 0x200328))

// Message Object 10 Arbitration Register Low
#define CAN_MSGARL10           (*((uword volatile far *) 0x200448))

// Message Object 11 Arbitration Register Low
#define CAN_MSGARL11           (*((uword volatile far *) 0x200468))

// Message Object 12 Arbitration Register Low
#define CAN_MSGARL12           (*((uword volatile far *) 0x200488))

// Message Object 13 Arbitration Register Low
#define CAN_MSGARL13           (*((uword volatile far *) 0x2004A8))

// Message Object 14 Arbitration Register Low
#define CAN_MSGARL14           (*((uword volatile far *) 0x2004C8))

// Message Object 15 Arbitration Register Low
#define CAN_MSGARL15           (*((uword volatile far *) 0x2004E8))

// Message Object 16 Arbitration Register Low
#define CAN_MSGARL16           (*((uword volatile far *) 0x200508))

// Message Object 17 Arbitration Register Low
#define CAN_MSGARL17           (*((uword volatile far *) 0x200528))

// Message Object 18 Arbitration Register Low
#define CAN_MSGARL18           (*((uword volatile far *) 0x200548))

// Message Object 19 Arbitration Register Low
#define CAN_MSGARL19           (*((uword volatile far *) 0x200568))

// Message Object 2 Arbitration Register Low
#define CAN_MSGARL2            (*((uword volatile far *) 0x200348))

// Message Object 20 Arbitration Register Low
#define CAN_MSGARL20           (*((uword volatile far *) 0x200588))

// Message Object 21 Arbitration Register Low
#define CAN_MSGARL21           (*((uword volatile far *) 0x2005A8))

// Message Object 22 Arbitration Register Low
#define CAN_MSGARL22           (*((uword volatile far *) 0x2005C8))

// Message Object 23 Arbitration Register Low
#define CAN_MSGARL23           (*((uword volatile far *) 0x2005E8))

// Message Object 24 Arbitration Register Low
#define CAN_MSGARL24           (*((uword volatile far *) 0x200608))

// Message Object 25 Arbitration Register Low
#define CAN_MSGARL25           (*((uword volatile far *) 0x200628))

// Message Object 26 Arbitration Register Low
#define CAN_MSGARL26           (*((uword volatile far *) 0x200648))

// Message Object 27 Arbitration Register Low
#define CAN_MSGARL27           (*((uword volatile far *) 0x200668))

// Message Object 28 Arbitration Register Low
#define CAN_MSGARL28           (*((uword volatile far *) 0x200688))

// Message Object 29 Arbitration Register Low
#define CAN_MSGARL29           (*((uword volatile far *) 0x2006A8))

// Message Object 3 Arbitration Register Low
#define CAN_MSGARL3            (*((uword volatile far *) 0x200368))

// Message Object 30 Arbitration Register Low
#define CAN_MSGARL30           (*((uword volatile far *) 0x2006C8))

// Message Object 31 Arbitration Register Low
#define CAN_MSGARL31           (*((uword volatile far *) 0x2006E8))

// Message Object 4 Arbitration Register Low
#define CAN_MSGARL4            (*((uword volatile far *) 0x200388))

// Message Object 5 Arbitration Register Low
#define CAN_MSGARL5            (*((uword volatile far *) 0x2003A8))

// Message Object 6 Arbitration Register Low
#define CAN_MSGARL6            (*((uword volatile far *) 0x2003C8))

// Message Object 7 Arbitration Register Low
#define CAN_MSGARL7            (*((uword volatile far *) 0x2003E8))

// Message Object 8 Arbitration Register Low
#define CAN_MSGARL8            (*((uword volatile far *) 0x200408))

// Message Object 9 Arbitration Register Low
#define CAN_MSGARL9            (*((uword volatile far *) 0x200428))

// Message Object 0 Message Configuration Register High
#define CAN_MSGCFGH0           (*((uword volatile far *) 0x200316))

// Message Object 1 Message Configuration Register High
#define CAN_MSGCFGH1           (*((uword volatile far *) 0x200336))

// Message Object 10 Message Configuration Register High
#define CAN_MSGCFGH10          (*((uword volatile far *) 0x200456))

// Message Object 11 Message Configuration Register High
#define CAN_MSGCFGH11          (*((uword volatile far *) 0x200476))

// Message Object 12 Message Configuration Register High
#define CAN_MSGCFGH12          (*((uword volatile far *) 0x200496))

// Message Object 13 Message Configuration Register High
#define CAN_MSGCFGH13          (*((uword volatile far *) 0x2004B6))

// Message Object 14 Message Configuration Register High
#define CAN_MSGCFGH14          (*((uword volatile far *) 0x2004D6))

// Message Object 15 Message Configuration Register High
#define CAN_MSGCFGH15          (*((uword volatile far *) 0x2004F6))

// Message Object 16 Message Configuration Register High
#define CAN_MSGCFGH16          (*((uword volatile far *) 0x200516))

// Message Object 17 Message Configuration Register High
#define CAN_MSGCFGH17          (*((uword volatile far *) 0x200536))

// Message Object 18 Message Configuration Register High
#define CAN_MSGCFGH18          (*((uword volatile far *) 0x200556))

// Message Object 19 Message Configuration Register High
#define CAN_MSGCFGH19          (*((uword volatile far *) 0x200576))

// Message Object 2 Message Configuration Register High
#define CAN_MSGCFGH2           (*((uword volatile far *) 0x200356))

// Message Object 20 Message Configuration Register High
#define CAN_MSGCFGH20          (*((uword volatile far *) 0x200596))

// Message Object 21 Message Configuration Register High
#define CAN_MSGCFGH21          (*((uword volatile far *) 0x2005B6))

// Message Object 22 Message Configuration Register High
#define CAN_MSGCFGH22          (*((uword volatile far *) 0x2005D6))

// Message Object 23 Message Configuration Register High
#define CAN_MSGCFGH23          (*((uword volatile far *) 0x2005F6))

// Message Object 24 Message Configuration Register High
#define CAN_MSGCFGH24          (*((uword volatile far *) 0x200616))

// Message Object 25 Message Configuration Register High
#define CAN_MSGCFGH25          (*((uword volatile far *) 0x200636))

// Message Object 26 Message Configuration Register High
#define CAN_MSGCFGH26          (*((uword volatile far *) 0x200656))

// Message Object 27 Message Configuration Register High
#define CAN_MSGCFGH27          (*((uword volatile far *) 0x200676))

// Message Object 28 Message Configuration Register High
#define CAN_MSGCFGH28          (*((uword volatile far *) 0x200696))

// Message Object 29 Message Configuration Register High
#define CAN_MSGCFGH29          (*((uword volatile far *) 0x2006B6))

// Message Object 3 Message Configuration Register High
#define CAN_MSGCFGH3           (*((uword volatile far *) 0x200376))

// Message Object 30 Message Configuration Register High
#define CAN_MSGCFGH30          (*((uword volatile far *) 0x2006D6))

// Message Object 31 Message Configuration Register High
#define CAN_MSGCFGH31          (*((uword volatile far *) 0x2006F6))

// Message Object 4 Message Configuration Register High
#define CAN_MSGCFGH4           (*((uword volatile far *) 0x200396))

// Message Object 5 Message Configuration Register High
#define CAN_MSGCFGH5           (*((uword volatile far *) 0x2003B6))

// Message Object 6 Message Configuration Register High
#define CAN_MSGCFGH6           (*((uword volatile far *) 0x2003D6))

// Message Object 7 Message Configuration Register High
#define CAN_MSGCFGH7           (*((uword volatile far *) 0x2003F6))

// Message Object 8 Message Configuration Register High
#define CAN_MSGCFGH8           (*((uword volatile far *) 0x200416))

// Message Object 9 Message Configuration Register High
#define CAN_MSGCFGH9           (*((uword volatile far *) 0x200436))

// Message Object 0 Message Configuration Register Low
#define CAN_MSGCFGL0           (*((uword volatile far *) 0x200314))
#define CAN_MSGCFGL0_DIR                      0x0008
#define CAN_MSGCFGL0_NODE                     0x0002
#define CAN_MSGCFGL0_RMM                      0x0001
#define CAN_MSGCFGL0_XTD                      0x0004

// Message Object 1 Message Configuration Register Low
#define CAN_MSGCFGL1           (*((uword volatile far *) 0x200334))
#define CAN_MSGCFGL1_DIR                      0x0008
#define CAN_MSGCFGL1_NODE                     0x0002
#define CAN_MSGCFGL1_RMM                      0x0001
#define CAN_MSGCFGL1_XTD                      0x0004

// Message Object 10 Message Configuration Register Low
#define CAN_MSGCFGL10          (*((uword volatile far *) 0x200454))
#define CAN_MSGCFGL10_DIR                      0x0008
#define CAN_MSGCFGL10_NODE                     0x0002
#define CAN_MSGCFGL10_RMM                      0x0001
#define CAN_MSGCFGL10_XTD                      0x0004

// Message Object 11 Message Configuration Register Low
#define CAN_MSGCFGL11          (*((uword volatile far *) 0x200474))
#define CAN_MSGCFGL11_DIR                      0x0008
#define CAN_MSGCFGL11_NODE                     0x0002
#define CAN_MSGCFGL11_RMM                      0x0001
#define CAN_MSGCFGL11_XTD                      0x0004

// Message Object 12 Message Configuration Register Low
#define CAN_MSGCFGL12          (*((uword volatile far *) 0x200494))
#define CAN_MSGCFGL12_DIR                      0x0008
#define CAN_MSGCFGL12_NODE                     0x0002
#define CAN_MSGCFGL12_RMM                      0x0001
#define CAN_MSGCFGL12_XTD                      0x0004

// Message Object 13 Message Configuration Register Low
#define CAN_MSGCFGL13          (*((uword volatile far *) 0x2004B4))
#define CAN_MSGCFGL13_DIR                      0x0008
#define CAN_MSGCFGL13_NODE                     0x0002
#define CAN_MSGCFGL13_RMM                      0x0001
#define CAN_MSGCFGL13_XTD                      0x0004

// Message Object 14 Message Configuration Reg Low
#define CAN_MSGCFGL14          (*((uword volatile far *) 0x2004D4))
#define CAN_MSGCFGL14_DIR                      0x0008
#define CAN_MSGCFGL14_NODE                     0x0002
#define CAN_MSGCFGL14_RMM                      0x0001
#define CAN_MSGCFGL14_XTD                      0x0004

// Message Object 15 Message Configuration Register Low
#define CAN_MSGCFGL15          (*((uword volatile far *) 0x2004F4))
#define CAN_MSGCFGL15_DIR                      0x0008
#define CAN_MSGCFGL15_NODE                     0x0002
#define CAN_MSGCFGL15_RMM                      0x0001
#define CAN_MSGCFGL15_XTD                      0x0004

// Message Object 16 Message Configuration Register Low
#define CAN_MSGCFGL16          (*((uword volatile far *) 0x200514))
#define CAN_MSGCFGL16_DIR                      0x0008
#define CAN_MSGCFGL16_NODE                     0x0002
#define CAN_MSGCFGL16_RMM                      0x0001
#define CAN_MSGCFGL16_XTD                      0x0004

// Message Object 17 Message Configuration Register Low
#define CAN_MSGCFGL17          (*((uword volatile far *) 0x200534))
#define CAN_MSGCFGL17_DIR                      0x0008
#define CAN_MSGCFGL17_NODE                     0x0002
#define CAN_MSGCFGL17_RMM                      0x0001
#define CAN_MSGCFGL17_XTD                      0x0004

// Message Object 18 Message Configuration Register Low
#define CAN_MSGCFGL18          (*((uword volatile far *) 0x200554))
#define CAN_MSGCFGL18_DIR                      0x0008
#define CAN_MSGCFGL18_NODE                     0x0002
#define CAN_MSGCFGL18_RMM                      0x0001
#define CAN_MSGCFGL18_XTD                      0x0004

// Message Object 19 Message Configuration Register Low
#define CAN_MSGCFGL19          (*((uword volatile far *) 0x200574))
#define CAN_MSGCFGL19_DIR                      0x0008
#define CAN_MSGCFGL19_NODE                     0x0002
#define CAN_MSGCFGL19_RMM                      0x0001
#define CAN_MSGCFGL19_XTD                      0x0004

// Message Object 2 Message Configuration Register Low
#define CAN_MSGCFGL2           (*((uword volatile far *) 0x200354))
#define CAN_MSGCFGL2_DIR                      0x0008
#define CAN_MSGCFGL2_NODE                     0x0002
#define CAN_MSGCFGL2_RMM                      0x0001
#define CAN_MSGCFGL2_XTD                      0x0004

// Message Object 20 Message Configuration Register Low
#define CAN_MSGCFGL20          (*((uword volatile far *) 0x200594))
#define CAN_MSGCFGL20_DIR                      0x0008
#define CAN_MSGCFGL20_NODE                     0x0002
#define CAN_MSGCFGL20_RMM                      0x0001
#define CAN_MSGCFGL20_XTD                      0x0004

// Message Object 21 Message Configuration Register Low
#define CAN_MSGCFGL21          (*((uword volatile far *) 0x2005B4))
#define CAN_MSGCFGL21_DIR                      0x0008
#define CAN_MSGCFGL21_NODE                     0x0002
#define CAN_MSGCFGL21_RMM                      0x0001
#define CAN_MSGCFGL21_XTD                      0x0004

// Message Object 22 Message Configuration Register Low
#define CAN_MSGCFGL22          (*((uword volatile far *) 0x2005D4))
#define CAN_MSGCFGL22_DIR                      0x0008
#define CAN_MSGCFGL22_NODE                     0x0002
#define CAN_MSGCFGL22_RMM                      0x0001
#define CAN_MSGCFGL22_XTD                      0x0004

// Message Object 23 Message Configuration Register Low
#define CAN_MSGCFGL23          (*((uword volatile far *) 0x2005F4))
#define CAN_MSGCFGL23_DIR                      0x0008
#define CAN_MSGCFGL23_NODE                     0x0002
#define CAN_MSGCFGL23_RMM                      0x0001
#define CAN_MSGCFGL23_XTD                      0x0004

// Message Object 24 Message Configuration Register Low
#define CAN_MSGCFGL24          (*((uword volatile far *) 0x200614))
#define CAN_MSGCFGL24_DIR                      0x0008
#define CAN_MSGCFGL24_NODE                     0x0002
#define CAN_MSGCFGL24_RMM                      0x0001
#define CAN_MSGCFGL24_XTD                      0x0004

// Message Object 25 Message Configuration Register Low
#define CAN_MSGCFGL25          (*((uword volatile far *) 0x200634))
#define CAN_MSGCFGL25_DIR                      0x0008
#define CAN_MSGCFGL25_NODE                     0x0002
#define CAN_MSGCFGL25_RMM                      0x0001
#define CAN_MSGCFGL25_XTD                      0x0004

// Message Object 26 Message Configuration Register Low
#define CAN_MSGCFGL26          (*((uword volatile far *) 0x200654))
#define CAN_MSGCFGL26_DIR                      0x0008
#define CAN_MSGCFGL26_NODE                     0x0002
#define CAN_MSGCFGL26_RMM                      0x0001
#define CAN_MSGCFGL26_XTD                      0x0004

// Message Object 27 Message Configuration Register Low
#define CAN_MSGCFGL27          (*((uword volatile far *) 0x200674))
#define CAN_MSGCFGL27_DIR                      0x0008
#define CAN_MSGCFGL27_NODE                     0x0002
#define CAN_MSGCFGL27_RMM                      0x0001
#define CAN_MSGCFGL27_XTD                      0x0004

// Message Object 28 Message Configuration Register Low
#define CAN_MSGCFGL28          (*((uword volatile far *) 0x200694))
#define CAN_MSGCFGL28_DIR                      0x0008
#define CAN_MSGCFGL28_NODE                     0x0002
#define CAN_MSGCFGL28_RMM                      0x0001
#define CAN_MSGCFGL28_XTD                      0x0004

// Message Object 29 Message Configuration Register Low
#define CAN_MSGCFGL29          (*((uword volatile far *) 0x2006B4))
#define CAN_MSGCFGL29_DIR                      0x0008
#define CAN_MSGCFGL29_NODE                     0x0002
#define CAN_MSGCFGL29_RMM                      0x0001
#define CAN_MSGCFGL29_XTD                      0x0004

// Message Object 3 Message Configuration Register Low
#define CAN_MSGCFGL3           (*((uword volatile far *) 0x200374))
#define CAN_MSGCFGL3_DIR                      0x0008
#define CAN_MSGCFGL3_NODE                     0x0002
#define CAN_MSGCFGL3_RMM                      0x0001
#define CAN_MSGCFGL3_XTD                      0x0004

// Message Object 30 Message Configuration Register Low
#define CAN_MSGCFGL30          (*((uword volatile far *) 0x2006D4))
#define CAN_MSGCFGL30_DIR                      0x0008
#define CAN_MSGCFGL30_NODE                     0x0002
#define CAN_MSGCFGL30_RMM                      0x0001
#define CAN_MSGCFGL30_XTD                      0x0004

// Message Object 31 Message Configuration Register Low
#define CAN_MSGCFGL31          (*((uword volatile far *) 0x2006F4))
#define CAN_MSGCFGL31_DIR                      0x0008
#define CAN_MSGCFGL31_NODE                     0x0002
#define CAN_MSGCFGL31_RMM                      0x0001
#define CAN_MSGCFGL31_XTD                      0x0004

// Message Object 4 Message Configuration Register Low
#define CAN_MSGCFGL4           (*((uword volatile far *) 0x200394))
#define CAN_MSGCFGL4_DIR                      0x0008
#define CAN_MSGCFGL4_NODE                     0x0002
#define CAN_MSGCFGL4_RMM                      0x0001
#define CAN_MSGCFGL4_XTD                      0x0004

// Message Object 5 Message Configuration Register Low
#define CAN_MSGCFGL5           (*((uword volatile far *) 0x2003B4))
#define CAN_MSGCFGL5_DIR                      0x0008
#define CAN_MSGCFGL5_NODE                     0x0002
#define CAN_MSGCFGL5_RMM                      0x0001
#define CAN_MSGCFGL5_XTD                      0x0004

// Message Object 6 Message Configuration Register Low
#define CAN_MSGCFGL6           (*((uword volatile far *) 0x2003D4))
#define CAN_MSGCFGL6_DIR                      0x0008
#define CAN_MSGCFGL6_NODE                     0x0002
#define CAN_MSGCFGL6_RMM                      0x0001
#define CAN_MSGCFGL6_XTD                      0x0004

// Message Object 7 Message Configuration Register Low
#define CAN_MSGCFGL7           (*((uword volatile far *) 0x2003F4))
#define CAN_MSGCFGL7_DIR                      0x0008
#define CAN_MSGCFGL7_NODE                     0x0002
#define CAN_MSGCFGL7_RMM                      0x0001
#define CAN_MSGCFGL7_XTD                      0x0004

// Message Object 8 Message Configuration Register Low
#define CAN_MSGCFGL8           (*((uword volatile far *) 0x200414))
#define CAN_MSGCFGL8_DIR                      0x0008
#define CAN_MSGCFGL8_NODE                     0x0002
#define CAN_MSGCFGL8_RMM                      0x0001
#define CAN_MSGCFGL8_XTD                      0x0004

// Message Object 9 Message Configuration Register Low
#define CAN_MSGCFGL9           (*((uword volatile far *) 0x200434))
#define CAN_MSGCFGL9_DIR                      0x0008
#define CAN_MSGCFGL9_NODE                     0x0002
#define CAN_MSGCFGL9_RMM                      0x0001
#define CAN_MSGCFGL9_XTD                      0x0004

// Message Object 0 Message Control Register High
#define CAN_MSGCTRH0           (*((uword volatile far *) 0x200312))

// Message Object 1 Message Control Register High
#define CAN_MSGCTRH1           (*((uword volatile far *) 0x200332))

// Message Object 10 Message Control Register High
#define CAN_MSGCTRH10          (*((uword volatile far *) 0x200452))

// Message Object 11 Message Control Register High
#define CAN_MSGCTRH11          (*((uword volatile far *) 0x200472))

// Message Object 12 Message Control Register High
#define CAN_MSGCTRH12          (*((uword volatile far *) 0x200492))

// Message Object 13 Message Control Register High
#define CAN_MSGCTRH13          (*((uword volatile far *) 0x2004B2))

// Message Object 14 Message Control Register High
#define CAN_MSGCTRH14          (*((uword volatile far *) 0x2004D2))

// Message Object 15 Message Control Register High
#define CAN_MSGCTRH15          (*((uword volatile far *) 0x2004F2))

// Message Object 16 Message Control Register High
#define CAN_MSGCTRH16          (*((uword volatile far *) 0x200512))

// Message Object 17 Message Control Register High
#define CAN_MSGCTRH17          (*((uword volatile far *) 0x200532))

// Message Object 18 Message Control Register High
#define CAN_MSGCTRH18          (*((uword volatile far *) 0x200552))

// Message Object 19 Message Control Register High
#define CAN_MSGCTRH19          (*((uword volatile far *) 0x200572))

// Message Object 2 Message Control Register High
#define CAN_MSGCTRH2           (*((uword volatile far *) 0x200352))

// Message Object 20 Message Control Register High
#define CAN_MSGCTRH20          (*((uword volatile far *) 0x200592))

// Message Object 21 Message Control Register High
#define CAN_MSGCTRH21          (*((uword volatile far *) 0x2005B2))

// Message Object 22 Message Control Register High
#define CAN_MSGCTRH22          (*((uword volatile far *) 0x2005D2))

// Message Object 23 Message Control Register High
#define CAN_MSGCTRH23          (*((uword volatile far *) 0x2005F2))

// Message Object 24 Message Control Register High
#define CAN_MSGCTRH24          (*((uword volatile far *) 0x200612))

// Message Object 25 Message Control Register High
#define CAN_MSGCTRH25          (*((uword volatile far *) 0x200632))

// Message Object 26 Message Control Register High
#define CAN_MSGCTRH26          (*((uword volatile far *) 0x200652))

// Message Object 27 Message Control Register High
#define CAN_MSGCTRH27          (*((uword volatile far *) 0x200672))

// Message Object 28 Message Control Register High
#define CAN_MSGCTRH28          (*((uword volatile far *) 0x200692))

// Message Object 29 Message Control Register High
#define CAN_MSGCTRH29          (*((uword volatile far *) 0x2006B2))

// Message Object 3 Message Control Register High
#define CAN_MSGCTRH3           (*((uword volatile far *) 0x200372))

// Message Object 30 Message Control Register High
#define CAN_MSGCTRH30          (*((uword volatile far *) 0x2006D2))

// Message Object 31 Message Control Register High
#define CAN_MSGCTRH31          (*((uword volatile far *) 0x2006F2))

// Message Object 4 Message Control Register High
#define CAN_MSGCTRH4           (*((uword volatile far *) 0x200392))

// Message Object 5 Message Control Register High
#define CAN_MSGCTRH5           (*((uword volatile far *) 0x2003B2))

// Message Object 6 Message Control Register High
#define CAN_MSGCTRH6           (*((uword volatile far *) 0x2003D2))

// Message Object 7 Message Control Register High
#define CAN_MSGCTRH7           (*((uword volatile far *) 0x2003F2))

// Message Object 8 Message Control Register High
#define CAN_MSGCTRH8           (*((uword volatile far *) 0x200412))

// Message Object 9 Message Control Register High
#define CAN_MSGCTRH9           (*((uword volatile far *) 0x200432))

// Message Object 0 Message Control Register Low
#define CAN_MSGCTRL0           (*((uword volatile far *) 0x200310))

// Message Object 1 Message Control Register Low
#define CAN_MSGCTRL1           (*((uword volatile far *) 0x200330))

// Message Object 10 Message Control Register Low
#define CAN_MSGCTRL10          (*((uword volatile far *) 0x200450))

// Message Object 11 Message Control Register Low
#define CAN_MSGCTRL11          (*((uword volatile far *) 0x200470))

// Message Object 12 Message Control Register Low
#define CAN_MSGCTRL12          (*((uword volatile far *) 0x200490))

// Message Object 13 Message Control Register Low
#define CAN_MSGCTRL13          (*((uword volatile far *) 0x2004B0))

// Message Object 14 Message Control Register Low
#define CAN_MSGCTRL14          (*((uword volatile far *) 0x2004D0))

// Message Object 15 Message Control Register Low
#define CAN_MSGCTRL15          (*((uword volatile far *) 0x2004F0))

// Message Object 16 Message Control Register Low
#define CAN_MSGCTRL16          (*((uword volatile far *) 0x200510))

// Message Object 17 Message Control Register Low
#define CAN_MSGCTRL17          (*((uword volatile far *) 0x200530))

// Message Object 18 Message Control Register Low
#define CAN_MSGCTRL18          (*((uword volatile far *) 0x200550))

// Message Object 19 Message Control Register Low
#define CAN_MSGCTRL19          (*((uword volatile far *) 0x200570))

// Message Object 2 Message Control Register Low
#define CAN_MSGCTRL2           (*((uword volatile far *) 0x200350))

// Message Object 20 Message Control Register Low
#define CAN_MSGCTRL20          (*((uword volatile far *) 0x200590))

// Message Object 21 Message Control Register Low
#define CAN_MSGCTRL21          (*((uword volatile far *) 0x2005B0))

// Message Object 22 Message Control Register Low
#define CAN_MSGCTRL22          (*((uword volatile far *) 0x2005D0))

// Message Object 23 Message Control Register Low
#define CAN_MSGCTRL23          (*((uword volatile far *) 0x2005F0))

// Message Object 24 Message Control Register Low
#define CAN_MSGCTRL24          (*((uword volatile far *) 0x200610))

// Message Object 25 Message Control Register Low
#define CAN_MSGCTRL25          (*((uword volatile far *) 0x200630))

// Message Object 26 Message Control Register Low
#define CAN_MSGCTRL26          (*((uword volatile far *) 0x200650))

// Message Object 27 Message Control Register Low
#define CAN_MSGCTRL27          (*((uword volatile far *) 0x200670))

// Message Object 28 Message Control Register Low
#define CAN_MSGCTRL28          (*((uword volatile far *) 0x200690))

// Message Object 29 Message Control Register Low
#define CAN_MSGCTRL29          (*((uword volatile far *) 0x2006B0))

// Message Object 3 Message Control Register Low
#define CAN_MSGCTRL3           (*((uword volatile far *) 0x200370))

// Message Object 30 Message Control Register Low
#define CAN_MSGCTRL30          (*((uword volatile far *) 0x2006D0))

// Message Object 31 Message Control Register Low
#define CAN_MSGCTRL31          (*((uword volatile far *) 0x2006F0))

// Message Object 4 Message Control Register Low
#define CAN_MSGCTRL4           (*((uword volatile far *) 0x200390))

// Message Object 5 Message Control Register Low
#define CAN_MSGCTRL5           (*((uword volatile far *) 0x2003B0))

// Message Object 6 Message Control Register Low
#define CAN_MSGCTRL6           (*((uword volatile far *) 0x2003D0))

// Message Object 7 Message Control Register Low
#define CAN_MSGCTRL7           (*((uword volatile far *) 0x2003F0))

// Message Object 8 Message Control Register Low
#define CAN_MSGCTRL8           (*((uword volatile far *) 0x200410))

// Message Object 9 Message Control Register Low
#define CAN_MSGCTRL9           (*((uword volatile far *) 0x200430))

// Message Object 0 Data Register 0 High
#define CAN_MSGDRH00           (*((uword volatile far *) 0x200302))

// Message Object 0 Data Register 4 High
#define CAN_MSGDRH04           (*((uword volatile far *) 0x200306))

// Message Object 1 Data Register 0 High
#define CAN_MSGDRH10           (*((uword volatile far *) 0x200322))

// Message Object 10 Data Register 0 High
#define CAN_MSGDRH100          (*((uword volatile far *) 0x200442))

// Message Object 10 Data Register 4 High
#define CAN_MSGDRH104          (*((uword volatile far *) 0x200446))

// Message Object 11 Data Register 0 High
#define CAN_MSGDRH110          (*((uword volatile far *) 0x200462))

// Message Object 11 Data Register 4 High
#define CAN_MSGDRH114          (*((uword volatile far *) 0x200466))

// Message Object 12 Data Register 0 High
#define CAN_MSGDRH120          (*((uword volatile far *) 0x200482))

// Message Object 12 Data Register 4 High
#define CAN_MSGDRH124          (*((uword volatile far *) 0x200486))

// Message Object 13 Data Register 0 High
#define CAN_MSGDRH130          (*((uword volatile far *) 0x2004A2))

// Message Object 13 Data Register 4 High
#define CAN_MSGDRH134          (*((uword volatile far *) 0x2004A6))

// Message Object 1 Data Register 4 High
#define CAN_MSGDRH14           (*((uword volatile far *) 0x200326))

// Message Object 14 Data Register 0 High
#define CAN_MSGDRH140          (*((uword volatile far *) 0x2004C2))

// Message Object 14 Data Register 4 High
#define CAN_MSGDRH144          (*((uword volatile far *) 0x2004C6))

// Message Object 15 Data Register 0 High
#define CAN_MSGDRH150          (*((uword volatile far *) 0x2004E2))

// Message Object 15 Data Register 4 High
#define CAN_MSGDRH154          (*((uword volatile far *) 0x2004E6))

// Message Object 16 Data Register 0 High
#define CAN_MSGDRH160          (*((uword volatile far *) 0x200502))

// Message Object 16 Data Register 4 High
#define CAN_MSGDRH164          (*((uword volatile far *) 0x200506))

// Message Object 17 Data Register 0 High
#define CAN_MSGDRH170          (*((uword volatile far *) 0x200522))

// Message Object 17 Data Register 4 High
#define CAN_MSGDRH174          (*((uword volatile far *) 0x200526))

// Message Object 18 Data Register 0 High
#define CAN_MSGDRH180          (*((uword volatile far *) 0x200542))

// Message Object 18 Data Register 4 High
#define CAN_MSGDRH184          (*((uword volatile far *) 0x200546))

// Message Object 19 Data Register 0 High
#define CAN_MSGDRH190          (*((uword volatile far *) 0x200562))

// Message Object 19 Data Register 4 High
#define CAN_MSGDRH194          (*((uword volatile far *) 0x200566))

// Message Object 2 Data Register 0 High
#define CAN_MSGDRH20           (*((uword volatile far *) 0x200342))

// Message Object 20 Data Register 0 High
#define CAN_MSGDRH200          (*((uword volatile far *) 0x200582))

// Message Object 20 Data Register 4 High
#define CAN_MSGDRH204          (*((uword volatile far *) 0x200586))

// Message Object 21 Data Register 0 High
#define CAN_MSGDRH210          (*((uword volatile far *) 0x2005A2))

// Message Object 21 Data Register 4 High
#define CAN_MSGDRH214          (*((uword volatile far *) 0x2005A6))

// Message Object 22 Data Register 0 High
#define CAN_MSGDRH220          (*((uword volatile far *) 0x2005C2))

// Message Object 22 Data Register 4 High
#define CAN_MSGDRH224          (*((uword volatile far *) 0x2005C6))

// Message Object 23 Data Register 0 High
#define CAN_MSGDRH230          (*((uword volatile far *) 0x2005E2))

// Message Object 23 Data Register 4 High
#define CAN_MSGDRH234          (*((uword volatile far *) 0x2005E6))

// Message Object 2 Data Register 4 High
#define CAN_MSGDRH24           (*((uword volatile far *) 0x200346))

// Message Object 24 Data Register 0 High
#define CAN_MSGDRH240          (*((uword volatile far *) 0x200602))

// Message Object 24 Data Register 4 High
#define CAN_MSGDRH244          (*((uword volatile far *) 0x200606))

// Message Object 25 Data Register 0 High
#define CAN_MSGDRH250          (*((uword volatile far *) 0x200622))

// Message Object 25 Data Register 4 High
#define CAN_MSGDRH254          (*((uword volatile far *) 0x200626))

// Message Object 26 Data Register 0 High
#define CAN_MSGDRH260          (*((uword volatile far *) 0x200642))

// Message Object 26 Data Register 4 High
#define CAN_MSGDRH264          (*((uword volatile far *) 0x200646))

// Message Object 27 Data Register 0 High
#define CAN_MSGDRH270          (*((uword volatile far *) 0x200662))

// Message Object 27 Data Register 4 High
#define CAN_MSGDRH274          (*((uword volatile far *) 0x200666))

// Message Object 28 Data Register 0 High
#define CAN_MSGDRH280          (*((uword volatile far *) 0x200682))

// Message Object 28 Data Register 4 High
#define CAN_MSGDRH284          (*((uword volatile far *) 0x200686))

// Message Object 29 Data Register 0 High
#define CAN_MSGDRH290          (*((uword volatile far *) 0x2006A2))

// Message Object 29 Data Register 4 High
#define CAN_MSGDRH294          (*((uword volatile far *) 0x2006A6))

// Message Object 3 Data Register 0 High
#define CAN_MSGDRH30           (*((uword volatile far *) 0x200362))

// Message Object 30 Data Register 0 High
#define CAN_MSGDRH300          (*((uword volatile far *) 0x2006C2))

// Message Object 30 Data Register 4 High
#define CAN_MSGDRH304          (*((uword volatile far *) 0x2006C6))

// Message Object 31 Data Register 0 High
#define CAN_MSGDRH310          (*((uword volatile far *) 0x2006E2))

// Message Object 31 Data Register 4 High
#define CAN_MSGDRH314          (*((uword volatile far *) 0x2006E6))

// Message Object 3 Data Register 4 High
#define CAN_MSGDRH34           (*((uword volatile far *) 0x200366))

// Message Object 4 Data Register 0 High
#define CAN_MSGDRH40           (*((uword volatile far *) 0x200382))

// Message Object 4 Data Register 4 High
#define CAN_MSGDRH44           (*((uword volatile far *) 0x200386))

// Message Object 5 Data Register 0 High
#define CAN_MSGDRH50           (*((uword volatile far *) 0x2003A2))

// Message Object 5 Data Register 4 High
#define CAN_MSGDRH54           (*((uword volatile far *) 0x2003A6))

// Message Object 6 Data Register 0 High
#define CAN_MSGDRH60           (*((uword volatile far *) 0x2003C2))

// Message Object 6 Data Register 4 High
#define CAN_MSGDRH64           (*((uword volatile far *) 0x2003C6))

// Message Object 7 Data Register 0 High
#define CAN_MSGDRH70           (*((uword volatile far *) 0x2003E2))

// Message Object 7 Data Register 4 High
#define CAN_MSGDRH74           (*((uword volatile far *) 0x2003E6))

// Message Object 8 Data Register 0 High
#define CAN_MSGDRH80           (*((uword volatile far *) 0x200402))

// Message Object 8 Data Register 4 High
#define CAN_MSGDRH84           (*((uword volatile far *) 0x200406))

// Message Object 9 Data Register 0 High
#define CAN_MSGDRH90           (*((uword volatile far *) 0x200422))

// Message Object 9 Data Register 4 High
#define CAN_MSGDRH94           (*((uword volatile far *) 0x200426))

// Message Object 0 Data Register 0 Low
#define CAN_MSGDRL00           (*((uword volatile far *) 0x200300))

// Message Object 0 Data Register 4 Low
#define CAN_MSGDRL04           (*((uword volatile far *) 0x200304))

// Message Object 1 Data Register 0 Low
#define CAN_MSGDRL10           (*((uword volatile far *) 0x200320))

// Message Object 10 Data Register 0 Low
#define CAN_MSGDRL100          (*((uword volatile far *) 0x200440))

// Message Object 10 Data Register 4 Low
#define CAN_MSGDRL104          (*((uword volatile far *) 0x200444))

// Message Object 11 Data Register 0 Low
#define CAN_MSGDRL110          (*((uword volatile far *) 0x200460))

// Message Object 11 Data Register 4 Low
#define CAN_MSGDRL114          (*((uword volatile far *) 0x200464))

// Message Object 12 Data Register 0 Low
#define CAN_MSGDRL120          (*((uword volatile far *) 0x200480))

// Message Object 12 Data Register 4 Low
#define CAN_MSGDRL124          (*((uword volatile far *) 0x200484))

// Message Object 13 Data Register 0 Low
#define CAN_MSGDRL130          (*((uword volatile far *) 0x2004A0))

// Message Object 13 Data Register 4 Low
#define CAN_MSGDRL134          (*((uword volatile far *) 0x2004A4))

// Message Object 1 Data Register 4 Low
#define CAN_MSGDRL14           (*((uword volatile far *) 0x200324))

// Message Object 14 Data Register 0 Low
#define CAN_MSGDRL140          (*((uword volatile far *) 0x2004C0))

// Message Object 14 Data Register 4 Low
#define CAN_MSGDRL144          (*((uword volatile far *) 0x2004C4))

// Message Object 15 Data Register 0 Low
#define CAN_MSGDRL150          (*((uword volatile far *) 0x2004E0))

// Message Object 15 Data Register 4 Low
#define CAN_MSGDRL154          (*((uword volatile far *) 0x2004E4))

// Message Object 16 Data Register 0 Low
#define CAN_MSGDRL160          (*((uword volatile far *) 0x200500))

// Message Object 16 Data Register 4 Low
#define CAN_MSGDRL164          (*((uword volatile far *) 0x200504))

// Message Object 17 Data Register 0 Low
#define CAN_MSGDRL170          (*((uword volatile far *) 0x200520))

// Message Object 17 Data Register 4 Low
#define CAN_MSGDRL174          (*((uword volatile far *) 0x200524))

// Message Object 18 Data Register 0 Low
#define CAN_MSGDRL180          (*((uword volatile far *) 0x200540))

// Message Object 18 Data Register 4 Low
#define CAN_MSGDRL184          (*((uword volatile far *) 0x200544))

// Message Object 19 Data Register 0 Low
#define CAN_MSGDRL190          (*((uword volatile far *) 0x200560))

// Message Object 19 Data Register 4 Low
#define CAN_MSGDRL194          (*((uword volatile far *) 0x200564))

// Message Object 2 Data Register 0 Low
#define CAN_MSGDRL20           (*((uword volatile far *) 0x200340))

// Message Object 20 Data Register 0 Low
#define CAN_MSGDRL200          (*((uword volatile far *) 0x200580))

// Message Object 20 Data Register 4 Low
#define CAN_MSGDRL204          (*((uword volatile far *) 0x200584))

// Message Object 21 Data Register 0 Low
#define CAN_MSGDRL210          (*((uword volatile far *) 0x2005A0))

// Message Object 21 Data Register 4 Low
#define CAN_MSGDRL214          (*((uword volatile far *) 0x2005A4))

// Message Object 22 Data Register 0 Low
#define CAN_MSGDRL220          (*((uword volatile far *) 0x2005C0))

// Message Object 22 Data Register 4 Low
#define CAN_MSGDRL224          (*((uword volatile far *) 0x2005C4))

// Message Object 23 Data Register 0 Low
#define CAN_MSGDRL230          (*((uword volatile far *) 0x2005E0))

// Message Object 23 Data Register 4 Low
#define CAN_MSGDRL234          (*((uword volatile far *) 0x2005E4))

// Message Object 2 Data Register 4 Low
#define CAN_MSGDRL24           (*((uword volatile far *) 0x200344))

// Message Object 24 Data Register 0 Low
#define CAN_MSGDRL240          (*((uword volatile far *) 0x200600))

// Message Object 24 Data Register 4 Low
#define CAN_MSGDRL244          (*((uword volatile far *) 0x200604))

// Message Object 25 Data Register 0 Low
#define CAN_MSGDRL250          (*((uword volatile far *) 0x200620))

// Message Object 25 Data Register 4 Low
#define CAN_MSGDRL254          (*((uword volatile far *) 0x200624))

// Message Object 26 Data Register 0 Low
#define CAN_MSGDRL260          (*((uword volatile far *) 0x200640))

// Message Object 26 Data Register 4 Low
#define CAN_MSGDRL264          (*((uword volatile far *) 0x200644))

// Message Object 27 Data Register 0 Low
#define CAN_MSGDRL270          (*((uword volatile far *) 0x200660))

// Message Object 27 Data Register 4 Low
#define CAN_MSGDRL274          (*((uword volatile far *) 0x200664))

// Message Object 28 Data Register 0 Low
#define CAN_MSGDRL280          (*((uword volatile far *) 0x200680))

// Message Object 28 Data Register 4 Low
#define CAN_MSGDRL284          (*((uword volatile far *) 0x200684))

// Message Object 29 Data Register 0 Low
#define CAN_MSGDRL290          (*((uword volatile far *) 0x2006A0))

// Message Object 29 Data Register 4 Low
#define CAN_MSGDRL294          (*((uword volatile far *) 0x2006A4))

// Message Object 3 Data Register 0 Low
#define CAN_MSGDRL30           (*((uword volatile far *) 0x200360))

// Message Object 30 Data Register 0 Low
#define CAN_MSGDRL300          (*((uword volatile far *) 0x2006C0))

// Message Object 30 Data Register 4 Low
#define CAN_MSGDRL304          (*((uword volatile far *) 0x2006C4))

// Message Object 31 Data Register 0 Low
#define CAN_MSGDRL310          (*((uword volatile far *) 0x2006E0))

// Message Object 31 Data Register 4 Low
#define CAN_MSGDRL314          (*((uword volatile far *) 0x2006E4))

// Message Object 3 Data Register 4 Low
#define CAN_MSGDRL34           (*((uword volatile far *) 0x200364))

// Message Object 4 Data Register 0 Low
#define CAN_MSGDRL40           (*((uword volatile far *) 0x200380))

// Message Object 4 Data Register 4 Low
#define CAN_MSGDRL44           (*((uword volatile far *) 0x200384))

// Message Object 5 Data Register 0 Low
#define CAN_MSGDRL50           (*((uword volatile far *) 0x2003A0))

// Message Object 5 Data Register 4 Low
#define CAN_MSGDRL54           (*((uword volatile far *) 0x2003A4))

// Message Object 6 Data Register 0 Low
#define CAN_MSGDRL60           (*((uword volatile far *) 0x2003C0))

// Message Object 6 Data Register 4 Low
#define CAN_MSGDRL64           (*((uword volatile far *) 0x2003C4))

// Message Object 7 Data Register 0 Low
#define CAN_MSGDRL70           (*((uword volatile far *) 0x2003E0))

// Message Object 7 Data Register 4 Low
#define CAN_MSGDRL74           (*((uword volatile far *) 0x2003E4))

// Message Object 8 Data Register 0 Low
#define CAN_MSGDRL80           (*((uword volatile far *) 0x200400))

// Message Object 8 Data Register 4 Low
#define CAN_MSGDRL84           (*((uword volatile far *) 0x200404))

// Message Object 9 Data Register 0 Low
#define CAN_MSGDRL90           (*((uword volatile far *) 0x200420))

// Message Object 9 Data Register 4 Low
#define CAN_MSGDRL94           (*((uword volatile far *) 0x200424))

// Message Object 0 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH0          (*((uword volatile far *) 0x20031A))

// Message Object 1 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH1          (*((uword volatile far *) 0x20033A))

// Message Object 10 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH10         (*((uword volatile far *) 0x20045A))

// Message Object 11 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH11         (*((uword volatile far *) 0x20047A))

// Message Object 12 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH12         (*((uword volatile far *) 0x20049A))

// Message Object 13 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH13         (*((uword volatile far *) 0x2004BA))

// Message Object 14 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH14         (*((uword volatile far *) 0x2004DA))

// Message Object 15 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH15         (*((uword volatile far *) 0x2004FA))

// Message Object 16 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH16         (*((uword volatile far *) 0x20051A))

// Message Object 17 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH17         (*((uword volatile far *) 0x20053A))

// Message Object 18 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH18         (*((uword volatile far *) 0x20055A))

// Message Object 19 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH19         (*((uword volatile far *) 0x20057A))

// Message Object 2 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH2          (*((uword volatile far *) 0x20035A))

// Message Object 20 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH20         (*((uword volatile far *) 0x20059A))

// Message Object 21 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH21         (*((uword volatile far *) 0x2005BA))

// Message Object 22 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH22         (*((uword volatile far *) 0x2005DA))

// Message Object 23 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH23         (*((uword volatile far *) 0x2005FA))

// Message Object 24 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH24         (*((uword volatile far *) 0x20061A))

// Message Object 25 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH25         (*((uword volatile far *) 0x20063A))

// Message Object 26 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH26         (*((uword volatile far *) 0x20065A))

// Message Object 27 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH27         (*((uword volatile far *) 0x20067A))

// Message Object 28 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH28         (*((uword volatile far *) 0x20069A))

// Message Object 29 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH29         (*((uword volatile far *) 0x2006BA))

// Message Object 3 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH3          (*((uword volatile far *) 0x20037A))

// Message Object 30 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH30         (*((uword volatile far *) 0x2006DA))

// Message Object 31 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH31         (*((uword volatile far *) 0x2006FA))

// Message Object 4 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH4          (*((uword volatile far *) 0x20039A))

// Message Object 5 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH5          (*((uword volatile far *) 0x2003BA))

// Message Object 6 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH6          (*((uword volatile far *) 0x2003DA))

// Message Object 7 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH7          (*((uword volatile far *) 0x2003FA))

// Message Object 8 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH8          (*((uword volatile far *) 0x20041A))

// Message Object 9 FIFO/Gateway Control Register High
#define CAN_MSGFGCRH9          (*((uword volatile far *) 0x20043A))

// Message Object 0 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL0          (*((uword volatile far *) 0x200318))
#define CAN_MSGFGCRL0_DLCC                     0x0800
#define CAN_MSGFGCRL0_FD                       0x2000
#define CAN_MSGFGCRL0_GDFS                     0x0100
#define CAN_MSGFGCRL0_IDC                      0x0400
#define CAN_MSGFGCRL0_SDT                      0x4000
#define CAN_MSGFGCRL0_SRREN                    0x0200
#define CAN_MSGFGCRL0_STT                      0x8000

// Message Object 1 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL1          (*((uword volatile far *) 0x200338))
#define CAN_MSGFGCRL1_DLCC                     0x0800
#define CAN_MSGFGCRL1_FD                       0x2000
#define CAN_MSGFGCRL1_GDFS                     0x0100
#define CAN_MSGFGCRL1_IDC                      0x0400
#define CAN_MSGFGCRL1_SDT                      0x4000
#define CAN_MSGFGCRL1_SRREN                    0x0200
#define CAN_MSGFGCRL1_STT                      0x8000

// Message Object 10 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL10         (*((uword volatile far *) 0x200458))
#define CAN_MSGFGCRL10_DLCC                     0x0800
#define CAN_MSGFGCRL10_FD                       0x2000
#define CAN_MSGFGCRL10_GDFS                     0x0100
#define CAN_MSGFGCRL10_IDC                      0x0400
#define CAN_MSGFGCRL10_SDT                      0x4000
#define CAN_MSGFGCRL10_SRREN                    0x0200
#define CAN_MSGFGCRL10_STT                      0x8000

// Message Object 11 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL11         (*((uword volatile far *) 0x200478))
#define CAN_MSGFGCRL11_DLCC                     0x0800
#define CAN_MSGFGCRL11_FD                       0x2000
#define CAN_MSGFGCRL11_GDFS                     0x0100
#define CAN_MSGFGCRL11_IDC                      0x0400
#define CAN_MSGFGCRL11_SDT                      0x4000
#define CAN_MSGFGCRL11_SRREN                    0x0200
#define CAN_MSGFGCRL11_STT                      0x8000

// Message Object 12 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL12         (*((uword volatile far *) 0x200498))
#define CAN_MSGFGCRL12_DLCC                     0x0800
#define CAN_MSGFGCRL12_FD                       0x2000
#define CAN_MSGFGCRL12_GDFS                     0x0100
#define CAN_MSGFGCRL12_IDC                      0x0400
#define CAN_MSGFGCRL12_SDT                      0x4000
#define CAN_MSGFGCRL12_SRREN                    0x0200
#define CAN_MSGFGCRL12_STT                      0x8000

// Message Object 13 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL13         (*((uword volatile far *) 0x2004B8))
#define CAN_MSGFGCRL13_DLCC                     0x0800
#define CAN_MSGFGCRL13_FD                       0x2000
#define CAN_MSGFGCRL13_GDFS                     0x0100
#define CAN_MSGFGCRL13_IDC                      0x0400
#define CAN_MSGFGCRL13_SDT                      0x4000
#define CAN_MSGFGCRL13_SRREN                    0x0200
#define CAN_MSGFGCRL13_STT                      0x8000

// Message Object 14 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL14         (*((uword volatile far *) 0x2004D8))
#define CAN_MSGFGCRL14_DLCC                     0x0800
#define CAN_MSGFGCRL14_FD                       0x2000
#define CAN_MSGFGCRL14_GDFS                     0x0100
#define CAN_MSGFGCRL14_IDC                      0x0400
#define CAN_MSGFGCRL14_SDT                      0x4000
#define CAN_MSGFGCRL14_SRREN                    0x0200
#define CAN_MSGFGCRL14_STT                      0x8000

// Message Object 15 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL15         (*((uword volatile far *) 0x2004F8))
#define CAN_MSGFGCRL15_DLCC                     0x0800
#define CAN_MSGFGCRL15_FD                       0x2000
#define CAN_MSGFGCRL15_GDFS                     0x0100
#define CAN_MSGFGCRL15_IDC                      0x0400
#define CAN_MSGFGCRL15_SDT                      0x4000
#define CAN_MSGFGCRL15_SRREN                    0x0200
#define CAN_MSGFGCRL15_STT                      0x8000

// Message Object 16 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL16         (*((uword volatile far *) 0x200518))
#define CAN_MSGFGCRL16_DLCC                     0x0800
#define CAN_MSGFGCRL16_FD                       0x2000
#define CAN_MSGFGCRL16_GDFS                     0x0100
#define CAN_MSGFGCRL16_IDC                      0x0400
#define CAN_MSGFGCRL16_SDT                      0x4000
#define CAN_MSGFGCRL16_SRREN                    0x0200
#define CAN_MSGFGCRL16_STT                      0x8000

// Message Object 17 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL17         (*((uword volatile far *) 0x200538))
#define CAN_MSGFGCRL17_DLCC                     0x0800
#define CAN_MSGFGCRL17_FD                       0x2000
#define CAN_MSGFGCRL17_GDFS                     0x0100
#define CAN_MSGFGCRL17_IDC                      0x0400
#define CAN_MSGFGCRL17_SDT                      0x4000
#define CAN_MSGFGCRL17_SRREN                    0x0200
#define CAN_MSGFGCRL17_STT                      0x8000

// Message Object 18 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL18         (*((uword volatile far *) 0x200558))
#define CAN_MSGFGCRL18_DLCC                     0x0800
#define CAN_MSGFGCRL18_FD                       0x2000
#define CAN_MSGFGCRL18_GDFS                     0x0100
#define CAN_MSGFGCRL18_IDC                      0x0400
#define CAN_MSGFGCRL18_SDT                      0x4000
#define CAN_MSGFGCRL18_SRREN                    0x0200
#define CAN_MSGFGCRL18_STT                      0x8000

// Message Object 19 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL19         (*((uword volatile far *) 0x200578))
#define CAN_MSGFGCRL19_DLCC                     0x0800
#define CAN_MSGFGCRL19_FD                       0x2000
#define CAN_MSGFGCRL19_GDFS                     0x0100
#define CAN_MSGFGCRL19_IDC                      0x0400
#define CAN_MSGFGCRL19_SDT                      0x4000
#define CAN_MSGFGCRL19_SRREN                    0x0200
#define CAN_MSGFGCRL19_STT                      0x8000

// Message Object 2 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL2          (*((uword volatile far *) 0x200358))
#define CAN_MSGFGCRL2_DLCC                     0x0800
#define CAN_MSGFGCRL2_FD                       0x2000
#define CAN_MSGFGCRL2_GDFS                     0x0100
#define CAN_MSGFGCRL2_IDC                      0x0400
#define CAN_MSGFGCRL2_SDT                      0x4000
#define CAN_MSGFGCRL2_SRREN                    0x0200
#define CAN_MSGFGCRL2_STT                      0x8000

// Message Object 20 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL20         (*((uword volatile far *) 0x200598))
#define CAN_MSGFGCRL20_DLCC                     0x0800
#define CAN_MSGFGCRL20_FD                       0x2000
#define CAN_MSGFGCRL20_GDFS                     0x0100
#define CAN_MSGFGCRL20_IDC                      0x0400
#define CAN_MSGFGCRL20_SDT                      0x4000
#define CAN_MSGFGCRL20_SRREN                    0x0200
#define CAN_MSGFGCRL20_STT                      0x8000

// Message Object 21 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL21         (*((uword volatile far *) 0x2005B8))
#define CAN_MSGFGCRL21_DLCC                     0x0800
#define CAN_MSGFGCRL21_FD                       0x2000
#define CAN_MSGFGCRL21_GDFS                     0x0100
#define CAN_MSGFGCRL21_IDC                      0x0400
#define CAN_MSGFGCRL21_SDT                      0x4000
#define CAN_MSGFGCRL21_SRREN                    0x0200
#define CAN_MSGFGCRL21_STT                      0x8000

// Message Object 22 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL22         (*((uword volatile far *) 0x2005D8))
#define CAN_MSGFGCRL22_DLCC                     0x0800
#define CAN_MSGFGCRL22_FD                       0x2000
#define CAN_MSGFGCRL22_GDFS                     0x0100
#define CAN_MSGFGCRL22_IDC                      0x0400
#define CAN_MSGFGCRL22_SDT                      0x4000
#define CAN_MSGFGCRL22_SRREN                    0x0200
#define CAN_MSGFGCRL22_STT                      0x8000

// Message Object 23 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL23         (*((uword volatile far *) 0x2005F8))
#define CAN_MSGFGCRL23_DLCC                     0x0800
#define CAN_MSGFGCRL23_FD                       0x2000
#define CAN_MSGFGCRL23_GDFS                     0x0100
#define CAN_MSGFGCRL23_IDC                      0x0400
#define CAN_MSGFGCRL23_SDT                      0x4000
#define CAN_MSGFGCRL23_SRREN                    0x0200
#define CAN_MSGFGCRL23_STT                      0x8000

// Message Object 24 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL24         (*((uword volatile far *) 0x200618))
#define CAN_MSGFGCRL24_DLCC                     0x0800
#define CAN_MSGFGCRL24_FD                       0x2000
#define CAN_MSGFGCRL24_GDFS                     0x0100
#define CAN_MSGFGCRL24_IDC                      0x0400
#define CAN_MSGFGCRL24_SDT                      0x4000
#define CAN_MSGFGCRL24_SRREN                    0x0200
#define CAN_MSGFGCRL24_STT                      0x8000

// Message Object 25 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL25         (*((uword volatile far *) 0x200638))
#define CAN_MSGFGCRL25_DLCC                     0x0800
#define CAN_MSGFGCRL25_FD                       0x2000
#define CAN_MSGFGCRL25_GDFS                     0x0100
#define CAN_MSGFGCRL25_IDC                      0x0400
#define CAN_MSGFGCRL25_SDT                      0x4000
#define CAN_MSGFGCRL25_SRREN                    0x0200
#define CAN_MSGFGCRL25_STT                      0x8000

// Message Object 26 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL26         (*((uword volatile far *) 0x200658))
#define CAN_MSGFGCRL26_DLCC                     0x0800
#define CAN_MSGFGCRL26_FD                       0x2000
#define CAN_MSGFGCRL26_GDFS                     0x0100
#define CAN_MSGFGCRL26_IDC                      0x0400
#define CAN_MSGFGCRL26_SDT                      0x4000
#define CAN_MSGFGCRL26_SRREN                    0x0200
#define CAN_MSGFGCRL26_STT                      0x8000

// Message Object 27 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL27         (*((uword volatile far *) 0x200678))
#define CAN_MSGFGCRL27_DLCC                     0x0800
#define CAN_MSGFGCRL27_FD                       0x2000
#define CAN_MSGFGCRL27_GDFS                     0x0100
#define CAN_MSGFGCRL27_IDC                      0x0400
#define CAN_MSGFGCRL27_SDT                      0x4000
#define CAN_MSGFGCRL27_SRREN                    0x0200
#define CAN_MSGFGCRL27_STT                      0x8000

// Message Object 28 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL28         (*((uword volatile far *) 0x200698))
#define CAN_MSGFGCRL28_DLCC                     0x0800
#define CAN_MSGFGCRL28_FD                       0x2000
#define CAN_MSGFGCRL28_GDFS                     0x0100
#define CAN_MSGFGCRL28_IDC                      0x0400
#define CAN_MSGFGCRL28_SDT                      0x4000
#define CAN_MSGFGCRL28_SRREN                    0x0200
#define CAN_MSGFGCRL28_STT                      0x8000

// Message Object 29 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL29         (*((uword volatile far *) 0x2006B8))
#define CAN_MSGFGCRL29_DLCC                     0x0800
#define CAN_MSGFGCRL29_FD                       0x2000
#define CAN_MSGFGCRL29_GDFS                     0x0100
#define CAN_MSGFGCRL29_IDC                      0x0400
#define CAN_MSGFGCRL29_SDT                      0x4000
#define CAN_MSGFGCRL29_SRREN                    0x0200
#define CAN_MSGFGCRL29_STT                      0x8000

// Message Object 3 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL3          (*((uword volatile far *) 0x200378))
#define CAN_MSGFGCRL3_DLCC                     0x0800
#define CAN_MSGFGCRL3_FD                       0x2000
#define CAN_MSGFGCRL3_GDFS                     0x0100
#define CAN_MSGFGCRL3_IDC                      0x0400
#define CAN_MSGFGCRL3_SDT                      0x4000
#define CAN_MSGFGCRL3_SRREN                    0x0200
#define CAN_MSGFGCRL3_STT                      0x8000

// Message Object 30 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL30         (*((uword volatile far *) 0x2006D8))
#define CAN_MSGFGCRL30_DLCC                     0x0800
#define CAN_MSGFGCRL30_FD                       0x2000
#define CAN_MSGFGCRL30_GDFS                     0x0100
#define CAN_MSGFGCRL30_IDC                      0x0400
#define CAN_MSGFGCRL30_SDT                      0x4000
#define CAN_MSGFGCRL30_SRREN                    0x0200
#define CAN_MSGFGCRL30_STT                      0x8000

// Message Object 31 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL31         (*((uword volatile far *) 0x2006F8))
#define CAN_MSGFGCRL31_DLCC                     0x0800
#define CAN_MSGFGCRL31_FD                       0x2000
#define CAN_MSGFGCRL31_GDFS                     0x0100
#define CAN_MSGFGCRL31_IDC                      0x0400
#define CAN_MSGFGCRL31_SDT                      0x4000
#define CAN_MSGFGCRL31_SRREN                    0x0200
#define CAN_MSGFGCRL31_STT                      0x8000

// Message Object 4 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL4          (*((uword volatile far *) 0x200398))
#define CAN_MSGFGCRL4_DLCC                     0x0800
#define CAN_MSGFGCRL4_FD                       0x2000
#define CAN_MSGFGCRL4_GDFS                     0x0100
#define CAN_MSGFGCRL4_IDC                      0x0400
#define CAN_MSGFGCRL4_SDT                      0x4000
#define CAN_MSGFGCRL4_SRREN                    0x0200
#define CAN_MSGFGCRL4_STT                      0x8000

// Message Object 5 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL5          (*((uword volatile far *) 0x2003B8))
#define CAN_MSGFGCRL5_DLCC                     0x0800
#define CAN_MSGFGCRL5_FD                       0x2000
#define CAN_MSGFGCRL5_GDFS                     0x0100
#define CAN_MSGFGCRL5_IDC                      0x0400
#define CAN_MSGFGCRL5_SDT                      0x4000
#define CAN_MSGFGCRL5_SRREN                    0x0200
#define CAN_MSGFGCRL5_STT                      0x8000

// Message Object 6 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL6          (*((uword volatile far *) 0x2003D8))
#define CAN_MSGFGCRL6_DLCC                     0x0800
#define CAN_MSGFGCRL6_FD                       0x2000
#define CAN_MSGFGCRL6_GDFS                     0x0100
#define CAN_MSGFGCRL6_IDC                      0x0400
#define CAN_MSGFGCRL6_SDT                      0x4000
#define CAN_MSGFGCRL6_SRREN                    0x0200
#define CAN_MSGFGCRL6_STT                      0x8000

// Message Object 7 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL7          (*((uword volatile far *) 0x2003F8))
#define CAN_MSGFGCRL7_DLCC                     0x0800
#define CAN_MSGFGCRL7_FD                       0x2000
#define CAN_MSGFGCRL7_GDFS                     0x0100
#define CAN_MSGFGCRL7_IDC                      0x0400
#define CAN_MSGFGCRL7_SDT                      0x4000
#define CAN_MSGFGCRL7_SRREN                    0x0200
#define CAN_MSGFGCRL7_STT                      0x8000

// Message Object 8 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL8          (*((uword volatile far *) 0x200418))
#define CAN_MSGFGCRL8_DLCC                     0x0800
#define CAN_MSGFGCRL8_FD                       0x2000
#define CAN_MSGFGCRL8_GDFS                     0x0100
#define CAN_MSGFGCRL8_IDC                      0x0400
#define CAN_MSGFGCRL8_SDT                      0x4000
#define CAN_MSGFGCRL8_SRREN                    0x0200
#define CAN_MSGFGCRL8_STT                      0x8000

// Message Object 9 FIFO/Gateway Control Register Low
#define CAN_MSGFGCRL9          (*((uword volatile far *) 0x200438))
#define CAN_MSGFGCRL9_DLCC                     0x0800
#define CAN_MSGFGCRL9_FD                       0x2000
#define CAN_MSGFGCRL9_GDFS                     0x0100
#define CAN_MSGFGCRL9_IDC                      0x0400
#define CAN_MSGFGCRL9_SDT                      0x4000
#define CAN_MSGFGCRL9_SRREN                    0x0200
#define CAN_MSGFGCRL9_STT                      0x8000

// TwinCAN Port Input Select Register
#define CAN_PISEL              (*((uword volatile far *) 0x200004))

// Receive Interrupt Pending Register High
#define CAN_RXIPNDH            (*((uword volatile far *) 0x200286))
#define CAN_RXIPNDH_RXIPND16                 0x0001
#define CAN_RXIPNDH_RXIPND17                 0x0002
#define CAN_RXIPNDH_RXIPND18                 0x0004
#define CAN_RXIPNDH_RXIPND19                 0x0008
#define CAN_RXIPNDH_RXIPND20                 0x0010
#define CAN_RXIPNDH_RXIPND21                 0x0020
#define CAN_RXIPNDH_RXIPND22                 0x0040
#define CAN_RXIPNDH_RXIPND23                 0x0080
#define CAN_RXIPNDH_RXIPND24                 0x0100
#define CAN_RXIPNDH_RXIPND25                 0x0200
#define CAN_RXIPNDH_RXIPND26                 0x0400
#define CAN_RXIPNDH_RXIPND27                 0x0800
#define CAN_RXIPNDH_RXIPND28                 0x1000
#define CAN_RXIPNDH_RXIPND29                 0x2000
#define CAN_RXIPNDH_RXIPND30                 0x4000
#define CAN_RXIPNDH_RXIPND31                 0x8000

// Receive Interrupt Pending Register Low
#define CAN_RXIPNDL            (*((uword volatile far *) 0x200284))
#define CAN_RXIPNDL_RXIPND0                  0x0001
#define CAN_RXIPNDL_RXIPND1                  0x0002
#define CAN_RXIPNDL_RXIPND10                 0x0400
#define CAN_RXIPNDL_RXIPND11                 0x0800
#define CAN_RXIPNDL_RXIPND12                 0x1000
#define CAN_RXIPNDL_RXIPND13                 0x2000
#define CAN_RXIPNDL_RXIPND14                 0x4000
#define CAN_RXIPNDL_RXIPND15                 0x8000
#define CAN_RXIPNDL_RXIPND2                  0x0004
#define CAN_RXIPNDL_RXIPND3                  0x0008
#define CAN_RXIPNDL_RXIPND4                  0x0010
#define CAN_RXIPNDL_RXIPND5                  0x0020
#define CAN_RXIPNDL_RXIPND6                  0x0040
#define CAN_RXIPNDL_RXIPND7                  0x0080
#define CAN_RXIPNDL_RXIPND8                  0x0100
#define CAN_RXIPNDL_RXIPND9                  0x0200

// Transmit Interrupt Pending Register High
#define CAN_TXIPNDH            (*((uword volatile far *) 0x20028A))
#define CAN_TXIPNDH_TXIPND16                 0x0001
#define CAN_TXIPNDH_TXIPND17                 0x0002
#define CAN_TXIPNDH_TXIPND18                 0x0004
#define CAN_TXIPNDH_TXIPND19                 0x0008
#define CAN_TXIPNDH_TXIPND20                 0x0010
#define CAN_TXIPNDH_TXIPND21                 0x0020
#define CAN_TXIPNDH_TXIPND22                 0x0040
#define CAN_TXIPNDH_TXIPND23                 0x0080
#define CAN_TXIPNDH_TXIPND24                 0x0100
#define CAN_TXIPNDH_TXIPND25                 0x0200
#define CAN_TXIPNDH_TXIPND26                 0x0400
#define CAN_TXIPNDH_TXIPND27                 0x0800
#define CAN_TXIPNDH_TXIPND28                 0x1000
#define CAN_TXIPNDH_TXIPND29                 0x2000
#define CAN_TXIPNDH_TXIPND30                 0x4000
#define CAN_TXIPNDH_TXIPND31                 0x8000

// Transmit Interrupt Pending Register Low
#define CAN_TXIPNDL            (*((uword volatile far *) 0x200288))
#define CAN_TXIPNDL_TXIPND0                  0x0001
#define CAN_TXIPNDL_TXIPND1                  0x0002
#define CAN_TXIPNDL_TXIPND10                 0x0400
#define CAN_TXIPNDL_TXIPND11                 0x0800
#define CAN_TXIPNDL_TXIPND12                 0x1000
#define CAN_TXIPNDL_TXIPND13                 0x2000
#define CAN_TXIPNDL_TXIPND14                 0x4000
#define CAN_TXIPNDL_TXIPND15                 0x8000
#define CAN_TXIPNDL_TXIPND2                  0x0004
#define CAN_TXIPNDL_TXIPND3                  0x0008
#define CAN_TXIPNDL_TXIPND4                  0x0010
#define CAN_TXIPNDL_TXIPND5                  0x0020
#define CAN_TXIPNDL_TXIPND6                  0x0040
#define CAN_TXIPNDL_TXIPND7                  0x0080
#define CAN_TXIPNDL_TXIPND8                  0x0100
#define CAN_TXIPNDL_TXIPND9                  0x0200

// CAPCOM 1 Register 0
#define CC1_CC0                (*((uword volatile *) 0xFE80))

// CAPCOM Channel 0 Interrupt Control Register
#define CC1_CC0IC              (*((uword volatile *) 0xFF78))
#define CC1_CC0IC_GPX                      ((T_Reg16 *) 0xFF78)->bit8
#define CC1_CC0IC_IE                       ((T_Reg16 *) 0xFF78)->bit6
#define CC1_CC0IC_IR                       ((T_Reg16 *) 0xFF78)->bit7

// CAPCOM 1 Register 1
#define CC1_CC1                (*((uword volatile *) 0xFE82))

// CAPCOM 1 Register 10
#define CC1_CC10               (*((uword volatile *) 0xFE94))

// CAPCOM Channel 10 Interrupt Control Register
#define CC1_CC10IC             (*((uword volatile *) 0xFF8C))
#define CC1_CC10IC_GPX                      ((T_Reg16 *) 0xFF8C)->bit8
#define CC1_CC10IC_IE                       ((T_Reg16 *) 0xFF8C)->bit6
#define CC1_CC10IC_IR                       ((T_Reg16 *) 0xFF8C)->bit7

// CAPCOM 1 Register 11
#define CC1_CC11               (*((uword volatile *) 0xFE96))

// CAPCOM Channel 11 Interrupt Control Register
#define CC1_CC11IC             (*((uword volatile *) 0xFF8E))
#define CC1_CC11IC_GPX                      ((T_Reg16 *) 0xFF8E)->bit8
#define CC1_CC11IC_IE                       ((T_Reg16 *) 0xFF8E)->bit6
#define CC1_CC11IC_IR                       ((T_Reg16 *) 0xFF8E)->bit7

// CAPCOM 1 Register 12
#define CC1_CC12               (*((uword volatile *) 0xFE98))

// CAPCOM Channel 12 Interrupt Control Register
#define CC1_CC12IC             (*((uword volatile *) 0xFF90))
#define CC1_CC12IC_GPX                      ((T_Reg16 *) 0xFF90)->bit8
#define CC1_CC12IC_IE                       ((T_Reg16 *) 0xFF90)->bit6
#define CC1_CC12IC_IR                       ((T_Reg16 *) 0xFF90)->bit7

// CAPCOM 1 Register 13
#define CC1_CC13               (*((uword volatile *) 0xFE9A))

// CAPCOM Channel 13 Interrupt Control Register
#define CC1_CC13IC             (*((uword volatile *) 0xFF92))
#define CC1_CC13IC_GPX                      ((T_Reg16 *) 0xFF92)->bit8
#define CC1_CC13IC_IE                       ((T_Reg16 *) 0xFF92)->bit6
#define CC1_CC13IC_IR                       ((T_Reg16 *) 0xFF92)->bit7

// CAPCOM 1 Register 14
#define CC1_CC14               (*((uword volatile *) 0xFE9C))

// CAPCOM Channel 14 Interrupt Control Register
#define CC1_CC14IC             (*((uword volatile *) 0xFF94))
#define CC1_CC14IC_GPX                      ((T_Reg16 *) 0xFF94)->bit8
#define CC1_CC14IC_IE                       ((T_Reg16 *) 0xFF94)->bit6
#define CC1_CC14IC_IR                       ((T_Reg16 *) 0xFF94)->bit7

// CAPCOM 1 Register 15
#define CC1_CC15               (*((uword volatile *) 0xFE9E))

// CAPCOM Channel 15 Interrupt Control Register
#define CC1_CC15IC             (*((uword volatile *) 0xFF96))
#define CC1_CC15IC_GPX                      ((T_Reg16 *) 0xFF96)->bit8
#define CC1_CC15IC_IE                       ((T_Reg16 *) 0xFF96)->bit6
#define CC1_CC15IC_IR                       ((T_Reg16 *) 0xFF96)->bit7

// CAPCOM Channel 1 Interrupt Control Register
#define CC1_CC1IC              (*((uword volatile *) 0xFF7A))
#define CC1_CC1IC_GPX                      ((T_Reg16 *) 0xFF7A)->bit8
#define CC1_CC1IC_IE                       ((T_Reg16 *) 0xFF7A)->bit6
#define CC1_CC1IC_IR                       ((T_Reg16 *) 0xFF7A)->bit7

// CAPCOM 1 Register 2
#define CC1_CC2                (*((uword volatile *) 0xFE84))

// CAPCOM Channel 2 Interrupt Control Register
#define CC1_CC2IC              (*((uword volatile *) 0xFF7C))
#define CC1_CC2IC_GPX                      ((T_Reg16 *) 0xFF7C)->bit8
#define CC1_CC2IC_IE                       ((T_Reg16 *) 0xFF7C)->bit6
#define CC1_CC2IC_IR                       ((T_Reg16 *) 0xFF7C)->bit7

// CAPCOM 1 Register 3
#define CC1_CC3                (*((uword volatile *) 0xFE86))

// CAPCOM Channel 3 Interrupt Control Register
#define CC1_CC3IC              (*((uword volatile *) 0xFF7E))
#define CC1_CC3IC_GPX                      ((T_Reg16 *) 0xFF7E)->bit8
#define CC1_CC3IC_IE                       ((T_Reg16 *) 0xFF7E)->bit6
#define CC1_CC3IC_IR                       ((T_Reg16 *) 0xFF7E)->bit7

// CAPCOM 1 Register 4
#define CC1_CC4                (*((uword volatile *) 0xFE88))

// CAPCOM Channel 4 Interrupt Control Register
#define CC1_CC4IC              (*((uword volatile *) 0xFF80))
#define CC1_CC4IC_GPX                      ((T_Reg16 *) 0xFF80)->bit8
#define CC1_CC4IC_IE                       ((T_Reg16 *) 0xFF80)->bit6
#define CC1_CC4IC_IR                       ((T_Reg16 *) 0xFF80)->bit7

// CAPCOM 1 Register 5
#define CC1_CC5                (*((uword volatile *) 0xFE8A))

// CAPCOM Channel 5 Interrupt Control Register
#define CC1_CC5IC              (*((uword volatile *) 0xFF82))
#define CC1_CC5IC_GPX                      ((T_Reg16 *) 0xFF82)->bit8
#define CC1_CC5IC_IE                       ((T_Reg16 *) 0xFF82)->bit6
#define CC1_CC5IC_IR                       ((T_Reg16 *) 0xFF82)->bit7

// CAPCOM 1 Register 6
#define CC1_CC6                (*((uword volatile *) 0xFE8C))

// CAPCOM Channel 6 Interrupt Control Register
#define CC1_CC6IC              (*((uword volatile *) 0xFF84))
#define CC1_CC6IC_GPX                      ((T_Reg16 *) 0xFF84)->bit8
#define CC1_CC6IC_IE                       ((T_Reg16 *) 0xFF84)->bit6
#define CC1_CC6IC_IR                       ((T_Reg16 *) 0xFF84)->bit7

// CAPCOM 1 Register 7
#define CC1_CC7                (*((uword volatile *) 0xFE8E))

// CC Register 7 Interrupt Control Register
#define CC1_CC7IC              (*((uword volatile *) 0xFF86))
#define CC1_CC7IC_GPX                      ((T_Reg16 *) 0xFF86)->bit8
#define CC1_CC7IC_IE                       ((T_Reg16 *) 0xFF86)->bit6
#define CC1_CC7IC_IR                       ((T_Reg16 *) 0xFF86)->bit7

// CAPCOM 1 Register 8
#define CC1_CC8                (*((uword volatile *) 0xFE90))

// CC Register 8 Interrupt Control Register
#define CC1_CC8IC              (*((uword volatile *) 0xFF88))
#define CC1_CC8IC_GPX                      ((T_Reg16 *) 0xFF88)->bit8
#define CC1_CC8IC_IE                       ((T_Reg16 *) 0xFF88)->bit6
#define CC1_CC8IC_IR                       ((T_Reg16 *) 0xFF88)->bit7

// CAPCOM 1 Register 9
#define CC1_CC9                (*((uword volatile *) 0xFE92))

// CC Register 9 Interrupt Control Register
#define CC1_CC9IC              (*((uword volatile *) 0xFF8A))
#define CC1_CC9IC_GPX                      ((T_Reg16 *) 0xFF8A)->bit8
#define CC1_CC9IC_IE                       ((T_Reg16 *) 0xFF8A)->bit6
#define CC1_CC9IC_IR                       ((T_Reg16 *) 0xFF8A)->bit7

// CAPCOM1 Double Register Mode Register
#define CC1_DRM                (*((uword volatile *) 0xFF5A))

// CAPCOM1 IO Control
#define CC1_IOC                (*((uword volatile *) 0xF062))
#define CC1_IOC_PL                       ((T_Reg16 *) 0xF062)->bit1
#define CC1_IOC_STAG                     ((T_Reg16 *) 0xF062)->bit2

// Capture/Compare Mode Registers for the CAPCOM Unit (CC0...CC3)
#define CC1_M0                 (*((uword volatile *) 0xFF52))
#define CC1_M0_ACC0                     ((T_Reg16 *) 0xFF52)->bit3
#define CC1_M0_ACC1                     ((T_Reg16 *) 0xFF52)->bit7
#define CC1_M0_ACC2                     ((T_Reg16 *) 0xFF52)->bit11
#define CC1_M0_ACC3                     ((T_Reg16 *) 0xFF52)->bit15

// Capture/Compare Mode Register for the CAPCOM Unit (CC4...CC79
#define CC1_M1                 (*((uword volatile *) 0xFF54))
#define CC1_M1_ACC4                     ((T_Reg16 *) 0xFF54)->bit3
#define CC1_M1_ACC5                     ((T_Reg16 *) 0xFF54)->bit7
#define CC1_M1_ACC6                     ((T_Reg16 *) 0xFF54)->bit11
#define CC1_M1_ACC7                     ((T_Reg16 *) 0xFF54)->bit15

// Capture/Compare Mode Registers for the CAPCOM Unit (CC8...CC11)
#define CC1_M2                 (*((uword volatile *) 0xFF56))
#define CC1_M2_ACC10                    ((T_Reg16 *) 0xFF56)->bit11
#define CC1_M2_ACC11                    ((T_Reg16 *) 0xFF56)->bit15
#define CC1_M2_ACC8                     ((T_Reg16 *) 0xFF56)->bit3
#define CC1_M2_ACC9                     ((T_Reg16 *) 0xFF56)->bit7

// Capture/Compare Mode Registers for the CAPCOM Unit (CC12...CC15)
#define CC1_M3                 (*((uword volatile *) 0xFF58))
#define CC1_M3_ACC12                    ((T_Reg16 *) 0xFF58)->bit3
#define CC1_M3_ACC13                    ((T_Reg16 *) 0xFF58)->bit7
#define CC1_M3_ACC14                    ((T_Reg16 *) 0xFF58)->bit11
#define CC1_M3_ACC15                    ((T_Reg16 *) 0xFF58)->bit15

// CAPCOM1 Output Register
#define CC1_OUT                (*((uword volatile *) 0xFF5C))
#define CC1_OUT_CC0IO                    ((T_Reg16 *) 0xFF5C)->bit0
#define CC1_OUT_CC10IO                   ((T_Reg16 *) 0xFF5C)->bit10
#define CC1_OUT_CC11IO                   ((T_Reg16 *) 0xFF5C)->bit11
#define CC1_OUT_CC12IO                   ((T_Reg16 *) 0xFF5C)->bit12
#define CC1_OUT_CC13IO                   ((T_Reg16 *) 0xFF5C)->bit13
#define CC1_OUT_CC14IO                   ((T_Reg16 *) 0xFF5C)->bit14
#define CC1_OUT_CC15IO                   ((T_Reg16 *) 0xFF5C)->bit15
#define CC1_OUT_CC1IO                    ((T_Reg16 *) 0xFF5C)->bit1
#define CC1_OUT_CC2IO                    ((T_Reg16 *) 0xFF5C)->bit2
#define CC1_OUT_CC3IO                    ((T_Reg16 *) 0xFF5C)->bit3
#define CC1_OUT_CC4IO                    ((T_Reg16 *) 0xFF5C)->bit4
#define CC1_OUT_CC5IO                    ((T_Reg16 *) 0xFF5C)->bit5
#define CC1_OUT_CC6IO                    ((T_Reg16 *) 0xFF5C)->bit6
#define CC1_OUT_CC7IO                    ((T_Reg16 *) 0xFF5C)->bit7
#define CC1_OUT_CC8IO                    ((T_Reg16 *) 0xFF5C)->bit8
#define CC1_OUT_CC9IO                    ((T_Reg16 *) 0xFF5C)->bit9

// CAPCOM 1 Single Event Enable Register
#define CC1_SEE                (*((uword volatile *) 0xFE2E))

// CAPCOM 1 Single Event Control Register
#define CC1_SEM                (*((uword volatile *) 0xFE2C))

// CAPCOM 1 Timer 0 Register
#define CC1_T0                 (*((uword volatile *) 0xFE50))

// Timer 0/1 Control Register
#define CC1_T01CON             (*((uword volatile *) 0xFF50))
#define CC1_T01CON_0                        ((T_Reg16 *) 0xFF50)->bit7
#define CC1_T01CON_T0M                      ((T_Reg16 *) 0xFF50)->bit3
#define CC1_T01CON_T0R                      ((T_Reg16 *) 0xFF50)->bit6
#define CC1_T01CON_T1M                      ((T_Reg16 *) 0xFF50)->bit11
#define CC1_T01CON_T1R                      ((T_Reg16 *) 0xFF50)->bit14

// CAPCOM 1 Timer 0 Interrupt Control Register
#define CC1_T0IC               (*((uword volatile *) 0xFF9C))
#define CC1_T0IC_GPX                      ((T_Reg16 *) 0xFF9C)->bit8
#define CC1_T0IC_IE                       ((T_Reg16 *) 0xFF9C)->bit6
#define CC1_T0IC_IR                       ((T_Reg16 *) 0xFF9C)->bit7

// CAPCOM 1 Timer 0 Reload Register
#define CC1_T0REL              (*((uword volatile *) 0xFE54))

// CAPCOM 1 Timer 1 Register
#define CC1_T1                 (*((uword volatile *) 0xFE52))

// CC Timer 1 Interrupt Control Register
#define CC1_T1IC               (*((uword volatile *) 0xFF9E))
#define CC1_T1IC_GPX                      ((T_Reg16 *) 0xFF9E)->bit8
#define CC1_T1IC_IE                       ((T_Reg16 *) 0xFF9E)->bit6
#define CC1_T1IC_IR                       ((T_Reg16 *) 0xFF9E)->bit7

// CC Timer 1 Reloed Register
#define CC1_T1REL              (*((uword volatile *) 0xFE56))

// CAPCOM 2 Register 16
#define CC2_CC16               (*((uword volatile *) 0xFE60))

// CAPCOM Channel 16 Interrupt Control Register
#define CC2_CC16IC             (*((uword volatile *) 0xF160))
#define CC2_CC16IC_GPX                      ((T_Reg16 *) 0xF160)->bit8
#define CC2_CC16IC_IE                       ((T_Reg16 *) 0xF160)->bit6
#define CC2_CC16IC_IR                       ((T_Reg16 *) 0xF160)->bit7

// CAPCOM 2 Register 17
#define CC2_CC17               (*((uword volatile *) 0xFE62))

// CAPCOM Channel 17 Interrupt Control Register
#define CC2_CC17IC             (*((uword volatile *) 0xF162))
#define CC2_CC17IC_GPX                      ((T_Reg16 *) 0xF162)->bit8
#define CC2_CC17IC_IE                       ((T_Reg16 *) 0xF162)->bit6
#define CC2_CC17IC_IR                       ((T_Reg16 *) 0xF162)->bit7

// CAPCOM 2 Register 18
#define CC2_CC18               (*((uword volatile *) 0xFE64))

// CAPCOM Channel 18 Interrupt Control Register
#define CC2_CC18IC             (*((uword volatile *) 0xF164))
#define CC2_CC18IC_GPX                      ((T_Reg16 *) 0xF164)->bit8
#define CC2_CC18IC_IE                       ((T_Reg16 *) 0xF164)->bit6
#define CC2_CC18IC_IR                       ((T_Reg16 *) 0xF164)->bit7

// CAPCOM 2 Register 19
#define CC2_CC19               (*((uword volatile *) 0xFE66))

// CAPCOM Channel 19 Interrupt Control Register
#define CC2_CC19IC             (*((uword volatile *) 0xF166))
#define CC2_CC19IC_GPX                      ((T_Reg16 *) 0xF166)->bit8
#define CC2_CC19IC_IE                       ((T_Reg16 *) 0xF166)->bit6
#define CC2_CC19IC_IR                       ((T_Reg16 *) 0xF166)->bit7

// CAPCOM 2 Register 20
#define CC2_CC20               (*((uword volatile *) 0xFE68))

// CAPCOM Channel 20 Interrupt Control Register
#define CC2_CC20IC             (*((uword volatile *) 0xF168))
#define CC2_CC20IC_GPX                      ((T_Reg16 *) 0xF168)->bit8
#define CC2_CC20IC_IE                       ((T_Reg16 *) 0xF168)->bit6
#define CC2_CC20IC_IR                       ((T_Reg16 *) 0xF168)->bit7

// CAPCOM 2 Register 21
#define CC2_CC21               (*((uword volatile *) 0xFE6A))

// CAPCOM Channel 21 Interrupt Control Register
#define CC2_CC21IC             (*((uword volatile *) 0xF16A))
#define CC2_CC21IC_GPX                      ((T_Reg16 *) 0xF16A)->bit8
#define CC2_CC21IC_IE                       ((T_Reg16 *) 0xF16A)->bit6
#define CC2_CC21IC_IR                       ((T_Reg16 *) 0xF16A)->bit7

// CAPCOM 2 Register 22
#define CC2_CC22               (*((uword volatile *) 0xFE6C))

// CAPCOM Channel 22 Interrupt Control Register
#define CC2_CC22IC             (*((uword volatile *) 0xF16C))
#define CC2_CC22IC_GPX                      ((T_Reg16 *) 0xF16C)->bit8
#define CC2_CC22IC_IE                       ((T_Reg16 *) 0xF16C)->bit6
#define CC2_CC22IC_IR                       ((T_Reg16 *) 0xF16C)->bit7

// CAPCOM 2 Register 23
#define CC2_CC23               (*((uword volatile *) 0xFE6E))

// CAPCOM Channel 23 Interrupt Control Register
#define CC2_CC23IC             (*((uword volatile *) 0xF16E))
#define CC2_CC23IC_GPX                      ((T_Reg16 *) 0xF16E)->bit8
#define CC2_CC23IC_IE                       ((T_Reg16 *) 0xF16E)->bit6
#define CC2_CC23IC_IR                       ((T_Reg16 *) 0xF16E)->bit7

// CAPCOM 2 Register 24
#define CC2_CC24               (*((uword volatile *) 0xFE70))

// CAPCOM Channel 24 Interrupt Control Register
#define CC2_CC24IC             (*((uword volatile *) 0xF170))
#define CC2_CC24IC_GPX                      ((T_Reg16 *) 0xF170)->bit8
#define CC2_CC24IC_IE                       ((T_Reg16 *) 0xF170)->bit6
#define CC2_CC24IC_IR                       ((T_Reg16 *) 0xF170)->bit7

// CAPCOM 2 Register 25
#define CC2_CC25               (*((uword volatile *) 0xFE72))

// CAPCOM Channel 25 Interrupt Control Register
#define CC2_CC25IC             (*((uword volatile *) 0xF172))
#define CC2_CC25IC_GPX                      ((T_Reg16 *) 0xF172)->bit8
#define CC2_CC25IC_IE                       ((T_Reg16 *) 0xF172)->bit6
#define CC2_CC25IC_IR                       ((T_Reg16 *) 0xF172)->bit7

// CAPCOM 2 Register 26
#define CC2_CC26               (*((uword volatile *) 0xFE74))

// CAPCOM Channel 26 Interrupt Control Register
#define CC2_CC26IC             (*((uword volatile *) 0xF174))
#define CC2_CC26IC_GPX                      ((T_Reg16 *) 0xF174)->bit8
#define CC2_CC26IC_IE                       ((T_Reg16 *) 0xF174)->bit6
#define CC2_CC26IC_IR                       ((T_Reg16 *) 0xF174)->bit7

// CAPCOM 2 Register 27
#define CC2_CC27               (*((uword volatile *) 0xFE76))

// CAPCOM Channel 27 Interrupt Control Register
#define CC2_CC27IC             (*((uword volatile *) 0xF176))
#define CC2_CC27IC_GPX                      ((T_Reg16 *) 0xF176)->bit8
#define CC2_CC27IC_IE                       ((T_Reg16 *) 0xF176)->bit6
#define CC2_CC27IC_IR                       ((T_Reg16 *) 0xF176)->bit7

// CAPCOM 2 Register 28
#define CC2_CC28               (*((uword volatile *) 0xFE78))

// CAPCOM Channel 28 Interrupt Control Register
#define CC2_CC28IC             (*((uword volatile *) 0xF178))
#define CC2_CC28IC_GPX                      ((T_Reg16 *) 0xF178)->bit8
#define CC2_CC28IC_IE                       ((T_Reg16 *) 0xF178)->bit6
#define CC2_CC28IC_IR                       ((T_Reg16 *) 0xF178)->bit7

// CAPCOM 2 Register 29
#define CC2_CC29               (*((uword volatile *) 0xFE7A))

// CAPCOM Channel 29 Interrupt Control Register
#define CC2_CC29IC             (*((uword volatile *) 0xF184))
#define CC2_CC29IC_GPX                      ((T_Reg16 *) 0xF184)->bit8
#define CC2_CC29IC_IE                       ((T_Reg16 *) 0xF184)->bit6
#define CC2_CC29IC_IR                       ((T_Reg16 *) 0xF184)->bit7

// CAPCOM 2 Register 30
#define CC2_CC30               (*((uword volatile *) 0xFE7C))

// CAPCOM Channel 30 Interrupt Control Register
#define CC2_CC30IC             (*((uword volatile *) 0xF18C))
#define CC2_CC30IC_GPX                      ((T_Reg16 *) 0xF18C)->bit8
#define CC2_CC30IC_IE                       ((T_Reg16 *) 0xF18C)->bit6
#define CC2_CC30IC_IR                       ((T_Reg16 *) 0xF18C)->bit7

// CAPCOM 2 Register 31
#define CC2_CC31               (*((uword volatile *) 0xFE7E))

// CAPCOM Channel 31 Interrupt Control Register
#define CC2_CC31IC             (*((uword volatile *) 0xF194))
#define CC2_CC31IC_GPX                      ((T_Reg16 *) 0xF194)->bit8
#define CC2_CC31IC_IE                       ((T_Reg16 *) 0xF194)->bit6
#define CC2_CC31IC_IR                       ((T_Reg16 *) 0xF194)->bit7

// CAPCOM 2 Double Register Mode Register
#define CC2_DRM                (*((uword volatile *) 0xFF2A))

// CAPCOM2 IO Control
#define CC2_IOC                (*((uword volatile *) 0xF066))
#define CC2_IOC_PL                       ((T_Reg16 *) 0xF066)->bit1
#define CC2_IOC_STAG                     ((T_Reg16 *) 0xF066)->bit2

// CC Mode Control Register 4
#define CC2_M4                 (*((uword volatile *) 0xFF22))
#define CC2_M4_ACC16                    ((T_Reg16 *) 0xFF22)->bit3
#define CC2_M4_ACC17                    ((T_Reg16 *) 0xFF22)->bit7
#define CC2_M4_ACC18                    ((T_Reg16 *) 0xFF22)->bit11
#define CC2_M4_ACC19                    ((T_Reg16 *) 0xFF22)->bit15

// CC Mode Control Register 5
#define CC2_M5                 (*((uword volatile *) 0xFF24))
#define CC2_M5_ACC20                    ((T_Reg16 *) 0xFF24)->bit3
#define CC2_M5_ACC21                    ((T_Reg16 *) 0xFF24)->bit7
#define CC2_M5_ACC22                    ((T_Reg16 *) 0xFF24)->bit11
#define CC2_M5_ACC23                    ((T_Reg16 *) 0xFF24)->bit15

// CC Mode Control Register 6
#define CC2_M6                 (*((uword volatile *) 0xFF26))
#define CC2_M6_ACC24                    ((T_Reg16 *) 0xFF26)->bit3
#define CC2_M6_ACC25                    ((T_Reg16 *) 0xFF26)->bit7
#define CC2_M6_ACC26                    ((T_Reg16 *) 0xFF26)->bit11
#define CC2_M6_ACC27                    ((T_Reg16 *) 0xFF26)->bit15

// CC Mode Control Register 7
#define CC2_M7                 (*((uword volatile *) 0xFF28))
#define CC2_M7_ACC28                    ((T_Reg16 *) 0xFF28)->bit3
#define CC2_M7_ACC29                    ((T_Reg16 *) 0xFF28)->bit7
#define CC2_M7_ACC30                    ((T_Reg16 *) 0xFF28)->bit11
#define CC2_M7_ACC31                    ((T_Reg16 *) 0xFF28)->bit15

// CAPCOM 2 Output Register
#define CC2_OUT                (*((uword volatile *) 0xFF2C))
#define CC2_OUT_CC0IO                    ((T_Reg16 *) 0xFF2C)->bit0
#define CC2_OUT_CC10IO                   ((T_Reg16 *) 0xFF2C)->bit10
#define CC2_OUT_CC11IO                   ((T_Reg16 *) 0xFF2C)->bit11
#define CC2_OUT_CC12IO                   ((T_Reg16 *) 0xFF2C)->bit12
#define CC2_OUT_CC13IO                   ((T_Reg16 *) 0xFF2C)->bit13
#define CC2_OUT_CC14IO                   ((T_Reg16 *) 0xFF2C)->bit14
#define CC2_OUT_CC15IO                   ((T_Reg16 *) 0xFF2C)->bit15
#define CC2_OUT_CC1IO                    ((T_Reg16 *) 0xFF2C)->bit1
#define CC2_OUT_CC2IO                    ((T_Reg16 *) 0xFF2C)->bit2
#define CC2_OUT_CC3IO                    ((T_Reg16 *) 0xFF2C)->bit3
#define CC2_OUT_CC4IO                    ((T_Reg16 *) 0xFF2C)->bit4
#define CC2_OUT_CC5IO                    ((T_Reg16 *) 0xFF2C)->bit5
#define CC2_OUT_CC6IO                    ((T_Reg16 *) 0xFF2C)->bit6
#define CC2_OUT_CC7IO                    ((T_Reg16 *) 0xFF2C)->bit7
#define CC2_OUT_CC8IO                    ((T_Reg16 *) 0xFF2C)->bit8
#define CC2_OUT_CC9IO                    ((T_Reg16 *) 0xFF2C)->bit9

// CAPCOM 2 Single Event Enable Register
#define CC2_SEE                (*((uword volatile *) 0xFE2A))

// CAPCOM 2 Single Event Control Register
#define CC2_SEM                (*((uword volatile *) 0xFE28))

// CAPCOM 2 Timer 7 Register
#define CC2_T7                 (*((uword volatile *) 0xF050))

// CAPCOM 2 Timer 7 and Timer 8 Control Register
#define CC2_T78CON             (*((uword volatile *) 0xFF20))
#define CC2_T78CON_T7M                      ((T_Reg16 *) 0xFF20)->bit3
#define CC2_T78CON_T7R                      ((T_Reg16 *) 0xFF20)->bit6
#define CC2_T78CON_T8M                      ((T_Reg16 *) 0xFF20)->bit11
#define CC2_T78CON_T8R                      ((T_Reg16 *) 0xFF20)->bit14

// CAPCOM 2 Timer 7 Interrupt Control Register
#define CC2_T7IC               (*((uword volatile *) 0xF17A))
#define CC2_T7IC_GPX                      ((T_Reg16 *) 0xF17A)->bit8
#define CC2_T7IC_IE                       ((T_Reg16 *) 0xF17A)->bit6
#define CC2_T7IC_IR                       ((T_Reg16 *) 0xF17A)->bit7

// CAPCOM 2 Timer 7 Reload Register
#define CC2_T7REL              (*((uword volatile *) 0xF054))

// CAPCOM 2 Timer 8 Register
#define CC2_T8                 (*((uword volatile *) 0xF052))

// CAPCOM 2 Timer 8 Interrupt Control Register
#define CC2_T8IC               (*((uword volatile *) 0xF17C))
#define CC2_T8IC_GPX                      ((T_Reg16 *) 0xF17C)->bit8
#define CC2_T8IC_IE                       ((T_Reg16 *) 0xF17C)->bit6
#define CC2_T8IC_IR                       ((T_Reg16 *) 0xF17C)->bit7

// CAPCOM 2 Timer 8 Reload Register
#define CC2_T8REL              (*((uword volatile *) 0xF056))

// Capture/Compare Register for Channel CC60
#define CCU6_CC60R             (*((uword volatile *) 0xE898))

// Capture/Compare Shadow Register for Channel 0
#define CCU6_CC60SR            (*((uword volatile *) 0xE8A0))

// Capture/Compare Register for Channel CC61
#define CCU6_CC61R             (*((uword volatile *) 0xE89A))

// Capture/Compare Shadow Register for Channel 1
#define CCU6_CC61SR            (*((uword volatile *) 0xE8A2))

// Capture/Compare Register for Channel CC62
#define CCU6_CC62R             (*((uword volatile *) 0xE89C))

// Capture/Compare Shadow Register for Channel 2
#define CCU6_CC62SR            (*((uword volatile *) 0xE8A4))

// Compare Register for Channel CC63
#define CCU6_CC63R             (*((uword volatile *) 0xE8B4))

// Compare Shadow Register for Channel  CC63
#define CCU6_CC63SR            (*((uword volatile *) 0xE8B6))

// Compare State Modification Register
#define CCU6_CMPMODIF          (*((uword volatile *) 0xE8AA))

// Compare Status Register
#define CCU6_CMPSTAT           (*((uword volatile *) 0xE8A8))

// CAPCOM 6 Emergency Interrupt Control Register
#define CCU6_EIC               (*((uword volatile *) 0xF188))
#define CCU6_EIC_GPX                      ((T_Reg16 *) 0xF188)->bit8
#define CCU6_EIC_IE                       ((T_Reg16 *) 0xF188)->bit6
#define CCU6_EIC_IR                       ((T_Reg16 *) 0xF188)->bit7

// CAPCOM 6 Interrupt Control Register
#define CCU6_IC                (*((uword volatile *) 0xF140))
#define CCU6_IC_GPX                      ((T_Reg16 *) 0xF140)->bit8
#define CCU6_IC_IE                       ((T_Reg16 *) 0xF140)->bit6
#define CCU6_IC_IR                       ((T_Reg16 *) 0xF140)->bit7

// Capture/Compare Interrupt Enable Register
#define CCU6_IEN               (*((uword volatile *) 0xE8D8))

// Capture/Compare Interrupt Node Pointer Register
#define CCU6_INP               (*((uword volatile *) 0xE8D6))

// Capture/Compare Interrupt Status Register
#define CCU6_IS                (*((uword volatile *) 0xE8D0))

// Capture/Compare Interrupt Status Reset Register
#define CCU6_ISR               (*((uword volatile *) 0xE8D4))

// Capture/Compare Interrupt Status Set Register
#define CCU6_ISS               (*((uword volatile *) 0xE8D2))

// Multi-Channel Mode Control Register
#define CCU6_MCMCTR            (*((uword volatile *) 0xE8CE))

// Multi-Channel Mode Output Register
#define CCU6_MCMOUT            (*((uword volatile *) 0xE8CC))

// Multi-Channel Mode Output Shadow Register
#define CCU6_MCMOUTS           (*((uword volatile *) 0xE8CA))

// Modulation Control Register
#define CCU6_MODCTR            (*((uword volatile *) 0xE8C0))

// Passive State Level Register
#define CCU6_PSLR              (*((uword volatile *) 0xE8C4))

// Timer T12 Counter Register
#define CCU6_T12               (*((uword volatile *) 0xE890))

// Dead-Time Control Register for Timer 12
#define CCU6_T12DTC            (*((uword volatile *) 0xE894))

// CAPCOM 6 Timer 12 Interrupt Control Register
#define CCU6_T12IC             (*((uword volatile *) 0xF190))
#define CCU6_T12IC_GPX                      ((T_Reg16 *) 0xF190)->bit8
#define CCU6_T12IC_IE                       ((T_Reg16 *) 0xF190)->bit6
#define CCU6_T12IC_IR                       ((T_Reg16 *) 0xF190)->bit7

// T12 Capture/Compare Mode Select Register
#define CCU6_T12MSEL           (*((uword volatile *) 0xE8C6))

// Timer 12 Period Register
#define CCU6_T12PR             (*((uword volatile *) 0xE892))

// Timer T13 Counter Register
#define CCU6_T13               (*((uword volatile *) 0xE8B0))

// CAPCOM 6 Timer 13 Interrupt Control Register
#define CCU6_T13IC             (*((uword volatile *) 0xF198))
#define CCU6_T13IC_GPX                      ((T_Reg16 *) 0xF198)->bit8
#define CCU6_T13IC_IE                       ((T_Reg16 *) 0xF198)->bit6
#define CCU6_T13IC_IR                       ((T_Reg16 *) 0xF198)->bit7

// Timer 13 Period Register
#define CCU6_T13PR             (*((uword volatile *) 0xE8B2))

// Timer Control Register 0
#define CCU6_TCTR0             (*((uword volatile *) 0xE8AC))

// Timer Control Register 2
#define CCU6_TCTR2             (*((uword volatile *) 0xE8AE))

// Timer Control Register 4
#define CCU6_TCTR4             (*((uword volatile *) 0xE8A6))

// Trap Control Register
#define CCU6_TRPCTR            (*((uword volatile *) 0xE8C2))

// CPU Contex Pointer Register
#define CP                     (*((uword volatile *) 0xFE10))

// CPU Control Register 1
#define CPUCON1                (*((uword volatile *) 0xFE18))

// CPU Control Register 2
#define CPUCON2                (*((uword volatile *) 0xFE1A))

// CPU Code Segment Point Register
#define CSP                    (*((uword volatile *) 0xFE08))

// Port 0 High Direction Control Register
#define DP0H                   (*((uword volatile *) 0xF102))
#define DP0H_P0                       ((T_Reg16 *) 0xF102)->bit0
#define DP0H_P1                       ((T_Reg16 *) 0xF102)->bit1
#define DP0H_P2                       ((T_Reg16 *) 0xF102)->bit2
#define DP0H_P3                       ((T_Reg16 *) 0xF102)->bit3
#define DP0H_P4                       ((T_Reg16 *) 0xF102)->bit4
#define DP0H_P5                       ((T_Reg16 *) 0xF102)->bit5
#define DP0H_P6                       ((T_Reg16 *) 0xF102)->bit6
#define DP0H_P7                       ((T_Reg16 *) 0xF102)->bit7

// Port 0 Low Direction Control Register
#define DP0L                   (*((uword volatile *) 0xF100))
#define DP0L_P0                       ((T_Reg16 *) 0xF100)->bit0
#define DP0L_P1                       ((T_Reg16 *) 0xF100)->bit1
#define DP0L_P2                       ((T_Reg16 *) 0xF100)->bit2
#define DP0L_P3                       ((T_Reg16 *) 0xF100)->bit3
#define DP0L_P4                       ((T_Reg16 *) 0xF100)->bit4
#define DP0L_P5                       ((T_Reg16 *) 0xF100)->bit5
#define DP0L_P6                       ((T_Reg16 *) 0xF100)->bit6
#define DP0L_P7                       ((T_Reg16 *) 0xF100)->bit7

// Port 1 High Direction Control Register
#define DP1H                   (*((uword volatile *) 0xF106))
#define DP1H_P0                       ((T_Reg16 *) 0xF106)->bit0
#define DP1H_P1                       ((T_Reg16 *) 0xF106)->bit1
#define DP1H_P2                       ((T_Reg16 *) 0xF106)->bit2
#define DP1H_P3                       ((T_Reg16 *) 0xF106)->bit3
#define DP1H_P4                       ((T_Reg16 *) 0xF106)->bit4
#define DP1H_P5                       ((T_Reg16 *) 0xF106)->bit5
#define DP1H_P6                       ((T_Reg16 *) 0xF106)->bit6
#define DP1H_P7                       ((T_Reg16 *) 0xF106)->bit7

// Port 1 Low Direction Control Register
#define DP1L                   (*((uword volatile *) 0xF104))
#define DP1L_P0                       ((T_Reg16 *) 0xF104)->bit0
#define DP1L_P1                       ((T_Reg16 *) 0xF104)->bit1
#define DP1L_P2                       ((T_Reg16 *) 0xF104)->bit2
#define DP1L_P3                       ((T_Reg16 *) 0xF104)->bit3
#define DP1L_P4                       ((T_Reg16 *) 0xF104)->bit4
#define DP1L_P5                       ((T_Reg16 *) 0xF104)->bit5
#define DP1L_P6                       ((T_Reg16 *) 0xF104)->bit6
#define DP1L_P7                       ((T_Reg16 *) 0xF104)->bit7

// Port 2 Direction Control Register
#define DP2                    (*((uword volatile *) 0xFFC2))
#define DP2_P10                      ((T_Reg16 *) 0xFFC2)->bit10
#define DP2_P11                      ((T_Reg16 *) 0xFFC2)->bit11
#define DP2_P12                      ((T_Reg16 *) 0xFFC2)->bit12
#define DP2_P13                      ((T_Reg16 *) 0xFFC2)->bit13
#define DP2_P14                      ((T_Reg16 *) 0xFFC2)->bit14
#define DP2_P15                      ((T_Reg16 *) 0xFFC2)->bit15
#define DP2_P8                       ((T_Reg16 *) 0xFFC2)->bit8
#define DP2_P9                       ((T_Reg16 *) 0xFFC2)->bit9

// Port 20 Direction Control Register
#define DP20                   (*((uword volatile *) 0xFFB6))
#define DP20_P0                       ((T_Reg16 *) 0xFFB6)->bit0
#define DP20_P1                       ((T_Reg16 *) 0xFFB6)->bit1
#define DP20_P12                      ((T_Reg16 *) 0xFFB6)->bit12
#define DP20_P2                       ((T_Reg16 *) 0xFFB6)->bit2
#define DP20_P4                       ((T_Reg16 *) 0xFFB6)->bit4
#define DP20_P5                       ((T_Reg16 *) 0xFFB6)->bit5

// Port 3 Direction Control Register
#define DP3                    (*((uword volatile *) 0xFFC6))
#define DP3_P0                       ((T_Reg16 *) 0xFFC6)->bit0
#define DP3_P1                       ((T_Reg16 *) 0xFFC6)->bit1
#define DP3_P10                      ((T_Reg16 *) 0xFFC6)->bit10
#define DP3_P11                      ((T_Reg16 *) 0xFFC6)->bit11
#define DP3_P12                      ((T_Reg16 *) 0xFFC6)->bit12
#define DP3_P13                      ((T_Reg16 *) 0xFFC6)->bit13
#define DP3_P15                      ((T_Reg16 *) 0xFFC6)->bit15
#define DP3_P2                       ((T_Reg16 *) 0xFFC6)->bit2
#define DP3_P3                       ((T_Reg16 *) 0xFFC6)->bit3
#define DP3_P4                       ((T_Reg16 *) 0xFFC6)->bit4
#define DP3_P5                       ((T_Reg16 *) 0xFFC6)->bit5
#define DP3_P6                       ((T_Reg16 *) 0xFFC6)->bit6
#define DP3_P7                       ((T_Reg16 *) 0xFFC6)->bit7
#define DP3_P8                       ((T_Reg16 *) 0xFFC6)->bit8
#define DP3_P9                       ((T_Reg16 *) 0xFFC6)->bit9

// Port 4 Direction Control Register
#define DP4                    (*((uword volatile *) 0xFFCA))
#define DP4_P0                       ((T_Reg16 *) 0xFFCA)->bit0
#define DP4_P1                       ((T_Reg16 *) 0xFFCA)->bit1
#define DP4_P2                       ((T_Reg16 *) 0xFFCA)->bit2
#define DP4_P3                       ((T_Reg16 *) 0xFFCA)->bit3
#define DP4_P4                       ((T_Reg16 *) 0xFFCA)->bit4
#define DP4_P5                       ((T_Reg16 *) 0xFFCA)->bit5
#define DP4_P6                       ((T_Reg16 *) 0xFFCA)->bit6
#define DP4_P7                       ((T_Reg16 *) 0xFFCA)->bit7

// Port 6 Direction Control Register
#define DP6                    (*((uword volatile *) 0xFFCE))
#define DP6_P0                       ((T_Reg16 *) 0xFFCE)->bit0
#define DP6_P1                       ((T_Reg16 *) 0xFFCE)->bit1
#define DP6_P2                       ((T_Reg16 *) 0xFFCE)->bit2
#define DP6_P3                       ((T_Reg16 *) 0xFFCE)->bit3
#define DP6_P4                       ((T_Reg16 *) 0xFFCE)->bit4
#define DP6_P5                       ((T_Reg16 *) 0xFFCE)->bit5
#define DP6_P6                       ((T_Reg16 *) 0xFFCE)->bit6
#define DP6_P7                       ((T_Reg16 *) 0xFFCE)->bit7

// Port 7 Direction Control Register
#define DP7                    (*((uword volatile *) 0xFFD2))
#define DP7_P4                       ((T_Reg16 *) 0xFFD2)->bit4
#define DP7_P5                       ((T_Reg16 *) 0xFFD2)->bit5
#define DP7_P6                       ((T_Reg16 *) 0xFFD2)->bit6
#define DP7_P7                       ((T_Reg16 *) 0xFFD2)->bit7

// Port 9 Direction Control Register
#define DP9                    (*((uword volatile *) 0xFF18))
#define DP9_P0                       ((T_Reg16 *) 0xFF18)->bit0
#define DP9_P1                       ((T_Reg16 *) 0xFF18)->bit1
#define DP9_P2                       ((T_Reg16 *) 0xFF18)->bit2
#define DP9_P3                       ((T_Reg16 *) 0xFF18)->bit3
#define DP9_P4                       ((T_Reg16 *) 0xFF18)->bit4
#define DP9_P5                       ((T_Reg16 *) 0xFF18)->bit5

// CPU Data Page Pointer 0 Register (10 bits)
#define DPP0                   (*((uword volatile *) 0xFE00))

// CPU Data Page Pointer 1 Register (10 bits)
#define DPP1                   (*((uword volatile *) 0xFE02))

// CPU Data Page Pointer 2 Register (10 bits)
#define DPP2                   (*((uword volatile *) 0xFE04))

// CPU Data Page Pointer 3 Register (10 bits)
#define DPP3                   (*((uword volatile *) 0xFE06))

// EBC Mode Control Register 0
#define EBCMOD0                (*((uword volatile *) 0xEE00))

// EBC Mode Control Register 1
#define EBCMOD1                (*((uword volatile *) 0xEE02))

// Interrupt Control Register
#define EOPIC                  (*((uword volatile *) 0xF180))
#define EOPIC_GPX                      ((T_Reg16 *) 0xF180)->bit8
#define EOPIC_IE                       ((T_Reg16 *) 0xF180)->bit6
#define EOPIC_IR                       ((T_Reg16 *) 0xF180)->bit7

// External Interrupt Control Register
#define EXICON                 (*((uword volatile *) 0xF1C0))

// External Interrupt Input Select Register
#define EXISEL0                (*((uword volatile *) 0xF1DA))

// External Interrupt Input Select Register
#define EXISEL1                (*((uword volatile *) 0xF1D8))

// CS0 Function Configuration Register
#define FCONCS0                (*((uword volatile *) 0xEE12))

// CS1 Function Configuration Register
#define FCONCS1                (*((uword volatile *) 0xEE1A))

// CS2 Function Configuration Register
#define FCONCS2                (*((uword volatile *) 0xEE22))

// CS3 Function Configuration Register
#define FCONCS3                (*((uword volatile *) 0xEE2A))

// CS4 Function Configuration Register
#define FCONCS4                (*((uword volatile *) 0xEE32))

// CS5 Function Configuration Register
#define FCONCS5                (*((uword volatile *) 0xEE3A))

// CS6 Function Configuration Register
#define FCONCS6                (*((uword volatile *) 0xEE42))

// CS7 Function Configuration Register
#define FCONCS7                (*((uword volatile *) 0xEE4A))

// Fast Interrupt  Address Register 0
#define FINT0ADDR              (*((uword volatile *) 0xEC02))

// Fast Interrupt Control Register 0
#define FINT0CSP               (*((uword volatile *) 0xEC00))

// Fast Interrupt  Address Register 1
#define FINT1ADDR              (*((uword volatile *) 0xEC06))

// Fast Interrupt Control Register 1
#define FINT1CSP               (*((uword volatile *) 0xEC04))

// Frequency Output Control Register
#define FOCON                  (*((uword volatile *) 0xFFAA))
#define FOCON_CLKEN                    ((T_Reg16 *) 0xFFAA)->bit7
#define FOCON_FOEN                     ((T_Reg16 *) 0xFFAA)->bit15
#define FOCON_FOSS                     ((T_Reg16 *) 0xFFAA)->bit14
#define FOCON_FOTL                     ((T_Reg16 *) 0xFFAA)->bit6

// GPT12 Capture/Reload Register
#define GPT12E_CAPREL          (*((uword volatile *) 0xFE4A))

// GPT2 CAPREL Interrupt Control Register
#define GPT12E_CRIC            (*((uword volatile *) 0xFF6A))
#define GPT12E_CRIC_GPX                      ((T_Reg16 *) 0xFF6A)->bit8
#define GPT12E_CRIC_IE                       ((T_Reg16 *) 0xFF6A)->bit6
#define GPT12E_CRIC_IR                       ((T_Reg16 *) 0xFF6A)->bit7

// GPT1 Timer 2 Register
#define GPT12E_T2              (*((uword volatile *) 0xFE40))

// GPT1 Timer 2 Control Register
#define GPT12E_T2CON           (*((uword volatile *) 0xFF40))
#define GPT12E_T2CON_T2CHDIR                  ((T_Reg16 *) 0xFF40)->bit14
#define GPT12E_T2CON_T2EDGE                   ((T_Reg16 *) 0xFF40)->bit13
#define GPT12E_T2CON_T2IRDIS                  ((T_Reg16 *) 0xFF40)->bit12
#define GPT12E_T2CON_T2R                      ((T_Reg16 *) 0xFF40)->bit6
#define GPT12E_T2CON_T2RC                     ((T_Reg16 *) 0xFF40)->bit9
#define GPT12E_T2CON_T2RDIR                   ((T_Reg16 *) 0xFF40)->bit15
#define GPT12E_T2CON_T2UD                     ((T_Reg16 *) 0xFF40)->bit7
#define GPT12E_T2CON_T2UDE                    ((T_Reg16 *) 0xFF40)->bit8

// GPT1 Timer 2 Interrupt Control Register
#define GPT12E_T2IC            (*((uword volatile *) 0xFF60))
#define GPT12E_T2IC_GPX                      ((T_Reg16 *) 0xFF60)->bit8
#define GPT12E_T2IC_IE                       ((T_Reg16 *) 0xFF60)->bit6
#define GPT12E_T2IC_IR                       ((T_Reg16 *) 0xFF60)->bit7

// GPT1 Timer 3 Register
#define GPT12E_T3              (*((uword volatile *) 0xFE42))

// GPT1 Timer 3 Control Register
#define GPT12E_T3CON           (*((uword volatile *) 0xFF42))
#define GPT12E_T3CON_T3CHDIR                  ((T_Reg16 *) 0xFF42)->bit14
#define GPT12E_T3CON_T3EDGE                   ((T_Reg16 *) 0xFF42)->bit13
#define GPT12E_T3CON_T3OE                     ((T_Reg16 *) 0xFF42)->bit9
#define GPT12E_T3CON_T3OTL                    ((T_Reg16 *) 0xFF42)->bit10
#define GPT12E_T3CON_T3R                      ((T_Reg16 *) 0xFF42)->bit6
#define GPT12E_T3CON_T3RDIR                   ((T_Reg16 *) 0xFF42)->bit15
#define GPT12E_T3CON_T3UD                     ((T_Reg16 *) 0xFF42)->bit7
#define GPT12E_T3CON_T3UDE                    ((T_Reg16 *) 0xFF42)->bit8

// GPT1 Timer 3 Interrupt Control Register
#define GPT12E_T3IC            (*((uword volatile *) 0xFF62))
#define GPT12E_T3IC_GPX                      ((T_Reg16 *) 0xFF62)->bit8
#define GPT12E_T3IC_IE                       ((T_Reg16 *) 0xFF62)->bit6
#define GPT12E_T3IC_IR                       ((T_Reg16 *) 0xFF62)->bit7

// GPT1 Timer 4 Register
#define GPT12E_T4              (*((uword volatile *) 0xFE44))

// GPT1 Timer 4 Control Register
#define GPT12E_T4CON           (*((uword volatile *) 0xFF44))
#define GPT12E_T4CON_T4CHDIR                  ((T_Reg16 *) 0xFF44)->bit14
#define GPT12E_T4CON_T4EDGE                   ((T_Reg16 *) 0xFF44)->bit13
#define GPT12E_T4CON_T4IRDIS                  ((T_Reg16 *) 0xFF44)->bit12
#define GPT12E_T4CON_T4R                      ((T_Reg16 *) 0xFF44)->bit6
#define GPT12E_T4CON_T4RC                     ((T_Reg16 *) 0xFF44)->bit9
#define GPT12E_T4CON_T4RDIR                   ((T_Reg16 *) 0xFF44)->bit15
#define GPT12E_T4CON_T4UD                     ((T_Reg16 *) 0xFF44)->bit7
#define GPT12E_T4CON_T4UDE                    ((T_Reg16 *) 0xFF44)->bit8

// GPT1 Timer 4 Interrupt Control Register
#define GPT12E_T4IC            (*((uword volatile *) 0xFF64))
#define GPT12E_T4IC_GPX                      ((T_Reg16 *) 0xFF64)->bit8
#define GPT12E_T4IC_IE                       ((T_Reg16 *) 0xFF64)->bit6
#define GPT12E_T4IC_IR                       ((T_Reg16 *) 0xFF64)->bit7

// GPT2 Timer 5 Register
#define GPT12E_T5              (*((uword volatile *) 0xFE46))

// GPT2 Timer 5 Control Register
#define GPT12E_T5CON           (*((uword volatile *) 0xFF46))
#define GPT12E_T5CON_CT3                      ((T_Reg16 *) 0xFF46)->bit10
#define GPT12E_T5CON_T5CC                     ((T_Reg16 *) 0xFF46)->bit11
#define GPT12E_T5CON_T5CLR                    ((T_Reg16 *) 0xFF46)->bit14
#define GPT12E_T5CON_T5R                      ((T_Reg16 *) 0xFF46)->bit6
#define GPT12E_T5CON_T5RC                     ((T_Reg16 *) 0xFF46)->bit9
#define GPT12E_T5CON_T5SC                     ((T_Reg16 *) 0xFF46)->bit15
#define GPT12E_T5CON_T5UD                     ((T_Reg16 *) 0xFF46)->bit7
#define GPT12E_T5CON_T5UDE                    ((T_Reg16 *) 0xFF46)->bit8

// GPT2 Timer 5 Interrupt Control Register
#define GPT12E_T5IC            (*((uword volatile *) 0xFF66))
#define GPT12E_T5IC_GPX                      ((T_Reg16 *) 0xFF66)->bit8
#define GPT12E_T5IC_IE                       ((T_Reg16 *) 0xFF66)->bit6
#define GPT12E_T5IC_IR                       ((T_Reg16 *) 0xFF66)->bit7

// GPT2 Timer 6 Register
#define GPT12E_T6              (*((uword volatile *) 0xFE48))

// GPT2 Timer 6 Control Register
#define GPT12E_T6CON           (*((uword volatile *) 0xFF48))
#define GPT12E_T6CON_T6CLR                    ((T_Reg16 *) 0xFF48)->bit14
#define GPT12E_T6CON_T6OE                     ((T_Reg16 *) 0xFF48)->bit9
#define GPT12E_T6CON_T6OTL                    ((T_Reg16 *) 0xFF48)->bit10
#define GPT12E_T6CON_T6R                      ((T_Reg16 *) 0xFF48)->bit6
#define GPT12E_T6CON_T6SR                     ((T_Reg16 *) 0xFF48)->bit15
#define GPT12E_T6CON_T6UD                     ((T_Reg16 *) 0xFF48)->bit7
#define GPT12E_T6CON_T6UDE                    ((T_Reg16 *) 0xFF48)->bit8

// GPT2 Timer 6 Interrupt Control Register
#define GPT12E_T6IC            (*((uword volatile *) 0xFF68))
#define GPT12E_T6IC_GPX                      ((T_Reg16 *) 0xFF68)->bit8
#define GPT12E_T6IC_IE                       ((T_Reg16 *) 0xFF68)->bit6
#define GPT12E_T6IC_IR                       ((T_Reg16 *) 0xFF68)->bit7

// Identifier
#define IDCHIP                 (*((uword volatile *) 0xF07C))

// Identifier
#define IDMANUF                (*((uword volatile *) 0xF07E))

// Identifier
#define IDMEM                  (*((uword volatile *) 0xF07A))

// Identifier
#define IDMEM2                 (*((uword volatile *) 0xF076))

// Identifier
#define IDPROG                 (*((uword volatile *) 0xF078))

// IIC Address Register
#define IIC_ADR                (*((uword volatile *) 0xE606))

// IIC Configuration Register
#define IIC_CFG                (*((uword volatile *) 0xE600))

// IIC Control Register
#define IIC_CON                (*((uword volatile *) 0xE602))

// IIC Data Interrupt Control Register
#define IIC_DIC                (*((uword volatile *) 0xF186))
#define IIC_DIC_GPX                      ((T_Reg16 *) 0xF186)->bit8
#define IIC_DIC_IE                       ((T_Reg16 *) 0xF186)->bit6
#define IIC_DIC_IR                       ((T_Reg16 *) 0xF186)->bit7

// IIC Protocol Event Interrupt Control Register
#define IIC_PEIC               (*((uword volatile *) 0xF18E))
#define IIC_PEIC_GPX                      ((T_Reg16 *) 0xF18E)->bit8
#define IIC_PEIC_IE                       ((T_Reg16 *) 0xF18E)->bit6
#define IIC_PEIC_IR                       ((T_Reg16 *) 0xF18E)->bit7

// IIC Receive/Transmit Buffer High Register
#define IIC_RTBH               (*((uword volatile *) 0xE60A))

// IIC Receive/Transmit Buffer Low Register
#define IIC_RTBL               (*((uword volatile *) 0xE608))

// IIC Status Register
#define IIC_ST                 (*((uword volatile *) 0xE604))

// CPU Multiply Divide Control Register
#define MDC                    (*((uword volatile *) 0xFF0E))
#define MDC_MDRIU                    ((T_Reg16 *) 0xFF0E)->bit4

// CPU Multiply Divide Register - High Word
#define MDH                    (*((uword volatile *) 0xFE0C))

// CPU Multiply Divide Register - Low Word
#define MDL                    (*((uword volatile *) 0xFE0E))

// Port 2 Open Drain Control Register
#define ODP2                   (*((uword volatile *) 0xF1C2))
#define ODP2_P10                      ((T_Reg16 *) 0xF1C2)->bit10
#define ODP2_P11                      ((T_Reg16 *) 0xF1C2)->bit11
#define ODP2_P12                      ((T_Reg16 *) 0xF1C2)->bit12
#define ODP2_P13                      ((T_Reg16 *) 0xF1C2)->bit13
#define ODP2_P14                      ((T_Reg16 *) 0xF1C2)->bit14
#define ODP2_P15                      ((T_Reg16 *) 0xF1C2)->bit15
#define ODP2_P8                       ((T_Reg16 *) 0xF1C2)->bit8
#define ODP2_P9                       ((T_Reg16 *) 0xF1C2)->bit9

// Port 3 Open Drain Control Register
#define ODP3                   (*((uword volatile *) 0xF1C6))
#define ODP3_P0                       ((T_Reg16 *) 0xF1C6)->bit0
#define ODP3_P1                       ((T_Reg16 *) 0xF1C6)->bit1
#define ODP3_P10                      ((T_Reg16 *) 0xF1C6)->bit10
#define ODP3_P11                      ((T_Reg16 *) 0xF1C6)->bit11
#define ODP3_P13                      ((T_Reg16 *) 0xF1C6)->bit13
#define ODP3_P2                       ((T_Reg16 *) 0xF1C6)->bit2
#define ODP3_P3                       ((T_Reg16 *) 0xF1C6)->bit3
#define ODP3_P4                       ((T_Reg16 *) 0xF1C6)->bit4
#define ODP3_P5                       ((T_Reg16 *) 0xF1C6)->bit5
#define ODP3_P6                       ((T_Reg16 *) 0xF1C6)->bit6
#define ODP3_P7                       ((T_Reg16 *) 0xF1C6)->bit7
#define ODP3_P8                       ((T_Reg16 *) 0xF1C6)->bit8
#define ODP3_P9                       ((T_Reg16 *) 0xF1C6)->bit9

// Port 4 Open Drain Control Register
#define ODP4                   (*((uword volatile *) 0xF1CA))
#define ODP4_P0                       ((T_Reg16 *) 0xF1CA)->bit0
#define ODP4_P1                       ((T_Reg16 *) 0xF1CA)->bit1
#define ODP4_P2                       ((T_Reg16 *) 0xF1CA)->bit2
#define ODP4_P3                       ((T_Reg16 *) 0xF1CA)->bit3
#define ODP4_P4                       ((T_Reg16 *) 0xF1CA)->bit4
#define ODP4_P5                       ((T_Reg16 *) 0xF1CA)->bit5
#define ODP4_P6                       ((T_Reg16 *) 0xF1CA)->bit6
#define ODP4_P7                       ((T_Reg16 *) 0xF1CA)->bit7

// Port 6 Open Drain Control Register
#define ODP6                   (*((uword volatile *) 0xF1CE))
#define ODP6_P0                       ((T_Reg16 *) 0xF1CE)->bit0
#define ODP6_P1                       ((T_Reg16 *) 0xF1CE)->bit1
#define ODP6_P2                       ((T_Reg16 *) 0xF1CE)->bit2
#define ODP6_P3                       ((T_Reg16 *) 0xF1CE)->bit3
#define ODP6_P4                       ((T_Reg16 *) 0xF1CE)->bit4
#define ODP6_P5                       ((T_Reg16 *) 0xF1CE)->bit5
#define ODP6_P6                       ((T_Reg16 *) 0xF1CE)->bit6
#define ODP6_P7                       ((T_Reg16 *) 0xF1CE)->bit7

// Port 7 Open Drain Control Register
#define ODP7                   (*((uword volatile *) 0xF1D2))
#define ODP7_P4                       ((T_Reg16 *) 0xF1D2)->bit4
#define ODP7_P5                       ((T_Reg16 *) 0xF1D2)->bit5
#define ODP7_P6                       ((T_Reg16 *) 0xF1D2)->bit6
#define ODP7_P7                       ((T_Reg16 *) 0xF1D2)->bit7

// Port 9 Open Drain Control Register
#define ODP9                   (*((uword volatile *) 0xFF1A))
#define ODP9_P0                       ((T_Reg16 *) 0xFF1A)->bit0
#define ODP9_P1                       ((T_Reg16 *) 0xFF1A)->bit1
#define ODP9_P2                       ((T_Reg16 *) 0xFF1A)->bit2
#define ODP9_P3                       ((T_Reg16 *) 0xFF1A)->bit3
#define ODP9_P4                       ((T_Reg16 *) 0xFF1A)->bit4
#define ODP9_P5                       ((T_Reg16 *) 0xFF1A)->bit5

// Constant Value 1's Register
#define ONES                   (*((uword volatile *) 0xFF1E))

// Port 0 High Register (Upper half)
#define P0H                    (*((uword volatile *) 0xFF02))
#define P0H_P0                       ((T_Reg16 *) 0xFF02)->bit0
#define P0H_P1                       ((T_Reg16 *) 0xFF02)->bit1
#define P0H_P2                       ((T_Reg16 *) 0xFF02)->bit2
#define P0H_P3                       ((T_Reg16 *) 0xFF02)->bit3
#define P0H_P4                       ((T_Reg16 *) 0xFF02)->bit4
#define P0H_P5                       ((T_Reg16 *) 0xFF02)->bit5
#define P0H_P6                       ((T_Reg16 *) 0xFF02)->bit6
#define P0H_P7                       ((T_Reg16 *) 0xFF02)->bit7

// Port 0 Low Register (Lower half)
#define P0L                    (*((uword volatile *) 0xFF00))
#define P0L_P0                       ((T_Reg16 *) 0xFF00)->bit0
#define P0L_P1                       ((T_Reg16 *) 0xFF00)->bit1
#define P0L_P2                       ((T_Reg16 *) 0xFF00)->bit2
#define P0L_P3                       ((T_Reg16 *) 0xFF00)->bit3
#define P0L_P4                       ((T_Reg16 *) 0xFF00)->bit4
#define P0L_P5                       ((T_Reg16 *) 0xFF00)->bit5
#define P0L_P6                       ((T_Reg16 *) 0xFF00)->bit6
#define P0L_P7                       ((T_Reg16 *) 0xFF00)->bit7

// Port 1 High Register (Upper half)
#define P1H                    (*((uword volatile *) 0xFF06))
#define P1H_P0                       ((T_Reg16 *) 0xFF06)->bit0
#define P1H_P1                       ((T_Reg16 *) 0xFF06)->bit1
#define P1H_P2                       ((T_Reg16 *) 0xFF06)->bit2
#define P1H_P3                       ((T_Reg16 *) 0xFF06)->bit3
#define P1H_P4                       ((T_Reg16 *) 0xFF06)->bit4
#define P1H_P5                       ((T_Reg16 *) 0xFF06)->bit5
#define P1H_P6                       ((T_Reg16 *) 0xFF06)->bit6
#define P1H_P7                       ((T_Reg16 *) 0xFF06)->bit7

// Port 1 Low Register (Lower half)
#define P1L                    (*((uword volatile *) 0xFF04))
#define P1L_P0                       ((T_Reg16 *) 0xFF04)->bit0
#define P1L_P1                       ((T_Reg16 *) 0xFF04)->bit1
#define P1L_P2                       ((T_Reg16 *) 0xFF04)->bit2
#define P1L_P3                       ((T_Reg16 *) 0xFF04)->bit3
#define P1L_P4                       ((T_Reg16 *) 0xFF04)->bit4
#define P1L_P5                       ((T_Reg16 *) 0xFF04)->bit5
#define P1L_P6                       ((T_Reg16 *) 0xFF04)->bit6
#define P1L_P7                       ((T_Reg16 *) 0xFF04)->bit7

// Port 2 Register (16 bits)
#define P2                     (*((uword volatile *) 0xFFC0))
#define P2_P10                      ((T_Reg16 *) 0xFFC0)->bit10
#define P2_P11                      ((T_Reg16 *) 0xFFC0)->bit11
#define P2_P12                      ((T_Reg16 *) 0xFFC0)->bit12
#define P2_P13                      ((T_Reg16 *) 0xFFC0)->bit13
#define P2_P14                      ((T_Reg16 *) 0xFFC0)->bit14
#define P2_P15                      ((T_Reg16 *) 0xFFC0)->bit15
#define P2_P8                       ((T_Reg16 *) 0xFFC0)->bit8
#define P2_P9                       ((T_Reg16 *) 0xFFC0)->bit9

// Port 20 Data Register
#define P20                    (*((uword volatile *) 0xFFB4))
#define P20_P0                       ((T_Reg16 *) 0xFFB4)->bit0
#define P20_P1                       ((T_Reg16 *) 0xFFB4)->bit1
#define P20_P12                      ((T_Reg16 *) 0xFFB4)->bit12
#define P20_P2                       ((T_Reg16 *) 0xFFB4)->bit2
#define P20_P4                       ((T_Reg16 *) 0xFFB4)->bit4
#define P20_P5                       ((T_Reg16 *) 0xFFB4)->bit5

// Port 3 Register
#define P3                     (*((uword volatile *) 0xFFC4))
#define P3_P0                       ((T_Reg16 *) 0xFFC4)->bit0
#define P3_P1                       ((T_Reg16 *) 0xFFC4)->bit1
#define P3_P10                      ((T_Reg16 *) 0xFFC4)->bit10
#define P3_P11                      ((T_Reg16 *) 0xFFC4)->bit11
#define P3_P12                      ((T_Reg16 *) 0xFFC4)->bit12
#define P3_P13                      ((T_Reg16 *) 0xFFC4)->bit13
#define P3_P15                      ((T_Reg16 *) 0xFFC4)->bit15
#define P3_P2                       ((T_Reg16 *) 0xFFC4)->bit2
#define P3_P3                       ((T_Reg16 *) 0xFFC4)->bit3
#define P3_P4                       ((T_Reg16 *) 0xFFC4)->bit4
#define P3_P5                       ((T_Reg16 *) 0xFFC4)->bit5
#define P3_P6                       ((T_Reg16 *) 0xFFC4)->bit6
#define P3_P7                       ((T_Reg16 *) 0xFFC4)->bit7
#define P3_P8                       ((T_Reg16 *) 0xFFC4)->bit8
#define P3_P9                       ((T_Reg16 *) 0xFFC4)->bit9

// Port 4 Register (8 bits)
#define P4                     (*((uword volatile *) 0xFFC8))
#define P4_P0                       ((T_Reg16 *) 0xFFC8)->bit0
#define P4_P1                       ((T_Reg16 *) 0xFFC8)->bit1
#define P4_P2                       ((T_Reg16 *) 0xFFC8)->bit2
#define P4_P3                       ((T_Reg16 *) 0xFFC8)->bit3
#define P4_P4                       ((T_Reg16 *) 0xFFC8)->bit4
#define P4_P5                       ((T_Reg16 *) 0xFFC8)->bit5
#define P4_P6                       ((T_Reg16 *) 0xFFC8)->bit6
#define P4_P7                       ((T_Reg16 *) 0xFFC8)->bit7

// Port 5 Register (16 bits)
#define P5                     (*((uword volatile *) 0xFFA2))
#define P5_P0                       ((T_Reg16 *) 0xFFA2)->bit0
#define P5_P1                       ((T_Reg16 *) 0xFFA2)->bit1
#define P5_P10                      ((T_Reg16 *) 0xFFA2)->bit10
#define P5_P11                      ((T_Reg16 *) 0xFFA2)->bit11
#define P5_P12                      ((T_Reg16 *) 0xFFA2)->bit12
#define P5_P13                      ((T_Reg16 *) 0xFFA2)->bit13
#define P5_P14                      ((T_Reg16 *) 0xFFA2)->bit14
#define P5_P15                      ((T_Reg16 *) 0xFFA2)->bit15
#define P5_P2                       ((T_Reg16 *) 0xFFA2)->bit2
#define P5_P3                       ((T_Reg16 *) 0xFFA2)->bit3
#define P5_P4                       ((T_Reg16 *) 0xFFA2)->bit4
#define P5_P5                       ((T_Reg16 *) 0xFFA2)->bit5
#define P5_P6                       ((T_Reg16 *) 0xFFA2)->bit6
#define P5_P7                       ((T_Reg16 *) 0xFFA2)->bit7
#define P5_P8                       ((T_Reg16 *) 0xFFA2)->bit8
#define P5_P9                       ((T_Reg16 *) 0xFFA2)->bit9

// Port 5 Digital Input Disable Register
#define P5DIDIS                (*((uword volatile *) 0xFFA4))
#define P5DIDIS_P0                       ((T_Reg16 *) 0xFFA4)->bit0
#define P5DIDIS_P1                       ((T_Reg16 *) 0xFFA4)->bit1
#define P5DIDIS_P10                      ((T_Reg16 *) 0xFFA4)->bit10
#define P5DIDIS_P11                      ((T_Reg16 *) 0xFFA4)->bit11
#define P5DIDIS_P12                      ((T_Reg16 *) 0xFFA4)->bit12
#define P5DIDIS_P13                      ((T_Reg16 *) 0xFFA4)->bit13
#define P5DIDIS_P14                      ((T_Reg16 *) 0xFFA4)->bit14
#define P5DIDIS_P15                      ((T_Reg16 *) 0xFFA4)->bit15
#define P5DIDIS_P2                       ((T_Reg16 *) 0xFFA4)->bit2
#define P5DIDIS_P3                       ((T_Reg16 *) 0xFFA4)->bit3
#define P5DIDIS_P4                       ((T_Reg16 *) 0xFFA4)->bit4
#define P5DIDIS_P5                       ((T_Reg16 *) 0xFFA4)->bit5
#define P5DIDIS_P6                       ((T_Reg16 *) 0xFFA4)->bit6
#define P5DIDIS_P7                       ((T_Reg16 *) 0xFFA4)->bit7
#define P5DIDIS_P8                       ((T_Reg16 *) 0xFFA4)->bit8
#define P5DIDIS_P9                       ((T_Reg16 *) 0xFFA4)->bit9

// Port 6 Register (8 bits)
#define P6                     (*((uword volatile *) 0xFFCC))
#define P6_P0                       ((T_Reg16 *) 0xFFCC)->bit0
#define P6_P1                       ((T_Reg16 *) 0xFFCC)->bit1
#define P6_P2                       ((T_Reg16 *) 0xFFCC)->bit2
#define P6_P3                       ((T_Reg16 *) 0xFFCC)->bit3
#define P6_P4                       ((T_Reg16 *) 0xFFCC)->bit4
#define P6_P5                       ((T_Reg16 *) 0xFFCC)->bit5
#define P6_P6                       ((T_Reg16 *) 0xFFCC)->bit6
#define P6_P7                       ((T_Reg16 *) 0xFFCC)->bit7

// Port 7 Register (8 bits)
#define P7                     (*((uword volatile *) 0xFFD0))
#define P7_P4                       ((T_Reg16 *) 0xFFD0)->bit4
#define P7_P5                       ((T_Reg16 *) 0xFFD0)->bit5
#define P7_P6                       ((T_Reg16 *) 0xFFD0)->bit6
#define P7_P7                       ((T_Reg16 *) 0xFFD0)->bit7

// Port 9 Register (6 bits)
#define P9                     (*((uword volatile *) 0xFF16))
#define P9_P0                       ((T_Reg16 *) 0xFF16)->bit0
#define P9_P1                       ((T_Reg16 *) 0xFF16)->bit1
#define P9_P2                       ((T_Reg16 *) 0xFF16)->bit2
#define P9_P3                       ((T_Reg16 *) 0xFF16)->bit3
#define P9_P4                       ((T_Reg16 *) 0xFF16)->bit4
#define P9_P5                       ((T_Reg16 *) 0xFF16)->bit5

// PEC Channel 0 Control Register
#define PECC0                  (*((uword volatile *) 0xFEC0))

// PEC Channel 1 Control Register
#define PECC1                  (*((uword volatile *) 0xFEC2))

// PEC Channel 2 Control Register
#define PECC2                  (*((uword volatile *) 0xFEC4))

// PEC Channel 3 Control Register
#define PECC3                  (*((uword volatile *) 0xFEC6))

// PEC Channel 4 Control Register
#define PECC4                  (*((uword volatile *) 0xFEC8))

// PEC Channel 5 Control Register
#define PECC5                  (*((uword volatile *) 0xFECA))

// PEC Channel 6 Control Register
#define PECC6                  (*((uword volatile *) 0xFECC))

// PEC Channel 7 Control Register
#define PECC7                  (*((uword volatile *) 0xFECE))

// PEC Interrupt Subnode Control Register
#define PECISNC                (*((uword volatile *) 0xFFA8))
#define PECISNC_C0IE                     ((T_Reg16 *) 0xFFA8)->bit0
#define PECISNC_C0IR                     ((T_Reg16 *) 0xFFA8)->bit1
#define PECISNC_C1IE                     ((T_Reg16 *) 0xFFA8)->bit2
#define PECISNC_C1IR                     ((T_Reg16 *) 0xFFA8)->bit3
#define PECISNC_C2IE                     ((T_Reg16 *) 0xFFA8)->bit4
#define PECISNC_C2IR                     ((T_Reg16 *) 0xFFA8)->bit5
#define PECISNC_C3IE                     ((T_Reg16 *) 0xFFA8)->bit6
#define PECISNC_C3IR                     ((T_Reg16 *) 0xFFA8)->bit7
#define PECISNC_C4IE                     ((T_Reg16 *) 0xFFA8)->bit8
#define PECISNC_C4IR                     ((T_Reg16 *) 0xFFA8)->bit9
#define PECISNC_C5IE                     ((T_Reg16 *) 0xFFA8)->bit10
#define PECISNC_C5IR                     ((T_Reg16 *) 0xFFA8)->bit11
#define PECISNC_C6IE                     ((T_Reg16 *) 0xFFA8)->bit12
#define PECISNC_C6IR                     ((T_Reg16 *) 0xFFA8)->bit13
#define PECISNC_C7IE                     ((T_Reg16 *) 0xFFA8)->bit14
#define PECISNC_C7IR                     ((T_Reg16 *) 0xFFA8)->bit15

// PEC Pointer 0 Segment Address Register
#define PECSEG0                (*((uword volatile *) 0xEC80))

// PEC Pointer 1 Segment Address Register
#define PECSEG1                (*((uword volatile *) 0xEC82))

// PEC Pointer 2 Segment Address Register
#define PECSEG2                (*((uword volatile *) 0xEC84))

// PEC Pointer 3 Segment Address Register
#define PECSEG3                (*((uword volatile *) 0xEC86))

// PEC Pointer 4 Segment Address Register
#define PECSEG4                (*((uword volatile *) 0xEC88))

// PEC Pointer 5 Segment Address Register
#define PECSEG5                (*((uword volatile *) 0xEC8A))

// PEC Pointer 6 Segment Address Register
#define PECSEG6                (*((uword volatile *) 0xEC8C))

// PEC Pointer 7 Segment Address Register
#define PECSEG7                (*((uword volatile *) 0xEC8E))

// Port Input Control Register
#define PICON                  (*((uword volatile *) 0xF1C4))
#define PICON_P20HIN                   ((T_Reg16 *) 0xF1C4)->bit9
#define PICON_P20LIN                   ((T_Reg16 *) 0xF1C4)->bit8
#define PICON_P2HIN                    ((T_Reg16 *) 0xF1C4)->bit1
#define PICON_P3HIN                    ((T_Reg16 *) 0xF1C4)->bit3
#define PICON_P3LIN                    ((T_Reg16 *) 0xF1C4)->bit2
#define PICON_P4LIN                    ((T_Reg16 *) 0xF1C4)->bit4
#define PICON_P6LIN                    ((T_Reg16 *) 0xF1C4)->bit5
#define PICON_P7LIN                    ((T_Reg16 *) 0xF1C4)->bit6
#define PICON_P9LIN                    ((T_Reg16 *) 0xF1C4)->bit7

// PLL Control Register
#define PLLCON                 (*((uword volatile *) 0xF1D0))
#define PLLCON_PLLCSF                   ((T_Reg16 *) 0xF1D0)->bit15

// Port P0H Output Control Register
#define POCON0H                (*((uword volatile *) 0xF082))

// Port 0L Output Control Register
#define POCON0L                (*((uword volatile *) 0xF080))

// Port 1H Output Control Register
#define POCON1H                (*((uword volatile *) 0xF086))

// Port 1L Output Control Register
#define POCON1L                (*((uword volatile *) 0xF084))

// Port 2 Output Control Register
#define POCON2                 (*((uword volatile *) 0xF088))

// Port 20 Output Control Register
#define POCON20                (*((uword volatile *) 0xF0AA))

// Port 3 Output Control Register
#define POCON3                 (*((uword volatile *) 0xF08A))

// Port 4 Output Control Register
#define POCON4                 (*((uword volatile *) 0xF08C))

// Port 6 Output Control Register
#define POCON6                 (*((uword volatile *) 0xF08E))

// Port 7 Output Control Register
#define POCON7                 (*((uword volatile *) 0xF090))

// Port 9 Output Control Register
#define POCON9                 (*((uword volatile *) 0xF094))

// CPU Program Status Word
#define PSW                    (*((uword volatile *) 0xFF10))
#define PSW_C                        ((T_Reg16 *) 0xFF10)->bit1
#define PSW_E                        ((T_Reg16 *) 0xFF10)->bit4
#define PSW_HLDEN                    ((T_Reg16 *) 0xFF10)->bit10
#define PSW_IEN                      ((T_Reg16 *) 0xFF10)->bit11
#define PSW_MULIP                    ((T_Reg16 *) 0xFF10)->bit5
#define PSW_N                        ((T_Reg16 *) 0xFF10)->bit0
#define PSW_USR0                     ((T_Reg16 *) 0xFF10)->bit6
#define PSW_V                        ((T_Reg16 *) 0xFF10)->bit2
#define PSW_Z                        ((T_Reg16 *) 0xFF10)->bit3

// Reset Configuration Register
#define RSTCFG                 (*((uword volatile *) 0xF108))
#define RSTCFG_ADP                      ((T_Reg16 *) 0xF108)->bit1
#define RSTCFG_ROC                      ((T_Reg16 *) 0xF108)->bit0
#define RSTCFG_WRC                      ((T_Reg16 *) 0xF108)->bit8

// Reset Control Register
#define RSTCON                 (*((uword volatile *) 0xF1E0))
#define RSTCON_0                        ((T_Reg16 *) 0xF1E0)->bit3
#define RSTCON_ROCOFF                   ((T_Reg16 *) 0xF1E0)->bit5
#define RSTCON_ROCON                    ((T_Reg16 *) 0xF1E0)->bit6
#define RSTCON_RODIS                    ((T_Reg16 *) 0xF1E0)->bit7
#define RSTCON_RORMV                    ((T_Reg16 *) 0xF1E0)->bit4

// RTC Control Register
#define RTC_CON                (*((uword volatile *) 0xF110))
#define RTC_CON_ACCPOS                   ((T_Reg16 *) 0xF110)->bit7
#define RTC_CON_PRE                      ((T_Reg16 *) 0xF110)->bit1
#define RTC_CON_REFCLK                   ((T_Reg16 *) 0xF110)->bit4
#define RTC_CON_RUN                      ((T_Reg16 *) 0xF110)->bit0
#define RTC_CON_T14DEC                   ((T_Reg16 *) 0xF110)->bit2
#define RTC_CON_T14INC                   ((T_Reg16 *) 0xF110)->bit3

// RTC Interrupt Control Register
#define RTC_IC                 (*((uword volatile *) 0xF1A0))
#define RTC_IC_GPX                      ((T_Reg16 *) 0xF1A0)->bit8
#define RTC_IC_IE                       ((T_Reg16 *) 0xF1A0)->bit6
#define RTC_IC_IR                       ((T_Reg16 *) 0xF1A0)->bit7

// RTC Interrupt Sub Node Control Register
#define RTC_ISNC               (*((uword volatile *) 0xF10C))
#define RTC_ISNC_0IE                      ((T_Reg16 *) 0xF10C)->bit2
#define RTC_ISNC_0IR                      ((T_Reg16 *) 0xF10C)->bit3
#define RTC_ISNC_1IE                      ((T_Reg16 *) 0xF10C)->bit4
#define RTC_ISNC_1IR                      ((T_Reg16 *) 0xF10C)->bit5
#define RTC_ISNC_2IE                      ((T_Reg16 *) 0xF10C)->bit6
#define RTC_ISNC_2IR                      ((T_Reg16 *) 0xF10C)->bit7
#define RTC_ISNC_3IE                      ((T_Reg16 *) 0xF10C)->bit8
#define RTC_ISNC_3IR                      ((T_Reg16 *) 0xF10C)->bit9
#define RTC_ISNC_T14IE                    ((T_Reg16 *) 0xF10C)->bit0
#define RTC_ISNC_T14IR                    ((T_Reg16 *) 0xF10C)->bit1

// RTC Timer Reload High Register
#define RTC_RELH               (*((uword volatile *) 0xF0CE))

// RTC Timer Reload Low Register
#define RTC_RELL               (*((uword volatile *) 0xF0CC))

// RTC Timer High Register
#define RTC_RTCH               (*((uword volatile *) 0xF0D6))

// RTC Timer Low Register
#define RTC_RTCL               (*((uword volatile *) 0xF0D4))

// Timer 14 Register
#define RTC_T14                (*((uword volatile *) 0xF0D2))

// Timer 14 Reload Register
#define RTC_T14REL             (*((uword volatile *) 0xF0D0))

// Security Level Command Register
#define SCUSLC                 (*((uword volatile *) 0xF0C0))

// Security Level Status Register
#define SCUSLS                 (*((uword volatile *) 0xF0C2))

// CPU System Stack Pointer Register
#define SP                     (*((uword volatile *) 0xFE12))

// SSC0 Baudrate Register
#define SSC0_BR                (*((uword volatile *) 0xF0B4))

// SSC0 Control Register
#define SSC0_CON               (*((uword volatile *) 0xFFB2))
#define SSC0_CON_AREN_BSY                 ((T_Reg16 *) 0xFFB2)->bit12
#define SSC0_CON_BEN_BE                   ((T_Reg16 *) 0xFFB2)->bit11
#define SSC0_CON_EN                       ((T_Reg16 *) 0xFFB2)->bit15
#define SSC0_CON_HB                       ((T_Reg16 *) 0xFFB2)->bit4
#define SSC0_CON_LB                       ((T_Reg16 *) 0xFFB2)->bit7
#define SSC0_CON_MS                       ((T_Reg16 *) 0xFFB2)->bit14
#define SSC0_CON_PEN_PE                   ((T_Reg16 *) 0xFFB2)->bit10
#define SSC0_CON_PH                       ((T_Reg16 *) 0xFFB2)->bit5
#define SSC0_CON_PO                       ((T_Reg16 *) 0xFFB2)->bit6
#define SSC0_CON_REN_RE                   ((T_Reg16 *) 0xFFB2)->bit9
#define SSC0_CON_TEN_TE                   ((T_Reg16 *) 0xFFB2)->bit8

// SSC0 Error Interrupt Control Register
#define SSC0_EIC               (*((uword volatile *) 0xFF76))
#define SSC0_EIC_GPX                      ((T_Reg16 *) 0xFF76)->bit8
#define SSC0_EIC_IE                       ((T_Reg16 *) 0xFF76)->bit6
#define SSC0_EIC_IR                       ((T_Reg16 *) 0xFF76)->bit7

// SSC0 Receive Buffer
#define SSC0_RB                (*((uword volatile *) 0xF0B2))

// SSC0 Receive Interrupt Control Register
#define SSC0_RIC               (*((uword volatile *) 0xFF74))
#define SSC0_RIC_GPX                      ((T_Reg16 *) 0xFF74)->bit8
#define SSC0_RIC_IE                       ((T_Reg16 *) 0xFF74)->bit6
#define SSC0_RIC_IR                       ((T_Reg16 *) 0xFF74)->bit7

// SSC0 Transmit Buffer
#define SSC0_TB                (*((uword volatile *) 0xF0B0))

// SSC0 Transmit Interrupt Control Register
#define SSC0_TIC               (*((uword volatile *) 0xFF72))
#define SSC0_TIC_GPX                      ((T_Reg16 *) 0xFF72)->bit8
#define SSC0_TIC_IE                       ((T_Reg16 *) 0xFF72)->bit6
#define SSC0_TIC_IR                       ((T_Reg16 *) 0xFF72)->bit7

// SSC1 Baudrate Register
#define SSC1_BR                (*((uword volatile *) 0xF05E))

// SSC1 Control Register
#define SSC1_CON               (*((uword volatile *) 0xFF5E))
#define SSC1_CON_AREN_BSY                 ((T_Reg16 *) 0xFF5E)->bit12
#define SSC1_CON_BEN_BE                   ((T_Reg16 *) 0xFF5E)->bit11
#define SSC1_CON_EN                       ((T_Reg16 *) 0xFF5E)->bit15
#define SSC1_CON_HB                       ((T_Reg16 *) 0xFF5E)->bit4
#define SSC1_CON_LB                       ((T_Reg16 *) 0xFF5E)->bit7
#define SSC1_CON_MS                       ((T_Reg16 *) 0xFF5E)->bit14
#define SSC1_CON_PEN_PE                   ((T_Reg16 *) 0xFF5E)->bit10
#define SSC1_CON_PH                       ((T_Reg16 *) 0xFF5E)->bit5
#define SSC1_CON_PO                       ((T_Reg16 *) 0xFF5E)->bit6
#define SSC1_CON_REN_RE                   ((T_Reg16 *) 0xFF5E)->bit9
#define SSC1_CON_TEN_TE                   ((T_Reg16 *) 0xFF5E)->bit8

// SSC1 Error Interrupt Control Register
#define SSC1_EIC               (*((uword volatile *) 0xF1AE))
#define SSC1_EIC_GPX                      ((T_Reg16 *) 0xF1AE)->bit8
#define SSC1_EIC_IE                       ((T_Reg16 *) 0xF1AE)->bit6
#define SSC1_EIC_IR                       ((T_Reg16 *) 0xF1AE)->bit7

// SSC1 Receive Buffer
#define SSC1_RB                (*((uword volatile *) 0xF05C))

// SSC1 Receive Interrupt Control Register
#define SSC1_RIC               (*((uword volatile *) 0xF1AC))
#define SSC1_RIC_GPX                      ((T_Reg16 *) 0xF1AC)->bit8
#define SSC1_RIC_IE                       ((T_Reg16 *) 0xF1AC)->bit6
#define SSC1_RIC_IR                       ((T_Reg16 *) 0xF1AC)->bit7

// SSC1 Transmit Buffer
#define SSC1_TB                (*((uword volatile *) 0xF05A))

// SSC1 Transmit Interrupt Control Register
#define SSC1_TIC               (*((uword volatile *) 0xF1AA))
#define SSC1_TIC_GPX                      ((T_Reg16 *) 0xF1AA)->bit8
#define SSC1_TIC_IE                       ((T_Reg16 *) 0xF1AA)->bit6
#define SSC1_TIC_IR                       ((T_Reg16 *) 0xF1AA)->bit7

// CPU Stack Overflow Pointer Register
#define STKOV                  (*((uword volatile *) 0xFE14))

// CPU Stack Underflow Pointer Register
#define STKUN                  (*((uword volatile *) 0xFE16))

// General System Control Register
#define SYSCON0                (*((uword volatile *) 0xF1BE))
#define SYSCON0_RTCCM                    ((T_Reg16 *) 0xF1BE)->bit14
#define SYSCON0_RTCRST                   ((T_Reg16 *) 0xF1BE)->bit15

// System Control Register 1
#define SYSCON1                (*((uword volatile *) 0xF1DC))
#define SYSCON1_CPSYS                    ((T_Reg16 *) 0xF1DC)->bit8

// CPU System Configuration Register 3
#define SYSCON3                (*((uword volatile *) 0xF1D4))
#define SYSCON3_ADCDIS                   ((T_Reg16 *) 0xF1D4)->bit0
#define SYSCON3_ASC0DIS                  ((T_Reg16 *) 0xF1D4)->bit1
#define SYSCON3_ASC1DIS                  ((T_Reg16 *) 0xF1D4)->bit10
#define SYSCON3_CANDIS                   ((T_Reg16 *) 0xF1D4)->bit13
#define SYSCON3_CC1DIS                   ((T_Reg16 *) 0xF1D4)->bit6
#define SYSCON3_CC2DIS                   ((T_Reg16 *) 0xF1D4)->bit7
#define SYSCON3_CC6DIS                   ((T_Reg16 *) 0xF1D4)->bit8
#define SYSCON3_GPTDIS                   ((T_Reg16 *) 0xF1D4)->bit3
#define SYSCON3_I2CDIS                   ((T_Reg16 *) 0xF1D4)->bit11
#define SYSCON3_PFMDIS                   ((T_Reg16 *) 0xF1D4)->bit5
#define SYSCON3_SSC0DIS                  ((T_Reg16 *) 0xF1D4)->bit2
#define SYSCON3_SSC1DIS                  ((T_Reg16 *) 0xF1D4)->bit15

// System Status Register
#define SYSSTAT                (*((uword volatile *) 0xF1E4))
#define SYSSTAT_CLKHIX                   ((T_Reg16 *) 0xF1E4)->bit13
#define SYSSTAT_CLKLOX                   ((T_Reg16 *) 0xF1E4)->bit12
#define SYSSTAT_HWR                      ((T_Reg16 *) 0xF1E4)->bit2
#define SYSSTAT_OSCLOCK                  ((T_Reg16 *) 0xF1E4)->bit15
#define SYSSTAT_PLLLOCK                  ((T_Reg16 *) 0xF1E4)->bit14
#define SYSSTAT_SWR                      ((T_Reg16 *) 0xF1E4)->bit1
#define SYSSTAT_WDTR                     ((T_Reg16 *) 0xF1E4)->bit0

// CS0 Timing Configuration Register
#define TCONCS0                (*((uword volatile *) 0xEE10))

// CS1 Timing Configuration Register
#define TCONCS1                (*((uword volatile *) 0xEE18))

// CS2 Timing Configuration Register
#define TCONCS2                (*((uword volatile *) 0xEE20))

// CS3 Timing Configuration Register
#define TCONCS3                (*((uword volatile *) 0xEE28))

// CS4 Timing Configuration Register
#define TCONCS4                (*((uword volatile *) 0xEE30))

// CS5 Timing Configuration Register
#define TCONCS5                (*((uword volatile *) 0xEE38))

// CS6 Timing Configuration Register
#define TCONCS6                (*((uword volatile *) 0xEE40))

// CS7 Timing Configuration Register
#define TCONCS7                (*((uword volatile *) 0xEE48))

// Startup Memory CS Timing Configuration Register
#define TCONCSSM               (*((uword volatile *) 0xEE0E))

// Trap Flag Register
#define TFR                    (*((uword volatile *) 0xFFAC))
#define TFR_ILLOPA                   ((T_Reg16 *) 0xFFAC)->bit2
#define TFR_NMI                      ((T_Reg16 *) 0xFFAC)->bit15
#define TFR_PACER                    ((T_Reg16 *) 0xFFAC)->bit4
#define TFR_PRTFLT                   ((T_Reg16 *) 0xFFAC)->bit3
#define TFR_SOFTBRK                  ((T_Reg16 *) 0xFFAC)->bit12
#define TFR_STKOF                    ((T_Reg16 *) 0xFFAC)->bit14
#define TFR_STKUF                    ((T_Reg16 *) 0xFFAC)->bit13
#define TFR_UNDOPC                   ((T_Reg16 *) 0xFFAC)->bit7

// Vector Segment Pointer
#define VECSEG                 (*((uword volatile *) 0xFF12))

// Watchdog Timer Register
#define WDT                    (*((uword volatile *) 0xFEAE))

// Watchdog Timer Control Register
#define WDTCON                 (*((uword volatile *) 0xFFAE))

// Constant Value 0's Register
#define ZEROS                  (*((uword volatile *) 0xFF1C))


//****************************************************************************
// @Typedefs
//****************************************************************************

typedef unsigned char  ubyte;    // 1 byte unsigned; prefix: ub 
typedef signed char    sbyte;    // 1 byte signed;   prefix: sb 
typedef unsigned int   uword;    // 2 byte unsigned; prefix: uw 
typedef signed int     sword;    // 2 byte signed;   prefix: sw 
typedef unsigned long  ulong;    // 4 byte unsigned; prefix: ul 
typedef signed long    slong;    // 4 byte signed;   prefix: sl 

typedef volatile struct
{
  unsigned int    bit0      : 1;
  unsigned int    bit1      : 1;
  unsigned int    bit2      : 1;
  unsigned int    bit3      : 1;
  unsigned int    bit4      : 1;
  unsigned int    bit5      : 1;
  unsigned int    bit6      : 1;
  unsigned int    bit7      : 1;
  unsigned int    bit8      : 1;
  unsigned int    bit9      : 1;
  unsigned int    bit10     : 1;
  unsigned int    bit11     : 1;
  unsigned int    bit12     : 1;
  unsigned int    bit13     : 1;
  unsigned int    bit14     : 1;
  unsigned int    bit15     : 1;
}  T_Reg16;

// USER CODE BEGIN (MAIN_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,7)

// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************


void MAIN_vUnlockProtecReg(void);

// USER CODE BEGIN (MAIN_Header,8)

// USER CODE END


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,9)

// USER CODE END


//****************************************************************************
// @Project Includes
//****************************************************************************

#include <Intrins.h>

#include  "IO.H"
#include  "ASC0.H"
#include  "ASC1.H"
#include  "GPT1.H"
#include  "GPT2.H"

// USER CODE BEGIN (MAIN_Header,10)

// USER CODE END


#endif  // ifndef _MAIN_H_
