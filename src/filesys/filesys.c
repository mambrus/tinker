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
extern struct hixs_t hixs;	//!< The call-structure we know exist.
tk_id_t __fcntr = 0;		//!< Number of open handles
tk_id_t __flid = 0;		//!< Last allocated unique file handle ID (global counter)
int __fs_alive = 0;		//!< Trap will run fs_fini after normal termination.
tk_inode_t *__Rnod;		//!< The root node

int fs_init()
{
	int v = sizeof(void *);
	int i = sizeof(int);
	int d = sizeof(dev_t);

	assert(sizeof(void *) <= sizeof(int));
	assert(sizeof(void *) <= sizeof(dev_t));
	extern __drv_finit_f __DRVINIT_START__;
	extern __drv_finit_f __DRVINIT_END__;
	drv_finit_t drv_init_first = &__DRVINIT_START__;
	drv_finit_t drv_init_last = &__DRVINIT_END__;
	drv_finit_t *drv_init_curr;
	int namelen;
	//extern tk_inode_t     *__Rnod;
	extern tk_id_t __icntr;
	extern tk_id_t __ilid;

	//Copy the current sys-calls structure
	memcpy(&old_syscalls, &hixs, sizeof(struct hixs_t));

	//Set new main-enty values for those we care about
	hixs.close = fs_close;
	hixs.fcntl = fs_fcntl;
	hixs.fstat = fs_fstat;	//HIXS_fstat;
	hixs.isatty = fs_isatty;
	hixs.link = fs_link;
	hixs.lseek = fs_lseek;
	hixs.open = fs_open;
	hixs.read = fs_read;
	hixs.stat = fs_stat;
	hixs.unlink = fs_unlink;
	hixs.write = fs_write;

	std_files[0].read = old_syscalls.read;	//Assign stdin with something useful
	std_files[1].write = old_syscalls.write;	//Assign stdout with something useful
	std_files[2].write = old_syscalls.write;	//Assign stderr with something useful

	//Create the '/' node (directory) - this is a special case and can't be created with mknode
	assure(__Rnod = (tk_inode_t *) calloc(1, sizeof(tk_inode_t)));
	__Rnod->id = __ilid++;
	__icntr++;
	namelen = sizeof("");
	__Rnod->name = (char *)calloc(1, namelen);
	strncpy(__Rnod->name, "", namelen);
	__Rnod->mode = ISA_IFDIR;
	__Rnod->belong = __Rnod;
	__Rnod->down = __Rnod;

	//Create the '/dev' node (directory)
	assure(mknod("/dev", S_IFDIR, 0) == 0);
	assure(mknod("/tmp", S_IFDIR, 0) == 0);

	//Some test nodes
	assert(mknod("/tmp/.ambrmi09", S_IFDIR, 0) == 0);
	assert(mknod("/tmp/afile", S_IFREG, 0) == 0);
	assert(mknod("/tmp/bfile", S_IFREG, 0) == 0);
	assert(mknod("/tmp/.ambrmi09/cfile", S_IFDIR, 0) == 0);

	//Start up the drivers
	const char *dinfo;
	for (drv_init_curr = (drv_finit_t *) drv_init_first;
	     drv_init_curr < (drv_finit_t *) drv_init_last; drv_init_curr++) {
		dinfo = (*drv_init_curr) (NULL);
		assure(dinfo);
		printf("Driver %-60s [started]\n", dinfo);
	}

	__fs_alive = 1;
	return 0;
}

int fs_fini()
{
	if (!__fs_alive) {
		printf("FS allready shut-down...\n");
	} else {
		extern __drv_finit_f __DRVFINI_START__;
		extern __drv_finit_f __DRVFINI_END__;
		drv_finit_t drv_fini_first = &__DRVFINI_START__;
		drv_finit_t drv_fini_last = &__DRVFINI_END__;
		drv_finit_t *drv_fini_curr;
		//extern tk_inode_t *__Rnod;

		//Close down the drivers
		const char *dinfo;
		for (drv_fini_curr = (drv_finit_t *) drv_fini_first;
		     drv_fini_curr < (drv_finit_t *) drv_fini_last;
		     drv_fini_curr++) {
			dinfo = (*drv_fini_curr) (NULL);
			assure(dinfo);
			printf("Driver %-60s [stopped]\n", dinfo);
		}

		free(__Rnod);
		memcpy(&hixs, &old_syscalls, sizeof(struct hixs_t));

		__fs_alive = 0;
	}
	return 0;
}

int tk_dbg_flags(_tk_dbgflag_t * flags, int oflag)
{
	int tmp_oflag = oflag;

/*
//testing the damned macros
int mupp = oflag & (O_RDWR_NL|O_WRONLY_NL);
int tupp = (O_RDWR_NL|O_WRONLY_NL);

int fread = FREAD;
int fwrite = FWRITE;
int frw = (FREAD|FWRITE);

int rdonly = O_RDONLY_NL;
int wronly = O_WRONLY_NL;
int rder = O_RDWR_NL;
*/
	memset(flags, 0, sizeof(_tk_dbgflag_t));

#ifdef O_RDONLY_NL
	assert((oflag & (O_RDWR_NL | O_WRONLY_NL)) != (O_RDWR_NL | O_WRONLY_NL));	//We're counting on that this logic appiles (I.e. both can't be set)
	if (tmp_oflag & O_RDWR_NL) {
		flags->_O_RDWR = 1;
		tmp_oflag &= ~O_RDWR_NL;
	} else {
		if (tmp_oflag & O_WRONLY_NL) {
			flags->_O_WRONLY = 1;
			tmp_oflag &= ~O_WRONLY_NL;
		} else {
			flags->_O_RDONLY = 1;
			tmp_oflag &= ~O_RDONLY_NL;
		}
	}
#else
	if (tmp_oflag & O_RDONLY) {
		flags->_O_RDONLY = 1;
		tmp_oflag &= ~O_RDONLY;
	};
	if (tmp_oflag & O_WRONLY) {
		flags->_O_WRONLY = 1;
		tmp_oflag &= ~O_WRONLY;
	};
	if (tmp_oflag & O_RDWR) {
		flags->_O_RDWR = 1;
		tmp_oflag &= ~O_RDWR;
	};
#endif
	if (tmp_oflag & O_APPEND) {
		flags->_O_APPEND = 1;
		tmp_oflag &= ~O_APPEND;
	};
	if (tmp_oflag & O_CREAT) {
		flags->_O_CREAT = 1;
		tmp_oflag &= ~O_CREAT;
	};
/*
	if (tmp_oflag & O_DSYNC){
		flags->_O_DSYNC=1;
		tmp_oflag &= ~O_DSYNC;
	};
*/
	if (tmp_oflag & O_EXCL) {
		flags->_O_EXCL = 1;
		tmp_oflag &= ~O_EXCL;
	};
	if (tmp_oflag & O_NOCTTY) {
		flags->_O_NOCTTY = 1;
		tmp_oflag &= ~O_NOCTTY;
	};
	if (tmp_oflag & O_NONBLOCK) {
		flags->_O_NONBLOCK = 1;
		tmp_oflag &= ~O_NONBLOCK;
	};
/*
	if (tmp_oflag & O_RSYNC){
		flags->_O_RSYNC=1;
		tmp_oflag &= ~O_RSYNC;
	};
*/
	if (tmp_oflag & O_SYNC) {
		flags->_O_SYNC = 1;
		tmp_oflag &= ~O_SYNC;
	};
	if (tmp_oflag & O_TRUNC) {
		flags->_O_TRUNC = 1;
		tmp_oflag &= ~O_TRUNC;
	};

	//If all known flags are tested, return value should be 0
	return tmp_oflag;

}

typedef struct {
	int _O_RDONLY;
	int _O_WRONLY;
	int _O_RDWR;
	int _O_APPEND;
	int _O_CREAT;
//      int _O_DSYNC;
	int _O_EXCL;
	int _O_NOCTTY;
	int _O_NONBLOCK;
//      int _O_RSYNC;
	int _O_SYNC;
	int _O_TRUNC;
} _dbg_flag_t;

/*!
http://www.opengroup.org/onlinepubs/009695399/
*/
int mknod(const char *filename, mode_t mode, dev_t dev)
{
	return imknod(__Rnod, filename, mode, dev);
}

/*
EPERM
    The process is not superuser.
ENODEV
    The file system type fstype is not known to the kernel.
ENOTBLK
    The file dev is not a block device special file.
EBUSY

        * The device is already mounted.
        * The mount point is busy. (E.g. it is some process' working directory or has a filesystem mounted on it already).
        * The request is to remount read-only, but there are files open for write.

EINVAL

        * A remount was attempted, but there is no filesystem mounted over the specified mount point.
        * The supposed filesystem has an invalid superblock.

EACCES

        * The filesystem is inherently read-only (possibly due to a switch on the device) and the process attempted to mount it read/write (by setting the MS_RDONLY bit off).
        * special_file or dir is not accessible due to file permissions.
        * special_file is not accessible because it is in a filesystem that is mounted with the MS_NODEV option.

EM_FILE
    The table of dummy devices is full. mount needs to create a dummy device (aka "unnamed" device) if the filesystem being mounted is not one that uses a device.
*/
#include <sys/mount.h>

/*!
@brief http://www.gnu.org/software/libc/manual/html_mono/libc.html#Mount-Unmount-Remount

@Note that TinKer permits the same device to be mounted on different places in
the current name-space at the same time. Therefore - to unmount you have to do
this on the directory. If you try unmounting on the device, TinKer will not
know which one of the (potential) several mountpoints you mean.
*/
int mount(const char *special_file,
	  const char *dir,
	  const char *fstype, unsigned long int options, const void *data)
{
	extern tk_inode_t *__Rnod;
	tk_inode_t *device_file;
	tk_inode_t *mount_dir;

	device_file = isearch(__Rnod, special_file);
	if (device_file == NULL) {
		errno = EINVAL;
		return -1;
	}
	if (!(device_file->mode & S_IFBLK)) {
		errno = ENOTBLK;
		return -1;
	}
	mount_dir = isearch(__Rnod, dir);
	if (mount_dir == NULL) {
		errno = EINVAL;
		return -1;
	}
	if (!(mount_dir->mode & S_IFDIR)) {
		errno = EACCES;
		return -1;
	}
	if (mount_dir->mount != NULL) {
		errno = EBUSY;
		return -1;
	}

	mount_dir->mount = calloc(1, sizeof(tk_mount_t));
	if (mount_dir->mount == NULL) {	//We lack error code for this case in the spec
		errno = ENOMEM;
		return -1;
	}
	mount_dir->mount->io_device = device_file->iohandle;
	mount_dir->mount->options = options;
	return 0;
}

/*!
@brief http://www.gnu.org/software/libc/manual/html_mono/libc.html#Mount-Unmount-Remount

@Note that TinKer permits the same device to be mounted on different places in
the current name-space at the same time. Therefore - to unmount you have to do
this on the directory. If you try unmounting on the device, TinKer will not
know which one of the (potential) several mountpoints you mean.
*/
int umount2(const char *file, int flags)
{
	extern tk_inode_t *__Rnod;
	tk_inode_t *mount_dir;

	mount_dir = isearch(__Rnod, file);
	if (mount_dir == NULL) {
		errno = EINVAL;
		return -1;
	}
	if (mount_dir->mount == NULL) {
		errno = EINVAL;
		return -1;
	}
	free(mount_dir->mount);
	mount_dir->mount = NULL;
	return 0;
}
