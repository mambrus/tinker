/***************************************************************************
 *   Copyright (C) 2006 by Michale Ambrus                                  *
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

#ifndef TK_HWSYS_BORLANDX86_H
#define TK_HWSYS_BORLANDX86_H

#define MINIMUM_STACK_SIZE 0x0600  //!< TBD this @todo TBD this
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
                                                                                                               \
    __asm{ push        ebp              }                                                                      \
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


