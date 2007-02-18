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

#ifndef bits_h
#define bits_h

#include <sys/types.h>


/* Helpers for revbits (see below) */
static __inline__ __uint8_t revbits_8(__uint8_t in){
	__uint8_t d = (sizeof(in)*8)-1;
	__uint8_t l = (sizeof(in)*8)/2;
	__uint8_t i;
	__uint8_t out = 0;
	__uint8_t hm = 0x80;
	__uint8_t lm = 0x01;
	__uint8_t hv;
	__uint8_t lv;

	for (i=0; i<l;i++){
		hv = in & hm;
		lv = in & lm;
		hv = hv >> d;
		lv = lv << d;
		out = out | hv;
		out = out | lv;
		d = d - 2;		
		hm = (hm >> 1);	
		lm = (lm << 1);
	}
	return out;
}

static __inline__ __uint16_t revbits_16(__uint16_t in){
	__uint8_t d = (sizeof(in)*8)-1;
	__uint8_t l = (sizeof(in)*8)/2;
	__uint8_t i;
	__uint16_t out = 0;
	__uint16_t hm = 0x8000;
	__uint16_t lm = 0x0001;
	__uint16_t hv;
	__uint16_t lv;

	for (i=0; i<l;i++){
		hv = in & hm;
		lv = in & lm;
		hv = hv >> d;
		lv = lv << d;
		out = out | hv;
		out = out | lv;
		d = d - 2;		
		hm = (hm >> 1);	
		lm = (lm << 1);
	}
	return out;
}


static __inline__ __uint32_t revbits_32(__uint32_t in){
	__uint8_t d = (sizeof(in)*8)-1;
	__uint8_t l = (sizeof(in)*8)/2;
	__uint8_t i;
	__uint32_t out = 0;
	__uint32_t hm = 0x80000000;
	__uint32_t lm = 0x00000001;
	__uint32_t hv;
	__uint32_t lv;

	for (i=0; i<l;i++){
		hv = in & hm;
		lv = in & lm;
		hv = hv >> d;
		lv = lv << d;
		out = out | hv;
		out = out | lv;
		d = d - 2;		
		hm = (hm >> 1);	
		lm = (lm << 1);
	}
	return out;
}

/* Reverse the bit order in an integer type valiable (1,2,4 byte 
   integers supported) 							     */
/* ------------------------------------------------------------------------- */
#define revbits( in ) 				\
	(sizeof(in) == 1) ? revbits_8(in) : 	\
	(sizeof(in) == 2) ? revbits_16(in) : 	\
	(sizeof(in) == 4) ? revbits32(in) : 0x0


/* Helpers for bit set/clear (see below) */
#define bitset_8( d, n ) \
	d = d | (0x80 >> n)

#define bitclear_8( d, n ) \
	d = d & ~(0x80 >> n)


#define bitset_16( d, n ) \
	d = d | (0x8000 >> n)

#define bitclear_16( d, n ) \
	d = d & ~(0x8000 >> n)

#define bitset_32( d, n ) \
	d = d | (0x80000000 >> n)

#define bitclear_32( d, n ) \
	d = d & ~(0x80000000 >> n)
	

/* Set and Clear specific bits in an integer type valiable (1,2,4 byte 
   integers supported) 							     */
/* Note: bit number order is from left to right 			     */
/* ------------------------------------------------------------------------- */
#define bitset( d, n ) 				\
	if (sizeof(d) == 1) 			\
		bitset_8(d, n);			\
	if (sizeof(d) == 2)			\
		bitset_16(d, n);		\
	if (sizeof(d) == 4)			\
		bitset_32(d, n);

#define bitclear( d, n ) 			\
	if (sizeof(d) == 1)			\
		bitclear_8(d, n);		\
	if (sizeof(d) == 2)			\
		bitclear_16(d, n);		\
	if (sizeof(d) == 4)			\
		bitclear_32(d, n);


/* Support for reading and writing SPR  (Special Purpose Registers)	     */
/* ------------------------------------------------------------------------- */
#define SET_SPR( SPRc, INc ) 				\
	__asm__ __volatile__ ("mtspr %[SPRa],%[INa]"	\
	:						\
	: [SPRa] "i" (SPRc), [INa] "r" (INc)		\
	: "memory" );

#define GET_SPR( SPRc, OUTc ) 				\
	__asm__ __volatile__ ("mfspr %[OUTa],%[SPRa]"	\
	: [OUTa] "=r" (OUTc)				\
	: [SPRa] "i" (SPRc)				\
	: "memory" );

/* Support for reading and writing GPR's  (General Purpose Registers)	     */
/* ------------------------------------------------------------------------- */
#define SET_GPR( GPRc, INc ) 				\
	__asm__ __volatile__ ("lwz %[GPRa],%[INa]"	\
	:						\
	: [GPRa] "i" (GPRc), [INa] "m" (INc)		\
	); /*Note, no clobber (intentional)*/

#define GET_GPR( GPRc, OUTc ) 				\
	__asm__ __volatile__ ("stw %[GPRa],%[OUTa]"	\
	: [OUTa] "=m" (OUTc)				\
	: [GPRa] "i" (GPRc)				\
	: "memory" );


/* Support for reading and writing CR  (Condition Register)	             */
/* ------------------------------------------------------------------------- */
#define SET_CR( INc ) 					\
	__asm__ __volatile__ ("mtcr %[INa]"		\
	:						\
	: [INa] "r" (INc)				\
	: "memory" );

#define GET_CR( OUTc )					\
	__asm__ __volatile__ ("mfcr %[OUTa]"		\
	: [OUTa] "=r" (OUTc)				\
	: 						\
	: "memory" );



#endif //bits_h



