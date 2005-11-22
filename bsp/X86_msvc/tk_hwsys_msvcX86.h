#ifndef TK_HWSYS_MSVCX86_H
#define TK_HWSYS_MSVCX86_H

/*! 
Architecture specific representation of a stack adress. In x86 this can be a 
simple char* even in 16bit modes, since a pointer contains both segment and 
offset.
*/
typedef char * stack_t;


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


#endif


