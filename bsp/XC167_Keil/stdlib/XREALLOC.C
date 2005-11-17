/***********************************************************************/
/*  Special Version of 'realloc' for xhuge accesses                    */
/*  Copyright KEIL ELEKTRONIK GmbH 1995 - 1998                         */
/***********************************************************************/
/*                                                                     */
/*  XREALLOC.C:                                                        */
/*                                                                     */
/*  To translate this file use C166 with the following invocation:     */
/*                                                                     */
/*     C166 XREALLOC.C  <memory model>                                 */
/*                                                                     */
/*  To link the modified XREALLOC.OBJ file to your application use the */
/*  following L166 invocation:                                         */
/*                                                                     */
/*     L166 <your object file list>, XREALLOC.OBJ <controls>           */
/*                                                                     */
/***********************************************************************/

#define XMEM             // the xhuge version

#include <string.h>
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
    #define memcpy      fmemcpy 
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

void MTYP *realloc (void MTYP *oldp, TLEN size)  {
#define p   ((struct __mp__ MTYP *) oldp)   
#define p0  ((struct __mp__ MTYP *)&p[-1]) /* block to realloc */
  void MTYP *newp;

/*  Make sure that block is word aligned                                    */
  size = (size + 1) & ~1L;
  newp = malloc (size);
  if (!newp) return (NULL);
  if (oldp)  {
    if (size > p0->len)  size = p0->len;
    xmemcpy (newp, oldp, size);
    free (oldp);
  }
  return (newp);
}

