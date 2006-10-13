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

//App & system
#define TK_MINIMUM_STACK_SIZE 	0x8000
#define TK_NORMAL_STACK_SIZE  	0x8000

//SCHED 
#define TK_MAX_THREADS          10  
#define TK_MAX_PRIO_LEVELS      4
#define TK_MAX_THREADS_AT_PRIO  3
#define TK_THREAD_NAME_LEN      0x17

//ITC
#define MAX_BLOCKED_ON_Q    	TK_MAX_THREADS
#define MAX_NUM_Q       		10

/*
#define TK_HOWTO_MALLOC 		TK_FNK_ORIGINAL
#define TK_HOWTO_CLOCK  		TK_FNK_STUBBED
#define TK_HOWTO_PRINTK 		TK_FNK_VOIDED
#define TK_HOWTO_ASSERT 		TK_FNK_RENAMED      
*/  

#define TK_HOWTO_MALLOC 		TK_FNK_ORIGINAL
//#define TK_HOWTO_CLOCK  		TK_FNK_STUBBED
#define TK_HOWTO_CLOCK  		TK_FNK_ORIGINAL
#define TK_HOWTO_PRINTK 		TK_FNK_RENAMED
#define TK_HOWTO_ASSERT 		TK_FNK_RENAMED      


#endif  //TK_TUNING_H

  
/*! 
 * @addgroup CVSLOG_tk_tuning_h tk_tuning_h
 * @ingroup CVSLOG
 *  $Log: tk_tuning.h,v $
 *  Revision 1.2  2006-10-13 21:47:11  ambrmi09
 *  2 new CPU ports added: PowerPC and Blackfin. Also testing a new a new concept
 *  for thread creation that potentionally has big transparency advantages (needs
 *  to be proven in preemtive cases).
 *
 *  * PowerPC tested against simulator
 *  * Blackfin only compile and link (gcc and Newlib needes adaptation/porting -
 *    GDB and sinulator still missing)
 *
 *  Revision 1.1  2006/10/09 17:08:06  ambrmi09
 *  * Sceleton for bfin and powerpc now compiles.
 *  * Refingnemen in auto-tools to build correct BSP depending on toolchain and settings
 *  * BFIN,bfin macro work-around
 *
 *  Revision 1.3  2006/09/13 18:29:30  ambrmi09
 *  Commited needed in repocitory
 *
 *  Revision 1.2  2006/04/08 10:15:55  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.1.2.4  2006/04/06 09:01:55  ambrmi09
 *  Safety commit due to change of local sandbox FS type (had files checked out
 *  while changing the type - not to be recommended).
 *
 *  Revision 1.1.2.3  2006/04/03 20:07:22  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.1.2.2  2006/04/03 15:21:46  ambrmi09
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










