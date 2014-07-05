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
#include <time.h>

#include <kernel/src/implement_tk.h>

extern thid_t theSchedule[TK_MAX_PRIO_LEVELS][TK_MAX_THREADS_AT_PRIO];
extern struct tcb_t_ proc_stat[TK_MAX_THREADS];
extern struct stat_t scheduleIdxs[TK_MAX_PRIO_LEVELS];


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
/*none*/ "TK_NOERR        - TinKer Termination without any errors\n",

/* 0*/   "MAX_THREADS     - Total amount of threads would exceed limit\n",
/* 2*/   "ASSERT          - Assertion failed. See earlier text output for more info\n",
/* 2*/   "MAX_PRIO_LEVELS - Chosen priority too high\n",
/* 3*/   "TBD             - TinKer error TBD (bit 3)\n",

/* 4*/   "STACK           - User stack out of bounds\n",
/* 5*/   "STKINT          - Stack integrity faliure detected\n",
/* 6*/   "THREADS_AT_PRIO - To many threads at this prio\n",
/* 7*/   "AMOK            - Kernel running amok detected\n",

/* 8*/   "NAME_LEN        - Thread-name to long\n",
/* 9*/   "NOMEM           - No memory left for allocation\n",
/*10*/   "TCB_INVALID     - Invalid TCB detected\n",
/*11*/   "TBD             - TinKer error TBD (bit 11)\n",

/*12*/   "TBD             - TinKer error TBD  (bit 12)\n",
/*13*/   "TBD             - TinKer error TBD  (bit 13)\n",
/*14*/   "ERR_HW          - Some HW driver detected a fatal error\n",
/*15*/   "UNKNOWN         - Unknown (or undefined) termination reason\n"
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

@note Hexadecimal notation. Be careful how you interpret negative numbers.
This function will always treat them as positive.

In case you wonder why this seemingly stupid function exists:

@note Remember that we can't relay on any stdlib. Memory might be broken!
*/
char *i2shex(unsigned int i){
   unsigned int  mask;
   unsigned int ti;

   i2shex_buff[0] = '0';
   i2shex_buff[1] = 'x';

   mask = 0xF000;
   ti = i & mask;
   ti = ti >> 12;
   ti <= 9 ? ti += ASCII_0 : ti += (ASCII_A - 10);
   i2shex_buff[2]=ti;

   mask = 0x0F00;
   ti = i & mask;
   ti = ti >> 8;
   ti <= 9 ? ti += ASCII_0 : ti += (ASCII_A - 10);
   i2shex_buff[3]=ti;

   mask = 0x00F0;
   ti = i & mask;
   ti = ti >> 4;
   ti <= 9 ? ti += ASCII_0 : ti += (ASCII_A - 10);
   i2shex_buff[4]=ti;

   mask = 0x000F;
   ti = i & mask;
   //ti = ti >> 0;
   ti <= 9 ? ti += ASCII_0 : ti += (ASCII_A - 10);
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
   unsigned int  iprio,idx;
   struct tcb_t_ *tcb;
   bit oIEN;
   struct timespec uptime;
   int rc;

   //The following row is risky - might breake if memory broken
   rc=getnanouptime(&uptime);

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
   safeprint ("****************************************");
   safeprint ("****************************************\n");
   safeprint (leadtext);
   safeprint (" at PC=");
   safeprint (i2shex(csp));
   safeprint (":");
   safeprint (i2shex(ip));
   safeprint (" TFR=");
   safeprint (i2shex(tfr));
   safeprint ("\n****************************************");
   safeprint ("****************************************\n");
   IEN  = 1;
   safeprint ("TFR meaning explained as follows:\n");

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
   tcb = _tk_current_tcb();

   IEN  = 1;
   safeprint ("Thread ID: ");
   safeprint (i2shex(tid));
   safeprint (", name: \"");
   safeprint (tcb->name);
   safeprint ("\"\n");

   safeprint ("Detecting if errno was set...\n");
   if (errno !=0 ){
      safeprint ("Yes, errno [");
      safeprint (i2shex(errno));
      safeprint ("] was set:\n");
      perror(strerror(errno));
   }else{
      safeprint ("No errno set\n\n");
   }

   safeprint ("\nDumping schedule...\n");
   safeprint ("----------------------------------------");
   safeprint ("----------------------------------------\n");

   for (iprio=0; iprio < TK_MAX_PRIO_LEVELS; iprio++){

      if (scheduleIdxs[iprio].procs_at_prio > 0) {

         safeprint ("Prio [");
         safeprint (i2shex(iprio));
         safeprint ("]\n");
         safeprint ("-idx-tid--gid--#cld-STAT-#err-STK_");
         safeprint ("STRT--CURR_STK--SSIZ-PRIO-IDX--[ thread name ]\n");


         for (idx=0; idx < TK_MAX_THREADS_AT_PRIO; idx++){
            tid = theSchedule[iprio][idx];
            tcb = &proc_stat[tid];

            if (
               (  (iprio || idx) && tid && (tcb->valid == TRUE) ) ||
               ( !(iprio || idx) && (tcb->valid == TRUE) )
            ){
               safeprint(&i2shex(tid)[2]);             safeprint(" ");  //! Tid that schedule says it has
               safeprint(&i2shex(tcb->Thid)[2]);       safeprint(" ");  //!< unsigned int / Process ID and Parent ID (Gid)
               safeprint(&i2shex(tcb->Gid)[2]);        safeprint(" ");  //!< unsigned int / Process ID and Parent ID (Gid)
               safeprint(&i2shex(tcb->noChilds)[2]); /*safeprint(" ");*/ //!< unsigned int / Numb of procs this has created

               //-v- PROCSTATE -v-
               if (tcb->state == 0){
                  if (tk_thread_id() == tid)
                     safeprint(">RDY ");
                  else
                     safeprint(" RDY ");
               }else{
			      safeprint(" ");

                  if (tcb->state & ____Z___)
                     safeprint("Z");
                  else
                     safeprint("_");

                  if (tcb->state & _____Q__)
                     safeprint("Q");
                  else
                     safeprint("_");

                  if (tcb->state & ______S_)
                     safeprint("S");
                  else
                     safeprint("_");

                  if (tcb->state & _______T)
                     safeprint("T");
                  else
                     safeprint("_");
               }
               //-^- PROCSTATE -^-
               safeprint(" ");


               safeprint(&i2shex(tcb->_errno_)[2]);    safeprint(" ");

               safeprint(&i2shex( ( ((unsigned long)&(tcb->stack_begin)) & 0xFFFF0000ul) >> 16 )[2]);
               safeprint(":");
               safeprint(&i2shex( (unsigned long)&(tcb->stack_begin) )[2]);
               safeprint(" ");

               safeprint(&i2shex( ( ((unsigned long)&(tcb->curr_sp)) & 0xFFFF0000ul) >> 16 )[2]);
               safeprint(":");
               safeprint(&i2shex( (unsigned long)&(tcb->curr_sp) )[2]);
               safeprint(" ");

               safeprint(&i2shex(tcb->stack_size)[2]);   safeprint(" ");

               /*
               safeprint(&i2shex( ( ((unsigned long)(tcb->stack_crc)) & 0xFFFF0000ul) >> 16 )[2]);
               safeprint(&i2shex( (unsigned long)(tcb->stack_crc) )[2]);
               safeprint("  ");
               */
               /*
               tcb->wakeuptime;          //!< clock_t / When to wake up if sleeping
               tcb->wakeupEvent;         //!< wakeE_t / Helper variable mainly for ITC
               tcb->start_funct;         //!< start_func_f / Address of the threads entry function. Used ONLY for debugging purposes
               tcb->init_funct;          //!< init_func_f / Support of the pThread <em>"once"</em> concept.
               tcb->*prmtRetAddr;        //!< void * / Preempted return adress - used in preempted mode.
               */

               safeprint(&i2shex(tcb->Prio)[2]);   safeprint(" ");
               safeprint(&i2shex(tcb->Idx)[2]);   safeprint(" ");
               safeprint(tcb->name);
               safeprint("\n");
            }
         }
         safeprint("\n");
      }
   }
   safeprint ("----------------------------------------");
   safeprint ("----------------------------------------\n");
   safeprint("Tinker uptime (S.nS): ");

   //rc=getnanouptime(&uptime);
   safeprint(&i2shex((uptime.tv_sec&0xFFFF0000) >>16 )[2]);
   safeprint(&i2shex(uptime.tv_sec)[2]);
   safeprint(".");
   safeprint(&i2shex((uptime.tv_nsec&0xFFFF0000) >>16 )[2]);
   safeprint(&i2shex(uptime.tv_nsec)[2]);

   //safeprint("(\nNote: HEX notation.Subtract time for printout.\n)");
   safeprint("\n(Note: HEX notation.)\n");



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

/*!
 * @defgroup CVSLOG_TRAPS_C TRAPS_C
 * @ingroup CVSLOG
 *  $Log: TRAPS.C,v $
 *  Revision 1.18  2006-03-24 11:22:54  ambrmi09
 *  - pThreads RW locks implemented (rough aproach - no usage error detection)
 *  - restructuring of the pThread src-files
 *
 *  Revision 1.17  2006/03/19 22:57:53  ambrmi09
 *  First naive implementation of a pthread mutex
 *
 *  Revision 1.16  2006/03/17 12:20:01  ambrmi09
 *  Major uppdate (5 days hard work)
 *
 *  - Finally tied up all loose ends in the concept. Threads are now
 *  joinable
 *
 *  - Corrected one error: compacting scheduele while cancelling a
 *  threads
 *
 *  - Several new API, mainly concerned with cancelation (corrsp pThread
 *  also)
 *
 *  - Found a nasty bug while creating threads in threads for XC167. TOS is
 *  really a patchy solution ;( This one had to do with the compiler
 *  being fooled by the inline assembly and optimized something that was not
 *  optimizable (saving stack segment got wacked).
 *
 *  - Designed a concurrent qsort test-app. This is good for showing
 *  boss-worker model. Number of threads recoed on XC167 was 50 and on MSVS
 *  more than 150! Interesting to notice was that TinKer creation and
 *  cancelation for threads was much faster than Windows own (20-30 times
 *  faster).
 *
 *  - A MSVC workspace for pThreads-Win32. Good for testing apps
 *  transparency.
 *
 *  - Increased memory on XC167 (phyCore HW). now 32k for stacks and 16k for
 *  malloc. We still lack RAM that is not deployed (pHycore has
 *  128k + 256k physical RAM memory i think). Maximum for
 *  stack is 64k however (type of pointers determine this). If memory is
 *  increased further, we get a TRAP_B saying bad memory interface. Typical
 *  error for config memory issues in DaVe.
 *
 *  Revision 1.15  2006/02/25 14:44:29  ambrmi09
 *  Found the nasty \ref BUG_000_001. Solution is robust but potentially degrades
 *  tinkers timing presition.
 *
 *  Found another bug caused by wraparound, that occures once every 71.6 minuts
 *  but selcom shows itself.
 *
 *  @note  that systimer (sys_mickey) wraps once every 49.7 days but
 *  kernel native time-keeping wraps 1000 times more often (71.6
 *  minutes). This is due to that current precision on sys_time is in
 *  mS, but kernel precision is in uS as a preparation to that the
 *  \ref clock will be replaced by a higher precision time function
 *  (\uptime or something similar).
 *
 *  prepared for better presision clock (true uS presision).
 *
 *  Revision 1.14  2006/02/23 15:33:33  ambrmi09
 *  Found a nasty "bug", that was not a read bug after all. At least not in the kernel as a feared. It turned out that I forgot some of the details about how timeouts were to be handled (especially in \ref ITC ). A timeout of value \b zero is equal of never to timeout (read more about it in define \ref FOREVER). However two important lesson learned: Even simple add operations get "funny" when adding large numbers (see line 303 in tk_ipc.c - in the \ref lock_stage function). Anyway. FOREVER should equal zero. (This issue makes me wonder sometimes how sane it really was to resurrect a project that has been dormant for nearly 10 years.) The CodeWright project ruler should be positioned on the actual line btw. This check-in will be accompanied  by a <tt>cvs tag</tt> for this reason, and for yet another nasty bug that seems to be a real dispatcher bug. The current source-set-up will show the bug within one mint (which is good since it makes it a little bit less of a search for the <I>"needle in the haystack</i>").
 *
 *  Revision 1.13  2006/02/23 11:34:58  ambrmi09
 *  - Improved post mortem
 *   - Fixed bug in i2hex2.
 *   - Added uptime output
 *   - mark running among RDY threads in schedule dump
 *
 *  - \ref putchar now supports easy switching between serial0 and serial1
 *
 *  mqueue.h and mqueue.c should be untouched. But trying to identify if
 *  qsort is the reason for TinKer sometimes to hang (recent possible bug).
 *
 *  Revision 1.12  2006/02/22 13:05:45  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *
 */

