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
#ifndef TK_IOHANDE_H
#define TK_IOHANDE_H

#include <tinker/hixs.h>

typedef struct tk_iohandle_s{
		pHIXS_close        close;
		//pHIXS_execve     execve;
		pHIXS_fcntl        fcntl;
		pHIXS_fstat        fstat;
		pHIXS_isatty       isatty;
		pHIXS_link         link;
		pHIXS_lseek        lseek;
		pHIXS_open         open;
		pHIXS_read         read;
		//pHIXS_sbrk       sbrk;
		pHIXS_stat         stat;
		pHIXS_unlink       unlink;
		pHIXS_write        write;
}tk_iohandle_t;

/*
typedef struct{
}op_ifdir_t;

typedef struct{
}op_ifblk_t;

typedef struct{
}op_ifreg_t;

typedef struct{
}op_iflnk_t;

typedef struct{
}op_ifsock_t;

typedef struct{
}op_ififo_t;


struct {
	tk_iohandle_t iohandle;
	union {
		op_ifdir_t	op_ifdir;
		op_ifblk_t	op_ifblk;
		op_ifreg_t	op_ifreg;
		op_iflnk_t	op_iflnk;
		op_ifsock_t	op_ifsock;
		op_ififo_t	op_ififo;
	}operations;
}tk_operations_t;

*/

#endif //TK_IOHANDE_H


