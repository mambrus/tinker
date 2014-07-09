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

/*
   This file contains some fundamental defines needed by the
   build-/configure-system I.e. constants that are used in evaluation since
   comparing a with an undefined constant equals comparing against zero,
   which might lead to confusion and unintended results.

   config.h is including this file. You should never need to include it by
   yourself.

   NOTE: the constants define what the kernel was/is compiled for and must
   not be confused with the built in constants used by your tool which might
   be different. One of the reasons of having these constants at all is for
   the application developer to be able to catch such mismatches early in
   the build process.
*/

/*
----------------------------------------------------------------------------
Boolean constants:

Please do not revere to __tk_yes or __tk_no in your application directly.
They are only here because the build-system requires it.
*/
#define __tk_no   0
#define __tk_yes  1

/*
----------------------------------------------------------------------------
Well known CPU architectures:
*/
#define __tk_a29k__        1
#define __tk_arm__         2
#define __tk_configure__   3
#define __tk_cris__        4
#define __tk_crx__         5
#define __tk_d10v__        6
#define __tk_d30v__        7
#define __tk_fr30__        8
#define __tk_frv__         9
#define __tk_h8300__       10
#define __tk_h8500__       11
#define __tk_hppa__        12
#define __tk_i386__        13
#define __tk_i960__        14
#define __tk_iq2000__      15
#define __tk_m32c__        16
#define __tk_m32r__        17
#define __tk_m68hc11__     18
#define __tk_m68k__        19
#define __tk_m88k__        20
#define __tk_mips__        21
#define __tk_mn10200__     22
#define __tk_mn10300__     23
#define __tk_mt__          24
#define __tk_necv70__      25
#define __tk_powerpc__     26
#define __tk_sh__          27
#define __tk_sparc__       28
#define __tk_tic4x__       29
#define __tk_tic80__       30
#define __tk_v850__        31
#define __tk_w65__         32
#define __tk_xscale__      33
#define __tk_xstormy16__   34
#define __tk_z8k__         35
#define __tk_x86_64__      36
#define __tk_armv7l__      38

/*
----------------------------------------------------------------------------
Some well known systems. Bit-masks as a system can be a combo

Combo examples, examples i.e. beyond the obvious:
1)
__SYS_KVM_ | __SYS_HOSTED__ | __SYS_HIXS__

=> Means TinKer is configured to run "hosted" with the HIXS sys-call API on
a kernel that in turn is a VM-guest

2)
__SYS_KVM_ | __SYS_HIXS__

=> Means TinKer is configured to run a as a vm, possibly a QEMU emulator.
Meaning tinker core thinks it runs a real HW. Most components & drivers can be
enabled, but it also doesn't have any help sorting, handling console e.t.a.
which has to be enabled (build-system will figure that out) BUT the drivers
may need to know the HW is virtual, as in some cases a driver will be
exchanged to a KVM virtio driver.

*/
#define __SYS_UNKNOWN__    0x100
#define __SYS_DEFAULT__    0x101
#define __SYS_HIXS__       0x102
#define __SYS_ANGEL_SWI__  0x104
#define __SYS_HOSTED__     0x108
#define __SYS_KVM__        0x110

/*
Some well known ABI's:
*/
#define __tk_elf__         0x200
#define __tk_eabi__        0x201
#define __tk_eabisim__     0x202
#define __tk_rtems__       0x203

/*
Some well known boards:
*/
#define __tk_unknown__     0x300
#define __tk_default__     0x301
#define __tk_localhost__   0x302

/*
Some well known cannonical triplet parts
*/

/* === CPU === */
#define CANONICAL_CPU_a29k      __tk_a29k__
#define CANONICAL_CPU_arm       __tk_arm__
#define CANONICAL_CPU_configure __tk_configure__
#define CANONICAL_CPU_cris      __tk_cris__
#define CANONICAL_CPU_crx       __tk_crx__
#define CANONICAL_CPU_d10v      __tk_d10v__
#define CANONICAL_CPU_d30v      __tk_d30v__
#define CANONICAL_CPU_fr30      __tk_fr30__
#define CANONICAL_CPU_frv       __tk_frv__
#define CANONICAL_CPU_h8300     __tk_h8300__
#define CANONICAL_CPU_h8500     __tk_h8500__
#define CANONICAL_CPU_hppa      __tk_hppa__
#define CANONICAL_CPU_i386      __tk_i386__
#define CANONICAL_CPU_i960      __tk_i960__
#define CANONICAL_CPU_iq2000    __tk_iq2000__
#define CANONICAL_CPU_m32c      __tk_m32c__
#define CANONICAL_CPU_m32r      __tk_m32r__
#define CANONICAL_CPU_m68hc11   __tk_m68hc11__
#define CANONICAL_CPU_m68k      __tk_m68k__
#define CANONICAL_CPU_m88k      __tk_m88k__
#define CANONICAL_CPU_mips      __tk_mips__
#define CANONICAL_CPU_mn10200   __tk_mn10200__
#define CANONICAL_CPU_mn10300   __tk_mn10300__
#define CANONICAL_CPU_mt        __tk_mt__
#define CANONICAL_CPU_necv70    __tk_necv70__
#define CANONICAL_CPU_powerpc   __tk_powerpc__
#define CANONICAL_CPU_sh        __tk_sh__
#define CANONICAL_CPU_sparc     __tk_sparc__
#define CANONICAL_CPU_tic4x     __tk_tic4x__
#define CANONICAL_CPU_tic80     __tk_tic80__
#define CANONICAL_CPU_v850      __tk_v850__
#define CANONICAL_CPU_w65       __tk_w65__
#define CANONICAL_CPU_xscale    __tk_xscale__
#define CANONICAL_CPU_xstormy16 __tk_xstormy16__
#define CANONICAL_CPU_z8k       __tk_z8k__
#define CANONICAL_CPU_x86_64    __tk_x86_64__
#define CANONICAL_CPU_armv7l    __tk_armv7l__

/* === VENDOR === */
#define CANON_VENDOR_unknown       0x400
#define CANON_VENDOR_hixs          0x401

/* === OS === */
#define CANONICAL_OS_linux_gnu    	0x500
#define CANON_OS_linux_gnueabihf   0x501

/* === --- === */

/*
Some well known function names for emergency consoles:
*/
int console_write(const char *, int);
int emrgcy_write(const char *, int);

#endif				// HIXS_H
