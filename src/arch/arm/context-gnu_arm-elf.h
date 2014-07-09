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

#ifndef _JBLEN
#error JBLEN is expected but not known (undefined)
#endif

#define EXTRA_MARGIN (2*sizeof(uint64_t))

#define PUSHALL()
#define POPALL()

#define GET_SP(OUT_SP) \
   asm __volatile__ ( \
      "mov %0, %%r13 " \
      : "=r" (OUT_SP) \
      : \
      : "memory" \
   )

#define SET_SP(IN_SP) \
   asm __volatile__ ( \
      "mov %%r13, %0 " \
      : \
      : "r" (IN_SP) \
   )

#define PUSH_CPU_GETCUR_STACK(TSP1,TEMP) \
   GET_SP( TSP1 ); \
   TEMP = setjmp( (double*)(TSP1 - (_JBLEN*sizeof(double)) - EXTRA_MARGIN)); \
   if (TEMP != (active_thread+1)) \
      GET_SP( TSP1 );

#define CHANGE_STACK_POP_CPU(TSP1,TEMP) \
 longjmp( (double*)(TSP1 - (_JBLEN*sizeof(double)) - EXTRA_MARGIN), active_thread+1);

#define CHANGE_STACK(TSP1,TEMP) \
  SET_SP( TSP1 );

#define INIT_SP(_stack_SP,_stack) \
   _stack_SP.stack_size = _stack.stack_size - EXTRA_MARGIN; \
   _stack_SP.bos = _stack.bos + _stack.stack_size - EXTRA_MARGIN;

#define BIND_STACK(_stack_struct,_temp2)

#define STACK_PTR(ADDR) \
   (ADDR.bos)

#define REINIT_STACKADDR(ADDR,size) \
   (ADDR.stack_size = size)

#else

#define EXTRA_MARGIN 20

#define REAL_STACK_SIZE(TCB) \
   ( TCB.stack_size )

#define PREP_TOS(_oldTOS,_newSP,_temp1,_temp2,_stack_struct) \
   PUSHALL(); \
   asm __volatile__ ( \
      " mov  %0, %%r13\n\t" \
      : "=r" (_temp1) \
      : \
      : "memory" \
   ); \
   asm __volatile__ ( \
      " mov  %%r13, %0\n\t" \
      : \
      : "r" (_oldTOS) \
   ); \
                                          \
                                          \
                                          \
                                          \
   asm __volatile__ ( \
      "stmdb sp!, {sp}" \
   ); \
   asm __volatile__ ( \
      "stmdb sp!, {r11, r12, sp}" \
   ); \
                                          \
                                          \
                                          \
   PUSHALL(); \
                                          \
   asm __volatile__ ( \
      " mov  %0, %%r13\n\t" \
      : "=r" (_newSP) \
      : \
      : "memory" \
   ); \
   asm __volatile__ ( \
      " mov  %%r13, %0\n\t" \
      : \
      : "r" (_temp1) \
   ); \
   POPALL();

#define PUSH_CPU_GETCUR_STACK(TSP1,TEMP) \
   PUSHALL(); \
   asm __volatile__ ( \
      "mov %0, %%r13 " \
      : "=r" (TSP1) \
      : \
      : "memory" \
   )

#define CHANGE_STACK_POP_CPU(TSP1,TEMP) \
   asm __volatile__ ( \
      "mov %%r13, %0 " \
      : \
      : "r" (TSP1) \
   ); \
   POPALL();

#define CHANGE_STACK(TSP1,TEMP) \
   asm __volatile__ ( \
      "mov %%r13, %0 " \
      : \
      : "r" (TSP1) \
   );

#define INIT_SP(_stack_SP,_stack) \
   _stack_SP.stack_size = _stack.stack_size - EXTRA_MARGIN; \
   _stack_SP.bos = _stack.bos + _stack.stack_size - EXTRA_MARGIN;

#define BIND_STACK(_stack_struct,_temp2)

#define GET_THREADS_RETVAL(THRETVAL,TEMP) \
   asm __volatile__ ( \
      "nop" \
   );

#define PUSHALL() \
   asm __volatile__ ( \
      "stmdb sp!, {r0-r12,lr}" \
   );

#define POPALL() \
   asm __volatile__ ( \
      "ldmia sp!, {r0-r12,lr}" \
   );

#define STACK_PTR(ADDR) \
   (ADDR.bos)

#define REINIT_STACKADDR(ADDR,size) \
   (ADDR.stack_size = size)

#define GET_SP(param) \
   asm __volatile__ ( \
      "mov %0, %%r13 " \
      : "=r" (param ) \
      : "0" (param) \
      : "memory" \
   )

#define GET_LR(param) \
   asm __volatile__ ( \
      "mov %0, %%r14 " \
      : "=r" (param ) \
      : "0" (param) \
      : "memory" \
   )

#define GET_REG(theReg,param) \
   asm __volatile__ ( \
      "mov %0, %%"#theReg \
      : "=r" (param ) \
      : "0" (param) \
      : "memory" \
   )

#define GET_PC(param) \
   asm __volatile__ ( \
      "mov %0, %%r15\n\t " \
      "sub %0, %0, #4 " \
      : "=r" (param ) \
      : "0" (param) \
      : "memory" \
   )

#define aGET_PC(param) \
   __asm__( "mov %0, #1", "=r" (param))

#endif

#endif
