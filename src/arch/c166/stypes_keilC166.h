/***************************************************************************
 *   Copyright (C) 2006 by Michael Ambrus                                  *
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
#ifndef TK_HWTYPES_KEILC166_H
#define TK_HWTYPES_KEILC166_H

#include <stddef.h>

#pragma pack(1)
typedef union {
	unsigned long linear;
	union {
		struct {
			unsigned char padding8;
			unsigned int padding6:6;
			unsigned int _seg:10;
		} seg24;
		struct {
			unsigned int _offs:14;
			unsigned int padding2:2;
			unsigned char padding8;
		} offs24;
	} u;
} userstackaddr_t;

typedef struct {
	unsigned int TcI:3;
	unsigned int TcM:3;
	unsigned int TcR:1;
	unsigned int TcUD:1;
	unsigned int TcUDE:1;

	unsigned int TcOE:1;
	unsigned int TcOTL:1;
	unsigned int BPS1:2;

	unsigned int TcEDGE:1;
	unsigned int TcCHDIR:1;
	unsigned int TcRDIR:1;
} GPT1_ControlRegCore_t;

typedef struct {
	unsigned int TaI:3;
	unsigned int TaM:3;
	unsigned int TaR:1;
	unsigned int TaUD:1;
	unsigned int TaUDE:1;

	unsigned int TaRC:1;
	unsigned int _paddingA:2;
	unsigned int TaIRDIS:1;

	unsigned int TaEDGE:1;
	unsigned int TaCHDIR:1;
	unsigned int TaRDIR:1;
} GPT1_ControlRegAux_t;

typedef struct {
	unsigned int TcI:3;
	unsigned int TcM:3;
	unsigned int TcR:1;
	unsigned int TcUD:1;

	unsigned int _paddingC1:1;

	unsigned int TcOE:1;
	unsigned int TcOTL:1;
	unsigned int BPS2:2;

	unsigned int _paddingC2:1;
	unsigned int TcCLR:1;
	unsigned int TcSR:1;
} GPT2_ControlRegCore_t;

typedef struct {
	unsigned int TaI:3;
	unsigned int TaM:3;
	unsigned int TaR:1;
	unsigned int TaUD:1;

	unsigned int _paddingA1:1;

	unsigned int TaRC:1;
	unsigned int TaCTc:1;
	unsigned int TaCC:1;

	unsigned int TaCI:2;
	unsigned int TaCLR:1;
	unsigned int TaSC:1;
} GPT2_ControlRegAux_t;
typedef struct {
	unsigned int GLVL:2;
	unsigned int ILVL:4;
	unsigned int IE:1;
	unsigned int IR:1;
	unsigned int GPX:1;
	unsigned int _padding:7;
} IC_t;

#pragma pack()
typedef union {
	unsigned long linear;
	struct {
		unsigned int _offs;
		unsigned int _seg;
	} segmented;
	struct {
		unsigned int _SP;
		unsigned int _SPSEG;
	} reg;
} systemstackaddr_t;
typedef struct {
	systemstackaddr_t systemstack;
	userstackaddr_t userstack;

	size_t sys_stack_size;
	size_t usr_stack_size;
} tk_stack_t;

extern void _tk_reinit_stackaddr_xc167keil(tk_stack_t * addr, size_t size);
#endif
