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

/*!
@file
@brief stubbed low level functions for GNU

This file contains low level stub-functions that GNU libc would require.

These stubs would enable you to build and link any GNU SW, but most
importantly glibc.

If you allready have a working GNU tool-chain, It's quite likely that it
already has the functions in this files stubbed. However, this file is
to be considered a template (and reminder) of which functions to
properly implement (since there are neither less nor more of them).

*/

#ifdef USE_GNU_SYSCALL_STUBS

#include <errno.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>

/* The below code is useless. va_ macros seem to operate on the user stack no matter what the stack pointer is set to */
/* Code is kept (but invalidated ) as a reminder to this problem */

/*
#include <stdio.h>
#include <stdarg.h>
#include <tk.h>
#include <../src/implement_tk.h>

#define MAX_STRINGS 255
#define MAX_STRLEN  255

char mystr_buff[MAX_STRINGS][MAX_STRLEN];
#include <string.h>
char argbuff[MAX_STRLEN];

int cidx = 0;
int lidx = 0;

int bsp_printf(char *formatstr, ...){
	va_list ap;
	int rc,i;
	tk_tcb_t *curr_tcb = _tk_current_tcb();

	va_start(ap, formatstr);
	memcpy(argbuff,ap,255);
	va_end(ap);
	rc = sprintf(mystr_buff[cidx],formatstr,argbuff);
	cidx = (cidx+1) % MAX_STRINGS;

	if ( (curr_tcb->Thid) == 0 ){
		for (i=lidx; lidx != cidx; lidx = (lidx+1) % MAX_STRINGS ){
			printf("%s",mystr_buff[lidx]);
		}
		fflush(stdout);
	}else{
		return 0;
	}
	return rc;
}
*/

//The following is a fix. It seems that GCC (libc?) prior to GCC 3.3.4 doesnt have this
//This might need further investigating
#ifndef caddr_t
#define caddr_t unsigned long
#endif

/**
_exit
The _exit function is the primitive used for process termination by exit. It
is declared in the header file unistd.h.

The _exit function is the primitive for causing a process to terminate with
status status. Calling this function does not execute cleanup functions
registered with atexit or on_exit.

Exits a program without cleaning up files. If your system doesn�t provide this
routine, it is best to avoid linking with subroutines that require it (such as exit or
system).
*/
/*
void _exit (int status){
   while(1);
};
*/

/**
close
Closes a file. Minimal implementation is shown in the following example (in
which file stands for the filename to substitute).
*/
int close(int file)
{
	return -1;
}

/**
@name environ
Points to a list of environment variables and their values. For a minimal
environment, the following empty list is adequate.
*/
//@{
/*
char *__env[1] = { 0 };
char **environ = __env;
*/
//@}

/**
execve
Transfers control to a new process. Minimal implementation (for a system without
processes) is shown in the following example (in which name stands for the
process name to substitute, argv stands for the argument value to subtitute, and
env stands for the environment to substitute).
*/
int execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}

/**
fork
Create a new process. Minimal implementation (for a system without processes)
is shown in the following example.
*/
int fork()
{
	errno = EAGAIN;
	return -1;
}

/**
fstat
Status of an open file. For consistency with other minimal implementations in
these examples, all files are regarded as character special devices.
The sys/stat.h header file required is distributed in the include subdirectory
for this C library.
*/
#include <sys/stat.h>
int fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

/**
getpid
Process-ID; this is sometimes used to generate strings unlikely to conflict with
other processes. Minimal implementation, for a system without processes is
shown in the following example.
*/

int getpid()
{
	return 1;
}

/**
isatty
Query whether output stream is a terminal. For consistency with the other
minimal implementations, which only support output to stdout, the minimal
implementation is shown in the following example.
*/
/*
int isatty(int file){
   return 1;
}
*/

/**
kill
Send a signal. Minimal implementation is shown in the following example.
*/
int kill(int pid, int sig)
{
	errno = EINVAL;
	return (-1);
}

/**
link
Establish a new name for an existing file. Minimal implementation is shown in the
following example.
*/
int link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}

/**
lseek
Set position in a file. Minimal implementation is shown in the following example.
*/
int lseek(int file, int ptr, int dir)
{
	return 0;
}

/**
read
Read from a file. Minimal implementation is shown in the following example.
*/
int read(int file, char *ptr, int len)
{
	return 0;
}

/**
sbrk
Increase program data space. As malloc and related functions depend on this, it is
useful to have a working implementation. The following suffices for a standalone
system; it exploits the symbol, end, automatically defined by the GNU linker, ld.
*/
caddr_t sbrk(int incr)
{
	extern char end;
	/* Defined by the linker. */
	static char *heap_end;
	char *prev_heap_end;
	if (heap_end == 0) {
		heap_end = &end;
	}
	prev_heap_end = heap_end;
/*
   if (heap_end + incr > stack_ptr)
   {
      _write (1, "Heap and stack collision\n", 25);
      abort ();
   }
   heap_end += incr;
   return (caddr_t) prev_heap_end;
*/
}

/**
stat

The stat function returns information about the attributes of the file named
by filename in the structure pointed to by buf.

If filename is the name of a symbolic link, the attributes you get describe
the file that the link points to. If the link points to a nonexistent file
name, then stat fails reporting a nonexistent file.

Status of a file (by name). Minimal implementation is shown in the following
example.
*/
/*
#include <sys/stat.h>
int stat(char *file, struct stat *st) {
   st->st_mode = S_IFCHR;
   return 0;
}
*/
/**
times
Timing information for current process. Minimal implementation is shown in the
following example.
*/
clock_t times(struct tms *buf)
{
	return -1;
}

/**
unlink
Remove a file�s directory entry. Minimal implementation is shown in the
following example.
*/
int unlink(char *name)
{
	errno = ENOENT;
	return -1;
}

/**
wait
Wait for a child process. Minimal implementation is shown in the following
example.
*/
int wait(int *status)
{
	errno = ECHILD;
	return -1;
}

/**
write
Writes a character to a file. libc subroutines can use this system routine for output
to all files, including stdout by first using MISSING_SYSCALL_NAMES with
target_cflags in configure.in . If you need to generate any output (for
instance, to a serial port for debugging), you should make your minimal write
capable of accomplishing this objective. The following minimal implementation
is an incomplete example; it relies on a writechar subroutine to actually perform
the output (a subroutine not provided here since it is usually in assembler form as
examples provided by your hardware manufacturer).
*/
int write(int file, char *ptr, int len)
{
	int todo;
	for (todo = 0; todo < len; todo++) {
		//writechar(*ptr++);
	}
	return len;
}
#endif

