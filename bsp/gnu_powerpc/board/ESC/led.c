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


#define DRV_IO_NAME( x, y ) \
	x ##y

//NOTE driver name is set here and it affects the whole module
#define DRV_IO( x ) \
	DRV_IO_NAME( led_ , x )


#define DEV_FILE_NAME( x ) \
	"/dev/led" #x

typedef struct{
	int 		handle_data;
}DRV_IO(data_t);

int ledstat_0=0;
int ledstat_1=0;
int ledstat_2=0;
int ledstat_3=0;

static const char DRV_IO(assert_info)[]="You're trying to access a non implemented function";
#include "led.h"
#include <board/board.h>

void led_on(led_id id, led_color color){
	if (id==LED_2) {
		if (color==LED_RED) {
			bitclear_16(PADAT,8);	
		}else{
			bitclear_16(PADAT,9);	
		}
	}else{
		if (color==LED_RED) {
			bitclear_16(PADAT,10);	
		}else{
			bitclear_16(PADAT,11);	
		}
	}
}

void led_off(led_id id, led_color color){
	if (id==LED_2) {
		if (color==LED_RED) {
			bitset_16(PADAT,8);	
		}else{
			bitset_16(PADAT,9);
		}	
	}else{
		if (color==LED_RED) {
			bitset_16(PADAT,10);	
		}else{
			bitset_16(PADAT,11);	
		}
	}
}

//=============================================================================
// Open functions
//=============================================================================	
int DRV_IO(open0)(const char *filename, int flags, ...){
	va_list ap;
		tk_fhandle_t *hndl;
		tk_inode_t *inode;
	va_start (ap, flags);
		inode=va_arg(ap,tk_inode_t *);
	va_end(ap);

	hndl=tk_new_handle(inode,(tk_flag_t)flags);	

	hndl->data=calloc(1,sizeof(DRV_IO(data_t)));
	((DRV_IO(data_t)*)(hndl->data))->handle_data=0x00;

	return (int)hndl;
}
int DRV_IO(open1)(const char *filename, int flags, ...){
	va_list ap;
		tk_fhandle_t *hndl;
		tk_inode_t *inode;
	va_start (ap, flags);
		inode=va_arg(ap,tk_inode_t *);
	va_end(ap);

	hndl=tk_new_handle(inode,(tk_flag_t)flags);	

	hndl->data=calloc(1,sizeof(DRV_IO(data_t)));
	((DRV_IO(data_t)*)(hndl->data))->handle_data=0x01;

	return (int)hndl;
}
int DRV_IO(open2)(const char *filename, int flags, ...){
	va_list ap;
		tk_fhandle_t *hndl;
		tk_inode_t *inode;
	va_start (ap, flags);
		inode=va_arg(ap,tk_inode_t *);
	va_end(ap);

	hndl=tk_new_handle(inode,(tk_flag_t)flags);	

	hndl->data=calloc(1,sizeof(DRV_IO(data_t)));
	((DRV_IO(data_t)*)(hndl->data))->handle_data=0x02;

	return (int)hndl;
}
int DRV_IO(open3)(const char *filename, int flags, ...){
	va_list ap;
		tk_fhandle_t *hndl;
		tk_inode_t *inode;
	va_start (ap, flags);
		inode=va_arg(ap,tk_inode_t *);
	va_end(ap);

	hndl=tk_new_handle(inode,(tk_flag_t)flags);	

	hndl->data=calloc(1,sizeof(DRV_IO(data_t)));
	((DRV_IO(data_t)*)(hndl->data))->handle_data=0x03;

	return (int)hndl;
}

//=============================================================================
// Read functions
//=============================================================================	
int DRV_IO(close0)(int file) {
	assert(DRV_IO(assert_info) == NULL);
	return -1;
}
int DRV_IO(close1)(int file) {
	assert(DRV_IO(assert_info) == NULL);
	return -1;
}
int DRV_IO(close2)(int file) {
	assert(DRV_IO(assert_info) == NULL);
	return -1;
}
int DRV_IO(close3)(int file) {
	assert(DRV_IO(assert_info) == NULL);
	return -1;
}


//=============================================================================
// Read functions
//=============================================================================	
int DRV_IO(read0)(int file, char *ptr, int len) {
	switch (len){
		case 1:
			*(__uint8_t*)ptr=ledstat_0;
			break;
		case 2:
			*(__uint16_t*)ptr=ledstat_0;
			break;
		case 4:
			*(__uint32_t*)ptr=ledstat_0;
			break;
		assert("Wrong size of data requested"==NULL);
	}		
	return len;
}
int DRV_IO(read1)(int file, char *ptr, int len) {
	switch (len){
		case 1:
			*(__uint8_t*)ptr=ledstat_1;
			break;
		case 2:
			*(__uint16_t*)ptr=ledstat_1;
			break;
		case 4:
			*(__uint32_t*)ptr=ledstat_1;
			break;
		assert("Wrong size of data requested"==NULL);
	}		
	return len;
}
int DRV_IO(read2)(int file, char *ptr, int len) {
	switch (len){
		case 1:
			*(__uint8_t*)ptr=ledstat_2;
			break;
		case 2:
			*(__uint16_t*)ptr=ledstat_2;
			break;
		case 4:
			*(__uint32_t*)ptr=ledstat_2;
			break;
		assert("Wrong size of data requested"==NULL);
	}		
	return len;
}
int DRV_IO(read3)(int file, char *ptr, int len) {
	switch (len){
		case 1:
			*(__uint8_t*)ptr=ledstat_3;
			break;
		case 2:
			*(__uint16_t*)ptr=ledstat_3;
			break;
		case 4:
			*(__uint32_t*)ptr=ledstat_3;
			break;
		assert("Wrong size of data requested"==NULL);
	}		
	return len;
}
		
//=============================================================================
// Write functions
//=============================================================================
/*
	0: (LED1_red)
	1: (LED1_green)
	2: (LED2_red)
	3: (LED2_green)
*/

int DRV_IO(write0)(int file, char *ptr, int len) {
	if (len<=sizeof(int)){
		switch (len){
			case 1:
				ledstat_0=*(__uint8_t*)ptr;
				break;
			case 2:
				ledstat_0=*(__uint16_t*)ptr;
				break;
			case 4:
				ledstat_0=*(__uint32_t*)ptr;
				break;
			assert("Wrong size of data passed"==NULL);
		}		
	}

	if (ledstat_0)
		led_on(LED_1, LED_RED);
	else
		led_off(LED_1, LED_RED);
		
	return len;
}
int DRV_IO(write1)(int file, char *ptr, int len) {
	int ledstat_;
	if (len<=sizeof(int)){
		switch (len){
			case 1:
				ledstat_1=*(__uint8_t*)ptr;
				break;
			case 2:
				ledstat_1=*(__uint16_t*)ptr;
				break;
			case 4:
				ledstat_1=*(__uint32_t*)ptr;
				break;
			assert("Wrong size of data passed"==NULL);
		}		
	}

	if (ledstat_1)
		led_on(LED_1, LED_GREEN);
	else
		led_off(LED_1, LED_GREEN);
		
	return len;
}
int DRV_IO(write2)(int file, char *ptr, int len) {
	int ledstat_;
	if (len<=sizeof(int)){
		switch (len){
			case 1:
				ledstat_2=*(__uint8_t*)ptr;
				break;
			case 2:
				ledstat_2=*(__uint16_t*)ptr;
				break;
			case 4:
				ledstat_2=*(__uint32_t*)ptr;
				break;
			assert("Wrong size of data passed"==NULL);
		}		
	}

	if (ledstat_2)
		led_on(LED_2, LED_RED);
	else
		led_off(LED_2, LED_RED);
		
	return len;
}
int DRV_IO(write3)(int file, char *ptr, int len) {
	int ledstat_;
	if (len<=sizeof(int)){
		switch (len){
			case 1:
				ledstat_3=*(__uint8_t*)ptr;
				break;
			case 2:
				ledstat_3=*(__uint16_t*)ptr;
				break;
			case 4:
				ledstat_3=*(__uint32_t*)ptr;
				break;
			assert("Wrong size of data passed"==NULL);
		}		
	}

	if (ledstat_3)
		led_on(LED_2, LED_GREEN);
	else
		led_off(LED_2, LED_GREEN);
		
	return len;
}

//=============================================================================
// Buffered mode access determination
//=============================================================================
/*This should never need to be called, but provided just in case */	
int DRV_IO(isatty)(int file) {
	return 0;
}

int DRV_IO(fstat)(int file, struct stat *st) {
	tk_fhandle_t *hndl = (tk_fhandle_t *)file;
	if (hndl->belong->mode != ISA_UNKNOWN)
		st->st_mode = hndl->belong->mode;
	else
		st->st_mode = S_IFBLK;

	st->st_blksize = sizeof(int);		//Should not be needed since blk & ubuff

	return 0;
}

//=============================================================================
// IO structures - pre-assigned
//=============================================================================
static const tk_iohandle_t DRV_IO(io0) = {
        DRV_IO(close0),
	        fs_ifblk_fcntl,	DRV_IO(fstat),	DRV_IO(isatty),fs_ifblk_link,	fs_ifblk_lseek,
        DRV_IO(open0),
        DRV_IO(read0),
        	fs_ifblk_stat,	fs_ifblk_unlink,
        DRV_IO(write0)
};
static const tk_iohandle_t DRV_IO(io1) = {
        DRV_IO(close1),
	        fs_ifblk_fcntl,	DRV_IO(fstat),	DRV_IO(isatty),fs_ifblk_link,	fs_ifblk_lseek,
        DRV_IO(open1),
        DRV_IO(read1),
        	fs_ifblk_stat,	fs_ifblk_unlink,
        DRV_IO(write1)
};
static const tk_iohandle_t DRV_IO(io2) = {
        DRV_IO(close2),
	        fs_ifblk_fcntl,	DRV_IO(fstat),	DRV_IO(isatty),fs_ifblk_link,	fs_ifblk_lseek,
        DRV_IO(open2),
        DRV_IO(read2),
        	fs_ifblk_stat,	fs_ifblk_unlink,
        DRV_IO(write2)
};
static const tk_iohandle_t DRV_IO(io3) = {
        DRV_IO(close3),
	        fs_ifblk_fcntl,	DRV_IO(fstat),	DRV_IO(isatty),fs_ifblk_link,	fs_ifblk_lseek,
        DRV_IO(open3),
        DRV_IO(read3),
        	fs_ifblk_stat,	fs_ifblk_unlink,
        DRV_IO(write3)
};

static const char DRV_IO(info_str)[]="led   @ " DEV_FILE_NAME()"[0-4]";

/* Init function(s) */
int DRV_IO(init_0__)() {
	assure(mknod(DEV_FILE_NAME(0_red),S_IFBLK, (dev_t)&DRV_IO(io0))	==0);
	return 0;
}
int DRV_IO(init_1__)() {
	assure(mknod(DEV_FILE_NAME(0_green),S_IFBLK, (dev_t)&DRV_IO(io1))	==0);
	return 0;
}
int DRV_IO(init_2__)() {
	assure(mknod(DEV_FILE_NAME(1_red),S_IFBLK, (dev_t)&DRV_IO(io2))	==0);
	return 0;
}
int DRV_IO(init_3__)() {
	assure(mknod(DEV_FILE_NAME(1_green),S_IFBLK, (dev_t)&DRV_IO(io3))	==0);
	return 0;
}

const char *DRV_IO(init__)() {
	DRV_IO(init_0__)();
	DRV_IO(init_1__)();
	DRV_IO(init_2__)();
	DRV_IO(init_3__)();
	return DRV_IO(info_str);
}

/* Fini function(s) */
int DRV_IO(fini_0__)() {
	return 0;
}
int DRV_IO(fini_1__)() {
	return 0;
}
int DRV_IO(fini_2__)() {
	return 0;
}
int DRV_IO(fini_3__)() {
	return 0;
}
const char *DRV_IO(fini__)() {
	DRV_IO(fini_0__)();
	DRV_IO(fini_1__)();
	DRV_IO(fini_2__)();
	DRV_IO(fini_3__)();
	return DRV_IO(info_str);
}

/*Put the main init/fini in corresponding sections so that filesys can pick them up */
drv_finit_t DRV_IO(init) __attribute__ ((section (".drvinit"))) =DRV_IO(init__);
drv_finit_t DRV_IO(fini) __attribute__ ((section (".drvfini"))) =DRV_IO(fini__);


/*
int DRV_IO(fcntl)(int file, int command, ...){
	assert(DRV_IO(assert_info) == NULL);
	errno = ENOSYS;
	return -1;
}
		
int DRV_IO(fstat)(int file, struct stat *st) {
	tk_fhandle_t *hndl = file;
	st->st_mode = hndl->belong->mode;;
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
*/

