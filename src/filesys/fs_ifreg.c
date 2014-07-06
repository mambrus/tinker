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
#include "inode.h"
#include <string.h>
#include <tk_mem.h>

#define DRV_IO_NAME(x,y) \
 x ##y

#define DRV_IO(x) \
 DRV_IO_NAME( fs_ifreg_ , x )

static const char DRV_IO(assert_info)[] =
    "You're trying to access a non implemented function";

#define SECTOR_SIZE 48
struct sector_t {
	struct sector_t *prev;
	char data[SECTOR_SIZE - 2 * sizeof(struct sector_t *)];
	struct sector_t *next;
};

#define DATA_IN_SECTOR_SIZE (SECTOR_SIZE-2*sizeof(struct sector_t *))

typedef struct {
	char *start;
	int size;
	int sectorsize;
	int isratio;
	int opt;

	heapid_t inode_heap;
	heapid_t sector_heap;
} DRV_IO(drv_data_t);

typedef struct {
	char *ptr;
	int sidx;
	int didx;
	struct sector_t *csector;
} DRV_IO(hndl_data_t);

typedef struct {
	int maxsize;
	int dsize;
	struct sector_t *sectors;
} DRV_IO(inode_data_t);

int DRV_IO(close) (int file)
{
	tk_fhandle_t *hndl = (tk_fhandle_t *) file;
	DRV_IO(hndl_data_t) * hdata;
	int oflag = ((tk_fhandle_t *) file)->oflag;

	hdata = ((DRV_IO(hndl_data_t) *) (hndl->data));
#if DEBUG
	memset(hdata, 0, sizeof(DRV_IO(hndl_data_t)));
#endif
	free(hdata);

#if DEBUG
	memset(hndl, 0, sizeof(tk_fhandle_t));
#endif
	free(hndl);

	return 0;
}

int DRV_IO(open) (const char *filename, int flags, ...) {
	va_list ap;
	tk_fhandle_t *hndl;
	tk_inode_t *inode;
	mode_t accessflg;
	va_start(ap, flags);
	if (flags & O_CREAT) {
		accessflg = va_arg(ap, mode_t);
		inode = va_arg(ap, tk_inode_t *);
		assert(accessflg == 0);
	} else {
		inode = va_arg(ap, tk_inode_t *);
	}
	va_end(ap);

	hndl = tk_new_handle(inode, flags);

	hndl->data = calloc(1, sizeof(DRV_IO(hndl_data_t)));

	if (flags & O_CREAT) {
		if (inode->idata == NULL) {

			inode->idata = malloc(sizeof(DRV_IO(inode_data_t)));
			((DRV_IO(hndl_data_t) *) (hndl->data))->csector =
			    malloc(SECTOR_SIZE);
			memset(((DRV_IO(hndl_data_t) *) (hndl->data))->csector,
			       0, SECTOR_SIZE);

			((DRV_IO(inode_data_t) *) (inode->idata))->sectors =
			    ((DRV_IO(hndl_data_t) *) (hndl->data))->csector;
			((DRV_IO(inode_data_t) *) (inode->idata))->maxsize =
			    DATA_IN_SECTOR_SIZE;
			((DRV_IO(inode_data_t) *) (inode->idata))->dsize = 0;

		} else {

			if (!(flags & O_APPEND)) {

				errno = EINVAL;
				return -1;
			}

		}
	}

	((DRV_IO(hndl_data_t) *) (hndl->data))->csector =
	    ((DRV_IO(inode_data_t) *) (inode->idata))->sectors;
	((DRV_IO(hndl_data_t) *) (hndl->data))->ptr =
	    ((DRV_IO(hndl_data_t) *) (hndl->data))->csector->data;

	((DRV_IO(hndl_data_t) *) (hndl->data))->sidx = 0;
	((DRV_IO(hndl_data_t) *) (hndl->data))->didx = 0;

	return (int)hndl;

}

int DRV_IO(fstat) (int file, struct stat * st) {

	tk_fhandle_t *hndl = (tk_fhandle_t *) file;
	st->st_mode = hndl->inode->mode;;

	return 0;
}

int DRV_IO(lseek) (int file, int ptr, int dir) {
	tk_fhandle_t *hndl = (tk_fhandle_t *) file;
	DRV_IO(hndl_data_t) * hdata;
	DRV_IO(inode_data_t) * idata;
	int oflag = ((tk_fhandle_t *) file)->oflag;
#if DEBUG
	_tk_dbgflag_t flags;
#endif

	assert(tk_dbg_flags(&flags, oflag) == 0);

	hdata = ((DRV_IO(hndl_data_t) *) (hndl->data));
	idata = ((DRV_IO(inode_data_t) *) (hndl->inode->idata));

	switch (dir) {
	case SEEK_SET:
		assert("the file offset shall be set to offset bytes" == NULL);
		break;
	case SEEK_CUR:
		assert
		    ("the file offset shall be set to its current location plus offset"
		     == NULL);
		break;
	case SEEK_END:
		{

			struct sector_t *tsect;
			struct sector_t *tsect2;
			int nsect;
			for (tsect = idata->sectors, nsect = 0; tsect->next;
			     tsect = tsect->next, nsect++) ;
			tsect2 = hdata->csector;
			hdata->csector = tsect;
			if (ptr + idata->dsize > idata->maxsize) {
				if (oflag & O_APPEND) {
					hdata->csector->next =
					    malloc(SECTOR_SIZE);
					if (hdata->csector->next == NULL) {
						errno = ENOSPC;
						hdata->csector = tsect2;
						return -1;
					}
					memset(hdata->csector->next, 0,
					       SECTOR_SIZE);
					hdata->csector->next->prev =
					    hdata->csector;
					idata->maxsize +=
					    SECTOR_SIZE -
					    2 * sizeof(struct sector_t *);
				} else {
					errno = EFBIG;
					return -1;
				}
			}

			hdata->didx += ptr;
			idata->dsize = hdata->didx;

			hdata->sidx = hdata->didx - nsect * DATA_IN_SECTOR_SIZE;
			hdata->ptr = &hdata->csector->data[hdata->sidx];
		}
		break;
	default:
		assert("Unknown direction for lseek" == 0);
		errno = EINVAL;
		return -1;
	}
	return 0;
}

int DRV_IO(read) (int file, char *ptr, int len) {
	tk_fhandle_t *hndl = (tk_fhandle_t *) file;
	DRV_IO(hndl_data_t) * hdata;
	DRV_IO(inode_data_t) * idata;
	int oflag = ((tk_fhandle_t *) file)->oflag;
	int n;
#if DEBUG
	_tk_dbgflag_t flags;
#endif

	assert(tk_dbg_flags(&flags, oflag) == 0);

	hdata = ((DRV_IO(hndl_data_t) *) (hndl->data));
	idata = ((DRV_IO(inode_data_t) *) (hndl->inode->idata));

	if (oflag & (O_WRONLY_NL)) {
		errno = EBADF;
		return -1;
	}
	if (hdata->didx > idata->dsize) {

		return 0;
	}
	if (DATA_IN_SECTOR_SIZE - hdata->sidx >= len) {

		memcpy(ptr, &hdata->csector->data[hdata->sidx], len);
		hdata->sidx += len;
		hdata->didx += len;
		n = len;
	} else {
		int slen, nleft = len;
		int _eof = 0;

		for (n = 0, nleft = len, slen =
		     DATA_IN_SECTOR_SIZE - hdata->sidx; nleft && !_eof;) {
			if (nleft > slen) {
				memcpy(ptr, &hdata->csector->data[hdata->sidx],
				       slen);
				ptr += slen;
				n += slen;
				nleft -= slen;
				hdata->sidx += slen;
				hdata->didx += slen;
			} else {
				memcpy(ptr, &hdata->csector->data[hdata->sidx],
				       nleft);
				ptr += nleft;
				n += nleft;
				nleft = 0;
				hdata->sidx += nleft;
				hdata->didx += nleft;
			}
			if (nleft) {
				if (hdata->csector->next != NULL) {
					slen = DATA_IN_SECTOR_SIZE;
					hdata->csector = hdata->csector->next;
					hdata->sidx = 0;
				} else {
					_eof = 1;
				}
			}
		}
	}
	return n;
}

int DRV_IO(write) (int file, char *ptr, int len) {
	tk_fhandle_t *hndl = (tk_fhandle_t *) file;
	DRV_IO(hndl_data_t) * hdata;
	DRV_IO(inode_data_t) * idata;
	int oflag = ((tk_fhandle_t *) file)->oflag;
	int n;
#if DEBUG
	_tk_dbgflag_t flags;
#endif

	assert(tk_dbg_flags(&flags, oflag) == 0);

	hdata = ((DRV_IO(hndl_data_t) *) (hndl->data));
	idata = ((DRV_IO(inode_data_t) *) (hndl->inode->idata));

	if (!(oflag & (O_RDWR_NL | O_WRONLY_NL))) {
		errno = EBADF;
		return -1;
	}

	if (len + hdata->didx > idata->dsize) {

		if (oflag & O_APPEND) {

			if (len + hdata->didx > idata->maxsize) {

				int i;
				struct sector_t *tsect;

				tsect = hdata->csector;
				for (i = 0;
				     len + hdata->didx > idata->maxsize; i++) {
					if (tsect->next == NULL) {
						tsect->next =
						    malloc(SECTOR_SIZE);
						memset(tsect->next, 0,
						       SECTOR_SIZE);
						idata->maxsize +=
						    DATA_IN_SECTOR_SIZE;
						tsect->next->prev = tsect;
					}
					tsect = tsect->next;
				}

			}
			if (DATA_IN_SECTOR_SIZE - hdata->sidx >= len) {

				memcpy(hdata->ptr, ptr, len);
				hdata->ptr += len;
				hdata->didx += len;
				hdata->sidx += len;
				n = len;
			} else {

				int slen, nleft = len;
				int _eof = 0;

				for (n = 0, nleft = len, slen =
				     DATA_IN_SECTOR_SIZE - hdata->sidx;
				     nleft && !_eof;) {
					if (nleft > slen) {
						memcpy(&hdata->csector->
						       data[hdata->sidx], ptr,
						       slen);
						ptr += slen;
						n += slen;
						nleft -= slen;
						hdata->sidx += slen;
						hdata->didx += slen;

					} else {
						memcpy(&hdata->csector->
						       data[hdata->sidx], ptr,
						       nleft);
						ptr += nleft;
						n += nleft;
						nleft = 0;
						hdata->sidx += nleft;
						hdata->didx += nleft;
					}
					if (nleft) {
						if (hdata->csector->next !=
						    NULL) {
							slen =
							    DATA_IN_SECTOR_SIZE;
							hdata->csector =
							    hdata->csector->
							    next;
							hdata->sidx = 0;
						} else {
							_eof = 1;
						}
					}
				}

				assert(_eof == 0);

				assert(hdata->csector->next == NULL);
			}

			idata->dsize = hdata->didx;
		} else {
			errno = ENOSPC;
			return -1;
		}
	} else {
		assert("writing in mid file (TBD)" == NULL);

		if (DATA_IN_SECTOR_SIZE - hdata->sidx >= len) {
			memcpy(hdata->ptr, ptr, len);
		} else {
			assert("TBD" == NULL);
		}
	}

	assert(n == len);
	return n;
}

int DRV_IO(fcntl) (int file, int command, ...) {
	assert(DRV_IO(assert_info) == NULL);
	errno = ENOSYS;
	return -1;
}

int DRV_IO(isatty) (int file) {
	assert(DRV_IO(assert_info) == NULL);
	return 1;
}

int DRV_IO(link) (char *old, char *new) {
	assert(DRV_IO(assert_info) == NULL);
	errno = EMLINK;
	return -1;
}

int DRV_IO(stat) (const char *file, struct stat * st) {
	assert(DRV_IO(assert_info) == NULL);
	st->st_mode = S_IFCHR;
	return 0;
}

int DRV_IO(unlink) (char *name) {
	assert(DRV_IO(assert_info) == NULL);
	errno = ENOENT;
	return -1;
}

const tk_iohandle_t DRV_IO(io) = {
	DRV_IO(close),
	    DRV_IO(fcntl),
	    DRV_IO(fstat),
	    DRV_IO(isatty),
	    DRV_IO(link), DRV_IO(lseek), DRV_IO(open), DRV_IO(read),
	    DRV_IO(stat), DRV_IO(unlink), DRV_IO(write)
};

int DRV_IO(init) (char *path,
		  char *start, int size, int sectorsize, int isratio, int opt) {
	int file;
	tk_inode_t *inod;
	DRV_IO(drv_data_t) * drvdata;
	int ninodes;
	int nsectors;
	int rc;
	int kmemopts;
	extern tk_inode_t *__Rnod;

	assure(mknod(path, S_IFBLK, (dev_t) & DRV_IO(io)) == 0);
	inod = isearch(__Rnod, path);
	assert(inod);
	inod->idata =
	    (DRV_IO(drv_data_t) *) (calloc(1, sizeof(DRV_IO(drv_data_t))));

	drvdata = (DRV_IO(drv_data_t) *) (inod->idata);
	if (drvdata == NULL)
		return ENOMEM;
	nsectors = size / sectorsize;
	ninodes = nsectors / isratio;

	nsectors -= ninodes;

	nsectors =
	    (size * isratio) / (sizeof(tk_inode_t) + isratio * sectorsize);
	ninodes = nsectors / isratio;

	if (sizeof(tk_inode_t) < sectorsize)
		ninodes--;
	else
		nsectors--;

	if (RAMDISK_KEEP_OLD_DATA) {
		kmemopts = KMEM_KEEP_UNINITIALIZED;
	} else {
		kmemopts = KMEM_BLANK_ID;
	}

	rc = tk_create_heap(&drvdata->inode_heap,
			    sizeof(tk_inode_t),
			    ninodes * sizeof(tk_inode_t),
			    kmemopts, NULL, NULL, start);
	if (rc != 0) {
		memset(drvdata, 0, sizeof(DRV_IO(drv_data_t)));
		free(drvdata);
		return rc;
	}
	rc = tk_create_heap(&drvdata->sector_heap,
			    sectorsize,
			    nsectors * sectorsize,
			    kmemopts, NULL, NULL, drvdata->inode_heap->last);
	if (rc != 0) {
		tk_destroy_heap(drvdata->inode_heap);
		memset(drvdata, 0, sizeof(DRV_IO(drv_data_t)));
		free(drvdata);
		return rc;
	}
#ifdef DEBUG
	char *tptr = start + size;
	int unused = (int)(tptr) - (int)(drvdata->sector_heap->last);
	assure(unused >= 0);
#endif
	assert(drvdata->sector_heap->last <= (start + size));

	drvdata->start = start;
	drvdata->size = size;
	drvdata->sectorsize = sectorsize;
	drvdata->isratio = isratio;
	drvdata->opt = opt;
}

int DRV_IO(fini) (int file) {
	tk_fhandle_t *hndl = (tk_fhandle_t *) file;
	DRV_IO(drv_data_t) * drvdata;

	drvdata = (DRV_IO(drv_data_t) *) (hndl->inode->idata);

	tk_destroy_heap(drvdata->inode_heap);
	tk_destroy_heap(drvdata->sector_heap);
	memset(drvdata, 0, sizeof(DRV_IO(drv_data_t)));
	free(drvdata);

	return unlink(hndl->inode->name);
}
