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

/*!
@file

This file is a Blackfin elf


*/

#ifndef TK_HWSYS_GNU_BFIN_H
#define TK_HWSYS_GNU_BFIN_H

#include <setjmp.h>

#define EXTRA_MARGIN 20		//<! Define SP this below the theoretical top (some compilers require it)
#define JUMPER_BASED

/*!
@name Mapping stack allocation API for this target
*/
//@{
#define stalloc      malloc
#define stalloc_free free
//@}

#define MAGIC_NUMBER 7327

#define TK_CLI()		/*TBD FIXME */
#define TK_STI()		/*TBD FIXME */

#define REAL_STACK_SIZE( TCB )            \
   ( TCB.stack_size )

#define PUSHALL()		/*No need to PUSHALL on this target- Already done by setjmp */
#define POPALL()		/*No need to POPALL on this target- Already done by longjmp */

#define GET_SP( OUT_SP ) 						\
	asm __volatile__ (							\
		"%[mystack] = SP;"						\
      : [mystack] "=m" (OUT_SP)					\
      : /**/									\
      : "memory"								\
   );

#define SET_SP( IN_SP )							\
   asm __volatile__ (							\
		"SP = %[mystack];"						\
      : /**/									\
      : [mystack] "m" (IN_SP)					\
   );				/*Note, no clobber (intentional) */

#define PUSH_CPU_GETCUR_STACK( TSP1, TEMP )     \
   GET_SP( TSP1 )								\
   TEMP = setjmp( TSP1 - (_JBLEN*sizeof(double)) - EXTRA_MARGIN);              \
   if (TEMP != (active_thread+1))				\
      GET_SP( TSP1 )

#define CHANGE_STACK_POP_CPU( TSP1, TEMP )		\
	longjmp( TSP1 - (_JBLEN*sizeof(double)) - EXTRA_MARGIN, active_thread+1);

#define CHANGE_STACK( TSP1, TEMP )        		\
  SET_SP( TSP1 )

#define INIT_SP( _stack_SP, _stack_begin )\
   _stack_SP.stack_size = _stack_begin.stack_size - EXTRA_MARGIN; 					\
   _stack_SP.tstack = _stack_begin.tstack + _stack_begin.stack_size - EXTRA_MARGIN;

//Does nothing on this port
#define BIND_STACK( _stack_struct, _temp2 )

//Allready a char', no need to do handle in any special way.
#define STACK_PTR( ADDR ) \
   (ADDR.tstack)

//Not needed to do anything really. But just in case, follow the new convention
#define REINIT_STACKADDR( ADDR, size ) \
   (ADDR.stack_size = size)

//------1---------2---------3---------4---------5---------6---------7---------8

#endif
