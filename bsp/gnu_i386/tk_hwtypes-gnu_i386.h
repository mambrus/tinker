/*******************************************************************
 *
 *  DESCRIPTION: TinKerOS.
 *
 *  AUTHOR: Michael Ambrus
 *                              
 *  HISTORY:    
 *
 *  Current $Revision: 1.2 $
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
 *  $Log: tk_hwtypes-gnu_i386.h,v $
 *  Revision 1.2  2006-03-05 11:11:24  ambrmi09
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