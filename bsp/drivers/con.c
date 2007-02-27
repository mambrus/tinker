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
#include <filesys/filesys.h>
#include <filesys/inode.h>

#define DRV_IO_NAME( x, y ) \
	x ##y

//NOTE driver name is set here and it affects the whole module
#define DRV_IO( x ) \
	DRV_IO_NAME( con_ , x )


#define DEV_FILE_NAME( x ) \
	"/dev/con" #x


int DRV_IO(close)(int file) {
	return -1;
}

int DRV_IO(fcntl)(int file, int command, ...){
	errno = ENOSYS;
	return -1;
}
		
int DRV_IO(fstat)(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}
	
int DRV_IO(isatty)(int file) {
	return 1;
}
		
int DRV_IO(link)(char *old, char *new) {
	errno=EMLINK;
	return -1;
}
	
int DRV_IO(lseek)(int file, int ptr, int dir) {
	return 0;
}

int DRV_IO(open)(const char *filename, int flags, ...){
	errno = ENOSYS;
	return -1;
}
	
int DRV_IO(read)(int file, char *ptr, int len) {
	return 0;
}
		
int DRV_IO(stat)(const char *file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}
		
int DRV_IO(unlink)(char *name) {
	errno=ENOENT;
	return -1;
}
	
int DRV_IO(write)(int file, char *ptr, int len) {
	return len;
}

/*IO structure - pre-assigned*/
static const tk_iohandle_t DRV_IO(io) = {
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
static const char DRV_IO(info_str0)[]="con_0 @ " DEV_FILE_NAME(0);
static const char DRV_IO(info_str1)[]="con_1 @ " DEV_FILE_NAME(1);

/* Init function(s) */
const char *DRV_IO(init_0__)() {
	assure(mknod(DEV_FILE_NAME(0),S_IFBLK, (dev_t)&DRV_IO(io))	==0);
	return DRV_IO(info_str0);
}
const char *DRV_IO(init_1__)() {
	assure(mknod(DEV_FILE_NAME(1),S_IFBLK, (dev_t)&DRV_IO(io))	==0);
	return DRV_IO(info_str1);
}

/* Fini function(s) */
const char *DRV_IO(fini_0__)() {
	//tdelete(DEV_FILE_NAME(0),S_IFBLK, &DRV_IO(io));
	return DRV_IO(info_str0);
}
const char *DRV_IO(fini_1__)() {
	return DRV_IO(info_str1);
}


/*Put the init/fini in corresponding sections so that filesys can pick them up */

drv_finit_t DRV_IO(init_0) __attribute__ ((section (".drvinit"))) =DRV_IO(init_0__);
drv_finit_t DRV_IO(init_1) __attribute__ ((section (".drvinit"))) =DRV_IO(init_1__);

drv_finit_t DRV_IO(fini_0) __attribute__ ((section (".drvfini"))) =DRV_IO(fini_0__);
drv_finit_t DRV_IO(fini_1) __attribute__ ((section (".drvfini"))) =DRV_IO(fini_1__);


