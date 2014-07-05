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


#ifndef TK_HWSYS_KEILC166_H
#define TK_HWSYS_KEILC166_H
//------1---------2---------3---------4---------5---------6---------7---------8
/*
The following pragmas are important compilor directives to ensure correct
behavious of the kernal. They should be supplied on the command line or in the
"misc control" of Uv2 project setting. For fonveniance (and for explanation)
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
proces. This is done iether with flags on the command prompt, or by definig this in the Uv2 project file
(which will then automatically be translated to a flag upon invocation). The nameing convention of the
device must follow Keils predifined devices, which you can find as files in the directory
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
@note According to XC167 manual chapter 4.3.4, subchapter
<i>"CSFRs Affecting the Whole CPU"</i>, modifying STKOV, STKUN indirectlly
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
   TSP1 = ((unsigned long)SPSEG<<16) + (unsigned long)TEMP; /*  This value will then be copied into the TCB    */


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
                                                                                                            \

#define GET_THREADS_RETVAL( THRETVAL, TEMP  )                                                               \
   __asm{ mov THRETVAL, R4 }                                                                                \
   __asm{ mov TEMP, R5 }                                                                                    \
   THRETVAL = (TEMP<<16) + THRETVAL;

#define STACK_PTR( ADDR )                                                                                   \
   ((char *)ADDR.systemstack.linear)


extern unsigned long Q_ASC0;
void _tk_initialize_system_ques( );

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


void _do_trap (unsigned int num);
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



#endif //TK_HWSYS_KEILC166_H

/*

*/


/*!
 * @defgroup CVSLOG_tk_hwsys_keilC166_h tk_hwsys_keilC166_h
 * @ingroup CVSLOG
 *
 *  $Log: context_keilC166.h,v $
 *  Revision 1.3  2007-03-23 20:27:24  ambrmi09
 *  1) Reorganization of ITC into several smaller files
 *  2) Component pthread now supports 3,5,9 and 16 priorities
 *
 *  Revision 1.2  2006-11-09 12:31:35  ambrmi09
 *  Fixed macros with line cont. last (broke build after indent)
 *
 *  Revision 1.1  2006/11/03 11:33:30  ambrmi09
 *  Moved architecture specific files out from bsp structure and into kernel
 *  source 'arch' structure. This makes sense since I plan to refine the
 *  project into kernel specific maintenence on one hand, and BSP (and
 *  eventually other's) on the other.
 *
 *  BSP's as part of the *kernlel* project will only be in the form of crude
 *  template BSP (community will take care of the rest).
 *
 *  Revision 1.27  2006/04/08 10:15:51  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.26.2.4  2006/04/03 20:07:21  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.26.2.3  2006/04/03 15:21:45  ambrmi09
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
 *  Revision 1.26.2.2  2006/03/31 17:42:55  ambrmi09
 *  Second idea for the new thread starter. This one plays nice with several
 *  more compilers beacuse of it's balances call-stack. It's not as
 *  beautiful as the former one IMO, but GNU is a pain in the but
 *  with it's call-stack optimizations (and decorations doesn't seem to work
 *  for Cygwin GCC ).
 *
 *  Revision 1.26.2.1  2006/03/30 10:52:18  ambrmi09
 *  First version of new threadstarter. It seems very promising. A *lot* of
 *  awfull pain concerning different targets has the potential to go away.
 *
 *  Revision 1.26  2006/03/24 11:22:53  ambrmi09
 *  - pThreads RW locks implemented (rough aproach - no usage error detection)
 *  - restructuring of the pThread src-files
 *
 *  Revision 1.25  2006/03/17 12:20:00  ambrmi09
 *  Major uppdate (5 days hard work)
 *
 *  - Finally tied up all loose ends in the concept. Threads are now
 *  joinable
 *
 *  - Corrected one error: compacting scheduele while cancelling a
 *  threads
 *
 *  - Several new API, mainly concerned with cancelation (corrsp pThread
 *  also)
 *
 *  - Found a nasty bug while creating threads in threads for XC167. TOS is
 *  really a patchy solution ;( This one had to do with the compiler
 *  being fooled by the inline assembly and optimized something that was not
 *  optimizable (saving stack segment got wacked).
 *
 *  - Designed a concurrent qsort test-app. This is good for showing
 *  boss-worker model. Number of threads recoed on XC167 was 50 and on MSVS
 *  more than 150! Interesting to notice was that TinKer creation and
 *  cancelation for threads was much faster than Windows own (20-30 times
 *  faster).
 *
 *  - A MSVC workspace for pThreads-Win32. Good for testing apps
 *  transparency.
 *
 *  - Increased memory on XC167 (phyCore HW). now 32k for stacks and 16k for
 *  malloc. We still lack RAM that is not deployed (pHycore has
 *  128k + 256k physical RAM memory i think). Maximum for
 *  stack is 64k however (type of pointers determine this). If memory is
 *  increased further, we get a TRAP_B saying bad memory interface. Typical
 *  error for config memory issues in DaVe.
 *
 *  Revision 1.24  2006/03/14 11:15:50  ambrmi09
 *  tk_hwsys_keilC166.h: Removed code that was not doing it's job, and also
 *  a bit misleading since whether it works or not in all cases depended on
 *  how the compiler happened to compile.
 *
 *  __asm{ mov R1,R0                 }
 *  __asm{ mov R2,DPP0               }
 *  __asm{ mov R3,STKOV              }
 *  __asm{ mov R4,STKUN              }
 *
 *  The above lines to store away registers and then to restore them from
 *  the same registers was used to avoid a underflow/overflow trap while
 *  manipulating the SP (STKOV, and SKTUN were relevant R0:DPP0 were
 *  actually not since they were not trigger any trap and would be restored
 *  later anyway). Simply pushing and poping doesn't work since we need to
 *  pop after SP has been changed, we would had popped the wrong stack.
 *
 *  The lines are simply removed, because several of the assumed to be left
 *  alone registers were modified by the compile (since we're mixing inline
 *  assembly with actual c-code we have no real control about that the
 *  compiler decides to do). If STKOV or STKUN traps does come up while
 *  creating threads in future, we need to find another mechanism to
 *  temporary store STKOV or STKUN <b>or to inhibit</b> traps until the end
 *  of the whole operation.
 *
 *  ---------------------------------------------------------------------
 *
 *  Revision 1.23  2006/03/14 10:30:51  ambrmi09
 *  Patch mentioned in previous commit. It takes care of a supposed
 *  alignement problem in the user-stack that could explain why some threads
 *  seem to write a few bytes outside their stack.
 *
 *  Revision 1.22  2006/03/12 17:51:49  ambrmi09
 *  <h3>This check-in is XC167 related and does not concern other
 *  ports.</h3>
 *
 *  Added a padding of 6 bytes to get around the malloc bug (or what I think
 *  is a bug in XC167 malloc). Turns out that I was wrong about the layout
 *  of the XC167 stack mentioned a few check-in comments ago (which is a bit
 *  funny since I defined it myself). On each "stack-blocks" last addresses
 *  are the local data of the thread. Return addresses and such, I had moved
 *  to a lower address. The map is as follows:
 *
 *  :--------------------------------------:
 *  : <- block begin (malloced)            :
 *  :                                      :
 *  :                                      :
 *  :     -^-  (grows "upwards")           :
 *  : <- Function and ISR return addresses :
 *  :                                      :
 *  :                                      :
 *  :                                      :
 *  :     -^-  (grows "upwards")           :
 *  : <- Local data                        :
 *  : <- Last address of block             :
 *  :--------------------------------------:
 *
 *  What worries me are mainly two things:
 *
 *  - Where does ISR's put it's local data (I know the DPP registers are
 *  involved somehow). By setting breakpoint on memory access of bytes just
 *  outside a block, I can see that the timer ISR writes something there ;(
 *
 *  - Could I have set the stack pointer too high up in memory. so that
 *  local data operates outside the stack region?
 *
 *  I'm not sure if any of the above concerns are actually valid. I thought
 *  I investigated this very well. And also the stack guards should catch
 *  situations mentioned above (STKOV, STKUN). But I feel it's best to
 *  mention it for future reference.
 *
 *  This does however rise the question if not thread stacks should have
 *  their own malloc function (kalloc) with it's own distinguished memory.
 *  It can't be kmem based though,  since thread stacks sizes can vary.
 *  By simply duplicating malloc we gain the following benefits:
 *
 *  - It becomes easier to debug ISR and scheduler related issues since
 *  other blocks don't (like queues) clobber the memory layout.
 *
 *  - Wild pointer operations on heap becomes less probable to corrupt
 *  stacks threads (ATM any malloc'ed block can come in between any stack).
 *
 *  - In XC167 case, we're limited to a memory address range of 16K for
 *  <b>all stacks</b>. However we have plenty more memory on the eval card
 *  (256KB + 512KB !)
 *
 *  - In other processors as in XC167 there might be memory areas with
 *  faster (or non cashable) memory. Those are perfect for stacks (the
 *  plainer the better).
 *
 *  - It might be possible to implement some kind of stack starvation trap.
 *  This would be very nice indeed since that is the single most common
 *  error in multi-threaded applications (i.e. this has nothing to do with
 *  the kernel itself, but is a service to aid system development).
 *
 *  Revision 1.21  2006/03/12 15:08:53  ambrmi09
 *  - Adjusted the source to accomodate the new file structure.
 *
 *  - All build environments uppdated and verified except BC5. For this one
 *  we stumbled across the header-file issue that I've been fearing. Seems
 *  we need to take care of that one after all.
 *
 *  @note The malloc bug still not solved.
 *
 *  Revision 1.20  2006/03/11 14:37:48  ambrmi09
 *  - Replaced printf with printk in in-depth parts of the kernel. This is
 *  to make porting easier since printk can then be mapped to whatever
 *  counsole output ability there is (including none if there isn't any).
 *
 *  - Conditionals for: 1) time ISR latency and 2) clock systimer faliure
 *  introduced. This is because debugging involves stopping the program but
 *  not the clock HW, which will trigger the "trap" as soon as resuming the
 *  program after a BP stop (or step). I.e. inhibit those part's when
 *  debugging (which is probably most of the time). Remeber to re-enable for
 *  "release" version of any application.
 *
 *  - Working on getting rid of all the compilation warnings.
 *
 *  - Detected a new serious bug. If an extra malloc is not executed
 *  *before* the first thread is created that requires a steck  (i.e. the
 *  idle tread sice root allready has a stack), that thread will fail with
 *  an illegal OP-code trap. This has been traced to be due to a faulty
 *  malloc and/or possibly a memory alignement problem. The first block
 *  allocated, will be about 6 positions to high up in the memory map, which
 *  means that sthe total block will not really fit. If that block is the
 *  stack of a thread, those positions will be either the context or the
 *  return adress of that thread (which is bad). I'm concerned about not
 *  detecting this error before, which leads me to believe that this
 *  actually is an alignement issue in malloc and it's anly pure chance
 *  wheather the bug will manifest or not. This is a problem related
 *  to the Keil_XC167 target only.
 *
 *  Revision 1.19  2006/03/07 08:24:12  ambrmi09
 *  A very crude port for ARM is running (LPC2129) - @note THIS IS HIGHLY EXPERIMENTAL CODE
 *
 *  Revision 1.18  2006/03/05 11:11:23  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.17  2006/02/28 11:50:07  ambrmi09
 *  - Trimmed the time constants (ruffly). 4sek per 14hrs drift
 *  - Revived the Borland C (BC5) target. Projectfile also added (BC5.ide)
 *  - Started experimenting with a indlude filename macro, that has the
 *    the potential of solving my ANSI header/function dilemma (\ref
 *    BUILDCHAIN )
 *  - Some "fishyness" about BC5 handling of time. Either \ref clock or
 *    \ref CLK_TCK doesn't follow standard (the latter I know for a fact,
 *    since it's 1e3 instead of 1e6 - but thats not all). \ref tk_msleep is
 *    adjusted to try to see the error.
 *
 *  Revision 1.16  2006/02/27 13:30:03  ambrmi09
 *  <b>Please read the in depth comments</b> about this check-in at \ref
 *  Blog051125
 *
 *  The test program (test.c) in this check-in is also particularly nasty
 *  since it gives really long latencies on each task switch (up to and
 *  above 500mS!). Test against this if you make any changes in either
 *  timing or dispatching. Even considering this hard case, the drift was
 *  very minor. The timing constants need re-trimming though.
 *
 *  Revision 1.15  2006/02/22 13:05:45  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.14  2005/12/04 15:48:52  ambrmi09
 *  API for ne pre-emptable timers in place. Implementing this will be a
 *  hard but fun "nut" to crack. ptime has the potential of comming
 *  very close to the high-res timers that POSIX 1003.1c define and is a
 *  good pointer whether pthreads is a good next step or not for TinKer.
 *
 *  Revision 1.13  2005/12/03 14:04:30  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *
 *******************************************************************/
