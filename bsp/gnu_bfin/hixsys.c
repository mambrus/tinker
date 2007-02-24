#include <tinker/config.h>

//#include <_ansi.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/stat.h>
//#include <_syslist.h>

#if !defined(BOARD)
   #error BOARD needs to be defined
#endif

#if !defined(TK_SYSTEM)
   #error System needs to be defined
#endif

#define NO_JMP_WRAPPER
#ifndef NO_JMP_WRAPPER
#include <setjmp.h>

void	longjmp(jmp_buf __jmpb, int __retval){
   asm  ( "JUMP longjmp");
}
int	setjmp(jmp_buf __jmpb){
   asm  ( "JUMP setjmp");
}

#if 0
  asm __volatile__ ( 
      "stw %%sp,%[mystack]"
      : [mystack] "=m" (OUT_SP)
      : /**/
      : "memory"
   );
#endif

#endif


int tk_bsp_sysinit (void){

   {


      #if !defined(__SYS_DEFAULT__)
      #error "Sanity check"
      #endif

      //Hook up the lib (nobody else will)
      #if (TK_SYSTEM==__SYS_DEFAULT__)
/*
         extern void    initialise_monitor_handles _PARAMS ((void));
         initialise_monitor_handles(); 
*/
      #elif (TK_SYSTEM==__SYS_HIXS__)
         extern int     (*hixs_close)        (int file);
         extern void    (*hixs_exit)         (int status);
         extern int     (*hixs_execve)       (char *name, char **argv, char **env);
         extern int     (*hixs_fork)         ();
         extern int     (*hixs_fstat)        (int file, struct stat *st);
         extern int     (*hixs_getpid)       ();
         extern int     (*hixs_gettimeofday) (struct timeval *tp, struct timezone *tzp);
         extern int     (*hixs_isatty)       (int file);
         extern int     (*hixs_kill)         (int pid, int sig);
         extern int     (*hixs_link)         (char *old, char *new);
         extern int     (*hixs_lseek)        (int file, int ptr, int dir);
         extern int     (*hixs_read)         (int file, char *ptr, int len);
         extern caddr_t (*hixs_sbrk)         (int incr);
         extern int     (*hixs_settimeofday) (const struct timeval *tp, const struct timezone *tzp);
         extern int     (*hixs_stat)         (char *file, struct stat *st);
         extern clock_t (*hixs_times)        (struct tms *buf);
         extern int     (*hixs_unlink)       (char *name);
         extern int     (*hixs_wait)         (int *status);
         extern int     (*hixs_write)        (int file, char *ptr, int len);
         extern void    (*hixs_syscall_mon)  (void *);

         hixs_close        = hixs_close;
         hixs_exit         = hixs_exit;
         hixs_execve       = hixs_execve;
         hixs_fork         = hixs_fork;
         hixs_fstat        = hixs_fstat;
         hixs_getpid       = hixs_getpid;
         hixs_gettimeofday = hixs_gettimeofday; 
         hixs_isatty       = hixs_isatty; 
         hixs_kill         = hixs_kill;
         hixs_link         = hixs_link;
         hixs_lseek        = hixs_lseek;
         hixs_read         = hixs_read;
         hixs_sbrk         = hixs_sbrk;
         hixs_settimeofday = hixs_settimeofday;
         hixs_stat         = hixs_stat;
         hixs_times        = hixs_times;           
         hixs_unlink       = hixs_unlink;
         hixs_wait         = hixs_wait;
         hixs_write        = hixs_write;            
         hixs_syscall_mon  = hixs_syscall_mon;     

      #else
         #error "System either not supported or provided"
      #endif

      //Initialize the UART:s for std- in/out/err
/*
      uc0.port = 0;
      uc0.LCR = LCR_WLS_8BIT | LCR_SBS_1BIT | LCR_PE_0 | LCR_PS_ODD | LCR_BK_0 | LCR_DLAB_1;
      uc0.baudrate = 115200;
      uart_polled_init(&uc0);

      uc1.port = 1;
      uc1.LCR = LCR_WLS_8BIT | LCR_SBS_1BIT | LCR_PE_0 | LCR_PS_ODD | LCR_BK_0 | LCR_DLAB_1;
      uc1.baudrate = 115200;
      uart_polled_init(&uc1);
*/
   }

   {  // Set up system timer
/*
      vic_control vc;

      //Showing our ISR intentions
      vc.func       = systimer_Handler;
      vc.vecaddr    = VICVectAddr7;              
      vc.vecchannel = VIC_CH_TIMER0;

      systimer_init(&vc);

*/
   }

}
