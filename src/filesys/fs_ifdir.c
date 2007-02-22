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


int fs_ifdir_close(int file) {
	return -1;
}

int fs_ifdir_fcntl (int file, int command, ...){
	errno = ENOSYS;
	return -1;
}
	
	
int fs_ifdir_fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}
	
int fs_ifdir_isatty(int file) {
	return 1;
}
		
int fs_ifdir_link(char *old, char *new) {
	errno=EMLINK;
	return -1;
}
	
int fs_ifdir_lseek(int file, int ptr, int dir) {
	return 0;
}

int fs_ifdir_open(const char *filename, int flags, ...){
	errno = ENOSYS;
	return -1;
}
	
int fs_ifdir_read(int file, char *ptr, int len) {
	return 0;
}
		
int fs_ifdir_stat(const char *file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}
		
int fs_ifdir_unlink(char *name) {
	errno=ENOENT;
	return -1;
}
	
int fs_ifdir_write(int file, char *ptr, int len) {
	return len;
}
