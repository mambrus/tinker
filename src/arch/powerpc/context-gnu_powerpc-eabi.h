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

This file is for the PowerPC architecture. Tested with several ABI for
Newlib glibc.

It implements setjmp and longjmp techniques which has some considerations
compared to traditional TinKer context switching:

* Both methods manipulate the CPU stack-pointer. We need to be
  certain that a thread comes out, it actually does so with the correct
  stack with it. Balancing way in and way and out is crucial. For the
  context switcher this is not a problem, but when creating a new thread
  this becomes a little tricky, as the function has to be doing a
  context-swich exactly half way, and all the steps has to be identical AND
  any optimizations between the two functions has to be identical.

* Some extra overhead while creating a thread, which will also actually run for a
  very short time (needed so it can create it's own scope)

* We're forced to rely on an external implementation in a very critical part of 
  the kernel.

TinKers original PUSH/POP based technique share a lot of similarities with
setjmp/longjmp, but they are not quite the same (see considerations above).
However, by using setjmp/longjump we will gain the following benefits and
drawbacks:

+ TinKer becomes even more portable and even less code needs to be tailored
  for a certain architecture (In this implementation there are actually only
  2 lines of assembly code).

+ We need not to bother with certain special handling due to different 
  GCC -m options or certain CPU variants limitations or issues. We'll let
  the tool-chains libc implementation handle those instead (great!)

+ Saving/restoring context can potentially be much faster. System knows what
  it needs to save, what not and it what order. Shadow registers/banking and
  DMA transfer are all theoretically possible assists. This would could play
  a very positive role in LKM/VM/hosted executions.

? Context does not have to be on TOS. It's a convenient place to put it as
  everything gets unlocked by the same key: the SP. But one drawback is that
  we get dependent on knowing the size of the sigjmp_buf. Availability of
  _JBLEN used in this solution is apparently not standard, just very common.

  An alternative could be to use curr_sp stack_t differently as. As it is
  our type, and as buth this type and the context intricate as are paired we
  could use BOS for the context. Stack will meet from two ends so to speak.
  What's essential is that it is still one key opening both. Similarly
  stack_t could have a context-buffer completely separated from the threads
  stack.

- Where HW/SIC/System is not so great (i.e. for deeply embedded targets) we
  might end up context-switching more than needed, and almost certainly in a
  non HW-accelerated way.

@note Tinker is from this implementation on adapted to handle both
techniques.

@attention Consider this implementation highly EXPERIMENTAL

@note In-line assembly syntax: 
asm ( "statements" : output_registers : input_registers : clobbered_registers);

*/

#ifndef TK_HWSYS_GNU_POWERPC_H
#define TK_HWSYS_GNU_POWERPC_H

#include <setjmp.h>

#define EXTRA_MARGIN 20   /* Define SP this below the theoretical top (some
                             compilers require it) */
#define JUMPER_BASED

/*!
@name Mapping stack allocation API for this target
*/
//@{
#define stalloc      malloc
#define stalloc_free free
//@}


#define MAGIC_NUMBER 7327

#define TK_CLI()  /*TBD FIXME*/
#define TK_STI()  /*TBD FIXME*/

#define REAL_STACK_SIZE( TCB )            \
   ( TCB.stack_size ) 

#define PUSHALL()	/*No need to PUSHALL on this target- Allready done by setjmp*/
#define POPALL()	/*No need to POPALL on this target- Allready done by longjmp*/

#define GET_SP( OUT_SP )					\
   asm __volatile__ (						\
      "stw %%sp,%[mystack]"					\
      : [mystack] "=m" (OUT_SP)					\
      : /**/							\
      : "memory"						\
   );
   
#define SET_SP( IN_SP )						\
   asm __volatile__ (						\
      "lwz %%sp,%[mystack]"					\
      : /**/							\
      : [mystack] "m" (IN_SP)					\
   );  /*Note, no clobber (intentional)*/

#define PUSH_CPU_GETCUR_STACK( TSP1, TEMP )			\
   GET_SP( TSP1 )						\
   TEMP = setjmp( (double*)(TSP1 - (_JBLEN*sizeof(double)) - EXTRA_MARGIN));              \
   if (TEMP != (active_thread+1))				\
      GET_SP( TSP1 )

#define CHANGE_STACK_POP_CPU( TSP1, TEMP )		\
	longjmp( (double*)(TSP1 - (_JBLEN*sizeof(double)) - EXTRA_MARGIN), active_thread+1);


#define CHANGE_STACK( TSP1, TEMP )        		\
  SET_SP( TSP1 )

#define INIT_SP( _stack_SP, _stack_begin )						\
   _stack_SP.stack_size = _stack_begin.stack_size - EXTRA_MARGIN; 			\
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


