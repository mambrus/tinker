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
				TIMER

This is quite similar to the TIME driver, but slightly more complex driver.
It's still a good starting point for self-studies and show you how to
implement multiple threads and use queues inside a driver.

Remeber that in TinKer there is no difference between driver-code and any
other code. I.e. the same queues and threads API as anywhere else can be
used.

The TIMER driver provides you with with the system up-time via a block
device (/dev/timer) in the form of a clock_t data, but it does so only
AFTER it's reference time has expired. Until then, any reading from
the device will lead to that the reading thread blocks.

You can change the behaviour of the driver by using the fcntl API. At
initial opening, the reference time will be relative to the time of
the next read. You can change this to absolute time (among others). Just
remeber to do any changes *before* the next read.

Since there is no way to awake a blocked thread thats blocked on a
time-out and prevent a deadlock by misstake, when you open the devive,
the time-out reverence will be preset to "now" + 10 seconds.

@NOTE
This driver blocks. To operate, it needs a POSIX 1003.1c compliant kernel.

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
#include <pthread.h>

#define DRV_IO_NAME( x, y ) \
	x ##y

//NOTE driver name is set here and it affects the whole module
#define DRV_IO( x ) \
	DRV_IO_NAME( tmr_ , x )

#define DEV_FILE_NAME( x ) \
	"/dev/timer" #x

static const char DRV_IO(assert_info)[] =
    "You're trying to access a non implemented function";

typedef enum { tmr_relative, tmr_absolute, tmr_noblock } op_mode_t;
typedef struct {
	clock_t time_open;
	clock_t time_ref;
	pthread_t tmr_thread;
	op_mode_t op_mode;
} DRV_IO(hndl_data_t);

void *timr_thread(void *inpar)
{
	tk_fhandle_t *hndl = inpar;
	DRV_IO(hndl_data_t) * op_data = hndl->data;
	clock_t ctime = clock();

	switch (op_data->op_mode) {
	case tmr_relative:
		usleep(op_data->time_ref);
		break;
	case tmr_absolute:
		usleep(op_data->time_ref - clock());
		break;
	}
	ctime = clock();
	ctime -= op_data->time_ref;
	return (void *)ctime;
}

int DRV_IO(close) (int file) {
	assert(DRV_IO(assert_info) == NULL);
	return -1;
}

int DRV_IO(fcntl) (int file, int command, ...) {
	assert(DRV_IO(assert_info) == NULL);
	errno = ENOSYS;
	return -1;
}

int DRV_IO(fstat) (int file, struct stat * st) {
	tk_fhandle_t *hndl = (tk_fhandle_t *) file;
	st->st_mode = hndl->inode->mode;;
	return 0;
}

int DRV_IO(isatty) (int file) {
	assert(DRV_IO(assert_info) == NULL);
	return 1;
}

int DRV_IO(link) (char *old, char *new) {
	assert(DRV_IO(assert_info) == NULL);
	errno = EMLINK;
	return -1;
}

int DRV_IO(lseek) (int file, int ptr, int dir) {
	assert(DRV_IO(assert_info) == NULL);
	return 0;
}

int DRV_IO(open) (const char *filename, int flags, ...) {
	va_list ap;
	tk_fhandle_t *hndl;
	tk_inode_t *inode;
	va_start(ap, flags);
	inode = va_arg(ap, tk_inode_t *);
	va_end(ap);

	hndl = tk_new_handle(inode, flags);

	hndl->data = calloc(1, sizeof(DRV_IO(hndl_data_t)));
	((DRV_IO(hndl_data_t) *) (hndl->data))->time_open = clock();
	((DRV_IO(hndl_data_t) *) (hndl->data))->time_ref = (clock_t) 10000000uL;

	return (int)hndl;
}

int DRV_IO(read) (int file, char *ptr, int len) {
	tk_fhandle_t *hndl = (tk_fhandle_t *) file;
	DRV_IO(hndl_data_t) * op_data = (DRV_IO(hndl_data_t) *) (hndl->data);

	clock_t ctime = clock();
	assure(pthread_create
	       (&(((DRV_IO(hndl_data_t) *) (hndl->data))->tmr_thread), NULL,
		timr_thread, file) == 0);
	pthread_join(((DRV_IO(hndl_data_t) *) (hndl->data))->tmr_thread,
		     &ctime);

	memcpy(ptr, &ctime, sizeof(clock_t));
	return sizeof(clock_t);
}

int DRV_IO(stat) (const char *file, struct stat * st) {
	assert(DRV_IO(assert_info) == NULL);
	st->st_mode = S_IFCHR;
	return 0;
}

int DRV_IO(unlink) (char *name) {
	assert(DRV_IO(assert_info) == NULL);
	errno = ENOENT;
	return -1;
}

int DRV_IO(write) (int file, char *ptr, int len) {
	tk_fhandle_t *hndl = (tk_fhandle_t *) file;
	DRV_IO(hndl_data_t) * op_data = (DRV_IO(hndl_data_t) *) (hndl->data);

	((DRV_IO(hndl_data_t) *) (hndl->data))->time_ref = *(clock_t *) ptr;
	return sizeof(len);
}

/*IO structure - pre-assigned*/
static const tk_iohandle_t DRV_IO(io) = {
	DRV_IO(close),
	    //DRV_IO(execve),
	    DRV_IO(fcntl),
	    DRV_IO(fstat),
	    DRV_IO(isatty),
	    DRV_IO(link), DRV_IO(lseek), DRV_IO(open), DRV_IO(read),
	    //DRV_IO(sbrk),
	    DRV_IO(stat), DRV_IO(unlink), DRV_IO(write)
};

static const char DRV_IO(info_str)[] = "timer @ " DEV_FILE_NAME();

/* Init function(s) */
void *DRV_IO(init_0__) (void *inarg)
{
	assert(sizeof(clock_t) <= sizeof(void *));
	assert(inarg == NULL);
	assure(mknod(DEV_FILE_NAME(), S_IFBLK, (dev_t) & DRV_IO(io)) == 0);
	return (void *)DRV_IO(info_str);
}

/* Fini function(s) */
void *DRV_IO(fini_0__) (void *inarg) {
	assert(inarg == NULL);
	//tdelete(DEV_FILE_NAME(0),S_IFBLK, &DRV_IO(io));
	return (void *)DRV_IO(info_str);
}

/*Put the init/fini in corresponding sections so that filesys can pick them up */

drv_finit_t DRV_IO(init_0) __attribute__ ((section(".drvinit"))) =
DRV_IO(init_0__);
drv_finit_t DRV_IO(fini_0) __attribute__ ((section(".drvfini"))) =
DRV_IO(fini_0__);
