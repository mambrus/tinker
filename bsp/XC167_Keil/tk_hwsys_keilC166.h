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

/*!

@note that the offset addres does not contain the 2 MSB bits that defines wich
DDP to use. It's your respponsibility to trunc these off before storing in the
off variable, and to add them back when you assign your user-stack pointer
(typically R0)

@note Orientation of bitfiels are compilor specific. Use commandline or pragma 
to ensure correct layout.

@note Keil C166 seem to lack directives for byte order within a struct.
This struct is empirically defined. Be observant about changes of
behaviour when changing compiler and/or version.

*/
#pragma pack(1)     /* byte alignment */
#pragma bytealign   /* ptrs to byte-aligned objects */
 

typedef union{ 
   unsigned long linear; 
   union {
      struct {
         unsigned char padding8;     // These two togeather must be 
         unsigned int  padding6  :6; // ... exacylly 14 bits long.
         unsigned int _seg       :10;                     
      }seg24;
      struct {
         unsigned int _offs      :14; 
         unsigned int padding2   :2;
         unsigned char padding8;
      }offs24;  
   }u;
}userstackaddr_t;

#pragma pack()    /* reset to default alignment */

typedef union{ 
   unsigned long linear; 
   struct { 
      unsigned int _offs; 
      unsigned int _seg;         
   }segmented; 
   struct { 
      unsigned int _SP; 
      unsigned int _SPSEG;          
   }reg; 
}systemstackaddr_t;

/*! 
Architecture specific representation of a stack adress. In this obscure
MPU/Compiler compo this need to be devided in two stacks for each
thread, that each is best represented in a different way. Both these
ways however also have a linear adrees for conveniant lookup in
physical memory.
*/

typedef struct { 
   systemstackaddr_t systemstack;      
   userstackaddr_t   userstack;   
   
   size_t            sys_stack_size;    //These two added togeather constitutes 
   size_t            usr_stack_size;    //the actual memory allocated
}stack_t;


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

//#include <XC16X.H>

#if defined( DEVICE )
   #include REGH( DEVICE )   
#else
   #error "Error, No device specified - can't determine register definition file !!!!!!!!!!!"
#endif

/*
#include <dave/MAIN.H>
sfr  SPSEG                = 0xFF0C;       //Bug in DaVE doesnt generate this
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
   
      
#define POPALL()                                                                                              \
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
   
#define MARKALL()                                                                                              \
   __asm{ mov R0,  #0203h              }                                                                       \    
   __asm{ mov R1,  #0405h              }                                                                       \
   __asm{ mov R2,  #0607h              }                                                                       \
   __asm{ mov R3,  #0809h              }                                                                       \
   __asm{ mov R4,  #0A0Bh              }                                                                       \
   __asm{ mov R5,  #0C0Dh              }                                                                       \
   __asm{ mov R6,  #0E0Fh              }                                                                       \
   __asm{ mov R7,  #1011h              }                                                                       \
   __asm{ mov R8,  #1213h              }                                                                       \
   __asm{ mov R9,  #1415h              }                                                                       \
   __asm{ mov R10, #1617h              }                                                                       \
   __asm{ mov R11, #1819h              }                                                                       \
   __asm{ mov R12, #1A1Bh              }                                                                       \
   __asm{ mov R13, #1C1Dh              }                                                                       \
   __asm{ mov R14, #1E1Fh              }                                                                       \
   __asm{ mov R15, #2021h              }                                                                       \
/* __asm{ mov DPP0,#2223h              } */  /*Avoid modifying these - set/used by compilor*/                  \
/* __asm{ mov DPP1,#2425h              } */                                                                    \
/* __asm{ mov DPP2,#2627h              } */                                                                    \
/* __asm{ mov DPP3,#2829h              } */                                                                    \
   __asm{ mov MDL, #2A2Bh              }                                                                       \
   __asm{ mov MDH, #2C2Dh              }                                                                       \
   __asm{ mov MDC, #0010h              } /*Only one bit that can be set ATM - set it for test*/                \
   __asm{ mov PSW, #0001h              }                                                                       \    
   



#define PREP_TOS( _oldTOS, _newSP, _temp1, _temp2, _stack_struct )                                             \
   PUSHALL();                           /*Push everything for later*/                                          \
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
   _temp2 = _stack_struct.userstack.u.offs24._offs;                                                            \
   __asm{ mov R0,_temp2             }                                                                          \
   _temp2 = _stack_struct.userstack.u.seg24._seg;                                                              \
   __asm{ mov DPP0,_temp2           }                                                                          \
                                                                                                               \
   /*<--- Compiler specific*/                                                                                  \
                                                                                                               \
                                                                                                               \
  PUSHALL();                          /*Push everything on the new stack, simulating a context state - MIGHT NEED OVERLOOCKING (R0 used for param pass)*/ \
   __asm{ mov R0,R1                 }                                                                          \
   __asm{ mov DPP0,R2               }                                                                          \
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

#define STACK_PTR( ADDR )                                                                                       \
   ((char *)ADDR.systemstack.linear)
   
   
/*!
Defines where the userstack is within the mallocated area. The system
stack will be on highest addresand the user stack will be below that.

Read ratio as x:y or user_size/system_size
*/
#define USR_SYS_RATIO 2

//Note that system stack has to fit even after usr_stack has been subtracted
//With a rato of 2 this will give a stack size for sys_stack of 64

#define MINIMUM_STACK_SIZE 0xC0 

void _tk_reinit_stackaddr_xc167keil( stack_t *addr, size_t size );

//#include <tk_ipc.h>  //< will create stupid errors

extern unsigned long Q_ASC0;
void _tk_initialize_system_ques( );

#define REINIT_STACKADDR( ADDR, size )  \
   _tk_reinit_stackaddr_xc167keil( &ADDR, size )
 
#define REAL_STACK_SIZE( ADDR )  \
   ( ADDR.sys_stack_size ) 

/*
   _temp2 = _stack_struct.userstack.u.offs24._offs;                                                            \
   __asm{ mov R0,_temp2             }                                                                          \
   _temp2 = _stack_struct.userstack.u.seg24._seg;                                                              \
   __asm{ mov DPP2,_temp2             }                                                                        \
   
   
   __asm{ mov R1,R0                 }                                                                          \
   __asm{ mov R2,DPP2               }                                                                          \


*/

#endif


