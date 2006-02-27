/*******************************************************************
 *
 *  DESCRIPTION: TinKerOS.
 *
 *  AUTHOR: Michael Ambrus
 *                              
 *  HISTORY:    
 *
 *  Current $Revision: 1.16 $
 *
 *******************************************************************/
  

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
#define MINIMUM_STACK_SIZE ((minimum_system_stack_size + 1)*USR_SYS_RATIO) + SAFEZONE)



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
   __asm{ push  STKUN                  }                                                                      \

   
      
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
   __asm{ pop  PSW                     }                                                                      \
   

#define PREP_TOS( _oldTOS, _newSP, _temp1, _temp2, _stack_struct )                                             \
   PUSHALL();                           /*Push everything for later*/                                          \
   _stack_struct.userstack.linear = _stack_struct.userstack.linear + _stack_struct.usr_stack_size;             \
   /*Store the current SP for later*/                                                                          \
   __asm{ mov _temp1,SPSEG          }                                                                          \
   _temp2 = (unsigned long)_temp1;                                                                             \
   _temp2 <<= 16;                                                                                              \
   __asm{ mov _temp1,SP             }                                                                          \
   _temp1 = _temp2 + _temp1;                                                                                   \
                                                                                                               \
   /*Cange the stack pointer to the intended one*/                                                             \
   _temp2 = (unsigned long)_oldTOS >> 16;                                                                      \
   __asm{ mov SP,_oldTOS            }                                                                          \
   __asm{ mov SPSEG,_temp2          }                                                                          \
                                                                                                               \
                                                                                                               \
   /*---> Compiler specific*/                                                                                  \
   __asm{ mov R1,R0                 }                                                                          \
   __asm{ mov R2,DPP0               }                                                                          \
   __asm{ mov R3,STKOV              }                                                                          \
   __asm{ mov R4,STKUN              }                                                                          \
                                                                                                               \
   _temp2 = _stack_struct.userstack.u.offs24._offs;                                                            \
   __asm{ mov R0,_temp2             }                                                                          \
   _temp2 = _stack_struct.userstack.u.seg24._seg;                                                              \
   __asm{ mov DPP0,_temp2           }                                                                          \
                                                                                                               \
   _temp2 = _stack_struct.systemstack.reg._SP /*+ 0xA0*/;                                                      \
   __asm{ mov STKOV,_temp2          }                                                                          \
   _temp2 = _stack_struct.systemstack.reg._SP + _stack_struct.sys_stack_size;                                  \
   __asm{ mov STKUN,_temp2          }                                                                          \
                                                                                                               \
                                                                                                               \
   /*<--- Compiler specific*/                                                                                  \
                                                                                                               \
                                                                                                               \
  PUSHALL();                          /*Push everything on the new stack, simulating a context state - MIGHT NEED OVERLOOCKING (R0 used for param pass)*/ \
   __asm{ mov R0,R1                 }                                                                          \
   __asm{ mov DPP0,R2               }                                                                          \
   __asm{ mov STKOV,R3              }                                                                          \
   __asm{ mov STKUN,R4              }                                                                          \
                                                                                                               \
                                                                                                               \
   _newSP = 0ul;                      /*Important, or the next assembly "cast" will fail (not clearing 16 MSB */ \
   __asm{ mov _newSP,SP             } /*The current SP is now the new _newSP, save it.      */                 \
   _newSP = ((unsigned long)SPSEG<<16) + (unsigned long)_newSP;                    /*  This value will then be copied into the TCB    */                  \
                                                                                                               \
                                                                                                               \
   /*Restore the old stack pointer and CPU conten so that we can continue, */                                  \
   _temp2 = (unsigned long)_temp1 >> 16;                                                                       \
   __asm{ mov SP,_temp1             }                                                                          \
   __asm{ mov SPSEG,_temp2          }                                                                          \
                                                                                                               \
  POPALL();   



//Push & pops of all regs and flags possibly not needed  
   
#define PUSH_CPU_GETCUR_STACK( TSP1, TEMP )                                                                   \
   PUSHALL()                                                                                                  \
   __asm{ mov TSP1,SPSEG              }                                                                       \
   TEMP = (unsigned long)TSP1 << 16;                                                                          \
   __asm{ mov TSP1,SP                 }                                                                       \
   TSP1 = TEMP + (unsigned long)TSP1;                                                                         \
  
#define CHANGE_STACK_POP_CPU( TSP1, TEMP )                                                                    \
   TEMP = (unsigned long)TSP1 >> 16;                                                                          \
   __asm{ mov SP,TSP1                 }                                                                       \
   __asm{ mov SPSEG,TEMP              }                                                                       \
   POPALL();                                                                                                  \

#define GET_THREADS_RETVAL( THRETVAL )                                                                        \

#define STACK_PTR( ADDR )                                                                                     \
   ((char *)ADDR.systemstack.linear)
   
   


//#include <tk_ipc.h>  //< will create stupid errors

extern unsigned long Q_ASC0;
void _tk_initialize_system_ques( );

#define REINIT_STACKADDR( ADDR, size )  \
   _tk_reinit_stackaddr_xc167keil( &ADDR, size )
 
#define REAL_STACK_SIZE( ADDR )  \
   ( ADDR.sys_stack_size ) 
   

#define TRY_CATCH_STACK_ERROR( STACK_T, TEMP )                                \
   __asm { mov TEMP, R0 }                                                     \
   if ( TEMP < STACK_T.userstack.u.offs24._offs + SAFEZONE ){                 \
      printf("tk: Error - user stack trashed!\n");                            \
      tk_exit(TK_ERR_STACK);                                                  \
   }                                                                          \
   if ( DPP0 < STACK_T.userstack.u.seg24._seg ){                              \
      printf("tk: Error - user stack trashed!\n");                            \
      tk_exit(TK_ERR_STACK);                                                  \
   }
   

#define TRAP( NUM )                                                           \
   _do_trap( NUM )

//#define TK_CLI()                                                              \
//   __asm{ BCLR PSW_IEN }                                                      \
//   Tk_IntFlagCntr++;                                                          \   
//
//
//#define TK_STI()	                                                           \
//   Tk_IntFlagCntr--;  /*Is ok since CLI is active no one can interfere*/      \
//   if (Tk_IntFlagCntr == 0)                                                   \
//      __asm{ BSET PSW_IEN }

#define TK_CLI()                                                              \
   __asm{ BCLR PSW_IEN }                                                      



#define TK_STI()	                                                            \
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
   __asm { mov TEMP, R2 }  \
   


/*!
@ingroup kernel_internals

TBD

*/
#define INTEGRITY_CERTIFY_STACK( TCB_T, TEMP)               \
   STK_CRC_CALC( TEMP );                                    \  
   TCB_T.stack_crc = TEMP                                   \



/*!
@ingroup kernel_internals

TBD

*/
#define TRY_CATCH_STACK_INTEGRITY_VIOLATION( TCB_T, TEMP)   \
   if ( TCB_T.stack_crc != 0 ){ \
   STK_CRC_CALC( TEMP );                                    \
   if ( TCB_T.stack_crc != TEMP )	{                       \
      printf ("\ntk: Error - Thread 0x%02X \"%s\" has got a tainted stack\n",TCB_T.Thid,TCB_T.name); \
      tk_exit( TK_ERR_STKINT );                             \
   }                                                        \
   } \



#endif //TK_HWSYS_KEILC166_H

/*

*/

  
/*!
 * @defgroup CVSLOG_tk_hwsys_keilC166_h tk_hwsys_keilC166_h
 * @ingroup CVSLOG
 *
 *  $Log: tk_hwsys_keilC166.h,v $
 *  Revision 1.16  2006-02-27 13:30:03  ambrmi09
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
