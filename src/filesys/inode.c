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

#include <filesys/inode.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#ifndef PATH_MAX
#include <sys/syslimits.h>
#endif

tk_id_t __icntr = 0;
tk_id_t __ilid = 0;

char *igetname(const char *s)
{
	int i;
	for (i = strlen(s); s[i] != '/'; i--) ;
	return (char *)&s[i + 1];
}

void igetpath(char *buff, const char *s)
{
	strncpy(buff, s, PATH_MAX);
	igetname(buff)[0] = '\0';
}

tk_inode_t *isearch(tk_inode_t * ci, const char *s)
{
	char fname[PATH_MAX];
	char *cd, *nd;

	strncpy(fname, s, PATH_MAX);

	for (cd = "", nd = strtok(fname, "/");
	     nd; cd = nd, nd = strtok(NULL, "/")
	    ) {
		if (ci->mount)
			return ci;

		if (ci) {

			ci = ci->down;
		}
		for (; ci && strncmp(ci->name, nd, NAME_MAX); ci = ci->next) ;

	}

	return ci;
}

int tk_rmnod(const char *filename)
{
	__icntr--;
}

int imknod(tk_inode_t * ci, const char *filename, mode_t mode, dev_t dev)
{
	char path[PATH_MAX];
	char *name;
	tk_inode_t *belong;
	tk_inode_t *newNode;
	tk_inode_t *seekNode;
	int namelen;

	if (!(strnlen(filename, PATH_MAX) < PATH_MAX)) {
		errno = ENAMETOOLONG;
		return -1;
	}

	igetpath(path, filename);
	name = igetname(filename);

	if (!((namelen = strnlen(name, NAME_MAX)) < NAME_MAX)) {
		errno = ENAMETOOLONG;
		return -1;
	}

	if (belong = isearch(ci, filename)) {

		errno = EEXIST;
		return -1;
	}
	if (!(belong = isearch(ci, path))) {

		errno = ENOENT;
		return -1;
	}
	if (!(belong->mode | ISA_IFDIR)) {
		errno = ENOTDIR;
		return -1;
	}

	assure(newNode = (tk_inode_t *) calloc(1, sizeof(tk_inode_t)));
	memset(newNode, 0, sizeof(tk_inode_t));
	__icntr++;
	newNode->id = __ilid++;
	newNode->name = (char *)calloc(1, namelen);
	strncpy(newNode->name, name, namelen);
	newNode->mode = mode;
	if (dev != 0) {
		newNode->iohandle = (tk_iohandle_t *) dev;
	} else {

		extern tk_iohandle_t fs_ifdir_io;
		extern tk_iohandle_t fs_ifblk_io;
		extern tk_iohandle_t fs_ifchr_io;
		extern tk_iohandle_t fs_ifreg_io;
		extern tk_iohandle_t fs_iflnk_io;
		extern tk_iohandle_t fs_ifsock_io;
		extern tk_iohandle_t fs_ififo_io;
		tk_mode_t type = mode;

		switch (type) {
		case ISA_IFDIR:
			newNode->iohandle = &fs_ifdir_io;
			break;
		case ISA_IFCHR:
			newNode->iohandle = &fs_ifchr_io;
			break;
		case ISA_IFBLK:
			newNode->iohandle = &fs_ifblk_io;
			break;
		case ISA_IFREG:
			newNode->iohandle = &fs_ifreg_io;
			break;
		case ISA_IFLNK:
			newNode->iohandle = &fs_iflnk_io;
			break;
		case ISA_IFSOCK:
			newNode->iohandle = &fs_ifsock_io;
			break;
		case ISA_IFIFO:
			newNode->iohandle = &fs_ififo_io;
			break;
			assert
			    ("Can't figure out what kind of device you're trying to mknod"
			     == NULL);
		}
	}

	newNode->belong = belong;

	if (belong->down != belong) {
		assert(belong->down);

		for (seekNode = belong->down; seekNode->next;
		     seekNode = seekNode->next) ;

		seekNode->next = newNode;
	} else {

		belong->down = newNode;
	}

	newNode->down = newNode;
	return 0;
}
