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
#define CHANGE_STACK_POP_CPU(                                                 \
   TSP1,                                                                      \
   TEMP                                                                       \
)

/**
@ingroup kernel_internals
        
@brief Dunno what this is ;/

@todo If this macro is really needed, document it also!
*/

#define GET_THREADS_RETVAL( THRETVAL )


/**
@ingroup kernel_internals
        
@brief Refer to a stack address as a pointer. 

Refer to a stack address as a pointer. Only the stacks actual adress
will be handled (assuming the whole of a stack can be referred to with
one address).

@note In case you system has complex stacks you might need to
post-adjust this.

@note In a XC167/Keil combo target, a thread has two stacks. One
system-stack (for return adresses and parameter passing) and one
user-stack (local function data). Only the real stack, i.e. the system
stack, is treated with this macro. Since we assume (i.e. we have
defined), that there is always one ond only one adress for a stack there
must be a relation between system- and user-stacks, and any user-stack
adress can be deducted from the system-stack address. However, note that
to set everything up correct in the structure one has to run a
post-operative stack normalization.


@note This will work on the targets supported up until TinKer's state up
until now (051123), but might need to be reworked or replaced with a
function later on. In future implementations a stack_t might not carry
an linear address at all, but a id. In that case a simple macro will not
do at all.
*/

#define STACK_PTR( ADDR )


/*
@brief Initialize or re-initialize a stack_t address

On some targets where the stack address in complex, this macro makes the
pos operative operation mentioned for STACK_PTR. On targets where
stack_t is a simple char*, this macro does nothing.

@note Always remember to run this macro after a assignement of a new
stack addres if you want the stack to have up-to-date structural
information.

@note In a XC167/Keil combo target, this is especially important. The
macro recalculates a user stack, based on the system-stack and some
additional information about where to find it (hardcoded and internal).

*/
#define REINIT_STACKADDR( ADDR, size )

/*
@brief Gets the real stack size.

Gets the real stack size to be used to get top of stack for return adresses.

@note <b>NOT TO BE USED FOR CALC ADRESSES FOR DATA</b> (some obscure
processors make a difference between those stacks.)

*/
#define REAL_STACK_SIZE( ADDR ) 


   
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







