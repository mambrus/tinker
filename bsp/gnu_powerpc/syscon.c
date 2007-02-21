/***************************************************************************
 *   Copyright (C) 2007 by Michael Ambrus                                  *
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
/*
This module serves as an initial system connector for PowerPC derivates. 
Other submodules can optionally seconfigure the syscall map in their
initial startup routines respectivly
*/

#include <tinker/config.h>


//#include "config.h"
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>

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

#if !defined(__SYS_DEFAULT__)
#error "Sanity chec failk"
#endif

#if (TK_SYSTEM == __SYS_HIXS__)
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
#else
	#error "System either not supported or provided"
#endif



int ppc_write (int file, char *ptr, int len){
	hixs_syscall_mon(ppc_write);
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
   hixs_syscall_mon(ppc_sbrk);
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


clock_t ppc_times(struct tms *buf){
	hixs_syscall_mon(ppc_times);
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
   hixs_syscall_mon(ppc_open);
   errno = ENOSYS;
   return -1;
}


int ppc_fcntl (int filedes, int command, ...){
   hixs_syscall_mon(ppc_fcntl);
   errno = ENOSYS;
   return -1;
}

void (*used_syscall)(void);
void ppc_syscall_mon(void *hix_syscall){
	used_syscall = hix_syscall;
}

int tk_bsp_sysinit (void){
	//Hook up the lib (nobody else will) 
	//They allready have pre-set values by the linker so we need
	//to activly re-assign them

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
}
