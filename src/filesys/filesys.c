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
#include <assert.h>
#include <string.h>
#include <limits.h>
#ifndef PATH_MAX
#include <sys/syslimits.h>
#endif

#include <filesys/filesys.h>

tk_iohandle_t std_files[3];	//!< The three standard handles (stdin,stdout & stderr)
struct hixs_t old_syscalls;	//!< Storage for any old HIXS syscalls (previous initialization);
extern struct hixs_t hixs;	//!< The call-stucture we know exist.
tk_id_t		__fcntr=0;	//!< Number of open handles
tk_id_t		__flid=0;	//!< Last allocated unique file handle ID (global counter)


int fs_init(){
	int v = sizeof(void*);
	int i = sizeof(int);
	int d = sizeof(dev_t);

	assert(sizeof(void*) <= sizeof(int));	
	assert(sizeof(void*) <= sizeof(dev_t));	
	extern __drv_finit_f 	__DRVINIT_START__;
	extern __drv_finit_f 	__DRVINIT_END__;
	drv_finit_t 		drv_init_first 	= &__DRVINIT_START__;
	drv_finit_t 		drv_init_last  	= &__DRVINIT_END__;
	drv_finit_t 		*drv_init_curr;
	int 			namelen;
	extern tk_inode_t 	*__Rnod;
	extern tk_id_t 		__icntr;
	extern tk_id_t 		__ilid;

	//Copy the current sys-calls structure
	memcpy(&old_syscalls,&hixs,sizeof(struct hixs_t));

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


	//Create the '/' node (directory) - this is a special case and can't be created with mknode
	assure(__Rnod = (tk_inode_t*)calloc(1,sizeof(tk_inode_t)));	
	__Rnod->id=__ilid++;
	__icntr++;
	namelen = sizeof("");
	__Rnod->name=(char*)calloc(1,namelen);
	strncpy(__Rnod->name,"",namelen);
	__Rnod->mode=ISA_IFDIR;
	__Rnod->belong=__Rnod;
	__Rnod->down=__Rnod;

	//Create the '/dev' node (directory)
	assure(mknod("/dev",S_IFDIR,0)			== 0);
	assure(mknod("/tmp",S_IFDIR,0)			== 0);

	//Some test nodes	
	assert(mknod("/tmp/.ambrmi09",S_IFDIR,0) 	== 0);
	assert(mknod("/tmp/afile",S_IFREG,0) 		== 0);
	assert(mknod("/tmp/bfile",S_IFREG,0) 		== 0);
	assert(mknod("/tmp/.ambrmi09/cfile",S_IFDIR,0)	== 0);


	//Start up the drivers
	const char* dinfo;
	for (
		drv_init_curr = (drv_finit_t*)drv_init_first;
		drv_init_curr < (drv_finit_t*)drv_init_last;
		drv_init_curr++
	){
		dinfo=(*drv_init_curr)(NULL);
		assure(dinfo);
		printf("Driver %-60s [started]\n",dinfo);
	}

	return 0;
}

int fs_fini(){
	extern __drv_finit_f __DRVFINI_START__;
	extern __drv_finit_f __DRVFINI_END__;	
	drv_finit_t drv_fini_first = &__DRVFINI_START__;
	drv_finit_t drv_fini_last = &__DRVFINI_END__;
	drv_finit_t *drv_fini_curr;
	extern tk_inode_t *__Rnod;

	//Close down the drivers
	const char* dinfo;
	for (
		drv_fini_curr = (drv_finit_t*)drv_fini_first;
		drv_fini_curr < (drv_finit_t*)drv_fini_last;
		drv_fini_curr++
	){
		dinfo=(*drv_fini_curr)(NULL);
		assure(dinfo);
		printf("Driver %-60s [stopped]\n",dinfo);
	}

	free(__Rnod);
	memcpy(&hixs,&old_syscalls,sizeof(struct hixs_t));

	return 0;
}



