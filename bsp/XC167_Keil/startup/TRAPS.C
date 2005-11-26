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

#define PRINT_TRAP            //<! set to print trap information
/*
#include <dave/MAIN.H>        //primitives for output on rs232 in case of printing
#include <dave/ASC0.H>
*/

/*
#define REGH( DEV ) \
   <DEV.h>

#if defined( DEVICE )
   #include REGH( DEVICE )   
#else
   #error "Error, No device specified - can't determine register definition file !!!!!!!!!!!"
#endif
*/

sfr   PSW      = 0xFF10;
sbit  IEN      = PSW^11;

#include <intrins.h>
#include <stdio.h>
#include <tk.h>


#ifdef PRINT_TRAP             // print trap information

#pragma NOFRAME               // do not save registers for interrupt

sfr TFR = 0xFFAC;             // TFR contais Trap Reason

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


static const char *trapinfo[16]={
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
/*15*/   "NMI       - Non Maskable Interrupt\n",
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

void printtrap(
	char *leadtext,
	unsigned int ip, 
	unsigned int csp,
	unsigned int tfr
){
  unsigned char bitloop;
  unsigned int  mask;
  unsigned int  tid;
  tk_tcb_t     *curr_tcb;
  bit oIEN;
  
  oIEN = IEN;
  IEN  = 0;
  safeprint ("\n"); 
  safeprint (leadtext); 
  safeprint (" at PC="); 
  safeprint (i2shex(csp));
  safeprint (":");
  safeprint (i2shex(ip));  
  safeprint (" TFR="); 
  safeprint (i2shex(tfr));
  safeprint ("\nTFR meaning explained as follows:\n"); 
  
  //tfr = 0xFFFF;  //set to test lookup table only
  
  for (bitloop=0,mask=1; bitloop<16; bitloop++){
     if (tfr & mask){
       safeprint (trapinfo[bitloop]); 
    }
     mask = mask << 1;     
  }
  safeprint ("\nInfo about violating thread:\n"); 
  tid = tk_thread_id();
  curr_tcb = _tk_current_tcb();
  safeprint ("Thread ID: 0x");
  safeprint (i2shex(tid));
  safeprint (", name: \"");
  safeprint (curr_tcb->name);
  safeprint ("\"\n");
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

  printtrap("\nNon-Maskable Trap (NMI)",ip,csp,tfr); 
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

  printtrap("\nStack Overflow Trap (STKOF)",ip,csp,tfr); 
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

  printtrap("\nStack Underflow Trap (STKUF)",ip,csp,tfr); 
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

  printtrap("\nClass B Trap",ip,csp,tfr); 
  safeprint ("\n\nExecution halted (waiting for reset)...\n"); 

#endif 
  /* add your code here */
  while (1);                 /* end-less loop */
}
