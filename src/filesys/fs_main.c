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

//FIXME make these more transparent
#define RX_BUFFLEN	102  
#define TX_BUFFLEN	102

extern tk_id_t		__fcntr;
extern tk_id_t		__flid;

extern tk_iohandle_t std_files[3];

static const char assert_info[]="Something is wrong - please report this bug";

/*!
@brief http://www.opengroup.org/onlinepubs/009695399/
*/
int fs_close(int file) {
	return 0;
}

/*!
@brief http://www.opengroup.org/onlinepubs/009695399/
*/
int fs_fcntl (int file, int command, ...){
	va_list ap;
		tk_fhandle_t *hndl;
		int rc;
	va_start (ap, command);
		/*Copy to locals*/
	//va_end(ap);			//Don't end AP here - we're piping the variable to next fubction for final parsing

	hndl = (tk_fhandle_t *)file;
	CHECK_FH(hndl,fcntl);

	rc=hndl->inode->iohandle->fcntl(file,command,ap);

	va_end(ap);
	
	return rc;
}

/*!
@brief http://www.opengroup.org/onlinepubs/009695399/

NOTE fstat is called only if buffered access needs to be determined
(i.e. if file was openwd with fopen). For low leve access (open, read, write
e.t.a.) it's never called exept if called explicitly by the application.
*/	
int fs_fstat(int file, struct stat *st) {
	if ((file>=0) && (file<=2)){
		st->st_mode = S_IFCHR;
		st->st_blksize = TX_BUFFLEN;
		return 0;
	}
	tk_fhandle_t *hndl= (tk_fhandle_t *)file;
	CHECK_FH(hndl,fstat);
	return hndl->inode->iohandle->fstat(file, st);
}
	
/*!
@brief http://www.opengroup.org/onlinepubs/009695399/
*/
int fs_isatty(int file) {
	if ((file>=0) && (file<=2)){
		return 1;
	}
	tk_fhandle_t *hndl= (tk_fhandle_t *)file;
	CHECK_FH(hndl,isatty);
	return hndl->inode->iohandle->isatty(file);
}

/*!
@brief http://www.opengroup.org/onlinepubs/009695399/
*/		
int fs_link(char *old, char *new) {
	assert(assert_info == NULL);
	errno=EMLINK;
	return -1;
}

/*!
@brief http://www.opengroup.org/onlinepubs/009695399/
*/	
int fs_lseek(int file, int ptr, int dir) {
	assert(assert_info == NULL);
	return -1;
}

/*!
@brief http://www.opengroup.org/onlinepubs/009695399/
*/
int fs_open(const char *filename, int oflag, ...){
	va_list ap;
		tk_inode_t *inode;
	va_start (ap, oflag);
		/*No need, nothing to parse*/
	va_end(ap);
	
	inode=isearch(filename);

	if (inode==NULL)		//Errno allready set by isearch
		return -1;

	assure(inode->iohandle);
	assure(inode->iohandle->open);
	return inode->iohandle->open(filename,oflag,inode);
}

/*!
@brief http://www.opengroup.org/onlinepubs/009695399/
*/	
int fs_read(int file, char *ptr, int len) {
	if ((file>=0) && (file<=2)){
		return std_files[file].read(file, ptr, len);
	}

	tk_fhandle_t *hndl= (tk_fhandle_t *)file;	
	CHECK_FH(hndl,fcntl);

	return hndl->inode->iohandle->read(file, ptr, len);
}

/*!
@brief http://www.opengroup.org/onlinepubs/009695399/
*/		
int fs_stat(const char *file, struct stat *st) {
	assert(assert_info == NULL);
	st->st_mode = S_IFCHR;
	return 0;
}

/*!
@brief http://www.opengroup.org/onlinepubs/009695399/
*/		
int fs_unlink(char *name) {
	assert(assert_info == NULL);
	errno=ENOENT;
	return -1;
}

/*!
@brief http://www.opengroup.org/onlinepubs/009695399/
*/	
int fs_write(int file, char *ptr, int len) {
	if ((file>=0) && (file<=2)){
		return std_files[file].write(file, ptr, len);
	}
	tk_fhandle_t *hndl= (tk_fhandle_t *)file;
	CHECK_FH(hndl,fcntl);
	return hndl->inode->iohandle->write(file, ptr, len);
}


/* Helper functions */

/*!
Creates a new handle
*/
tk_fhandle_t *tk_new_handle(tk_inode_t *inode, tk_flag_t aflags){
	tk_fhandle_t *hndl;
	
	assure( (hndl=(tk_fhandle_t*)calloc(1,sizeof(tk_fhandle_t))));
	__fcntr++;
	hndl->id=__flid++;
	hndl->flags=aflags;
	hndl->inode=inode;
	return hndl;
}

/*!
Frees a handle
*/

int tk_free_handle(tk_fhandle_t *fhndl){
	free(fhndl);
	__fcntr--;

	return 0;
}
