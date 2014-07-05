/*
@file
This file implements the \e stalloc_free function and is part of the
package \ref STALLOC

@note The implementation in this file is a copy & paste from Keils
free.c. Therefore the TinKer project claims no copy rights, nor should
should it be liable in \b any legal dispute. Please be aware that
mixing licenses might affect the whole application. USE AT OWN RISK!
*/

#include "stalloc.h"

struct __stmp__ {		/* memory pool */
	struct __stmp__ ST_MTYP *next;	/* single-linked list */
	stsize_t len;		/* length of following block */
};

extern void ST_MTYP *ST_MTYP __stmp__;	/* Memory Pool Header */

#define	HLEN	(sizeof(struct __stmp__))

/*  Memory pool headers.  __stmp__ points to the first available block or is
 *  NULL if there are no free blocks.
 *
 *  Note that the list is maintained in address order.  AVAIL points to the
 *  block with the lowest address.  That block points to the block with the
 *  next higher address and so on.                                          */

void stalloc_free(void ST_MTYP * memp)
{
/*  FREE attempts to organize Q, P0, and P so that Q < P0 < P.  Then, P0 is
 *  inserted into the free list so that the list is maintained in address
 *  order.
 *
 *  FREE also attempts to consolidate small blocks into the largest block
 *  possible.  So, after allocating all memory and freeing all memory,
 *  you will have a single block that is the size of the memory pool.  The
 *  overhead for the merge is very minimal.                                */

	struct __stmp__ ST_MTYP *q;	/* ptr to free block */
	struct __stmp__ ST_MTYP *p;	/* q->next */
#define p0 ((struct __stmp__ ST_MTYP *) memp)	/* block to free */

/*  If the user tried to free NULL, get out now.  Otherwise, get the address
 *  of the header of the memp block (P0).  Then, try to locate Q and P such
 *  that Q < P0 < P.                                                       */

	if (!memp)
		return;

	memp = &p0[-1];		/* get address of header */

/*  Initialize.  Q = Location of first available block.                    */
	q = __stmp__;
	if (!q) {
		__stmp__ = p0;
		return;
	}

/*  B2. Advance P. Hop through the list until we find a free block that is
 *  located in memory AFTER the block we're trying to free.                */
	while (1) {
		p = q->next;
		if (!p || (p > memp))
			break;
		q = p;
	}

/*  B3. Check upper bound. If P0 and P are contiguous, merge block P into
 *  block P0.                                                              */

	if (p && ((((char ST_MTYP *)p0) + p0->len + HLEN) == (char ST_MTYP *)p)) {
		p0->len += p->len + HLEN;
		p0->next = p->next;
	} else {
		p0->next = p;
	}

/*  B4. Check lower bound. If Q and P0 are contiguous, merge P0 into Q.  */

	if ((((char ST_MTYP *)q) + q->len + HLEN) == (char ST_MTYP *)p0) {
		q->len += p0->len + HLEN;
		q->next = p0->next;
	} else {
		q->next = p0;
	}
}
