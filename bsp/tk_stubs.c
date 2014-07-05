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
@brief stubed funtions

This file contains stub-functions . Use these funtions iniially in your port
until you got working equivalents.

When involved with porting, the \ref clock, \ref printf and \ref malloc
functions are most often not implemented or wrong.

The functions in this file are an esential part of the \ref SCHED internals,
you need to have atleas stub that will do something  meantingful, so that
the dispaching can work.

*/

#include <time.h>

#if (TK_HOWTO_CLOCK == TK_FNK_STUBBED)
long int stub_sub_mickey = 0;
long int stub_mickey = 0;
/*!
Simulates time by increasing a variable everytime function is called.
 */
clock_t clock_stubbed(){
   stub_sub_mickey++;
   if (stub_sub_mickey >=100){
      stub_sub_mickey = 0;
      stub_mickey++;
   }
   return (clock_t)stub_mickey;
}
#endif


/*!
 * @defgroup CVSLOG_tk_stubs_c tk_stubs_c
 * @ingroup CVSLOG
 *
 *  $Log: tk_stubs.c,v $
 *  Revision 1.4  2006-10-09 17:08:05  ambrmi09
 *  * Sceleton for bfin and powerpc now compiles.
 *  * Refingnemen in auto-tools to build correct BSP depending on toolchain and settings
 *  * BFIN,bfin macro work-around
 *
 *  Revision 1.3  2006/09/13 18:29:30  ambrmi09
 *  Commited needed in repocitory
 *
 *  Revision 1.2  2006/04/08 10:15:50  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.1.2.4  2006/04/07 12:10:05  ambrmi09
 *  Skeleton for handling syscalls using the ARM Angel interface in place
 *
 *  Basic terminal I/O for gnu_arm (LPC2129) - only output so far (input requires
 *  blocking).
 *
 *  Revision 1.1.2.3  2006/04/06 09:01:55  ambrmi09
 *  Safety commit due to change of local sandbox FS type (had files checked out
 *  while changing the type - not to be recommended).
 *
 *  Revision 1.1.2.2  2006/04/03 20:07:20  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.1.2.1  2006/04/03 15:21:43  ambrmi09
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
 */



