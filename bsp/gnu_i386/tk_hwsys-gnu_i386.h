/***************************************************************************
 *   Copyright (C) 2006 by Michale Ambrus                                  *
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


#define GET_THREADS_RETVAL( THRETVAL )    \
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
   ((char *)ADDR.tstack)

//Not needed to do anything really. But just in case, follow the new convention 
#define REINIT_STACKADDR( ADDR, size ) \
   (ADDR.stack_size = size)

//Just a stub ATM - TBD
#define TRAP( NUM )                                                           \
   exit( NUM )   

#endif


