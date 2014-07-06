/***************************************************************************
 *   Copyright (C) 2005,2006 by Michael Ambrus                             *
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

#ifndef TK_HWTYPES_H
#define TK_HWTYPES_H

#if defined(DOXYGEN_ONLY)

//------1---------2---------3---------4---------5---------6---------7---------8

//------1---------2---------3---------4---------5---------6---------7---------8

#endif				//DOXYGEN_ONLY

//------1---------2---------3---------4---------5---------6---------7---------8

#if defined(_WIN32) &&  defined(_MSC_VER)
#include <kernel/bsp/X86_msvc/tk_hwtypes_mvcsX86.h>

#elif defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
#include <../bsp/X86_Borland/tk_hwtypes_borlandX86.h>

#elif defined( __C166__ )
#include <../bsp/XC167_Keil/tk_hwtypes_keilC166.h>

#elif defined(__GNUC__)

#ifndef ARCH
#error For GNU targets, ARCH has to be defined
#endif

#define str( x ) \
      #x

#define INCLNAME( farch ) \
      <../src/arch/farch/stypes-gnu_ farch.h>

#define INCLABI( farch, abi )                      \
      <../src/arch/farch/stypes-gnu_ farch-abi.h>

#ifdef ABI
#include INCLABI( ARCH, ABI )
#else
#include INCLNAME( ARCH )
#endif

#undef str
#undef INCLNAME
#undef INCLABI

#else
#error "Can\'t determine the target for the TINKER kernel"

#endif

//------1---------2---------3---------4---------5---------6---------7---------8

#endif				//TK_HWSYS_H

