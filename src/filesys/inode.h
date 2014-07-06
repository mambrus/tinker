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

//! Identity typedef used in inode and fhandle stucts
typedef int tk_id_t;

/*!
Pure mode names (to aid debugging - note that any mode variable
*theoretically* can be OR'ed among these which means any stabs info
will not be able to resolve the name.

This is however very unlikely an is furthermore  of no concern for
the running code.
*/

typedef enum {
	ISA_UNKNOWN = 0,	//!< Unknown type
	ISA_IFDIR = S_IFDIR,	//!< directory file.
	ISA_IFCHR = S_IFCHR,	//!< character-oriented device file.
	ISA_IFBLK = S_IFBLK,	//!< block-oriented device file.
	ISA_IFREG = S_IFREG,	//!< regular file
	ISA_IFLNK = S_IFLNK,	//!< symbolic link
	ISA_IFSOCK = S_IFSOCK,	//!< socket
	ISA_IFIFO = S_IFIFO	//!< FIFO or pipe
} tk_mode_t;

typedef struct tk_mount_s {
	tk_iohandle_t *io_device;	//!<  Driver entries to mounted low-level device
	tk_iohandle_t *io_fstype;	//!<  Entries to mounted fstype (future feature - now always NULL);
	//const char *fstype;
	unsigned long int options;
	//const void *data;
} tk_mount_t;

typedef struct tk_inode_s {
	tk_id_t id;		//!<  A "unique" ID number for this node (a counter)
	char *name;		//!<  Name of the node
	tk_mode_t mode;		//!<  What kind of node this is
	struct tk_mount_s *mount;	//!<  If this is a mount-point this field contains the mount options. If not, this is zero.
	struct tk_inode_s *belong;	//!<  Whom this node belongs to
	struct tk_inode_s *next;	//!<  Next node on the same level
	struct tk_inode_s *down;	//!<  Points to first inode that belongs to this one
	void *idata;		/*!<  Certain IO can instantiate driver multiple times.
				   This field is for instance specific driver data. */
	tk_iohandle_t *iohandle;	//!<  Operations that can be made on this i-node
} tk_inode_t;

int imknod(tk_inode_t * ci, const char *filename, mode_t mode, dev_t dev);
tk_inode_t *isearch(tk_inode_t * ci, const char *s);
char *igetname(const char *s);
void igetpath(char *buff, const char *s);

#endif				//TK_INODE_H

/*
ifdir
ifblk
ifchr
ifreg
iflnk
ifsock
ififo
*/

/*

			node
			  |

  ^|		  ^		  ^		  ^
  |v		  |		  |		  |
node  ->	node  ->	node  ->	node  ->	NULL
		  |

	^|		  ^		  ^		  ^
	|v		  |		  |		  |
	node  ->	node  ->	node  ->	node  ->	NULL

*/
