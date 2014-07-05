/***************************************************************************
 *   Copyright (C) 2006, Michael Ambrus                                    *
 *   michael.ambrus@gmail.com                                              *
 *   Copyright (C) 2010,2014 by Michael Ambrus                             *
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
#ifndef TK_CONTEXT_H
#define TK_CONTEXT_H
/*
asm ( "statements" : output_registers : input_registers : clobbered_registers);

http://tisu.mit.jyu.fi/embedded/TIE345/luentokalvot/Embedded_3_ARM.pdf
http://www.arm.com/miscPDFs/9658.pdf
*/

#define stalloc malloc
#define stalloc_free free

#if defined (__GNUC__)
#include <tinker/config.h>
#include <stdint.h>
#endif

/* Default and fall-back as this is the most fool-proof way to get all
   context saved & restored */
#ifndef JUMPER_BASED
#define JUMPER_BASED 1
#endif

#if !TK_HOSTED
/*
	Need select ability on sub-arch for this to fully work (TBD)
	#define TK_CLI() asm __volatile__ (" CPSID aif\n\t");
	#define TK_STI() asm __volatile__ (" CPSIE aif\n\t");
*/
#define TK_CLI() asm __volatile__ (" CPSID if\n\t");
#define TK_STI() asm __volatile__ (" CPSIE if\n\t");
#else
#define TK_CLI()
#define TK_STI()
#endif

//------1---------2---------3---------4---------5---------6---------7---------8
#if JUMPER_BASED
//------1---------2---------3---------4---------5---------6---------7---------8
#include <setjmp.h>

#ifndef _JBLEN
#error JBLEN is expected but not known (undefined)
#endif

#define EXTRA_MARGIN (2*sizeof(uint64_t))

#define PUSHALL()		/*No need to PUSHALL on this arch - Already done by setjmp */
#define POPALL()		/*No need to POPALL on this arch - Already done by longjmp */

#define GET_SP( OUT_SP )                  \
   asm __volatile__ (                     \
      "mov %0, %%r13 "                    \
      : "=r" (OUT_SP)                     \
      : /**/                              \
      : "memory"                          \
   )

#define SET_SP( IN_SP )                   \
   asm __volatile__ (                     \
      "mov %%r13, %0 "                    \
      : /**/                              \
      : "r" (IN_SP)                       \
   )				/*Note, no clobber (intentional) */

#define PUSH_CPU_GETCUR_STACK( TSP1, TEMP )                                      \
   GET_SP( TSP1 );                                                               \
   TEMP = setjmp( (double*)(TSP1 - (_JBLEN*sizeof(double)) - EXTRA_MARGIN));     \
   if (TEMP != (active_thread+1))                                                \
      GET_SP( TSP1 );

#define CHANGE_STACK_POP_CPU( TSP1, TEMP )                                       \
	longjmp( (double*)(TSP1 - (_JBLEN*sizeof(double)) - EXTRA_MARGIN), active_thread+1);

#define CHANGE_STACK( TSP1, TEMP )                                               \
  SET_SP( TSP1 );

#define INIT_SP( _stack_SP, _stack_begin )                                       \
   _stack_SP.stack_size = _stack_begin.stack_size - EXTRA_MARGIN;                \
   _stack_SP.tstack = _stack_begin.tstack + _stack_begin.stack_size - EXTRA_MARGIN;

//Does nothing on this port
#define BIND_STACK( _stack_struct, _temp2 )

//Allready a char', no need to do handle in any special way.
#define STACK_PTR( ADDR )                 \
   (ADDR.tstack)

//Not needed to do anything really. But just in case, follow the new convention
#define REINIT_STACKADDR( ADDR, size )    \
   (ADDR.stack_size = size)

//------1---------2---------3---------4---------5---------6---------7---------8
#else				//Not JUMPER_BASED
//------1---------2---------3---------4---------5---------6---------7---------8
#define EXTRA_MARGIN 20

#define REAL_STACK_SIZE( TCB )            \
   ( TCB.stack_size )

/*
*/

#define PREP_TOS( _oldTOS, _newSP, _temp1, _temp2, _stack_struct )   \
   PUSHALL();                             \
   asm __volatile__ (                     \
      " mov  %0, %%r13\n\t"               \
      : "=r" (_temp1)                     \
      : /**/                              \
      : "memory"                          \
   );                                     \
   asm __volatile__ (                     \
      " mov  %%r13, %0\n\t"               \
      : /**/                              \
      : "r" (_oldTOS)                     \
   ); /*Note, no clobber (intentional)*/  \
                                          \
                                          \
   /*---> Compiler specific*/             \
                                          \
   asm __volatile__ (                     \
      "stmdb sp!, {sp}"                   \
   );                                     \
   asm __volatile__ (                     \
      "stmdb sp!, {r11, r12, sp}"         \
   );                                     \
                                          \
    /*<--- Compiler specific*/            \
                                          \
   PUSHALL();                             \
                                          \
   asm __volatile__ (                     \
      " mov  %0, %%r13\n\t"               \
      : "=r" (_newSP)                     \
      : /**/                              \
      : "memory"                          \
   );                                     \
   asm __volatile__ (                     \
      " mov  %%r13, %0\n\t"               \
      : /**/                              \
      : "r" (_temp1)                      \
   ); /*Note, no clobber (intentional)*/  \
   POPALL();

#define PUSH_CPU_GETCUR_STACK( TSP1, TEMP )  \
   PUSHALL();                             \
   asm __volatile__ (                     \
      "mov %0, %%r13 "                    \
      : "=r" (TSP1)                       \
      : /**/                              \
      : "memory"                          \
   )

#define CHANGE_STACK_POP_CPU( TSP1, TEMP )   \
   asm __volatile__ (                     \
      "mov %%r13, %0 "                    \
      : /**/                              \
      : "r" (TSP1)                        \
   );  /*Note, no clobber (intentional)*/ \
   POPALL();

//   asm __volatile__ (                  \
//      "add %r11, %r13, #0x14"            \
//   );  /*Note, no clobber (intentional)*/

#define CHANGE_STACK( TSP1, TEMP )        \
   asm __volatile__ (                     \
      "mov %%r13, %0 "                    \
      : /**/                              \
      : "r" (TSP1)                        \
   );				/*Note, no clobber (intentional) */

#define INIT_SP( _stack_SP, _stack_begin )\
   _stack_SP.stack_size = _stack_begin.stack_size - EXTRA_MARGIN; 					\
   _stack_SP.tstack = _stack_begin.tstack + _stack_begin.stack_size - EXTRA_MARGIN;

//Does nothing on this port
#define BIND_STACK( _stack_struct, _temp2 )

//TBD
#define GET_THREADS_RETVAL( THRETVAL, TEMP ) \
   asm __volatile__ (                     \
      "nop"                               \
   );

//Note: flags (CPSR) not saved. FIXME
#define PUSHALL()                         \
   asm __volatile__ (                     \
      "stmdb sp!, {r0-r12,lr}"            \
   );

//Note: flags (CPSR) not restored. FIXME
#define POPALL()                          \
   asm __volatile__ (                     \
      "ldmia sp!, {r0-r12,lr}"            \
   );

//Already a char', no need to do handle in any special way.
#define STACK_PTR( ADDR ) \
   (ADDR.tstack)

//Not needed to do anything really. But just in case, follow the new convention
#define REINIT_STACKADDR( ADDR, size ) \
   (ADDR.stack_size = size)

//------1---------2---------3---------4---------5---------6---------7---------8

//Non generic - don't use outside ARM specific code
#define GET_SP( param )                   \
   asm __volatile__ (                     \
      "mov %0, %%r13 "                    \
      : "=r" (param )                     \
      : "0" (param)                       \
      : "memory"                          \
   )

#define GET_LR( param )                   \
   asm __volatile__ (                     \
      "mov %0, %%r14 "                    \
      : "=r" (param )                     \
      : "0" (param)                       \
      : "memory"                          \
   )

#define GET_REG( theReg, param )          \
   asm __volatile__ (                     \
      "mov %0, %%"#theReg                 \
      : "=r" (param )                     \
      : "0" (param)                       \
      : "memory"                          \
   )

#define GET_PC( param )                   \
   asm __volatile__ (                     \
      "mov %0, %%r15\n\t "                \
      "sub %0, %0, #4 "                   \
      : "=r" (param )                     \
      : "0" (param)                       \
      : "memory"                          \
   )

#define aGET_PC( param )                   \
   __asm__( "mov %0, #1", "=r" (param))

//------1---------2---------3---------4---------5---------6---------7---------8
#endif				//JUMPER_BASED
//------1---------2---------3---------4---------5---------6---------7---------8
#endif
