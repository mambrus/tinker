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


#define RX_BUFFLEN	102 
#define TX_BUFFLEN	102


#include <assert.h>
#include <string.h>
#include <limits.h>
#ifndef PATH_MAX
#include <sys/syslimits.h>
#endif

tk_iohandle_t std_files[3];	//!< The thre standard handles (stdin,stdout & stderr)
struct hixs_t old_syscalls;	//!< Storage for any old HIXS syscalls (previous initialization);
extern struct hixs_t hixs;	//!< The call-stucture we know exist.

tk_inode_t *root;
static int icntr=0;

int fs_init(){
	assert(sizeof(void*) <= sizeof(int));
	memcpy(&old_syscalls,&hixs,sizeof(struct hixs_t));
	extern __drv_finit_f __DRVINIT_START__;
	extern __drv_finit_f __DRVINIT_END__;
	drv_finit_t drv_init_first = &__DRVINIT_START__;
	drv_finit_t drv_init_last  = &__DRVINIT_END__;
	drv_finit_t *drv_init_curr;

	//Set new main-enty values for those we care about

	hixs.close        = fs_close;
	hixs.fcntl        = fs_fcntl;
	hixs.fstat        = fs_fstat; //HIXS_fstat; 
	hixs.isatty       = fs_isatty; 
	hixs.link         = fs_link;
	hixs.lseek        = fs_lseek;
	hixs.open         = fs_open;
	hixs.read         = fs_read; 
	hixs.stat         = fs_stat;
	hixs.unlink       = fs_unlink;
	hixs.write        = fs_write;

	std_files[0].read=old_syscalls.read;   //Assign stdin with something usefull
	std_files[1].write=old_syscalls.write; //Assign stdout with something usefull
	std_files[2].write=old_syscalls.write; //Assign stderr with something usefull


	//Create the '/' node (directory) - this is a special case and cant be created with mknode
	assure(root = (tk_inode_t*)calloc(1,sizeof(tk_inode_t)));	
	root->id=icntr++;
	root->mode=ISA_IFDIR;

	//Create the '/dev' node (directory)
	mknod("/dev",S_IFDIR,0);


	//Start up the drivers
	for (
		drv_init_curr = drv_init_first;
		drv_init_curr < drv_init_last;
		drv_init_curr++
	)
		assure((*drv_init_curr)()==0);

	
	return 0;
}

int fs_fini(){
	extern __drv_finit_f __DRVFINI_START__;
	extern __drv_finit_f __DRVFINI_END__;	
	drv_finit_t drv_fini_first = &__DRVFINI_START__;
	drv_finit_t drv_fini_last = &__DRVFINI_END__;
	drv_finit_t *drv_fini_curr;

	//Close down the drivers
	for (
		drv_fini_curr = drv_fini_first;
		drv_fini_curr < drv_fini_last;
		drv_fini_curr++
	)
		assure((*drv_fini_curr)()==0);


	free(root);
	memcpy(&hixs,&old_syscalls,sizeof(struct hixs_t));

	return 0;
}

char *getname(const char *s){
	int i;
	for (i=strlen(s);s[i]!='/';i--);
	return (char *)&s[i+1];
}

void getpath(char *buff, const char *s){	
	strncpy(buff,s,PATH_MAX);
	getname(buff)[0]=0;
}

/*! Return the i-node associated with a name - or NULL uf none is found*/
tk_inode_t *findinode(const char*s){
}

 int mknod(const char *filename, mode_t mode, dev_t dev){
	char path[PATH_MAX];
	char *name;
	tk_inode_t *belong;
	tk_inode_t *newNode;
	tk_inode_t *seekNode;
	int namelen;

	getpath(path,filename);
	name=getname(filename);

	if (!((namelen=strnlen(name,NAME_MAX)) < NAME_MAX)){
		errno=ENOSPC;	/*Breaking the naming rules - or something else is crashing...*/
		return -1;
	}
	if (!(belong=findinode(filename))){
		/*Owner path does not exist*/
		errno=ENOENT;
		return -1;
	}
	if (!(belong=findinode(path))){
		/*Owner path does not exist, good error code is missing so we use a similar*/
		errno=EEXIST;
		return -1;
	}
	if (belong->mode =! ISA_IFDIR){
		errno=ENOSPC;	/*Trying to create a node whos owner is not a directory*/
		return -1;
	}
	
	//Create the node
	assure(newNode = (tk_inode_t*)calloc(1,sizeof(tk_inode_t)));
	newNode->id=icntr++;
	newNode->name=(char*)calloc(1,namelen);
	strncpy(newNode->name,name,namelen);
	newNode->belong=belong;
	newNode->mode=mode;

	for (seekNode=belong->next;seekNode->next;seekNode=seekNode->next);
	seekNode->next=newNode;
}




/*   --- Main fs system calls follow ---  */

/*
int close (int filedes)  	Function
The function close closes the file descriptor filedes. Closing a 
file has the following consequences:

    * The file descriptor is deallocated.
    * Any record locks owned by the process on the file are 
unlocked.
    * When all file descriptors associated with a pipe or FIFO have 
been closed, any unread data is discarded. 

This function is a cancellation point in multi-threaded programs. 
This is a problem if the thread allocates some resources (like 
memory, file descriptors, semaphores or whatever) at the time close 
is called. If the thread gets canceled these resources stay 
allocated until the program ends. To avoid this, calls to close 
should be protected using cancellation handlers.

The normal return value from close is 0; a value of -1 is returned 
in case of failure. The following errno error conditions are defined 
for this function:

EBADF
    The filedes argument is not a valid file descriptor.
EINTR
    The close call was interrupted by a signal. See Interrupted 
Primitives. Here is an example of how to handle EINTR properly:

                   TEMP_FAILURE_RETRY (close (desc));
 


ENOSPC
EIO
EDQUOT
    When the file is accessed by NFS, these errors from write can 
sometimes not be detected until close. See I/O Primitives, for 
details on their meaning. 

Please note that there is no separate close64 function. This is not 
necessary since this function does not determine nor depend on the 
mode of the file. The kernel which performs the close operation 
knows which mode the descriptor is used for and can handle this 
situation.

To close a stream, call fclose (see Closing Streams) instead of 
trying to close its underlying file descriptor with close. This 
flushes any buffered output and updates the stream object to 
indicate that it is closed. 
*/
int fs_close(int file) {
	return 0;
}

int fs_fcntl (int file, int command, ...){
	va_list ap;
	/*any local vars here...*/
	va_start (ap, command);

	_syscall_mon(fs_fcntl);
	errno = ENOSYS;
	return -1;
}
	
int fs_fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	st->st_blksize = TX_BUFFLEN;
	return 0;
}
	
int fs_isatty(int file) {
	return 1;
}
		
int fs_link(char *old, char *new) {
	errno=EMLINK;
	return -1;
}
	
int fs_lseek(int file, int ptr, int dir) {
	return 0;
}

int fs_open(const char *filename, int file, ...){
	va_list ap;
	/*any local vars here...*/
	va_start (ap, file);

	errno = ENOSYS;
	return -1;
}
	
int fs_read(int file, char *ptr, int len) {

	return 0;
}
		
int fs_stat(const char *file, struct stat *st) {

	st->st_mode = S_IFCHR;
	return 0;
}
		
int fs_unlink(char *name) {

	errno=ENOENT;
	return -1;
}
	
int fs_write(int file, char *ptr, int len) {

	if (file<3){
		return std_files[file].write(file, ptr, len);
	}
	return len;
}


