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
 

#ifndef TK_HWTYPES_GNU_BFIN_H
#define TK_HWTYPES_GNU_BFIN_H
//------1---------2---------3---------4---------5---------6---------7---------8

//------1---------2---------3---------4---------5---------6---------7---------8

#include <stddef.h>

/*! 
Architecture specific representation of a stack adress. In x86 this can be a 
simple char* even in 16bit modes, since a pointer contains both segment and 
offset.
*/
typedef struct{
   char * tstack;
   size_t stack_size;
} stack_t;


//------1---------2---------3---------4---------5---------6---------7---------8

#endif


  
/*!
 * @ingroup CVSLOG
 *
 *  $Log: tk_hwtypes-gnu_bfin_elf.h,v $
 *  Revision 1.2  2006-10-13 21:47:10  ambrmi09
 *  2 new CPU ports added: PowerPC and Blackfin. Also testing a new a new concept
 *  for thread creation that potentionally has big transparency advantages (needs
 *  to be proven in preemtive cases).
 *
 *  * PowerPC tested against simulator
 *  * Blackfin only compile and link (gcc and Newlib needes adaptation/porting -
 *    GDB and sinulator still missing)
 *
 *  Revision 1.2  2006/10/09 17:08:06  ambrmi09
 *  * Sceleton for bfin and powerpc now compiles.
 *  * Refingnemen in auto-tools to build correct BSP depending on toolchain and settings
 *  * BFIN,bfin macro work-around
 *
 *  Revision 1.1  2006/10/09 00:20:40  ambrmi09
 *  Sceleton for bfin and PPC added
 *
 *******************************************************************/