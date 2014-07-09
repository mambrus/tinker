/***************************************************************************
 *   Copyright (C) 2014 by Michael Ambrus                                  *
 *   michael.ambrus@gmail.com                                              *
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

/* ARCH=armv7l: ARMv7 VFPv3-D16 port, softfp ABI */
/*===============================================*/

#ifndef CONTEXT_GNU_ARMV7L_H
#define CONTEXT_GNU_ARMV7L_H

#define stalloc malloc
#define stalloc_free free

#if defined (__GNUC__)
#include <tinker/config.h>
#include <stdint.h>
#endif

#ifndef JUMPER_BASED
#define JUMPER_BASED 1
#endif

#if !TK_HOSTED
#define TK_CLI() asm __volatile__ (" CPSID if\n\t");
#define TK_STI() asm __volatile__ (" CPSIE if\n\t");
#else
#define TK_CLI()
#define TK_STI()
#endif

#if JUMPER_BASED
#include <setjmp.h>
//#define EXTRA_MARGIN (2*sizeof(uint64_t))
#define EXTRA_MARGIN 0

#define PUSHALL()		/*No need to PUSHALL on this arch - Already done by setjmp */
#define POPALL()		/*No need to POPALL on this arch - Already done by longjmp */

#define GET_SP(OUT_SP) \
   asm __volatile__ ( \
      "mov %0, %%r13 " \
      : "=r" (OUT_SP) \
      : \
      : "memory" \
   );

#define SET_SP(IN_SP) \
   asm __volatile__ ( \
      "mov %%r13, %0 " \
      : \
      : "r" (IN_SP) \
   );

#define PUSH_CPU_GETCUR_STACK( TSP1, TEMP )     \
   GET_SP( TSP1.sp )                            \
   TEMP = setjmp( TSP1.context );               \
   if (TEMP != (active_thread+1))               \
      GET_SP( TSP1.sp );

#define CHANGE_STACK_POP_CPU( TSP1, TEMP )      \
    longjmp( TSP1.context, active_thread+1);

#define CHANGE_STACK( TSP1, TEMP )              \
  SET_SP( TSP1 );

#else				//JUMPER_BASED
	#error Non jumper-based context-switch not implemented
#endif				//JUMPER_BASED
#endif
