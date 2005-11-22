/***********************************************************************/
/*  This file is part of the C166 Compiler package                     */
/*  Copyright KEIL ELEKTRONIK GmbH 1993 - 1999                         */
/***********************************************************************/
/*


 ==========================
| Modified  Michael Ambrus |
 ==========================



*/


/*                                                                     */
/*  INIT_MEM.C:                                                        */
/*                                                                     */
/*  To translate this file use C166 with the following invocation:     */
/*                                                                     */
/*     C166 INIT_MEM.C <memory model>                                  */
/*                                                                     */
/*  To link the modified INIT_MEM.OBJ file to your application use the */
/*  following L166 invocation:                                         */
/*                                                                     */
/*     L166 <your object file list>, INIT_MEM.OBJ <controls>           */
/*                                                                     */
/***********************************************************************/

#include <stdlib.h>

#if defined (XMEM)
  #message "Target __FILE__ was compiled with define XMEM"  
  #define MTYP          xhuge
  #define init_mempool  xinit_mempool
  #define calloc        xcalloc
  #define malloc        xmalloc
  #define free          xfree
  #define realloc       xrealloc
  #define __mp__        __xmp__
  #define TLEN          unsigned long
#else
  #define TLEN          unsigned int
  #if (__MODEL__ == 5 || __MODEL__ == 6)
    #define MTYP  huge   // for HCOMPACT and HLARGE
  #else
    #define MTYP  far    // for other models
  #endif
#endif

/*!
 * Memory pool block structure and typedefs.
 * Memory is laid out as follows:
 *
 * {[NEXT | LEN] [BLOCK (LEN bytes)]} {[NEXT | LEN][BLOCK] } ...
 *
 * Note that the size of a node is:
 *          mp.len + sizeof (struct mp)
 */

struct __mp__  {                   //!< memory pool 
  struct __mp__ MTYP *next;        //!< single-linked list 
  TLEN                 len;        //!< length of following block
};

void MTYP * MTYP __mp__;    /* Memory Pool Head */

#define	HLEN  (sizeof (struct __mp__))

/*! 
 *  Memory pool headers.  __mp__ points to the first available block or is
 *  NULL if there are no free blocks. 
 *
 *  Note that the list is maintained in address order.  __mp__ points to the
 *  block with the lowest address.  That block points to the block with the 
 *  next higher address and so on.
 */

void init_mempool (
  void MTYP *pool,           //!< address of the memory pool
  TLEN size)                 //!< size of the pool in bytes  
{              

   /*  Set the __mp__ to point to the beginning of the pool and set
    *  the pool size.                                                        */

   __mp__ = pool;

   /*  Set the link of the block in the pool to NULL (since it's the only
    *  block) and initialize the size of its data area.                      */

   ((struct __mp__ MTYP *) pool)->next       = NULL;
   //((struct __mp__ MTYP *) pool)->next       = __mp__;
   ((struct __mp__ MTYP *) pool)->len        = size - HLEN;
}
