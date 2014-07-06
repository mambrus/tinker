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
//------1---------2---------3---------4---------5---------6---------7---------8
/*
The following pragmas are important compiler directives to ensure correct
behaviour of the kernel. They should be supplied on the command line or in the
"misc control" of Uv2 project setting. For convenience (and for explanation)
they are given here to. <b>Be sure not to have conflicting directives between
command line and pragmas.</b>
*/

/*!
The DYNAMICUSRSTK directive informs the C166 Compiler that your program
modifies the user stack area. This directive is useful for developers who
create real-time operating systems that change the user stack area.

This directive makes it possible to have a 16K user stack for each task.
Without this directive, the entire user stack must reside in the NDATA memory
class which is limited to a maximum of 64K.

@see http://www.keil.com/support/man/docs/c166/c166_dynamicusrstk.htm
*/
#pragma dynamicusrstk

/*!
The SAVEUSR directive instructs the C166 Compiler to save temporary results and
saved-by-callee variables on the user stack. This option may be necessary if
you run out of system stack space. The system stack is always located in
on-chip RAM, which is faster than external RAM but is limited in size.

@see http://www.keil.com/support/man/docs/c166/c166_saveusr.htm
*/
#pragma saveusr

//#pragma savesys

/*
The REENTRANT directive instructs the C166 Compiler to store automatic
variables that cannot be allocated to a CPU register on the user stack. The
generated code is fully reentrant.

http://www.keil.com/support/man/docs/c166/c166_reentrant.htm
*/
#pragma reentrant

#pragma NOFD

//------1---------2---------3---------4---------5---------6---------7---------8

#include <stddef.h>

//------1---------2---------3---------4---------5---------6---------7---------8

#define REGH( DEV ) \
   <DEV.h>

/**
Note that macro name DEVICE is supposed to be set by the invoker (i.e. when you run the compilation
process. This is done either with flags on the command prompt, or by defining this in the Uv2 project file
(which will then automatically be translated to a flag upon invocation). The naming convention of the
device must follow Keils predefined devices, which you can find as files in the directory
%install_dir%\Keil\C166\INC\ .
*/

#if defined( DEVICE )
#include REGH( DEVICE )
#else
#error "Error, No device specified - can't determine register definition file !!!!!!!!!!!"
#endif

/*!
How printk is implemented on this target
*/
#define printk(x) printf x

/*!
How stack malloc operations are implemented
*/

#include <../bsp/XC167_Keil/stalloc.h>

/*
void *stalloc (TLEN size);
void stalloc_free (void *memp);
void stalloc_init_mempool (void MTYP *pool, TLEN size);
*/

/*
#include <dave/MAIN.H>
sfr  SPSEG                = 0xFF0C;       //Bug in DaVE doesnt generate this
*/

/*!
@note According to XC167 manual chapter 4.3.4, sub-chapter
<i>"CSFRs Affecting the Whole CPU"</i>, modifying STKOV, STKUN indirectly
via push and pop should be OK until next "real" OP code that uses that SFR.
*/

#define PUSHALL()                                                                                             \
   __asm{ push  PSW                    }                                                                      \
   __asm{ push  R0                     }                                                                      \
   __asm{ push  R1                     }                                                                      \
   __asm{ push  R2                     }                                                                      \
   __asm{ push  R3                     }                                                                      \
   __asm{ push  R4                     }                                                                      \
   __asm{ push  R5                     }                                                                      \
   __asm{ push  R6                     }                                                                      \
   __asm{ push  R7                     }                                                                      \
   __asm{ push  R8                     }                                                                      \
   __asm{ push  R9                     }                                                                      \
   __asm{ push  R10                    }                                                                      \
   __asm{ push  R11                    }                                                                      \
   __asm{ push  R12                    }                                                                      \
   __asm{ push  R13                    }                                                                      \
   __asm{ push  R14                    }                                                                      \
   __asm{ push  R15                    }                                                                      \
   __asm{ push  DPP0                   }                                                                      \
   __asm{ push  DPP1                   }                                                                      \
   __asm{ push  DPP2                   }                                                                      \
   __asm{ push  DPP3                   }                                                                      \
   __asm{ push  MDL                    }                                                                      \
   __asm{ push  MDH                    }                                                                      \
   __asm{ push  MDC                    }                                                                      \
   __asm{ push  STKOV                  }                                                                      \
   __asm{ push  STKUN                  }

#define POPALL()                                                                                              \
   __asm{ pop  STKUN                   }                                                                      \
   __asm{ pop  STKOV                   }                                                                      \
   __asm{ pop  MDC                     }                                                                      \
   __asm{ pop  MDH                     }                                                                      \
   __asm{ pop  MDL                     }                                                                      \
   __asm{ pop  DPP3                    }                                                                      \
   __asm{ pop  DPP2                    }                                                                      \
   __asm{ pop  DPP1                    }                                                                      \
   __asm{ pop  DPP0                    }                                                                      \
   __asm{ pop  R15                     }                                                                      \
   __asm{ pop  R14                     }                                                                      \
   __asm{ pop  R13                     }                                                                      \
   __asm{ pop  R12                     }                                                                      \
   __asm{ pop  R11                     }                                                                      \
   __asm{ pop  R10                     }                                                                      \
   __asm{ pop  R9                      }                                                                      \
   __asm{ pop  R8                      }                                                                      \
   __asm{ pop  R7                      }                                                                      \
   __asm{ pop  R6                      }                                                                      \
   __asm{ pop  R5                      }                                                                      \
   __asm{ pop  R4                      }                                                                      \
   __asm{ pop  R3                      }                                                                      \
   __asm{ pop  R2                      }                                                                      \
   __asm{ pop  R1                      }                                                                      \
   __asm{ pop  R0                      }                                                                      \
   __asm{ pop  PSW                     }

#define PUSH_CPU_GETCUR_STACK( TSP1, TEMP )                                                                   \
   PUSHALL()                                                                                                  \
   TEMP = 0ul;                      /*Important, or the next assembly "cast" will fail (not clearing 16 MSB */ \
   __asm{ mov TEMP,SP             } /*The current SP is now the new _newSP, save it.      */                  \
   TSP1 = ((unsigned long)SPSEG<<16) + (unsigned long)TEMP;	/*  This value will then be copied into the TCB    */

#define CHANGE_STACK_POP_CPU( TSP1, TEMP )                                                                    \
   TEMP = (unsigned long)TSP1 >> 16;                                                                          \
   __asm{ mov SP,TSP1                 }                                                                       \
   __asm{ mov SPSEG,TEMP              }                                                                       \
   POPALL();

#define CHANGE_STACK( TSP1, TEMP )                                                                            \
   TEMP = (unsigned long)TSP1 >> 16;                                                                          \
   __asm{ mov SP,TSP1                 }                                                                       \
   __asm{ mov SPSEG,TEMP              }

#define INIT_SP( _stack_SP, _stack_begin )                                                                  \
   _stack_SP.usr_stack_size      = _stack_begin.usr_stack_size;                                             \
   _stack_SP.userstack.linear    = _stack_begin.userstack.linear   + _stack_begin.usr_stack_size;           \
   _stack_SP.sys_stack_size      = _stack_begin.sys_stack_size;                                             \
   _stack_SP.systemstack.linear  = _stack_begin.systemstack.linear + _stack_begin.sys_stack_size;

#define BIND_STACK( _stack_struct, _temp2 )                                                                 \
   _temp2 = _stack_struct.userstack.u.offs24._offs;    /*Set up the user-stack pointer (DPP0:r0)*/          \
   _temp2 = _temp2 - 4;                                /*Add some slack (alignement issue)*/                \
   __asm{ mov R0,_temp2             }                  /*I.e. the pointer for the threads local data*/      \
   _temp2 = _stack_struct.userstack.u.seg24._seg;                                                           \
   __asm{ mov DPP0,_temp2           }                                                                       \
                                                                                                            \
   _temp2 = _stack_struct.systemstack.reg._SP + 4 /*Note: had to add a few bytes (not good really)*/;       \
   __asm{ mov STKOV,_temp2          }                                                                       \
   _temp2 = _stack_struct.systemstack.reg._SP + _stack_struct.sys_stack_size;                               \
   __asm{ mov STKUN,_temp2          }

#define GET_THREADS_RETVAL( THRETVAL, TEMP  )                                                               \
   __asm{ mov THRETVAL, R4 }                                                                                \
   __asm{ mov TEMP, R5 }                                                                                    \
   THRETVAL = (TEMP<<16) + THRETVAL;

#define STACK_PTR( ADDR )                                                                                   \
   ((char *)ADDR.systemstack.linear)

extern unsigned long Q_ASC0;
void _tk_initialize_system_ques();

#define REINIT_STACKADDR( ADDR, size )                                                                      \
   _tk_reinit_stackaddr_xc167keil( &ADDR, size )

#define REAL_STACK_SIZE( ADDR )                                                                             \
   ( ADDR.sys_stack_size )

#define TRY_CATCH_STACK_ERROR( STACK_T, TEMP )                                \
   __asm { mov TEMP, R0 }                                                     \
   if ( TEMP < STACK_T.userstack.u.offs24._offs + SAFEZONE ){                 \
      printk(("tk: Error - user stack trashed!\n"));                          \
      tk_exit(TC_ERR_STACK);                                                  \
   }                                                                          \
   if ( DPP0 < STACK_T.userstack.u.seg24._seg ){                              \
      printk(("tk: Error - user stack trashed!\n"));                          \
      tk_exit(TC_ERR_STACK);                                                  \
   }

void _do_trap(unsigned int num);
#define TRAP( NUM )                                                           \
   _do_trap( NUM )

#define OBSOLETE_TK_CLI()                                                     \
   __asm{ BCLR PSW_IEN }                                                      \
   _tk_IntFlagCntr++;

#define OBSOLETE_TK_STI()	                                              \
   _tk_IntFlagCntr--;  /*Is ok since CLI is active no one can interfere*/      \
   if (_tk_IntFlagCntr == 0)                                                   \
      __asm{ BSET PSW_IEN }

#define TK_CLI()                                                              \
   __asm{ BCLR PSW_IEN }

#define TK_STI()	                                                      \
   __asm{ BSET PSW_IEN }

#define STK_CRC_CALC( TEMP )                                               \
   __asm { mov TEMP, R0 }                                                  \
   TEMP = TEMP - 32 - 4;       /*Stupid, but due to [Rx-] bug*/            \
   __asm { mov R3, TEMP }                                                  \
   TEMP = 0;                                                               \
   __asm { mov R2, TEMP }                                                  \
   TEMP = 1;                                                               \
   __asm { mov R4, TEMP }                                                  \
                           \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { add R2, [R3+] } \
   __asm { ROL R2, R4 }    \
   __asm { mov TEMP, R2 }

/*!
@ingroup kernel_internals

TBD

*/
#define INTEGRITY_CERTIFY_STACK( TCB_T, TEMP)               \
   STK_CRC_CALC( TEMP );                                    \
   TCB_T.stack_crc = TEMP

/*!
@ingroup kernel_internals

TBD

*/
#define TRY_CATCH_STACK_INTEGRITY_VIOLATION( TCB_T, TEMP)      \
   if ( TCB_T.stack_crc != 0 ){                                \
      STK_CRC_CALC( TEMP );                                    \
      if ( TCB_T.stack_crc != TEMP )	{                       \
         printk ("\ntk: Error - Thread 0x%02X \"%s\" has got a tainted stack\n",TCB_T.Thid,TCB_T.name); \
         tk_exit( TK_ERR_STKINT );                             \
      }                                                        \
   }

#endif				//TK_HWSYS_KEILC166_H


