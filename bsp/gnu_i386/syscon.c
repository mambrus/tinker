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
#include <tinker/hixs.h>

#if !defined(BOARD)
#error BOARD needs to be defined
#endif

#if !defined(TK_SYSTEM)
#error System needs to be defined
#endif

#if !defined(__SYS_DEFAULT__)
#error "Sanity chec failk"
#endif

#if (TK_SYSTEM == __SYS_HIXS__)
extern struct hixs_t hixs;
#else
#error "System either not supported or provided"
#endif

int x86_write(int file, char *ptr, int len)
{
	console_write(ptr, len);	// Ignore the file ID - write all on console regardless
	if (ptr[len - 1] == '\n')
		console_write("\r", 1);	// Add carrige return (hack, migh be avoided by tuning istty)
	return len;		/*Wrote it all (say...) */
};

//=====================================
char *myheap_end = (char *)0x10080000;	//<- NOTE: Test purpose only, dont use this unless you know what you're doing
//=====================================
caddr_t x86_sbrk(int incr)
{
	extern char *myheap_end;
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

/*A simple up-counter to simulate a real clock*/
clock_t x86_clock()
{
	static __uint32_t loop;
	loop += 10000;
	return loop;
}

clock_t x86_times(struct tms * buf)
{
	clock_t upTime = x86_clock();
	if (buf != NULL) {
		(*buf).tms_utime = upTime;
		buf->tms_stime = 0;
		buf->tms_cutime = 0;
		buf->tms_cstime = 0;
	}
	return upTime;
}

int x86_open(const char *filename, int flags, ...)
{
	errno = ENOSYS;
	return -1;
}

int x86_fcntl(int filedes, int command, ...)
{
	errno = ENOSYS;
	return -1;
}

void (*used_syscall) (void);
void x86_syscall_mon(void *hix_syscall)
{
	used_syscall = hix_syscall;
}

int tk_bsp_sysinit(void)
{
	//Hook up the lib (nobody else will)
	//They allready have pre-set values by the linker so we need
	//to activly re-assign them

	hixs.close = hixs.close;
	hixs.exit = hixs.exit;
	hixs.execve = hixs.execve;
	hixs.fcntl = x86_fcntl;	// <- NOTE
	hixs.fork = hixs.fork;
	hixs.fstat = hixs.fstat;
	hixs.getpid = hixs.getpid;
	hixs.gettimeofday = hixs.gettimeofday;
	hixs.isatty = hixs.isatty;
	hixs.kill = hixs.kill;
	hixs.link = hixs.link;
	hixs.lseek = hixs.lseek;
	hixs.open = x86_open;	// <- NOTE
	hixs.read = hixs.read;
	//hixs.sbrk         = x86_sbrk;         // <- NOTE use built in -it's OK
	hixs.settimeofday = hixs.settimeofday;
	hixs.stat = hixs.stat;
	hixs.times = x86_times;	// <- NOTE
	hixs.unlink = hixs.unlink;
	hixs.wait = hixs.wait;
	hixs.write = x86_write;	// <- NOTE
	hixs.syscall_mon = x86_syscall_mon;	// <- NOTE
}
