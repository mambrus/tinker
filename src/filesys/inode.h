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


#include <tinker/config.h>
#include <tinker/hixs.h>
#include <filesys/iohandle.h>

typedef enum {
	ISA_IFDIR	= S_IFDIR,	//!< directory file.
	ISA_IFCHR	= S_IFCHR,	//!< character-oriented device file.
	ISA_IFBLK	= S_IFBLK,	//!< block-oriented device file.
	ISA_IFREG	= S_IFREG,	//!< regular file
	ISA_IFLNK	= S_IFLNK,	//!< symbolic link
	ISA_IFSOCK	= S_IFSOCK,	//!< socket
	ISA_IFIFO	= S_IFIFO,	//!< FIFO or pipe
	ISA_UNKNOWN			//!< Unknown type
}tk_mode_t;


typedef struct tk_inode_s{
	int			id;		//!<  A "unique" ID number for this node (a counter)
	char			*name;		//!<  Name of the node
	tk_mode_t		mode;		//!<  What kind of node this is
	struct tk_inode_s 	*belong;	//!<  Whom this node belongs to
	struct tk_inode_s	*next;		//!<  Next node on the same level
	//struct  tk_inode_s	*down;		//!<  Points to first inode that belongs to this one
	struct tk_iohandle_s	*iohande;	//!<  Operations that can be made on this i-node
}tk_inode_t;

#endif //TK_INODE_H

/*
ifdir
ifblk
ifreg
iflnk
ifsock
ififo
*/



