/*!
@file

asm ( "statements" : output_registers : input_registers : clobbered_registers);

*/

#ifndef TK_HWSYS_CYGWIN_H
#define TK_HWSYS_CYGWIN_H

/*
#define TK_CLI()                          \
   asm __volatile__ (" CLI ");

#define TK_STI()                          \
   asm __volatile__ (" STI ");
*/   

#define TK_CLI()
#define TK_STI()


   
#define REAL_STACK_SIZE( TCB )            \
   ( TCB.stack_size ) 
   

#define PREP_TOS( _oldTOS, _newSP, _temp1, _temp2, _stack_struct )   \
   PUSHALL();                             \
   asm __volatile__ (                     \
      "nop"                               \
   );                                     \
                                          \
   /*---> Compiler specific*/             \
                                          \
   asm __volatile__ (                     \
      "nop"                               \
   );                                     \
                                          \
    /*<--- Compiler specific*/            \
                                          \
   PUSHALL();                             \
                                          \
   asm __volatile__ (                     \
      "nop"                               \
   );                                     \
   POPALL();


#define PUSH_CPU_GETCUR_STACK( TSP1, TEMP )  \
   PUSHALL();                             \
   asm __volatile__ (                     \
      "nop"                               \
   );                                     \
  
#define CHANGE_STACK_POP_CPU( TSP1, TEMP )   \
   asm __volatile__ (                     \
      "nop"                               \
   );  /*Note, no clobber (intentional)*/ \
   POPALL();

//function enters as a result of a ret instruction. EAX is passed
//as the return value. Not shure if it works on every processor


#define GET_THREADS_RETVAL( THRETVAL )    \
   asm __volatile__ (                     \
      "nop"                               \
   );

#define PUSHALL()                         \
   asm __volatile__ (                     \
      "nop"                               \
   );

#define POPALL()                          \
   asm __volatile__ (                     \
      "nop"                               \
   )


//Allready a char', no need to do handle in any special way.
#define STACK_PTR( ADDR ) \
   (ADDR.tstack)

//Not needed to do anything really. But just in case, follow the new convention 
#define REINIT_STACKADDR( ADDR, size ) \
   (ADDR.stack_size = size)

//Just a stub ATM - TBD
#define TRAP( NUM )                                                           \
   exit( NUM )   

#endif


