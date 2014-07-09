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
#ifndef TK_HWSYS_KEILC166_H
#define TK_HWSYS_KEILC166_H
#pragma dynamicusrstk
#pragma saveusr
#pragma reentrant

#pragma NOFD

#include <stddef.h>

#define REGH(DEV) \
   <DEV.h>
#if defined( DEVICE )
#include REGH( DEVICE )
#else
#error "Error, No device specified - can't determine register definition file !!!!!!!!!!!"
#endif

#define printk(x) printf x

#include <../bsp/XC167_Keil/stalloc.h>
#define PUSHALL() \
   __asm{ push PSW } \
   __asm{ push R0 } \
   __asm{ push R1 } \
   __asm{ push R2 } \
   __asm{ push R3 } \
   __asm{ push R4 } \
   __asm{ push R5 } \
   __asm{ push R6 } \
   __asm{ push R7 } \
   __asm{ push R8 } \
   __asm{ push R9 } \
   __asm{ push R10 } \
   __asm{ push R11 } \
   __asm{ push R12 } \
   __asm{ push R13 } \
   __asm{ push R14 } \
   __asm{ push R15 } \
   __asm{ push DPP0 } \
   __asm{ push DPP1 } \
   __asm{ push DPP2 } \
   __asm{ push DPP3 } \
   __asm{ push MDL } \
   __asm{ push MDH } \
   __asm{ push MDC } \
   __asm{ push STKOV } \
   __asm{ push STKUN }

#define POPALL() \
   __asm{ pop STKUN } \
   __asm{ pop STKOV } \
   __asm{ pop MDC } \
   __asm{ pop MDH } \
   __asm{ pop MDL } \
   __asm{ pop DPP3 } \
   __asm{ pop DPP2 } \
   __asm{ pop DPP1 } \
   __asm{ pop DPP0 } \
   __asm{ pop R15 } \
   __asm{ pop R14 } \
   __asm{ pop R13 } \
   __asm{ pop R12 } \
   __asm{ pop R11 } \
   __asm{ pop R10 } \
   __asm{ pop R9 } \
   __asm{ pop R8 } \
   __asm{ pop R7 } \
   __asm{ pop R6 } \
   __asm{ pop R5 } \
   __asm{ pop R4 } \
   __asm{ pop R3 } \
   __asm{ pop R2 } \
   __asm{ pop R1 } \
   __asm{ pop R0 } \
   __asm{ pop PSW }

#define PUSH_CPU_GETCUR_STACK(TSP1,TEMP) \
   PUSHALL() \
   TEMP = 0ul; \
   __asm{ mov TEMP,SP } \
   TSP1 = ((unsigned long)SPSEG<<16) + (unsigned long)TEMP;

#define CHANGE_STACK_POP_CPU(TSP1,TEMP) \
   TEMP = (unsigned long)TSP1 >> 16; \
   __asm{ mov SP,TSP1 } \
   __asm{ mov SPSEG,TEMP } \
   POPALL();

#define CHANGE_STACK(TSP1,TEMP) \
   TEMP = (unsigned long)TSP1 >> 16; \
   __asm{ mov SP,TSP1 } \
   __asm{ mov SPSEG,TEMP }

#define INIT_SP(_stack_SP,_stack) \
   _stack_SP.usr_stack_size = _stack.usr_stack_size; \
   _stack_SP.userstack.linear = _stack.userstack.linear + _stack.usr_stack_size; \
   _stack_SP.sys_stack_size = _stack.sys_stack_size; \
   _stack_SP.systemstack.linear = _stack.systemstack.linear + _stack.sys_stack_size;

#define BIND_STACK(_stack_struct,_temp2) \
   _temp2 = _stack_struct.userstack.u.offs24._offs; \
   _temp2 = _temp2 - 4; \
   __asm{ mov R0,_temp2 } \
   _temp2 = _stack_struct.userstack.u.seg24._seg; \
   __asm{ mov DPP0,_temp2 } \
                                                                                                            \
   _temp2 = _stack_struct.systemstack.reg._SP + 4 ; \
   __asm{ mov STKOV,_temp2 } \
   _temp2 = _stack_struct.systemstack.reg._SP + _stack_struct.sys_stack_size; \
   __asm{ mov STKUN,_temp2 }

#define GET_THREADS_RETVAL(THRETVAL,TEMP) \
   __asm{ mov THRETVAL, R4 } \
   __asm{ mov TEMP, R5 } \
   THRETVAL = (TEMP<<16) + THRETVAL;

#define STACK_PTR(ADDR) \
   ((char *)ADDR.systemstack.linear)

extern unsigned long Q_ASC0;
void _tk_initialize_system_ques();

#define REINIT_STACKADDR(ADDR,size) \
   _tk_reinit_stackaddr_xc167keil( &ADDR, size )

#define REAL_STACK_SIZE(ADDR) \
   ( ADDR.sys_stack_size )

#define TRY_CATCH_STACK_ERROR(STACK_T,TEMP) \
   __asm { mov TEMP, R0 } \
   if ( TEMP < STACK_T.userstack.u.offs24._offs + SAFEZONE ){ \
      printk(("tk: Error - user stack trashed!\n")); \
      tk_exit(TC_ERR_STACK); \
   } \
   if ( DPP0 < STACK_T.userstack.u.seg24._seg ){ \
      printk(("tk: Error - user stack trashed!\n")); \
      tk_exit(TC_ERR_STACK); \
   }

void _do_trap(unsigned int num);
#define TRAP(NUM) \
   _do_trap( NUM )

#define OBSOLETE_TK_CLI() \
   __asm{ BCLR PSW_IEN } \
   _tk_IntFlagCntr++;

#define OBSOLETE_TK_STI() \
   _tk_IntFlagCntr--; \
   if (_tk_IntFlagCntr == 0) \
      __asm{ BSET PSW_IEN }

#define TK_CLI() \
   __asm{ BCLR PSW_IEN }

#define TK_STI() \
   __asm{ BSET PSW_IEN }

#define STK_CRC_CALC(TEMP) \
   __asm { mov TEMP, R0 } \
   TEMP = TEMP - 32 - 4; \
   __asm { mov R3, TEMP } \
   TEMP = 0; \
   __asm { mov R2, TEMP } \
   TEMP = 1; \
   __asm { mov R4, TEMP } \
                           \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 } \
   __asm { mov TEMP, R2 }

#define INTEGRITY_CERTIFY_STACK(TCB_T,TEMP) \
   STK_CRC_CALC( TEMP ); \
   TCB_T.stack_crc = TEMP

#define TRY_CATCH_STACK_INTEGRITY_VIOLATION(TCB_T,TEMP) \
   if ( TCB_T.stack_crc != 0 ){ \
      STK_CRC_CALC( TEMP ); \
      if ( TCB_T.stack_crc != TEMP ) { \
         printk ("\ntk: Error - Thread 0x%02X \"%s\" has got a tainted stack\n",TCB_T.Thid,TCB_T.name); \
         tk_exit( TK_ERR_STKINT ); \
      } \
   }

#endif
