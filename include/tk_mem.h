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

/** Error codes **/
//#define ERR_UNDEF_HEAPID      0x02 /*!< No such or invalid \link "heap" heap_t*/
//#define ERR_NOMEM             0x03 /*!< Can't allocate memory */


/*!
@brief Organisational structure of each heap

There should not be any reason for a normal user to address this
structure explicitly. Use \ref heapid_t for normal operations instead.
*/
typedef struct {
   int      csize;      //<! Maximum number of elements this pool can hold
   int      esize;      //<! Size of each element
   void *   theheap;    //<! Pointer to the allocatded memory, null if unallocated
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
unsigned long  tk_create_heap ( heapid_t* );
unsigned long  tk_destroy_heap( heapid_t  ); 
//@}

/*! 
@name API for createing and destoy timers 

These functions are involved with creating and destroying timers 

@see KMEM

*/ 

//@{
void *         tk_mem_malloct ( heapid_t, size_t); 
void           tk_mem_free    ( heapid_t, void* ); 
//@}

#endif //TK_MEM_H
//------1---------2---------3---------4---------5---------6---------7---------8

/** @defgroup KMEM A better malloc
@ingroup COMPONENTS
@brief A better mallox

XXX

\ref memid_t

@see COMPONENTS 

*/

/*! 
 * @addtogroup CVSLOG CVSLOG
 *  $Log: tk_mem.h,v $
 *  Revision 1.1  2006-02-16 15:11:00  ambrmi09
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


