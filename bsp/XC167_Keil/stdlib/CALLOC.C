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
/*  CALLOC.C:                                                          */
/*                                                                     */
/*  To translate this file use C166 with the following invocation:     */
/*                                                                     */
/*     C166 CALLOC.C  <memory model>                                   */
/*                                                                     */
/*  To link the modified CALLOC.OBJ file to your application use the   */
/*  following L166 invocation:                                         */
/*                                                                     */
/*     L166 <your object file list>, CALLOC.OBJ <controls>             */
/*                                                                     */
/***********************************************************************/

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
  #endif
#endif

void MTYP *calloc (TLEN size, TLEN len)  {
  void MTYP *p;

  size *= len;
  p = malloc (size);
#if (__MODEL__ == 5 || __MODEL__ == 6)
  if (p) memset (p, 0, size);
#else
  if (p) fmemset (p, 0, size);
#endif
  return (p);
}
