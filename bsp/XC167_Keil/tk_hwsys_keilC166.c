#include "tk_hwsys_keilC166.h"
#include <tk_ipc.h>


/*

ratio             :=  user_stack_size/system_stack_size
whlole_stack_size :=  (system_stack_size + user_stack_size)

=>

user_stack_size   := ratio * system_stack_size
system_stack_size := whlole_stack_size - user_stack_size

=>

user_stack_size   := ratio * (whlole_stack_size - user_stack_size)
user_stack_size   := ratio * whlole_stack_size - ratio * user_stack_size


user_stack_size + ratio * user_stack_size := ratio * whlole_stack_size
user_stack_size * ( 1+ ratio )            := ratio * whlole_stack_size

user_stack_size                           := ratio * whlole_stack_size / ( 1+ ratio )

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
   
   addr->userstack.linear = addr->systemstack.linear + usr_stack_size;   
}

unsigned long Q_ASC0;

void _tk_initialize_system_ques( ){
   q_create_ny("ASC0",0x05,FIFO,&Q_ASC0);
}