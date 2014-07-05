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

KMEM can be used both by TinKer internals and by drivers or even in
certain cases applications. It fullfills the following needs:

* Provides TinKer internal dynamic allocation where a working malloc/free is
not available.

* Makes dynamic memory handling deterministic in cases where that is
desireable.

* Provides a locking mechanism for heap accessing in cases this is needed.

* Makes is possible to point out specific memory areas for each heap, which
is usefull in cases where the C-library is delibreratly held unknowing of
certain memory, for example storage disks (battery backupes RAM area) e.t.a.

The basic idea of KMEM to have several heaps in memory, each of fixed size
elements. This makes managing each heap very fast but also determinisic
(both functionally and in temporal space).

The API for using each heap mimics the ANSI malloc and free, exept that each
function takes an identifier as first argument to identify which heap you're
accessing. I.e. the size argument for malloc must be a even multiple of
the particular heaps element size.

TinKer heaps can be located both on the global heap, but also in any memory
pointed out by the creation of each heap. The latter permits heap to be
allocated and managed that the C-library is not aware of but that the linker
script knows of (or it can in certain cases be the address of an global array).

Each heap has a heap header. This header is located in a fixed size header
pool. This is to accommodate systems where no functional malloc/free are
available and provides the TinKer scheduler component with a working solution
to create elements for internal usages without

For an further in-depth discussions about this component, see \ref
KMEM

@see KMEM
@see COMPONENTS

*/
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <tk_mem.h>

#if defined (__GNUC__)
   #include <tinker/config.h>
#endif

#ifndef TK_KMEM_NHEAPS
/*!
Number of heaps managed by KMEM
*/
#define TK_KMEM_NHEAPS 0x8
#endif //TK_KMEM_NHEAPS

heap_t header_pool[TK_KMEM_NHEAPS];	//!< The heap-header pool
int cheap_idx = 0; 			//!<Current heap-header allocation index
int nheaps = 0; 			//!<Number of heaps in use

//------1---------2---------3---------4---------5---------6---------7---------8
/*! @brief Constructor of TinKer's \ref COMPONENTS "component" - KMEM: \ref KMEM

Constructor of TinKer's \ref COMPONENTS "component" - KMEM: \ref KMEM

@note that we don't pass any size for the memory to handle. This is
because only various constructors are supposed to create new heaps.

@see COMPONENTS
@see KMEM
*/

unsigned long  tk_mem         ( void ){
	int i;
	for (i=0;i<TK_KMEM_NHEAPS;i++){
		memset(&header_pool[i],0,sizeof(heap_t));
	}
	return ERR_OK;
}

/*! @brief Destructor of TinKer's \ref COMPONENTS "component" - KMEM: \ref KMEM

Destructor of TinKer's \ref COMPONENTS "component" - KMEM: \ref KMEM

@see COMPONENTS
@see KMEM
*/
unsigned long  tk_mem_destruct( void ){
	int i;
	for (i=0;i<TK_KMEM_NHEAPS;i++){
		if (header_pool[i].heap && header_pool[i].is_malloced)
			free(header_pool[i].heap);
	}
   return ERR_OK;
}

//------1---------2---------3---------4---------5---------6---------7---------8
/*! @brief Creates a new heap

Creates a new heap, returns a heap handler (heapid).

@Note that the total memory usage will be larger than num*size.

@see KMEM
*/
unsigned long  tk_create_heap (
	heapid_t	*heapid,	//!< Returned heap ID
	int		hsize,		//!< Raw memory regions size allowed for this heap (in bytes)
	int		dsize,		//!< Size each element will have
	int 		opt,		//!< Options bit-mask
	lock_f		lock,		//!< Function for un-locking acces when operation on the heap. NULL if no locking is needed.
	unlock_f	unlock,		//!< Function for locking acces when operation on the heap. NULL if no locking is needed.
	char		*heap_ptr	//!< Memory address to use as heap, or NULL for global heap usage
){
	int 		i,found_ID;
	int 		esz;		//!< True element size
	void 		*ptr;
	int         	mxnum;		//!< Calculated maximum number of elements that will fit untruncated

	esz = sizeof(int) + dsize;
	mxnum = hsize / esz;
	*heapid = NULL;

	if (nheaps>=TK_KMEM_NHEAPS){
		*heapid=NULL;
		return ENOMEM;
	}

	for (
		i=0,found_ID=TK_KMEM_NHEAPS+1;
		i<TK_KMEM_NHEAPS && found_ID>TK_KMEM_NHEAPS;
		i++
	){
		if (header_pool[cheap_idx].heap==NULL){
			*heapid=&header_pool[cheap_idx];
			found_ID=cheap_idx;
		}
		cheap_idx++;
		cheap_idx%=TK_KMEM_NHEAPS;
	}
	assert(*heapid != NULL);

	/* Allocate the actual heap. Note that one extra sizeof(in) per element is also alocated */
	if (heap_ptr==NULL){
		(*heapid)->heap = malloc( hsize );
		(*heapid)->is_malloced=1;
	}else{
		(*heapid)->heap = heap_ptr;
		(*heapid)->is_malloced=0;
	}

	if ((*heapid)->heap == NULL){
		//Failiure to allocate memory for heap
		//Release the handler again
		memset(*heapid,0,sizeof(heap_t));

		*heapid = NULL;
		return ENOMEM;
	}

	/*Fill in initial stuff in header*/
	(*heapid)->lock   =  lock;
	(*heapid)->unlock =  unlock;

	(*heapid)->self		= *heapid;
	(*heapid)->dsize	=  dsize;
	(*heapid)->maxnum 	=  mxnum;
	(*heapid)->blocks	=  0;
	(*heapid)->indx		=  0;

	/*Blank the heap (or at least each first byte telling wheter each block is free or not)*/
	//*(int*)((char*)ptr + i*esz) = 0;

	ptr = (*heapid)->heap;

	if (opt & KMEM_KEEP_UNINITIALIZED){
		//Leave the blocks as is, but recalculated the heap-header control data
		for (i=0; i<mxnum; i++){
			if (*(int*)ptr != 0){
				(*heapid)->blocks++;
			}else{
				(*heapid)->indx = i;
			}
		}
		ptr = (char*)ptr + esz;
	}else{
		if (opt & KMEM_BLANK_ZERO){
			memset((*heapid)->heap,0,hsize);
		}else if (opt & KMEM_BLANK_ID){
			memset((*heapid)->heap,found_ID,hsize);
		}

		if (!(opt & KMEM_BLANK_ZERO)){	//Cos if zero, this is allready done (opimization)
			for (i=0; i<mxnum; i++){
				*(int*)ptr = 0;
				ptr = (char*)ptr + esz;
			}
		}
	}

	(*heapid)->last   =  ptr;
	nheaps++;

	return ERR_OK;
}

/*! @brief Destroys a heap

This function is typically invoced on system shutdown only.

@see KMEM

@note Lacks locking

@todo This function has not meen tested. Please test it.

*/
unsigned long  tk_destroy_heap(
   heapid_t  heapid
){
	if (heapid->self == NULL)
		return ERR_UNDEF_HEAPID;
	if (heapid->self != (void*)heapid)
		return ERR_UNDEF_HEAPID;
	if (heapid->blocks != 0)
		return EBUSY;

	if (heapid->is_malloced)
		free (heapid->heap);

	//Free the header too (clean it and mark free)
	memset(heapid,0,sizeof(heap_t));
	nheaps--;

	return ERR_OK;
}

//------1---------2---------3---------4---------5---------6---------7---------8

/*! @brief malloc (on specified heap)

XXX

@see KMEM
*/
void *tk_mem_malloct (
	heapid_t heapid,
	size_t size
){
	int i;
	int esz;
	void *ptr;

	if (heapid->self == NULL){
		//errno = ERR_UNDEF_HEAPID;
		return NULL;
	}
	if (heapid->self != (void*)heapid){
		//errno = ERR_UNDEF_HEAPID;
		return NULL;
	}
	if (heapid->blocks >= heapid->maxnum) {
		//errno = ENOMEM;
		return NULL;
	}

	if (heapid->lock)
		heapid->lock();

	/* Find next free block */
	esz = sizeof(int) + heapid->dsize;
	ptr = (char*)(heapid->heap) + esz*heapid->indx;

	for (i=heapid->indx; *(int*)ptr; ){
		i++;
		i%=heapid->maxnum;
		ptr = (char*)(heapid->heap) + esz*i;
	}
	i++;
	i%=heapid->maxnum;
	heapid->indx=i;
	heapid->blocks++;

	/* Mark it as occupied */
	*(int*)ptr = 1;

	ptr = (char*)ptr + sizeof(int);

	if (heapid->unlock)
		heapid->unlock();

	return ptr;
}

/*! @brief free (on specified heap)

XXX

@see KMEM
*/
void tk_mem_free(
   heapid_t heapid,
   void* ptr
){
	if (heapid->self == NULL){
	//errno = ERR_UNDEF_HEAPID;
		return;
	}
	if (heapid->self != (void*)heapid){
	//errno = ERR_UNDEF_HEAPID;
		return;
	}
	if (heapid->blocks >= heapid->maxnum){
	//errno = ENOMEM;
		return;
	}
	if (heapid->lock)
		heapid->lock();

	ptr = (char*)ptr - sizeof(int);
	heapid->blocks--;
	*(int*)ptr = 0;

	if (heapid->unlock)
		heapid->unlock();
}

//------1---------2---------3---------4---------5---------6---------7---------8
/** @defgroup KMEM KMEM: A better malloc
@ingroup COMPONENTS
@brief A better malloc

This component will provide both TinKer and any application with a form
of heap that doesn't have many of the drawback normally attached with
malloc and the reason this technique is avoided by many embedded
developers: I.e. unpredictable allocation success and unknown allocation
time.

Not to mention the most important one:
@note Using malloc in a standard library is almost never thread safe!

The price you have to pay for this is:

- Allocating a new heap before you can allocate on it
- Decide on a fixed size that will be allocatable for each element

What you gain is:

- Much faster allocation
- Predictable allocation both regarding
   - Memory
   - Time
   - A certain level of memory protection between heaps
   - Thread-safety (if lock/unlock functions are supplied upon creation)

That said, it's appenant that the component doesn't solve every imaginable
memory allocation need. But for those it does, it's a huge improvement

The API is very simple and intentionally made to mimic malloc and free
as much as possible. This way migrating between KMEM and old-fashioned
heap-usage back and forth should be quite pain-less. You could even make
it completely transparent by using macros to make the application code
totally transparent.



\ref memid_t

@see COMPONENTS

@dot
digraph mem_main {
   node [
      shape=record,
      style=filled,
      fillcolor=yellow,
      fontname=Courier,
      nojustify="true",
      fontsize=10.0
   ];

   edge [
      color="red",
      fontname=Courier,
      nojustify="true",
      fontsize=10.0
   ];

   graph [
      rankdir = "TB",
      fontname=Courier,
      nojustify="true",
      fontsize=10.0
   ];

   Elements_1 [ orientation=73.0, label="{\
      <a0> #0  ----| \
      <a1> #1  BUSY| \
      <a2> #2  BUSY| \
      <a3> #3  ----| \
      <a..> .. ....| \
      <aN> #N  ---- }"];

   Heap_1 [ orientation=73.0, label="{\
      num            | \
      size           | \
      blocks         | \
      indx           | \
      <heap> heap    | \
      <self> self     }"];

   Heap_1:self:w -> Heap_1 ;
   Heap_1:heap:w -> Elements_1 ;
   Heap_1:indx:w -> Elements_1:<a2>:e ;
}

@enddot

<H2>Thread-safety using KMEM</H2>

Normally a malloc is not thread-safe. You can acheve a certain level of
thread safety by passing along two functions \ref lock_f and \ref
unlock_f upon creation of each heap. Note however that this covers only
cases where a code entity tith several threads operating on the
same heap needs to "malloc" and "free" structures on the fly. <b>It does
not give</b> you a catch all mechanism to protect shared data - i.e. the
actual element it self (if categorized as a critical section) is not
protected agains concurrent access.

This is all well but...: We don't wan't to create a dependancy towards
any syncronisation component. And since vital parts of TinKer on a low
lever of TinKer that <em>don't have access to</em> for exanple \ref ITC,
we provide this as an generic option by the \ref lock_f and \ref
unlock_f functions. I.e. \ref ITC itself could use this component. Also
\ref SCHED fort that matter... In case you need syncronisatiuon
(protection agains concurrent access) in those cases, you'd pass along a
function with a much more promitive locking mechanism - clearing and
setting of interrupts for example.

In mamy cases however you don't need locking. The
functions \ref tk_mem_malloct and \ref tk_mem_free are them selfes
thread-safe, only their \ref heap_t that they operate on is not.
Different heaps are not vounerable between each other at all. So only in
cases that full-fill <b>both</b> the following two requirements, you
need the \ref lock_f and \ref unlock_f functions:

- More than one thread operates on the particular heap
- At least one of them are involved in distructing and creating new
elements.
   - And this happens concurrently to other threads.

In cases where this all of this is not satisfied then you dont need
locking and you simply pass NULL as arguments to \ref tk_create_heap for
both of these functions.

<p><b>Go gack to</b> \ref COMPONENTS</p>

*/


/*!
 * @defgroup CVSLOG_tk_mem_c tk_mem_c
 * @ingroup CVSLOG
 *  $Log: tk_mem.c,v $
 *  Revision 1.9  2007-04-23 16:26:07  ambrmi09
 *  Mounting is close
 *
 *  Revision 1.8  2007-04-23 09:39:22  ambrmi09
 *  KMEM refinement
 *
 *  Revision 1.7  2006-04-08 10:16:03  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.6.2.1  2006/04/03 20:07:30  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.6  2006/03/05 11:11:28  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.5  2006/02/22 13:05:47  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.4  2006/02/20 15:22:01  ambrmi09
 *  Documentation stuff. No code changes.
 *
 *  Revision 1.3  2006/02/17 20:26:56  ambrmi09
 *  Corrected some documentation structure so that Doxygen will build
 *  properly.
 *
 *  Revision 1.2  2006/02/16 23:42:59  ambrmi09
 *  First working version of \ref KMEM
 *
 *  Revision 1.1  2006/02/16 15:11:00  ambrmi09
 *  Introduced a new component for better and safer useage of the heap.
 *  Package is called \red KMEM and the files are tk_mem.c and tk_mem.h (so
 *  far).
 *
 *  Started to take care of the long needed issue with error codes and
 *  better error handling. Introduced errno.h to begin with, whitch is part
 *  of the package \ref kernel_reimpl_ansi. Its not a good solution yet,
 *  since both kernel and ANSI codes are in the same file we have to invent
 *  a way to omit the ANSI defines when a tool-chain that has errno.h is
 *  used.
 *
 *
 *******************************************************************/














