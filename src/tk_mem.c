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
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <tk_mem.h>

#if defined (__GNUC__)
#include <tinker/config.h>
#endif

#ifndef TK_KMEM_NHEAPS

#define TK_KMEM_NHEAPS 0x8
#endif

heap_t header_pool[TK_KMEM_NHEAPS];
int cheap_idx = 0;
int nheaps = 0;
unsigned long tk_mem(void)
{
	int i;
	for (i = 0; i < TK_KMEM_NHEAPS; i++) {
		memset(&header_pool[i], 0, sizeof(heap_t));
	}
	return ERR_OK;
}

unsigned long tk_mem_destruct(void)
{
	int i;
	for (i = 0; i < TK_KMEM_NHEAPS; i++) {
		if (header_pool[i].heap && header_pool[i].is_malloced)
			free(header_pool[i].heap);
	}
	return ERR_OK;
}

unsigned long tk_create_heap(heapid_t * heapid,
			     int hsize,
			     int dsize,
			     int opt,
			     lock_f lock, unlock_f unlock, char *heap_ptr)
{
	int i, found_ID;
	int esz;
	void *ptr;
	int mxnum;

	esz = sizeof(int) + dsize;
	mxnum = hsize / esz;
	*heapid = NULL;

	if (nheaps >= TK_KMEM_NHEAPS) {
		*heapid = NULL;
		return ENOMEM;
	}

	for (i = 0, found_ID = TK_KMEM_NHEAPS + 1;
	     i < TK_KMEM_NHEAPS && found_ID > TK_KMEM_NHEAPS; i++) {
		if (header_pool[cheap_idx].heap == NULL) {
			*heapid = &header_pool[cheap_idx];
			found_ID = cheap_idx;
		}
		cheap_idx++;
		cheap_idx %= TK_KMEM_NHEAPS;
	}
	assert(*heapid != NULL);

	if (heap_ptr == NULL) {
		(*heapid)->heap = malloc(hsize);
		(*heapid)->is_malloced = 1;
	} else {
		(*heapid)->heap = heap_ptr;
		(*heapid)->is_malloced = 0;
	}

	if ((*heapid)->heap == NULL) {

		memset(*heapid, 0, sizeof(heap_t));

		*heapid = NULL;
		return ENOMEM;
	}

	(*heapid)->lock = lock;
	(*heapid)->unlock = unlock;

	(*heapid)->self = *heapid;
	(*heapid)->dsize = dsize;
	(*heapid)->maxnum = mxnum;
	(*heapid)->blocks = 0;
	(*heapid)->indx = 0;

	ptr = (*heapid)->heap;

	if (opt & KMEM_KEEP_UNINITIALIZED) {

		for (i = 0; i < mxnum; i++) {
			if (*(int *)ptr != 0) {
				(*heapid)->blocks++;
			} else {
				(*heapid)->indx = i;
			}
		}
		ptr = (char *)ptr + esz;
	} else {
		if (opt & KMEM_BLANK_ZERO) {
			memset((*heapid)->heap, 0, hsize);
		} else if (opt & KMEM_BLANK_ID) {
			memset((*heapid)->heap, found_ID, hsize);
		}

		if (!(opt & KMEM_BLANK_ZERO)) {
			for (i = 0; i < mxnum; i++) {
				*(int *)ptr = 0;
				ptr = (char *)ptr + esz;
			}
		}
	}

	(*heapid)->last = ptr;
	nheaps++;

	return ERR_OK;
}

unsigned long tk_destroy_heap(heapid_t heapid)
{
	if (heapid->self == NULL)
		return ERR_UNDEF_HEAPID;
	if (heapid->self != (void *)heapid)
		return ERR_UNDEF_HEAPID;
	if (heapid->blocks != 0)
		return EBUSY;

	if (heapid->is_malloced)
		free(heapid->heap);

	memset(heapid, 0, sizeof(heap_t));
	nheaps--;

	return ERR_OK;
}

void *tk_mem_malloct(heapid_t heapid, size_t size)
{
	int i;
	int esz;
	void *ptr;

	if (heapid->self == NULL) {

		return NULL;
	}
	if (heapid->self != (void *)heapid) {

		return NULL;
	}
	if (heapid->blocks >= heapid->maxnum) {

		return NULL;
	}

	if (heapid->lock)
		heapid->lock();

	esz = sizeof(int) + heapid->dsize;
	ptr = (char *)(heapid->heap) + esz * heapid->indx;

	for (i = heapid->indx; *(int *)ptr;) {
		i++;
		i %= heapid->maxnum;
		ptr = (char *)(heapid->heap) + esz * i;
	}
	i++;
	i %= heapid->maxnum;
	heapid->indx = i;
	heapid->blocks++;

	*(int *)ptr = 1;

	ptr = (char *)ptr + sizeof(int);

	if (heapid->unlock)
		heapid->unlock();

	return ptr;
}

void tk_mem_free(heapid_t heapid, void *ptr)
{
	if (heapid->self == NULL) {

		return;
	}
	if (heapid->self != (void *)heapid) {

		return;
	}
	if (heapid->blocks >= heapid->maxnum) {

		return;
	}
	if (heapid->lock)
		heapid->lock();

	ptr = (char *)ptr - sizeof(int);
	heapid->blocks--;
	*(int *)ptr = 0;

	if (heapid->unlock)
		heapid->unlock();
}
