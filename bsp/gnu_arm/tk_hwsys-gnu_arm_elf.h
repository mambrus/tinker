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
 
/*!
@file

asm ( "statements" : output_registers : input_registers : clobbered_registers);

http://tisu.mit.jyu.fi/embedded/TIE345/luentokalvot/Embedded_3_ARM.pdf
http://www.arm.com/miscPDFs/9658.pdf

*/

#ifndef TK_HWSYS_GNU_ARM_H
#define TK_HWSYS_GNU_ARM_H


#define MINIMUM_STACK_SIZE 0x0200  //!< TBD this @todo TBD this

/*!
How printk is implemented on this target. I.e. no ability to output on console

@note macro should digest <b>all</b> arguments.
*/
#define printk(x) ((void)0)

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
   

/*
*/

#define PREP_TOS( _oldTOS, _newSP, _temp1, _temp2, _stack_struct )   \
   PUSHALL();                             \
   asm __volatile__ (                     \
      " mov  %0, %%r13\n\t"               \
      : "=r" (_temp1)                     \
      : /**/                              \
      : "memory"                          \
   );                                     \
   asm __volatile__ (                     \
      " mov  %%r13, %0\n\t"               \
      : /**/                              \
      : "r" (_oldTOS)                     \
   ); /*Note, no clobber (intentional)*/  \
                                          \
                                          \
   /*---> Compiler specific*/             \
                                          \
   asm __volatile__ (                     \
      "stmdb sp!, {sp}"                   \
   );                                     \
   asm __volatile__ (                     \
      "stmdb sp!, {r11, r12, sp}"         \
   );                                     \
                                          \
    /*<--- Compiler specific*/            \
                                          \
   PUSHALL();                             \
                                          \
   asm __volatile__ (                     \
      " mov  %0, %%r13\n\t"               \
      : "=r" (_newSP)                     \
      : /**/                              \
      : "memory"                          \
   );                                     \
   asm __volatile__ (                     \
      " mov  %%r13, %0\n\t"               \
      : /**/                              \
      : "r" (_temp1)                      \
   ); /*Note, no clobber (intentional)*/  \
   POPALL();


/*
   asm __volatile__ (                     \
      "stmdb sp!, {r11}"                  \
   );                                     \
   asm __volatile__ (                     \
      "stmdb sp!, {r11, r12, lr}"         \
   );                                     \
 

){
0x00001b04 <_tk_context_switch_to_thread>:     mov   r12, sp
0x00001b08 <_tk_context_switch_to_thread+4>:   stmdb sp!, {r11, r12, lr, pc}
0x00001b0c <_tk_context_switch_to_thread+8>:   sub   r11, r12, #4	; 0x4
0x00001b10 <_tk_context_switch_to_thread+12>:  sub   sp, sp, #8	; 0x8
0x00001b14 <_tk_context_switch_to_thread+16>:  str   r0, [r11, #-16]
0x00001b18 <_tk_context_switch_to_thread+20>:  str   r1, [r11, #-20]
   //TRY_CATCH_STACK_ERROR( proc_stat[SID].stack_begin, cswTEMP2 );
   //INTEGRITY_CERTIFY_STACK( proc_stat[SID], cswTEMP2 ); //Certify the stack we're leaving from

   
   PUSH_CPU_GETCUR_STACK( cswTSP, cswTEMP );
0x00001b1c <_tk_context_switch_to_thread+24>:  stmdb sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
0x00001b20 <_tk_context_switch_to_thread+28>:  ldr   r3, [pc, #164]	; 0x1bcc <$d>
0x00001b24 <_tk_context_switch_to_thread+32>:  ldr   r3, [r3]
0x00001b28 <_tk_context_switch_to_thread+36>:  mov   r2, r3
0x00001b2c <_tk_context_switch_to_thread+40>:  mov   r2, sp
0x00001b30 <_tk_context_switch_to_thread+44>:  ldr   r3, [pc, #148]	; 0x1bcc <$d>
0x00001b34 <_tk_context_switch_to_thread+48>:  str   r2, [r3]

   STACK_PTR( proc_stat[SID].curr_sp ) = cswTSP;
0x00001b38 <_tk_context_switch_to_thread+52>:  ldr   r2, [r11, #-20]
0x00001b3c <_tk_context_switch_to_thread+56>:  ldr   r3, [pc, #136]	; 0x1bcc <$d>
0x00001b40 <_tk_context_switch_to_thread+60>:  ldr   r12, [r3]
0x00001b44 <_tk_context_switch_to_thread+64>:  ldr   r1, [pc, #132]	; 0x1bd0 <$d+4>
0x00001b48 <_tk_context_switch_to_thread+68>:  mov   r0, #56	; 0x38
0x00001b4c <_tk_context_switch_to_thread+72>:  mov   r3, r2
0x00001b50 <_tk_context_switch_to_thread+76>:  mov   r3, r3, lsl #2
0x00001b54 <_tk_context_switch_to_thread+80>:  add   r3, r3, r2
0x00001b58 <_tk_context_switch_to_thread+84>:  mov   r2, r3, lsl #2
0x00001b5c <_tk_context_switch_to_thread+88>:  add   r3, r3, r2
0x00001b60 <_tk_context_switch_to_thread+92>:  mov   r3, r3, lsl #2
0x00001b64 <_tk_context_switch_to_thread+96>:  add   r3, r3, r1
0x00001b68 <_tk_context_switch_to_thread+100>: add   r3, r3, r0
0x00001b6c <_tk_context_switch_to_thread+104>: str   r12, [r3]
   
   cswTSP = STACK_PTR( proc_stat[RID].curr_sp );
0x00001b70 <_tk_context_switch_to_thread+108>: ldr   r2, [r11, #-16]
0x00001b74 <_tk_context_switch_to_thread+112>: ldr   r1, [pc, #84]	; 0x1bd0 <$d+4>
0x00001b78 <_tk_context_switch_to_thread+116>: mov   r0, #56	; 0x38
0x00001b7c <_tk_context_switch_to_thread+120>: mov   r3, r2
0x00001b80 <_tk_context_switch_to_thread+124>: mov   r3, r3, lsl #2
0x00001b84 <_tk_context_switch_to_thread+128>: add   r3, r3, r2
0x00001b88 <_tk_context_switch_to_thread+132>: mov   r2, r3, lsl #2
0x00001b8c <_tk_context_switch_to_thread+136>: add   r3, r3, r2
0x00001b90 <_tk_context_switch_to_thread+140>: mov   r3, r3, lsl #2
0x00001b94 <_tk_context_switch_to_thread+144>: add   r3, r3, r1
0x00001b98 <_tk_context_switch_to_thread+148>: add   r3, r3, r0
0x00001b9c <_tk_context_switch_to_thread+152>: ldr   r2, [r3]
0x00001ba0 <_tk_context_switch_to_thread+156>: ldr   r3, [pc, #36]	; 0x1bcc <$d>
0x00001ba4 <_tk_context_switch_to_thread+160>: str   r2, [r3]
   active_thread=RID;
0x00001ba8 <_tk_context_switch_to_thread+164>: ldr   r2, [pc, #36]	; 0x1bd4 <$d+8>
0x00001bac <_tk_context_switch_to_thread+168>: ldr   r3, [r11, #-16]
0x00001bb0 <_tk_context_switch_to_thread+172>: str   r3, [r2]

   CHANGE_STACK_POP_CPU( cswTSP, cswTEMP );
0x00001bb4 <_tk_context_switch_to_thread+176>: ldr   r3, [pc, #16]	; 0x1bcc <$d>
0x00001bb8 <_tk_context_switch_to_thread+180>: ldr   r3, [r3]
0x00001bbc <_tk_context_switch_to_thread+184>: mov   sp, r3
0x00001bc0 <_tk_context_switch_to_thread+188>: ldmia sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
   //TRY_CATCH_STACK_INTEGRITY_VIOLATION( proc_stat[active_thread], cswTEMP2 ); //Check integrity is OK before running 
}
0x00001bc4 <_tk_context_switch_to_thread+192>: sub   sp, r11, #12	; 0xc
0x00001bc8 <_tk_context_switch_to_thread+196>: ldmia sp, {r11, sp, pc}
0x00001bcc <$d>:                               andmi r0, r0, r0, asr r11
0x00001bd0 <$d+4>:                             andmi r0, r0, r8, asr r9
0x00001bd4 <$d+8>:                             andmi r0, r0, r4, lsl #24

 ----------------------------------------------------------------------
void tk_yield( void ){
0x00001bd8 <tk_yield>:    mov   r12, sp
0x00001bdc <tk_yield+4>:  stmdb sp!, {r11, r12, lr, pc}
0x00001be0 <tk_yield+8>:  sub   r11, r12, #4	; 0x4
   TK_CLI();   
   PUSHALL();   
0x00001be4 <tk_yield+12>: stmdb sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
   TK_STI();
   
   _tk_wakeup_timedout_threads();
0x00001be8 <tk_yield+16>: bl    0x175c <_tk_wakeup_timedout_threads>
   
   TK_CLI();
   
   //Do not premit interrupts between the following two. Proc statuses 
   //(i.e. thread statuses) frozen in time.
   thread_to_run = _tk_next_runable_thread();
0x00001bec <tk_yield+20>: bl    0x193c <_tk_next_runable_thread>
0x00001bf0 <tk_yield+24>: mov   r2, r0
0x00001bf4 <tk_yield+28>: ldr   r3, [pc, #36]	; 0x1c20 <$d>
0x00001bf8 <tk_yield+32>: str   r2, [r3]
   _tk_context_switch_to_thread(thread_to_run,active_thread);   
0x00001bfc <tk_yield+36>: ldr   r3, [pc, #28]	; 0x1c20 <$d>
0x00001c00 <tk_yield+40>: ldr   r2, [r3]
0x00001c04 <tk_yield+44>: ldr   r3, [pc, #24]	; 0x1c24 <$d+4>
0x00001c08 <tk_yield+48>: ldr   r3, [r3]
0x00001c0c <tk_yield+52>: mov   r0, r2
0x00001c10 <tk_yield+56>: mov   r1, r3
0x00001c14 <tk_yield+60>: bl    0x1b04 <_tk_context_switch_to_thread>
   
   POPALL();
0x00001c18 <tk_yield+64>: ldmia sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, lr}
   TK_STI();
}
0x00001c1c <tk_yield+68>: ldmia sp, {r11, sp, pc}
0x00001c20 <$d>:          andmi r0, r0, r8, lsr #16
0x00001c24 <$d+4>:        andmi r0, r0, r4, lsl #24

*/

#define PUSH_CPU_GETCUR_STACK( TSP1, TEMP )  \
   PUSHALL();                             \
   asm __volatile__ (                     \
      "mov %0, %%r13 "                    \
      : "=r" (TSP1)                       \
      : /**/                              \
      : "memory"                          \
   )
  
#define CHANGE_STACK_POP_CPU( TSP1, TEMP )   \
   asm __volatile__ (                     \
      "mov %%r13, %0 "                    \
      : /**/                              \
      : "r" (TSP1)                        \
   );  /*Note, no clobber (intentional)*/ \
   POPALL();                              \
   asm __volatile__ (                  \
      "add %r11, %r13, #0x14"            \
   );  /*Note, no clobber (intentional)*/ 


//TBD
#define GET_THREADS_RETVAL( THRETVAL )    \
   asm __volatile__ (                     \
      "nop"                               \
   );

//Note: flags (CPSR) not saved. TBD!!!
#define PUSHALL()                         \
   asm __volatile__ (                     \
      "stmdb sp!, {r0-r12,lr}"            \
   );

//Note: flags (CPSR) not restored. TBD!!!
#define POPALL()                          \
   asm __volatile__ (                     \
      "ldmia sp!, {r0-r12,lr}"            \
   );

//Allready a char', no need to do handle in any special way.
#define STACK_PTR( ADDR ) \
   (ADDR.tstack)

//Not needed to do anything really. But just in case, follow the new convention 
#define REINIT_STACKADDR( ADDR, size ) \
   (ADDR.stack_size = size)

//Just a stub ATM - TBD
#define TRAP( NUM )                                                           \
   exit( NUM )   

//------1---------2---------3---------4---------5---------6---------7---------8

//Non generic - don't use outside ARM specific code
#define GET_SP( param )                   \
   asm __volatile__ (                     \
      "mov %0, %%r13 "                    \
      : "=r" (param )                     \
      : "0" (param)                       \
      : "memory"                          \
   )

#define GET_LR( param )                   \
   asm __volatile__ (                     \
      "mov %0, %%r14 "                    \
      : "=r" (param )                     \
      : "0" (param)                       \
      : "memory"                          \
   )

#define GET_PC( param )                   \
   asm __volatile__ (                     \
      "mov %0, %%r15\n\t "                \
      "sub %0, %0, #4 "                   \
      : "=r" (param )                     \
      : "0" (param)                       \
      : "memory"                          \
   )


#define aGET_PC( param )                   \
   __asm__( "mov %0, #1", "=r" (param))


#endif


