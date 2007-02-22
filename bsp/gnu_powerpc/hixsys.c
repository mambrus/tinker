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

#if defined (__tinker__)
#	include <tinker/hixs.h>
#else
#	include "hixs.h"
#endif

#if !defined( _NO_HIXS_SYSCALLMON_ )
#	define MON(x) hixs.syscall_mon(x)
#else
#	define MON(x)	((void)0)
#endif


struct hixs_t hixs={
	HIXS_close,
	HIXS_exit,
	HIXS_execve,
	HIXS_fcntl,
	HIXS_fork,
	HIXS_fstat,
	HIXS_getpid,
	HIXS_gettimeofday,
	HIXS_isatty,
	HIXS_kill,
	HIXS_link,
	HIXS_lseek,
	HIXS_open,
	HIXS_read,
	HIXS_sbrk,
	HIXS_settimeofday,
	HIXS_stat,
	HIXS_times,
	HIXS_unlink,
	HIXS_wait,
	HIXS_write,
	#if !defined( _NO_HIXS_SYSCALLMON_ )
	HIXS_syscall_mon
	#endif
};

#if defined __cplusplus
extern "C" {
#endif
	int     _close        (int file)					{ MON(hixs.close); return hixs.close(file);}
	void    _exit         (int status)					{ MON(hixs.exit); hixs.exit(status);}
	int     _execve       (char *name, char **argv, char **env)		{ MON(hixs.execve); return hixs.execve(name, argv, env);}
	int     _fcntl        (int filedes, int command, ...){
					va_list ap;
					/*any local vars here...*/
					va_start (ap, command);
					MON(hixs.fcntl); 
					return hixs.fcntl(filedes, command, va_arg (ap, int));
				}
	int     _fork         ()						{ MON(hixs.fork); return hixs.fork();}
	int     _fstat        (int file, struct stat *st)			{ MON(hixs.fstat); return hixs.fstat(file, st);}
	int     _getpid       ()						{ MON(hixs.getpid); return hixs.getpid();}
	int     _gettimeofday (struct timeval *tp, struct timezone *tzp)	{ MON(hixs.gettimeofday); return hixs.gettimeofday(tp, tzp);}
	int      isatty       (int file)					{ MON(hixs.isatty); return hixs.isatty(file);}
	int     _kill         (int pid, int sig)				{ MON(hixs.kill); return hixs.kill(pid, sig);}
	int     _link         (char *old, char *new)				{ MON(hixs.link); return hixs.link(old, new);}
	int     _lseek        (int file, int ptr, int dir)			{ MON(hixs.lseek); return hixs.lseek(file, ptr, dir);}
	int     _open         (const char *filename, int flags, ...){ 
					va_list ap;
					/*any local vars here...*/
					va_start (ap, flags);
					MON(hixs.open); 
					return hixs.open(filename, flags, va_arg (ap, int));
				}
	int     _read         (int file, char *ptr, int len)			{ MON(hixs.read); return hixs.read(file, ptr, len);}
	caddr_t _sbrk         (int incr)					{ MON(hixs.sbrk); return hixs.sbrk(incr);}
	int     _settimeofday (const struct timeval *tp, const struct timezone *tzp){ MON(hixs.settimeofday); return hixs.settimeofday(tp, tzp);}
	int     _stat         (const char *file, struct stat *st)		{ MON(hixs.stat); return hixs.stat(file, st);}
	clock_t _times        (struct tms *buf)					{ MON(hixs.times); return hixs.times(buf);}
	int     _unlink       (char *name)					{ MON(hixs.unlink); return hixs.unlink(name);}
	int     _wait         (int *status)					{ MON(hixs.wait); return hixs.wait(status);}
	int     _write        (int file, char *ptr, int len)			{ MON(hixs.write); return hixs.write(file, ptr, len);}
	
	#if !defined( _NO_HIXS_SYSCALLMON_ )
	void    _syscall_mon  (void *sys_func_ptr){ hixs.syscall_mon(sys_func_ptr);}
	#endif

#if defined __cplusplus
}
#endif
