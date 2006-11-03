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

asm ( "statements" : output_registers : input_registers : clobbered_registers);

*/

#ifndef TK_HWSYS_GNU_i386_H
#define TK_HWSYS_GNU_i386_H

#define EXTRA_MARGIN 20                //<! Define SP this below the theoretical top (some compilers require it)

int bsp_printf(char *formatstr, ...);
/*!
How printk is implemented on this target. Alternate between the two followin. printf is prefered, but it sometimes doesnt work under Linux GDB
*/
//#define printk(x) bsp_printf x
#define printk(x) printf x

//The following variant is prefered for Linux while running inde GDB since GDB for linux works in buffered mode. 
//But this causes a compilation error if macro is used an unscopyed 'if' statement before the 'case' 

#define printk(x) { printf x; fflush(stdout); }

/*!
How initializing the BSP is done on this target (Only needed for bare bone targets).
In this case, the kernal is intended to run under Linux and no special case has been handled
for bare-bone systems (since we have no port it yet/ever).
*/
#define tk_bsp_sysinit() ((void*)0)


/*!
@name Mapping stack allocation API for this target
*/
//@{
#define stalloc      malloc
#define stalloc_free free
//@}

/*
#define TK_CLI()                          \
   asm __volatile__ (" CLI ");

#define TK_STI()                          \
   asm __volatile__ (" STI ");
*/   

#define TK_CLI()
#define TK_STI()


   
#define REAL_STACK_SIZE( TCB )            \
   ( TCB.stack_size ) 
   

#define PREP_TOS( _oldTOS, _newSP, _temp1, _temp2, _stack_struct )   \
   PUSHALL();                             \
   asm __volatile__ (                     \
      " movl %%esp, %%eax\n\t"            \
      " movl %%ebx, %%esp"                \
      : "=a" (_temp1)                     \
      : "b" (_oldTOS)                     \
      : "memory"                          \
   );                                     \
                                          \
   /*---> Compiler specific*/             \
                                          \
   asm __volatile__ (                     \
      " push %ebp\n\t"                    \
      " push %ebx"                        \
   );                                     \
                                          \
    /*<--- Compiler specific*/            \
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


#define PUSH_CPU_GETCUR_STACK( TSP1, TEMP )  \
   PUSHALL();                             \
   asm __volatile__ (                     \
      " movl %%esp, %%eax"                \
      : "=a" (TSP1)                       \
      : /*No input reg*/                  \
      : "memory"                          \
   );                                     \
  
#define CHANGE_STACK_POP_CPU( TSP1, TEMP )   \
   asm __volatile__ (                     \
      " movl %%ebx, %%esp"                \
      : /*no output*/                     \
      : "b" (TSP1)                        \
   );  /*Note, no clobber (intentional)*/ \
   POPALL();

//function enters as a result of a ret instruction. EAX is passed
//as the return value. Not shure if it works on every processor

#define CHANGE_STACK( TSP1, TEMP )        \
   asm __volatile__ (                     \
      " movl %%ebx, %%esp"                \
      : /*no output*/                     \
      : "b" (TSP1)                        \
   );  /*Note, no clobber (intentional)*/ 


#define INIT_SP( _stack_SP, _stack_begin )\
   _stack_SP.stack_size = _stack_begin.stack_size - EXTRA_MARGIN; 					\
   _stack_SP.tstack = _stack_begin.tstack + _stack_begin.stack_size - EXTRA_MARGIN;  \

//Does nothing on this port
#define BIND_STACK( _stack_struct, _temp2 )     

#define GET_THREADS_RETVAL( THRETVAL, TEMP )  \
   asm __volatile__ (                     \
      "nop\n\t"                           \
      : "=a" (THRETVAL)                   \
      : /*No imput reg*/                  \
      : "memory"                          \
      );

#define PUSHALL()                         \
   asm __volatile__ (                     \
      " pushf\n\t"                        \
      " pusha"                            \
   );

#define POPALL()                          \
   asm __volatile__ (                     \
      " popa\n\t"                         \
      " popf"                             \
   )


//Allready a char', no need to do handle in any special way.
#define STACK_PTR( ADDR ) \
   (ADDR.tstack)

//Not needed to do anything really. But just in case, follow the new convention 
#define REINIT_STACKADDR( ADDR, size ) \
   (ADDR.stack_size = size)

//Just a stub ATM - TBD
#define TRAP( NUM )                                                           \
   exit( NUM )   

#endif


