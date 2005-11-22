#ifndef TK_HWSYS_H
#define TK_HWSYS_H

#if defined(DOXYGEN_ONLY)

//------1---------2---------3---------4---------5---------6---------7---------8

/**
@ingroup kernel_internals   
        
@brief Pushes all CPU on current stack

Pushes all CPU on stack in a way that corresponts to what happens when an
interrupt. Pushes will (and should be) on the current stack in scope.

@note The rule to follow is not needed in a non-preeimptive version of
of the kernel, but a very good rule to follow just in case you deside to add
preemptivity later.

@todo Verify for XC167
   
*/
#define PUSHALL()

/**
@ingroup kernel_internals
        
@brief Pops all CPU from current stack

Pops all CPU from current stack the same way an interrupt does when it's ISR is
finished.

@note You have to change the SP to the new stack before using this macro

@todo Verify for XC167
   
*/
#define POPALL()

/**
@ingroup kernel_internals
        
@brief Prepares Top Of Stack

Use this macro to set up a initil stack content on a <B>newly</B> created
threads stack so that the stack follows two requirements:<br><br>

1. The return adress shall be the start adress of the threads function

2. The stack shall follow the same rules as PUSALL and POPALL, i.e it should
follow the rule that the top of the stack should be the same as when an
interrupt has occured.

@note Some extra storage variables are needed to manipulate the SP. <b> These
might not be alocatd on the same stack they manipulate.</B> Best rule to follow
is to have these variables in the code segment (i.e. globally declared).

*/
#define PREP_TOS(                                                             \
   TSP2,    //!< Temporary stackpointer for internal manipulations            \
   TSP1,    //!< in:  The newly created stacks start adress                   \
   TOS,     //!< out: The resultin SP. I.e. (char*)(TSP1+lengt-tos_data)      \
   TEMP     //!< Extra variable for internal stuff (not needed by all targets)\ 
)


/**
@ingroup kernel_internals
        
@brief Pushes all CPU on current stack and return SP.

Pushes all CPU on current stack and returnes the stack pointer after that push

Typically used in the first stage of a context switch. The resulting SP is 
saved in the threads TCB to be used when it's time to revive it.

*/
   
#define PUSH_CPU_GETCUR_STACK(                                                \
   TSP1,    //!< The resultin SP                                              \
   TEMP     //!< Extra variable for internal stuff (not needed by all targets)\ 
)  

/**
@ingroup kernel_internals
        
@brief Change to new SP and pop all CPU

Change SP to the one supplied and push all CPU from it.

Typically used in the second stage of a context switch. The intended SP is retrived
from the corresponding threads TCB.

*/
#define CHANGE_STACK_POP_CPU( 
TSP1, 
TEMP 
)                                                                    \

/**
@ingroup kernel_internals
        
@brief Dunno what this is ;/

@todo If this macro is really needed, document it also!
*/

#define GET_THREADS_RETVAL( THRETVAL )                                                                        \

   
//------1---------2---------3---------4---------5---------6---------7---------8
#elif defined(WIN32) && defined(_MSVC_)
#include <../bsp/X86_msvc/tk_hwsys_msvcX86.h>

#elif defined(WIN32) && defined(_BC50_)
#include <../bsp/X86_Borland/tk_hwsys_borlandX86.h>


#elif defined( __C166__ )
#include <../bsp/XC167_Keil/tk_hwsys_keilC166.h>


#elif defined(__GNUC__)
#error "Implementation (only simulated \"scheduler in process\" possible) for a GNU system not done yet"

#else
#error "Can\'t determine the target for the TINKER kernel"   

#endif

//------1---------2---------3---------4---------5---------6---------7---------8

#endif  //TK_HWSYS_H





