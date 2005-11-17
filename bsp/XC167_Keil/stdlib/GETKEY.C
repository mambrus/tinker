/***********************************************************************/
/*  This file is part of the C166 Compiler package                     */
/*  Copyright KEIL ELEKTRONIK GmbH 1992-2003                           */
/***********************************************************************/
/*                                                                     */
/*  GETKEY.C:  This routine is the general character input of C166.    */
/*                                                                     */
/*  To translate this file use C166 with the following invocation:     */
/*                                                                     */
/*     C166 GETKEY.C                                                   */
/*                                                                     */
/*  To link the modified GETKEY.OBJ file to your application use the   */
/*  following L166 invocation:                                         */
/*                                                                     */
/*     L166 <your object file list>, GETKEY.OBJ <controls>             */
/*                                                                     */
/***********************************************************************/

#include <reg166.h>


signed char _getkey (void)  {
  char c;

  while (!S0RIR);
  c = (char) S0RBUF;
  S0RIR = 0;
  return (c);
}
