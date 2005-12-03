/*******************************************************************
 *
 *  DESCRIPTION: TinKerOS.
 *
 *  AUTHOR: Michael Ambrus
 *                              
 *  HISTORY:    
 *
 *  Current $Revision: 1.3 $
 *
 *******************************************************************/
  

#ifndef TK_HWTYPES_KEILC166_H
#define TK_HWTYPES_KEILC166_H      
//------1---------2---------3---------4---------5---------6---------7---------8

//------1---------2---------3---------4---------5---------6---------7---------8


#include <stddef.h>  //Needed for size_t

#pragma pack(1)     /* byte alignment */

//BIG NOTE!! The following #pragma will crash in dave file who use pointercasts for SFR data
//======================================================================
//Output will not work (in best case), in worst case C166 crashes
//Leave it here as a reference so that I remeber. Also, will tinket work?
//Structures might change, does it matter?
//#pragma bytealign   /* ptrs to byte-aligned objects */
 
/*!

@brief TBD

@note that the offset addres does not contain the 2 MSB bits that defines wich
DDP to use. It's your respponsibility to trunc these off before storing in the
off variable, and to add them back when you assign your user-stack pointer
(typically R0)

@note Orientation of bitfiels are compilor specific. Use commandline or pragma 
to ensure correct layout.

@note Keil C166 seem to lack directives for byte order within a struct.
This struct is empirically defined. Be observant about changes of
behaviour when changing compiler and/or version.

*/
typedef union{ 
   unsigned long linear; 
   union {
      struct {
         unsigned char padding8;     // These two togeather must be 
         unsigned int  padding6  :6; // ... exacylly 14 bits long.
         unsigned int _seg       :10;                     
      }seg24;
      struct {
         unsigned int _offs      :14; 
         unsigned int padding2   :2;
         unsigned char padding8;
      }offs24;  
   }u;
}userstackaddr_t;

#pragma pack()    /* reset to default alignment */



/*!

@brief TBD

@note Orientation of bitfiels are compilor specific. Use commandline or pragma 
to ensure correct layout.

@note Keil C166 seem to lack directives for byte order within a struct.
This struct is empirically defined. Be observant about changes of
behaviour when changing compiler and/or version.

*/

typedef union{ 
   unsigned long linear; 
   struct { 
      unsigned int _offs; 
      unsigned int _seg;         
   }segmented; 
   struct { 
      unsigned int _SP; 
      unsigned int _SPSEG;          
   }reg; 
}systemstackaddr_t;

/*! 
Architecture specific representation of a stack adress. In this obscure
MPU/Compiler combo, this need to be devided in two stacks for each
thread, that each is best represented in a different way. Both these
ways however also have a linear adrees for conveniant lookup in
physical memory.
*/

typedef struct { 
   systemstackaddr_t systemstack;      
   userstackaddr_t   userstack;   
   
   size_t            sys_stack_size;    //These two added togeather constitutes 
   size_t            usr_stack_size;    //the actual memory allocated
}stack_t;


//------1---------2---------3---------4---------5---------6---------7---------8

#endif


  
/*!
 * @addtogroup CVSLOG CVSLOG
 *
 *  $Log: tk_hwtypes_keilC166.h,v $
 *  Revision 1.3  2005-12-03 14:04:30  ambrmi09
 *  A crude documentation structure added. Sorce files modified a little, but
 *  only in comments (for Doxygens sake).
 *
 *
 *******************************************************************/