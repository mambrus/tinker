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
#ifndef TK_MEM_H
#define TK_MEM_H

#if defined(__GNUC__)
#include <tinker/config.h>
#if (!defined(TK_COMP_KMEM) || (TK_COMP_KMEM==0) ) && !defined(ECODES_ONLY)
#error "<tk_mem.h> belongs to a component that your build of TinKer didn't include. Please reconfigure and rebuild TinKer."
#endif
#endif

#include <stddef.h>

#define ECODES_ONLY
#include <tk_ptime.h>
#undef ECODES_ONLY

enum KMEM_ERROR_CODES {

	ERR_UNDEF_HEAPID = ERR_PTIMER_SENTINEL,
	ERR_KMEM_SENTINEL
};

#define KMEM_KEEP_UNINITIALIZED (1<<0)
#define KMEM_BLANK_ZERO (1<<1)
#define KMEM_BLANK_ID (1<<2)

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif
	typedef void l_func(void);
	typedef l_func *lock_f;
	typedef void u_func(void);
	typedef u_func *unlock_f;
	typedef struct {
		int maxnum;
		int dsize;
		int blocks;
		int indx;
		char *heap;
		int is_malloced;
		lock_f lock;
		unlock_f unlock;
		void *self;
		char *last;
	} heap_t;
	typedef heap_t *heapid_t;
	unsigned long tk_mem(void);
	unsigned long tk_mem_destruct(void);
	unsigned long tk_create_heap(heapid_t *, int, int, int, lock_f,
				     unlock_f, char *);
	unsigned long tk_destroy_heap(heapid_t);
	void *tk_mem_malloct(heapid_t, size_t);
	void tk_mem_free(heapid_t, void *);

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#endif
