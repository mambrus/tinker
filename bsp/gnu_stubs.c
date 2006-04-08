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

#include <errno.h>

/**
_exit
The _exit function is the primitive used for process termination by exit. It 
is declared in the header file unistd.h. 

The _exit function is the primitive for causing a process to terminate with 
status status. Calling this function does not execute cleanup functions 
registered with atexit or on_exit.  

Exits a program without cleaning up files. If your system doesn’t provide this
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
int close(int file){
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
int execve(char *name, char **argv, char **env){
   errno=ENOMEM;
   return -1;
}


/**
fork
Create a new process. Minimal implementation (for a system without processes)
is shown in the following example.
*/
int fork() {
   errno=EAGAIN;
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
int fstat(int file, struct stat *st) {
   st->st_mode = S_IFCHR;
   return 0;
}

/**
getpid
Process-ID; this is sometimes used to generate strings unlikely to conflict with
other processes. Minimal implementation, for a system without processes is
shown in the following example.
*/

int getpid() {
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
int kill(int pid, int sig){
errno=EINVAL;
   return(-1);
}

/**
link
Establish a new name for an existing file. Minimal implementation is shown in the
following example.
*/
int link(char *old, char *new){
   errno=EMLINK;
   return -1;
}

/**
lseek
Set position in a file. Minimal implementation is shown in the following example.
*/
int lseek(int file, int ptr, int dir){
   return 0;
}

/**
read
Read from a file. Minimal implementation is shown in the following example.
*/
int read(int file, char *ptr, int len){
   return 0;
}

/**
sbrk
Increase program data space. As malloc and related functions depend on this, it is
useful to have a working implementation. The following suffices for a standalone
system; it exploits the symbol, end, automatically defined by the GNU linker, ld.
*/
caddr_t sbrk(int incr){
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
int times(struct tms *buf){
   return -1;
}

/**
unlink
Remove a file’s directory entry. Minimal implementation is shown in the
following example.
*/
int unlink(char *name){
   errno=ENOENT;
   return -1;
}

/**
wait
Wait for a child process. Minimal implementation is shown in the following
example.
*/
int wait(int *status) {
   errno=ECHILD;
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
int write(int file, char *ptr, int len){
   int todo;
	for (todo = 0; todo < len; todo++) {
	   //writechar(*ptr++);
	}
	return len;
}

/*! 
 * @defgroup CVSLOG_gnu_stubs_c gnu_stubs_c
 * @ingroup CVSLOG
 *
 *  $Log: gnu_stubs.c,v $
 *  Revision 1.2  2006-04-08 10:15:50  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.1.2.5  2006/04/08 09:59:00  ambrmi09
 *  In preparation to join with main trunk.
 *
 *  Revision 1.1.2.4  2006/04/07 12:10:05  ambrmi09
 *  Skeleton for handling syscalls using the ARM Angel interface in place
 *
 *  Basic terminal I/O for gnu_arm (LPC2129) - only output so far (input requires
 *  blocking).
 *
 *  Revision 1.1.2.3  2006/04/06 09:01:55  ambrmi09
 *  Safety commit due to change of local sandbox FS type (had files checked out
 *  while changing the type - not to be recommended).
 *
 *  Revision 1.1.2.2  2006/04/03 20:07:20  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.1.2.1  2006/04/03 15:21:43  ambrmi09
 *  All targets updated with the new thread-starter (alternative 2).
 *
 *  This alternative has one weakness (explained elsewhere togeather
 *  with alternative 1), but so far it's the only one that will compile
 *  and function equally among all 4 (very different) compilers currently
 *  tested against: GCC, MSVC, BC5 and Keil.
 *
 *  If nothing else turns up, I'm willing to overcome the drawback (it's
 *  quite handleable) because it *truly* takes away a lot of pain with
 *  porting.
 *
 *  The ARM port (architecture level) is than's to this now fully operational
 *  without the r13 hack in the context switch. This includes thread
 *  cancellation and thread argument passing (which were not functioning in
 *  the old port).
 *
 *  If this revised code proves itself (i.e. no surprises turns up) then
 *  TinKer can be considered "almost ported" to any HW target that GCC is
 *  ported for :D (/cheers)
 *
 */
 
 