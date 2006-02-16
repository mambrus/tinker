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
   int      num;        //<! Maximum number of elements this pool can hold
   int      size;       //<! Size of each element
   int      blocks;     //<! Current number of blocks in use
   int      indx;       //<! Points at block firts after the last allocated block (probable for next alloc).
   void *   heap;       //<! Pointer to the allocatded memory, null if unallocated
   void *   self;       //<! Use this as a sanity check on systems where there's no MMU
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
unsigned long  tk_create_heap ( heapid_t*, int, int);
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



//------1---------2---------3---------4---------5---------6---------7---------8

/** @defgroup KMEM A better malloc
@ingroup COMPONENTS
@brief A better malloc

This component will provide both TinKer and any application with a form
of heap that doesn't have many of the drawback normally attached with
malloc and the reason this technique is avoided by many embedded
developers: I.e. unpredictable allocation success and unknown allocation
time.

The price you have to pay for this is:

- Allocating a new heap before you can allocate on it 
- Decide on a fixed size that will be allocatable for each element

What you gain is:

- Much faster allocation
- Predictable allocation both regarding   
   - Memory   
   - Time   
   - A certain level of memory protection between heaps
   
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

*/

#endif //TK_MEM_H

/*! 
 * @addtogroup CVSLOG CVSLOG
 *  $Log: tk_mem.h,v $
 *  Revision 1.2  2006-02-16 23:42:59  ambrmi09
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


