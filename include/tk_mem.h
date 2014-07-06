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

/*!
@file
@ingroup KMEM

@brief A better malloc

TBD

For in-depth discussions about this component, see \ref
KMEM

@see KMEM
@see COMPONENTS
*/

#ifndef TK_MEM_H
#define TK_MEM_H

#if defined(__GNUC__)
#include <tinker/config.h>
#if (!defined(TK_COMP_KMEM) || (TK_COMP_KMEM==0) ) && !defined(ECODES_ONLY)
#error "<tk_mem.h> belongs to a component that your build of TinKer didn't include. Please reconfigure and rebuild TinKer."
#endif
#endif

#include <stddef.h>		//Needed for size_t

#define ECODES_ONLY
#include <tk_ptime.h>		//last in error chain
#undef ECODES_ONLY

/*- Error codes **/

enum KMEM_ERROR_CODES {

	ERR_UNDEF_HEAPID = ERR_PTIMER_SENTINEL,	/*!< @brief No such or invalid \ref heap_t "heap" */
	ERR_KMEM_SENTINEL
};

/*Bitfields controlling creation of a heap*/
#define		KMEM_KEEP_UNINITIALIZED 	(1<<0)
#define		KMEM_BLANK_ZERO			(1<<1)
#define		KMEM_BLANK_ID			(1<<2)

#include <tinker/config.h>
#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
extern "C" {
#endif

/*!
@name Function specification for locking "malloc" and "free" operations on a particular heap.

This is for parameters relating to a mechanism that either contains a
- Taking a semaphore or mutex, where such as available and needed
- Clearing of interrupt abiliyt, where this is the only means of
locking/un-locking
- Null, where locking is not necessary
*/
//@{
	typedef void l_func(void);
	typedef l_func *lock_f;
//@}

/*!
@name Function specification for un-locking "malloc" and "free" operations on a particular heap.

This is for parameters relating to a mechanism that either contains a
- Giving back a semaphore or mutex, where such as available and needed
- Setting of interrupt abiliyt, where this is the only means of
locking/un-locking
- Null, where locking is not necessary
*/
//@{typedef unsigned int u_func(void *);
	typedef void u_func(void);
	typedef u_func *unlock_f;
//@}

/*!

@brief Organisational structure of each heap

There should not be any reason for a normal user to address this
structure explicitly. Use \ref heapid_t for normal operations instead.
*/
	typedef struct {
		int maxnum;	//!< Maximum number of elements this pool can hold.
		int dsize;	//!< Size of each element
		int blocks;	//!< Current number of blocks in use
		int indx;	//!< Points at block firts after the last allocated block (probable for next alloc).
		char *heap;	//!< Pointer to the allocatded memory, NULL if unallocated. I.e. NULL also means this header is avaiable
		int is_malloced;	//!< If heap is on global heap or if it was provided extrernally
		lock_f lock;	//!< Function for un-locking acces when operation on the heap. NULL if no locking is needed.
		unlock_f unlock;	//!< Function for locking acces when operation on the heap. NULL if no locking is needed.
		void *self;	//!< Use this as a sanity-check on systems where there's no MMU
		char *last;	//!< Top Of heap - Last address + 1 element this heap uses (consecutive non-global heaps can be placed on this address)
	} heap_t;

/*!
@brief To identify the heap on which to operate on.

Please don't rely on that this is a pointer. Use only with intended
API's. The type might change to something else depending on
implementation of \ref KMEM
*/
	typedef heap_t *heapid_t;

/*!
@name Creation and destruction of this component

Use these functions only at boot, and shut-down.

@see COMPONENTS
*/

//@{
	unsigned long tk_mem(void);
	unsigned long tk_mem_destruct(void);
//@}

/*! @name Creation and destruction of individual heaps

Use these functions in other \ref COMPONENTS init and destruct
functions.

@see COMPONENTS
@see KMEM
*/

//@{
	unsigned long tk_create_heap(heapid_t *, int, int, int, lock_f,
				     unlock_f, char *);
	unsigned long tk_destroy_heap(heapid_t);
//@}

/*!
@name API for createing and destoy memory (elements)

These functions are involved with creating and destroying (elements)

@see KMEM

*/

//@{
	void *tk_mem_malloct(heapid_t, size_t);
	void tk_mem_free(heapid_t, void *);
//@}

#if defined(__cplusplus) && (TK_CPLUSPLUS == 0)
}
#endif
#endif				//TK_MEM_H
