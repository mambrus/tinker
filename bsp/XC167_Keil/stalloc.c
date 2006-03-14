/*
@file
This file implements the \e stalloc function and is part of the
package \ref STALLOC

@note The implementation in this file is a copy & paste from Keils
malloc.c. Therefore the TinKer project claims no copy rights, nor should
should it be liable in \b any legal dispute. Please be aware that
mixing licenses might affect the whole application. USE AT OWN RISK!
*/

#include "stalloc.h"
#pragma browse


struct __stmp__  {                      /* memory pool */
  struct __stmp__ ST_MTYP    *next;        /* single-linked list */
  stsize_t                    len;        /* length of following block */
};

extern void ST_MTYP * ST_MTYP __stmp__;       /* Memory Pool Header */
#define	HLEN	(sizeof(struct __stmp__))

/*  Memory pool header:  __stmp__ points to the first available.
 *
 *  Note that the list is maintained in address order.  __stmp__ points to the
 *  block with the lowest address.  That block points to the block with the
 *  next higher address and so on.                                          */

#define MIN_BLOCK	(HLEN * 4)

void ST_MTYP *stalloc (stsize_t size)   {
  struct __stmp__ ST_MTYP *q;  /* ptr to free block */
  struct __stmp__ ST_MTYP *p;  /* q->next */
  stsize_t k;                 /* space remaining in the allocated block */

/*  Make sure that block is word aligned                                    */
  size = (size + 1) & ~1L;

/*  Initialization:  Q is the pointer to the next available block.          */
  q = (struct __stmp__ ST_MTYP *) &__stmp__;

/*  End-Of-List:  P points to the next block.  If that block DNE (P==NULL),
 *  we are at the end of the list.                                          */

  while (1)  {
    p = q->next;
    if (!p)  return (ST_NULL); /* FAILURE */

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

  q = (struct __stmp__ ST_MTYP *) (((char ST_MTYP *) (&p[1])) + k);
  q->len = size;
  return (&q[1]);           /* SUCCESS */
}
