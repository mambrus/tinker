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
  

#include "tk_hwsys_keilC166.h"
#include <tk_itc.h>

/*

@ingroup kernel_internal_BSP

@brief Reinitializes the stack.

On XC167 (and other C166 derivate using Keil as compiler) there are always
<b>two</b> stacks. Conceptually TinKer knows only of one, and this function
helps to create the two stacks within the same TinKer stack, i.e. within
the same memory area (usually allocated from heap with malloc).

Below are some notes for where the stacks are deducted to be.

ratio             :=  user_stack_size/system_stack_size
whole_stack_size  :=  (system_stack_size + user_stack_size)

=>

user_stack_size   := ratio * system_stack_size
system_stack_size := whole_stack_size - user_stack_size

=>

user_stack_size   := ratio * (whole_stack_size - user_stack_size)
user_stack_size   := ratio * whole_stack_size - ratio * user_stack_size


user_stack_size + ratio * user_stack_size := ratio * whole_stack_size
user_stack_size * ( 1+ ratio )            := ratio * whole_stack_size

user_stack_size                           := ratio * whole_stack_size / ( 1+ ratio )

@note According to the XC167 manual: <i>Only word accesses are supported to 
the system stack.</i> Therefor we need the word alignement as you se in the 
code.

*/

void _tk_reinit_stackaddr_xc167keil( stack_t *addr, size_t size );
void _tk_reinit_stackaddr_xc167keil( 
   stack_t *addr, 
   size_t size 
){
   size_t usr_stack_size;
   size_t sys_stack_size;
   
   usr_stack_size = size * USR_SYS_RATIO/(1+USR_SYS_RATIO);
   sys_stack_size = size - usr_stack_size;

   //make sure we have 32 bit alignement (sys stack will contain return adresses)
   for (;sys_stack_size % 4;sys_stack_size++);
   
   usr_stack_size = size - sys_stack_size;

   
   addr->usr_stack_size = usr_stack_size;
   addr->sys_stack_size = sys_stack_size;
   
   addr->userstack.linear = addr->systemstack.linear + sys_stack_size;                      //Need also adjust in TOS
   //addr->userstack.linear = addr->systemstack.linear + sys_stack_size + usr_stack_size;   //good as temp solution
   //addr->userstack.linear = addr->systemstack.linear + usr_stack_size;                    //old
}


void _do_trap (unsigned int num){
   __asm { 
      mov R3, num   
      trap #0x0D
   }                                                      
}


  

/*! 
 * @defgroup CVSLOG_tk_hwsys_keilC166_c tk_hwsys_keilC166_c
 * @ingroup CVSLOG
 *
 *  $Log: context_keilC166.c,v $
 *  Revision 1.1  2006-11-03 11:33:30  ambrmi09
 *  Moved architecture specific files out from bsp structure and into kernel
 *  source 'arch' structure. This makes sense since I plan to refine the
 *  project into kernel specific maintenence on one hand, and BSP (and
 *  eventually other's) on the other.
 *
 *  BSP's as part of the *kernlel* project will only be in the form of crude
 *  template BSP (community will take care of the rest).
 *
 *  Revision 1.12  2006/04/08 10:15:51  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.11.2.1  2006/04/03 20:07:21  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.11  2006/03/12 15:08:53  ambrmi09
 *  - Adjusted the source to accomodate the new file structure.
 *
 *  - All build environments uppdated and verified except BC5. For this one
 *  we stumbled across the header-file issue that I've been fearing. Seems
 *  we need to take care of that one after all.
 *
 *  @note The malloc bug still not solved.
 *
 *  Revision 1.10  2006/03/05 11:11:23  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.9  2006/02/22 13:05:45  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.8  2005/12/03 14:04:30  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *
 *******************************************************************/
