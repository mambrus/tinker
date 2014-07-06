/***************************************************************************
 *   Copyright (C) 2006 by Michael Ambrus                                  *
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
#ifndef MOUNT_H_TK
#define MOUNT_H_TK

#define MS_MGC_MASK (1<<0)
#define MS_REMOUNT (1<<1)
#define MS_RDONLY (1<<2)
#define S_IMMUTABLE (1<<3)
#define S_APPEND (1<<4)
#define MS_NOSUID (1<<5)
#define MS_NOEXEC (1<<6)
#define MS_NODEV (1<<7)
#define MS_SYNCHRONOUS (1<<8)
#define MS_MANDLOCK (1<<9)
#define MS_NOATIME (1<<10)
#define MS_NODIRATIME (1<<11)
#define MS_MGC_VAL (1<<16)
#define MNT_FORCE (1<<0)

int mount(const char *special_file, const char *dir, const char *fstype,
	  unsigned long int options, const void *data);
int umount2(const char *file, int flags);

#endif
