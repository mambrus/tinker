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
  

//------1---------2---------3---------4---------5---------6---------7---------8
#ifndef TK_TUNING_H
#define TK_TUNING_H

#define SAFEZONE        0x40
/*!
Defines where the userstack is within the mallocated area. The system
stack will be on highest addresand the user stack will be below that.

Read ratio as x:y or user_size/system_size
*/
#define USR_SYS_RATIO 2

//Note that system stack has to fit even after usr_stack has been subtracted
//With a rato of 2 this will give a stack size for sys_stack of 64

#define minimum_system_stack_size  0xC0
//#define TK_MINIMUM_STACK_SIZE ((minimum_system_stack_size + 1)*USR_SYS_RATIO) + SAFEZONE)

#define TK_MINIMUM_STACK_SIZE 0x0300  //!< TBD this @todo TBD this
#define TK_NORMAL_STACK_SIZE  0x0600  

//SCHED 
#define TK_MAX_THREADS          50
#define TK_MAX_PRIO_LEVELS      0x10
#define TK_MAX_THREADS_AT_PRIO  TK_MAX_THREADS
#define TK_THREAD_NAME_LEN      0x17

//ITC
#define TK_MAX_BLOCKED_ON_Q    	10
#define TK_MAX_NUM_Q       		75



#endif  //TK_TUNING_H

  
/*! 
 * @addgroup CVSLOG_tk_tuning_h tk_tuning_h
 * @ingroup CVSLOG
 *  $Log: tk_tuning.h,v $
 *  Revision 1.3  2006-11-05 14:18:56  ambrmi09
 *  Build system and source modified to make better use of config.h
 *
 *  This file now contains information about how the kernel is configured
 *  and can be used by both application and kernel build (old solution only
 *  let kernel-buils know of these details).
 *
 *  This applies to both tk_tuning, component configuration among others.
 *  Use './configure --help' to see a full list. Note that  if a certain
 *  feature is not configured, the old tk_tuning will fill in the gaps.
 *  This is especially usefull when not using GNU build- and configure-
 *  tool-chain. Hopefully, we'll be able to get rid of tk_tuning.h in the
 *  future.
 *
 *  Revision 1.2  2006/04/08 10:15:51  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.1.2.3  2006/04/03 20:07:21  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.1.2.2  2006/04/03 15:21:45  ambrmi09
 *  All targets updated with the new thread-starter (alternative 2).
 *
 *  This alternative has one weakness (explained elsewhere togeather
 *  with alternative 1), but so far it's the only one that will compile
 *  and function equally among all 4 (very different) compilers currently
 *  tested against: GCC, MSVC, BC5 and Keil.
 *
 *  If nothing else turns up, I'm willing to overcome the drawback (it's
 *  quite handleable) because it *truly* takes away a lot of pain with
 *  porting.
 *
 *  The ARM port (architecture level) is than's to this now fully operational
 *  without the r13 hack in the context switch. This includes thread
 *  cancellation and thread argument passing (which were not functioning in
 *  the old port).
 *
 *  If this revised code proves itself (i.e. no surprises turns up) then
 *  TinKer can be considered "almost ported" to any HW target that GCC is
 *  ported for :D (/cheers)
 *
 *  Revision 1.1.2.1  2006/03/31 17:42:55  ambrmi09
 *  Second idea for the new thread starter. This one plays nice with several
 *  more compilers beacuse of it's balances call-stack. It's not as
 *  beautiful as the former one IMO, but GNU is a pain in the but
 *  with it's call-stack optimizations (and decorations doesn't seem to work
 *  for Cygwin GCC ).
 *
 *
 *******************************************************************/






