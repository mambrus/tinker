#include <tinker/config.h>


//#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
//#undef errno
//extern int errno;
//#include "warning.h"

/*
#include <asm/bits.h>
#include <asm/msr.h>
#include <asm/cm.h>
#include <asm/plprcr.h>
#include <asm/ictrl.h>
#include <asm/sccr.h>
#include <mmap_regs.h>
#include <console.h>
#include <isr.h>
*/


#include <reent.h>
#include <stdio.h>
#include <sys/times.h>
#include <time.h>


#if !defined(BOARD)
   #error BOARD needs to be defined
#endif

#if !defined(TK_SYSTEM)
   #error System needs to be defined
#endif

// System ID definitions
#define __SYS_DEFAULT__    0
#define __SYS_HIXS__       1


int ppc_write (int file, char *ptr, int len){
	if ((__uint32_t)ptr > 0x103ff000 )
		return 0;
	console_write(ptr, len);	// Ignore the file ID - write all on console regardless
	if (ptr[len-1]=='\n')
		console_write("\r", 1);	// Add carrige return (hack, migh be avoided by tuning istty)
	return len; /*Wrote it all (say...)*/
};

//=====================================
char* myheap_end = (char*)0x10080000;   //<- NOTE: Test purpose only, dont use this unless you know what you're doing
//=====================================
caddr_t ppc_sbrk(int incr) {
   extern char* myheap_end;
   /* Defined by the linker. */
   static char *heap_end;
   char *prev_heap_end;

   if (heap_end == 0) {
      heap_end = myheap_end;
   }

   prev_heap_end = heap_end;

   heap_end += incr;   
   return (caddr_t) prev_heap_end;   
}


/*According to POSIX, CLOCKS_PER_SEC is a fixed value */
#ifdef CLOCKS_PER_SEC
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000000 
#endif

#if MICKEYS_PER_SEC > CLOCKS_PER_SEC
#define CRATIO  (MICKEYS_PER_SEC/CLOCKS_PER_SEC)
#else
#define MRATIO  (CLOCKS_PER_SEC/MICKEYS_PER_SEC)
#endif

clock_t ppc_clock           (){
	unsigned int TmickS_low  = __sys_mickey;           //Low  32 bit worth value (in board time-base)
	long long    TmickS_high = __sys_mackey;           //High ditto

	#ifdef CRATIO	
	long long Tcl = ((TmickS_high << 32) + TmickS_low ) / CRATIO;
	#else
	long long Tcl = ((TmickS_high << 32) + TmickS_low ) * MRATIO;
	#endif
	
	return (clock_t)Tcl; //Possibly trunc it... (best we can do anyway)	
}

clock_t ppc_times(struct tms *buf){
	clock_t upTime = ppc_clock();
	if (buf!=NULL){
		(*buf).tms_utime  = upTime;
		buf->tms_stime  = 0;
		buf->tms_cutime = 0;
		buf->tms_cstime = 0;
	}
	return upTime;
}

int ppc_open (const char *filename, int flags, ...){
   errno = ENOSYS;
   return -1;
}


int ppc_fcntl (int filedes, int command, ...){
   errno = ENOSYS;
   return -1;
}

void (*used_syscall)(void);
void ppc_syscall_mon(void *hix_syscall){
	used_syscall = hix_syscall;
}


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
		extern int     (*hixs_fcntl)        (int filedes, int command, ...);
		extern int     (*hixs_fork)         ();
		extern int     (*hixs_fstat)        (int file, struct stat *st);
		extern int     (*hixs_getpid)       ();
		extern int     (*hixs_gettimeofday) (struct timeval *tp, struct timezone *tzp);
		extern int     (*hixs_isatty)       (int file);
		extern int     (*hixs_kill)         (int pid, int sig);
		extern int     (*hixs_link)         (char *old, char *new);
		extern int     (*hixs_lseek)        (int file, int ptr, int dir);
		extern int     (*hixs_open)         (const char *filename, int flags, ...);
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
		hixs_fcntl        = ppc_fcntl;		// <- NOTE
		hixs_fork         = hixs_fork;
		hixs_fstat        = hixs_fstat;
		hixs_getpid       = hixs_getpid;
		hixs_gettimeofday = hixs_gettimeofday; 
		hixs_isatty       = hixs_isatty; 
		hixs_kill         = hixs_kill;
		hixs_link         = hixs_link;
		hixs_lseek        = hixs_lseek;
		hixs_open         = ppc_open;		// <- NOTE
		hixs_read         = hixs_read;
		//hixs_sbrk         = ppc_sbrk;		// <- NOTE use built in -it's OK
		hixs_settimeofday = hixs_settimeofday;
		hixs_stat         = hixs_stat;
		hixs_times        = ppc_times;		// <- NOTE
		hixs_unlink       = hixs_unlink;
		hixs_wait         = hixs_wait;
		hixs_write        = ppc_write;		// <- NOTE
		hixs_syscall_mon  = ppc_syscall_mon;	// <- NOTE

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
