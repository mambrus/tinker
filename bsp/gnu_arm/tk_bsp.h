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
@brief Definitions of board sys-calls

Definitions of the board sys-calls. Redefine each for a certain board+uC
combo.

@note use the definitions as a template for your own fnkn mappings
*/

#ifndef TK_BSP_H
#define TK_BSP_H

#include <tinker/config.h>

#if !defined(BOARD)
   #error BOARD needs to be defined
#endif

#if !defined(TK_SYSTEM)
   #error System needs to be defined
#endif


// Board ID definitions
#define BITFIRE            0
#define lpc21xx            BITFIRE


#if (BOARD == BITFIRE)
#define bsp_Open            BF_Open
#define bsp_Close           BF_Close
#define bsp_WriteC          BF_WriteC
#define bsp_Write0          BF_Write0
#define bsp_Write           BF_Write
#define bsp_Read            BF_Read
#define bsp_ReadC           BF_ReadC
#define bsp_IsTTY           BF_IsTTY
#define bsp_Seek            BF_Seek
#define bsp_FLen            BF_FLen
#define bsp_TmpNam          BF_TmpNam
#define bsp_Remove          BF_Remove
#define bsp_Rename          BF_Rename
#define bsp_Clock           BF_Clock
#define bsp_Time            BF_Time
#define bsp_System          BF_System
#define bsp_Errno           BF_Errno
#define bsp_GetCmdLine      BF_GetCmdLine
#define bsp_HeapInfo        BF_HeapInfo
#define bsp_EnterSVC        BF_EnterSVC
#define bsp_ReportException BF_ReportException
//#define tk_bsp_sysinit      TK_BF_sysinit
#endif

int bsp_Open            (/*TBD*/);
int bsp_Close           (/*TBD*/);
int bsp_WriteC          (/*TBD*/);
int bsp_Write0          (/*TBD*/);
int bsp_Write           (int fh, char *ptr,int len);
int bsp_Read            (/*TBD*/);
int bsp_ReadC           (/*TBD*/);
int bsp_IsTTY           (/*TBD*/);
int bsp_Seek            (/*TBD*/);
int bsp_FLen            (/*TBD*/);
int bsp_TmpNam          (/*TBD*/);
int bsp_Remove          (/*TBD*/);
int bsp_Rename          (/*TBD*/);
int bsp_Clock           (/*TBD*/);
int bsp_Time            (/*TBD*/);
int bsp_System          (/*TBD*/);
int bsp_Errno           (/*TBD*/);
int bsp_GetCmdLine      (/*TBD*/);
int bsp_HeapInfo        (/*TBD*/);
int bsp_EnterSVC        (/*TBD*/);
int bsp_ReportException (/*TBD*/);
//int tk_bsp_sysinit      (void);

#endif  //TK_BSP_H

/*!
 * @defgroup CVSLOG_tk_bsp_h tk_bsp_h
 * @ingroup CVSLOG
 *  $Log: tk_bsp.h,v $
 *  Revision 1.6  2007-02-24 12:17:14  ambrmi09
 *  1) Sync PowerPC sources
 *  2) Add structure for i386-hixs-elf target. (Hopefully our new test-bed)
 *     for developping the filesys component.)
 *
 *  Revision 1.5  2006/11/05 14:18:56  ambrmi09
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
 *  Revision 1.4  2006/09/28 17:42:44  ambrmi09
 *  HIXS system integration for RM done. ARM now has two different system call API supported. This is mostly interesting from a transparency point of view, but also a good exersisze for bfin and ppc, which are not ported yet (since I'm not planning on implementing any other system integrations than HIXS from now on).
 *
 *  Revision 1.3  2006/09/13 18:29:30  ambrmi09
 *  Commited needed in repocitory
 *
 *  Revision 1.2  2006/04/08 10:15:55  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.1.2.1  2006/04/07 12:10:06  ambrmi09
 *  Skeleton for handling syscalls using the ARM Angel interface in place
 *
 *  Basic terminal I/O for gnu_arm (LPC2129) - only output so far (input requires
 *  blocking).
 *
 */

