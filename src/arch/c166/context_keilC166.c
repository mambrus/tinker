/***************************************************************************
 *   Copyright (C) 2005,2006 by Michael Ambrus                             *
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
the system stack.</i> Therefor we need the word alignment as you see in the
code.

*/

void _tk_reinit_stackaddr_xc167keil(stack_t * addr, size_t size);
void _tk_reinit_stackaddr_xc167keil(stack_t * addr, size_t size)
{
	size_t usr_stack_size;
	size_t sys_stack_size;

	usr_stack_size = size * USR_SYS_RATIO / (1 + USR_SYS_RATIO);
	sys_stack_size = size - usr_stack_size;

	//make sure we have 32 bit alignment (sys stack will contain return addresses)
	for (; sys_stack_size % 4; sys_stack_size++) ;

	usr_stack_size = size - sys_stack_size;

	addr->usr_stack_size = usr_stack_size;
	addr->sys_stack_size = sys_stack_size;

	addr->userstack.linear = addr->systemstack.linear + sys_stack_size;	//Need also adjust in TOS
	//addr->userstack.linear = addr->systemstack.linear + sys_stack_size + usr_stack_size;   //good as temp solution
	//addr->userstack.linear = addr->systemstack.linear + usr_stack_size;                    //old
}

void _do_trap(unsigned int num)
{
	__asm {
		mov R3, num trap
#0x0D
}}
