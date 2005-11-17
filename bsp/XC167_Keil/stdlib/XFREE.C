/***********************************************************************/
/*  Special Version of 'free' for xhuge accesses                       */
/*  Copyright KEIL ELEKTRONIK GmbH 1995 - 1999                         */
/***********************************************************************/
/*                                                                     */
/*  XFREE.C:                                                           */
/*                                                                     */
/*  To translate this file use C166 with the following invocation:     */
/*                                                                     */
/*     C166 XFREE.C  <memory model>                                    */
/*                                                                     */
/*  To link the modified XFREE.OBJ file to your application use the    */
/*  following L166 invocation:                                         */
/*                                                                     */
/*     L166 <your object file list>, XFREE.OBJ <controls>              */
/*                                                                     */
/***********************************************************************/

#pragma WARNING disable = 113

#define XMEM             // the xhuge version

#include <stdlib.h>

#if defined (XMEM)  
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

struct __mp__  {                      /* memory pool */
  struct __mp__ MTYP    *next;        /* single-linked list */
  TLEN                    len;        /* length of following block */
};

extern void MTYP * MTYP __mp__;       /* Memory Pool Header */

#define	HLEN	(sizeof(struct __mp__))

/*  Memory pool headers.  __mp__ points to the first available block or is
 *  NULL if there are no free blocks.
 *
 *  Note that the list is maintained in address order.  AVAIL points to the
 *  block with the lowest address.  That block points to the block with the
 *  next higher address and so on.                                          */


void free (void MTYP *memp)  {
/*  FREE attempts to organize Q, P0, and P so that Q < P0 < P.  Then, P0 is
 *  inserted into the free list so that the list is maintained in address
 *  order.
 *
 *  FREE also attempts to consolidate small blocks into the largest block
 *  possible.  So, after allocating all memory and freeing all memory,
 *  you will have a single block that is the size of the memory pool.  The
 *  overhead for the merge is very minimal.                                */

  struct __mp__ MTYP *q;                  /* ptr to free block */
  struct __mp__ MTYP *p;                  /* q->next */
#define p0 ((struct __mp__ MTYP *) memp)  /* block to free */

/*  If the user tried to free NULL, get out now.  Otherwise, get the address
 *  of the header of the memp block (P0).  Then, try to locate Q and P such
 *  that Q < P0 < P.                                                       */

  if (!memp)  return;

  memp = &p0 [-1];              /* get address of header */

/*  Initialize.  Q = Location of first available block.                    */
  q = __mp__;
  if (!q)  {
    __mp__ = p0;
    return;
  }

/*  B2. Advance P. Hop through the list until we find a free block that is
 *  located in memory AFTER the block we're trying to free.                */
  while (1)  {
    p = q->next;
    if (!p || (p > memp)) break;
    q = p;
  }

/*  B3. Check upper bound. If P0 and P are contiguous, merge block P into
 *  block P0.                                                              */

  if (p && ((((char MTYP *)p0) + p0->len + HLEN) == (char MTYP *) p))  {
    p0->len += p->len + HLEN;
    p0->next = p->next;
  }
  else  {
    p0->next = p;
  }


/*  B4. Check lower bound. If Q and P0 are contiguous, merge P0 into Q.  */

  if ((((char MTYP *)q) + q->len + HLEN) == (char MTYP *) p0)  {
    q->len += p0->len + HLEN;
    q->next = p0->next;
  }
  else  {
    q->next = p0;
  }
}

