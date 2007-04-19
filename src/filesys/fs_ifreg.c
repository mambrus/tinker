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

#define DRV_IO_NAME( x, y ) \
	x ##y

//NOTE driver name is set here and it affects the whole module
#define DRV_IO( x ) \
	DRV_IO_NAME( fs_ifreg_ , x )

static const char DRV_IO(assert_info)[]="You're trying to access a non implemented function";

typedef struct{
}DRV_IO(drv_data_t);

typedef struct{

}DRV_IO(hndl_data_t);


int DRV_IO(close)(int file) {
	assert(DRV_IO(assert_info) == NULL);
	return -1;
}

int DRV_IO(fcntl)(int file, int command, ...){
	assert(DRV_IO(assert_info) == NULL);
	errno = ENOSYS;
	return -1;
}
	
	
int DRV_IO(fstat)(int file, struct stat *st) {
/*
	assert(DRV_IO(assert_info) == NULL);
	st->st_mode = S_IFCHR;
*/

	tk_fhandle_t *hndl = (tk_fhandle_t *)file;
	st->st_mode = hndl->inode->mode;;

	return 0;
}
	
int DRV_IO(isatty)(int file) {
	assert(DRV_IO(assert_info) == NULL);
	return 1;
}
		
int DRV_IO(link)(char *old, char *new) {
	assert(DRV_IO(assert_info) == NULL);
	errno=EMLINK;
	return -1;
}
/*!
Upon successful completion, the resulting offset, as measured in bytes from 
the beginning of the file, shall be returned. Otherwise, (off_t)-1 shall be 
returned, errno shall be set to indicate the error, and the file offset shall 
remain unchanged.
*/	
int DRV_IO(lseek)(int file, int ptr, int dir) {
	switch (dir) {
		case SEEK_SET:
			//the file offset shall be set to offset bytes.
		break;
		case SEEK_CUR:
			//the file offset shall be set to its current location plus offset.
		break;
		case SEEK_END:
			//the file offset shall be set to the size of the file plus offset.
		break;
		default:
			assert("Unknown direction for lseek" == 0);

	}
	return 0;
}

int DRV_IO(open)(const char *filename, int flags, ...){
	va_list ap;
		tk_fhandle_t *hndl;
		tk_inode_t *inode;
	va_start (ap, flags);
		inode=va_arg(ap,tk_inode_t *);
	va_end(ap);

	hndl=tk_new_handle(inode,(tk_flag_t)flags);

	hndl->data=calloc(1,sizeof(DRV_IO(hndl_data_t)));
/*
	((DRV_IO(hndl_data_t)*)(hndl->data))->q = 
		mq_open( filename,flags,0666,NULL);
*/
	return (int)hndl;
/*
	assert(DRV_IO(assert_info) == NULL);
	errno = ENOSYS;
	return -1;
*/
}
	
int DRV_IO(read)(int file, char *ptr, int len) {
	assert(DRV_IO(assert_info) == NULL);
	return 0;
}
		
int DRV_IO(stat)(const char *file, struct stat *st) {
	assert(DRV_IO(assert_info) == NULL);
	st->st_mode = S_IFCHR;
	return 0;
}
		
int DRV_IO(unlink)(char *name) {
	assert(DRV_IO(assert_info) == NULL);
	errno=ENOENT;
	return -1;
}
	
int DRV_IO(write)(int file, char *ptr, int len) {
	assert(DRV_IO(assert_info) == NULL);
	return len;
}


const tk_iohandle_t DRV_IO(io) = {
        DRV_IO(close),
	//DRV_IO(execve),
        DRV_IO(fcntl),
        DRV_IO(fstat),
        DRV_IO(isatty),
        DRV_IO(link),
        DRV_IO(lseek),
        DRV_IO(open),
        DRV_IO(read),
        //DRV_IO(sbrk),
        DRV_IO(stat),
        DRV_IO(unlink),
        DRV_IO(write)
};


