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
/*!
				TIME

This is one of the simplest drivers possible but that shows some of the
things you can do, and also the basics for how to write device drivers.

  ** You're encouraged to copy this file and use it as a template **

The TIME driver provides you with with the system up-time via a block
device (/dev/time) in the form of a clock_t data.

Optionally, you can open the device for writing and set a reference time.
If you write the current time to the device, each time you read from the
device again the time you get will now be the delta between you wrote to
the device and when you read it.

This difference is stored in the file-handle (fh) itself in the optional
"data" field. This has has the following implications.

1) You can use the same driver to measure delta-times in independent
   threads. As long as you use a unique fh, the operations will not
   interfere with each other.

2) To use the driver to measure delta-times, the same fh for reading
   has to be used that was once used for writing the reference time. I.e.
   you have to open the file O_RDWR.

@NOTE
Since this driver does not block or use any other syncronisation, it
can be used without a kernel.

@NOTE
This simple driver will ignore flags that affect blocking and scheduling
for the sake of simplicity. The drivers behavior is instead hard-coded
(if you want, you can implement that as an exercise youself).

*/
#include <filesys/filesys.h>
#include <filesys/inode.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#define DRV_IO_NAME( x, y ) \
	x ##y

//NOTE driver name is set here and it affects the whole module
#define DRV_IO( x ) \
	DRV_IO_NAME( tim_ , x )


#define DEV_FILE_NAME( x ) \
	"/dev/time" #x


static const char DRV_IO(assert_info)[]="You're trying to access a non implemented function";

typedef struct{
	clock_t		time_open;
	clock_t		time_offset;
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

int DRV_IO(lseek)(int file, int ptr, int dir) {
	assert(DRV_IO(assert_info) == NULL);
	return 0;
}

int DRV_IO(open)(const char *filename, int flags, ...){
	va_list ap;
		tk_fhandle_t *hndl;
		tk_inode_t *inode;
	va_start (ap, flags);
		inode=va_arg(ap,tk_inode_t *);
	va_end(ap);

	hndl=tk_new_handle(inode,flags);

	hndl->data=calloc(1,sizeof(DRV_IO(hndl_data_t)));
	((DRV_IO(hndl_data_t)*)(hndl->data))->time_open=clock();

	return (int)hndl;
}

int DRV_IO(read)(int file, char *ptr, int len) {
	tk_fhandle_t *hndl = (tk_fhandle_t *)file;
	clock_t ctime=clock();
	ctime-=((DRV_IO(hndl_data_t)*)(hndl->data))->time_offset;
	memcpy(ptr,&ctime,sizeof(clock_t));
	return sizeof(clock_t);
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
	tk_fhandle_t *hndl = (tk_fhandle_t *)file;
	((DRV_IO(hndl_data_t)*)(hndl->data))->time_offset=clock();
	return sizeof(len);
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

static const char DRV_IO(info_str)[]="time  @ " DEV_FILE_NAME();

/* Init function(s) */
void *DRV_IO(init_0__)(void *inarg) {
	assert(inarg==NULL);
	assure(mknod(DEV_FILE_NAME(),S_IFBLK, (dev_t)&DRV_IO(io))	==0);
	return (void*)DRV_IO(info_str);
}

/* Fini function(s) */
void *DRV_IO(fini_0__)(void *inarg) {
	assert(inarg==NULL);
	//tdelete(DEV_FILE_NAME(0),S_IFBLK, &DRV_IO(io));
	return (void*)DRV_IO(info_str);
}

/*Put the init/fini in corresponding sections so that filesys can pick them up */

drv_finit_t DRV_IO(init_0) __attribute__ ((section (".drvinit"))) =DRV_IO(init_0__);
drv_finit_t DRV_IO(fini_0) __attribute__ ((section (".drvfini"))) =DRV_IO(fini_0__);



