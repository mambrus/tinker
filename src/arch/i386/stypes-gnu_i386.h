/*******************************************************************
 *
 *  DESCRIPTION: TinKerOS.
 *
 *  AUTHOR: Michael Ambrus
 *
 *  HISTORY:
 *
 *  Current $Revision: 1.1 $
 *
 *******************************************************************/


#ifndef TK_HWTYPES_GNU_i386_H
#define TK_HWTYPES_GNU_i386_H
//------1---------2---------3---------4---------5---------6---------7---------8

//------1---------2---------3---------4---------5---------6---------7---------8

#include <stddef.h>

/*!
Architecture specific representation of a stack adress. In x86 this can be a
simple char* even in 16bit modes, since a pointer contains both segment and
offset.
*/
typedef struct{
   char * tstack;
   size_t stack_size;
} stack_t;


//------1---------2---------3---------4---------5---------6---------7---------8

#endif



/*!
 * @defgroup CVSLOGtk_hwtypes_borlandX86_h tk_hwtypes_borlandX86_h
 * @ingroup CVSLOG
 *
 *  $Log: stypes-gnu_i386.h,v $
 *  Revision 1.1  2006-11-03 11:33:30  ambrmi09
 *  Moved architecture specific files out from bsp structure and into kernel
 *  source 'arch' structure. This makes sense since I plan to refine the
 *  project into kernel specific maintenence on one hand, and BSP (and
 *  eventually other's) on the other.
 *
 *  BSP's as part of the *kernlel* project will only be in the form of crude
 *  template BSP (community will take care of the rest).
 *
 *  Revision 1.2  2006/03/05 11:11:24  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.1  2006/03/04 19:31:02  ambrmi09
 *  Structure for new targets based on GNU toolchain added.
 *  - gnu-i386  (Linux, Cygwin)
 *  - gnu-arm   (LPC2129 i.e. ARM7 derivate)
 *
 *  Revision 1.1  2006/02/28 11:24:48  ambrmi09
 *  Initial checkin of missing files for Borland C and MSVC
 *
 *
 *******************************************************************/
