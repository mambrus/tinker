/***********************************************************************/
/*  This file is part of the C166 Compiler package                     */
/*  Copyright KEIL ELEKTRONIK GmbH 1992-2003                           */
/***********************************************************************/
/*                                                                     */
/*  PUTCHAR.C:  This routine is the general character output of C166.  */
/*                                                                     */
/*  To translate this file use C166 with the following invocation:     */
/*                                                                     */
/*     C166 PUTCHAR.C                                                  */
/*                                                                     */
/*  To link the modified PUTCHAR.OBJ file to your application use the  */
/*  following L166 invocation:                                         */
/*                                                                     */
/*     L166 <your object file list>, PUTCHAR.OBJ <controls>            */
/*                                                                     */
/***********************************************************************/

#include <reg166.h>

#define XON  0x11
#define XOFF 0x13

signed char putchar (signed char c)  {

  if (c == '\n')  {
    if (S0RIR)  {
      if (S0RBUF == XOFF)  {
        do  {
          S0RIR = 0;
          while (!S0RIR);
        }
        while (S0RBUF != XON);
        S0RIR = 0; 
      }
    }
    while (!S0TIR);
    S0TIR = 0;
    S0TBUF = 0x0d;                         /* output CR  */
  }
  if (S0RIR)  {
    if (S0RBUF == XOFF)  {
      do  {
        S0RIR = 0;
        while (!S0RIR);
      }
      while (S0RBUF != XON);
      S0RIR = 0; 
    }
  }
  while (!S0TIR);
  S0TIR = 0;
  return (S0TBUF = c);
}
