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
#ifndef CONTEXT_GNU_POWERP_ELF_H
#define CONTEXT_GNU_POWERP_ELF_H

#include <tinker/config.h>
#include "context-gnu_powerpc-eabi.h"
#include "bits.h"

#undef TK_CLI
#undef TK_STI

#if ( TK_DCPU == __tk_860__ )
#define _EIE 80
#define _EID 81

#define TK_CLI() SET_SPR(_EID,0xFF)
#define TK_STI() SET_SPR(_EIE,0xFF)

#else
#warning Can't handle interrupts safly for this ARCH/ABI combination
#define TK_CLI assert("Can't handle interrupts safely for this ARCH/ABI combination" == NULL)
#define TK_STI assert("Can't handle interrupts safely for this ARCH/ABI combination" == NULL)

#endif

#endif
