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
				FIFO

This driver is used as the base for handling all FIFO's and is hence a 
sub-unit for files of type S_IFIFO.

On this level FIFO's are created and removed using the init/fini mechanism.
On user API lvl, the general FS_FIFO layer is used and the user creates 
a FIFO by with mknod and destroys it with unlink.

A new FIFO is created by starting a new instance of the driver. Therefore 
each new  instance has it's own unique data.

The driver expects an argument when it's created thats either a string
with the name of the new FIFO or NULL for creating an instance called 
/dev/fifo.

@NOTE 
This driver is needed for generic FS fifos

@NOTE 
This is a character device

*/
#include <filesys/filesys.h>
#include <filesys/inode.h>
#include <assert.h>
#include <mqueue.h>
#include <string.h>

#define DEF_PRIO	10
#define DEF_MAX_MSG	15

#define DRV_IO_NAME( x, y ) \
	x ##y

//NOTE driver name is set here and it affects the whole module
#define DRV_IO( x ) \
	DRV_IO_NAME( fifo_ , x )


#define DEV_FILE_NAME( x ) \
	"/dev/fifo" #x


static const char DRV_IO(assert_info)[]="You're trying to access a non implemented function";

typedef struct{
	mqd_t       q;
	struct mq_attr qattr;
	unsigned    prio;
}DRV_IO(drv_data_t);

typedef struct{
	mqd_t       q;
}DRV_IO(hndl_data_t);

typedef struct{
	int 		len;
	char		*data;
}DRV_IO(msg_t);


/*! 
fifo close
*/	
int DRV_IO(close)(int file) {
	assert(DRV_IO(assert_info) == NULL);
	return -1;
}

/*! 
fifo open
*/
int DRV_IO(open)(const char *filename, int flags, ...){
	va_list ap;
		tk_fhandle_t *hndl;
		tk_inode_t *inode;
	va_start (ap, flags);
		inode=va_arg(ap,tk_inode_t *);
	va_end(ap);

	hndl=tk_new_handle(inode,flags);

	hndl->data=calloc(1,sizeof(DRV_IO(hndl_data_t)));
	((DRV_IO(hndl_data_t)*)(hndl->data))->q = 
		mq_open( filename,flags,0666,NULL);

	return (int)hndl;
}

/*! 
fifo read
*/		
int DRV_IO(read)(int file, char *ptr, int len) {
	tk_fhandle_t *hndl = (tk_fhandle_t *)file;
	DRV_IO(msg_t) msg;	
	int rlen;

	rlen=mq_receive(
		((DRV_IO(hndl_data_t)*)(hndl->data))->q,
		(char*)&msg,
		sizeof(DRV_IO(msg_t)),
		NULL);

	assert( rlen==sizeof(DRV_IO(msg_t)) );

	assert(len>=msg.len);	//FIXME need better handling of bigger data. Send in chunks several times?
	memcpy(ptr,msg.data,msg.len);
	free(msg.data);

	return msg.len;
}
		
/*! 
fifo write will use mq_send for transfering it's data. mq is designed for fixed
length data, but fifos can be of variable size. Therefor we'll send a pointer to 
a copy of the message in the heap instead.
*/	
int DRV_IO(write)(int file, char *ptr, int len) {
	tk_fhandle_t *hndl = (tk_fhandle_t *)file;
	DRV_IO(msg_t) msg;
	DRV_IO(drv_data_t) *Q;

	Q=(DRV_IO(drv_data_t)*)(hndl->inode->idata);

	msg.data 	= (char*)malloc(len);
	assert(msg.data);
	memcpy(msg.data,ptr,len);
	msg.len 	= len;

	assure(mq_send(
		((DRV_IO(hndl_data_t)*)(hndl->data))->q,
		(char*)&msg,
		sizeof(DRV_IO(msg_t)),
		Q->prio) == 0);

	//((DRV_IO(hndl_data_t)*)(hndl->data))->time_offset=clock();
	return sizeof(len);
}

/*! 
*/	
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

static const char DRV_IO(info_str)[]="fifo  @ " DEV_FILE_NAME();

/* Init function(s) */
void *DRV_IO(init__)(void *inarg) {
	tk_inode_t *inod;
	char *path;
	DRV_IO(drv_data_t) *Q;

	if (inarg==NULL){
		path = DEV_FILE_NAME();
	}else{
		path = (char*)inarg;
	}

	assure(mknod(path,S_IFIFO, (dev_t)&DRV_IO(io))	==0);
	inod=isearch(path);
	assert(inod);
	inod->idata=(DRV_IO(drv_data_t)*)(calloc( 1, sizeof(DRV_IO(drv_data_t)) ));

	Q=(DRV_IO(drv_data_t)*)(inod->idata);
	
	mq_unlink(path);  //Don't assert - "failiure" is normal here
	
	Q->qattr.mq_maxmsg	= DEF_MAX_MSG;
	Q->prio			= DEF_PRIO;
	Q->qattr.mq_msgsize 	= sizeof(DRV_IO(msg_t));
	//Q->q = mq_open( path,O_CREAT|O_RDWR,0666,&(Q->qattr));
	Q->q = mq_open( path,O_CREAT,0666,&(Q->qattr));

	return (void*)DRV_IO(info_str);
}

/* Fini function(s) */
void *DRV_IO(fini__)(void *inarg) {
	//tdelete(DEV_FILE_NAME(0),S_IFBLK, &DRV_IO(io));
	return (void*)DRV_IO(info_str);
}

/*Put the init/fini in corresponding sections so that filesys can pick them up */

drv_finit_t DRV_IO(init) __attribute__ ((section (".drvinit"))) =DRV_IO(init__);
drv_finit_t DRV_IO(fini) __attribute__ ((section (".drvfini"))) =DRV_IO(fini__);



