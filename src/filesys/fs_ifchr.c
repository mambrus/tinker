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
#include "filesys.h"
#include "inode.h"

int fs_ifchr_close(int file) {
	_syscall_mon(fs_ifchr_close);
	return -1;
}

int fs_ifchr_fcntl (int file, int command, ...){
	_syscall_mon(fs_ifchr_fcntl);
	errno = ENOSYS;
	return -1;
}
	
	
int fs_ifchr_fstat(int file, struct stat *st) {
	_syscall_mon(fs_ifchr_fstat);	
	st->st_mode = S_IFCHR;
	return 0;
}
	
int fs_ifchr_isatty(int file) {
	_syscall_mon(fs_ifchr_isatty);	
	return 1;
}
		
int fs_ifchr_link(char *old, char *new) {
	_syscall_mon(fs_ifchr_link);	
	errno=EMLINK;
	return -1;
}
	
int fs_ifchr_lseek(int file, int ptr, int dir) {
	_syscall_mon(fs_ifchr_lseek);	
	return 0;
}

int fs_ifchr_open(const char *filename, int flags, ...){
	_syscall_mon(fs_ifchr_open);	
	errno = ENOSYS;
	return -1;
}
	
int fs_ifchr_read(int file, char *ptr, int len) {
	_syscall_mon(fs_ifchr_read);	
	return 0;
}
		
int fs_ifchr_stat(const char *file, struct stat *st) {
	_syscall_mon(fs_ifchr_stat);
	st->st_mode = S_IFCHR;
	return 0;
}
		
int fs_ifchr_unlink(char *name) {
	_syscall_mon(fs_ifchr_unlink);
	errno=ENOENT;
	return -1;
}
	
int fs_ifchr_write(int file, char *ptr, int len) {
	_syscall_mon(fs_ifchr_write);
	return len;
}
