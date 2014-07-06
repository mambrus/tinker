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

//------1---------2---------3---------4---------5---------6---------7---------8
#ifndef TK_TUNING_H
#define TK_TUNING_H

#if defined(__GNUC__)
#include <tinker/config.h>
#endif

#define TRAP( NUM )     \
   tk_trap( NUM )

#include <arch/powerpc/bits.h>
#include "CPU/860/mmap_regs.h"

/*!
Called by handler to notify inerrupt controller that ISR is finished (or interrupt is handled).

   If the interrupt source is IRQx edge-triggered, clear the service bit
   in the SI Pending Register(SIPEND). Bit is cleared by writing a
   one to it.

@note Assuming all external IRQ[0..7] are edge trigged

@note This function/macro is mandatory by TinKer API

@attention Bit is <b>cleared</b> by writing <b>ONE</b> to it (@!# Motorola ;( )

*/
#define tk_isr_eoi(level) \
		bitset(SIPEND,level);

/* No need to set any more tunings here. This port exists only for GNU and configure.in
at this level contains all the defaults we need. */

#endif				//TK_TUNING_H

