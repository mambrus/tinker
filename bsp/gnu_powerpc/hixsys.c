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
This module serves as a replacement for hixs crt0.o so that you might omitt 
it - or replace it with your own with potentially different context.
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
#include <stdarg.h>


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
#error "Sanity check"
#endif

//Hook up the lib (nobody else will)
#if (TK_SYSTEM==__SYS_DEFAULT__)
#	error - when not usung HIXS, this module will provide duplicates of syscall (this is not what you want)
#elif (TK_SYSTEM==__SYS_HIXS__)

	/* Function call definitions, type declaration and storage */
	#define DEF_FUN_HIXS( v, x, y )    \
	typedef v _HIXS_##x y;          \
	typedef _HIXS_##x * pHIXS_##x;  \
	v HIXS_##x y;
	
	DEF_FUN_HIXS(int,     close,        (int file));
	DEF_FUN_HIXS(void,    exit,         (int status));
	DEF_FUN_HIXS(int,     execve,       (char *name, char **argv, char **env));
	DEF_FUN_HIXS(int,     fcntl,        (int filedes, int command, ...));
	DEF_FUN_HIXS(int,     fork,         ());
	DEF_FUN_HIXS(int,     fstat,        (int file, struct stat *st));
	DEF_FUN_HIXS(int,     getpid,       ());
	DEF_FUN_HIXS(int,     gettimeofday, (struct timeval *tp, struct timezone *tzp));
	DEF_FUN_HIXS(int,     isatty,       (int file));
	DEF_FUN_HIXS(int,     kill,         (int pid, int sig));
	DEF_FUN_HIXS(int,     link,         (char *old, char *new));
	DEF_FUN_HIXS(int,     lseek,        (int file, int ptr, int dir));
	DEF_FUN_HIXS(int,     open,         (const char *filename, int flags, ...));
	DEF_FUN_HIXS(int,     read,         (int file, char *ptr, int len));
	DEF_FUN_HIXS(caddr_t, sbrk,         (int incr));
	DEF_FUN_HIXS(int,     settimeofday, (const struct timeval *tp, const struct timezone *tzp))
	DEF_FUN_HIXS(int,     stat,         (const char *file, struct stat *st));
	DEF_FUN_HIXS(clock_t, times,        (struct tms *buf));
	DEF_FUN_HIXS(int,     unlink,       (char *name));
	DEF_FUN_HIXS(int,     wait,         (int *status));
	DEF_FUN_HIXS(int,     write,        (int file, char *ptr, int len));
	#if !defined( _NO_HIXS_SYSCALLMON_ )
	DEF_FUN_HIXS(void,    syscall_mon,  (void *));
	#endif

/*Connect them to the stibs we know exists stubs - 
(will go in .sbss section, i.e. initialized static but R/W data)*/
	
	pHIXS_close        hixs_close        = HIXS_close;
	pHIXS_exit         hixs_exit         = HIXS_exit;
	pHIXS_execve       hixs_execve       = HIXS_execve;
	pHIXS_fcntl        hixs_fcntl        = HIXS_fcntl;
	pHIXS_fork         hixs_fork         = HIXS_fork;
	pHIXS_fstat        hixs_fstat        = HIXS_fstat;
	pHIXS_getpid       hixs_getpid       = HIXS_getpid;
	pHIXS_gettimeofday hixs_gettimeofday = HIXS_gettimeofday;
	pHIXS_isatty       hixs_isatty       = HIXS_isatty;
	pHIXS_kill         hixs_kill         = HIXS_kill;
	pHIXS_link         hixs_link         = HIXS_link;
	pHIXS_lseek        hixs_lseek        = HIXS_lseek;
	pHIXS_open         hixs_open         = HIXS_open;
	pHIXS_read         hixs_read         = HIXS_read;
	pHIXS_sbrk         hixs_sbrk         = HIXS_sbrk;
	pHIXS_settimeofday hixs_settimeofday = HIXS_settimeofday;
	pHIXS_stat         hixs_stat         = HIXS_stat;
	pHIXS_times        hixs_times        = HIXS_times;
	pHIXS_unlink       hixs_unlink       = HIXS_unlink;
	pHIXS_wait         hixs_wait         = HIXS_wait;
	pHIXS_write        hixs_write        = HIXS_write;
	#if !defined( _NO_HIXS_SYSCALLMON_ )
	pHIXS_syscall_mon  hixs_syscall_mon  = HIXS_syscall_mon;
	#endif

#if defined __cplusplus
extern "C" {
#endif
	int     _close        (int file){ return hixs_close(file);}
	void    _exit         (int status){ return hixs_exit(status);}
	int     _execve       (char *name, char **argv, char **env){ return hixs_execve(name, argv, env);}
	int     _fcntl        (int filedes, int command, ...){
					va_list ap;
					/*any local vars here...*/
					va_start (ap, command);
					return hixs_fcntl(filedes, command, va_arg (ap, int));
				}
	int     _fork         (){ return hixs_fork();}
	int     _fstat        (int file, struct stat *st){ return hixs_fstat(file, st);}
	int     _getpid       (){ return hixs_getpid();}
	int     _gettimeofday (struct timeval *tp, struct timezone *tzp){ return hixs_gettimeofday(tp, tzp);}
	int      isatty       (int file){ return hixs_isatty(file);}
	int     _kill         (int pid, int sig){ return hixs_kill(pid, sig);}
	int     _link         (char *old, char *new){ return hixs_link(old, new);}
	int     _lseek        (int file, int ptr, int dir){ return hixs_lseek(file, ptr, dir);}
	int     _open        (const char *filename, int flags, ...){ 
					va_list ap;
					/*any local vars here...*/
					va_start (ap, flags);
					return hixs_open(filename, flags, va_arg (ap, int));
				}
	int     _read         (int file, char *ptr, int len){ return hixs_read(file, ptr, len);}
	caddr_t _sbrk         (int incr){ return hixs_sbrk(incr);}
	int     _settimeofday (const struct timeval *tp, const struct timezone *tzp){ return hixs_settimeofday(tp, tzp);}
	int     _stat         (const char *file, struct stat *st){ return hixs_stat(file, st);}
	clock_t _times        (struct tms *buf){ return hixs_times(buf);}
	int     _unlink       (char *name){ return hixs_unlink(name);}
	int     _wait         (int *status){ return hixs_wait(status);}
	int     _write        (int file, char *ptr, int len){ return hixs_write(file, ptr, len);}
	
	#if !defined( _NO_HIXS_SYSCALLMON_ )
	void    _syscall_mon  (void *sys_func_ptr){ return hixs_syscall_mon(sys_func_ptr);}
	#endif

#if defined __cplusplus
}
#endif

#endif
