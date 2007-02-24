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

#ifndef assure
#define assure(x) assert(x)
#endif

int fs_init();
int fs_fini();

/*Main system calls*/

int fs_close(int file);
int fs_fcntl (int files, int command, ...);
int fs_fstat(int file, struct stat *st);
int fs_isatty(int file);
int fs_link(char *old, char *new);
int fs_lseek(int file, int ptr, int dir);
int fs_open(const char *filename, int flags, ...);
int fs_read(int file, char *ptr, int len);
int fs_stat(const char *file, struct stat *st);
int fs_unlink(char *name);
int fs_write(int file, char *ptr, int len);


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


int fs_ifdir_close(int file) ;
int fs_ifdir_fcntl (int file, int command, ...);
int fs_ifdir_fstat(int file, struct stat *st) ;
int fs_ifdir_isatty(int file) ;
int fs_ifdir_link(char *old, char *new) ;
int fs_ifdir_lseek(int file, int ptr, int dir) ;
int fs_ifdir_open(const char *filename, int flags, ...);
int fs_ifdir_read(int file, char *ptr, int len) ;
int fs_ifdir_stat(const char *file, struct stat *st) ;
int fs_ifdir_unlink(char *name) ;
int fs_ifdir_write(int file, char *ptr, int len) ;

int fs_ifchr_close(int file) ;
int fs_ifchr_fcntl (int file, int command, ...);
int fs_ifchr_fstat(int file, struct stat *st) ;
int fs_ifchr_isatty(int file) ;
int fs_ifchr_link(char *old, char *new) ;
int fs_ifchr_lseek(int file, int ptr, int dir) ;
int fs_ifchr_open(const char *filename, int flags, ...);
int fs_ifchr_read(int file, char *ptr, int len) ;
int fs_ifchr_stat(const char *file, struct stat *st) ;
int fs_ifchr_unlink(char *name) ;
int fs_ifchr_write(int file, char *ptr, int len) ;

int fs_ifblk_close(int file) ;
int fs_ifblk_fcntl (int file, int command, ...);
int fs_ifblk_fstat(int file, struct stat *st) ;
int fs_ifblk_isatty(int file) ;
int fs_ifblk_link(char *old, char *new) ;
int fs_ifblk_lseek(int file, int ptr, int dir) ;
int fs_ifblk_open(const char *filename, int flags, ...);
int fs_ifblk_read(int file, char *ptr, int len) ;
int fs_ifblk_stat(const char *file, struct stat *st) ;
int fs_ifblk_unlink(char *name) ;
int fs_ifblk_write(int file, char *ptr, int len) ;

int fs_ifreg_close(int file) ;
int fs_ifreg_fcntl (int file, int command, ...);
int fs_ifreg_fstat(int file, struct stat *st) ;
int fs_ifreg_isatty(int file) ;
int fs_ifreg_link(char *old, char *new) ;
int fs_ifreg_lseek(int file, int ptr, int dir) ;
int fs_ifreg_open(const char *filename, int flags, ...);
int fs_ifreg_read(int file, char *ptr, int len) ;
int fs_ifreg_stat(const char *file, struct stat *st) ;
int fs_ifreg_unlink(char *name) ;
int fs_ifreg_write(int file, char *ptr, int len) ;

int fs_iflnk_close(int file) ;
int fs_iflnk_fcntl (int file, int command, ...);
int fs_iflnk_fstat(int file, struct stat *st) ;
int fs_iflnk_isatty(int file) ;
int fs_iflnk_link(char *old, char *new) ;
int fs_iflnk_lseek(int file, int ptr, int dir) ;
int fs_iflnk_open(const char *filename, int flags, ...);
int fs_iflnk_read(int file, char *ptr, int len) ;
int fs_iflnk_stat(const char *file, struct stat *st) ;
int fs_iflnk_unlink(char *name) ;
int fs_iflnk_write(int file, char *ptr, int len) ;

int fs_sock_close(int file) ;
int fs_sock_fcntl (int file, int command, ...);
int fs_sock_fstat(int file, struct stat *st) ;
int fs_sock_isatty(int file) ;
int fs_sock_link(char *old, char *new) ;
int fs_sock_lseek(int file, int ptr, int dir) ;
int fs_sock_open(const char *filename, int flags, ...);
int fs_sock_read(int file, char *ptr, int len) ;
int fs_sock_stat(const char *file, struct stat *st) ;
int fs_sock_unlink(char *name) ;
int fs_sock_write(int file, char *ptr, int len) ;

int fs_ififo_close(int file) ;
int fs_ififo_fcntl (int file, int command, ...);
int fs_ififo_fstat(int file, struct stat *st) ;
int fs_ififo_isatty(int file) ;
int fs_ififo_link(char *old, char *new) ;
int fs_ififo_lseek(int file, int ptr, int dir) ;
int fs_ififo_open(const char *filename, int flags, ...);
int fs_ififo_read(int file, char *ptr, int len) ;
int fs_ififo_stat(const char *file, struct stat *st) ;
int fs_ififo_unlink(char *name) ;
int fs_ififo_write(int file, char *ptr, int len) ;

#endif // FILESYS_H

