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

#define BOARD BITFIRE 

#if !defined(BOARD)
#error BOARD needs to be defined
#endif


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
                           
                           

/*!
 * @defgroup CVSLOG_tk_bsp_h tk_bsp_h
 * @ingroup CVSLOG
 *  $Log: tk_bsp.h,v $
 *  Revision 1.2  2006-04-08 10:15:55  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.1.2.1  2006/04/07 12:10:06  ambrmi09
 *  Skeleton for handling syscalls using the ARM Angel interface in place
 *
 *  Basic terminal I/O for gnu_arm (LPC2129) - only output so far (input requires
 *  blocking).
 *
 */
 
 
 