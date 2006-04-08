/***********************************************************************/
/*  This file is part of the uVision/ARM development tools             */
/*  Copyright KEIL ELEKTRONIK GmbH 2002-2003                           */
/***********************************************************************/
/*                                                                     */
/*  LPC21XX.H:  Header file for Philips LPC2114 / LPC2119              */
/*                                      LPC2124 / LPC2129              */
/*                                                                     */
/***********************************************************************/

#ifndef __LPC21xx_H
#define __LPC21xx_H

/* Vectored Interrupt Controller (VIC) */
#define VICIRQStatus   (*((volatile unsigned long *) 0xFFFFF000u))
#define VICFIQStatus   (*((volatile unsigned long *) 0xFFFFF004u))
#define VICRawIntr     (*((volatile unsigned long *) 0xFFFFF008u))
#define VICIntSelect   (*((volatile unsigned long *) 0xFFFFF00Cu))
#define VICIntEnable   (*((volatile unsigned long *) 0xFFFFF010u))
#define VICIntEnClr    (*((volatile unsigned long *) 0xFFFFF014u))
#define VICSoftInt     (*((volatile unsigned long *) 0xFFFFF018u))
#define VICSoftIntClr  (*((volatile unsigned long *) 0xFFFFF01Cu))
#define VICProtection  (*((volatile unsigned long *) 0xFFFFF020u))
#define VICVectAddr    (*((volatile unsigned long *) 0xFFFFF030u))
#define VICDefVectAddr (*((volatile unsigned long *) 0xFFFFF034u))
#define VICVectAddr0   (*((volatile unsigned long *) 0xFFFFF100u))
#define VICVectAddr1   (*((volatile unsigned long *) 0xFFFFF104u))
#define VICVectAddr2   (*((volatile unsigned long *) 0xFFFFF108u))
#define VICVectAddr3   (*((volatile unsigned long *) 0xFFFFF10Cu))
#define VICVectAddr4   (*((volatile unsigned long *) 0xFFFFF110u))
#define VICVectAddr5   (*((volatile unsigned long *) 0xFFFFF114u))
#define VICVectAddr6   (*((volatile unsigned long *) 0xFFFFF118u))
#define VICVectAddr7   (*((volatile unsigned long *) 0xFFFFF11Cu))
#define VICVectAddr8   (*((volatile unsigned long *) 0xFFFFF120u))
#define VICVectAddr9   (*((volatile unsigned long *) 0xFFFFF124u))
#define VICVectAddr10  (*((volatile unsigned long *) 0xFFFFF128u))
#define VICVectAddr11  (*((volatile unsigned long *) 0xFFFFF12Cu))
#define VICVectAddr12  (*((volatile unsigned long *) 0xFFFFF130u))
#define VICVectAddr13  (*((volatile unsigned long *) 0xFFFFF134u))
#define VICVectAddr14  (*((volatile unsigned long *) 0xFFFFF138u))
#define VICVectAddr15  (*((volatile unsigned long *) 0xFFFFF13Cu))
#define VICVectCntl0   (*((volatile unsigned long *) 0xFFFFF200u))
#define VICVectCntl1   (*((volatile unsigned long *) 0xFFFFF204u))
#define VICVectCntl2   (*((volatile unsigned long *) 0xFFFFF208u))
#define VICVectCntl3   (*((volatile unsigned long *) 0xFFFFF20Cu))
#define VICVectCntl4   (*((volatile unsigned long *) 0xFFFFF210u))
#define VICVectCntl5   (*((volatile unsigned long *) 0xFFFFF214u))
#define VICVectCntl6   (*((volatile unsigned long *) 0xFFFFF218u))
#define VICVectCntl7   (*((volatile unsigned long *) 0xFFFFF21Cu))
#define VICVectCntl8   (*((volatile unsigned long *) 0xFFFFF220u))
#define VICVectCntl9   (*((volatile unsigned long *) 0xFFFFF224u))
#define VICVectCntl10  (*((volatile unsigned long *) 0xFFFFF228u))
#define VICVectCntl11  (*((volatile unsigned long *) 0xFFFFF22Cu))
#define VICVectCntl12  (*((volatile unsigned long *) 0xFFFFF230u))
#define VICVectCntl13  (*((volatile unsigned long *) 0xFFFFF234u))
#define VICVectCntl14  (*((volatile unsigned long *) 0xFFFFF238u))
#define VICVectCntl15  (*((volatile unsigned long *) 0xFFFFF23Cu))

/* Pin Connect Block */
#define PINSEL0        (*((volatile unsigned long *) 0xE002C000u))
#define PINSEL1        (*((volatile unsigned long *) 0xE002C004u))
#define PINSEL2        (*((volatile unsigned long *) 0xE002C014u))

/* General Purpose Input/Output (GPIO) */
#define IOPIN0         (*((volatile unsigned long *) 0xE0028000u))
#define IOSET0         (*((volatile unsigned long *) 0xE0028004u))
#define IODIR0         (*((volatile unsigned long *) 0xE0028008u))
#define IOCLR0         (*((volatile unsigned long *) 0xE002800Cu))
#define IOPIN1         (*((volatile unsigned long *) 0xE0028010u))
#define IOSET1         (*((volatile unsigned long *) 0xE0028014u))
#define IODIR1         (*((volatile unsigned long *) 0xE0028018u))
#define IOCLR1         (*((volatile unsigned long *) 0xE002801Cu))

/* Memory Accelerator Module (MAM) */
#define MAMCR          (*((volatile unsigned char *) 0xE01FC000u))
#define MAMTIM         (*((volatile unsigned char *) 0xE01FC004u))
#define MAMMAP         (*((volatile unsigned char *) 0xE01FC040u))

/* Phase Locked Loop (PLL) */
#define PLLCON         (*((volatile unsigned char *) 0xE01FC080u))
#define PLLCFG         (*((volatile unsigned char *) 0xE01FC084u))
#define PLLSTAT        (*((volatile unsigned short*) 0xE01FC088u))
#define PLLFEED        (*((volatile unsigned char *) 0xE01FC08Cu))

/* VPB Divider */
#define VPBDIV         (*((volatile unsigned char *) 0xE01FC100u))

/* Power Control */
#define PCON           (*((volatile unsigned char *) 0xE01FC0C0u))
#define PCONP          (*((volatile unsigned long *) 0xE01FC0C4u))

/* External Interrupts */
#define EXTINT         (*((volatile unsigned char *) 0xE01FC140u))
#define EXTWAKE        (*((volatile unsigned char *) 0xE01FC144u))
#define EXTMODE        (*((volatile unsigned char *) 0xE01FC148u))
#define EXTPOLAR       (*((volatile unsigned char *) 0xE01FC14Cu))

/* Timer 0 */
#define TIMER0_IR      (*((volatile unsigned long *) 0xE0004000u))
#define TIMER0_TCR     (*((volatile unsigned long *) 0xE0004004u))
#define TIMER0_TC      (*((volatile unsigned long *) 0xE0004008u))
#define TIMER0_PR      (*((volatile unsigned long *) 0xE000400Cu))
#define TIMER0_PC      (*((volatile unsigned long *) 0xE0004010u))
#define TIMER0_MCR     (*((volatile unsigned long *) 0xE0004014u))
#define TIMER0_MR0     (*((volatile unsigned long *) 0xE0004018u))
#define TIMER0_MR1     (*((volatile unsigned long *) 0xE000401Cu))
#define TIMER0_MR2     (*((volatile unsigned long *) 0xE0004020u))
#define TIMER0_MR3     (*((volatile unsigned long *) 0xE0004024u))
#define TIMER0_CCR     (*((volatile unsigned long *) 0xE0004028u))
#define TIMER0_CR0     (*((volatile unsigned long *) 0xE000402Cu))
#define TIMER0_CR1     (*((volatile unsigned long *) 0xE0004030u))
#define TIMER0_CR2     (*((volatile unsigned long *) 0xE0004034u))
#define TIMER0_CR3     (*((volatile unsigned long *) 0xE0004038u))
#define TIMER0_EMR     (*((volatile unsigned long *) 0xE000403Cu))

/* Timer 1 */
#define TIMER1_IR      (*((volatile unsigned long *) 0xE0008000u))
#define TIMER1_TCR     (*((volatile unsigned long *) 0xE0008004u))
#define TIMER1_TC      (*((volatile unsigned long *) 0xE0008008u))
#define TIMER1_PR      (*((volatile unsigned long *) 0xE000800Cu))
#define TIMER1_PC      (*((volatile unsigned long *) 0xE0008010u))
#define TIMER1_MCR     (*((volatile unsigned long *) 0xE0008014u))
#define TIMER1_MR0     (*((volatile unsigned long *) 0xE0008018u))
#define TIMER1_MR1     (*((volatile unsigned long *) 0xE000801Cu))
#define TIMER1_MR2     (*((volatile unsigned long *) 0xE0008020u))
#define TIMER1_MR3     (*((volatile unsigned long *) 0xE0008024u))
#define TIMER1_CCR     (*((volatile unsigned long *) 0xE0008028u))
#define TIMER1_CR0     (*((volatile unsigned long *) 0xE000802Cu))
#define TIMER1_CR1     (*((volatile unsigned long *) 0xE0008030u))
#define TIMER1_CR2     (*((volatile unsigned long *) 0xE0008034u))
#define TIMER1_CR3     (*((volatile unsigned long *) 0xE0008038u))
#define TIMER1_EMR     (*((volatile unsigned long *) 0xE000803Cu))

/* Pulse Width Modulator (PWM) */
#define PWM_IR         (*((volatile unsigned long *) 0xE0014000u))
#define PWM_TCR        (*((volatile unsigned long *) 0xE0014004u))
#define PWM_TC         (*((volatile unsigned long *) 0xE0014008u))
#define PWM_PR         (*((volatile unsigned long *) 0xE001400Cu))
#define PWM_PC         (*((volatile unsigned long *) 0xE0014010u))
#define PWM_MCR        (*((volatile unsigned long *) 0xE0014014u))
#define PWM_MR0        (*((volatile unsigned long *) 0xE0014018u))
#define PWM_MR1        (*((volatile unsigned long *) 0xE001401Cu))
#define PWM_MR2        (*((volatile unsigned long *) 0xE0014020u))
#define PWM_MR3        (*((volatile unsigned long *) 0xE0014024u))
#define PWM_MR4        (*((volatile unsigned long *) 0xE0014040u))
#define PWM_MR5        (*((volatile unsigned long *) 0xE0014044u))
#define PWM_MR6        (*((volatile unsigned long *) 0xE0014048u))
#define PWM_CCR        (*((volatile unsigned long *) 0xE0014028u))
#define PWM_CR0        (*((volatile unsigned long *) 0xE001402Cu))
#define PWM_CR1        (*((volatile unsigned long *) 0xE0014030u))
#define PWM_CR2        (*((volatile unsigned long *) 0xE0014034u))
#define PWM_CR3        (*((volatile unsigned long *) 0xE0014038u))
#define PWM_EMR        (*((volatile unsigned long *) 0xE001403Cu))
#define PWM_PCR        (*((volatile unsigned long *) 0xE001404Cu))
#define PWM_LER        (*((volatile unsigned long *) 0xE0014050u))

/* Universal Asynchronous Receiver Transmitter 0 (UART0) */
#define UART0_RBR      (*((volatile unsigned char *) 0xE000C000u))
#define UART0_THR      (*((volatile unsigned char *) 0xE000C000u))
#define UART0_IER      (*((volatile unsigned char *) 0xE000C004u))
#define UART0_IIR      (*((volatile unsigned char *) 0xE000C008u))
#define UART0_FCR      (*((volatile unsigned char *) 0xE000C008u))
#define UART0_LCR      (*((volatile unsigned char *) 0xE000C00Cu))
#define UART0_MCR      (*((volatile unsigned char *) 0xE000C010u))
#define UART0_LSR      (*((volatile unsigned char *) 0xE000C014u))
#define UART0_MSR      (*((volatile unsigned char *) 0xE000C018u))
#define UART0_SCR      (*((volatile unsigned char *) 0xE000C01Cu))
#define UART0_DLL      (*((volatile unsigned char *) 0xE000C000u))
#define UART0_DLM      (*((volatile unsigned char *) 0xE000C004u))

/* Universal Asynchronous Receiver Transmitter 1 (UART1) */
#define UART1_RBR      (*((volatile unsigned char *) 0xE0010000u))
#define UART1_THR      (*((volatile unsigned char *) 0xE0010000u))
#define UART1_IER      (*((volatile unsigned char *) 0xE0010004u))
#define UART1_IIR      (*((volatile unsigned char *) 0xE0010008u))
#define UART1_FCR      (*((volatile unsigned char *) 0xE0010008u))
#define UART1_LCR      (*((volatile unsigned char *) 0xE001000Cu))
#define UART1_MCR      (*((volatile unsigned char *) 0xE0010010u))
#define UART1_LSR      (*((volatile unsigned char *) 0xE0010014u))
#define UART1_MSR      (*((volatile unsigned char *) 0xE0010018u))
#define UART1_SCR      (*((volatile unsigned char *) 0xE001001Cu))
#define UART1_DLL      (*((volatile unsigned char *) 0xE0010000u))
#define UART1_DLM      (*((volatile unsigned char *) 0xE0010004u))

/* I2C Interface */
#define I2C0_I2CONSET   (*((volatile unsigned char *) 0xE001C000u))
#define I2C0_I2STAT     (*((volatile unsigned char *) 0xE001C004u))
#define I2C0_I2DAT      (*((volatile unsigned char *) 0xE001C008u))
#define I2C0_I2ADR      (*((volatile unsigned char *) 0xE001C00Cu))
#define I2C0_I2SCLH     (*((volatile unsigned short*) 0xE001C010u))
#define I2C0_I2SCLL     (*((volatile unsigned short*) 0xE001C014u))
#define I2C0_I2CONCLR   (*((volatile unsigned char *) 0xE001C018u))
#define I2C1_I2CONSET   (*((volatile unsigned char *) 0xE005C000u))
#define I2C1_I2STAT     (*((volatile unsigned char *) 0xE005C004u))
#define I2C1_I2DAT      (*((volatile unsigned char *) 0xE005C008u))
#define I2C1_I2ADR      (*((volatile unsigned char *) 0xE005C00Cu))
#define I2C1_I2SCLH     (*((volatile unsigned short*) 0xE005C010u))
#define I2C1_I2SCLL     (*((volatile unsigned short*) 0xE005C014u))
#define I2C1_I2CONCLR   (*((volatile unsigned char *) 0xE005C018u))

/* SPI0 (Serial Peripheral Interface 0) */
#define SPI0_SPCR      (*((volatile unsigned char *) 0xE0020000u))
#define SPI0_SPSR      (*((volatile unsigned char *) 0xE0020004u))
#define SPI0_SPDR      (*((volatile unsigned char *) 0xE0020008u))
#define SPI0_SPCCR     (*((volatile unsigned char *) 0xE002000Cu))
#define SPI0_SPTCR     (*((volatile unsigned char *) 0xE0020010u))
#define SPI0_SPTSR     (*((volatile unsigned char *) 0xE0020014u))
#define SPI0_SPTOR     (*((volatile unsigned char *) 0xE0020018u))
#define SPI0_SPINT     (*((volatile unsigned char *) 0xE002001Cu))

/* SPI1 (Serial Peripheral Interface 1) */
#define SPI1_SPCR      (*((volatile unsigned char *) 0xE0030000u))
#define SPI1_SPSR      (*((volatile unsigned char *) 0xE0030004u))
#define SPI1_SPDR      (*((volatile unsigned char *) 0xE0030008u))
#define SPI1_SPCCR     (*((volatile unsigned char *) 0xE003000Cu))
#define SPI1_SPTCR     (*((volatile unsigned char *) 0xE0030010u))
#define SPI1_SPTSR     (*((volatile unsigned char *) 0xE0030014u))
#define SPI1_SPTOR     (*((volatile unsigned char *) 0xE0030018u))
#define SPI1_SPINT     (*((volatile unsigned char *) 0xE003001Cu))

/* Real Time Clock */
#define RTC_ILR        (*((volatile unsigned char *) 0xE0024000u))
#define RTC_CTC        (*((volatile unsigned short*) 0xE0024004u))
#define RTC_CCR        (*((volatile unsigned char *) 0xE0024008u))
#define RTC_CIIR       (*((volatile unsigned char *) 0xE002400Cu))
#define RTC_AMR        (*((volatile unsigned char *) 0xE0024010u))
#define RTC_CTIME0     (*((volatile unsigned long *) 0xE0024014u))
#define RTC_CTIME1     (*((volatile unsigned long *) 0xE0024018u))
#define RTC_CTIME2     (*((volatile unsigned long *) 0xE002401Cu))
#define RTC_SEC        (*((volatile unsigned char *) 0xE0024020u))
#define RTC_MIN        (*((volatile unsigned char *) 0xE0024024u))
#define RTC_HOUR       (*((volatile unsigned char *) 0xE0024028u))
#define RTC_DOM        (*((volatile unsigned char *) 0xE002402Cu))
#define RTC_DOW        (*((volatile unsigned char *) 0xE0024030u))
#define RTC_DOY        (*((volatile unsigned short*) 0xE0024034u))
#define RTC_MONTH      (*((volatile unsigned char *) 0xE0024038u))
#define RTC_YEAR       (*((volatile unsigned short*) 0xE002403Cu))
#define RTC_ALSEC      (*((volatile unsigned char *) 0xE0024060u))
#define RTC_ALMIN      (*((volatile unsigned char *) 0xE0024064u))
#define RTC_ALHOUR     (*((volatile unsigned char *) 0xE0024068u))
#define RTC_ALDOM      (*((volatile unsigned char *) 0xE002406Cu))
#define RTC_ALDOW      (*((volatile unsigned char *) 0xE0024070u))
#define RTC_ALDOY      (*((volatile unsigned short*) 0xE0024074u))
#define RTC_ALMON      (*((volatile unsigned char *) 0xE0024078u))
#define RTC_ALYEAR     (*((volatile unsigned short*) 0xE002407Cu))
#define RTC_PREINT     (*((volatile unsigned short*) 0xE0024080u))
#define RTC_PREFRAC    (*((volatile unsigned short*) 0xE0024084u))

/* A/D Converter */
#define ADCR           (*((volatile unsigned long *) 0xE0034000u))
#define ADDR           (*((volatile unsigned long *) 0xE0034004u))

/* Watchdog */
#define WDMOD          (*((volatile unsigned char *) 0xE0000000u))
#define WDTC           (*((volatile unsigned long *) 0xE0000004u))
#define WDFEED         (*((volatile unsigned char *) 0xE0000008u))
#define WDTV           (*((volatile unsigned long *) 0xE000000Cu))


/*** Fuer FreeRTOS ***/
#define T0_PR           TIMER0_PR
#define T0_IR           TIMER0_IR
#define T0_MR0          TIMER0_MR0
#define T0_MCR          TIMER0_MCR
#define T0_TCR          TIMER0_TCR

#endif  // __LPC21xx_H