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
#ifndef FILESYS_H
#define FILESYS_H

#include <assert.h>
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <tinker/hixs.h>
#include <malloc.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <filesys/inode.h>

#ifndef assure
#define assure(x) assert(x)
#endif

#define CHECK_FH_NOASSURE(hndl,fnk)				\
	if (							\
		((int)hndl 			== -1)   ||	\
		(hndl 				== NULL) ||	\
		(hndl->inode 			== NULL) ||	\
		(hndl->inode->iohandle		== NULL) ||	\
		(hndl->inode->iohandle->fnk	== NULL)	\
	){							\
		errno=EBADF;					\
		return -1;					\
	}							\

#define CHECK_FH_ASSURE(hndl,fnk)				\
	CHECK_FH_NOASSURE(hndl,fnk)				\
	assure(hndl);						\
	assure(hndl->inode);					\
	assure(hndl->inode->iohandle);				\
	assure(hndl->inode->iohandle->fnk);

#define CHECK_FH CHECK_FH_ASSURE
//#define CHECK_FH CHECK_FH_NOASSURE

/*!
Pure flag names (to aid debugging - note that any flag variable can be OR'ed
among these - which means any stabs info will not be able to resolve the
name (which however is of of concern for the running code).
*/
/*
typedef enum{
	ISA_FLG_UNKNWN 	= 0,
	ISA_RDONLY 	= O_RDONLY,
	ISA_WRONLY	= O_WRONLY,
	ISA_RDWR	= O_RDWR,
	ISA_APPEND	= O_APPEND,
	ISA_CREAT	= O_CREAT,
//	ISA_DSYNC	= O_DSYNC,
	ISA_EXCL	= O_EXCL,
	ISA_NOCTTY	= O_NOCTTY,
	ISA_NONBLOCK	= O_NONBLOCK,
//	ISA_RSYNC	= O_RSYNC,
	ISA_SYNC	= O_SYNC,
	ISA_TRUNC	= O_TRUNC
}tk_flag_t;
*/

/*!oflags as struct - used for bebug purposes*/
typedef struct {
	int _O_RDONLY;
	int _O_WRONLY;
	int _O_RDWR;
	int _O_APPEND;
	int _O_CREAT;
//      int _O_DSYNC;
	int _O_EXCL;
	int _O_NOCTTY;
	int _O_NONBLOCK;
//      int _O_RSYNC;
	int _O_SYNC;
	int _O_TRUNC;
} _tk_dbgflag_t;

//! File handle type - this is the top-level type used as a file handle
typedef struct {
	tk_id_t id;		//!< Unique ID (global counter)
//      tk_id_t         lid;            //!< Local ID  (local counter)
	tk_inode_t *inode;	//!< Which inode this file-handle belongs to
	int oflag;		//!< The flags for this node
	void *data;		//!< Optional data associated with a handle
} tk_fhandle_t;

/*!
Helper function to create a file handle

An IO device "open" function would normally use this
*/
tk_fhandle_t *tk_new_handle(tk_inode_t * inode, int oflag);

/*!
Helper function to destroy a file handle

An IO devive "close" function would normally use this
*/
int tk_free_handle(tk_fhandle_t *);

/*!
Helper function to unpack the operation flags (i.e. those set by open)

If un unknown flag is set, the function will return a non-zero value
else zero will be returned
*/
int tk_dbg_flags(_tk_dbgflag_t * flags, int oflag);

/*!
Main init function for the filesys component.

This function will pick up drivers init functions which creates each driver
*/
int fs_init(void);

/*!
Main fini function for the filesys component

This function will pick up drivers fini functions which destroys each driver
*/
int fs_fini(void);

/*!
Init/fini function prototype. Any ini/fini functions must follow this
prototype.

Both input argument and return value are implementation specific and depends
on the driver. Each driver must however respect the following rules.

- If function succeeds, a non NULL value must be returned. If the return
  contains data that is not a textstring, the first element must be '\0'.
- Even if the driver expects an input argument, it must also accept (and detect)
  NULL. NULL is passed by the fs_init main startup and this can't know what each
  driver expects as an input argument. If the argument is null, the driver
  should instansiate a default IO.

Normally the input argument is either NULL a string describing a file-name.

Normally the output is a informative textstring that can be passed to printf.

*/
typedef void *__drv_finit_f(void *);

/*!
Init/fini function pointer (same is used for both)

@see __drv_finit_f
*/
typedef __drv_finit_f *drv_finit_t;

// Main system calls

int fs_close(int file);
int fs_fcntl(int files, int command, ...);
int fs_fstat(int file, struct stat *st);
int fs_isatty(int file);
int fs_link(char *old, char *new);
int fs_lseek(int file, int ptr, int dir);
int fs_open(const char *filename, int flags, ...);
int fs_read(int file, char *ptr, int len);
int fs_stat(const char *file, struct stat *st);
int fs_unlink(char *name);
int fs_write(int file, char *ptr, int len);

// Subcathegorized system calls - one for each of type of inode:
int fs_ifdir_close(int file);
int fs_ifdir_fcntl(int file, int command, ...);
int fs_ifdir_fstat(int file, struct stat *st);
int fs_ifdir_isatty(int file);
int fs_ifdir_link(char *old, char *new);
int fs_ifdir_lseek(int file, int ptr, int dir);
int fs_ifdir_open(const char *filename, int flags, ...);
int fs_ifdir_read(int file, char *ptr, int len);
int fs_ifdir_stat(const char *file, struct stat *st);
int fs_ifdir_unlink(char *name);
int fs_ifdir_write(int file, char *ptr, int len);

int fs_ifchr_close(int file);
int fs_ifchr_fcntl(int file, int command, ...);
int fs_ifchr_fstat(int file, struct stat *st);
int fs_ifchr_isatty(int file);
int fs_ifchr_link(char *old, char *new);
int fs_ifchr_lseek(int file, int ptr, int dir);
int fs_ifchr_open(const char *filename, int flags, ...);
int fs_ifchr_read(int file, char *ptr, int len);
int fs_ifchr_stat(const char *file, struct stat *st);
int fs_ifchr_unlink(char *name);
int fs_ifchr_write(int file, char *ptr, int len);

int fs_ifblk_close(int file);
int fs_ifblk_fcntl(int file, int command, ...);
int fs_ifblk_fstat(int file, struct stat *st);
int fs_ifblk_isatty(int file);
int fs_ifblk_link(char *old, char *new);
int fs_ifblk_lseek(int file, int ptr, int dir);
int fs_ifblk_open(const char *filename, int flags, ...);
int fs_ifblk_read(int file, char *ptr, int len);
int fs_ifblk_stat(const char *file, struct stat *st);
int fs_ifblk_unlink(char *name);
int fs_ifblk_write(int file, char *ptr, int len);

int fs_ifreg_close(int file);
int fs_ifreg_fcntl(int file, int command, ...);
int fs_ifreg_fstat(int file, struct stat *st);
int fs_ifreg_isatty(int file);
int fs_ifreg_link(char *old, char *new);
int fs_ifreg_lseek(int file, int ptr, int dir);
int fs_ifreg_open(const char *filename, int flags, ...);
int fs_ifreg_read(int file, char *ptr, int len);
int fs_ifreg_stat(const char *file, struct stat *st);
int fs_ifreg_unlink(char *name);
int fs_ifreg_write(int file, char *ptr, int len);

#define RAMDISK_KEEP_OLD_DATA (1<<0)
int fs_ifreg_init(char *, char *, int, int, int, int);
int fs_ifreg_fini(int);

int fs_iflnk_close(int file);
int fs_iflnk_fcntl(int file, int command, ...);
int fs_iflnk_fstat(int file, struct stat *st);
int fs_iflnk_isatty(int file);
int fs_iflnk_link(char *old, char *new);
int fs_iflnk_lseek(int file, int ptr, int dir);
int fs_iflnk_open(const char *filename, int flags, ...);
int fs_iflnk_read(int file, char *ptr, int len);
int fs_iflnk_stat(const char *file, struct stat *st);
int fs_iflnk_unlink(char *name);
int fs_iflnk_write(int file, char *ptr, int len);

int fs_sock_close(int file);
int fs_sock_fcntl(int file, int command, ...);
int fs_sock_fstat(int file, struct stat *st);
int fs_sock_isatty(int file);
int fs_sock_link(char *old, char *new);
int fs_sock_lseek(int file, int ptr, int dir);
int fs_sock_open(const char *filename, int flags, ...);
int fs_sock_read(int file, char *ptr, int len);
int fs_sock_stat(const char *file, struct stat *st);
int fs_sock_unlink(char *name);
int fs_sock_write(int file, char *ptr, int len);

int fs_ififo_close(int file);
int fs_ififo_fcntl(int file, int command, ...);
int fs_ififo_fstat(int file, struct stat *st);
int fs_ififo_isatty(int file);
int fs_ififo_link(char *old, char *new);
int fs_ififo_lseek(int file, int ptr, int dir);
int fs_ififo_open(const char *filename, int flags, ...);
int fs_ififo_read(int file, char *ptr, int len);
int fs_ififo_stat(const char *file, struct stat *st);
int fs_ififo_unlink(char *name);
int fs_ififo_write(int file, char *ptr, int len);

#endif				// FILESYS_H

/*
Subcathegorized system calls - one for each of type of inode:
ifdir
ifchr
ifblk
ifreg
iflnk
ifsock
ififo

NOTE Not all of these make sense for each type of inode.
In case a stubbed dummy is but in its place so that we avoid
calling zero funtion pointes by misstake.

fs_ifdir.c
fs_ifchr.c
fs_ifblk.c
fs_ifreg.c
fs_iflnk.c
fs_ifsock.c
fs_ififo.c

*/
