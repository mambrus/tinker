#include "tk_hwsys_keilC166.h"
#include <tk_ipc.h>

/*

@ingroup kernel_internal_BSP

@brief Reinitializes the stack.

On XC167 (and other C166 derivate using Keil as compiler) there are always
<b>two</b> stacks. Conceptually TinKer knows only of one, and this function
helps to create the two stacks within the same TinKer stack, i.e. within
the same memory area (usually allocated from heap with malloc).

Below are some notes for where the stacks are deducted to be.

ratio             :=  user_stack_size/system_stack_size
whole_stack_size  :=  (system_stack_size + user_stack_size)

=>

user_stack_size   := ratio * system_stack_size
system_stack_size := whole_stack_size - user_stack_size

=>

user_stack_size   := ratio * (whole_stack_size - user_stack_size)
user_stack_size   := ratio * whole_stack_size - ratio * user_stack_size


user_stack_size + ratio * user_stack_size := ratio * whole_stack_size
user_stack_size * ( 1+ ratio )            := ratio * whole_stack_size

user_stack_size                           := ratio * whole_stack_size / ( 1+ ratio )

@note According to the XC167 manual: <i>Only word accesses are supported to 
the system stack.</i> Therefor we need the word alignement as you se in the 
code.

*/

void _tk_reinit_stackaddr_xc167keil( 
   stack_t *addr, 
   size_t size 
){
   size_t usr_stack_size;
   size_t sys_stack_size;
   
   usr_stack_size = size * USR_SYS_RATIO/(1+USR_SYS_RATIO);
   sys_stack_size = size - usr_stack_size;

   //make sure we have 32 bit alignement (sys stack will contain return adresses)
   for (;sys_stack_size % 4;sys_stack_size++);
   
   usr_stack_size = size - sys_stack_size;

   
   addr->usr_stack_size = usr_stack_size;
   addr->sys_stack_size = sys_stack_size;
   
   addr->userstack.linear = addr->systemstack.linear + sys_stack_size;                      //Need also adjust in TOS
   //addr->userstack.linear = addr->systemstack.linear + sys_stack_size + usr_stack_size;   //good as temp solution
   //addr->userstack.linear = addr->systemstack.linear + usr_stack_size;                    //old
}

unsigned long Q_ASC0;

void _tk_initialize_system_ques( ){
   q_create_ny("ASC0",0x05,FIFO,&Q_ASC0);
}