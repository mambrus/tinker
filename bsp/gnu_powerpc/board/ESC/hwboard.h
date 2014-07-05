/*+***************************************************************************
**
**
**
**
** Original Author: Mihael Ambrus	Creation Date: 070213
**
** Approved:	Department: MCC-VS
**
**                 Copyright (c) Maquet Critical Care, Sweden
**                        All rights reserved
**
***************************************************************************-*/

#ifndef hwboard_h
#define hwboard_h
/*
Some stuff to define the fundamentals of the board that neighter the build-
nor the run-time system can figure out by themselves.
*/

/*
Frequency of the clock that drives the chip. I.e. either EXTCLK or OSCM
(XTAL/EXTAL input - crystal frequency). The system does not care what type
of circuit that frives the chip, only the frequency. Hence OSCCLK...
*/

#define F_OSCCLK	4000000

/*
Where internal memory mapping for SPR and DP-RAM is located.
Omitted for now since it's derived from the bdi2000 setting and extracted
into mmap_regs.h. Will change this later....
*/
//#define DMM1    0xFA200000

/*
Defines if the CM uses some of the DPRAM for internal needs
In ch.h for now...
*/
//#define DP_ERAM 3

#endif				//hwboard_h
