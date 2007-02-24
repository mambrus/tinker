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
#ifndef _DEFINES_H
#define _DEFINES_H

// This file contains some fundamental defines needed by the buildsystem
// I.e. constants that are used in evaluation since comparing a with
// an undefined constant equals comparing against zero, which might lead
// to confusion and un-intended results.

// config.h is including this file. You should never need to include it by 
// yourself.

// NOTE: the constants define what the kernel was/is compiled for and
// must not be confused with the built in constants used by your tool
// which might be different. One of the reasons of having these constants
// at all is for the application developper to be able to catch such 
// missmatches early in the build processs.

//-----------------------------------------------------------------------------
// Boolean constants:
//
// Please do not refere to __tk_yes or __tk_no in your application directly.
// They are only here because the buildsystem requires it.

#define __tk_no 0
#define __tk_yes 1

//-----------------------------------------------------------------------------
// Well known CPU architectures:
#define __tk_a29k__		1
#define __tk_arm__		2
#define __tk_configure__	3
#define __tk_cris__		4
#define __tk_crx__		5
#define __tk_d10v__		6
#define __tk_d30v__		7
#define __tk_fr30__		8
#define __tk_frv__		9
#define __tk_h8300__		10
#define __tk_h8500__		11
#define __tk_hppa__		12
#define __tk_i386__		13
#define __tk_i960__		14
#define __tk_iq2000__		15
#define __tk_m32c__		16
#define __tk_m32r__		17
#define __tk_m68hc11__		18
#define __tk_m68k__		19
#define __tk_m88k__		20
#define __tk_mips__		21
#define __tk_mn10200__		22
#define __tk_mn10300__		23
#define __tk_mt__		24
#define __tk_necv70__		25
#define __tk_powerpc__		26
#define __tk_sh__		27
#define __tk_sparc__		28
#define __tk_tic4x__		29
#define __tk_tic80__		30
#define __tk_v850__		31
#define __tk_w65__		32
#define __tk_xscale__		33
#define __tk_xstormy16__	34
#define __tk_z8k__		35

//-----------------------------------------------------------------------------
// Some well known systems:
#define __SYS_UNKNOWN__		0x100
#define __SYS_DEFAULT__		0x101
#define __SYS_HIXS__		0x102
#define __SYS_ANGEL_SWI__	0x103

//-----------------------------------------------------------------------------
// Some well known ABI's:
#define __tk_elf__		0x200
#define __tk_eabi__		0x201
#define __tk_eabisim__		0x202
#define __tk_rtems__		0x203

//-----------------------------------------------------------------------------
// Some well known boards:
#define __tk_unknown__		0x300
#define __tk_default__		0x301
#define __tk_localhost__	0x302

//-----------------------------------------------------------------------------
// Some well function names for emergency consoles
int console_write(const char*, int );
int emrgcy_write(const char*, int );

/*
* #define TK_ABI elf
#define TK_ALIAS_HOST powerpc-hixs-elf
* #define TK_ARCH powerpc
* #define TK_BOARD generic
#define TK_SYSTEM
*/


#endif // HIXS_H

