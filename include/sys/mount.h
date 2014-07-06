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

/*!
@file
@ingroup kernel_reimpl_ansi

@brief ANSI sys/mount.h file <b>IMPORTANT:</b> See the note in the detailed section!

For in-depth discussions about re-implementing ANSI functions, see \ref
kernel_reimpl_ansi.

@see kernel_reimpl_ansi

<HR>
*/

//Bits for mounting
/*!
This multibit field contains a magic number. If it does not have the value
MS_MGC_VAL, mount assumes all the following bits are zero and the data
argument is a null string, regardless of their actual values.
*/
#define MS_MGC_MASK	(1<<0)
/*!
This bit on means to remount the filesystem. Off means to mount it.
*/
#define MS_REMOUNT	(1<<1)
/*!
This bit on specifies that no writing to the filesystem shall be allowed
 while it is mounted. This cannot be overridden by ioctl. This option is
available on nearly all filesystems.
*/
#define MS_RDONLY	(1<<2)
/*!
This bit on specifies that no writing to the files in the filesystem shall be
allowed while it is mounted. This can be overridden for a particular file
access by a properly privileged call to ioctl. This option is a relatively new
invention and is not available on many filesystems.
*/
#define S_IMMUTABLE	(1<<3)
/*!
This bit on specifies that the only file writing that shall be allowed while
the filesystem is mounted is appending. Some filesystems allow this to be
overridden for a particular process by a properly privileged call to ioctl.
This is a relatively new invention and is not available on many filesystems.
*/
#define S_APPEND	(1<<4)
/*!
This bit on specifies that Setuid and Setgid permissions on files in the
filesystem shall be ignored while it is mounted.
*/
#define MS_NOSUID	(1<<5)
/*!
This bit on specifies that no files in the filesystem shall be executed while
the filesystem is mounted.
*/
#define MS_NOEXEC	(1<<6)
/*!
This bit on specifies that no device special files in the filesystem shall be
accessible while the filesystem is mounted.
*/
#define MS_NODEV	(1<<7)
/*!
This bit on specifies that all writes to the filesystem while it is mounted
shall be synchronous; i.e. data shall be synced before each write completes
rather than held in the buffer cache.
*/
#define MS_SYNCHRONOUS	(1<<8)
/*!
This bit on specifies that mandatory locks on files shall be permitted while
the filesystem is mounted.
*/
#define MS_MANDLOCK	(1<<9)
/*!
This bit on specifies that access times of files shall not be updated when the
files are accessed while the filesystem is mounted.
*/
#define MS_NOATIME	(1<<10)
/*!
This bit on specifies that access times of directories shall not be updated
when the directories are accessed while the filesystem in mounted.
*/
#define MS_NODIRATIME	(1<<11)
/*!
A so called "magic number". This macro is provided for standard complience.*/
#define MS_MGC_VAL	(1<<16)

//Bits for unmounting

/*!
This bit on means to force the unmounting even if the filesystem is busy,
by making it unbusy first. If the bit is off and the filesystem is busy,
umount2 fails with errno = EBUSY. Depending on the filesystem, this may override
all, some, or no busy conditions.
*/
#define MNT_FORCE	(1<<0)

int mount(const char *special_file, const char *dir, const char *fstype,
	  unsigned long int options, const void *data);
int umount2(const char *file, int flags);

#endif				//MOUNT_H_TK

