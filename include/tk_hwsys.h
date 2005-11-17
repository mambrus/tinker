





#if defined(_MSVC_) || defined(WIN32) 
//------------------------------------------------------------------------------------------------------------->


#define PREP_TOS( TSP2, TSP1, TOS )                                                                            \
                                                                                                               \
    __asm{ pushfd                       } /*Save CPU states affected so that we migth continue*/               \
    __asm{ pushad                       }                                                                      \
    __asm{ mov TSP2,esp                 } /*Cange the stack pointer to the actual one*/                        \
    __asm{ mov esp,TSP1                 }                                                                      \
                                                                                                               \
    /*---> Compiler specific*/                                                                                 \
    /*push ebp*/                                                                                               \
                                                                                                               \
    __asm{ push        ebp              }                                                                      \
    __asm{ mov         ebp,esp          }                                                                      \
    __asm{ sub         esp,40h          }                                                                      \
    __asm{ push        ebx              }                                                                      \
    __asm{ push        esi              }                                                                      \
    __asm{ push        edi              }                                                                      \
    __asm{ lea         edi,[ebp-40h]    }                                                                      \
    __asm{ mov         ecx,10h          }                                                                      \
    __asm{ mov         eax,0CCCCCCCCh   }                                                                      \
    __asm{ rep stos    dword ptr [edi]  }                                                                      \
                                                                                                               \
    /*<--- Compiler specific*/                                                                                 \
                                                                                                               \
                                                                                                               \
    __asm{ pushfd                       }/*Part of the stack content our kernel expects to find on the stack*/ \
    __asm{ pushad                       }                                                                      \
                                                                                                               \
    __asm{ mov TOS,esp                  } /*The current SP is now the new TOS, save it..     */                \
    __asm{ mov esp,TSP2                 } /*Restore the stack pointer so that we can continue*/                \                                                                      \
                                                                                                               \
    __asm{ popad                        } /*Restore the CPU to the state befor this process was invoked*/      \
    __asm{ popfd                        }



//Push & pops of all regs and flags possibly not needed
#define PUSH_CPU_GETCUR_STACK( TESP )                                                                          \
    __asm{ pushfd                       }                                                                      \
    __asm{ pushad                       }                                                                      \
    __asm{ mov TESP,esp                 }
  
#define CHANGE_STACK_POP_CPU( TESP )                                                                           \
    __asm{ mov esp,TESP                 }                                                                      \
    __asm{ popad                        }                                                                      \
    __asm{ popfd                        }

//function enters as a result of a ret instruction. EAX is passed
//as the return value. Not shure if it works on every processor

#define GET_THREADS_RETVAL( THRETVAL )                                                                         \
   __asm{ mov THRETVAL,EAX             }

//<-------------------------------------------------------------------------------------------------------------
#elif defined( __C166__ )
//------------------------------------------------------------------------------------------------------------->
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
   



#define PREP_TOS( TSP2, TSP1, TOS, TEMP )                                                                     \
   PUSHALL();                           /*Push everything for later*/                                         \
   /*Store the current SP for later*/                                                                         \
   __asm{ mov TSP2,SPSEG              }                                                                       \
   TEMP = (unsigned long)TSP2;                                                                          \
   TEMP <<= 16;                                                                          \
   __asm{ mov TSP2,SP                 }                                                                       \
   TSP2 = TEMP + TSP2;                                                                                        \
                                                                                                              \
   /*Cange the stack pointer to the intended one*/                                                            \
   TEMP = (unsigned long)TSP1 >> 16;                                                                          \
   __asm{ mov SP,TSP1                 }                                                                       \
   __asm{ mov SPSEG,TEMP              }                                                                       \
                                                                                                              \
                                                                                                              \
   /*---> Compiler specific*/                                                                                 \
   /*<--- Compiler specific*/                                                                                 \
                                                                                                              \
                                                                                                              \
  PUSHALL();                             /*Push everything on the new stack, simulating a context state - MIGHT NEED OVERLOOCKING (R0 used for param pass)*/     \
                                                                                                              \
   TOS = 0ul;                            /*  Important, or the next assembly "cast" will fail (not clearing 16 MSB */ \
   __asm{ mov TOS,SP                   } /*The current SP is now the new TOS, save it.      */                \
   TOS = ((unsigned long)SPSEG<<16) + (unsigned long)TOS;                    /*  This value will then be copied into the TCB    */                \
                                                                                                              \
                                                                                                              \
   /*Restore the old stack pointer and CPU conten so that we can continue, */                                 \
   TEMP = (unsigned long)TSP2 >> 16;                                                                          \   
   __asm{ mov SP,TSP2                 }                                                                       \
   __asm{ mov SPSEG,TEMP              }                                                                       \
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

//<-------------------------------------------------------------------------------------------------------------
#elif defined(__GNUC__)
//------------------------------------------------------------------------------------------------------------->
   #error "Implementation (only simulated \"scheduler in process\" possible) for a GNU system not done yet"
#else
   #error "Can\'t determine the target for the TINKER kernel"
#endif





