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
#include <assert.h>
#include <semaphore.h>
#include <string.h>
#include <sja1000.h>

#define DRV_IO_NAME( x, y ) \
	x ##y

//NOTE driver name is set here and it affects the whole module
#define DRV_IO( x ) \
	DRV_IO_NAME( can_ , x )

#define DEV_FILE_NAME( x ) \
	"/dev/can" #x

static const char DRV_IO(assert_info)[] =
    "You're trying to access a non implemented function";

sem_t __can_rx_sem;

typedef struct {
} DRV_IO(hndl_data_t);

typedef struct {
} DRV_IO(drv_data_t);

typedef struct {
} DRV_IO(inode_data_t);

int DRV_IO(close) (int file)
{
	tk_fhandle_t *hndl = (tk_fhandle_t *) file;
	free(hndl->data);
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

	return (int)hndl;
}

int DRV_IO(read) (int file, char *ptr, int len) {
	tk_fhandle_t *hndl = (tk_fhandle_t *) file;

	return sja1000_read(ptr, len);
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
	return sja1000_write(ptr, len);
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

static const char DRV_IO(info_str)[] =
    "can   @ " DEV_FILE_NAME(0) " - SJA1000 pelican mode";

//! @TODO This is for 860/ESC and should not really be here - we need to make this more transparent
typedef enum {
	lvl_IRQ_0 = 0,
	lvl_Intrnl_0,
	lvl_IRQ_1,
	lvl_Intrnl_1,
	lvl_IRQ_2,
	lvl_Intrnl_2,
	lvl_IRQ_3,
	lvl_Intrnl_3,
	lvl_IRQ_4_,
	lvl_Intrnl_4,
	lvl_IRQ_5,
	lvl_Intrnl_5,
	lvl_IRQ_6,
	lvl_Intrnl_6,
	lvl_IRQ_7,
	lvl_Intrnl_7
} irq_level;

/* Init function(s) */
/*!
Default init (BOARD ESC) - obsolete
*/
void *DRV_IO(init_0__) (void *inarg)
{
	assert(inarg == NULL);
	assure(sja1000_init(0x20000000, lvl_IRQ_3, 1, 1, 500000, 0, 0xffffffff)
	       == 0);
	assure(mknod(DEV_FILE_NAME(0), S_IFBLK, (dev_t) & DRV_IO(io)) == 0);
	return (void *)DRV_IO(info_str);
}

/*!
Specific init
*/
int DRV_IO(init) (__uint32_t baddr,	//!< Base address of the circuit
		  int IRQn,	//!< Interrupt number for the main CAN handler
		  int pmode,	//!< 1=pelican mode, 0=basic mode
		  int xmode,	//!< 1=extended frame (i.e. 29 bits header), 0=basic frame (i.e. 11 bits header)
		  int bps,	//!< Speed of the bus, nominal speed is 500 Kbps
		  __uint32_t ac,	//!< Acceptance code @note MSB is the same either EFF or SFF
		  __uint32_t am	//!< Acceptance mask @note MSB is the same either EFF or SFF
    ) {
	int rc;
	extern tk_inode_t *__Rnod;

	assure(sja1000_init(baddr, IRQn, pmode, xmode, bps, ac, am) == 0);
	rc = mknod(DEV_FILE_NAME(0), S_IFBLK, (dev_t) & DRV_IO(io));
	return isearch(__Rnod, DEV_FILE_NAME(0));
}

/*!
Default fini function(s) - obsolete
*/
void *DRV_IO(fini_0__) (void *inarg) {
	assert(inarg == NULL);
	//tdelete(DEV_FILE_NAME(0),S_IFBLK, &DRV_IO(io));
	return (void *)DRV_IO(info_str);
}

/*!
Specific fini
*/
int DRV_IO(fini) (int file) {
	tk_fhandle_t *hndl = (tk_fhandle_t *) file;
	DRV_IO(drv_data_t) * drvdata;

	drvdata = (DRV_IO(drv_data_t) *) (hndl->inode->idata);

	memset(drvdata, 0, sizeof(DRV_IO(drv_data_t)));
	free(drvdata);

	return unlink(hndl->inode->name);
}

/*Put the init/fini in corresponding sections so that filesys can pick them up */
/*Obsolete. Board specific initialization must start these
drv_finit_t DRV_IO(init_0) __attribute__ ((section (".drvinit"))) =DRV_IO(init_0__);
drv_finit_t DRV_IO(fini_0) __attribute__ ((section (".drvfini"))) =DRV_IO(fini_0__);
*/
