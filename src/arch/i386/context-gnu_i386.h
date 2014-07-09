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
#ifndef TK_HWSYS_GNU_i386_H
#define TK_HWSYS_GNU_i386_H

#include <tinker/config.h>

#define EXTRA_MARGIN 20

#define stalloc malloc
#define stalloc_free free
#define TK_CLI()
#define TK_STI()

#define REAL_STACK_SIZE(TCB) \
   ( TCB.stack_size )

#define PREP_TOS(_oldTOS,_newSP,_temp1,_temp2,_stack_struct) \
   PUSHALL(); \
   asm __volatile__ ( \
      " movl %%esp, %%eax\n\t" \
      " movl %%ebx, %%esp" \
      : "=a" (_temp1) \
      : "b" (_oldTOS) \
      : "memory" \
   ); \
                                          \
                                          \
                                          \
   asm __volatile__ ( \
      " push %ebp\n\t" \
      " push %ebx" \
   ); \
                                          \
                                          \
                                          \
   PUSHALL(); \
                                          \
   asm __volatile__ ( \
      " movl %%esp, %%eax\n\t" \
      " movl %%ebx, %%esp" \
      : "=a" (_newSP) \
      : "b" (_temp1) \
      : "memory" \
   ); \
   POPALL();

#define PUSH_CPU_GETCUR_STACK(TSP1,TEMP) \
   PUSHALL(); \
   asm __volatile__ ( \
      " movl %%esp, %%eax" \
      : "=a" (TSP1) \
      : \
      : "memory" \
   );

#define CHANGE_STACK_POP_CPU(TSP1,TEMP) \
   asm __volatile__ ( \
      " movl %%ebx, %%esp" \
      : \
      : "b" (TSP1) \
   ); \
   POPALL();

#define CHANGE_STACK(TSP1,TEMP) \
   asm __volatile__ ( \
      " movl %%ebx, %%esp" \
      : \
      : "b" (TSP1) \
   );

#define INIT_SP(_stack_SP,_stack) \
   _stack_SP.stack_size = _stack.stack_size - EXTRA_MARGIN; \
   _stack_SP.bos = _stack.bos + _stack.stack_size - EXTRA_MARGIN;

#define BIND_STACK(_stack_struct,_temp2)

#define GET_THREADS_RETVAL(THRETVAL,TEMP) \
   asm __volatile__ ( \
      "nop\n\t" \
      : "=a" (THRETVAL) \
      : \
      : "memory" \
      );

#define PUSHALL() \
   asm __volatile__ ( \
      " pushf\n\t" \
      " pusha" \
   );

#define POPALL() \
   asm __volatile__ ( \
      " popa\n\t" \
      " popf" \
   )

#define STACK_PTR(ADDR) \
   (ADDR.bos)

#define REINIT_STACKADDR(ADDR,size) \
   (ADDR.stack_size = size)

#endif
