#ifndef TK_HWSYS_MSVCX86_H
#define TK_HWSYS_MSVCX86_H

#define MINIMUM_STACK_SIZE 0x0600  //!< TBD this @todo TBD this

/*!
How printk is implemented on this target
*/
#define printk printf

/*!
@name Mapping stack allocation API for this target
*/
//@{
#define stalloc      malloc
#define stalloc_free free
//@}



/*
#define TK_CLI()                                                              \
   __asm{ CLI }

#define TK_STI()	                                                            \
   __asm{ STI }
*/   

#define TK_CLI()
#define TK_STI()


   
#define REAL_STACK_SIZE( TCB )  \
   ( TCB.stack_size ) 
   

#define PREP_TOS( _oldTOS, _newSP, _temp1, _temp2, _stack_struct )                                             \
                                                                                                               \
    __asm{ pushfd                       } /*Save CPU states affected so that we migth continue*/               \
    __asm{ pushad                       }                                                                      \
    __asm{ mov _temp1,esp               } /*Cange the stack pointer to the actual one*/                        \
    __asm{ mov esp,_oldTOS              }                                                                      \
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
    __asm{ mov _newSP,esp               } /*The current SP is now the new _newSP, save it..     */             \
    __asm{ mov esp,_temp1               } /*Restore the stack pointer so that we can continue*/                \
                                                                                                               \
    __asm{ popad                        } /*Restore the CPU to the state befor this process was invoked*/      \
    __asm{ popfd                        }
    



//Push & pops of all regs and flags possibly not needed
#define PUSH_CPU_GETCUR_STACK( TSP1, TEMP )                                                                    \
    __asm{ pushfd                       }                                                                      \
    __asm{ pushad                       }                                                                      \
    __asm{ mov TSP1,esp                 }
  
#define CHANGE_STACK_POP_CPU( TSP1, TEMP )                                                                     \
    __asm{ mov esp,TSP1                 }                                                                      \
    __asm{ popad                        }                                                                      \
    __asm{ popfd                        }

//function enters as a result of a ret instruction. EAX is passed
//as the return value. Not shure if it works on every processor

#define GET_THREADS_RETVAL( THRETVAL )                                                                         \
   __asm{ mov THRETVAL,EAX             }

#define PUSHALL()  \
    __asm{ pushfd                       }                                                                      \
    __asm{ pushad                       }                                                                      \

#define POPALL()  \
    __asm{ popad                        }                                                                      \
    __asm{ popfd                        }																								\


//Allready a char', no need to do handle in any special way.
#define STACK_PTR( ADDR ) \
   ((char *)ADDR.tstack)

//Not needed to do anything really. But just in case, follow the new convention 
#define REINIT_STACKADDR( ADDR, size ) \
   (ADDR.stack_size = size)

//Just a stub ATM - TBD
#define TRAP( NUM )                                                           \
   exit( NUM )   

#endif


