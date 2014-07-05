/*
@file
This file implements the \e stalloc_init_mem function and is part of the
package \ref STALLOC

@note The implementation in this file is a copy & paste from Keils
init_mem.c. Therefore the TinKer project claims no copy rights, nor should
should it be liable in \b any legal dispute. Please be aware that
mixing licenses might affect the whole application. USE AT OWN RISK!
*/

#include "stalloc.h"

/*!
 * Memory pool block structure and typedefs.
 * Memory is laid out as follows:
 *
 * {[NEXT | LEN] [BLOCK (LEN bytes)]} {[NEXT | LEN][BLOCK] } ...
 *
 * Note that the size of a node is:
 *          mp.len + sizeof (struct mp)
 */

struct __stmp__ {		//!< memory pool
	struct __stmp__ ST_MTYP *next;	//!< single-linked list
	stsize_t len;		//!< length of following block
};

void ST_MTYP *ST_MTYP __stmp__;	/* Memory Pool Head */

#define	HLEN  (sizeof (struct __stmp__))

/*!
 *  Memory pool headers.  __stmp__ points to the first available block or is
 *  NULL if there are no free blocks.
 *
 *  Note that the list is maintained in address order.  __stmp__ points to the
 *  block with the lowest address.  That block points to the block with the
 *  next higher address and so on.
 */

void stalloc_init_mempool(void ST_MTYP * pool,	//!< address of the memory pool
			  stsize_t size)	//!< size of the pool in bytes
{

	/*  Set the __stmp__ to point to the beginning of the pool and set
	 *  the pool size.                                                        */

	__stmp__ = pool;

	/*  Set the link of the block in the pool to NULL (since it's the only
	 *  block) and initialize the size of its data area.                      */

	((struct __stmp__ ST_MTYP *)pool)->next = ST_NULL;
	//((struct __stmp__ ST_MTYP *) pool)->next       = __stmp__;
	((struct __stmp__ ST_MTYP *)pool)->len = size - HLEN;
}
