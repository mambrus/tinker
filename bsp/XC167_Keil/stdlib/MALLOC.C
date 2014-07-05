/***********************************************************************/
/*  This file is part of the C166 Compiler package                     */
/*  Copyright KEIL ELEKTRONIK GmbH 1993 - 1998                         */
/***********************************************************************/
/*


 ==========================
| Modified  Michael Ambrus |
 ==========================



*/


/*                                                                     */
/*  MALLOC.C:                                                          */
/*                                                                     */
/*  To translate this file use C166 with the following invocation:     */
/*                                                                     */
/*     C166 MALLOC.C  <memory model>                                   */
/*                                                                     */
/*  To link the modified MALLOC.OBJ file to your application use the   */
/*  following L166 invocation:                                         */
/*                                                                     */
/*     L166 <your object file list>, MALLOC.OBJ <controls>             */
/*                                                                     */
/***********************************************************************/

#include <stdlib.h>
#pragma browse


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

/*  Memory pool header:  __mp__ points to the first available.
 *
 *  Note that the list is maintained in address order.  __mp__ points to the
 *  block with the lowest address.  That block points to the block with the
 *  next higher address and so on.                                          */

#define MIN_BLOCK	(HLEN * 4)

void MTYP *malloc (TLEN size)   {
  struct __mp__ MTYP *q;  /* ptr to free block */
  struct __mp__ MTYP *p;  /* q->next */
  TLEN k;                 /* space remaining in the allocated block */

/*  Make sure that block is word aligned                                    */
  size = (size + 1) & ~1L;

/*  Initialization:  Q is the pointer to the next available block.          */
  q = (struct __mp__ MTYP *) &__mp__;

/*  End-Of-List:  P points to the next block.  If that block DNE (P==NULL),
 *  we are at the end of the list.                                          */

  while (1)  {
    p = q->next;
    if (!p)  return (NULL); /* FAILURE */

/*  Found Space:  If block is large enough, reserve if.  Otherwise, copy P
 *  to Q and try the next free block.                                       */
    if (p->len >= size) break;
    q = p;
  }

/*  Reserve P:  Use at least part of the P block to satisfy the allocation
 *  request.  At this time, the following pointers are setup:
 *  P points to the block from which we allocate Q->next points to P        */

  k = p->len - size;		/* calc. remaining bytes in block */

  if (k < MIN_BLOCK)  {		/* rem. bytes too small for new block */
    q->next = p->next;
    return (&p[1]);		    /* SUCCESS */
  }

/*  Split P Block:  If P is larger than we need, we split P into two blocks:
 *  the leftover space and the allocated space.  That means, we need to
 *  create a header in the allocated space.                                 */

  k -= HLEN;
  p->len = k;

  q = (struct __mp__ MTYP *) (((char MTYP *) (&p[1])) + k);
  q->len = size;
  return (&q[1]);           /* SUCCESS */
}
