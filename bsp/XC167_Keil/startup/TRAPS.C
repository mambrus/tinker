/***********************************************************************/
/*  This file is part of the C166 Compiler package                     */
/*  Copyright KEIL ELEKTRONIK GmbH 1996-2003                           */
/*


 ==========================
| Modified  Michael Ambrus |
 ==========================



*/



/***********************************************************************/
/*                                                                     */
/*  TRAPS.C:  TRAP Handler for C16x/XC16x/ST10/Super10 hardware traps  */
/*                                                                     */
/*  To translate this file use C166 with the following invocation:     */
/*                                                                     */
/*     C166 TRAPS.C                                                    */
/*                                                                     */
/*  To link the modified TRAPS.OBJ file to your application use the    */
/*  following L166 invocation:                                         */
/*                                                                     */
/*     L166 <your object file list>, TRAPS.OBJ <controls>              */
/*                                                                     */
/***********************************************************************/
#include <intrins.h>
#include <stdio.h>
#include <tk.h>
#include <errno.h>
#include <kernel/src/implement_tk.h>

#define REGH( DEV ) \
   <DEV.h>
#if defined( DEVICE )
   #include REGH( DEVICE )   
#else
   #error "Error, No device specified - can't determine register definition file !!!!!!!!!!!"
#endif

extern unsigned int ASC0_uwGetData(void);

#define PRINT_TRAP            //<! set to print trap information

sfr   PSW      = 0xFF10;
sbit  IEN      = PSW^11;
sfr   TFR      = 0xFFAC;      // TFR contais Trap Reason


/*!
The most primitive of all prints. This will print anything even if
there is no valid stack that works. 

@note It will print the first letter alway, but we might want to
create a busy-wait macro also.
*/
#define PRINT_6_LETTERS( A, B, C, D, E, F )  \
  ASC0_vSendData(0x0D); busywait(10);        \
  ASC0_vSendData(0x0A); busywait(10);        \
  ASC0_vSendData( A );  busywait(10);        \
  ASC0_vSendData( B );  busywait(10);        \
  ASC0_vSendData( C );  busywait(10);        \
  ASC0_vSendData( D );  busywait(10);        \
  ASC0_vSendData( E );  busywait(10);        \
  ASC0_vSendData( F );  busywait(10);        \  
  ASC0_vSendData(0x0D); busywait(10);        \
  ASC0_vSendData(0x0A); busywait(10);


#ifdef PRINT_TRAP             // If print trap information wanted
#pragma NOFRAME               // Do not save registers for interrupt



/* Value in hex */
/*  |           */

/*http://www.infineon.com/upload/Document/cmc_upload/documents/049/625/xc167_um_v2.0_2004_04_sys.pdf*/

/*  Val  TFR Bit  Conditions

 *    1 TFR.0  = <noinfo>  Illegal External Bus Access (no external BUS defined for address)
 *    2 TFR.1  = <noinfo>  Illegal Instruction Access  (branch to odd address)
 *    4 TFR.2  = ILLOPA    Illegal Word Operand Access (word read/write on odd address)
 *    8 TFR.3  = PRTFLT    Protection Fault (protected instruction with illegal format)
 *   10 TFR.4  = PACER     Illegal or erroneous access to program memory interface
 *   20 TFR.5    <undef>
 *   40 TFR.6    <undef>
 *   80 TFR.7  = UNDOPC    Undefined Opcode (invalid 166/167 op-code)
 *  100 TFR.12 = SOFTBRK   Software break event
 *  200 TFR.13 = STKUF     Stack Underflow
 *  400 TFR.14 = STKOF     Stack Overflow
 *  800 TFR.15 = NMI       Non Maskable Interrupt
 */


static const char * trapinfo[17]={
/*none*/ "<noinfo>  - TFR not set (Very strange!!!) \n",

/* 0*/   "<noinfo>  - Illegal External Bus Access (no external BUS defined for address)\n",
/* 2*/   "<noinfo>  - Illegal Instruction Access  (branch to odd address)\n",
/* 2*/   "ILLOPA    - Illegal Word Operand Access (word read/write on odd address)\n",
/* 3*/   "PRTFLT    - Protection Fault (protected instruction with illegal format)\n",

/* 4*/   "PACER     - Illegal or erroneous access to program memory interface\n",
/* 5*/   "<undef>   - Impossible TFR.5 set\n",
/* 6*/   "<undef>   - Impossible TFR.6 set\n",
/* 7*/   "UNDOPC    - Undefined Opcode (invalid 166/167 op-code)\n",

/* 8*/   "<undef>   - Impossible TFR.8 set\n",
/* 9*/   "<undef>   - Impossible TFR.9 set\n",
/*10*/   "<undef>   - Impossible TFR.10 set\n",
/*11*/   "<undef>   - Impossible TFR.11 set\n",

/*12*/   "SOFTBRK   - Software break event\n",
/*13*/   "STKUF     - Stack Underflow\n",
/*14*/   "STKOF     - Stack Overflow\n",
/*15*/   "NMI       - Non Maskable Interrupt\n"
};


static const char * kerneltraps[]={
/*none*/ "TK_NOERR  - TinKer Termination without any errors\n",

/* 0*/   "TBD       - TinKer error TBD\n",
/* 2*/   "ASSERT    - Assertion failed. See earlier text output for more info\n",
/* 2*/   "TBD       - TinKer error TBD\n",
/* 3*/   "TBD       - TinKer error TBD\n",

/* 4*/   "STACK     - User stack out of bounds\n",
/* 5*/   "STKINT    - Stack integrity faliure detected\n",
/* 6*/   "TBD       - TinKer error TBD\n",
/* 7*/   "TBD       - TinKer error TBD\n",

/* 8*/   "TBD       - TinKer error TBD\n",
/* 9*/   "TBD       - TinKer error TBD\n",
/*10*/   "TBD       - TinKer error TBD\n",
/*11*/   "TBD       - TinKer error TBD\n",

/*12*/   "TBD       - TinKer error TBD\n",
/*13*/   "TBD       - TinKer error TBD\n",
/*14*/   "TBD       - TinKer error TBD\n",
/*15*/   "TBD       - TinKer error TBD\n"
};
 


long burntime(int x, int y, int z){
   long rc;
   rc  = 10000uL * x;
   rc += 100uL * y;
   rc += z;

   return z;
}

unsigned int busywait( unsigned int time_ms ){
   int k,x,y,z;
   long rc;
   int xwait;
   int ywait;

   xwait = time_ms / 100;
   xwait ? ywait = time_ms / xwait : ywait = time_ms;
   for (k=0;k<13;k++)
      for (x=0;x<(xwait+1);x++)
         for (y=0;y<(ywait+1);y++)
            for (z=0;z<100;z++)
               rc = burntime(x,y,z);

   return 0;			   
}

void safeprint(char *s){
   int i;

   for (i=0; s[i]; i++){ 

      if ( s[i] == '\n' || s[i] == '\r' ){
         ASC0_vSendData(0x0D);
         busywait(10);
         ASC0_vSendData(0x0A);
         busywait(10);
      }else{
         ASC0_vSendData(s[i]);
         busywait(10);
      }

   }
}

#define ASCII_0 48
#define ASCII_A 65

char i2shex_buff[7];
/*!
A very crude way to convert a integer value to human-readable form

@note Remeber that we can't relay on any stdlib. Memory might be broken!
*/
char *i2shex(int i){
   unsigned int  mask;
   int ti;
   
   i2shex_buff[0] = '0';
   i2shex_buff[1] = 'x';
   
   mask = 0xF000;
   ti = i & mask;
   ti = ti >> 12;
   ti < 9 ? ti += ASCII_0 : ti += (ASCII_A - 10);
   i2shex_buff[2]=ti;
   
   mask = 0x0F00;
   ti = i & mask;
   ti = ti >> 8;
   ti < 9 ? ti += ASCII_0 : ti += (ASCII_A - 10);
   i2shex_buff[3]=ti;
   
   mask = 0x00F0;
   ti = i & mask;
   ti = ti >> 4;
   ti < 9 ? ti += ASCII_0 : ti += (ASCII_A - 10);
   i2shex_buff[4]=ti;
   
   mask = 0x000F;
   ti = i & mask;
   //ti = ti >> 0;
   ti < 9 ? ti += ASCII_0 : ti += (ASCII_A - 10);
   i2shex_buff[5]=ti;
   
   i2shex_buff[6]=0;
   
   return i2shex_buff;
}
/*!
@brief A common info printing function for all TRAPS

A common info printing function for all TRAPS
*/
void printtrap(
   const char *leadtext,
   char *lookuptable[],
   unsigned int ip, 
   unsigned int csp,
   unsigned int tfr
){   
   unsigned char bitloop;
   unsigned int  mask;
   unsigned int  tid;
   struct tcb_t *curr_tcb;
   bit oIEN;
  
   oIEN = IEN;
   if ((leadtext == 0uL) || (leadtext[0] > 'z') ){
      //In case DDPx was broken, no pointers will work properlly
      PRINT_6_LETTERS('N','U','L','L','_','P');
      PRINT_6_LETTERS('D','D','P','0','B','R');
      PRINT_6_LETTERS('N','O','T','R','U','S');
      //oputunistic aproach. Try set DDP0 to some known "valid" value.
      DPP0 = 0x0101;      
      __asm{ mov R0, #0x18AC};  //Just guessing a value
   };      
   IEN  = 0;  
   safeprint ("\n"); 
   safeprint (leadtext); 
   IEN  = 1;
   safeprint (" at PC="); 
   safeprint (i2shex(csp));
   safeprint (":");
   safeprint (i2shex(ip));  
   safeprint (" TFR="); 
   safeprint (i2shex(tfr));
   safeprint ("\nTFR meaning explained as follows:\n"); 
  
   //tfr = 0xFFFF;  //set to test lookup table only

   IEN  = 0;  
   if (tfr){
      for (bitloop=0,mask=1; bitloop<16; bitloop++){
         if (tfr & mask){
            safeprint (lookuptable[bitloop+1]); 
         }
         mask = mask << 1;     
      }
   }else{
      safeprint (lookuptable[0]); 
   }
   IEN  = 1;
   safeprint ("\nInfo about violating thread:\n"); 
   
   IEN  = 0;
   tid = tk_thread_id();
   curr_tcb = _tk_current_tcb();
   
   IEN  = 1;
   safeprint ("Thread ID: ");
   safeprint (i2shex(tid));
   safeprint (", name: \"");
   safeprint (curr_tcb->name);
   safeprint ("\"\n");
   
   safeprint ("\n\nDetecting if errno was set...\n\n");   
   if (errno !=0 ){
     safeprint ("\n\nNote: errno [");   
     safeprint (i2shex(errno));
     safeprint ("] was set:\n");
     perror(strerror(errno));
   }
  

   IEN = oIEN;
}
  
#endif 


/*
 * Non-Maskable Interrupt
 */
void NMI_trap (void) interrupt 0x02  {
#ifdef PRINT_TRAP
  unsigned int ip, csp;
  unsigned int tfr;

  ip  = _pop_ ();
  csp = _pop_ ();
  tfr = TFR;

  PRINT_6_LETTERS('T','R','A','P','_','N');
  printtrap("\nNon-Maskable Trap (NMI)",trapinfo,ip,csp,tfr); 
  safeprint ("\n\nExecution halted (waiting for reset)...\n"); 

#endif 

  while (1);                 /* end-less loop */
}


/*
 * Stack Overflow Interrupt
 */
void STKOF_trap (void) interrupt 0x04  {
#ifdef PRINT_TRAP
  unsigned int ip, csp;
  unsigned int tfr;

  ip  = _pop_ ();
  csp = _pop_ ();
  tfr = TFR;

  PRINT_6_LETTERS('T','R','A','P','_','B');
  printtrap("\nStack Overflow Trap (STKOF)",trapinfo,ip,csp,tfr); 
  safeprint ("\n\nExecution halted (waiting for reset)...\n"); 

#endif 

  while (1);                 /* end-less loop */
}


/*
 * Stack Underflow Interrupt
 */
void STKUF_trap (void) interrupt 0x06  {
#ifdef PRINT_TRAP
  unsigned int ip, csp;
  unsigned int tfr;

  ip  = _pop_ ();
  csp = _pop_ ();
  tfr = TFR;

  PRINT_6_LETTERS('T','R','A','P','_','U');
  printtrap("\nStack Underflow Trap (STKUF)",trapinfo,ip,csp,tfr); 
  safeprint ("\n\nExecution halted (waiting for reset)...\n"); 

#endif 

  while (1);                 /* end-less loop */
}



/*
 * Class B Hardware Trap:
 *   ->  Undefined Opcode              - 10 TFR.4  = PACER     Illegal or erroneous access to program memory interface
 *   ->  Protected Instruction Fault
 *   ->  Illegal Word Operand Access
 *   ->  Illegal Instruction Access
 *   ->  Illegal External Bus Access
 */


void Class_B_trap (void) interrupt 0x0A  {

#ifdef PRINT_TRAP
  unsigned int ip, csp;
  unsigned int tfr;

  ip  = _pop_ ();
  csp = _pop_ ();
  tfr = TFR;

  PRINT_6_LETTERS('T','R','A','P','_','B');
  printtrap("\nClass B Trap",trapinfo,ip,csp,tfr); 
  safeprint ("\n\nExecution halted (waiting for reset)...\n"); 

#endif 
  /* add your code here */
  while (1);                 /* end-less loop */
}


/*!
@brief User trap. PC will end up here after call to\ref TRAP
User trap. PC will end up here after call to\ref TRAP
*/
void user_trap (void) interrupt 0x0D  {
#ifdef PRINT_TRAP
  unsigned int ip, csp;
  unsigned int _tfr;
  
  ip  = _pop_ ();
  csp = _pop_ ();
  __asm{ mov _tfr, R3 };
  //_tfr = TFR;

  PRINT_6_LETTERS('T','K','_','T','R','P');
  printtrap("\nTinKer Error Trap",kerneltraps,ip,csp,_tfr);   
  safeprint ("\n\nExecution halted (waiting for reset)...\n"); 



#endif 
  /* add your code here */
  while (1);                 /* end-less loop */
}
