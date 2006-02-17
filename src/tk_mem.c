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


  
/*! 
 * @addtogroup CVSLOG CVSLOG
 *  $Log: tk_mem.c,v $
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
 













