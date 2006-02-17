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

//------1---------2---------3---------4---------5---------6---------7---------8
/** @defgroup KMEM A better malloc
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


*/

#endif //TK_MEM_H

  
/*! 
 * @addtogroup CVSLOG CVSLOG
 *  $Log: tk_mem.h,v $
 *  Revision 1.3  2006-02-17 20:26:56  ambrmi09
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


