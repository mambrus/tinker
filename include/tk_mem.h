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

#include <stddef.h>  //Needed for size_t

/*- Error codes **/
//#define ERR_UNDEF_HEAPID      0x02 /*!< No such or invalid \ref heap_t "heap" */
//#define ERR_NOMEM             0x03 /*!< Can't allocate memory */


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
typedef l_func * lock_f; 
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
typedef u_func * unlock_f;
//@}

/*!

@brief Organisational structure of each heap

There should not be any reason for a normal user to address this
structure explicitly. Use \ref heapid_t for normal operations instead.
*/
typedef struct {   
   int      num;        //!< Maximum number of elements this pool can hold
   int      size;       //!< Size of each element
   int      blocks;     //!< Current number of blocks in use
   int      indx;       //!< Points at block firts after the last allocated block (probable for next alloc).
   void *   heap;       //!< Pointer to the allocatded memory, null if unallocated
   lock_f   lock;       //!< Function for un-locking acces when operation on the heap. NULL if no locking is needed.
   unlock_f unlock;     //!< Function for locking acces when operation on the heap. NULL if no locking is needed.
   void *   self;       //!< Use this as a sanity-check on systems where there's no MMU
}heap_t;


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
unsigned long  tk_mem         ( void ); 
unsigned long  tk_mem_destruct( void ); 
//@}

/*! @name Creation and destruction of individual heaps

Use these functions in other \ref COMPONENTS init and destruct
functions.

@see COMPONENTS 
@see KMEM 
*/ 

//@{ 
unsigned long  tk_create_heap ( heapid_t*, int, int, lock_f, unlock_f);
unsigned long  tk_destroy_heap( heapid_t  ); 
//@}

/*! 
@name API for createing and destoy memory (elements)

These functions are involved with creating and destroying (elements)

@see KMEM

*/ 

//@{
void *         tk_mem_malloct ( heapid_t, size_t); 
void           tk_mem_free    ( heapid_t, void* ); 
//@}


#endif //TK_MEM_H

  
/*! 
 * @defgroup CVSLOG_tk_mem_h tk_mem_h
 * @ingroup CVSLOG
 *  $Log: tk_mem.h,v $
 *  Revision 1.7  2006-02-22 13:05:46  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  Revision 1.6  2006/02/20 15:22:00  ambrmi09
 *  Documentation stuff. No code changes.
 *
 *  Revision 1.5  2006/02/19 12:44:33  ambrmi09
 *  - Documented ITC
 *  - Started to build up the structure for the \ref PTHREAD component
 *
 *  Revision 1.4  2006/02/17 21:17:26  ambrmi09
 *  More doc structure - this time for the new PTHREAD's components
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


