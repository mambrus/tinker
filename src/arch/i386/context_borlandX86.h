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
#ifndef TK_HWSYS_BORLANDX86_H
#define TK_HWSYS_BORLANDX86_H

#define printk(x) printf x

#define tk_bsp_sysinit() ((void*)0)

#define stalloc malloc
#define stalloc_free free
#define TK_CLI()
#define TK_STI()

#define REAL_STACK_SIZE(TCB) \
   ( TCB.stack_size )

#define PUSH_CPU_GETCUR_STACK(TSP1,TEMP) \
    __asm{ pushfd } \
    __asm{ pushad } \
    __asm{ mov TSP1,esp }

#define CHANGE_STACK_POP_CPU(TSP1,TEMP) \
    __asm{ mov esp,TSP1 } \
    __asm{ popad } \
    __asm{ popfd }

#define CHANGE_STACK(TSP1,TEMP) \
    __asm{ mov esp,TSP1 }

#define INIT_SP(_stack_SP,_stack_begin) \
   _stack_SP.stack_size = _stack_begin.stack_size; \
   _stack_SP.tstack = _stack_begin.tstack + _stack_begin.stack_size;

#define BIND_STACK(_stack_struct,_temp2)

#define GET_THREADS_RETVAL(THRETVAL,TEMP) \
   __asm{ mov THRETVAL,EAX }

#define PUSHALL() \
    __asm{ pushfd } \
    __asm{ pushad }

#define POPALL() \
    __asm{ popad } \
    __asm{ popfd }

#define STACK_PTR(ADDR) \
   ((char *)ADDR.tstack)

#define REINIT_STACKADDR(ADDR,size) \
   (ADDR.stack_size = size)

#define TRAP(NUM) \
   exit( NUM )

#endif
