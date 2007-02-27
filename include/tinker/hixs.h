/***************************************************************************
 *   Copyright (C) 2006 by Michael Ambrus                                  *
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
#ifndef HIXS_H
#define HIXS_H

#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>

#include <reent.h>
#include <stdio.h>
#include <sys/times.h>
#include <time.h>
#include <stdarg.h>

#if defined (__tinker__)
#	include <tinker/hixs.h>
#else
#	include "hixs.h"
#endif




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

//Special function to monitor syscalls. Even this can be set in userspace to
//provide abilities to set breakpoint without having the full newlib sources.
//Function will be called by all stubbed syscalls and input argument is the 
//stubbed functions adress. 
#if !defined( _NO_HIXS_SYSCALLMON_ )
DEF_FUN_HIXS(void,    syscall_mon,  (void *));
#endif


struct hixs_t{
	pHIXS_close        close;
	pHIXS_exit         exit;
	pHIXS_execve       execve;
	pHIXS_fcntl        fcntl;
	pHIXS_fork         fork;
	pHIXS_fstat        fstat;
	pHIXS_getpid       getpid;
	pHIXS_gettimeofday gettimeofday;
	pHIXS_isatty       isatty;
	pHIXS_kill         kill;
	pHIXS_link         link;
	pHIXS_lseek        lseek;
	pHIXS_open         open;
	pHIXS_read         read;
	pHIXS_sbrk         sbrk;
	pHIXS_settimeofday settimeofday;
	pHIXS_stat         stat;
	pHIXS_times        times;
	pHIXS_unlink       unlink;
	pHIXS_wait         wait;
	pHIXS_write        write;
	#if !defined( _NO_HIXS_SYSCALLMON_ )
	pHIXS_syscall_mon  syscall_mon;
	#endif
};

#endif // HIXS_H

