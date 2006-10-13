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

http://tisu.mit.jyu.fi/embedded/TIE345/luentokalvot/Embedded_3_ARM.pdf
http://www.arm.com/miscPDFs/9658.pdf

*/

#ifndef TK_HWSYS_GNU_ARM_H
#define TK_HWSYS_GNU_ARM_H

#define EXTRA_MARGIN 20                //<! Define SP this below the theoretical top (some compilers require it)

/*!
How printk is implemented on this target. I.e. no ability to output on console

@note macro should digest <b>all</b> arguments.
*/
#if (TK_HOWTO_PRINTK == TK_FNK_VOIDED)
#   define printk(x) ((void)0)
#elif (TK_HOWTO_PRINTK == TK_FNK_RENAMED)
#   define eprintf(...) fprintf (stdout, __VA_ARGS__)
#   define printk(x) eprintf x
//#   define printk(x) printf x
#else
#   error Can't handle requested option for printk
#endif

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
   );  /*Note, no clobber (intentional)*/ \


#define INIT_SP( _stack_SP, _stack_begin )\
   _stack_SP.stack_size = _stack_begin.stack_size - EXTRA_MARGIN; 					\
   _stack_SP.tstack = _stack_begin.tstack + _stack_begin.stack_size - EXTRA_MARGIN;  \

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

//Allready a char', no need to do handle in any special way.
#define STACK_PTR( ADDR ) \
   (ADDR.tstack)

//Not needed to do anything really. But just in case, follow the new convention 
#define REINIT_STACKADDR( ADDR, size ) \
   (ADDR.stack_size = size)

//Just a stub ATM - TBD
#define TRAP( NUM )                                                           \
   exit( NUM )   

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


#endif


