/*!
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
/*! @brief Constructor of \link component COMPONENTS KMEM

Constructor of \link component COMPONENTS KMEM

@note that we don't pass any size for the memory to handle. This is
because only various constructors are supposed to create new heaps, and
this is done at boot time anyway.

@see COMPONENTS 
@see KMEM
*/

unsigned long  tk_mem         ( void ){
   return ERR_OK; 
}

/*! @brief Destructor of \link component COMPONENTS KMEM

Destructor of \link component COMPONENTS KMEM

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
unsigned long  tk_create_heap ( heapid_t *heapid ){}

/*! @brief Destroys a heap

XXX

@see KMEM
*/
unsigned long  tk_destroy_heap( heapid_t  heapid){}


//------1---------2---------3---------4---------5---------6---------7---------8

/*! @brief malloc (on specified heap)

XXX

@see KMEM
*/
void *         tk_mem_malloct ( heapid_t heapid, size_t size ){}

/*! @brief free (on specified heap)

XXX

@see KMEM
*/
void           tk_mem_free    ( heapid_t heapid, void* ptr){}

  
/*! 
 * @addtogroup CVSLOG CVSLOG
 *  $Log: tk_mem.c,v $
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
 













