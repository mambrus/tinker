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
#include <stdlib.h>
#include <errno.h>
#include <tk_mem.h>


//------1---------2---------3---------4---------5---------6---------7---------8
/*! @brief Constructor of TinKer's \ref COMPONENTS "component" - KMEM: \ref KMEM

Constructor of TinKer's \ref COMPONENTS "component" - KMEM: \ref KMEM 

@note that we don't pass any size for the memory to handle. This is
because only various constructors are supposed to create new heaps, and
this is done at boot time anyway.

@see COMPONENTS 
@see KMEM
*/

unsigned long  tk_mem         ( void ){
   return ERR_OK; 
}

/*! @brief Destructor of TinKer's \ref COMPONENTS "component" - KMEM: \ref KMEM

Destructor of TinKer's \ref COMPONENTS "component" - KMEM: \ref KMEM

@see COMPONENTS 
@see KMEM
*/
unsigned long  tk_mem_destruct( void ){
   return ERR_OK; 
}

//------1---------2---------3---------4---------5---------6---------7---------8
/*! @brief Creates a new heap

XXX

@see KMEM
*/
unsigned long  tk_create_heap ( 
   heapid_t*   heapid,  //!< Returned heap ID
   int         size,    //!< Size each element will have
   int         num,     //!< Requested maximum number of elements
   lock_f      lock,    //!< Function for un-locking acces when operation on the heap. NULL if no locking is needed.
   unlock_f    unlock   //!< Function for locking acces when operation on the heap. NULL if no locking is needed.
){
   int i;
   int esz;
   void *ptr;
   
   esz = sizeof(int) + size;
   
   /* Allocate a heap "header" to act as a supervisory entity */
   *heapid = malloc(sizeof(heap_t));
   if (*heapid == NULL)
      return ENOMEM;
      
   /* Allocate the actual heap. Note that one extra byte per element is alocated also*/
   (*heapid)->heap = malloc( num*size );

   if ((*heapid)->heap == NULL){
      free (*heapid);
      *heapid = NULL;
      return ENOMEM;
   }
   
   /*Fill in initial stuff in header*/
   (*heapid)->lock   =  lock;
   (*heapid)->unlock =  unlock;
   
   (*heapid)->self   = *heapid;
   (*heapid)->size   =  size;
   (*heapid)->num    =  num;  
   (*heapid)->blocks =  0;     
   (*heapid)->indx   =  0;
   
   
   /*Blank the heap (or at least each first byte telling wheter each block is free or not)*/
   //*(int*)((char*)ptr + i*esz) = 0;

   ptr = (*heapid)->heap;
   for (i=0; i<num; i++){      
      *(int*)ptr = 0;
	  ptr = (char*)ptr + esz;
   }
      
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
      
   free (heapid->heap);
   free (heapid);
      
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
   if (heapid->blocks >= heapid->num) {
      //errno = ENOMEM;
      return NULL;
   }
   
   if (heapid->lock)
      heapid->lock();
   
   /* Find next free block */
   esz = sizeof(int) + heapid->size;
   ptr = (char*)(heapid->heap) + esz*heapid->indx;

   for (i=heapid->indx; *(int*)ptr; ){
	   i++;
      i%=heapid->num;
      ptr = (char*)(heapid->heap) + esz*i;
   }
   i++;
   i%=heapid->num;
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
   if (heapid->blocks >= heapid->num){
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
 *  Revision 1.5  2006-02-22 13:05:47  ambrmi09
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
 













