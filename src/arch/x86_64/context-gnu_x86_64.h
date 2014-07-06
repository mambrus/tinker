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
#define TK_CLI() asm __volatile__ (" CLI ");
#define TK_STI() asm __volatile__ (" STI ");
#else
#define TK_CLI()
#define TK_STI()
#endif

//------1---------2---------3---------4---------5---------6---------7---------8
#if JUMPER_BASED
//------1---------2---------3---------4---------5---------6---------7---------8
#include <setjmp.h>

#ifndef _JBLEN
//#define _JBLEN (16*sizeof(uint64_t))
//#warning JBLEN is not known, just guessing...(TBD)
#define _JBLEN 2048
#endif

#define EXTRA_MARGIN (2*sizeof(uint64_t))

#define REAL_STACK_SIZE( TCB )            \
   ( TCB.stack_size )

#define PUSHALL()		/*No need to PUSHALL on this arch - Already done by setjmp */
#define POPALL()		/*No need to POPALL on this arch - Already done by longjmp */

#define GET_SP( OUT_SP )                  \
   asm __volatile__ (                     \
      "mov %%rsp,%[mystack]"              \
      : [mystack] "=m" (OUT_SP)           \
      : /**/                              \
      : "memory"                          \
   );

#define SET_SP( IN_SP )                   \
   asm __volatile__ (                     \
      "mov %[mystack],%%rsp"              \
      : /**/                              \
      : [mystack] "m" (IN_SP)             \
   );				/*Note, no clobber (intentional) */

#define PUSH_CPU_GETCUR_STACK( TSP1, TEMP )     \
   GET_SP( TSP1 )                               \
   TEMP = setjmp( (void*)(TSP1 - _JBLEN - EXTRA_MARGIN));              \
   if (TEMP != (active_thread+1))               \
      GET_SP( TSP1 )

#define CHANGE_STACK_POP_CPU( TSP1, TEMP )      \
    longjmp( (void*)(TSP1 - _JBLEN - EXTRA_MARGIN), active_thread+1);

#define CHANGE_STACK( TSP1, TEMP )              \
  SET_SP( TSP1 )

#define INIT_SP( _stack_SP, _stack_begin )                      \
   _stack_SP.stack_size = _stack_begin.stack_size - EXTRA_MARGIN;           \
   _stack_SP.tstack = _stack_begin.tstack + _stack_begin.stack_size - EXTRA_MARGIN;

//Does nothing on this port
#define BIND_STACK( _stack_struct, _temp2 )

//Already a char', no need to do handle in any special way.
#define STACK_PTR( ADDR ) \
   (ADDR.tstack)

//Not needed to do anything really. But just in case, follow the new convention
#define REINIT_STACKADDR( ADDR, size ) \
   (ADDR.stack_size = size)

//------1---------2---------3---------4---------5---------6---------7---------8
#else				//Not JUMPER_BASED
//------1---------2---------3---------4---------5---------6---------7---------8
#error Non jumper-based context-switch is TBD for this architecture
/* Below is just templated from i386 (not enough) */

#define REAL_STACK_SIZE(TCB) \
   ( TCB.stack_size )

#define PREP_TOS(_oldTOS,_newSP,_temp1,_temp2,_stack_struct) \
   PUSHALL(); \
   asm __volatile__ (                     \
      " movl %%esp, %%eax\n\t"            \
      " movl %%ebx, %%esp"                \
      : "=a" (_temp1)                     \
      : "b" (_oldTOS)                     \
      : "memory"                          \
   );                                     \
                                          \
                                          \
                                          \
   asm __volatile__ (                     \
      " push %ebp\n\t"                    \
      " push %ebx"                        \
   );                                     \
                                          \
                                          \
                                          \
   PUSHALL();                             \
                                          \
   asm __volatile__ (                     \
      " movl %%esp, %%eax\n\t"            \
      " movl %%ebx, %%esp"                \
      : "=a" (_newSP)                     \
      : "b" (_temp1)                      \
      : "memory"                          \
   );                                     \
   POPALL();

#define PUSH_CPU_GETCUR_STACK(TSP1,TEMP)  \
   PUSHALL();                             \
   asm __volatile__ (                     \
      " movl %%esp, %%eax"                \
      : "=a" (TSP1)                       \
      :                                   \
      : "memory"                          \
   );

#define CHANGE_STACK_POP_CPU(TSP1,TEMP)   \
   asm __volatile__ (                     \
      " movl %%ebx, %%esp"                \
      :                                   \
      : "b" (TSP1)                        \
   );                                     \
   POPALL();

#define CHANGE_STACK(TSP1,TEMP)           \
   asm __volatile__ (                     \
      " movl %%ebx, %%esp"                \
      :                                   \
      : "b" (TSP1)                        \
   );

#define INIT_SP(_stack_SP,_stack_begin)                           \
   _stack_SP.stack_size = _stack_begin.stack_size - EXTRA_MARGIN; \
   _stack_SP.tstack = _stack_begin.tstack + _stack_begin.stack_size - EXTRA_MARGIN;

#define BIND_STACK(_stack_struct,_temp2)

#define GET_THREADS_RETVAL(THRETVAL,TEMP) \
   asm __volatile__ (                     \
      "nop\n\t"                           \
      : "=a" (THRETVAL)                   \
      :                                   \
      : "memory"                          \
      );

#define MNEM_0(OP)                        \
   #OP" \n\t"

#define MNEM_1(OP,OPRND)                  \
   #OP" "#OPRND" \n\t"

#define PUSH(R)                           \
   MNEM_1( push, R )

#define POP(R)                            \
   MNEM_1( pop, R )

#define PUSHF                             \
    MNEM_0( pushf )

#define POPF                              \
    MNEM_0( popf )

#define PUSHALL()                         \
   asm __volatile__ (                     \
      PUSHF                               \
      PUSH( %rax )                        \
      PUSH( %rbx )                        \
      PUSH( %rcx )                        \
      PUSH( %rdx )                        \
      PUSH( %rbp )                        \
      PUSH( %rsp )                        \
      PUSH( %rsi )                        \
      PUSH( %rdi )                        \
      PUSH( %r8 )                         \
      PUSH( %r9 )                         \
      PUSH( %r10 )                        \
      PUSH( %r11 )                        \
      PUSH( %r12 )                        \
      PUSH( %r13 )                        \
      PUSH( %r14 )                        \
      PUSH( %r15 )                        \
   );

#define POPALL()                          \
   asm __volatile__ (                     \
      POP( %r15 )                         \
      POP( %r14 )                         \
      POP( %r13 )                         \
      POP( %r12 )                         \
      POP( %r11 )                         \
      POP( %r10 )                         \
      POP( %r9 )                          \
      POP( %r8 )                          \
      POP( %rdi )                         \
      POP( %rsi )                         \
      POP( %rsp )                         \
      POP( %rbp )                         \
      POP( %rdx )                         \
      POP( %rcx )                         \
      POP( %rbx )                         \
      POP( %rax )                         \
      POPF                                \
   )

#define STACK_PTR(ADDR)                   \
   (ADDR.tstack)

#define REINIT_STACKADDR(ADDR,size)       \
   (ADDR.stack_size = size)

//------1---------2---------3---------4---------5---------6---------7---------8
#endif				//JUMPER_BASED
//------1---------2---------3---------4---------5---------6---------7---------8
#endif
