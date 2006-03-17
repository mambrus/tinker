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

/*!
On MSVC this will never ever work since EAX is actually \b trashed 
on entry of each function :/
None of the 3 canlling conventions (_cdecl, _stdcall. _fastcall) will not 
trash EAX. It's almost like MS did this on purpose for some obscure reason.

See the following dissasembly output for _cdecl

331:  void *_tk_destructor( void *foo ){
004040D0   push        ebp
004040D1   mov         ebp,esp
004040D3   sub         esp,40h
004040D6   push        ebx
004040D7   push        esi
004040D8   push        edi
004040D9   lea         edi,[ebp-40h]
004040DC   mov         ecx,10h
004040E1   mov         eax,0CCCCCCCCh         <-- The return value is in there, but EAX is trashed  ;(
004040E6   rep stos    dword ptr [edi]

This is called prolog and epiloge code and is one of the reasons why we might have to figure out another way
to prepare threads on the stack.

In MSCV's case there is a workaround, and that is to declare the function in question as naked. I.e.:

__declspec(naked) void   * _tk_destructor( void *foo )
{
bla... 
}

@todo Threads normal exit and selfdestruct. This issue is not solved and will come back!
*/

#define GET_THREADS_RETVAL( THRETVAL, TEMP )                                                                   \
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


