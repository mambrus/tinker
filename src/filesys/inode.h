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
#ifndef TK_INODE_H
#define TK_INODE_H

#include <tinker/config.h>
#include <tinker/hixs.h>

#include <filesys/iohandle.h>

typedef int tk_id_t;
typedef enum {
	ISA_UNKNOWN = 0,
	ISA_IFDIR = S_IFDIR,
	ISA_IFCHR = S_IFCHR,
	ISA_IFBLK = S_IFBLK,
	ISA_IFREG = S_IFREG,
	ISA_IFLNK = S_IFLNK,
	ISA_IFSOCK = S_IFSOCK,
	ISA_IFIFO = S_IFIFO
} tk_mode_t;

typedef struct tk_mount_s {
	tk_iohandle_t *io_device;
	tk_iohandle_t *io_fstype;

	unsigned long int options;

} tk_mount_t;

typedef struct tk_inode_s {
	tk_id_t id;
	char *name;
	tk_mode_t mode;
	struct tk_mount_s *mount;
	struct tk_inode_s *belong;
	struct tk_inode_s *next;
	struct tk_inode_s *down;
	void *idata;

	tk_iohandle_t *iohandle;
} tk_inode_t;

int imknod(tk_inode_t * ci, const char *filename, mode_t mode, dev_t dev);
tk_inode_t *isearch(tk_inode_t * ci, const char *s);
char *igetname(const char *s);
void igetpath(char *buff, const char *s);

#endif
